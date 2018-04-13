Imports System.Windows.Forms

Public Class aboutDialog

    Private Sub OK_Button_Click(ByVal sender As System.Object, ByVal e As System.EventArgs)
        Me.DialogResult = System.Windows.Forms.DialogResult.OK
        Me.Close()
    End Sub

    Private Sub Cancel_Button_Click(ByVal sender As System.Object, ByVal e As System.EventArgs)
        Me.DialogResult = System.Windows.Forms.DialogResult.Cancel
        Me.Close()
    End Sub

    Private Sub aboutDialog_Load(sender As Object, e As EventArgs) Handles MyBase.Load

    End Sub

    Private Sub aboutDialog_Click(sender As Object, e As EventArgs) Handles MyBase.Click
        System.Diagnostics.Process.Start("https://www.thevfdcollective.com")
        Me.Close()
    End Sub
End Class
