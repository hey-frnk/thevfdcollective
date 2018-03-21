<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class fwUpdater
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.p_step1 = New System.Windows.Forms.Panel()
        Me.b_Step1 = New System.Windows.Forms.Button()
        Me.Label3 = New System.Windows.Forms.Label()
        Me.PictureBox1 = New System.Windows.Forms.PictureBox()
        Me.Label2 = New System.Windows.Forms.Label()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.p_Step2 = New System.Windows.Forms.Panel()
        Me.p_step2_1 = New System.Windows.Forms.Panel()
        Me.b_Browse = New System.Windows.Forms.Button()
        Me.t_Path = New System.Windows.Forms.TextBox()
        Me.Label6 = New System.Windows.Forms.Label()
        Me.p_step2_2 = New System.Windows.Forms.Panel()
        Me.l_NewVersion = New System.Windows.Forms.Label()
        Me.Label7 = New System.Windows.Forms.Label()
        Me.l_FWString = New System.Windows.Forms.Label()
        Me.Label4 = New System.Windows.Forms.Label()
        Me.b_Upload = New System.Windows.Forms.Button()
        Me.Label5 = New System.Windows.Forms.Label()
        Me.p_step1.SuspendLayout()
        CType(Me.PictureBox1, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.p_Step2.SuspendLayout()
        Me.p_step2_1.SuspendLayout()
        Me.p_step2_2.SuspendLayout()
        Me.SuspendLayout()
        '
        'p_step1
        '
        Me.p_step1.Controls.Add(Me.b_Step1)
        Me.p_step1.Controls.Add(Me.Label3)
        Me.p_step1.Controls.Add(Me.PictureBox1)
        Me.p_step1.Controls.Add(Me.Label2)
        Me.p_step1.Controls.Add(Me.Label1)
        Me.p_step1.Location = New System.Drawing.Point(14, 15)
        Me.p_step1.Margin = New System.Windows.Forms.Padding(3, 4, 3, 4)
        Me.p_step1.Name = "p_step1"
        Me.p_step1.Size = New System.Drawing.Size(356, 400)
        Me.p_step1.TabIndex = 0
        '
        'b_Step1
        '
        Me.b_Step1.BackColor = System.Drawing.Color.Teal
        Me.b_Step1.FlatAppearance.BorderSize = 0
        Me.b_Step1.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.b_Step1.ForeColor = System.Drawing.Color.White
        Me.b_Step1.Location = New System.Drawing.Point(230, 362)
        Me.b_Step1.Name = "b_Step1"
        Me.b_Step1.Size = New System.Drawing.Size(120, 35)
        Me.b_Step1.TabIndex = 125
        Me.b_Step1.Text = "Yes. Next!"
        Me.b_Step1.UseVisualStyleBackColor = False
        '
        'Label3
        '
        Me.Label3.Font = New System.Drawing.Font("Arial", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label3.ForeColor = System.Drawing.Color.Purple
        Me.Label3.Location = New System.Drawing.Point(3, 315)
        Me.Label3.Name = "Label3"
        Me.Label3.Size = New System.Drawing.Size(347, 27)
        Me.Label3.TabIndex = 3
        Me.Label3.Text = "Is OpenVFD connected correctly?"
        Me.Label3.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
        '
        'PictureBox1
        '
        Me.PictureBox1.Image = Global.myOpenVFD.My.Resources.Resources.HXP_TN
        Me.PictureBox1.Location = New System.Drawing.Point(16, 94)
        Me.PictureBox1.Name = "PictureBox1"
        Me.PictureBox1.Size = New System.Drawing.Size(321, 218)
        Me.PictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize
        Me.PictureBox1.TabIndex = 2
        Me.PictureBox1.TabStop = False
        '
        'Label2
        '
        Me.Label2.Font = New System.Drawing.Font("Arial", 15.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label2.ForeColor = System.Drawing.Color.Teal
        Me.Label2.Location = New System.Drawing.Point(6, 55)
        Me.Label2.Name = "Label2"
        Me.Label2.Size = New System.Drawing.Size(344, 24)
        Me.Label2.TabIndex = 1
        Me.Label2.Text = "Step 1:"
        Me.Label2.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
        '
        'Label1
        '
        Me.Label1.Font = New System.Drawing.Font("Arial", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label1.ForeColor = System.Drawing.Color.Purple
        Me.Label1.Location = New System.Drawing.Point(3, 12)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(347, 43)
        Me.Label1.TabIndex = 0
        Me.Label1.Text = "Hey there! Let me help you uploading new software " & Global.Microsoft.VisualBasic.ChrW(13) & Global.Microsoft.VisualBasic.ChrW(10) & "to your OpenVFD clock. Let's " &
    "go!"
        Me.Label1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
        '
        'p_Step2
        '
        Me.p_Step2.Controls.Add(Me.p_step2_1)
        Me.p_Step2.Controls.Add(Me.p_step2_2)
        Me.p_Step2.Controls.Add(Me.l_FWString)
        Me.p_Step2.Controls.Add(Me.Label4)
        Me.p_Step2.Controls.Add(Me.b_Upload)
        Me.p_Step2.Controls.Add(Me.Label5)
        Me.p_Step2.Location = New System.Drawing.Point(14, 15)
        Me.p_Step2.Margin = New System.Windows.Forms.Padding(3, 4, 3, 4)
        Me.p_Step2.Name = "p_Step2"
        Me.p_Step2.Size = New System.Drawing.Size(356, 400)
        Me.p_Step2.TabIndex = 126
        Me.p_Step2.Visible = False
        '
        'p_step2_1
        '
        Me.p_step2_1.Controls.Add(Me.b_Browse)
        Me.p_step2_1.Controls.Add(Me.t_Path)
        Me.p_step2_1.Controls.Add(Me.Label6)
        Me.p_step2_1.Location = New System.Drawing.Point(9, 168)
        Me.p_step2_1.Name = "p_step2_1"
        Me.p_step2_1.Size = New System.Drawing.Size(341, 144)
        Me.p_step2_1.TabIndex = 128
        '
        'b_Browse
        '
        Me.b_Browse.BackColor = System.Drawing.Color.Teal
        Me.b_Browse.FlatAppearance.BorderSize = 0
        Me.b_Browse.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.b_Browse.ForeColor = System.Drawing.Color.White
        Me.b_Browse.Location = New System.Drawing.Point(219, 69)
        Me.b_Browse.Name = "b_Browse"
        Me.b_Browse.Size = New System.Drawing.Size(120, 22)
        Me.b_Browse.TabIndex = 126
        Me.b_Browse.Text = "Browse..."
        Me.b_Browse.TextAlign = System.Drawing.ContentAlignment.TopCenter
        Me.b_Browse.UseVisualStyleBackColor = False
        '
        't_Path
        '
        Me.t_Path.Font = New System.Drawing.Font("Arial", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.t_Path.Location = New System.Drawing.Point(5, 69)
        Me.t_Path.Name = "t_Path"
        Me.t_Path.ReadOnly = True
        Me.t_Path.Size = New System.Drawing.Size(208, 22)
        Me.t_Path.TabIndex = 1
        '
        'Label6
        '
        Me.Label6.Font = New System.Drawing.Font("Arial", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label6.ForeColor = System.Drawing.Color.Purple
        Me.Label6.Location = New System.Drawing.Point(-2, 34)
        Me.Label6.Name = "Label6"
        Me.Label6.Size = New System.Drawing.Size(341, 32)
        Me.Label6.TabIndex = 0
        Me.Label6.Text = "Where's the new software?"
        Me.Label6.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
        '
        'p_step2_2
        '
        Me.p_step2_2.Controls.Add(Me.l_NewVersion)
        Me.p_step2_2.Controls.Add(Me.Label7)
        Me.p_step2_2.Location = New System.Drawing.Point(9, 168)
        Me.p_step2_2.Name = "p_step2_2"
        Me.p_step2_2.Size = New System.Drawing.Size(341, 144)
        Me.p_step2_2.TabIndex = 129
        Me.p_step2_2.Visible = False
        '
        'l_NewVersion
        '
        Me.l_NewVersion.Font = New System.Drawing.Font("Arial", 27.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.l_NewVersion.ForeColor = System.Drawing.Color.Teal
        Me.l_NewVersion.Location = New System.Drawing.Point(-6, 46)
        Me.l_NewVersion.Name = "l_NewVersion"
        Me.l_NewVersion.Size = New System.Drawing.Size(350, 48)
        Me.l_NewVersion.TabIndex = 128
        Me.l_NewVersion.Text = "v#.##"
        Me.l_NewVersion.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
        '
        'Label7
        '
        Me.Label7.Font = New System.Drawing.Font("Arial", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label7.ForeColor = System.Drawing.Color.Purple
        Me.Label7.Location = New System.Drawing.Point(-3, 94)
        Me.Label7.Name = "Label7"
        Me.Label7.Size = New System.Drawing.Size(347, 32)
        Me.Label7.TabIndex = 127
        Me.Label7.Text = "New version"
        Me.Label7.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
        '
        'l_FWString
        '
        Me.l_FWString.Font = New System.Drawing.Font("Arial", 27.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.l_FWString.ForeColor = System.Drawing.Color.Teal
        Me.l_FWString.Location = New System.Drawing.Point(3, 51)
        Me.l_FWString.Name = "l_FWString"
        Me.l_FWString.Size = New System.Drawing.Size(350, 48)
        Me.l_FWString.TabIndex = 127
        Me.l_FWString.Text = "v#.##"
        Me.l_FWString.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
        '
        'Label4
        '
        Me.Label4.Font = New System.Drawing.Font("Arial", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label4.ForeColor = System.Drawing.Color.Purple
        Me.Label4.Location = New System.Drawing.Point(6, 99)
        Me.Label4.Name = "Label4"
        Me.Label4.Size = New System.Drawing.Size(347, 32)
        Me.Label4.TabIndex = 126
        Me.Label4.Text = "Current version"
        Me.Label4.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
        '
        'b_Upload
        '
        Me.b_Upload.BackColor = System.Drawing.Color.Teal
        Me.b_Upload.Enabled = False
        Me.b_Upload.FlatAppearance.BorderSize = 0
        Me.b_Upload.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.b_Upload.ForeColor = System.Drawing.Color.White
        Me.b_Upload.Location = New System.Drawing.Point(9, 362)
        Me.b_Upload.Name = "b_Upload"
        Me.b_Upload.Size = New System.Drawing.Size(341, 35)
        Me.b_Upload.TabIndex = 125
        Me.b_Upload.Text = "Looks good. Upload!"
        Me.b_Upload.UseVisualStyleBackColor = False
        '
        'Label5
        '
        Me.Label5.Font = New System.Drawing.Font("Arial", 15.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label5.ForeColor = System.Drawing.Color.Teal
        Me.Label5.Location = New System.Drawing.Point(9, 9)
        Me.Label5.Name = "Label5"
        Me.Label5.Size = New System.Drawing.Size(341, 24)
        Me.Label5.TabIndex = 1
        Me.Label5.Text = "Step 2:"
        Me.Label5.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
        '
        'fwUpdater
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(7.0!, 16.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(388, 429)
        Me.Controls.Add(Me.p_Step2)
        Me.Controls.Add(Me.p_step1)
        Me.Font = New System.Drawing.Font("Arial", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog
        Me.Margin = New System.Windows.Forms.Padding(3, 4, 3, 4)
        Me.MaximizeBox = False
        Me.MinimizeBox = False
        Me.Name = "fwUpdater"
        Me.ShowInTaskbar = False
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent
        Me.Text = "Firmware Update Wizard"
        Me.p_step1.ResumeLayout(False)
        Me.p_step1.PerformLayout()
        CType(Me.PictureBox1, System.ComponentModel.ISupportInitialize).EndInit()
        Me.p_Step2.ResumeLayout(False)
        Me.p_step2_1.ResumeLayout(False)
        Me.p_step2_1.PerformLayout()
        Me.p_step2_2.ResumeLayout(False)
        Me.ResumeLayout(False)

    End Sub

    Friend WithEvents p_step1 As Panel
    Friend WithEvents PictureBox1 As PictureBox
    Friend WithEvents Label2 As Label
    Friend WithEvents Label1 As Label
    Friend WithEvents Label3 As Label
    Friend WithEvents b_Step1 As Button
    Friend WithEvents p_Step2 As Panel
    Friend WithEvents b_Upload As Button
    Friend WithEvents Label5 As Label
    Friend WithEvents Label6 As Label
    Friend WithEvents l_FWString As Label
    Friend WithEvents Label4 As Label
    Friend WithEvents p_step2_1 As Panel
    Friend WithEvents t_Path As TextBox
    Friend WithEvents b_Browse As Button
    Friend WithEvents p_step2_2 As Panel
    Friend WithEvents l_NewVersion As Label
    Friend WithEvents Label7 As Label
End Class
