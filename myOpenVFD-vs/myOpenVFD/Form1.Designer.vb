<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()>
Partial Class OpenVFD
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()>
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
    <System.Diagnostics.DebuggerStepThrough()>
    Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container()
        Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(OpenVFD))
        Me.GroupBox3 = New System.Windows.Forms.GroupBox()
        Me.b_Connect = New System.Windows.Forms.Button()
        Me.l_connect = New System.Windows.Forms.Label()
        Me.txt_SerialPorts = New System.Windows.Forms.ComboBox()
        Me.Label2 = New System.Windows.Forms.Label()
        Me.comPort = New System.IO.Ports.SerialPort(Me.components)
        Me.p_SetColors = New System.Windows.Forms.Panel()
        Me.check_LED1 = New System.Windows.Forms.CheckBox()
        Me.check_LED2 = New System.Windows.Forms.CheckBox()
        Me.check_LED3 = New System.Windows.Forms.CheckBox()
        Me.check_LED4 = New System.Windows.Forms.CheckBox()
        Me.check_LED5 = New System.Windows.Forms.CheckBox()
        Me.check_LED6 = New System.Windows.Forms.CheckBox()
        Me.Label3 = New System.Windows.Forms.Label()
        Me.ledEditor = New Cyotek.Windows.Forms.ColorEditor()
        Me.ledWheel = New Cyotek.Windows.Forms.ColorWheel()
        Me.ledManager = New Cyotek.Windows.Forms.ColorEditorManager()
        Me.p_Msg = New System.Windows.Forms.Panel()
        Me.cb_customWelcome = New System.Windows.Forms.CheckBox()
        Me.b_Msg = New System.Windows.Forms.Button()
        Me.Label6 = New System.Windows.Forms.Label()
        Me.txt_DisplayLength = New FilterTextBox.FilterTextBox()
        Me.Label5 = New System.Windows.Forms.Label()
        Me.Label4 = New System.Windows.Forms.Label()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.txt_Msg = New FilterTextBox.FilterTextBox()
        Me.p_TimeSync = New System.Windows.Forms.Panel()
        Me.l_Date = New System.Windows.Forms.Label()
        Me.l_Time = New System.Windows.Forms.Label()
        Me.b_Sync = New System.Windows.Forms.Button()
        Me.l_SyncStatus = New System.Windows.Forms.Label()
        Me.Label7 = New System.Windows.Forms.Label()
        Me.p_Presets = New System.Windows.Forms.Panel()
        Me.tab_ColorPresets = New System.Windows.Forms.TabControl()
        Me.tab_Static = New System.Windows.Forms.TabPage()
        Me.pre_purple = New System.Windows.Forms.Label()
        Me.pre_magenta = New System.Windows.Forms.Label()
        Me.pre_cyan = New System.Windows.Forms.Label()
        Me.pre_orange = New System.Windows.Forms.Label()
        Me.pre_yellow = New System.Windows.Forms.Label()
        Me.pre_blue = New System.Windows.Forms.Label()
        Me.pre_green = New System.Windows.Forms.Label()
        Me.pre_red = New System.Windows.Forms.Label()
        Me.pre_warmwhite = New System.Windows.Forms.Label()
        Me.pre_white = New System.Windows.Forms.Label()
        Me.pre_off = New System.Windows.Forms.Label()
        Me.pre_pastel = New System.Windows.Forms.PictureBox()
        Me.pre_green2blue = New System.Windows.Forms.PictureBox()
        Me.pre_red2blue = New System.Windows.Forms.PictureBox()
        Me.pre_red2green = New System.Windows.Forms.PictureBox()
        Me.pre_Rainbow = New System.Windows.Forms.PictureBox()
        Me.Label8 = New System.Windows.Forms.Label()
        Me.tab_Dynamic = New System.Windows.Forms.TabPage()
        Me.p_LEDMic = New System.Windows.Forms.Panel()
        Me.Label31 = New System.Windows.Forms.Label()
        Me.Label20 = New System.Windows.Forms.Label()
        Me.dancingTrackBar = New System.Windows.Forms.TrackBar()
        Me.b_Mic = New System.Windows.Forms.Button()
        Me.Label21 = New System.Windows.Forms.Label()
        Me.Label10 = New System.Windows.Forms.Label()
        Me.p_LEDs = New System.Windows.Forms.Panel()
        Me.note = New System.Windows.Forms.PictureBox()
        Me.speaker = New System.Windows.Forms.PictureBox()
        Me.Label9 = New System.Windows.Forms.Label()
        Me.bS_Cop = New System.Windows.Forms.Button()
        Me.bS_Mic = New System.Windows.Forms.Button()
        Me.bS_RCode = New System.Windows.Forms.Button()
        Me.bS_Chase = New System.Windows.Forms.Button()
        Me.bS_Cross = New System.Windows.Forms.Button()
        Me.bS_Spectrum = New System.Windows.Forms.Button()
        Me.p_LEDCross = New System.Windows.Forms.Panel()
        Me.Label11 = New System.Windows.Forms.Label()
        Me.Label12 = New System.Windows.Forms.Label()
        Me.crossTrackBar = New System.Windows.Forms.TrackBar()
        Me.Label32 = New System.Windows.Forms.Label()
        Me.b_Cross = New System.Windows.Forms.Button()
        Me.p_LEDSpectrum = New System.Windows.Forms.Panel()
        Me.Label22 = New System.Windows.Forms.Label()
        Me.b_Spectrum = New System.Windows.Forms.Button()
        Me.p_LEDCop = New System.Windows.Forms.Panel()
        Me.b_Cop = New System.Windows.Forms.Button()
        Me.cb_CopMode = New System.Windows.Forms.ComboBox()
        Me.Label25 = New System.Windows.Forms.Label()
        Me.Label26 = New System.Windows.Forms.Label()
        Me.p_LEDResistorCode = New System.Windows.Forms.Panel()
        Me.p_Resistors = New System.Windows.Forms.Panel()
        Me.LinkLabel1 = New System.Windows.Forms.LinkLabel()
        Me.Label15 = New System.Windows.Forms.Label()
        Me.p_ResistorsMore = New System.Windows.Forms.Panel()
        Me.LinkLabel2 = New System.Windows.Forms.LinkLabel()
        Me.Label24 = New System.Windows.Forms.Label()
        Me.Label19 = New System.Windows.Forms.Label()
        Me.Label18 = New System.Windows.Forms.Label()
        Me.Label17 = New System.Windows.Forms.Label()
        Me.Label16 = New System.Windows.Forms.Label()
        Me.PictureBox1 = New System.Windows.Forms.PictureBox()
        Me.b_Resistor = New System.Windows.Forms.Button()
        Me.p_LEDChaseFade = New System.Windows.Forms.Panel()
        Me.b_ChaseFade = New System.Windows.Forms.Button()
        Me.cb_ChFdMode = New System.Windows.Forms.ComboBox()
        Me.l_chFdInfo = New System.Windows.Forms.Label()
        Me.Label14 = New System.Windows.Forms.Label()
        Me.tab_Ambient = New System.Windows.Forms.TabPage()
        Me.l_MoodStatus = New System.Windows.Forms.Label()
        Me.Label30 = New System.Windows.Forms.Label()
        Me.p_Mood = New System.Windows.Forms.Panel()
        Me.pImage = New System.Windows.Forms.PictureBox()
        Me.PictureBox2 = New System.Windows.Forms.PictureBox()
        Me.b_SetColors = New System.Windows.Forms.Button()
        Me.b_SendMsg = New System.Windows.Forms.Button()
        Me.b_TimeSync = New System.Windows.Forms.Button()
        Me.b_Presets = New System.Windows.Forms.Button()
        Me.timeTimer = New System.Windows.Forms.Timer(Me.components)
        Me.ledTimer = New System.Windows.Forms.Timer(Me.components)
        Me.ledSubTimer = New System.Windows.Forms.Timer(Me.components)
        Me.b_SaveSettings = New System.Windows.Forms.Button()
        Me.p_SaveSettings = New System.Windows.Forms.Panel()
        Me.timeNightShiftEnable = New System.Windows.Forms.DateTimePicker()
        Me.GroupBox2 = New System.Windows.Forms.GroupBox()
        Me.Label33 = New System.Windows.Forms.Label()
        Me.cb_scheduledNightShift = New System.Windows.Forms.CheckBox()
        Me.cb_nightShiftToggle = New System.Windows.Forms.CheckBox()
        Me.GroupBox1 = New System.Windows.Forms.GroupBox()
        Me.cb_12h = New System.Windows.Forms.CheckBox()
        Me.cb_intd = New System.Windows.Forms.CheckBox()
        Me.l_Reset = New System.Windows.Forms.Label()
        Me.l_Save2 = New System.Windows.Forms.Label()
        Me.l_Save1 = New System.Windows.Forms.Label()
        Me.Label13 = New System.Windows.Forms.Label()
        Me.Panel2 = New System.Windows.Forms.Panel()
        Me.Label23 = New System.Windows.Forms.Label()
        Me.b_About = New System.Windows.Forms.Button()
        Me.p_About = New System.Windows.Forms.Panel()
        Me.Button1 = New System.Windows.Forms.Button()
        Me.b_UserManual = New System.Windows.Forms.Button()
        Me.b_UpdateFirmware = New System.Windows.Forms.Button()
        Me.b_UpdateCheck = New System.Windows.Forms.Button()
        Me.l_FWString = New System.Windows.Forms.Label()
        Me.Label29 = New System.Windows.Forms.Label()
        Me.l_myVersion = New System.Windows.Forms.Label()
        Me.Label28 = New System.Windows.Forms.Label()
        Me.Label27 = New System.Windows.Forms.Label()
        Me.p_Buttons = New System.Windows.Forms.Panel()
        Me.scrTimer = New System.Windows.Forms.Timer(Me.components)
        Me.scrSimulator = New System.Windows.Forms.Timer(Me.components)
        Me.timeNightShiftDisable = New System.Windows.Forms.DateTimePicker()
        Me.Label34 = New System.Windows.Forms.Label()
        Me.cb_leadingZero = New System.Windows.Forms.CheckBox()
        Me.Label35 = New System.Windows.Forms.Label()
        Me.GroupBox3.SuspendLayout()
        Me.p_SetColors.SuspendLayout()
        Me.p_Msg.SuspendLayout()
        Me.p_TimeSync.SuspendLayout()
        Me.p_Presets.SuspendLayout()
        Me.tab_ColorPresets.SuspendLayout()
        Me.tab_Static.SuspendLayout()
        CType(Me.pre_pastel, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.pre_green2blue, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.pre_red2blue, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.pre_red2green, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.pre_Rainbow, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.tab_Dynamic.SuspendLayout()
        Me.p_LEDMic.SuspendLayout()
        CType(Me.dancingTrackBar, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.p_LEDs.SuspendLayout()
        CType(Me.note, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.speaker, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.p_LEDCross.SuspendLayout()
        CType(Me.crossTrackBar, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.p_LEDSpectrum.SuspendLayout()
        Me.p_LEDCop.SuspendLayout()
        Me.p_LEDResistorCode.SuspendLayout()
        Me.p_Resistors.SuspendLayout()
        Me.p_ResistorsMore.SuspendLayout()
        CType(Me.PictureBox1, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.p_LEDChaseFade.SuspendLayout()
        Me.tab_Ambient.SuspendLayout()
        CType(Me.pImage, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.PictureBox2, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.p_SaveSettings.SuspendLayout()
        Me.GroupBox2.SuspendLayout()
        Me.GroupBox1.SuspendLayout()
        Me.Panel2.SuspendLayout()
        Me.p_About.SuspendLayout()
        Me.p_Buttons.SuspendLayout()
        Me.SuspendLayout()
        '
        'GroupBox3
        '
        Me.GroupBox3.Controls.Add(Me.b_Connect)
        Me.GroupBox3.Controls.Add(Me.l_connect)
        Me.GroupBox3.Controls.Add(Me.txt_SerialPorts)
        Me.GroupBox3.Controls.Add(Me.Label2)
        Me.GroupBox3.Font = New System.Drawing.Font("Arial", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.GroupBox3.ForeColor = System.Drawing.Color.Purple
        Me.GroupBox3.Location = New System.Drawing.Point(14, 15)
        Me.GroupBox3.Margin = New System.Windows.Forms.Padding(3, 4, 3, 4)
        Me.GroupBox3.Name = "GroupBox3"
        Me.GroupBox3.Padding = New System.Windows.Forms.Padding(3, 4, 3, 4)
        Me.GroupBox3.Size = New System.Drawing.Size(241, 102)
        Me.GroupBox3.TabIndex = 97
        Me.GroupBox3.TabStop = False
        Me.GroupBox3.Text = "Connect to OpenVFD"
        '
        'b_Connect
        '
        Me.b_Connect.BackColor = System.Drawing.Color.Teal
        Me.b_Connect.FlatAppearance.BorderSize = 0
        Me.b_Connect.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.b_Connect.Font = New System.Drawing.Font("Arial", 15.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.b_Connect.ForeColor = System.Drawing.Color.White
        Me.b_Connect.Location = New System.Drawing.Point(158, 21)
        Me.b_Connect.Name = "b_Connect"
        Me.b_Connect.Size = New System.Drawing.Size(73, 57)
        Me.b_Connect.TabIndex = 125
        Me.b_Connect.Text = "►"
        Me.b_Connect.UseVisualStyleBackColor = False
        '
        'l_connect
        '
        Me.l_connect.Font = New System.Drawing.Font("Arial", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.l_connect.Location = New System.Drawing.Point(151, 81)
        Me.l_connect.Name = "l_connect"
        Me.l_connect.Size = New System.Drawing.Size(85, 16)
        Me.l_connect.TabIndex = 93
        Me.l_connect.Text = "Connect"
        Me.l_connect.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
        '
        'txt_SerialPorts
        '
        Me.txt_SerialPorts.BackColor = System.Drawing.Color.White
        Me.txt_SerialPorts.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList
        Me.txt_SerialPorts.Font = New System.Drawing.Font("Trebuchet MS", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.txt_SerialPorts.FormattingEnabled = True
        Me.txt_SerialPorts.Location = New System.Drawing.Point(12, 52)
        Me.txt_SerialPorts.Margin = New System.Windows.Forms.Padding(3, 4, 3, 4)
        Me.txt_SerialPorts.Name = "txt_SerialPorts"
        Me.txt_SerialPorts.Size = New System.Drawing.Size(124, 26)
        Me.txt_SerialPorts.TabIndex = 2
        '
        'Label2
        '
        Me.Label2.AutoSize = True
        Me.Label2.Font = New System.Drawing.Font("Arial", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label2.Location = New System.Drawing.Point(8, 26)
        Me.Label2.Name = "Label2"
        Me.Label2.Size = New System.Drawing.Size(131, 16)
        Me.Label2.TabIndex = 84
        Me.Label2.Text = "Where's OpenVFD?"
        '
        'comPort
        '
        Me.comPort.BaudRate = 115200
        '
        'p_SetColors
        '
        Me.p_SetColors.Controls.Add(Me.check_LED1)
        Me.p_SetColors.Controls.Add(Me.check_LED2)
        Me.p_SetColors.Controls.Add(Me.check_LED3)
        Me.p_SetColors.Controls.Add(Me.check_LED4)
        Me.p_SetColors.Controls.Add(Me.check_LED5)
        Me.p_SetColors.Controls.Add(Me.check_LED6)
        Me.p_SetColors.Controls.Add(Me.Label3)
        Me.p_SetColors.Controls.Add(Me.ledEditor)
        Me.p_SetColors.Controls.Add(Me.ledWheel)
        Me.p_SetColors.Location = New System.Drawing.Point(0, 130)
        Me.p_SetColors.Name = "p_SetColors"
        Me.p_SetColors.Size = New System.Drawing.Size(784, 331)
        Me.p_SetColors.TabIndex = 104
        Me.p_SetColors.Visible = False
        '
        'check_LED1
        '
        Me.check_LED1.AutoSize = True
        Me.check_LED1.Checked = True
        Me.check_LED1.CheckState = System.Windows.Forms.CheckState.Checked
        Me.check_LED1.ForeColor = System.Drawing.Color.Purple
        Me.check_LED1.Location = New System.Drawing.Point(25, 241)
        Me.check_LED1.Name = "check_LED1"
        Me.check_LED1.Size = New System.Drawing.Size(63, 20)
        Me.check_LED1.TabIndex = 117
        Me.check_LED1.Text = "LED 1"
        Me.check_LED1.UseVisualStyleBackColor = True
        '
        'check_LED2
        '
        Me.check_LED2.AutoSize = True
        Me.check_LED2.Checked = True
        Me.check_LED2.CheckState = System.Windows.Forms.CheckState.Checked
        Me.check_LED2.ForeColor = System.Drawing.Color.Purple
        Me.check_LED2.Location = New System.Drawing.Point(25, 215)
        Me.check_LED2.Name = "check_LED2"
        Me.check_LED2.Size = New System.Drawing.Size(63, 20)
        Me.check_LED2.TabIndex = 116
        Me.check_LED2.Text = "LED 2"
        Me.check_LED2.UseVisualStyleBackColor = True
        '
        'check_LED3
        '
        Me.check_LED3.AutoSize = True
        Me.check_LED3.Checked = True
        Me.check_LED3.CheckState = System.Windows.Forms.CheckState.Checked
        Me.check_LED3.ForeColor = System.Drawing.Color.Purple
        Me.check_LED3.Location = New System.Drawing.Point(25, 189)
        Me.check_LED3.Name = "check_LED3"
        Me.check_LED3.Size = New System.Drawing.Size(63, 20)
        Me.check_LED3.TabIndex = 115
        Me.check_LED3.Text = "LED 3"
        Me.check_LED3.UseVisualStyleBackColor = True
        '
        'check_LED4
        '
        Me.check_LED4.AutoSize = True
        Me.check_LED4.Checked = True
        Me.check_LED4.CheckState = System.Windows.Forms.CheckState.Checked
        Me.check_LED4.ForeColor = System.Drawing.Color.Purple
        Me.check_LED4.Location = New System.Drawing.Point(25, 163)
        Me.check_LED4.Name = "check_LED4"
        Me.check_LED4.Size = New System.Drawing.Size(63, 20)
        Me.check_LED4.TabIndex = 114
        Me.check_LED4.Text = "LED 4"
        Me.check_LED4.UseVisualStyleBackColor = True
        '
        'check_LED5
        '
        Me.check_LED5.AutoSize = True
        Me.check_LED5.Checked = True
        Me.check_LED5.CheckState = System.Windows.Forms.CheckState.Checked
        Me.check_LED5.ForeColor = System.Drawing.Color.Purple
        Me.check_LED5.Location = New System.Drawing.Point(25, 137)
        Me.check_LED5.Name = "check_LED5"
        Me.check_LED5.Size = New System.Drawing.Size(63, 20)
        Me.check_LED5.TabIndex = 113
        Me.check_LED5.Text = "LED 5"
        Me.check_LED5.UseVisualStyleBackColor = True
        '
        'check_LED6
        '
        Me.check_LED6.AutoSize = True
        Me.check_LED6.Checked = True
        Me.check_LED6.CheckState = System.Windows.Forms.CheckState.Checked
        Me.check_LED6.ForeColor = System.Drawing.Color.Purple
        Me.check_LED6.Location = New System.Drawing.Point(25, 111)
        Me.check_LED6.Name = "check_LED6"
        Me.check_LED6.Size = New System.Drawing.Size(63, 20)
        Me.check_LED6.TabIndex = 112
        Me.check_LED6.Text = "LED 6"
        Me.check_LED6.UseVisualStyleBackColor = True
        '
        'Label3
        '
        Me.Label3.AutoSize = True
        Me.Label3.Font = New System.Drawing.Font("Arial", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label3.ForeColor = System.Drawing.Color.Teal
        Me.Label3.Location = New System.Drawing.Point(22, 42)
        Me.Label3.Name = "Label3"
        Me.Label3.Size = New System.Drawing.Size(165, 32)
        Me.Label3.TabIndex = 111
        Me.Label3.Text = "Let's paint t̶h̶e̶ ̶n̶i̶g̶h̶t̶ ̶s̶k̶y̶ " & Global.Microsoft.VisualBasic.ChrW(13) & Global.Microsoft.VisualBasic.ChrW(10) & "                     some LEDs!"
        '
        'ledEditor
        '
        Me.ledEditor.Location = New System.Drawing.Point(487, 21)
        Me.ledEditor.Margin = New System.Windows.Forms.Padding(3, 4, 3, 4)
        Me.ledEditor.Name = "ledEditor"
        Me.ledEditor.Size = New System.Drawing.Size(275, 289)
        Me.ledEditor.TabIndex = 110
        '
        'ledWheel
        '
        Me.ledWheel.Color = System.Drawing.Color.FromArgb(CType(CType(0, Byte), Integer), CType(CType(0, Byte), Integer), CType(CType(0, Byte), Integer))
        Me.ledWheel.Location = New System.Drawing.Point(172, 21)
        Me.ledWheel.Name = "ledWheel"
        Me.ledWheel.Size = New System.Drawing.Size(309, 289)
        Me.ledWheel.TabIndex = 109
        '
        'ledManager
        '
        Me.ledManager.ColorEditor = Me.ledEditor
        Me.ledManager.ColorWheel = Me.ledWheel
        '
        'p_Msg
        '
        Me.p_Msg.Controls.Add(Me.cb_customWelcome)
        Me.p_Msg.Controls.Add(Me.b_Msg)
        Me.p_Msg.Controls.Add(Me.Label6)
        Me.p_Msg.Controls.Add(Me.txt_DisplayLength)
        Me.p_Msg.Controls.Add(Me.Label5)
        Me.p_Msg.Controls.Add(Me.Label4)
        Me.p_Msg.Controls.Add(Me.Label1)
        Me.p_Msg.Controls.Add(Me.txt_Msg)
        Me.p_Msg.Location = New System.Drawing.Point(0, 130)
        Me.p_Msg.Name = "p_Msg"
        Me.p_Msg.Size = New System.Drawing.Size(784, 331)
        Me.p_Msg.TabIndex = 114
        Me.p_Msg.Visible = False
        '
        'cb_customWelcome
        '
        Me.cb_customWelcome.AutoSize = True
        Me.cb_customWelcome.Location = New System.Drawing.Point(25, 229)
        Me.cb_customWelcome.Name = "cb_customWelcome"
        Me.cb_customWelcome.Size = New System.Drawing.Size(520, 20)
        Me.cb_customWelcome.TabIndex = 125
        Me.cb_customWelcome.Text = "Customize welcome message. Enable this option to set a custom welcome message"
        Me.cb_customWelcome.UseVisualStyleBackColor = True
        '
        'b_Msg
        '
        Me.b_Msg.BackColor = System.Drawing.Color.Teal
        Me.b_Msg.FlatAppearance.BorderSize = 0
        Me.b_Msg.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.b_Msg.Font = New System.Drawing.Font("Arial", 27.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.b_Msg.ForeColor = System.Drawing.Color.White
        Me.b_Msg.Location = New System.Drawing.Point(324, 258)
        Me.b_Msg.Name = "b_Msg"
        Me.b_Msg.Size = New System.Drawing.Size(433, 52)
        Me.b_Msg.TabIndex = 124
        Me.b_Msg.Text = "Send Message"
        Me.b_Msg.UseVisualStyleBackColor = False
        '
        'Label6
        '
        Me.Label6.AutoSize = True
        Me.Label6.Font = New System.Drawing.Font("Arial", 27.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label6.ForeColor = System.Drawing.Color.Purple
        Me.Label6.Location = New System.Drawing.Point(80, 263)
        Me.Label6.Name = "Label6"
        Me.Label6.Size = New System.Drawing.Size(201, 44)
        Me.Label6.TabIndex = 5
        Me.Label6.Text = "Second(s)"
        '
        'txt_DisplayLength
        '
        Me.txt_DisplayLength.BeepOnError = False
        Me.txt_DisplayLength.CustomFilter = Nothing
        Me.txt_DisplayLength.Filtermode = FilterTextBox.FilterModus.NumericDecimal
        Me.txt_DisplayLength.Font = New System.Drawing.Font("Arial", 27.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.txt_DisplayLength.ForeColor = System.Drawing.Color.Purple
        Me.txt_DisplayLength.Location = New System.Drawing.Point(26, 260)
        Me.txt_DisplayLength.MaxLength = 1
        Me.txt_DisplayLength.Name = "txt_DisplayLength"
        Me.txt_DisplayLength.Size = New System.Drawing.Size(48, 50)
        Me.txt_DisplayLength.TabIndex = 4
        Me.txt_DisplayLength.Text = "1"
        Me.txt_DisplayLength.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'Label5
        '
        Me.Label5.AutoSize = True
        Me.Label5.Location = New System.Drawing.Point(23, 161)
        Me.Label5.Name = "Label5"
        Me.Label5.Size = New System.Drawing.Size(528, 64)
        Me.Label5.TabIndex = 3
        Me.Label5.Text = resources.GetString("Label5.Text")
        '
        'Label4
        '
        Me.Label4.AutoSize = True
        Me.Label4.Font = New System.Drawing.Font("Arial", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label4.Location = New System.Drawing.Point(23, 143)
        Me.Label4.Name = "Label4"
        Me.Label4.Size = New System.Drawing.Size(127, 16)
        Me.Label4.TabIndex = 2
        Me.Label4.Text = "Rules of the game:"
        '
        'Label1
        '
        Me.Label1.AutoSize = True
        Me.Label1.Location = New System.Drawing.Point(23, 26)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(305, 16)
        Me.Label1.TabIndex = 1
        Me.Label1.Text = "Write a poem... Or something lovely. Just be kind :)"
        '
        'txt_Msg
        '
        Me.txt_Msg.BeepOnError = False
        Me.txt_Msg.CustomFilter = "ABCDEFGHIJLNOPQRSTUVY .-0"
        Me.txt_Msg.Filtermode = FilterTextBox.FilterModus.CustomFilter
        Me.txt_Msg.Font = New System.Drawing.Font("Arial", 48.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.txt_Msg.ForeColor = System.Drawing.Color.Teal
        Me.txt_Msg.Location = New System.Drawing.Point(26, 50)
        Me.txt_Msg.MaxLength = 18
        Me.txt_Msg.Name = "txt_Msg"
        Me.txt_Msg.Size = New System.Drawing.Size(731, 81)
        Me.txt_Msg.TabIndex = 0
        Me.txt_Msg.Text = "HAILEESTEIN FELD  "
        Me.txt_Msg.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'p_TimeSync
        '
        Me.p_TimeSync.Controls.Add(Me.l_Date)
        Me.p_TimeSync.Controls.Add(Me.l_Time)
        Me.p_TimeSync.Controls.Add(Me.b_Sync)
        Me.p_TimeSync.Controls.Add(Me.l_SyncStatus)
        Me.p_TimeSync.Controls.Add(Me.Label7)
        Me.p_TimeSync.Location = New System.Drawing.Point(0, 130)
        Me.p_TimeSync.Name = "p_TimeSync"
        Me.p_TimeSync.Size = New System.Drawing.Size(784, 331)
        Me.p_TimeSync.TabIndex = 115
        Me.p_TimeSync.Visible = False
        '
        'l_Date
        '
        Me.l_Date.AutoSize = True
        Me.l_Date.Font = New System.Drawing.Font("Arial", 21.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.l_Date.Location = New System.Drawing.Point(51, 186)
        Me.l_Date.Name = "l_Date"
        Me.l_Date.Size = New System.Drawing.Size(376, 34)
        Me.l_Date.TabIndex = 127
        Me.l_Date.Text = "Sat - September 1st - 2017"
        '
        'l_Time
        '
        Me.l_Time.AutoSize = True
        Me.l_Time.Font = New System.Drawing.Font("Arial", 80.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.l_Time.ForeColor = System.Drawing.Color.Purple
        Me.l_Time.Location = New System.Drawing.Point(36, 63)
        Me.l_Time.Name = "l_Time"
        Me.l_Time.Size = New System.Drawing.Size(482, 124)
        Me.l_Time.TabIndex = 126
        Me.l_Time.Text = "00:00:00"
        '
        'b_Sync
        '
        Me.b_Sync.BackColor = System.Drawing.Color.Teal
        Me.b_Sync.FlatAppearance.BorderSize = 0
        Me.b_Sync.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.b_Sync.Font = New System.Drawing.Font("Arial", 27.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.b_Sync.ForeColor = System.Drawing.Color.White
        Me.b_Sync.Location = New System.Drawing.Point(578, 94)
        Me.b_Sync.Name = "b_Sync"
        Me.b_Sync.Size = New System.Drawing.Size(176, 126)
        Me.b_Sync.TabIndex = 125
        Me.b_Sync.Text = "Sync"
        Me.b_Sync.UseVisualStyleBackColor = False
        '
        'l_SyncStatus
        '
        Me.l_SyncStatus.AutoSize = True
        Me.l_SyncStatus.Font = New System.Drawing.Font("Arial", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.l_SyncStatus.Location = New System.Drawing.Point(588, 241)
        Me.l_SyncStatus.Name = "l_SyncStatus"
        Me.l_SyncStatus.Size = New System.Drawing.Size(100, 16)
        Me.l_SyncStatus.TabIndex = 119
        Me.l_SyncStatus.Text = "Ready to sync."
        '
        'Label7
        '
        Me.Label7.AutoSize = True
        Me.Label7.Font = New System.Drawing.Font("Arial", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label7.Location = New System.Drawing.Point(575, 50)
        Me.Label7.Name = "Label7"
        Me.Label7.Size = New System.Drawing.Size(179, 32)
        Me.Label7.TabIndex = 1
        Me.Label7.Text = "Sync OpenVFD to the time " & Global.Microsoft.VisualBasic.ChrW(13) & Global.Microsoft.VisualBasic.ChrW(10) & "and date of your computer."
        '
        'p_Presets
        '
        Me.p_Presets.Controls.Add(Me.tab_ColorPresets)
        Me.p_Presets.Location = New System.Drawing.Point(0, 130)
        Me.p_Presets.Name = "p_Presets"
        Me.p_Presets.Size = New System.Drawing.Size(784, 331)
        Me.p_Presets.TabIndex = 120
        Me.p_Presets.Visible = False
        '
        'tab_ColorPresets
        '
        Me.tab_ColorPresets.Controls.Add(Me.tab_Static)
        Me.tab_ColorPresets.Controls.Add(Me.tab_Dynamic)
        Me.tab_ColorPresets.Controls.Add(Me.tab_Ambient)
        Me.tab_ColorPresets.Dock = System.Windows.Forms.DockStyle.Fill
        Me.tab_ColorPresets.Font = New System.Drawing.Font("Arial", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.tab_ColorPresets.Location = New System.Drawing.Point(0, 0)
        Me.tab_ColorPresets.Name = "tab_ColorPresets"
        Me.tab_ColorPresets.SelectedIndex = 0
        Me.tab_ColorPresets.Size = New System.Drawing.Size(784, 331)
        Me.tab_ColorPresets.TabIndex = 0
        '
        'tab_Static
        '
        Me.tab_Static.BackColor = System.Drawing.Color.White
        Me.tab_Static.Controls.Add(Me.pre_purple)
        Me.tab_Static.Controls.Add(Me.pre_magenta)
        Me.tab_Static.Controls.Add(Me.pre_cyan)
        Me.tab_Static.Controls.Add(Me.pre_orange)
        Me.tab_Static.Controls.Add(Me.pre_yellow)
        Me.tab_Static.Controls.Add(Me.pre_blue)
        Me.tab_Static.Controls.Add(Me.pre_green)
        Me.tab_Static.Controls.Add(Me.pre_red)
        Me.tab_Static.Controls.Add(Me.pre_warmwhite)
        Me.tab_Static.Controls.Add(Me.pre_white)
        Me.tab_Static.Controls.Add(Me.pre_off)
        Me.tab_Static.Controls.Add(Me.pre_pastel)
        Me.tab_Static.Controls.Add(Me.pre_green2blue)
        Me.tab_Static.Controls.Add(Me.pre_red2blue)
        Me.tab_Static.Controls.Add(Me.pre_red2green)
        Me.tab_Static.Controls.Add(Me.pre_Rainbow)
        Me.tab_Static.Controls.Add(Me.Label8)
        Me.tab_Static.ForeColor = System.Drawing.Color.FromArgb(CType(CType(32, Byte), Integer), CType(CType(32, Byte), Integer), CType(CType(32, Byte), Integer))
        Me.tab_Static.Location = New System.Drawing.Point(4, 25)
        Me.tab_Static.Name = "tab_Static"
        Me.tab_Static.Padding = New System.Windows.Forms.Padding(3)
        Me.tab_Static.Size = New System.Drawing.Size(776, 302)
        Me.tab_Static.TabIndex = 0
        Me.tab_Static.Text = "Static Presets"
        '
        'pre_purple
        '
        Me.pre_purple.AutoSize = True
        Me.pre_purple.Font = New System.Drawing.Font("Arial", 20.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.pre_purple.ForeColor = System.Drawing.Color.DarkViolet
        Me.pre_purple.Location = New System.Drawing.Point(581, 61)
        Me.pre_purple.Name = "pre_purple"
        Me.pre_purple.Size = New System.Drawing.Size(115, 32)
        Me.pre_purple.TabIndex = 16
        Me.pre_purple.Text = "#purple"
        '
        'pre_magenta
        '
        Me.pre_magenta.AutoSize = True
        Me.pre_magenta.Font = New System.Drawing.Font("Arial", 20.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.pre_magenta.ForeColor = System.Drawing.Color.Magenta
        Me.pre_magenta.Location = New System.Drawing.Point(436, 13)
        Me.pre_magenta.Name = "pre_magenta"
        Me.pre_magenta.Size = New System.Drawing.Size(144, 32)
        Me.pre_magenta.TabIndex = 15
        Me.pre_magenta.Text = "#magenta"
        '
        'pre_cyan
        '
        Me.pre_cyan.AutoSize = True
        Me.pre_cyan.Font = New System.Drawing.Font("Arial", 20.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.pre_cyan.ForeColor = System.Drawing.Color.FromArgb(CType(CType(0, Byte), Integer), CType(CType(200, Byte), Integer), CType(CType(180, Byte), Integer))
        Me.pre_cyan.Location = New System.Drawing.Point(518, 184)
        Me.pre_cyan.Name = "pre_cyan"
        Me.pre_cyan.Size = New System.Drawing.Size(92, 32)
        Me.pre_cyan.TabIndex = 14
        Me.pre_cyan.Text = "#cyan"
        '
        'pre_orange
        '
        Me.pre_orange.AutoSize = True
        Me.pre_orange.Font = New System.Drawing.Font("Arial", 20.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.pre_orange.ForeColor = System.Drawing.Color.Orange
        Me.pre_orange.Location = New System.Drawing.Point(61, 204)
        Me.pre_orange.Name = "pre_orange"
        Me.pre_orange.Size = New System.Drawing.Size(122, 32)
        Me.pre_orange.TabIndex = 13
        Me.pre_orange.Text = "#orange"
        '
        'pre_yellow
        '
        Me.pre_yellow.AutoSize = True
        Me.pre_yellow.Font = New System.Drawing.Font("Arial", 20.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.pre_yellow.ForeColor = System.Drawing.Color.Gold
        Me.pre_yellow.Location = New System.Drawing.Point(137, 149)
        Me.pre_yellow.Name = "pre_yellow"
        Me.pre_yellow.Size = New System.Drawing.Size(114, 32)
        Me.pre_yellow.TabIndex = 12
        Me.pre_yellow.Text = "#yellow"
        '
        'pre_blue
        '
        Me.pre_blue.AutoSize = True
        Me.pre_blue.Font = New System.Drawing.Font("Arial", 20.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.pre_blue.ForeColor = System.Drawing.Color.FromArgb(CType(CType(0, Byte), Integer), CType(CType(100, Byte), Integer), CType(CType(220, Byte), Integer))
        Me.pre_blue.Location = New System.Drawing.Point(671, 201)
        Me.pre_blue.Name = "pre_blue"
        Me.pre_blue.Size = New System.Drawing.Size(87, 32)
        Me.pre_blue.TabIndex = 11
        Me.pre_blue.Text = "#blue"
        '
        'pre_green
        '
        Me.pre_green.AutoSize = True
        Me.pre_green.Font = New System.Drawing.Font("Arial", 20.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.pre_green.ForeColor = System.Drawing.Color.FromArgb(CType(CType(0, Byte), Integer), CType(CType(170, Byte), Integer), CType(CType(0, Byte), Integer))
        Me.pre_green.Location = New System.Drawing.Point(333, 195)
        Me.pre_green.Name = "pre_green"
        Me.pre_green.Size = New System.Drawing.Size(105, 32)
        Me.pre_green.TabIndex = 10
        Me.pre_green.Text = "#green"
        '
        'pre_red
        '
        Me.pre_red.AutoSize = True
        Me.pre_red.Font = New System.Drawing.Font("Arial", 20.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.pre_red.ForeColor = System.Drawing.Color.FromArgb(CType(CType(192, Byte), Integer), CType(CType(0, Byte), Integer), CType(CType(0, Byte), Integer))
        Me.pre_red.Location = New System.Drawing.Point(36, 120)
        Me.pre_red.Name = "pre_red"
        Me.pre_red.Size = New System.Drawing.Size(73, 32)
        Me.pre_red.TabIndex = 9
        Me.pre_red.Text = "#red"
        '
        'pre_warmwhite
        '
        Me.pre_warmwhite.AutoSize = True
        Me.pre_warmwhite.Font = New System.Drawing.Font("Arial", 20.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.pre_warmwhite.ForeColor = System.Drawing.Color.Khaki
        Me.pre_warmwhite.Location = New System.Drawing.Point(180, 238)
        Me.pre_warmwhite.Name = "pre_warmwhite"
        Me.pre_warmwhite.Size = New System.Drawing.Size(173, 32)
        Me.pre_warmwhite.TabIndex = 8
        Me.pre_warmwhite.Text = "#warmwhite"
        '
        'pre_white
        '
        Me.pre_white.AutoSize = True
        Me.pre_white.Font = New System.Drawing.Font("Arial", 20.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.pre_white.ForeColor = System.Drawing.Color.DarkGray
        Me.pre_white.Location = New System.Drawing.Point(16, 265)
        Me.pre_white.Name = "pre_white"
        Me.pre_white.Size = New System.Drawing.Size(100, 32)
        Me.pre_white.TabIndex = 7
        Me.pre_white.Text = "#white"
        '
        'pre_off
        '
        Me.pre_off.AutoSize = True
        Me.pre_off.Font = New System.Drawing.Font("Arial", 20.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.pre_off.Location = New System.Drawing.Point(693, 13)
        Me.pre_off.Name = "pre_off"
        Me.pre_off.Size = New System.Drawing.Size(65, 32)
        Me.pre_off.TabIndex = 6
        Me.pre_off.Text = "#off"
        '
        'pre_pastel
        '
        Me.pre_pastel.Image = Global.myOpenVFD.My.Resources.Resources.pastelrainbow
        Me.pre_pastel.Location = New System.Drawing.Point(84, 61)
        Me.pre_pastel.Name = "pre_pastel"
        Me.pre_pastel.Size = New System.Drawing.Size(204, 39)
        Me.pre_pastel.TabIndex = 5
        Me.pre_pastel.TabStop = False
        '
        'pre_green2blue
        '
        Me.pre_green2blue.Image = Global.myOpenVFD.My.Resources.Resources.green2blue
        Me.pre_green2blue.Location = New System.Drawing.Point(467, 246)
        Me.pre_green2blue.Name = "pre_green2blue"
        Me.pre_green2blue.Size = New System.Drawing.Size(175, 36)
        Me.pre_green2blue.TabIndex = 4
        Me.pre_green2blue.TabStop = False
        '
        'pre_red2blue
        '
        Me.pre_red2blue.Image = CType(resources.GetObject("pre_red2blue.Image"), System.Drawing.Image)
        Me.pre_red2blue.Location = New System.Drawing.Point(587, 123)
        Me.pre_red2blue.Name = "pre_red2blue"
        Me.pre_red2blue.Size = New System.Drawing.Size(141, 36)
        Me.pre_red2blue.TabIndex = 3
        Me.pre_red2blue.TabStop = False
        '
        'pre_red2green
        '
        Me.pre_red2green.Image = Global.myOpenVFD.My.Resources.Resources.red2green_alt
        Me.pre_red2green.Location = New System.Drawing.Point(339, 58)
        Me.pre_red2green.Name = "pre_red2green"
        Me.pre_red2green.Size = New System.Drawing.Size(160, 35)
        Me.pre_red2green.TabIndex = 2
        Me.pre_red2green.TabStop = False
        '
        'pre_Rainbow
        '
        Me.pre_Rainbow.Image = Global.myOpenVFD.My.Resources.Resources.rainbow_alt
        Me.pre_Rainbow.Location = New System.Drawing.Point(255, 106)
        Me.pre_Rainbow.Name = "pre_Rainbow"
        Me.pre_Rainbow.Size = New System.Drawing.Size(282, 63)
        Me.pre_Rainbow.TabIndex = 1
        Me.pre_Rainbow.TabStop = False
        '
        'Label8
        '
        Me.Label8.AutoSize = True
        Me.Label8.ForeColor = System.Drawing.Color.FromArgb(CType(CType(32, Byte), Integer), CType(CType(32, Byte), Integer), CType(CType(32, Byte), Integer))
        Me.Label8.Location = New System.Drawing.Point(18, 17)
        Me.Label8.Name = "Label8"
        Me.Label8.Size = New System.Drawing.Size(314, 16)
        Me.Label8.TabIndex = 0
        Me.Label8.Text = "Some color presets we believe to look amazing."
        '
        'tab_Dynamic
        '
        Me.tab_Dynamic.BackColor = System.Drawing.Color.White
        Me.tab_Dynamic.Controls.Add(Me.p_LEDMic)
        Me.tab_Dynamic.Controls.Add(Me.Label10)
        Me.tab_Dynamic.Controls.Add(Me.p_LEDs)
        Me.tab_Dynamic.Controls.Add(Me.bS_Cop)
        Me.tab_Dynamic.Controls.Add(Me.bS_Mic)
        Me.tab_Dynamic.Controls.Add(Me.bS_RCode)
        Me.tab_Dynamic.Controls.Add(Me.bS_Chase)
        Me.tab_Dynamic.Controls.Add(Me.bS_Cross)
        Me.tab_Dynamic.Controls.Add(Me.bS_Spectrum)
        Me.tab_Dynamic.Controls.Add(Me.p_LEDCross)
        Me.tab_Dynamic.Controls.Add(Me.p_LEDSpectrum)
        Me.tab_Dynamic.Controls.Add(Me.p_LEDCop)
        Me.tab_Dynamic.Controls.Add(Me.p_LEDResistorCode)
        Me.tab_Dynamic.Controls.Add(Me.p_LEDChaseFade)
        Me.tab_Dynamic.Location = New System.Drawing.Point(4, 25)
        Me.tab_Dynamic.Name = "tab_Dynamic"
        Me.tab_Dynamic.Padding = New System.Windows.Forms.Padding(3)
        Me.tab_Dynamic.Size = New System.Drawing.Size(776, 302)
        Me.tab_Dynamic.TabIndex = 1
        Me.tab_Dynamic.Text = "Dynamic Presets"
        '
        'p_LEDMic
        '
        Me.p_LEDMic.Controls.Add(Me.Label31)
        Me.p_LEDMic.Controls.Add(Me.Label20)
        Me.p_LEDMic.Controls.Add(Me.dancingTrackBar)
        Me.p_LEDMic.Controls.Add(Me.b_Mic)
        Me.p_LEDMic.Controls.Add(Me.Label21)
        Me.p_LEDMic.Location = New System.Drawing.Point(10, 220)
        Me.p_LEDMic.Name = "p_LEDMic"
        Me.p_LEDMic.Size = New System.Drawing.Size(760, 66)
        Me.p_LEDMic.TabIndex = 133
        Me.p_LEDMic.Visible = False
        '
        'Label31
        '
        Me.Label31.AutoSize = True
        Me.Label31.ForeColor = System.Drawing.Color.Purple
        Me.Label31.Location = New System.Drawing.Point(420, 29)
        Me.Label31.Name = "Label31"
        Me.Label31.Size = New System.Drawing.Size(36, 16)
        Me.Label31.TabIndex = 136
        Me.Label31.Text = "tight"
        '
        'Label20
        '
        Me.Label20.AutoSize = True
        Me.Label20.ForeColor = System.Drawing.Color.Purple
        Me.Label20.Location = New System.Drawing.Point(223, 29)
        Me.Label20.Name = "Label20"
        Me.Label20.Size = New System.Drawing.Size(35, 16)
        Me.Label20.TabIndex = 135
        Me.Label20.Text = "chill"
        '
        'dancingTrackBar
        '
        Me.dancingTrackBar.Location = New System.Drawing.Point(263, 19)
        Me.dancingTrackBar.Maximum = 42
        Me.dancingTrackBar.Minimum = 5
        Me.dancingTrackBar.Name = "dancingTrackBar"
        Me.dancingTrackBar.Size = New System.Drawing.Size(158, 45)
        Me.dancingTrackBar.TabIndex = 125
        Me.dancingTrackBar.Value = 5
        '
        'b_Mic
        '
        Me.b_Mic.BackColor = System.Drawing.Color.Teal
        Me.b_Mic.FlatAppearance.BorderSize = 0
        Me.b_Mic.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.b_Mic.Font = New System.Drawing.Font("Arial", 27.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.b_Mic.ForeColor = System.Drawing.Color.White
        Me.b_Mic.Location = New System.Drawing.Point(524, 8)
        Me.b_Mic.Name = "b_Mic"
        Me.b_Mic.Size = New System.Drawing.Size(232, 51)
        Me.b_Mic.TabIndex = 124
        Me.b_Mic.Text = "Set"
        Me.b_Mic.UseVisualStyleBackColor = False
        '
        'Label21
        '
        Me.Label21.AutoSize = True
        Me.Label21.Font = New System.Drawing.Font("Arial", 27.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label21.ForeColor = System.Drawing.Color.Purple
        Me.Label21.Location = New System.Drawing.Point(4, 10)
        Me.Label21.Name = "Label21"
        Me.Label21.Size = New System.Drawing.Size(172, 44)
        Me.Label21.TabIndex = 0
        Me.Label21.Text = "distance"
        '
        'Label10
        '
        Me.Label10.AutoSize = True
        Me.Label10.ForeColor = System.Drawing.Color.Purple
        Me.Label10.Location = New System.Drawing.Point(82, 50)
        Me.Label10.Name = "Label10"
        Me.Label10.Size = New System.Drawing.Size(413, 16)
        Me.Label10.TabIndex = 129
        Me.Label10.Text = "Animated visualization... Hope it looks smoother with real LEDs."
        '
        'p_LEDs
        '
        Me.p_LEDs.Controls.Add(Me.note)
        Me.p_LEDs.Controls.Add(Me.speaker)
        Me.p_LEDs.Controls.Add(Me.Label9)
        Me.p_LEDs.Location = New System.Drawing.Point(10, 69)
        Me.p_LEDs.Name = "p_LEDs"
        Me.p_LEDs.Size = New System.Drawing.Size(766, 144)
        Me.p_LEDs.TabIndex = 128
        '
        'note
        '
        Me.note.Image = Global.myOpenVFD.My.Resources.Resources.speaker_note
        Me.note.Location = New System.Drawing.Point(700, 3)
        Me.note.Name = "note"
        Me.note.Size = New System.Drawing.Size(30, 22)
        Me.note.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom
        Me.note.TabIndex = 8
        Me.note.TabStop = False
        Me.note.Visible = False
        '
        'speaker
        '
        Me.speaker.Image = Global.myOpenVFD.My.Resources.Resources.speaker_on
        Me.speaker.Location = New System.Drawing.Point(713, 10)
        Me.speaker.Name = "speaker"
        Me.speaker.Size = New System.Drawing.Size(43, 80)
        Me.speaker.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom
        Me.speaker.TabIndex = 7
        Me.speaker.TabStop = False
        Me.speaker.Visible = False
        '
        'Label9
        '
        Me.Label9.AutoSize = True
        Me.Label9.ForeColor = System.Drawing.Color.Purple
        Me.Label9.Location = New System.Drawing.Point(9, 110)
        Me.Label9.Name = "Label9"
        Me.Label9.Size = New System.Drawing.Size(44, 16)
        Me.Label9.TabIndex = 6
        Me.Label9.Text = "Time:"
        '
        'bS_Cop
        '
        Me.bS_Cop.BackColor = System.Drawing.Color.Teal
        Me.bS_Cop.FlatAppearance.BorderSize = 0
        Me.bS_Cop.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.bS_Cop.Font = New System.Drawing.Font("Arial", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.bS_Cop.ForeColor = System.Drawing.Color.White
        Me.bS_Cop.Location = New System.Drawing.Point(645, 10)
        Me.bS_Cop.Name = "bS_Cop"
        Me.bS_Cop.Size = New System.Drawing.Size(121, 25)
        Me.bS_Cop.TabIndex = 127
        Me.bS_Cop.Text = "Police Lights"
        Me.bS_Cop.UseVisualStyleBackColor = False
        '
        'bS_Mic
        '
        Me.bS_Mic.BackColor = System.Drawing.Color.Teal
        Me.bS_Mic.FlatAppearance.BorderSize = 0
        Me.bS_Mic.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.bS_Mic.Font = New System.Drawing.Font("Arial", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.bS_Mic.ForeColor = System.Drawing.Color.White
        Me.bS_Mic.Location = New System.Drawing.Point(518, 10)
        Me.bS_Mic.Name = "bS_Mic"
        Me.bS_Mic.Size = New System.Drawing.Size(121, 25)
        Me.bS_Mic.TabIndex = 126
        Me.bS_Mic.Text = "Dancing Mode"
        Me.bS_Mic.UseVisualStyleBackColor = False
        '
        'bS_RCode
        '
        Me.bS_RCode.BackColor = System.Drawing.Color.Teal
        Me.bS_RCode.FlatAppearance.BorderSize = 0
        Me.bS_RCode.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.bS_RCode.Font = New System.Drawing.Font("Arial", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.bS_RCode.ForeColor = System.Drawing.Color.White
        Me.bS_RCode.Location = New System.Drawing.Point(391, 10)
        Me.bS_RCode.Name = "bS_RCode"
        Me.bS_RCode.Size = New System.Drawing.Size(121, 25)
        Me.bS_RCode.TabIndex = 125
        Me.bS_RCode.Text = "Resistor Codes"
        Me.bS_RCode.UseVisualStyleBackColor = False
        '
        'bS_Chase
        '
        Me.bS_Chase.BackColor = System.Drawing.Color.Teal
        Me.bS_Chase.FlatAppearance.BorderSize = 0
        Me.bS_Chase.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.bS_Chase.Font = New System.Drawing.Font("Arial", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.bS_Chase.ForeColor = System.Drawing.Color.White
        Me.bS_Chase.Location = New System.Drawing.Point(264, 10)
        Me.bS_Chase.Name = "bS_Chase"
        Me.bS_Chase.Size = New System.Drawing.Size(121, 25)
        Me.bS_Chase.TabIndex = 124
        Me.bS_Chase.Text = "Color Chase Fade"
        Me.bS_Chase.UseVisualStyleBackColor = False
        '
        'bS_Cross
        '
        Me.bS_Cross.BackColor = System.Drawing.Color.Teal
        Me.bS_Cross.FlatAppearance.BorderSize = 0
        Me.bS_Cross.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.bS_Cross.Font = New System.Drawing.Font("Arial", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.bS_Cross.ForeColor = System.Drawing.Color.White
        Me.bS_Cross.Location = New System.Drawing.Point(137, 10)
        Me.bS_Cross.Name = "bS_Cross"
        Me.bS_Cross.Size = New System.Drawing.Size(121, 25)
        Me.bS_Cross.TabIndex = 123
        Me.bS_Cross.Text = "Cross Fade"
        Me.bS_Cross.UseVisualStyleBackColor = False
        '
        'bS_Spectrum
        '
        Me.bS_Spectrum.BackColor = System.Drawing.Color.Teal
        Me.bS_Spectrum.FlatAppearance.BorderSize = 0
        Me.bS_Spectrum.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.bS_Spectrum.Font = New System.Drawing.Font("Arial", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.bS_Spectrum.ForeColor = System.Drawing.Color.White
        Me.bS_Spectrum.Location = New System.Drawing.Point(10, 10)
        Me.bS_Spectrum.Name = "bS_Spectrum"
        Me.bS_Spectrum.Size = New System.Drawing.Size(121, 25)
        Me.bS_Spectrum.TabIndex = 122
        Me.bS_Spectrum.Text = "Spectrum Fade"
        Me.bS_Spectrum.UseVisualStyleBackColor = False
        '
        'p_LEDCross
        '
        Me.p_LEDCross.Controls.Add(Me.Label11)
        Me.p_LEDCross.Controls.Add(Me.Label12)
        Me.p_LEDCross.Controls.Add(Me.crossTrackBar)
        Me.p_LEDCross.Controls.Add(Me.Label32)
        Me.p_LEDCross.Controls.Add(Me.b_Cross)
        Me.p_LEDCross.Location = New System.Drawing.Point(10, 220)
        Me.p_LEDCross.Name = "p_LEDCross"
        Me.p_LEDCross.Size = New System.Drawing.Size(760, 66)
        Me.p_LEDCross.TabIndex = 131
        Me.p_LEDCross.Visible = False
        '
        'Label11
        '
        Me.Label11.AutoSize = True
        Me.Label11.ForeColor = System.Drawing.Color.Purple
        Me.Label11.Location = New System.Drawing.Point(420, 28)
        Me.Label11.Name = "Label11"
        Me.Label11.Size = New System.Drawing.Size(36, 16)
        Me.Label11.TabIndex = 140
        Me.Label11.Text = "tight"
        '
        'Label12
        '
        Me.Label12.AutoSize = True
        Me.Label12.ForeColor = System.Drawing.Color.Purple
        Me.Label12.Location = New System.Drawing.Point(223, 28)
        Me.Label12.Name = "Label12"
        Me.Label12.Size = New System.Drawing.Size(35, 16)
        Me.Label12.TabIndex = 139
        Me.Label12.Text = "chill"
        '
        'crossTrackBar
        '
        Me.crossTrackBar.Location = New System.Drawing.Point(263, 18)
        Me.crossTrackBar.Maximum = 42
        Me.crossTrackBar.Minimum = 5
        Me.crossTrackBar.Name = "crossTrackBar"
        Me.crossTrackBar.Size = New System.Drawing.Size(158, 45)
        Me.crossTrackBar.TabIndex = 138
        Me.crossTrackBar.Value = 5
        '
        'Label32
        '
        Me.Label32.AutoSize = True
        Me.Label32.Font = New System.Drawing.Font("Arial", 27.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label32.ForeColor = System.Drawing.Color.Purple
        Me.Label32.Location = New System.Drawing.Point(4, 10)
        Me.Label32.Name = "Label32"
        Me.Label32.Size = New System.Drawing.Size(172, 44)
        Me.Label32.TabIndex = 137
        Me.Label32.Text = "distance"
        '
        'b_Cross
        '
        Me.b_Cross.BackColor = System.Drawing.Color.Teal
        Me.b_Cross.FlatAppearance.BorderSize = 0
        Me.b_Cross.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.b_Cross.Font = New System.Drawing.Font("Arial", 27.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.b_Cross.ForeColor = System.Drawing.Color.White
        Me.b_Cross.Location = New System.Drawing.Point(524, 8)
        Me.b_Cross.Name = "b_Cross"
        Me.b_Cross.Size = New System.Drawing.Size(232, 51)
        Me.b_Cross.TabIndex = 124
        Me.b_Cross.Text = "Set"
        Me.b_Cross.UseVisualStyleBackColor = False
        '
        'p_LEDSpectrum
        '
        Me.p_LEDSpectrum.Controls.Add(Me.Label22)
        Me.p_LEDSpectrum.Controls.Add(Me.b_Spectrum)
        Me.p_LEDSpectrum.Location = New System.Drawing.Point(10, 220)
        Me.p_LEDSpectrum.Name = "p_LEDSpectrum"
        Me.p_LEDSpectrum.Size = New System.Drawing.Size(760, 66)
        Me.p_LEDSpectrum.TabIndex = 132
        '
        'Label22
        '
        Me.Label22.AutoSize = True
        Me.Label22.Location = New System.Drawing.Point(8, 31)
        Me.Label22.Name = "Label22"
        Me.Label22.Size = New System.Drawing.Size(281, 16)
        Me.Label22.TabIndex = 125
        Me.Label22.Text = "Guides you through all the amazing colors."
        '
        'b_Spectrum
        '
        Me.b_Spectrum.BackColor = System.Drawing.Color.Teal
        Me.b_Spectrum.FlatAppearance.BorderSize = 0
        Me.b_Spectrum.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.b_Spectrum.Font = New System.Drawing.Font("Arial", 27.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.b_Spectrum.ForeColor = System.Drawing.Color.White
        Me.b_Spectrum.Location = New System.Drawing.Point(524, 8)
        Me.b_Spectrum.Name = "b_Spectrum"
        Me.b_Spectrum.Size = New System.Drawing.Size(232, 51)
        Me.b_Spectrum.TabIndex = 124
        Me.b_Spectrum.Text = "Set"
        Me.b_Spectrum.UseVisualStyleBackColor = False
        '
        'p_LEDCop
        '
        Me.p_LEDCop.Controls.Add(Me.b_Cop)
        Me.p_LEDCop.Controls.Add(Me.cb_CopMode)
        Me.p_LEDCop.Controls.Add(Me.Label25)
        Me.p_LEDCop.Controls.Add(Me.Label26)
        Me.p_LEDCop.Location = New System.Drawing.Point(10, 220)
        Me.p_LEDCop.Name = "p_LEDCop"
        Me.p_LEDCop.Size = New System.Drawing.Size(760, 66)
        Me.p_LEDCop.TabIndex = 134
        Me.p_LEDCop.Visible = False
        '
        'b_Cop
        '
        Me.b_Cop.BackColor = System.Drawing.Color.Teal
        Me.b_Cop.FlatAppearance.BorderSize = 0
        Me.b_Cop.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.b_Cop.Font = New System.Drawing.Font("Arial", 27.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.b_Cop.ForeColor = System.Drawing.Color.White
        Me.b_Cop.Location = New System.Drawing.Point(524, 8)
        Me.b_Cop.Name = "b_Cop"
        Me.b_Cop.Size = New System.Drawing.Size(232, 51)
        Me.b_Cop.TabIndex = 124
        Me.b_Cop.Text = "Set"
        Me.b_Cop.UseVisualStyleBackColor = False
        '
        'cb_CopMode
        '
        Me.cb_CopMode.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList
        Me.cb_CopMode.Font = New System.Drawing.Font("Arial", 27.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.cb_CopMode.ForeColor = System.Drawing.Color.Purple
        Me.cb_CopMode.FormattingEnabled = True
        Me.cb_CopMode.Items.AddRange(New Object() {"0"})
        Me.cb_CopMode.Location = New System.Drawing.Point(156, 7)
        Me.cb_CopMode.Name = "cb_CopMode"
        Me.cb_CopMode.Size = New System.Drawing.Size(60, 52)
        Me.cb_CopMode.TabIndex = 2
        '
        'Label25
        '
        Me.Label25.AutoSize = True
        Me.Label25.Font = New System.Drawing.Font("Arial", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label25.Location = New System.Drawing.Point(235, 19)
        Me.Label25.Name = "Label25"
        Me.Label25.Size = New System.Drawing.Size(187, 32)
        Me.Label25.TabIndex = 1
        Me.Label25.Text = "Should it be on patrol or on " & Global.Microsoft.VisualBasic.ChrW(13) & Global.Microsoft.VisualBasic.ChrW(10) & "a high speed pursuit?"
        '
        'Label26
        '
        Me.Label26.AutoSize = True
        Me.Label26.Font = New System.Drawing.Font("Arial", 27.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label26.ForeColor = System.Drawing.Color.Purple
        Me.Label26.Location = New System.Drawing.Point(4, 7)
        Me.Label26.Name = "Label26"
        Me.Label26.Size = New System.Drawing.Size(146, 44)
        Me.Label26.TabIndex = 0
        Me.Label26.Text = "pattern"
        Me.Label26.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
        '
        'p_LEDResistorCode
        '
        Me.p_LEDResistorCode.Controls.Add(Me.p_Resistors)
        Me.p_LEDResistorCode.Controls.Add(Me.p_ResistorsMore)
        Me.p_LEDResistorCode.Controls.Add(Me.PictureBox1)
        Me.p_LEDResistorCode.Controls.Add(Me.b_Resistor)
        Me.p_LEDResistorCode.Location = New System.Drawing.Point(10, 220)
        Me.p_LEDResistorCode.Name = "p_LEDResistorCode"
        Me.p_LEDResistorCode.Size = New System.Drawing.Size(760, 66)
        Me.p_LEDResistorCode.TabIndex = 133
        Me.p_LEDResistorCode.Visible = False
        '
        'p_Resistors
        '
        Me.p_Resistors.Controls.Add(Me.LinkLabel1)
        Me.p_Resistors.Controls.Add(Me.Label15)
        Me.p_Resistors.Location = New System.Drawing.Point(137, 3)
        Me.p_Resistors.Name = "p_Resistors"
        Me.p_Resistors.Size = New System.Drawing.Size(380, 62)
        Me.p_Resistors.TabIndex = 132
        '
        'LinkLabel1
        '
        Me.LinkLabel1.AutoSize = True
        Me.LinkLabel1.Location = New System.Drawing.Point(326, 34)
        Me.LinkLabel1.Name = "LinkLabel1"
        Me.LinkLabel1.Size = New System.Drawing.Size(40, 16)
        Me.LinkLabel1.TabIndex = 127
        Me.LinkLabel1.TabStop = True
        Me.LinkLabel1.Text = "More"
        '
        'Label15
        '
        Me.Label15.AutoSize = True
        Me.Label15.ForeColor = System.Drawing.Color.FromArgb(CType(CType(0, Byte), Integer), CType(CType(64, Byte), Integer), CType(CType(64, Byte), Integer))
        Me.Label15.Location = New System.Drawing.Point(3, 2)
        Me.Label15.Name = "Label15"
        Me.Label15.Size = New System.Drawing.Size(341, 48)
        Me.Label15.TabIndex = 126
        Me.Label15.Text = "Geek alert! Resistors limit electrical current flow." & Global.Microsoft.VisualBasic.ChrW(13) & Global.Microsoft.VisualBasic.ChrW(10) & "Color rings printed on them" &
    " tell the resistance value," & Global.Microsoft.VisualBasic.ChrW(13) & Global.Microsoft.VisualBasic.ChrW(10) & "so each color belongs to a number."
        '
        'p_ResistorsMore
        '
        Me.p_ResistorsMore.Controls.Add(Me.LinkLabel2)
        Me.p_ResistorsMore.Controls.Add(Me.Label24)
        Me.p_ResistorsMore.Controls.Add(Me.Label19)
        Me.p_ResistorsMore.Controls.Add(Me.Label18)
        Me.p_ResistorsMore.Controls.Add(Me.Label17)
        Me.p_ResistorsMore.Controls.Add(Me.Label16)
        Me.p_ResistorsMore.Location = New System.Drawing.Point(137, 3)
        Me.p_ResistorsMore.Name = "p_ResistorsMore"
        Me.p_ResistorsMore.Size = New System.Drawing.Size(380, 62)
        Me.p_ResistorsMore.TabIndex = 129
        '
        'LinkLabel2
        '
        Me.LinkLabel2.AutoSize = True
        Me.LinkLabel2.Location = New System.Drawing.Point(326, 34)
        Me.LinkLabel2.Name = "LinkLabel2"
        Me.LinkLabel2.Size = New System.Drawing.Size(39, 16)
        Me.LinkLabel2.TabIndex = 133
        Me.LinkLabel2.TabStop = True
        Me.LinkLabel2.Text = "Back"
        '
        'Label24
        '
        Me.Label24.AutoSize = True
        Me.Label24.Location = New System.Drawing.Point(296, 2)
        Me.Label24.Name = "Label24"
        Me.Label24.Size = New System.Drawing.Size(60, 16)
        Me.Label24.TabIndex = 132
        Me.Label24.Text = "9: White"
        '
        'Label19
        '
        Me.Label19.AutoSize = True
        Me.Label19.Location = New System.Drawing.Point(225, 2)
        Me.Label19.Name = "Label19"
        Me.Label19.Size = New System.Drawing.Size(65, 48)
        Me.Label19.TabIndex = 131
        Me.Label19.Text = "6: Blue" & Global.Microsoft.VisualBasic.ChrW(13) & Global.Microsoft.VisualBasic.ChrW(10) & "7: Purple" & Global.Microsoft.VisualBasic.ChrW(13) & Global.Microsoft.VisualBasic.ChrW(10) & "8: Gray"
        '
        'Label18
        '
        Me.Label18.AutoSize = True
        Me.Label18.Location = New System.Drawing.Point(149, 2)
        Me.Label18.Name = "Label18"
        Me.Label18.Size = New System.Drawing.Size(70, 48)
        Me.Label18.TabIndex = 130
        Me.Label18.Text = "3: Orange" & Global.Microsoft.VisualBasic.ChrW(13) & Global.Microsoft.VisualBasic.ChrW(10) & "4: Yellow" & Global.Microsoft.VisualBasic.ChrW(13) & Global.Microsoft.VisualBasic.ChrW(10) & "5: Green"
        '
        'Label17
        '
        Me.Label17.AutoSize = True
        Me.Label17.Location = New System.Drawing.Point(73, 2)
        Me.Label17.Name = "Label17"
        Me.Label17.Size = New System.Drawing.Size(70, 48)
        Me.Label17.TabIndex = 129
        Me.Label17.Text = "3: Orange" & Global.Microsoft.VisualBasic.ChrW(13) & Global.Microsoft.VisualBasic.ChrW(10) & "4: Yellow" & Global.Microsoft.VisualBasic.ChrW(13) & Global.Microsoft.VisualBasic.ChrW(10) & "5: Green"
        '
        'Label16
        '
        Me.Label16.AutoSize = True
        Me.Label16.Location = New System.Drawing.Point(3, 2)
        Me.Label16.Name = "Label16"
        Me.Label16.Size = New System.Drawing.Size(64, 48)
        Me.Label16.TabIndex = 128
        Me.Label16.Text = "0: Black" & Global.Microsoft.VisualBasic.ChrW(13) & Global.Microsoft.VisualBasic.ChrW(10) & "1: Brown" & Global.Microsoft.VisualBasic.ChrW(13) & Global.Microsoft.VisualBasic.ChrW(10) & "2: Red"
        '
        'PictureBox1
        '
        Me.PictureBox1.Image = CType(resources.GetObject("PictureBox1.Image"), System.Drawing.Image)
        Me.PictureBox1.Location = New System.Drawing.Point(11, 8)
        Me.PictureBox1.Name = "PictureBox1"
        Me.PictureBox1.Size = New System.Drawing.Size(120, 50)
        Me.PictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom
        Me.PictureBox1.TabIndex = 125
        Me.PictureBox1.TabStop = False
        '
        'b_Resistor
        '
        Me.b_Resistor.BackColor = System.Drawing.Color.Teal
        Me.b_Resistor.FlatAppearance.BorderSize = 0
        Me.b_Resistor.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.b_Resistor.Font = New System.Drawing.Font("Arial", 27.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.b_Resistor.ForeColor = System.Drawing.Color.White
        Me.b_Resistor.Location = New System.Drawing.Point(524, 8)
        Me.b_Resistor.Name = "b_Resistor"
        Me.b_Resistor.Size = New System.Drawing.Size(232, 51)
        Me.b_Resistor.TabIndex = 124
        Me.b_Resistor.Text = "Set"
        Me.b_Resistor.UseVisualStyleBackColor = False
        '
        'p_LEDChaseFade
        '
        Me.p_LEDChaseFade.Controls.Add(Me.b_ChaseFade)
        Me.p_LEDChaseFade.Controls.Add(Me.cb_ChFdMode)
        Me.p_LEDChaseFade.Controls.Add(Me.l_chFdInfo)
        Me.p_LEDChaseFade.Controls.Add(Me.Label14)
        Me.p_LEDChaseFade.Location = New System.Drawing.Point(10, 220)
        Me.p_LEDChaseFade.Name = "p_LEDChaseFade"
        Me.p_LEDChaseFade.Size = New System.Drawing.Size(760, 66)
        Me.p_LEDChaseFade.TabIndex = 132
        Me.p_LEDChaseFade.Visible = False
        '
        'b_ChaseFade
        '
        Me.b_ChaseFade.BackColor = System.Drawing.Color.Teal
        Me.b_ChaseFade.FlatAppearance.BorderSize = 0
        Me.b_ChaseFade.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.b_ChaseFade.Font = New System.Drawing.Font("Arial", 27.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.b_ChaseFade.ForeColor = System.Drawing.Color.White
        Me.b_ChaseFade.Location = New System.Drawing.Point(524, 8)
        Me.b_ChaseFade.Name = "b_ChaseFade"
        Me.b_ChaseFade.Size = New System.Drawing.Size(232, 51)
        Me.b_ChaseFade.TabIndex = 124
        Me.b_ChaseFade.Text = "Set"
        Me.b_ChaseFade.UseVisualStyleBackColor = False
        '
        'cb_ChFdMode
        '
        Me.cb_ChFdMode.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList
        Me.cb_ChFdMode.Font = New System.Drawing.Font("Arial", 27.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.cb_ChFdMode.ForeColor = System.Drawing.Color.Purple
        Me.cb_ChFdMode.FormattingEnabled = True
        Me.cb_ChFdMode.Items.AddRange(New Object() {"0", "1", "2", "3"})
        Me.cb_ChFdMode.Location = New System.Drawing.Point(124, 7)
        Me.cb_ChFdMode.Name = "cb_ChFdMode"
        Me.cb_ChFdMode.Size = New System.Drawing.Size(55, 52)
        Me.cb_ChFdMode.TabIndex = 2
        '
        'l_chFdInfo
        '
        Me.l_chFdInfo.AutoSize = True
        Me.l_chFdInfo.Font = New System.Drawing.Font("Arial", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.l_chFdInfo.Location = New System.Drawing.Point(197, 28)
        Me.l_chFdInfo.Name = "l_chFdInfo"
        Me.l_chFdInfo.Size = New System.Drawing.Size(126, 16)
        Me.l_chFdInfo.TabIndex = 1
        Me.l_chFdInfo.Text = "... from right to left"
        '
        'Label14
        '
        Me.Label14.AutoSize = True
        Me.Label14.Font = New System.Drawing.Font("Arial", 27.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label14.ForeColor = System.Drawing.Color.Purple
        Me.Label14.Location = New System.Drawing.Point(4, 10)
        Me.Label14.Name = "Label14"
        Me.Label14.Size = New System.Drawing.Size(120, 44)
        Me.Label14.TabIndex = 0
        Me.Label14.Text = "mode"
        '
        'tab_Ambient
        '
        Me.tab_Ambient.Controls.Add(Me.l_MoodStatus)
        Me.tab_Ambient.Controls.Add(Me.Label30)
        Me.tab_Ambient.Controls.Add(Me.p_Mood)
        Me.tab_Ambient.Controls.Add(Me.pImage)
        Me.tab_Ambient.Controls.Add(Me.PictureBox2)
        Me.tab_Ambient.Location = New System.Drawing.Point(4, 25)
        Me.tab_Ambient.Name = "tab_Ambient"
        Me.tab_Ambient.Size = New System.Drawing.Size(776, 302)
        Me.tab_Ambient.TabIndex = 2
        Me.tab_Ambient.Text = "Ambient Light"
        Me.tab_Ambient.UseVisualStyleBackColor = True
        '
        'l_MoodStatus
        '
        Me.l_MoodStatus.ForeColor = System.Drawing.Color.Purple
        Me.l_MoodStatus.Location = New System.Drawing.Point(604, 110)
        Me.l_MoodStatus.Name = "l_MoodStatus"
        Me.l_MoodStatus.Size = New System.Drawing.Size(164, 32)
        Me.l_MoodStatus.TabIndex = 6
        Me.l_MoodStatus.Text = "Click on the computer" & Global.Microsoft.VisualBasic.ChrW(13) & Global.Microsoft.VisualBasic.ChrW(10) & "to begin!"
        Me.l_MoodStatus.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
        '
        'Label30
        '
        Me.Label30.ForeColor = System.Drawing.Color.Purple
        Me.Label30.Location = New System.Drawing.Point(8, 110)
        Me.Label30.Name = "Label30"
        Me.Label30.Size = New System.Drawing.Size(197, 36)
        Me.Label30.TabIndex = 5
        Me.Label30.Text = "Generate ambient light colors" & Global.Microsoft.VisualBasic.ChrW(13) & Global.Microsoft.VisualBasic.ChrW(10) & "based on the display mood"
        Me.Label30.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
        '
        'p_Mood
        '
        Me.p_Mood.Location = New System.Drawing.Point(247, 250)
        Me.p_Mood.Name = "p_Mood"
        Me.p_Mood.Size = New System.Drawing.Size(323, 49)
        Me.p_Mood.TabIndex = 4
        '
        'pImage
        '
        Me.pImage.BackColor = System.Drawing.Color.Black
        Me.pImage.Cursor = System.Windows.Forms.Cursors.Hand
        Me.pImage.Location = New System.Drawing.Point(266, 25)
        Me.pImage.Name = "pImage"
        Me.pImage.Size = New System.Drawing.Size(246, 138)
        Me.pImage.TabIndex = 3
        Me.pImage.TabStop = False
        '
        'PictureBox2
        '
        Me.PictureBox2.Image = Global.myOpenVFD.My.Resources.Resources.computer
        Me.PictureBox2.Location = New System.Drawing.Point(211, 11)
        Me.PictureBox2.Name = "PictureBox2"
        Me.PictureBox2.Size = New System.Drawing.Size(357, 237)
        Me.PictureBox2.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom
        Me.PictureBox2.TabIndex = 2
        Me.PictureBox2.TabStop = False
        '
        'b_SetColors
        '
        Me.b_SetColors.BackColor = System.Drawing.Color.Teal
        Me.b_SetColors.FlatAppearance.BorderSize = 0
        Me.b_SetColors.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.b_SetColors.ForeColor = System.Drawing.Color.White
        Me.b_SetColors.Location = New System.Drawing.Point(93, 24)
        Me.b_SetColors.Name = "b_SetColors"
        Me.b_SetColors.Size = New System.Drawing.Size(70, 57)
        Me.b_SetColors.TabIndex = 121
        Me.b_SetColors.Text = "Custom Colors"
        Me.b_SetColors.UseVisualStyleBackColor = False
        '
        'b_SendMsg
        '
        Me.b_SendMsg.BackColor = System.Drawing.Color.Teal
        Me.b_SendMsg.FlatAppearance.BorderSize = 0
        Me.b_SendMsg.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.b_SendMsg.ForeColor = System.Drawing.Color.White
        Me.b_SendMsg.Location = New System.Drawing.Point(190, 24)
        Me.b_SendMsg.Name = "b_SendMsg"
        Me.b_SendMsg.Size = New System.Drawing.Size(70, 57)
        Me.b_SendMsg.TabIndex = 122
        Me.b_SendMsg.Text = "Send Message"
        Me.b_SendMsg.UseVisualStyleBackColor = False
        '
        'b_TimeSync
        '
        Me.b_TimeSync.BackColor = System.Drawing.Color.Teal
        Me.b_TimeSync.FlatAppearance.BorderSize = 0
        Me.b_TimeSync.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.b_TimeSync.ForeColor = System.Drawing.Color.White
        Me.b_TimeSync.Location = New System.Drawing.Point(266, 24)
        Me.b_TimeSync.Name = "b_TimeSync"
        Me.b_TimeSync.Size = New System.Drawing.Size(70, 57)
        Me.b_TimeSync.TabIndex = 123
        Me.b_TimeSync.Text = "Time Sync"
        Me.b_TimeSync.UseVisualStyleBackColor = False
        '
        'b_Presets
        '
        Me.b_Presets.BackColor = System.Drawing.Color.Teal
        Me.b_Presets.FlatAppearance.BorderSize = 0
        Me.b_Presets.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.b_Presets.ForeColor = System.Drawing.Color.White
        Me.b_Presets.Location = New System.Drawing.Point(17, 24)
        Me.b_Presets.Name = "b_Presets"
        Me.b_Presets.Size = New System.Drawing.Size(70, 57)
        Me.b_Presets.TabIndex = 124
        Me.b_Presets.Text = "Color Presets"
        Me.b_Presets.UseVisualStyleBackColor = False
        '
        'timeTimer
        '
        Me.timeTimer.Enabled = True
        '
        'ledTimer
        '
        Me.ledTimer.Enabled = True
        Me.ledTimer.Interval = 500
        '
        'ledSubTimer
        '
        '
        'b_SaveSettings
        '
        Me.b_SaveSettings.BackColor = System.Drawing.Color.Teal
        Me.b_SaveSettings.FlatAppearance.BorderSize = 0
        Me.b_SaveSettings.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.b_SaveSettings.ForeColor = System.Drawing.Color.White
        Me.b_SaveSettings.Location = New System.Drawing.Point(342, 24)
        Me.b_SaveSettings.Name = "b_SaveSettings"
        Me.b_SaveSettings.Size = New System.Drawing.Size(70, 57)
        Me.b_SaveSettings.TabIndex = 125
        Me.b_SaveSettings.Text = "Clock Settings"
        Me.b_SaveSettings.UseVisualStyleBackColor = False
        '
        'p_SaveSettings
        '
        Me.p_SaveSettings.Controls.Add(Me.GroupBox2)
        Me.p_SaveSettings.Controls.Add(Me.GroupBox1)
        Me.p_SaveSettings.Controls.Add(Me.l_Reset)
        Me.p_SaveSettings.Controls.Add(Me.l_Save2)
        Me.p_SaveSettings.Controls.Add(Me.l_Save1)
        Me.p_SaveSettings.Location = New System.Drawing.Point(0, 130)
        Me.p_SaveSettings.Name = "p_SaveSettings"
        Me.p_SaveSettings.Size = New System.Drawing.Size(784, 331)
        Me.p_SaveSettings.TabIndex = 126
        Me.p_SaveSettings.Visible = False
        '
        'timeNightShiftEnable
        '
        Me.timeNightShiftEnable.CalendarFont = New System.Drawing.Font("Arial", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.timeNightShiftEnable.CalendarTitleBackColor = System.Drawing.SystemColors.ControlText
        Me.timeNightShiftEnable.CalendarTitleForeColor = System.Drawing.Color.Purple
        Me.timeNightShiftEnable.CustomFormat = "HH:mm"
        Me.timeNightShiftEnable.Font = New System.Drawing.Font("Arial", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.timeNightShiftEnable.Format = System.Windows.Forms.DateTimePickerFormat.Custom
        Me.timeNightShiftEnable.Location = New System.Drawing.Point(144, 96)
        Me.timeNightShiftEnable.Name = "timeNightShiftEnable"
        Me.timeNightShiftEnable.ShowUpDown = True
        Me.timeNightShiftEnable.Size = New System.Drawing.Size(56, 22)
        Me.timeNightShiftEnable.TabIndex = 8
        Me.timeNightShiftEnable.Value = New Date(2018, 3, 20, 22, 0, 0, 0)
        '
        'GroupBox2
        '
        Me.GroupBox2.Controls.Add(Me.Label35)
        Me.GroupBox2.Controls.Add(Me.cb_scheduledNightShift)
        Me.GroupBox2.Controls.Add(Me.cb_nightShiftToggle)
        Me.GroupBox2.Controls.Add(Me.Label34)
        Me.GroupBox2.Controls.Add(Me.timeNightShiftDisable)
        Me.GroupBox2.Controls.Add(Me.Label33)
        Me.GroupBox2.Controls.Add(Me.timeNightShiftEnable)
        Me.GroupBox2.Font = New System.Drawing.Font("Arial", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.GroupBox2.ForeColor = System.Drawing.Color.Purple
        Me.GroupBox2.Location = New System.Drawing.Point(14, 131)
        Me.GroupBox2.Name = "GroupBox2"
        Me.GroupBox2.Size = New System.Drawing.Size(206, 183)
        Me.GroupBox2.TabIndex = 7
        Me.GroupBox2.TabStop = False
        Me.GroupBox2.Text = "Night Shift"
        '
        'Label33
        '
        Me.Label33.AutoSize = True
        Me.Label33.Location = New System.Drawing.Point(9, 99)
        Me.Label33.Name = "Label33"
        Me.Label33.Size = New System.Drawing.Size(131, 16)
        Me.Label33.TabIndex = 0
        Me.Label33.Text = "Every evening from"
        '
        'cb_scheduledNightShift
        '
        Me.cb_scheduledNightShift.AutoSize = True
        Me.cb_scheduledNightShift.ForeColor = System.Drawing.Color.Purple
        Me.cb_scheduledNightShift.Location = New System.Drawing.Point(11, 71)
        Me.cb_scheduledNightShift.Name = "cb_scheduledNightShift"
        Me.cb_scheduledNightShift.Size = New System.Drawing.Size(161, 20)
        Me.cb_scheduledNightShift.TabIndex = 4
        Me.cb_scheduledNightShift.Text = "Scheduled night shift"
        Me.cb_scheduledNightShift.UseVisualStyleBackColor = True
        '
        'cb_nightShiftToggle
        '
        Me.cb_nightShiftToggle.AutoSize = True
        Me.cb_nightShiftToggle.ForeColor = System.Drawing.Color.Purple
        Me.cb_nightShiftToggle.Location = New System.Drawing.Point(11, 29)
        Me.cb_nightShiftToggle.Name = "cb_nightShiftToggle"
        Me.cb_nightShiftToggle.Size = New System.Drawing.Size(136, 20)
        Me.cb_nightShiftToggle.TabIndex = 3
        Me.cb_nightShiftToggle.Text = "Toggle night shift"
        Me.cb_nightShiftToggle.UseVisualStyleBackColor = True
        '
        'GroupBox1
        '
        Me.GroupBox1.Controls.Add(Me.cb_leadingZero)
        Me.GroupBox1.Controls.Add(Me.cb_12h)
        Me.GroupBox1.Controls.Add(Me.cb_intd)
        Me.GroupBox1.Font = New System.Drawing.Font("Arial", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.GroupBox1.ForeColor = System.Drawing.Color.Purple
        Me.GroupBox1.Location = New System.Drawing.Point(14, 17)
        Me.GroupBox1.Name = "GroupBox1"
        Me.GroupBox1.Size = New System.Drawing.Size(206, 102)
        Me.GroupBox1.TabIndex = 6
        Me.GroupBox1.TabStop = False
        Me.GroupBox1.Text = "Set time and date format"
        '
        'cb_12h
        '
        Me.cb_12h.AutoSize = True
        Me.cb_12h.ForeColor = System.Drawing.Color.Purple
        Me.cb_12h.Location = New System.Drawing.Point(12, 21)
        Me.cb_12h.Name = "cb_12h"
        Me.cb_12h.Size = New System.Drawing.Size(127, 20)
        Me.cb_12h.TabIndex = 3
        Me.cb_12h.Text = "12h hour format"
        Me.cb_12h.UseVisualStyleBackColor = True
        '
        'cb_intd
        '
        Me.cb_intd.AutoSize = True
        Me.cb_intd.ForeColor = System.Drawing.Color.Purple
        Me.cb_intd.Location = New System.Drawing.Point(12, 69)
        Me.cb_intd.Name = "cb_intd"
        Me.cb_intd.Size = New System.Drawing.Size(136, 20)
        Me.cb_intd.TabIndex = 4
        Me.cb_intd.Text = "MM/DD/YY format"
        Me.cb_intd.UseVisualStyleBackColor = True
        '
        'l_Reset
        '
        Me.l_Reset.AutoSize = True
        Me.l_Reset.Cursor = System.Windows.Forms.Cursors.Hand
        Me.l_Reset.Font = New System.Drawing.Font("Arial", 9.75!, CType((System.Drawing.FontStyle.Bold Or System.Drawing.FontStyle.Underline), System.Drawing.FontStyle), System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.l_Reset.ForeColor = System.Drawing.Color.Teal
        Me.l_Reset.Location = New System.Drawing.Point(493, 306)
        Me.l_Reset.Name = "l_Reset"
        Me.l_Reset.Size = New System.Drawing.Size(281, 16)
        Me.l_Reset.TabIndex = 2
        Me.l_Reset.Text = "(Or you can set the settings back to default)"
        '
        'l_Save2
        '
        Me.l_Save2.AutoSize = True
        Me.l_Save2.Cursor = System.Windows.Forms.Cursors.Hand
        Me.l_Save2.Font = New System.Drawing.Font("Arial", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.l_Save2.ForeColor = System.Drawing.Color.Teal
        Me.l_Save2.Location = New System.Drawing.Point(288, 187)
        Me.l_Save2.Name = "l_Save2"
        Me.l_Save2.Size = New System.Drawing.Size(233, 16)
        Me.l_Save2.TabIndex = 1
        Me.l_Save2.Text = "the changes you made to the clock."
        '
        'l_Save1
        '
        Me.l_Save1.Cursor = System.Windows.Forms.Cursors.Hand
        Me.l_Save1.Font = New System.Drawing.Font("Arial", 72.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.l_Save1.ForeColor = System.Drawing.Color.Purple
        Me.l_Save1.Location = New System.Drawing.Point(241, 75)
        Me.l_Save1.Name = "l_Save1"
        Me.l_Save1.Size = New System.Drawing.Size(338, 111)
        Me.l_Save1.TabIndex = 0
        Me.l_Save1.Text = "SAVE"
        Me.l_Save1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
        '
        'Label13
        '
        Me.Label13.Cursor = System.Windows.Forms.Cursors.Default
        Me.Label13.Font = New System.Drawing.Font("Arial", 72.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label13.ForeColor = System.Drawing.Color.Purple
        Me.Label13.Location = New System.Drawing.Point(3, 0)
        Me.Label13.Name = "Label13"
        Me.Label13.Size = New System.Drawing.Size(367, 111)
        Me.Label13.TabIndex = 127
        Me.Label13.Text = "CLICK"
        Me.Label13.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
        '
        'Panel2
        '
        Me.Panel2.Controls.Add(Me.Label23)
        Me.Panel2.Controls.Add(Me.Label13)
        Me.Panel2.Location = New System.Drawing.Point(200, 205)
        Me.Panel2.Name = "Panel2"
        Me.Panel2.Size = New System.Drawing.Size(362, 168)
        Me.Panel2.TabIndex = 128
        '
        'Label23
        '
        Me.Label23.AutoSize = True
        Me.Label23.Cursor = System.Windows.Forms.Cursors.Hand
        Me.Label23.Font = New System.Drawing.Font("Arial", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label23.ForeColor = System.Drawing.Color.Teal
        Me.Label23.Location = New System.Drawing.Point(67, 109)
        Me.Label23.Name = "Label23"
        Me.Label23.Size = New System.Drawing.Size(243, 16)
        Me.Label23.TabIndex = 3
        Me.Label23.Text = "on the buttons above to do somethin."
        '
        'b_About
        '
        Me.b_About.BackColor = System.Drawing.Color.Teal
        Me.b_About.FlatAppearance.BorderSize = 0
        Me.b_About.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.b_About.ForeColor = System.Drawing.Color.White
        Me.b_About.Location = New System.Drawing.Point(441, 24)
        Me.b_About.Name = "b_About"
        Me.b_About.Size = New System.Drawing.Size(70, 57)
        Me.b_About.TabIndex = 129
        Me.b_About.Text = "About"
        Me.b_About.UseVisualStyleBackColor = False
        '
        'p_About
        '
        Me.p_About.Controls.Add(Me.Button1)
        Me.p_About.Controls.Add(Me.b_UserManual)
        Me.p_About.Controls.Add(Me.b_UpdateFirmware)
        Me.p_About.Controls.Add(Me.b_UpdateCheck)
        Me.p_About.Controls.Add(Me.l_FWString)
        Me.p_About.Controls.Add(Me.Label29)
        Me.p_About.Controls.Add(Me.l_myVersion)
        Me.p_About.Controls.Add(Me.Label28)
        Me.p_About.Controls.Add(Me.Label27)
        Me.p_About.Location = New System.Drawing.Point(0, 130)
        Me.p_About.Name = "p_About"
        Me.p_About.Size = New System.Drawing.Size(784, 331)
        Me.p_About.TabIndex = 127
        Me.p_About.Visible = False
        '
        'Button1
        '
        Me.Button1.BackColor = System.Drawing.Color.Teal
        Me.Button1.FlatAppearance.BorderSize = 0
        Me.Button1.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.Button1.ForeColor = System.Drawing.Color.White
        Me.Button1.Location = New System.Drawing.Point(675, 17)
        Me.Button1.Name = "Button1"
        Me.Button1.Size = New System.Drawing.Size(99, 53)
        Me.Button1.TabIndex = 133
        Me.Button1.Text = "Do absolutely nothing"
        Me.Button1.UseVisualStyleBackColor = False
        '
        'b_UserManual
        '
        Me.b_UserManual.BackColor = System.Drawing.Color.Teal
        Me.b_UserManual.FlatAppearance.BorderSize = 0
        Me.b_UserManual.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.b_UserManual.ForeColor = System.Drawing.Color.White
        Me.b_UserManual.Location = New System.Drawing.Point(14, 17)
        Me.b_UserManual.Name = "b_UserManual"
        Me.b_UserManual.Size = New System.Drawing.Size(99, 53)
        Me.b_UserManual.TabIndex = 132
        Me.b_UserManual.Text = "Get the " & Global.Microsoft.VisualBasic.ChrW(13) & Global.Microsoft.VisualBasic.ChrW(10) & "User Manual"
        Me.b_UserManual.UseVisualStyleBackColor = False
        '
        'b_UpdateFirmware
        '
        Me.b_UpdateFirmware.BackColor = System.Drawing.Color.Teal
        Me.b_UpdateFirmware.FlatAppearance.BorderSize = 0
        Me.b_UpdateFirmware.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.b_UpdateFirmware.ForeColor = System.Drawing.Color.White
        Me.b_UpdateFirmware.Location = New System.Drawing.Point(673, 266)
        Me.b_UpdateFirmware.Name = "b_UpdateFirmware"
        Me.b_UpdateFirmware.Size = New System.Drawing.Size(99, 53)
        Me.b_UpdateFirmware.TabIndex = 131
        Me.b_UpdateFirmware.Text = "Firmware Update"
        Me.b_UpdateFirmware.UseVisualStyleBackColor = False
        '
        'b_UpdateCheck
        '
        Me.b_UpdateCheck.BackColor = System.Drawing.Color.Teal
        Me.b_UpdateCheck.FlatAppearance.BorderSize = 0
        Me.b_UpdateCheck.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.b_UpdateCheck.ForeColor = System.Drawing.Color.White
        Me.b_UpdateCheck.Location = New System.Drawing.Point(14, 266)
        Me.b_UpdateCheck.Name = "b_UpdateCheck"
        Me.b_UpdateCheck.Size = New System.Drawing.Size(99, 53)
        Me.b_UpdateCheck.TabIndex = 130
        Me.b_UpdateCheck.Text = "Check for Updates"
        Me.b_UpdateCheck.UseVisualStyleBackColor = False
        '
        'l_FWString
        '
        Me.l_FWString.AutoSize = True
        Me.l_FWString.Font = New System.Drawing.Font("Arial", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.l_FWString.ForeColor = System.Drawing.Color.Teal
        Me.l_FWString.Location = New System.Drawing.Point(452, 203)
        Me.l_FWString.Name = "l_FWString"
        Me.l_FWString.Size = New System.Drawing.Size(72, 16)
        Me.l_FWString.TabIndex = 4
        Me.l_FWString.Text = "#_fwstring"
        '
        'Label29
        '
        Me.Label29.AutoSize = True
        Me.Label29.Font = New System.Drawing.Font("Arial", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label29.ForeColor = System.Drawing.Color.Teal
        Me.Label29.Location = New System.Drawing.Point(246, 203)
        Me.Label29.Name = "Label29"
        Me.Label29.Size = New System.Drawing.Size(188, 16)
        Me.Label29.TabIndex = 3
        Me.Label29.Text = "OpenVFD Firmware Version:"
        '
        'l_myVersion
        '
        Me.l_myVersion.AutoSize = True
        Me.l_myVersion.Font = New System.Drawing.Font("Arial", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.l_myVersion.ForeColor = System.Drawing.Color.Teal
        Me.l_myVersion.Location = New System.Drawing.Point(452, 187)
        Me.l_myVersion.Name = "l_myVersion"
        Me.l_myVersion.Size = New System.Drawing.Size(64, 16)
        Me.l_myVersion.TabIndex = 2
        Me.l_myVersion.Text = "#_vstring"
        '
        'Label28
        '
        Me.Label28.AutoSize = True
        Me.Label28.Font = New System.Drawing.Font("Arial", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label28.ForeColor = System.Drawing.Color.Teal
        Me.Label28.Location = New System.Drawing.Point(246, 187)
        Me.Label28.Name = "Label28"
        Me.Label28.Size = New System.Drawing.Size(142, 16)
        Me.Label28.TabIndex = 1
        Me.Label28.Text = "myOpenVFD Version:"
        '
        'Label27
        '
        Me.Label27.AutoSize = True
        Me.Label27.Cursor = System.Windows.Forms.Cursors.Hand
        Me.Label27.Font = New System.Drawing.Font("Arial", 72.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label27.ForeColor = System.Drawing.Color.Purple
        Me.Label27.Location = New System.Drawing.Point(226, 75)
        Me.Label27.Name = "Label27"
        Me.Label27.Size = New System.Drawing.Size(323, 111)
        Me.Label27.TabIndex = 0
        Me.Label27.Text = "About"
        '
        'p_Buttons
        '
        Me.p_Buttons.Controls.Add(Me.b_SetColors)
        Me.p_Buttons.Controls.Add(Me.b_Presets)
        Me.p_Buttons.Controls.Add(Me.b_About)
        Me.p_Buttons.Controls.Add(Me.b_TimeSync)
        Me.p_Buttons.Controls.Add(Me.b_SendMsg)
        Me.p_Buttons.Controls.Add(Me.b_SaveSettings)
        Me.p_Buttons.Enabled = False
        Me.p_Buttons.Location = New System.Drawing.Point(261, 12)
        Me.p_Buttons.Name = "p_Buttons"
        Me.p_Buttons.Size = New System.Drawing.Size(523, 103)
        Me.p_Buttons.TabIndex = 130
        '
        'scrTimer
        '
        Me.scrTimer.Interval = 1250
        '
        'scrSimulator
        '
        Me.scrSimulator.Enabled = True
        Me.scrSimulator.Interval = 2000
        '
        'timeNightShiftDisable
        '
        Me.timeNightShiftDisable.CalendarFont = New System.Drawing.Font("Arial", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.timeNightShiftDisable.CalendarTitleBackColor = System.Drawing.SystemColors.ControlText
        Me.timeNightShiftDisable.CalendarTitleForeColor = System.Drawing.Color.Purple
        Me.timeNightShiftDisable.CustomFormat = "HH:mm"
        Me.timeNightShiftDisable.Font = New System.Drawing.Font("Arial", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.timeNightShiftDisable.Format = System.Windows.Forms.DateTimePickerFormat.Custom
        Me.timeNightShiftDisable.Location = New System.Drawing.Point(48, 122)
        Me.timeNightShiftDisable.Name = "timeNightShiftDisable"
        Me.timeNightShiftDisable.ShowUpDown = True
        Me.timeNightShiftDisable.Size = New System.Drawing.Size(56, 22)
        Me.timeNightShiftDisable.TabIndex = 10
        Me.timeNightShiftDisable.Value = New Date(2018, 3, 20, 8, 0, 0, 0)
        '
        'Label34
        '
        Me.Label34.AutoSize = True
        Me.Label34.Location = New System.Drawing.Point(9, 124)
        Me.Label34.Name = "Label34"
        Me.Label34.Size = New System.Drawing.Size(36, 16)
        Me.Label34.TabIndex = 9
        Me.Label34.Text = "until"
        '
        'cb_leadingZero
        '
        Me.cb_leadingZero.AutoSize = True
        Me.cb_leadingZero.Enabled = False
        Me.cb_leadingZero.ForeColor = System.Drawing.Color.Purple
        Me.cb_leadingZero.Location = New System.Drawing.Point(12, 44)
        Me.cb_leadingZero.Name = "cb_leadingZero"
        Me.cb_leadingZero.Size = New System.Drawing.Size(140, 20)
        Me.cb_leadingZero.TabIndex = 5
        Me.cb_leadingZero.Text = "Enable leading '0'"
        Me.cb_leadingZero.UseVisualStyleBackColor = True
        '
        'Label35
        '
        Me.Label35.AutoSize = True
        Me.Label35.Location = New System.Drawing.Point(109, 124)
        Me.Label35.Name = "Label35"
        Me.Label35.Size = New System.Drawing.Size(93, 16)
        Me.Label35.TabIndex = 11
        Me.Label35.Text = "next morning"
        '
        'OpenVFD
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(7.0!, 16.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.BackColor = System.Drawing.Color.White
        Me.ClientSize = New System.Drawing.Size(784, 461)
        Me.Controls.Add(Me.p_SaveSettings)
        Me.Controls.Add(Me.p_SetColors)
        Me.Controls.Add(Me.p_Presets)
        Me.Controls.Add(Me.p_Msg)
        Me.Controls.Add(Me.p_About)
        Me.Controls.Add(Me.p_Buttons)
        Me.Controls.Add(Me.p_TimeSync)
        Me.Controls.Add(Me.GroupBox3)
        Me.Controls.Add(Me.Panel2)
        Me.Font = New System.Drawing.Font("Arial", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.ForeColor = System.Drawing.Color.FromArgb(CType(CType(32, Byte), Integer), CType(CType(32, Byte), Integer), CType(CType(32, Byte), Integer))
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle
        Me.Margin = New System.Windows.Forms.Padding(3, 4, 3, 4)
        Me.MaximizeBox = False
        Me.Name = "OpenVFD"
        Me.ShowIcon = False
        Me.Text = "myOpenVFD"
        Me.GroupBox3.ResumeLayout(False)
        Me.GroupBox3.PerformLayout()
        Me.p_SetColors.ResumeLayout(False)
        Me.p_SetColors.PerformLayout()
        Me.p_Msg.ResumeLayout(False)
        Me.p_Msg.PerformLayout()
        Me.p_TimeSync.ResumeLayout(False)
        Me.p_TimeSync.PerformLayout()
        Me.p_Presets.ResumeLayout(False)
        Me.tab_ColorPresets.ResumeLayout(False)
        Me.tab_Static.ResumeLayout(False)
        Me.tab_Static.PerformLayout()
        CType(Me.pre_pastel, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.pre_green2blue, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.pre_red2blue, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.pre_red2green, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.pre_Rainbow, System.ComponentModel.ISupportInitialize).EndInit()
        Me.tab_Dynamic.ResumeLayout(False)
        Me.tab_Dynamic.PerformLayout()
        Me.p_LEDMic.ResumeLayout(False)
        Me.p_LEDMic.PerformLayout()
        CType(Me.dancingTrackBar, System.ComponentModel.ISupportInitialize).EndInit()
        Me.p_LEDs.ResumeLayout(False)
        Me.p_LEDs.PerformLayout()
        CType(Me.note, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.speaker, System.ComponentModel.ISupportInitialize).EndInit()
        Me.p_LEDCross.ResumeLayout(False)
        Me.p_LEDCross.PerformLayout()
        CType(Me.crossTrackBar, System.ComponentModel.ISupportInitialize).EndInit()
        Me.p_LEDSpectrum.ResumeLayout(False)
        Me.p_LEDSpectrum.PerformLayout()
        Me.p_LEDCop.ResumeLayout(False)
        Me.p_LEDCop.PerformLayout()
        Me.p_LEDResistorCode.ResumeLayout(False)
        Me.p_Resistors.ResumeLayout(False)
        Me.p_Resistors.PerformLayout()
        Me.p_ResistorsMore.ResumeLayout(False)
        Me.p_ResistorsMore.PerformLayout()
        CType(Me.PictureBox1, System.ComponentModel.ISupportInitialize).EndInit()
        Me.p_LEDChaseFade.ResumeLayout(False)
        Me.p_LEDChaseFade.PerformLayout()
        Me.tab_Ambient.ResumeLayout(False)
        CType(Me.pImage, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.PictureBox2, System.ComponentModel.ISupportInitialize).EndInit()
        Me.p_SaveSettings.ResumeLayout(False)
        Me.p_SaveSettings.PerformLayout()
        Me.GroupBox2.ResumeLayout(False)
        Me.GroupBox2.PerformLayout()
        Me.GroupBox1.ResumeLayout(False)
        Me.GroupBox1.PerformLayout()
        Me.Panel2.ResumeLayout(False)
        Me.Panel2.PerformLayout()
        Me.p_About.ResumeLayout(False)
        Me.p_About.PerformLayout()
        Me.p_Buttons.ResumeLayout(False)
        Me.ResumeLayout(False)

    End Sub

    Friend WithEvents GroupBox3 As GroupBox
    Friend WithEvents l_connect As Label
    Friend WithEvents txt_SerialPorts As ComboBox
    Friend WithEvents Label2 As Label
    Friend WithEvents comPort As IO.Ports.SerialPort
    Friend WithEvents p_SetColors As Panel
    Friend WithEvents Label3 As Label
    Friend WithEvents ledEditor As Cyotek.Windows.Forms.ColorEditor
    Friend WithEvents ledWheel As Cyotek.Windows.Forms.ColorWheel
    Friend WithEvents ledManager As Cyotek.Windows.Forms.ColorEditorManager
    Friend WithEvents p_Msg As Panel
    Friend WithEvents txt_Msg As FilterTextBox.FilterTextBox
    Friend WithEvents Label5 As Label
    Friend WithEvents Label4 As Label
    Friend WithEvents Label1 As Label
    Friend WithEvents txt_DisplayLength As FilterTextBox.FilterTextBox
    Friend WithEvents Label6 As Label
    Friend WithEvents p_TimeSync As Panel
    Friend WithEvents Label7 As Label
    Friend WithEvents l_SyncStatus As Label
    Friend WithEvents p_Presets As Panel
    Friend WithEvents b_SetColors As Button
    Friend WithEvents b_SendMsg As Button
    Friend WithEvents b_TimeSync As Button
    Friend WithEvents b_Presets As Button
    Friend WithEvents b_Connect As Button
    Friend WithEvents b_Msg As Button
    Friend WithEvents b_Sync As Button
    Friend WithEvents l_Date As Label
    Friend WithEvents l_Time As Label
    Friend WithEvents timeTimer As Timer
    Friend WithEvents tab_ColorPresets As TabControl
    Friend WithEvents tab_Static As TabPage
    Friend WithEvents tab_Dynamic As TabPage
    Friend WithEvents Label8 As Label
    Friend WithEvents pre_Rainbow As PictureBox
    Friend WithEvents pre_red2green As PictureBox
    Friend WithEvents pre_red2blue As PictureBox
    Friend WithEvents pre_green2blue As PictureBox
    Friend WithEvents pre_pastel As PictureBox
    Friend WithEvents pre_warmwhite As Label
    Friend WithEvents pre_white As Label
    Friend WithEvents pre_off As Label
    Friend WithEvents pre_purple As Label
    Friend WithEvents pre_magenta As Label
    Friend WithEvents pre_cyan As Label
    Friend WithEvents pre_orange As Label
    Friend WithEvents pre_yellow As Label
    Friend WithEvents pre_blue As Label
    Friend WithEvents pre_green As Label
    Friend WithEvents pre_red As Label
    Friend WithEvents bS_Chase As Button
    Friend WithEvents bS_Cross As Button
    Friend WithEvents bS_Spectrum As Button
    Friend WithEvents bS_Cop As Button
    Friend WithEvents bS_Mic As Button
    Friend WithEvents bS_RCode As Button
    Friend WithEvents p_LEDs As Panel
    Friend WithEvents ledTimer As Timer
    Friend WithEvents Label9 As Label
    Friend WithEvents Label10 As Label
    Friend WithEvents ledSubTimer As Timer
    Friend WithEvents speaker As PictureBox
    Friend WithEvents note As PictureBox
    Friend WithEvents p_LEDCross As Panel
    Friend WithEvents b_Cross As Button
    Friend WithEvents p_LEDSpectrum As Panel
    Friend WithEvents b_Spectrum As Button
    Friend WithEvents p_LEDResistorCode As Panel
    Friend WithEvents PictureBox1 As PictureBox
    Friend WithEvents b_Resistor As Button
    Friend WithEvents p_LEDChaseFade As Panel
    Friend WithEvents b_ChaseFade As Button
    Friend WithEvents cb_ChFdMode As ComboBox
    Friend WithEvents l_chFdInfo As Label
    Friend WithEvents Label14 As Label
    Friend WithEvents p_Resistors As Panel
    Friend WithEvents LinkLabel1 As LinkLabel
    Friend WithEvents Label15 As Label
    Friend WithEvents p_ResistorsMore As Panel
    Friend WithEvents LinkLabel2 As LinkLabel
    Friend WithEvents Label24 As Label
    Friend WithEvents Label19 As Label
    Friend WithEvents Label18 As Label
    Friend WithEvents Label17 As Label
    Friend WithEvents Label16 As Label
    Friend WithEvents p_LEDMic As Panel
    Friend WithEvents b_Mic As Button
    Friend WithEvents Label21 As Label
    Friend WithEvents Label22 As Label
    Friend WithEvents b_SaveSettings As Button
    Friend WithEvents p_SaveSettings As Panel
    Friend WithEvents l_Reset As Label
    Friend WithEvents l_Save2 As Label
    Friend WithEvents l_Save1 As Label
    Friend WithEvents Label13 As Label
    Friend WithEvents Panel2 As Panel
    Friend WithEvents Label23 As Label
    Friend WithEvents p_LEDCop As Panel
    Friend WithEvents b_Cop As Button
    Friend WithEvents cb_CopMode As ComboBox
    Friend WithEvents Label25 As Label
    Friend WithEvents Label26 As Label
    Friend WithEvents b_About As Button
    Friend WithEvents p_About As Panel
    Friend WithEvents Label27 As Label
    Friend WithEvents l_FWString As Label
    Friend WithEvents Label29 As Label
    Friend WithEvents l_myVersion As Label
    Friend WithEvents Label28 As Label
    Friend WithEvents b_UpdateFirmware As Button
    Friend WithEvents b_UpdateCheck As Button
    Friend WithEvents p_Buttons As Panel
    Friend WithEvents b_UserManual As Button
    Friend WithEvents Button1 As Button
    Friend WithEvents tab_Ambient As TabPage
    Friend WithEvents scrTimer As Timer
    Friend WithEvents PictureBox2 As PictureBox
    Friend WithEvents l_MoodStatus As Label
    Friend WithEvents Label30 As Label
    Friend WithEvents p_Mood As Panel
    Friend WithEvents pImage As PictureBox
    Friend WithEvents scrSimulator As Timer
    Friend WithEvents Label31 As Label
    Friend WithEvents Label20 As Label
    Friend WithEvents dancingTrackBar As TrackBar
    Friend WithEvents Label11 As Label
    Friend WithEvents Label12 As Label
    Friend WithEvents crossTrackBar As TrackBar
    Friend WithEvents Label32 As Label
    Friend WithEvents check_LED1 As CheckBox
    Friend WithEvents check_LED2 As CheckBox
    Friend WithEvents check_LED3 As CheckBox
    Friend WithEvents check_LED4 As CheckBox
    Friend WithEvents check_LED5 As CheckBox
    Friend WithEvents check_LED6 As CheckBox
    Friend WithEvents cb_customWelcome As CheckBox
    Friend WithEvents timeNightShiftEnable As DateTimePicker
    Friend WithEvents GroupBox2 As GroupBox
    Friend WithEvents Label33 As Label
    Friend WithEvents cb_scheduledNightShift As CheckBox
    Friend WithEvents cb_nightShiftToggle As CheckBox
    Friend WithEvents GroupBox1 As GroupBox
    Friend WithEvents cb_12h As CheckBox
    Friend WithEvents cb_intd As CheckBox
    Friend WithEvents timeNightShiftDisable As DateTimePicker
    Friend WithEvents Label34 As Label
    Friend WithEvents cb_leadingZero As CheckBox
    Friend WithEvents Label35 As Label
End Class
