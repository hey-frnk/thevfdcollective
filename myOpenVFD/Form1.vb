Imports System.IO.Ports
Imports Microsoft.VisualStudio.Modeling.Diagrams
Imports Microsoft.VisualStudio.Modeling.Diagrams.HslColor
Imports Microsoft.VisualStudio.Modeling.Diagrams.HslColorConverter

Public Class OpenVFD

    'Global variables
    Dim cLED0 As Color = Color.White
    Dim cLED1 As Color = Color.White
    Dim cLED2 As Color = Color.White
    Dim cLED3 As Color = Color.White
    Dim cLED4 As Color = Color.White
    Dim cLED5 As Color = Color.White

    Dim enabledFade As Byte = 0

    Dim hslWheel As New HslColor(0, 192, 155)
    Dim hslColorArray(6) As HslColor
    Dim ResistorArray(10) As Color
    Dim moodLight(6) As Color

    Dim timebar(25) As PictureBox
    Dim ledbar(6) As PictureBox
    Dim moodbar(6) As PictureBox

    Dim copFlipState As Boolean = False

    Dim randomint As Integer = 2
    Dim globalTimeFactor As Integer = 0
    Dim fullWrite As Boolean = False

    Dim pCounter As Integer = 0


    'Function section

    'Function handling incoming serial data
    Function ReceiveSerialData() As Byte()
        Dim Incoming = New Byte(10) {}

        Try
            For i As Integer = 0 To 9
                Incoming(i) = comPort.ReadByte
            Next

            If Incoming Is Nothing Then
                MsgBox("Error")
                Return {0}
            Else
                Return Incoming
            End If
        Catch ex As TimeoutException
            MsgBox("Error")
            Return {0}
        End Try

    End Function

    'Turn bytes into string
    Private Function ASCIIBytesToString(ByVal bytes() As Byte) As String
        Return System.Text.Encoding.ASCII.GetString(bytes)
    End Function

    'Screenshot and downsize
    Private Function Screenshot() As Bitmap
        'Create new empty bitmap as large as screen
        Dim b As Bitmap = New Bitmap(Screen.PrimaryScreen.Bounds.Width, Screen.PrimaryScreen.Bounds.Height)

        'Do screenshot
        Using g As Graphics = Graphics.FromImage(b)
            g.CopyFromScreen(0, 0, 0, 0, b.Size, CopyPixelOperation.SourceCopy)
            g.Save()
        End Using

        'Resize and free memory
        Dim j As Bitmap = New Bitmap(b, New Size(60, 10))
        b.Dispose()

        Return j
    End Function

    '-------------------------------------------------------------------------------------------------------------------------


    'DEV command: Shift senseless text to display
    Private Sub shiftCommand()
        Dim startCommand As Byte() = {35}
        Dim endCommand As Byte() = {36}
        Dim command As String = ASCIIBytesToString(startCommand) + "HAILEESTEINFELD1234567" + ASCIIBytesToString(endCommand)

        comPort.Write(command)
    End Sub

    'Shift message
    Private Sub shiftMessage(msgByte As Byte, msgLenth As Byte, msgLongFlag0 As Byte, msgLongFlag1 As Byte, msg As String)
        Dim startCommand As Byte() = {35}
        Dim endCommand As Byte() = {36}
        Dim msgCommand As Byte()
        If msgByte = 31 Then
            msgCommand = {31}
        Else
            msgCommand = {33}
        End If
        Dim msgBytes As Byte() = {1, 0, 0}
        msgBytes(0) = msgLenth
        msgBytes(1) = msgLongFlag0
        msgBytes(2) = msgLongFlag1

        Dim whiteSpaces As String = "      "
        Dim command As String = ASCIIBytesToString(startCommand) + ASCIIBytesToString(msgCommand) + msg + whiteSpaces + whiteSpaces + ASCIIBytesToString(msgBytes) + ASCIIBytesToString(endCommand)

        If msgLongFlag0 = 1 Then
            command = ASCIIBytesToString(startCommand) + ASCIIBytesToString(msgCommand) + msg + whiteSpaces + ASCIIBytesToString(msgBytes) + ASCIIBytesToString(endCommand)
        End If
        If msgLongFlag1 = 1 Then
            command = ASCIIBytesToString(startCommand) + ASCIIBytesToString(msgCommand) + msg + ASCIIBytesToString(msgBytes) + ASCIIBytesToString(endCommand)
        End If

        comPort.Write(command)
    End Sub

    'Shift custom LED color
    Private Sub shiftLEDWheel()
        Dim command = New Byte(24) {}
        'Start byte, stop byte
        command(0) = 35
        command(23) = 36

        'Mode define
        command(1) = 1

        'Fill LED information
        command(2) = cLED0.G
        command(3) = cLED0.R
        command(4) = cLED0.B
        command(5) = cLED1.G
        command(6) = cLED1.R
        command(7) = cLED1.B
        command(8) = cLED2.G
        command(9) = cLED2.R
        command(10) = cLED2.B
        command(11) = cLED3.G
        command(12) = cLED3.R
        command(13) = cLED3.B
        command(14) = cLED4.G
        command(15) = cLED4.R
        command(16) = cLED4.B
        command(17) = cLED5.G
        command(18) = cLED5.R
        command(19) = cLED5.B

        'Zero out the rest
        command(20) = 0
        command(21) = 0
        command(22) = 0

        Try
            comPort.Write(command, 0, 24)
        Catch ex As Exception
        End Try
    End Sub

    'Shift custom LED color with ledSmoothFade(), used in ambient light mode
    Private Sub shiftSmooth()
        Dim command = New Byte(24) {}
        'Start byte, stop byte
        command(0) = 35
        command(23) = 36

        'Mode define
        command(1) = 2

        'Fill LED information
        For i As Integer = 0 To 5
            command((3 * i) + 2) = moodLight(i).G
            command((3 * i) + 3) = moodLight(i).R
            command((3 * i) + 4) = moodLight(i).B
        Next

        'Zero out the rest
        command(20) = 0
        command(21) = 0
        command(22) = 0

        Try
            comPort.Write(command, 0, 24)
        Catch ex As Exception
        End Try

    End Sub

    'Shift static color, special case of shiftDynamicColor (below)
    Private Sub shiftStaticColor(colorOption As Byte)
        Dim command = New Byte(24) {}

        'Start byte, stop byte
        command(0) = 35
        command(23) = 36

        'Mode define
        command(1) = 32
        command(2) = 1

        'Fill LED mode information
        command(3) = colorOption

        'Zero out the rest
        For i As Integer = 4 To 22
            command(i) = 0
        Next

        'Write to serial
        Try
            comPort.Write(command, 0, 24)
        Catch ex As Exception
        End Try

        'Freeze myOpenVFD for 800 ms.
        System.Threading.Thread.Sleep(800)
    End Sub

    'Just a fancy version of shift single color
    Private Sub shiftDynamicColor(newMode As Byte, param0 As Byte)
        Dim command = New Byte(24) {}

        'Start byte, stop byte
        command(0) = 35
        command(23) = 36

        'Mode define, use parameters
        command(1) = 32
        command(2) = newMode
        command(3) = param0

        'Zero out the rest
        For i As Integer = 4 To 22
            command(i) = 0
        Next

        'Write to serial
        Try
            comPort.Write(command, 0, 24)
        Catch ex As Exception
        End Try

        System.Threading.Thread.Sleep(1000)
    End Sub


    'Startup routine
    Private Sub OpenVFD_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        'Get startup parameter
        Dim k As Integer
        For Each s As String In Environment.GetCommandLineArgs
            If k > 0 Then
                If s = "-dev" Then
                    p_Buttons.Enabled = True
                    Me.Text = "myOpenVFD [Developer Mode]"
                End If
            End If
            k += 1
        Next

        'Get serial ports
        Try
            b_Connect.Enabled = False
            Try
                For Each sp As String In My.Computer.Ports.SerialPortNames
                    txt_SerialPorts.Items.Add(sp)
                Next
            Catch ex As Exception
            End Try
        Catch ex As Exception
        End Try

        'Draw LED simulation picture boxes
        For i = 0 To 24
            timebar(i) = New PictureBox
            timebar(i).Name = "rect" + Str(i)
            timebar(i).BackColor = Color.LightGray
            timebar(i).Location = New Point(75 + 25 * i, 110)
            timebar(i).Size = New Size(20, 20)
            timebar(i).Visible = True
            p_LEDs.Controls.Add(timebar(i))
        Next
        For i = 0 To 5
            ledbar(i) = New PictureBox
            ledbar(i).Name = "ledb" + Str(i)
            ledbar(i).BackColor = Color.LightGray
            ledbar(i).Location = New Point(615 - (108 * i), 10)
            ledbar(i).Size = New Size(80, 80)
            ledbar(i).Visible = True
            p_LEDs.Controls.Add(ledbar(i))
        Next
        For i = 0 To 5
            moodbar(i) = New PictureBox
            moodbar(i).Name = "ledb" + Str(i)
            moodbar(i).BackColor = Color.LightGray
            moodbar(i).Location = New Point(50 * i, 5)
            moodbar(i).Size = New Size(30, 30)
            moodbar(i).Visible = True
            p_Mood.Controls.Add(moodbar(i))
        Next

        For i = 0 To 5
            hslColorArray(i) = New HslColor(i * 40, 192, 155)
        Next

        'Define colors
        ResistorArray(0) = Color.Black
        ResistorArray(1) = Color.SaddleBrown
        ResistorArray(2) = Color.Firebrick
        ResistorArray(3) = Color.DarkOrange
        ResistorArray(4) = Color.Yellow
        ResistorArray(5) = Color.ForestGreen
        ResistorArray(6) = Color.SteelBlue
        ResistorArray(7) = Color.DarkViolet
        ResistorArray(8) = Color.Gray
        ResistorArray(9) = Color.Snow

        crossTrackBar.Value = 10
        cb_ChFdMode.Text = "0"
        dancingTrackBar.Value = 10
        cb_CopMode.Text = "0"
    End Sub


    'myOpenVFD commands:
    Private Sub b_Connect_Click(sender As Object, e As EventArgs) Handles b_Connect.Click
        If l_connect.Text = "Connect" Then
            comPort.Close()
            comPort.PortName = txt_SerialPorts.Text
            comPort.BaudRate = 115200
            comPort.DataBits = 8
            comPort.Parity = Parity.None
            comPort.StopBits = StopBits.One
            comPort.Handshake = Handshake.None
            comPort.Encoding = System.Text.Encoding.Default

            comPort.Open()

            p_Buttons.Enabled = True
            l_connect.Text = "Disconnect"
            b_Connect.Text = "◼"

        ElseIf l_connect.Text = "Disconnect" Then
            txt_SerialPorts.Text = ""
            l_connect.Text = "Connect"
            b_Connect.Text = "►"
            b_Connect.Enabled = False
            comPort.Close()

            p_Buttons.Enabled = False

            p_About.Hide()
            p_Msg.Hide()
            p_Presets.Hide()
            p_SaveSettings.Hide()
            p_SetColors.Hide()
            p_TimeSync.Hide()

        End If
    End Sub

    Private Sub txt_SerialPorts_SelectedIndexChanged(sender As Object, e As EventArgs) Handles txt_SerialPorts.SelectedIndexChanged
        If Not txt_SerialPorts.Text = "" Then
            b_Connect.Enabled = True
        Else
            b_Connect.Enabled = False
        End If
    End Sub

    'Private Sub ch_5_CheckedChanged(sender As Object, e As EventArgs)
    '    If ch_5.Checked Then
    '        ledManager.Color = cLED5
    '    End If
    'End Sub

    'Private Sub ch_4_CheckedChanged(sender As Object, e As EventArgs)
    '    If ch_4.Checked Then
    '        ledManager.Color = cLED4
    '    End If
    'End Sub

    'Private Sub ch_3_CheckedChanged(sender As Object, e As EventArgs)
    '    If ch_3.Checked Then
    '        ledManager.Color = cLED3
    '    End If
    'End Sub

    'Private Sub ch_2_CheckedChanged(sender As Object, e As EventArgs)
    '    If ch_2.Checked Then
    '        ledManager.Color = cLED2
    '    End If
    'End Sub

    'Private Sub ch_1_CheckedChanged(sender As Object, e As EventArgs)
    '    If ch_1.Checked Then
    '        ledManager.Color = cLED1
    '    End If
    'End Sub

    'Private Sub ch_0_CheckedChanged(sender As Object, e As EventArgs)
    '    If ch_0.Checked Then
    '        ledManager.Color = cLED0
    '    End If
    'End Sub

    Private Sub b_SetColors_Click(sender As Object, e As EventArgs) Handles b_SetColors.Click
        p_SetColors.Visible = True
        p_SetColors.BringToFront()

        scrTimer.Enabled = False
        l_MoodStatus.Text = "Click on the computer" & vbCrLf & "to begin!"
    End Sub

    Private Sub b_SendMsg_Click(sender As Object, e As EventArgs) Handles b_SendMsg.Click
        p_Msg.Visible = True
        p_Msg.BringToFront()

        scrTimer.Enabled = False
        l_MoodStatus.Text = "Click on the computer" & vbCrLf & "to begin!"
    End Sub

    Private Sub b_TimeSync_Click(sender As Object, e As EventArgs) Handles b_TimeSync.Click
        p_TimeSync.Visible = True
        p_TimeSync.BringToFront()

        scrTimer.Enabled = False
        l_MoodStatus.Text = "Click on the computer" & vbCrLf & "to begin!"
    End Sub

    Private Sub b_Presets_Click(sender As Object, e As EventArgs) Handles b_Presets.Click
        p_Presets.Visible = True
        p_Presets.BringToFront()
    End Sub

    Private Sub b_SaveSettings_Click(sender As Object, e As EventArgs) Handles b_SaveSettings.Click
        p_SaveSettings.Visible = True
        p_SaveSettings.BringToFront()

        scrTimer.Enabled = False
        l_MoodStatus.Text = "Click on the computer" & vbCrLf & "to begin!"
    End Sub

    Private Sub timeTimer_Tick(sender As Object, e As EventArgs) Handles timeTimer.Tick
        l_Time.Text = FormatDateTime(My.Computer.Clock.LocalTime, DateFormat.LongTime).ToString
        l_Date.Text = FormatDateTime(My.Computer.Clock.LocalTime, DateFormat.LongDate).ToString
    End Sub

    Private Sub bS_Spectrum_Click(sender As Object, e As EventArgs) Handles bS_Spectrum.Click
        enabledFade = 0
        speaker.Visible = False
        note.Visible = False

        p_LEDSpectrum.Visible = True
        p_LEDSpectrum.BringToFront()
    End Sub

    Private Sub bS_Cross_Click(sender As Object, e As EventArgs) Handles bS_Cross.Click
        enabledFade = 1

        For i = 0 To 5
            hslColorArray(i) = New HslColor(i * 40, 192, 155)
        Next
        speaker.Visible = False
        note.Visible = False

        p_LEDCross.Visible = True
        p_LEDCross.BringToFront()
    End Sub

    Private Sub bS_Chase_Click(sender As Object, e As EventArgs) Handles bS_Chase.Click
        enabledFade = 2
        speaker.Visible = False
        note.Visible = False

        p_LEDChaseFade.Visible = True
        p_LEDChaseFade.BringToFront()
    End Sub

    Private Sub bS_RCode_Click(sender As Object, e As EventArgs) Handles bS_RCode.Click
        enabledFade = 3
        For i = 0 To 24
            timebar(i).BackColor = Color.LightGray
        Next
        speaker.Visible = False
        note.Visible = False

        p_LEDResistorCode.Visible = True
        p_LEDResistorCode.BringToFront()
    End Sub

    Private Sub bS_Mic_Click(sender As Object, e As EventArgs) Handles bS_Mic.Click
        enabledFade = 4
        For i = 0 To 5
            hslColorArray(i) = New HslColor(i * 20, 192, 155)
        Next
        For i = 0 To 24
            timebar(i).BackColor = Color.LightGray
        Next
        speaker.Visible = True
        note.Visible = True

        p_LEDMic.Visible = True
        p_LEDMic.BringToFront()
    End Sub

    Private Sub bS_Cop_Click(sender As Object, e As EventArgs) Handles bS_Cop.Click
        enabledFade = 5

        speaker.Visible = False
        note.Visible = False

        p_LEDCop.Visible = True
        p_LEDCop.BringToFront()
    End Sub

    Private Sub LinkLabel1_LinkClicked(sender As Object, e As LinkLabelLinkClickedEventArgs) Handles LinkLabel1.LinkClicked
        p_ResistorsMore.Visible = True
        p_ResistorsMore.BringToFront()
    End Sub

    Private Sub LinkLabel2_LinkClicked(sender As Object, e As LinkLabelLinkClickedEventArgs) Handles LinkLabel2.LinkClicked
        p_Resistors.Visible = True
        p_Resistors.BringToFront()
    End Sub

    Private Sub cb_ChFdMode_SelectedIndexChanged(sender As Object, e As EventArgs) Handles cb_ChFdMode.SelectedIndexChanged
        If cb_ChFdMode.Text = "0" Then
            l_chFdInfo.Text = "... from right to left, every second"
        ElseIf cb_ChFdMode.Text = "1" Then
            l_chFdInfo.Text = "... from left to right, every second"
        ElseIf cb_ChFdMode.Text = "2" Then
            l_chFdInfo.Text = "... flips the direction on every second change"
        ElseIf cb_ChFdMode.Text = "3" Then
            l_chFdInfo.Text = "... changes on clap or to the beat of your music"
        End If
    End Sub

    Private Sub Label27_Click(sender As Object, e As EventArgs) Handles Label27.Click
        aboutDialog.ShowDialog()
    End Sub

    Private Sub b_UserManual_Click(sender As Object, e As EventArgs) Handles b_UserManual.Click
        System.Diagnostics.Process.Start("https://www.thevfdcollective.com/support")
    End Sub

    Private Sub b_UpdateCheck_Click(sender As Object, e As EventArgs) Handles b_UpdateCheck.Click
        System.Diagnostics.Process.Start("https://www.thevfdcollective.com/support")
    End Sub

    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Button1.Click
        MsgBox("Memory access violation at address 0xBADC0DED in module 'myOpenVFD.exe'. Write of address 0xC001CAFE", MsgBoxStyle.Critical, "Stack Overflow 0xBADC0DED")
        MsgBox("Just kidding. #nerdface", MsgBoxStyle.Exclamation, "Oops")
    End Sub

    Private Sub l_ambient_Click(sender As Object, e As EventArgs) Handles pImage.Click
        If scrTimer.Enabled = True Then
            scrTimer.Enabled = False
            l_MoodStatus.Text = "Click on the computer" & vbCrLf & "to begin!"
        ElseIf scrTimer.Enabled = False Then
            scrTimer.Enabled = True
            l_MoodStatus.Text = "Calculating for you."
        End If
    End Sub

    Private Sub scrTimer_Tick(sender As Object, e As EventArgs) Handles scrTimer.Tick
        'Get downsized screenshot
        Dim bmp As Bitmap = Screenshot()

        'Calculate two dimensional mean value
        For arrIndex As Byte = 0 To 5
            Dim rD As Integer = 0
            Dim bD As Integer = 0
            Dim gD As Integer = 0

            For i As Byte = 0 To 9
                For j As Byte = 1 To 7
                    Dim k As Byte = i + (10 * arrIndex)
                    rD += bmp.GetPixel(k, j).R
                    gD += bmp.GetPixel(k, j).G
                    bD += bmp.GetPixel(k, j).B
                Next
            Next

            rD \= 70
            gD \= 70
            bD \= 70

            Dim r As Byte = Convert.ToByte(rD)
            Dim g As Byte = Convert.ToByte(gD)
            Dim b As Byte = Convert.ToByte(bD)

            Dim rS As Int16 = IIf(r < 128, r, r - 256)
            Dim gS As Int16 = IIf(g < 128, g, g - 256)
            Dim bS As Int16 = IIf(b < 128, b, b - 256)

            Dim thresholdVal As Byte
            Dim rShiftFactor As Byte = 1

            'Weighting
            If r > b And r > g Then
                thresholdVal = r >> 2 '25 percent of r
                'r dominant
                Dim bgDiff As Int16 = bS - gS
                If bgDiff > thresholdVal Then
                    'b subdominant, divide g by 4
                    g >>= (rShiftFactor)
                ElseIf bgDiff < thresholdVal Then
                    'g subdominant, divide b by 4
                    b >>= (rShiftFactor + 1)
                Else
                    g >>= (rShiftFactor)
                    b >>= (rShiftFactor + 1)
                End If
            ElseIf g > r And g > b Then
                If (Convert.ToInt16(g) + (Convert.ToInt16(g) >> 1)) < Convert.ToInt16(255) Then
                    g = (g + (g >> 1))
                Else
                    g = 255
                End If
                thresholdVal = g >> 2 '25 percent of g
                'g dominant 
                Dim rbDiff As Int16 = rS - bS
                If rbDiff > thresholdVal Then
                    'r subdominant, divide b by 4
                    b >>= (rShiftFactor + 3)
                ElseIf rbDiff < thresholdVal Then
                    'b subdominant, divide r by 4
                    r >>= (rShiftFactor + 1)
                Else
                    r >>= (rShiftFactor + 1)
                    b >>= (rShiftFactor + 3)
                End If
            ElseIf b > r And b > g Then
                thresholdVal = b >> 2 '25 percent of b
                'b dominant
                Dim rgDiff As Int16 = rS - gS
                If rgDiff > thresholdVal Then
                    'r subdominant, divide g by 4
                    g >>= rShiftFactor
                ElseIf rgDiff < thresholdVal Then
                    'g subdominant, divide r by 4
                    r >>= rShiftFactor
                Else
                    r >>= rShiftFactor
                    g >>= rShiftFactor
                End If
            End If

            'HSL transform, Saturation increase
            Dim hRGB As New HslColor
            hRGB = HslColor.FromRgbColor(Color.FromArgb(Convert.ToByte(r), Convert.ToByte(g), Convert.ToByte(b)))
            hRGB.Saturation = hRGB.Saturation + (hRGB.Saturation >> 3)


            'Set array colors
            moodLight(5 - arrIndex) = hRGB.ToRgbColor
        Next

        shiftSmooth()
    End Sub

    Private Sub tab_ColorPresets_SelectedIndexChanged(sender As Object, e As EventArgs) Handles tab_ColorPresets.SelectedIndexChanged
        If Not tab_ColorPresets.SelectedIndex = tab_Ambient.TabIndex Then
            scrTimer.Enabled = False
            l_MoodStatus.Text = "Click on the computer" & vbCrLf & "to begin!"
        End If
    End Sub

    Private Sub b_UpdateFirmware_Click(sender As Object, e As EventArgs) Handles b_UpdateFirmware.Click
        fwUpdater.ShowDialog()
    End Sub

    Private Sub l_Save1_Click(sender As Object, e As EventArgs) Handles l_Save1.Click, l_Save2.Click
        'Fill information byte array

        Dim command = New Byte(24) {}
        'Start byte, stop byte
        command(0) = 35
        command(23) = 36

        'Mode define
        command(1) = 51

        'Set the rest zero
        For i As Integer = 2 To 22
            command(i) = 0
        Next

        Try
            comPort.Write(command, 0, 24)
        Catch ex As Exception
        End Try

        l_Save1.Text = "WAIT"

        System.Threading.Thread.Sleep(2500)

        l_Save1.Text = "SAVE"
    End Sub

    Private Sub l_Reset_Click(sender As Object, e As EventArgs) Handles l_Reset.Click
        'Fill information byte array

        Dim command = New Byte(24) {}
        'Start byte, stop byte
        command(0) = 35
        command(23) = 36

        'Mode define
        command(1) = 52

        'Set the rest zero
        For i As Integer = 2 To 22
            command(i) = 0
        Next

        Try
            comPort.Write(command, 0, 24)
        Catch ex As Exception
        End Try

        l_Reset.Text = "Please wait while we do the restore..."

        System.Threading.Thread.Sleep(2500)

        l_Reset.Text = "(Or you can set the settings back to default)"
    End Sub

    Private Sub b_About_Click(sender As Object, e As EventArgs) Handles b_About.Click
        scrTimer.Enabled = False
        l_MoodStatus.Text = "Click on the computer" & vbCrLf & "to begin!"

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
            comPort.Write(command, 0, 24)


            System.Threading.Thread.Sleep(200)

            Dim receiveMsg As Byte()
            receiveMsg = ReceiveSerialData()

            'Write Message Completion
            If receiveMsg(0) = 35 And receiveMsg(1) = 34 And receiveMsg(9) = 36 Then
                Dim j = New Byte(7) {}
                For i As Integer = 2 To 8
                    j(i - 2) = receiveMsg(i)
                Next

                p_About.Visible = True
                p_About.BringToFront()

                Dim fwstring As String = System.Text.Encoding.ASCII.GetString(j).TrimEnd(vbNullChar)
                l_FWString.Text = fwstring

                l_myVersion.Text = My.Application.Info.Version.ToString()
            End If
        Catch ex As Exception
            MsgBox(ex.Message, MsgBoxStyle.Exclamation, "Oops... Something went wrong!")
            Exit Sub
        End Try
    End Sub


    Private Sub cb_12h_CheckedChanged(sender As Object, e As EventArgs) Handles cb_12h.CheckedChanged

        'Fill information byte array

        Dim command = New Byte(24) {}
        'Start byte, stop byte
        command(0) = 35
        command(23) = 36

        'Mode define
        command(1) = &H12
        command(2) = 0  '12h cflag

        '12h mode set
        If cb_12h.Checked Then
            command(3) = 1
            cb_leadingZero.Enabled = True
        Else
            command(3) = 0
            cb_leadingZero.Enabled = False
        End If

        'Set the rest zero
        For i As Integer = 4 To 22
            command(i) = 0
        Next

        Try
            comPort.Write(command, 0, 24)
        Catch ex As Exception
        End Try

        System.Threading.Thread.Sleep(2500)
    End Sub

    Private Sub cb_intd_CheckedChanged(sender As Object, e As EventArgs) Handles cb_intd.CheckedChanged
        'Fill information byte array

        Dim command = New Byte(24) {}
        'Start byte, stop byte
        command(0) = 35
        command(23) = 36

        'Mode define
        command(1) = &H12
        command(2) = 1  '12h cflag

        'intd mode set
        If cb_intd.Checked Then
            command(3) = 1
        Else
            command(3) = 0
        End If

        'Set the rest zero
        For i As Integer = 4 To 22
            command(i) = 0
        Next

        Try
            comPort.Write(command, 0, 24)
        Catch ex As Exception
        End Try

        System.Threading.Thread.Sleep(2500)
    End Sub

    Private Sub cb_leadingZero_CheckedChanged(sender As Object, e As EventArgs) Handles cb_leadingZero.CheckedChanged
        'Fill information byte array

        Dim command = New Byte(24) {}
        'Start byte, stop byte
        command(0) = 35
        command(23) = 36

        'Mode define
        command(1) = &H12
        command(2) = 3  'leading zero enable flag

        'intd mode set
        If cb_leadingZero.Checked Then
            command(3) = 1
        Else
            command(3) = 0
        End If

        'Set the rest zero
        For i As Integer = 4 To 22
            command(i) = 0
        Next

        Try
            comPort.Write(command, 0, 24)
        Catch ex As Exception
        End Try

        System.Threading.Thread.Sleep(2500)
    End Sub

    Private Sub cb_nightShiftToggle_CheckedChanged(sender As Object, e As EventArgs) Handles cb_nightShiftToggle.CheckedChanged
        'Fill information byte array

        Dim command = New Byte(24) {}
        'Start byte, stop byte
        command(0) = 35
        command(23) = 36

        'Mode define
        command(1) = &H11
        command(2) = &H23 'MS flag 1
        command(9) = &H23 'MS flag 2

        command(7) = 42 'Any random value to make no changes

        If cb_nightShiftToggle.Checked Then
            command(8) = 1
        Else
            command(8) = 0
        End If

        'Set the rest zero
        For i As Integer = 10 To 22
            command(i) = 0
        Next

        Try
            comPort.Write(command, 0, 24)
        Catch ex As Exception
        End Try

        System.Threading.Thread.Sleep(2500)
    End Sub

    Private Sub cb_scheduledNightShift_CheckedChanged(sender As Object, e As EventArgs) Handles cb_scheduledNightShift.CheckedChanged
        'Fill information byte array

        Dim command = New Byte(24) {}
        'Start byte, stop byte
        command(0) = 35
        command(23) = 36

        'Mode define
        command(1) = &H11
        command(2) = &H23 'MS flag 1
        command(9) = &H23 'MS flag 2

        If cb_scheduledNightShift.Checked Then
            command(7) = 1

            command(3) = Convert.ToByte(timeNightShiftEnable.Value.Minute)
            command(4) = Convert.ToByte(timeNightShiftEnable.Value.Hour)
            command(5) = Convert.ToByte(timeNightShiftDisable.Value.Minute)
            command(6) = Convert.ToByte(timeNightShiftDisable.Value.Hour)
        Else
            command(7) = 0
        End If

        command(8) = 42 'Also some random value

        'Set the rest zero
        For i As Integer = 10 To 22
            command(i) = 0
        Next

        Try
            comPort.Write(command, 0, 24)
        Catch ex As Exception
        End Try

        System.Threading.Thread.Sleep(2500)
    End Sub

    'LED modellizing
    Private Sub ledTimer_Tick(sender As Object, e As EventArgs) Handles ledTimer.Tick
        ' Modellize spectrum fade
        If enabledFade = 0 Then
            ledTimer.Interval = 500
            If hslWheel.Hue < 240 Then
                hslWheel.Hue += 20
            ElseIf hslWheel.Hue = 240 Then
                hslWheel.Hue = 0
            End If

            Dim k As Integer = hslWheel.Hue / 10
            For i = 0 To k
                timebar(i).BackColor = Color.Teal
            Next
            For i = k + 1 To 24
                timebar(i).BackColor = Color.LightGray
            Next

            For i = 0 To 5
                ledbar(i).BackColor = hslWheel.ToRgbColor()
            Next
        End If

        'Modellize cross fade
        If enabledFade = 1 Then
            ledTimer.Interval = 500
            For i = 0 To 5
                If hslColorArray(i).Hue < 240 Then
                    hslColorArray(i).Hue += 20
                ElseIf hslColorArray(i).Hue = 240 Then
                    hslColorArray(i).Hue = 0
                End If
                ledbar(i).BackColor = hslColorArray(i).ToRgbColor
            Next

            Dim k As Integer = hslColorArray(0).Hue / 10
            For i = 0 To k
                timebar(i).BackColor = Color.Teal
            Next
            For i = k + 1 To 24
                timebar(i).BackColor = Color.LightGray
            Next
        End If

        'Modellize chase fade
        If enabledFade = 2 Then
            ledTimer.Interval = 1000
            ledSubTimer.Enabled = True
            ledSubTimer.Start()

            If hslWheel.Hue < 240 Then
                hslWheel.Hue += 20
            ElseIf hslWheel.Hue = 240 Then
                hslWheel.Hue = 0
            End If

            Dim k As Integer = hslWheel.Hue / 10
            For i = 0 To k
                timebar(i).BackColor = Color.Teal
            Next
            For i = k + 1 To 24
                timebar(i).BackColor = Color.LightGray
            Next
        End If

        'Modellize resistor fade
        If enabledFade = 3 Then
            ledTimer.Interval = 200

            With My.Computer.Clock.LocalTime
                Dim timeArray = New Byte(6) {}
                timeArray(0) = .Second Mod 10
                timeArray(1) = .Second \ 10
                timeArray(2) = .Minute Mod 10
                timeArray(3) = .Minute \ 10
                timeArray(4) = .Hour Mod 10
                timeArray(5) = .Hour \ 10

                For i As Integer = 0 To 5
                    ledbar(i).BackColor = ResistorArray(timeArray(i))
                Next
            End With

        End If

        'Modellize mic input
        If enabledFade = 4 Then

            ledTimer.Interval = CInt(Int(500 + (500 * Rnd())))
            fullWrite = Not fullWrite
            randomint = CInt(Int((5 * Rnd()) + 1))
            ledSubTimer.Enabled = True
            ledSubTimer.Start()

            If fullWrite Then
                speaker.Image = My.Resources.speaker_on
                note.Visible = True
                note.BringToFront()
            Else
                speaker.Image = My.Resources.speaker_off
                note.Visible = False
            End If

            For i = 0 To 5
                If hslColorArray(i).Hue < 240 Then
                    hslColorArray(i).Hue += 20
                ElseIf hslColorArray(i).Hue = 240 Then
                    hslColorArray(i).Hue = 0
                End If
            Next
        End If

        If enabledFade = 5 Then
            ledTimer.Interval = 200
            copFlipState = Not copFlipState

            If copFlipState Then
                For i As Integer = 0 To 2
                    ledbar(i).BackColor = Color.Red
                Next
                For i As Integer = 3 To 5
                    ledbar(i).BackColor = Color.Blue
                Next
            Else
                For i As Integer = 0 To 2
                    ledbar(i).BackColor = Color.Blue
                Next
                For i As Integer = 3 To 5
                    ledbar(i).BackColor = Color.Red
                Next
            End If
        End If
    End Sub

    Private Sub ledSubTimer_Tick(sender As Object, e As EventArgs) Handles ledSubTimer.Tick
        If enabledFade = 2 Then
            ledSubTimer.Interval = 100
            ledbar(globalTimeFactor).BackColor = hslWheel.ToRgbColor
            If globalTimeFactor < 5 Then
                globalTimeFactor += 1
            ElseIf globalTimeFactor = 5 Then
                globalTimeFactor = 0
                ledSubTimer.Stop()
                ledSubTimer.Enabled = False
            End If
        End If

        If enabledFade = 4 Then
            ledSubTimer.Interval = 50
            If fullWrite = True Then
                For i As Integer = 0 To globalTimeFactor
                    ledbar(globalTimeFactor).BackColor = hslColorArray(i).ToRgbColor
                Next
            End If
            If fullWrite = False Then
                If globalTimeFactor < randomint Then
                    'For i As Integer = 0 To globalTimeFactor
                    ledbar(globalTimeFactor).BackColor = hslColorArray(globalTimeFactor).ToRgbColor
                    'Next
                Else
                    ledbar(globalTimeFactor).BackColor = Color.LightGray
                End If
            End If

            If globalTimeFactor < 5 Then
                globalTimeFactor += 1
            ElseIf globalTimeFactor = 5 Then
                globalTimeFactor = 0
                ledSubTimer.Stop()
                ledSubTimer.Enabled = False
            End If
        End If
    End Sub

    Private Sub scrSimulator_Tick(sender As Object, e As EventArgs) Handles scrSimulator.Tick
        Dim bmp As Bitmap

        If pImage.Image IsNot Nothing Then
            pImage.Image.Dispose()
        End If
        If pCounter = 0 Then
            pImage.Image = My.Resources.res0
            bmp = My.Resources.res0
        ElseIf pCounter = 1 Then
            pImage.Image = My.Resources.res1
            bmp = My.Resources.res1
        ElseIf pCounter = 2 Then
            pImage.Image = My.Resources.res2
            bmp = My.Resources.res2
        ElseIf pCounter = 3 Then
            pImage.Image = My.Resources.res3
            bmp = My.Resources.res3
        Else
            pImage.Image = My.Resources.res3
            bmp = My.Resources.res3
        End If

        Dim bmpResd As Bitmap = New Bitmap(bmp, New Size(60, 10))
        bmp.Dispose()


        For arrIndex As Byte = 0 To 5
            Dim rD As Integer = 0
            Dim bD As Integer = 0
            Dim gD As Integer = 0

            For i As Byte = 0 To 9
                For j As Byte = 1 To 7
                    Dim k As Byte = i + (10 * arrIndex)
                    rD += bmpResd.GetPixel(k, j).R
                    gD += bmpResd.GetPixel(k, j).G
                    bD += bmpResd.GetPixel(k, j).B
                Next
            Next

            rD \= 70
            gD \= 70
            bD \= 70
            moodbar(arrIndex).BackColor = Color.FromArgb(Convert.ToByte(rD), Convert.ToByte(gD), Convert.ToByte(bD))
        Next



        If pCounter < 3 Then
            pCounter += 1
        ElseIf pCounter = 3 Then
            pCounter = 0
        End If

    End Sub

    '-------------------------------------------------------------------------------------------------------------------------

    'OpenVFD commands:
    'Color wheel has changed
    Private Sub ledManager_ColorChanged(sender As Object, e As EventArgs) Handles ledManager.ColorChanged
        If check_LED1.Checked Then
            cLED0 = ledManager.Color
        End If
        If check_LED2.Checked Then
            cLED1 = ledManager.Color
        End If
        If check_LED3.Checked Then
            cLED2 = ledManager.Color
        End If
        If check_LED4.Checked Then
            cLED3 = ledManager.Color
        End If
        If check_LED5.Checked Then
            cLED4 = ledManager.Color
        End If
        If check_LED6.Checked Then
            cLED5 = ledManager.Color
        End If

        shiftLEDWheel()
    End Sub

    Private Sub btn_Msg_Click(sender As Object, e As EventArgs) Handles b_Msg.Click
        If cb_customWelcome.Checked Then
            Dim msg As String = txt_Msg.Text

            If txt_Msg.Text.Length = 6 Then
                shiftMessage(33, 0, 0, 0, msg)
            Else
                MsgBox("The welcome message must have 6 characters!", MsgBoxStyle.Information, "Please re-enter the custom welcome message")
                Exit Sub
            End If
        Else
            Dim msgDisplayTime As Byte = Byte.Parse(txt_DisplayLength.Text)
            Dim msgLongFlag0 As Byte = 0
            Dim msgLongFlag1 As Byte = 0
            If txt_Msg.Text.Length > 6 Then
                msgLongFlag0 = 1
            End If
            If txt_Msg.Text.Length > 12 Then
                msgLongFlag1 = 1
            End If

            Dim msg As String = txt_Msg.Text

            If Not txt_Msg.Text.Length = 6 And Not txt_Msg.Text.Length = 12 And Not txt_Msg.Text.Length = 18 Then
                MsgBox("Oops, try to fill it with either 6, 12 or 18 characters!", MsgBoxStyle.Information, "Please re-enter the message")
                Exit Sub
            End If

            shiftMessage(31, msgDisplayTime, msgLongFlag0, msgLongFlag1, msg)
        End If
    End Sub

    Private Sub b_Sync_Click(sender As Object, e As EventArgs) Handles b_Sync.Click
        'Fill information byte array

        Dim command = New Byte(24) {}
        'Start byte, stop byte
        command(0) = 35
        command(23) = 36

        'Mode define
        command(1) = 16

        'Fill LED information
        command(2) = Convert.ToByte(My.Computer.Clock.LocalTime.Second)
        command(3) = Convert.ToByte(My.Computer.Clock.LocalTime.Minute)
        command(4) = Convert.ToByte(My.Computer.Clock.LocalTime.Hour)
        command(5) = Convert.ToByte(My.Computer.Clock.LocalTime.Day)
        command(6) = Convert.ToByte(My.Computer.Clock.LocalTime.Month)
        command(7) = Convert.ToByte((My.Computer.Clock.LocalTime.Year) Mod 100)

        command(8) = 35

        'Set the rest zero
        For i As Integer = 9 To 22
            command(i) = 0
        Next

        Try
            comPort.Write(command, 0, 24)
        Catch ex As Exception

        End Try

        'Wait for some time
        System.Threading.Thread.Sleep(1600)

        Dim receiveMsg As Byte()
        receiveMsg = ReceiveSerialData()

        'Write Message Completion
        If receiveMsg(0) = 35 And receiveMsg(1) = 16 And receiveMsg(9) = 36 Then
            Dim j = New Byte(7) {}
            For i As Integer = 2 To 8
                j(i - 2) = receiveMsg(i)
            Next

            Dim successMessage As String = System.Text.Encoding.ASCII.GetString(j).TrimEnd(vbNullChar)
            Dim second As String = "nc Successful."
            l_SyncStatus.Text = String.Concat(successMessage, second)
        End If
    End Sub

    Private Sub pre_off_Click(sender As Object, e As EventArgs) Handles pre_off.Click
        shiftStaticColor(0)
    End Sub

    Private Sub pre_white_Click(sender As Object, e As EventArgs) Handles pre_white.Click
        shiftStaticColor(1)
    End Sub

    Private Sub pre_warmwhite_Click(sender As Object, e As EventArgs) Handles pre_warmwhite.Click
        shiftStaticColor(2)
    End Sub

    Private Sub pre_red_Click(sender As Object, e As EventArgs) Handles pre_red.Click
        shiftStaticColor(3)
    End Sub

    Private Sub pre_green_Click(sender As Object, e As EventArgs) Handles pre_green.Click
        shiftStaticColor(4)
    End Sub

    Private Sub pre_blue_Click(sender As Object, e As EventArgs) Handles pre_blue.Click
        shiftStaticColor(5)
    End Sub

    Private Sub pre_yellow_Click(sender As Object, e As EventArgs) Handles pre_yellow.Click
        shiftStaticColor(6)
    End Sub

    Private Sub pre_orange_Click(sender As Object, e As EventArgs) Handles pre_orange.Click
        shiftStaticColor(7)
    End Sub

    Private Sub pre_cyan_Click(sender As Object, e As EventArgs) Handles pre_cyan.Click
        shiftStaticColor(8)
    End Sub

    Private Sub pre_magenta_Click(sender As Object, e As EventArgs) Handles pre_magenta.Click
        shiftStaticColor(9)
    End Sub

    Private Sub pre_purple_Click(sender As Object, e As EventArgs) Handles pre_purple.Click
        shiftStaticColor(10)
    End Sub

    Private Sub pre_Rainbow_Click(sender As Object, e As EventArgs) Handles pre_Rainbow.Click
        shiftStaticColor(11)
    End Sub

    Private Sub pre_pastel_Click(sender As Object, e As EventArgs) Handles pre_pastel.Click
        shiftStaticColor(12)
    End Sub

    Private Sub pre_green2blue_Click(sender As Object, e As EventArgs) Handles pre_green2blue.Click
        shiftStaticColor(13)
    End Sub

    Private Sub pre_red2blue_Click(sender As Object, e As EventArgs) Handles pre_red2blue.Click
        shiftStaticColor(14)
    End Sub

    Private Sub pre_red2green_Click(sender As Object, e As EventArgs) Handles pre_red2green.Click
        shiftStaticColor(15)
    End Sub

    Private Sub b_Spectrum_Click(sender As Object, e As EventArgs) Handles b_Spectrum.Click
        shiftDynamicColor(2, 0)
    End Sub

    Private Sub b_Cross_Click(sender As Object, e As EventArgs) Handles b_Cross.Click
        Dim k As Byte = Byte.Parse(crossTrackBar.Value)
        shiftDynamicColor(3, k)
    End Sub

    Private Sub b_ChaseFade_Click(sender As Object, e As EventArgs) Handles b_ChaseFade.Click
        Dim k As Byte = Byte.Parse(cb_ChFdMode.Text)
        shiftDynamicColor(4, k)
    End Sub

    Private Sub b_Resistor_Click(sender As Object, e As EventArgs) Handles b_Resistor.Click
        shiftDynamicColor(5, 0)
    End Sub

    Private Sub b_Mic_Click(sender As Object, e As EventArgs) Handles b_Mic.Click
        Dim k As Byte = Byte.Parse(dancingTrackBar.Value)
        shiftDynamicColor(6, k)
    End Sub

    Private Sub b_Cop_Click(sender As Object, e As EventArgs) Handles b_Cop.Click
        Dim k As Byte = Byte.Parse(cb_CopMode.Text)
        shiftDynamicColor(7, k)
    End Sub


    Dim tmpTextStr As String

    Private Sub cb_customWelcome_CheckedChanged(sender As Object, e As EventArgs) Handles cb_customWelcome.CheckedChanged
        If cb_customWelcome.Checked Then
            tmpTextStr = txt_Msg.Text
            txt_Msg.Text = "HELLO "
        Else
            If Not tmpTextStr = "" Then
                txt_Msg.Text = tmpTextStr
            End If
        End If
    End Sub

End Class
