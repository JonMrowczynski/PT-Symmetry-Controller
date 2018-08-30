<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class Form1
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
		Me.components = New System.ComponentModel.Container
		Me.Start_Button = New System.Windows.Forms.Button
		Me.Stop_Button = New System.Windows.Forms.Button
		Me.Graph_Canvas = New System.Windows.Forms.PictureBox
		Me.MenuStrip1 = New System.Windows.Forms.MenuStrip
		Me.DeviceToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem
		Me.NewToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem
		Me.ToolStripSeparator1 = New System.Windows.Forms.ToolStripSeparator
		Me.ExitToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem
		Me.DevicesToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem
		Me.AlphaToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem
		Me.BetaToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem
		Me.ChannelsToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem
		Me.Analog15vToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem
		Me.Analog110vToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem
		Me.Analog25vToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem
		Me.Analog35vToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem
		Me.Analog45vToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem
		Me.Digital1photogateToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem
		Me.Digital1MotionToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem
		Me.Digital1RotaryToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem
		Me.Digital1RotaryX4ToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem
		Me.Digital1RadiationToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem
		Me.Digital2photogateToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem
		Me.Digital2MotionToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem
		Me.Digital2RotaryToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem
		Me.Digital2RotaryX4ToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem
		Me.Digital2RadiationToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem
		Me.AudiointernalToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem
		Me.ActionToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem
		Me.GetStatusToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem
		Me.SetMeasurementPeriodToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem
		Me.SetDisplayDepthToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem
		Me.StartToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem
		Me.StopMeasurementsToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem
		Me.HelpToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem
		Me.NGIOLibVerboseToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem
		Me.AboutNGIOMeasureToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem
		Me.Timer1 = New System.Windows.Forms.Timer(Me.components)
		Me.MeasBox = New System.Windows.Forms.Label
		Me.UnitsLabel = New System.Windows.Forms.Label
		Me.CalibrationsToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem
		Me.Calib0ToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem
		Me.Calib1ToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem
		Me.Calib2ToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem
		CType(Me.Graph_Canvas, System.ComponentModel.ISupportInitialize).BeginInit()
		Me.MenuStrip1.SuspendLayout()
		Me.SuspendLayout()
		'
		'Start_Button
		'
		Me.Start_Button.Location = New System.Drawing.Point(21, 27)
		Me.Start_Button.Name = "Start_Button"
		Me.Start_Button.Size = New System.Drawing.Size(75, 23)
		Me.Start_Button.TabIndex = 0
		Me.Start_Button.Text = "Start"
		Me.Start_Button.UseVisualStyleBackColor = True
		'
		'Stop_Button
		'
		Me.Stop_Button.Location = New System.Drawing.Point(102, 27)
		Me.Stop_Button.Name = "Stop_Button"
		Me.Stop_Button.Size = New System.Drawing.Size(75, 23)
		Me.Stop_Button.TabIndex = 1
		Me.Stop_Button.Text = "Stop"
		Me.Stop_Button.UseVisualStyleBackColor = True
		'
		'Graph_Canvas
		'
		Me.Graph_Canvas.BackColor = System.Drawing.SystemColors.Window
		Me.Graph_Canvas.Location = New System.Drawing.Point(13, 56)
		Me.Graph_Canvas.Name = "Graph_Canvas"
		Me.Graph_Canvas.Size = New System.Drawing.Size(782, 446)
		Me.Graph_Canvas.TabIndex = 2
		Me.Graph_Canvas.TabStop = False
		'
		'MenuStrip1
		'
		Me.MenuStrip1.Items.AddRange(New System.Windows.Forms.ToolStripItem() {Me.DeviceToolStripMenuItem, Me.DevicesToolStripMenuItem, Me.ChannelsToolStripMenuItem, Me.CalibrationsToolStripMenuItem, Me.ActionToolStripMenuItem, Me.HelpToolStripMenuItem})
		Me.MenuStrip1.Location = New System.Drawing.Point(0, 0)
		Me.MenuStrip1.Name = "MenuStrip1"
		Me.MenuStrip1.Size = New System.Drawing.Size(807, 24)
		Me.MenuStrip1.TabIndex = 3
		Me.MenuStrip1.Text = "MenuStrip1"
		'
		'DeviceToolStripMenuItem
		'
		Me.DeviceToolStripMenuItem.DropDownItems.AddRange(New System.Windows.Forms.ToolStripItem() {Me.NewToolStripMenuItem, Me.ToolStripSeparator1, Me.ExitToolStripMenuItem})
		Me.DeviceToolStripMenuItem.Name = "DeviceToolStripMenuItem"
		Me.DeviceToolStripMenuItem.Size = New System.Drawing.Size(35, 20)
		Me.DeviceToolStripMenuItem.Text = "File"
		'
		'NewToolStripMenuItem
		'
		Me.NewToolStripMenuItem.Name = "NewToolStripMenuItem"
		Me.NewToolStripMenuItem.Size = New System.Drawing.Size(106, 22)
		Me.NewToolStripMenuItem.Text = "New"
		'
		'ToolStripSeparator1
		'
		Me.ToolStripSeparator1.Name = "ToolStripSeparator1"
		Me.ToolStripSeparator1.Size = New System.Drawing.Size(103, 6)
		'
		'ExitToolStripMenuItem
		'
		Me.ExitToolStripMenuItem.Name = "ExitToolStripMenuItem"
		Me.ExitToolStripMenuItem.Size = New System.Drawing.Size(106, 22)
		Me.ExitToolStripMenuItem.Text = "Exit"
		'
		'DevicesToolStripMenuItem
		'
		Me.DevicesToolStripMenuItem.DropDownItems.AddRange(New System.Windows.Forms.ToolStripItem() {Me.AlphaToolStripMenuItem, Me.BetaToolStripMenuItem})
		Me.DevicesToolStripMenuItem.Name = "DevicesToolStripMenuItem"
		Me.DevicesToolStripMenuItem.Size = New System.Drawing.Size(56, 20)
		Me.DevicesToolStripMenuItem.Text = "Devices"
		'
		'AlphaToolStripMenuItem
		'
		Me.AlphaToolStripMenuItem.Name = "AlphaToolStripMenuItem"
		Me.AlphaToolStripMenuItem.Size = New System.Drawing.Size(111, 22)
		Me.AlphaToolStripMenuItem.Text = "alpha"
		'
		'BetaToolStripMenuItem
		'
		Me.BetaToolStripMenuItem.Name = "BetaToolStripMenuItem"
		Me.BetaToolStripMenuItem.Size = New System.Drawing.Size(111, 22)
		Me.BetaToolStripMenuItem.Text = "beta"
		'
		'ChannelsToolStripMenuItem
		'
		Me.ChannelsToolStripMenuItem.DropDownItems.AddRange(New System.Windows.Forms.ToolStripItem() {Me.Analog15vToolStripMenuItem, Me.Analog110vToolStripMenuItem, Me.Analog25vToolStripMenuItem, Me.Analog35vToolStripMenuItem, Me.Analog45vToolStripMenuItem, Me.Digital1photogateToolStripMenuItem, Me.Digital1MotionToolStripMenuItem, Me.Digital1RotaryToolStripMenuItem, Me.Digital1RotaryX4ToolStripMenuItem, Me.Digital1RadiationToolStripMenuItem, Me.Digital2photogateToolStripMenuItem, Me.Digital2MotionToolStripMenuItem, Me.Digital2RotaryToolStripMenuItem, Me.Digital2RotaryX4ToolStripMenuItem, Me.Digital2RadiationToolStripMenuItem, Me.AudiointernalToolStripMenuItem})
		Me.ChannelsToolStripMenuItem.Name = "ChannelsToolStripMenuItem"
		Me.ChannelsToolStripMenuItem.Size = New System.Drawing.Size(63, 20)
		Me.ChannelsToolStripMenuItem.Text = "Channels"
		'
		'Analog15vToolStripMenuItem
		'
		Me.Analog15vToolStripMenuItem.Name = "Analog15vToolStripMenuItem"
		Me.Analog15vToolStripMenuItem.Size = New System.Drawing.Size(181, 22)
		Me.Analog15vToolStripMenuItem.Text = "Analog1 (5v)"
		'
		'Analog110vToolStripMenuItem
		'
		Me.Analog110vToolStripMenuItem.Name = "Analog110vToolStripMenuItem"
		Me.Analog110vToolStripMenuItem.Size = New System.Drawing.Size(181, 22)
		Me.Analog110vToolStripMenuItem.Text = "Analog1 (+/-10v)"
		'
		'Analog25vToolStripMenuItem
		'
		Me.Analog25vToolStripMenuItem.Name = "Analog25vToolStripMenuItem"
		Me.Analog25vToolStripMenuItem.Size = New System.Drawing.Size(181, 22)
		Me.Analog25vToolStripMenuItem.Text = "Analog2 (5v)"
		'
		'Analog35vToolStripMenuItem
		'
		Me.Analog35vToolStripMenuItem.Name = "Analog35vToolStripMenuItem"
		Me.Analog35vToolStripMenuItem.Size = New System.Drawing.Size(181, 22)
		Me.Analog35vToolStripMenuItem.Text = "Analog3 (5v)"
		'
		'Analog45vToolStripMenuItem
		'
		Me.Analog45vToolStripMenuItem.Name = "Analog45vToolStripMenuItem"
		Me.Analog45vToolStripMenuItem.Size = New System.Drawing.Size(181, 22)
		Me.Analog45vToolStripMenuItem.Text = "Analog4 (5v)"
		'
		'Digital1photogateToolStripMenuItem
		'
		Me.Digital1photogateToolStripMenuItem.Name = "Digital1photogateToolStripMenuItem"
		Me.Digital1photogateToolStripMenuItem.Size = New System.Drawing.Size(181, 22)
		Me.Digital1photogateToolStripMenuItem.Text = "Digital1 (photogate)"
		'
		'Digital1MotionToolStripMenuItem
		'
		Me.Digital1MotionToolStripMenuItem.Name = "Digital1MotionToolStripMenuItem"
		Me.Digital1MotionToolStripMenuItem.Size = New System.Drawing.Size(181, 22)
		Me.Digital1MotionToolStripMenuItem.Text = "Digital1 (Motion)"
		'
		'Digital1RotaryToolStripMenuItem
		'
		Me.Digital1RotaryToolStripMenuItem.Name = "Digital1RotaryToolStripMenuItem"
		Me.Digital1RotaryToolStripMenuItem.Size = New System.Drawing.Size(181, 22)
		Me.Digital1RotaryToolStripMenuItem.Text = "Digital1 (Rotary)"
		'
		'Digital1RotaryX4ToolStripMenuItem
		'
		Me.Digital1RotaryX4ToolStripMenuItem.Name = "Digital1RotaryX4ToolStripMenuItem"
		Me.Digital1RotaryX4ToolStripMenuItem.Size = New System.Drawing.Size(181, 22)
		Me.Digital1RotaryX4ToolStripMenuItem.Text = "Digital1 (RotaryX4)"
		'
		'Digital1RadiationToolStripMenuItem
		'
		Me.Digital1RadiationToolStripMenuItem.Name = "Digital1RadiationToolStripMenuItem"
		Me.Digital1RadiationToolStripMenuItem.Size = New System.Drawing.Size(181, 22)
		Me.Digital1RadiationToolStripMenuItem.Text = "Digital1 (Radiation)"
		'
		'Digital2photogateToolStripMenuItem
		'
		Me.Digital2photogateToolStripMenuItem.Name = "Digital2photogateToolStripMenuItem"
		Me.Digital2photogateToolStripMenuItem.Size = New System.Drawing.Size(181, 22)
		Me.Digital2photogateToolStripMenuItem.Text = "Digital2 (photogate)"
		'
		'Digital2MotionToolStripMenuItem
		'
		Me.Digital2MotionToolStripMenuItem.Name = "Digital2MotionToolStripMenuItem"
		Me.Digital2MotionToolStripMenuItem.Size = New System.Drawing.Size(181, 22)
		Me.Digital2MotionToolStripMenuItem.Text = "Digital2 (Motion)"
		'
		'Digital2RotaryToolStripMenuItem
		'
		Me.Digital2RotaryToolStripMenuItem.Name = "Digital2RotaryToolStripMenuItem"
		Me.Digital2RotaryToolStripMenuItem.Size = New System.Drawing.Size(181, 22)
		Me.Digital2RotaryToolStripMenuItem.Text = "Digital2 (Rotary)"
		'
		'Digital2RotaryX4ToolStripMenuItem
		'
		Me.Digital2RotaryX4ToolStripMenuItem.Name = "Digital2RotaryX4ToolStripMenuItem"
		Me.Digital2RotaryX4ToolStripMenuItem.Size = New System.Drawing.Size(181, 22)
		Me.Digital2RotaryX4ToolStripMenuItem.Text = "Digital2 (RotaryX4)"
		'
		'Digital2RadiationToolStripMenuItem
		'
		Me.Digital2RadiationToolStripMenuItem.Name = "Digital2RadiationToolStripMenuItem"
		Me.Digital2RadiationToolStripMenuItem.Size = New System.Drawing.Size(181, 22)
		Me.Digital2RadiationToolStripMenuItem.Text = "Digital2 (Radiation)"
		'
		'AudiointernalToolStripMenuItem
		'
		Me.AudiointernalToolStripMenuItem.Name = "AudiointernalToolStripMenuItem"
		Me.AudiointernalToolStripMenuItem.Size = New System.Drawing.Size(181, 22)
		Me.AudiointernalToolStripMenuItem.Text = "Audio (internal)"
		'
		'ActionToolStripMenuItem
		'
		Me.ActionToolStripMenuItem.DropDownItems.AddRange(New System.Windows.Forms.ToolStripItem() {Me.GetStatusToolStripMenuItem, Me.SetMeasurementPeriodToolStripMenuItem, Me.SetDisplayDepthToolStripMenuItem, Me.StartToolStripMenuItem, Me.StopMeasurementsToolStripMenuItem})
		Me.ActionToolStripMenuItem.Name = "ActionToolStripMenuItem"
		Me.ActionToolStripMenuItem.Size = New System.Drawing.Size(49, 20)
		Me.ActionToolStripMenuItem.Text = "Action"
		'
		'GetStatusToolStripMenuItem
		'
		Me.GetStatusToolStripMenuItem.Name = "GetStatusToolStripMenuItem"
		Me.GetStatusToolStripMenuItem.Size = New System.Drawing.Size(202, 22)
		Me.GetStatusToolStripMenuItem.Text = "Get Status"
		'
		'SetMeasurementPeriodToolStripMenuItem
		'
		Me.SetMeasurementPeriodToolStripMenuItem.Name = "SetMeasurementPeriodToolStripMenuItem"
		Me.SetMeasurementPeriodToolStripMenuItem.Size = New System.Drawing.Size(202, 22)
		Me.SetMeasurementPeriodToolStripMenuItem.Text = "Set Measurement Period"
		'
		'SetDisplayDepthToolStripMenuItem
		'
		Me.SetDisplayDepthToolStripMenuItem.Name = "SetDisplayDepthToolStripMenuItem"
		Me.SetDisplayDepthToolStripMenuItem.Size = New System.Drawing.Size(202, 22)
		Me.SetDisplayDepthToolStripMenuItem.Text = "Set Display Depth"
		'
		'StartToolStripMenuItem
		'
		Me.StartToolStripMenuItem.Name = "StartToolStripMenuItem"
		Me.StartToolStripMenuItem.Size = New System.Drawing.Size(202, 22)
		Me.StartToolStripMenuItem.Text = "Start  Measurements"
		'
		'StopMeasurementsToolStripMenuItem
		'
		Me.StopMeasurementsToolStripMenuItem.Name = "StopMeasurementsToolStripMenuItem"
		Me.StopMeasurementsToolStripMenuItem.Size = New System.Drawing.Size(202, 22)
		Me.StopMeasurementsToolStripMenuItem.Text = "Stop Measurements"
		'
		'HelpToolStripMenuItem
		'
		Me.HelpToolStripMenuItem.DropDownItems.AddRange(New System.Windows.Forms.ToolStripItem() {Me.NGIOLibVerboseToolStripMenuItem, Me.AboutNGIOMeasureToolStripMenuItem})
		Me.HelpToolStripMenuItem.Name = "HelpToolStripMenuItem"
		Me.HelpToolStripMenuItem.Size = New System.Drawing.Size(40, 20)
		Me.HelpToolStripMenuItem.Text = "Help"
		'
		'NGIOLibVerboseToolStripMenuItem
		'
		Me.NGIOLibVerboseToolStripMenuItem.Name = "NGIOLibVerboseToolStripMenuItem"
		Me.NGIOLibVerboseToolStripMenuItem.Size = New System.Drawing.Size(199, 22)
		Me.NGIOLibVerboseToolStripMenuItem.Text = "NGIO lib verbose"
		'
		'AboutNGIOMeasureToolStripMenuItem
		'
		Me.AboutNGIOMeasureToolStripMenuItem.Name = "AboutNGIOMeasureToolStripMenuItem"
		Me.AboutNGIOMeasureToolStripMenuItem.Size = New System.Drawing.Size(199, 22)
		Me.AboutNGIOMeasureToolStripMenuItem.Text = "About NGIO Measure..."
		'
		'Timer1
		'
		Me.Timer1.Interval = 80
		'
		'MeasBox
		'
		Me.MeasBox.AutoSize = True
		Me.MeasBox.BackColor = System.Drawing.SystemColors.Window
		Me.MeasBox.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
		Me.MeasBox.Font = New System.Drawing.Font("Courier New", 9.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.MeasBox.Location = New System.Drawing.Point(204, 29)
		Me.MeasBox.MaximumSize = New System.Drawing.Size(61, 18)
		Me.MeasBox.MinimumSize = New System.Drawing.Size(61, 18)
		Me.MeasBox.Name = "MeasBox"
		Me.MeasBox.Size = New System.Drawing.Size(61, 18)
		Me.MeasBox.TabIndex = 4
		Me.MeasBox.TextAlign = System.Drawing.ContentAlignment.TopRight
		'
		'UnitsLabel
		'
		Me.UnitsLabel.AutoSize = True
		Me.UnitsLabel.Location = New System.Drawing.Point(271, 31)
		Me.UnitsLabel.Name = "UnitsLabel"
		Me.UnitsLabel.Size = New System.Drawing.Size(0, 13)
		Me.UnitsLabel.TabIndex = 5
		'
		'CalibrationsToolStripMenuItem
		'
		Me.CalibrationsToolStripMenuItem.DropDownItems.AddRange(New System.Windows.Forms.ToolStripItem() {Me.Calib0ToolStripMenuItem, Me.Calib1ToolStripMenuItem, Me.Calib2ToolStripMenuItem})
		Me.CalibrationsToolStripMenuItem.Name = "CalibrationsToolStripMenuItem"
		Me.CalibrationsToolStripMenuItem.Size = New System.Drawing.Size(75, 20)
		Me.CalibrationsToolStripMenuItem.Text = "Calibrations"
		'
		'Calib0ToolStripMenuItem
		'
		Me.Calib0ToolStripMenuItem.Name = "Calib0ToolStripMenuItem"
		Me.Calib0ToolStripMenuItem.Size = New System.Drawing.Size(152, 22)
		Me.Calib0ToolStripMenuItem.Text = "Calib0"
		'
		'Calib1ToolStripMenuItem
		'
		Me.Calib1ToolStripMenuItem.Name = "Calib1ToolStripMenuItem"
		Me.Calib1ToolStripMenuItem.Size = New System.Drawing.Size(152, 22)
		Me.Calib1ToolStripMenuItem.Text = "Calib1"
		'
		'Calib2ToolStripMenuItem
		'
		Me.Calib2ToolStripMenuItem.Name = "Calib2ToolStripMenuItem"
		Me.Calib2ToolStripMenuItem.Size = New System.Drawing.Size(152, 22)
		Me.Calib2ToolStripMenuItem.Text = "Calib2"
		'
		'Form1
		'
		Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
		Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
		Me.ClientSize = New System.Drawing.Size(807, 514)
		Me.Controls.Add(Me.UnitsLabel)
		Me.Controls.Add(Me.MeasBox)
		Me.Controls.Add(Me.Graph_Canvas)
		Me.Controls.Add(Me.Stop_Button)
		Me.Controls.Add(Me.Start_Button)
		Me.Controls.Add(Me.MenuStrip1)
		Me.MainMenuStrip = Me.MenuStrip1
		Me.Name = "Form1"
		Me.Text = "NGIO Measure"
		CType(Me.Graph_Canvas, System.ComponentModel.ISupportInitialize).EndInit()
		Me.MenuStrip1.ResumeLayout(False)
		Me.MenuStrip1.PerformLayout()
		Me.ResumeLayout(False)
		Me.PerformLayout()

	End Sub
	Friend WithEvents Start_Button As System.Windows.Forms.Button
	Friend WithEvents Stop_Button As System.Windows.Forms.Button
	Friend WithEvents Graph_Canvas As System.Windows.Forms.PictureBox
	Friend WithEvents MenuStrip1 As System.Windows.Forms.MenuStrip
	Friend WithEvents DeviceToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
	Friend WithEvents NewToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
	Friend WithEvents ToolStripSeparator1 As System.Windows.Forms.ToolStripSeparator
	Friend WithEvents ExitToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
	Friend WithEvents DevicesToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
	Friend WithEvents ChannelsToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
	Friend WithEvents ActionToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
	Friend WithEvents HelpToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
	Friend WithEvents AlphaToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
	Friend WithEvents BetaToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
	Friend WithEvents GetStatusToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
	Friend WithEvents SetMeasurementPeriodToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
	Friend WithEvents SetDisplayDepthToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
	Friend WithEvents StartToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
	Friend WithEvents StopMeasurementsToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
	Friend WithEvents Timer1 As System.Windows.Forms.Timer
	Friend WithEvents Analog15vToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
	Friend WithEvents Analog110vToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
	Friend WithEvents Analog25vToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
	Friend WithEvents Analog35vToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
	Friend WithEvents Analog45vToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
	Friend WithEvents Digital1photogateToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
	Friend WithEvents Digital1MotionToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
	Friend WithEvents Digital1RotaryToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
	Friend WithEvents Digital1RotaryX4ToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
	Friend WithEvents Digital1RadiationToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
	Friend WithEvents Digital2photogateToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
	Friend WithEvents Digital2MotionToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
	Friend WithEvents Digital2RotaryToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
	Friend WithEvents Digital2RotaryX4ToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
	Friend WithEvents Digital2RadiationToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
	Friend WithEvents AudiointernalToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
	Friend WithEvents MeasBox As System.Windows.Forms.Label
	Friend WithEvents UnitsLabel As System.Windows.Forms.Label
	Friend WithEvents NGIOLibVerboseToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
	Friend WithEvents AboutNGIOMeasureToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
	Friend WithEvents CalibrationsToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
	Friend WithEvents Calib0ToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
	Friend WithEvents Calib1ToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
	Friend WithEvents Calib2ToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem

End Class
