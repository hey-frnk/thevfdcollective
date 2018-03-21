Imports System.Windows.Forms
Imports System.IO
Imports System

Public Class fwUpdater

    Dim fwNewString As String = "#"

    Private Sub OK_Button_Click(ByVal sender As System.Object, ByVal e As System.EventArgs)
        Me.DialogResult = System.Windows.Forms.DialogResult.OK
        Me.Close()
    End Sub

    Private Sub Cancel_Button_Click(ByVal sender As System.Object, ByVal e As System.EventArgs)
        Me.DialogResult = System.Windows.Forms.DialogResult.Cancel
        Me.Close()
    End Sub

    Private Sub b_Step1_Click(sender As Object, e As EventArgs) Handles b_Step1.Click
        'Fill information byte array
        Dim command = New Byte(24) {}
        'Start byte, stop byte
        command(0) = 35
        command(23) = 36

        'Mode define
        command(1) = 34

        'Set the rest zero
        For i As Integer = 2 To 22
            command(i) = 0
        Next

        Try
            OpenVFD.comPort.Write(command, 0, 24)


            System.Threading.Thread.Sleep(200)

            Dim receiveMsg As Byte()
            receiveMsg = OpenVFD.ReceiveSerialData()

            'Write Message Completion
            If receiveMsg(0) = 35 And receiveMsg(1) = 34 And receiveMsg(9) = 36 Then
                Dim j = New Byte(7) {}
                For i As Integer = 2 To 8
                    j(i - 2) = receiveMsg(i)
                Next

                p_Step2.Visible = True
                p_Step2.BringToFront()
                p_step1.Visible = False

                Dim fwstring As String = System.Text.Encoding.ASCII.GetString(j).TrimEnd(vbNullChar)
                l_FWString.Text = fwstring
            End If
        Catch ex As Exception
            MsgBox(ex.Message, MsgBoxStyle.Exclamation, "Oops... Something went wrong!")
            Exit Sub
        End Try
    End Sub

    Private Sub fwUpdater_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        p_step1.Visible = True
        p_step1.BringToFront()
    End Sub

    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles b_Browse.Click
        Dim updateStr As String = ""

        Using fwOpenFile As New OpenFileDialog
            With fwOpenFile
                .Filter = "ZIP-Files (*.zip)|*.zip"
                .Title = "Select a firmware update ZIP file"
                If .ShowDialog = Windows.Forms.DialogResult.OK Then
                    updateStr = fwOpenFile.FileName
                    t_Path.Text = updateStr
                    Try
                        Try
                            IO.Directory.CreateDirectory(Path.Combine(Application.StartupPath, "updateTMP"))
                        Catch ex As Exception
                            MsgBox(ex.Message, MsgBoxStyle.Critical, "Error")
                        End Try
                        Dim file As String = updateStr
                        Dim cu As New ClassUnzip(file, Path.Combine(Application.StartupPath, "updateTMP"))
                        AddHandler cu.UnzipFinishd, AddressOf Unziped
                        cu.UnzipNow()

                        Try
                            fwNewString = My.Computer.FileSystem.ReadAllText(Application.StartupPath & "\updateTMP\version")
                            l_NewVersion.Text = fwNewString

                            If My.Computer.FileSystem.FileExists(Application.StartupPath & "\updateTMP\" & fwNewString & ".hex") = True Then
                                p_step2_2.Visible = True
                                p_step2_2.BringToFront()

                                b_Upload.Enabled = True
                            End If
                        Catch ex As Exception

                        End Try
                    Catch ex As Exception
                        MsgBox(ex.Message, MsgBoxStyle.Critical, "Error")
                    End Try
                Else
                    Exit Sub
                End If
            End With
        End Using
    End Sub

    Private Sub Unziped()
    End Sub

    Private Sub b_Upload_Click(sender As Object, e As EventArgs) Handles b_Upload.Click
        If MsgBox("You sure? There's no undo!" & vbCrLf & vbCrLf & "DO NOT DISCONNECT OpenVFD FROM COMPUTER OR POWER SOURCE, OTHERWISE THIS WILL BRICK THE DEVICE!" & vbCrLf & vbCrLf & "Keep AVRDUDE (flashing software) opened during upload, do not use the new instance of OpenVFD controller until flashing is complete!", MsgBoxStyle.YesNo, "Firmware Update - Warning") = MsgBoxResult.Yes Then
            Try

                Dim psi As New ProcessStartInfo

                psi.WorkingDirectory = Application.StartupPath & "\updateTMP"
                psi.Arguments = " -CC:avrdude.conf -v -p atmega328p -c arduino -P " & OpenVFD.txt_SerialPorts.Text & " -b115200 -D -Uflash:w:" & fwNewString & ".hex:i"
                psi.FileName = "avrdude"
                psi.WindowStyle = ProcessWindowStyle.Maximized

                Process.Start(psi)

                Application.Restart()
            Catch ex As Exception
                MsgBox(ex.Message, MsgBoxStyle.Critical, "Error")
            End Try
        End If
    End Sub
End Class
