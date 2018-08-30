'/*********************************************************************************
'
'Copyright (c) 2010, Vernier Software & Technology
'All rights reserved.
'
'Redistribution and use in source and binary forms, with or without
'modification, are permitted provided that the following conditions are met:
'    * Redistributions of source code must retain the above copyright
'      notice, this list of conditions and the following disclaimer.
'    * Redistributions in binary form must reproduce the above copyright
'      notice, this list of conditions and the following disclaimer in the
'      documentation and/or other materials provided with the distribution.
'    * Neither the name of Vernier Software & Technology nor the
'      names of its contributors may be used to endorse or promote products
'      derived from this software without specific prior written permission.
'
'THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
'ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
'WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
'DISCLAIMED. IN NO EVENT SHALL VERNIER SOFTWARE & TECHNOLOGY BE LIABLE FOR ANY
'DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
'(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
'LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
'ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
'(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
'SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
'
'**********************************************************************************/
Imports NGIOdotNET
Imports VSTCoreDefsdotNET
Imports System.Runtime.InteropServices
Imports System.Text


Public Class Form1

	Dim NGIO_lib_handle As IntPtr
	Dim NGIO_device_handle As IntPtr
	Dim NGIO_audio_device_handle As IntPtr
	Dim bIsCollectingMeasurements As Boolean
	Dim measCirBuf As MeasurementTuplesCirBuf
	Const NUM_MEASUREMENTS_IN_LOCAL_BIG_BUF As UInteger = 100000
	Dim raw_measurements(NUM_MEASUREMENTS_IN_LOCAL_BIG_BUF) As Integer
	Dim raw_timestamps(NUM_MEASUREMENTS_IN_LOCAL_BIG_BUF) As Long
	Dim graph_history_y_min As Double
	Dim graph_history_y_max As Double
	Dim activeChannel As SByte
	Dim measPeriodInSeconds As Double
	Dim openDeviceType As UInteger
	Dim tenVoltRangeFlag As Boolean
    Dim digitalSamplingMode As Byte
    Dim lib_major_version As UShort
    Dim lib_minor_version As UShort

	Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
		Dim trace_threshold As Integer = 0

		NGIO_lib_handle = NGIO.Init()
		NGIO.GetDLLVersion(NGIO_lib_handle, lib_major_version, lib_minor_version)
		NGIO_device_handle = IntPtr.Zero
		NGIO_audio_device_handle = IntPtr.Zero
		bIsCollectingMeasurements = False
		measCirBuf = New MeasurementTuplesCirBuf(501)
		activeChannel = NGIO_ParmBlk.CHANNEL_ID_ANALOG1
		measPeriodInSeconds = 0.01
		tenVoltRangeFlag = False
		digitalSamplingMode = NGIOSetSamplingModeParams.SAMPLING_MODE_APERIODIC_EDGE_DETECT

		ClearGraph()
		UpdateControls()
		UpdateChannelMenu(IntPtr.Zero, 0, False, 0)

		NGIO.Diags_SetDebugTraceThreshold(NGIO_lib_handle, NGIO.TRACE_SEVERITY_LOWEST)
		NGIOLibVerboseToolStripMenuItem.Checked = True

		Timer1.Start()
	End Sub

	Private Sub Form1_Resize(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Resize
		'Debug.WriteLine("resize message from Elliot")
		Graph_Canvas.Width = Me.Size.Width - 33
		Graph_Canvas.Height = Me.Size.Height - 102
		Graph_Canvas.Invalidate()

	End Sub

	Private Sub Graph_Canvas_Paint(ByVal sender As System.Object, ByVal e As System.Windows.Forms.PaintEventArgs) Handles Graph_Canvas.Paint
		Dim g As Graphics = e.Graphics
		Dim clientRect As New Rectangle(0, 0, Graph_Canvas.Width, Graph_Canvas.Height)
		Dim graphRect As New Rectangle(0, 0, Graph_Canvas.Width, Graph_Canvas.Height)
		Dim i As Integer
		Dim meas_x_range As Double
		Dim meas_x_min As Double
		Dim meas_x_max As Double
		Dim meas_y_range As Double
		Dim meas_y_min As Double
		Dim meas_y_max As Double
		Dim testy As Double
		Dim testx As Double
		Dim x As Integer
		Dim y As Integer
		Dim deltaX As Integer
		Dim deltaY As Integer
		Dim old_x As Integer
		Dim old_y As Integer
		Dim numMeasurements As Integer
		Dim meas_x_frac As Double
		Dim meas_y_frac As Double
		Dim device_handle As IntPtr = GetOpenDeviceHandle(False)

		If (IntPtr.Zero <> device_handle) And (measCirBuf.NumElementsAvailable() > 0) Then
			'Calculate what portion of the client area will hold the graph.
			If (graphRect.Width > 200) And (graphRect.Height > 200) Then
				graphRect = New Rectangle(graphRect.Left + 100, graphRect.Top + 25, graphRect.Width - 125, graphRect.Height - 75)
			End If

			'Calculate full graph time range.
			numMeasurements = measCirBuf.NumElementsAvailable()
			meas_x_min = 0.0
			meas_x_max = 1.0
			If numMeasurements > 0 Then
				measCirBuf.CopyElement(0, testy, meas_x_min)
				measCirBuf.CopyElement(numMeasurements - 1, testy, meas_x_max)
			End If
			meas_x_range = meas_x_max - meas_x_min
			If meas_x_range < 0.001 Then
				meas_x_range = 0.001
				meas_x_max = meas_x_min + meas_x_range
			End If

			'Calculate full graph y range.
			measCirBuf.CopyElement(0, meas_y_min, testx)
			meas_y_max = meas_y_min
			For i = 1 To (numMeasurements - 1)
				measCirBuf.CopyElement(i, testy, testx)
				If testy < meas_y_min Then
					meas_y_min = testy
				End If
				If testy > meas_y_max Then
					meas_y_max = testy
				End If
			Next i
			meas_y_range = meas_y_max - meas_y_min

			'Make sure that meas_y_range corresponds to a delta of at least 0.1
			If meas_y_range < 0.1 Then
				meas_y_range = 0.1
				meas_y_max = meas_y_min + meas_y_range
			End If

			If graph_history_y_max > graph_history_y_min Then
				If graph_history_y_max > meas_y_max Then
					meas_y_max = graph_history_y_max
				End If
				If graph_history_y_min < meas_y_min Then
					meas_y_min = graph_history_y_min
				End If
				meas_y_range = meas_y_max - meas_y_min
			End If
			graph_history_y_max = meas_y_max
			graph_history_y_min = meas_y_min

			'Do some drawing now.
			g.DrawLine(System.Drawing.Pens.Black, graphRect.Left, clientRect.Top, graphRect.Left, clientRect.Bottom)
			g.DrawLine(System.Drawing.Pens.Black, graphRect.Right, clientRect.Top, graphRect.Right, clientRect.Bottom)

			g.DrawLine(System.Drawing.Pens.Blue, clientRect.Left, graphRect.Bottom, clientRect.Right, graphRect.Bottom)
			g.DrawLine(System.Drawing.Pens.Blue, clientRect.Left, graphRect.Top, clientRect.Right, graphRect.Top)

			Dim str As String = meas_x_min.ToString("0.000") & " secs"
			Dim font As New Font("Courier New", 10)
			x = graphRect.Left + 10
			y = graphRect.Bottom + 10
			g.DrawString(str, font, System.Drawing.Brushes.Black, x, y)

			str = meas_x_max.ToString("0.000") & " secs"
			Dim textSize As SizeF = g.MeasureString(str, font)
			x = graphRect.Right - textSize.Width() - 10
			y = graphRect.Bottom + 10
			g.DrawString(str, font, System.Drawing.Brushes.Black, x, y)

			Dim units As String = GetUnits()

			Dim fonty As New Font("Times New Roman", 11)
			str = meas_y_min.ToString("G5") & " " & units
			textSize = g.MeasureString(str, fonty)
			x = clientRect.Left + 10
			y = graphRect.Bottom - textSize.Height() - 10
			g.DrawString(str, fonty, System.Drawing.Brushes.Blue, x, y)

			str = meas_y_max.ToString("G5") & " " & units
			x = clientRect.Left + 10
			y = graphRect.Top + 10
			g.DrawString(str, fonty, System.Drawing.Brushes.Blue, x, y)

			Dim redPen As New Pen(Color.Red, 3.0)

			For i = 0 To (numMeasurements - 1)
				measCirBuf.CopyElement(i, meas_y_frac, meas_x_frac)
				meas_y_frac = (meas_y_frac - meas_y_min) / meas_y_range
				deltaY = Math.Round(meas_y_frac * graphRect.Height())
				meas_x_frac = (meas_x_frac - meas_x_min) / meas_x_range
				deltaX = Math.Round(meas_x_frac * graphRect.Width())
				x = graphRect.Left + deltaX
				y = graphRect.Bottom - deltaY

				If 0 = i Then
					g.DrawLine(redPen, x, y, x, y)
				ElseIf ((x <> old_x) Or (y <> old_y)) Then
					g.DrawLine(redPen, old_x, old_y, x, y)
				End If
				old_x = x
				old_y = y
			Next i
		Else
			'g.DrawLine(System.Drawing.Pens.Black, 0, 0, Graph_Canvas.Width, Graph_Canvas.Height)
			'g.DrawLine(System.Drawing.Pens.Black, 0, Graph_Canvas.Height, Graph_Canvas.Width, 0)
		End If
	End Sub

	Private Sub Form1_FormClosed(ByVal sender As System.Object, ByVal e As System.Windows.Forms.FormClosedEventArgs) Handles MyBase.FormClosed
		Timer1.Stop()

		If IntPtr.Zero <> GetOpenDeviceHandle(False) Then
			CloseDevice()
		End If
		NGIO.Uninit(NGIO_lib_handle)
	End Sub

	Private Sub DevicesToolStripMenuItemDevices_DropDownOpening(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles DevicesToolStripMenuItem.DropDownOpening
		Dim signature As UInteger
		Dim status_mask As UInteger
		Dim hList As IntPtr
		Dim numDevices As UInteger
		Dim numDevicesAdded As UInteger
		Dim deviceName As StringBuilder = New StringBuilder(CInt(NGIO.MAX_SIZE_DEVICE_NAME))
		Dim openDeviceName As StringBuilder = New StringBuilder(CInt(NGIO.MAX_SIZE_DEVICE_NAME))
		Dim deviceName2 As String
		Dim openDeviceName2 As String = ""
		Dim status As Integer
		Dim mnu As ToolStripMenuItem
		Dim N As UInteger = 0

		If IntPtr.Zero <> NGIO_device_handle Then
			NGIO.Device_GetOpenDeviceName(NGIO_device_handle, openDeviceName, NGIO.MAX_SIZE_DEVICE_NAME)
			openDeviceName2 = openDeviceName.ToString()
		End If

		Me.DevicesToolStripMenuItem.DropDownItems.Clear()
		numDevicesAdded = 0

		NGIO.SearchForDevices(NGIO_lib_handle, NGIO.DEVTYPE_LABQUEST, NGIO.COMM_TRANSPORT_USB, IntPtr.Zero, signature)

		hList = NGIO.OpenDeviceListSnapshot(NGIO_lib_handle, NGIO.DEVTYPE_LABQUEST, numDevices, signature)
		If (hList <> IntPtr.Zero) Then
			N = 0
			Do While N < numDevices
				status = NGIO.DeviceListSnapshot_GetNthEntry(hList, N, deviceName, NGIO.MAX_SIZE_DEVICE_NAME, status_mask)
				If 0 = status Then
					deviceName2 = deviceName.ToString()
					mnu = New ToolStripMenuItem()
					mnu.Text = deviceName2
					If 0 = String.Compare(openDeviceName2, deviceName2) Then
						mnu.Checked() = True
					End If
					AddDeviceNHandler(mnu, numDevicesAdded)
					numDevicesAdded = numDevicesAdded + 1
					Me.DevicesToolStripMenuItem.DropDownItems.Add(mnu)
				Else
					Exit Do
				End If
				N = N + 1
			Loop

			NGIO.CloseDeviceListSnapshot(hList)
		End If

        NGIO.SearchForDevices(NGIO_lib_handle, NGIO.DEVTYPE_LABQUEST2, NGIO.COMM_TRANSPORT_USB, IntPtr.Zero, signature)

        hList = NGIO.OpenDeviceListSnapshot(NGIO_lib_handle, NGIO.DEVTYPE_LABQUEST2, numDevices, signature)
        If (hList <> IntPtr.Zero) Then
            N = 0
            Do While N < numDevices
                status = NGIO.DeviceListSnapshot_GetNthEntry(hList, N, deviceName, NGIO.MAX_SIZE_DEVICE_NAME, status_mask)
                If 0 = status Then
                    deviceName2 = deviceName.ToString()
                    mnu = New ToolStripMenuItem()
                    mnu.Text = deviceName2
                    If 0 = String.Compare(openDeviceName2, deviceName2) Then
                        mnu.Checked() = True
                    End If
                    AddDeviceNHandler(mnu, numDevicesAdded)
                    numDevicesAdded = numDevicesAdded + 1
                    Me.DevicesToolStripMenuItem.DropDownItems.Add(mnu)
                Else
                    Exit Do
                End If
                N = N + 1
            Loop

            NGIO.CloseDeviceListSnapshot(hList)
        End If

        NGIO.SearchForDevices(NGIO_lib_handle, NGIO.DEVTYPE_LABQUEST_MINI, NGIO.COMM_TRANSPORT_USB, IntPtr.Zero, signature)

		hList = NGIO.OpenDeviceListSnapshot(NGIO_lib_handle, NGIO.DEVTYPE_LABQUEST_MINI, numDevices, signature)
		If (hList <> IntPtr.Zero) Then
			N = 0
			Do While N < numDevices
				status = NGIO.DeviceListSnapshot_GetNthEntry(hList, N, deviceName, NGIO.MAX_SIZE_DEVICE_NAME, status_mask)
				If 0 = status Then
					deviceName2 = deviceName.ToString()
					mnu = New ToolStripMenuItem()
					mnu.Text = deviceName2
					If 0 = String.Compare(openDeviceName2, deviceName2) Then
						mnu.Checked() = True
					End If
					AddDeviceNHandler(mnu, numDevicesAdded)
					numDevicesAdded = numDevicesAdded + 1
					Me.DevicesToolStripMenuItem.DropDownItems.Add(mnu)
				Else
					Exit Do
				End If
				N = N + 1
			Loop

			NGIO.CloseDeviceListSnapshot(hList)
		End If

	End Sub

	Private Sub Device0MenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs)
		OnDeviceNMenuItem_Click(0)
	End Sub

	Private Sub Device1MenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs)
		OnDeviceNMenuItem_Click(1)
	End Sub

	Private Sub Device2MenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs)
		OnDeviceNMenuItem_Click(2)
	End Sub

	Private Sub Device3MenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs)
		OnDeviceNMenuItem_Click(3)
	End Sub

	Private Sub Device4MenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs)
		OnDeviceNMenuItem_Click(4)
	End Sub

	Private Sub AddDeviceNHandler(ByVal deviceMenuItem As ToolStripMenuItem, ByVal N As UInteger)
		Select Case N
			Case 0
				AddHandler deviceMenuItem.Click, AddressOf Device0MenuItem_Click
			Case 1
				AddHandler deviceMenuItem.Click, AddressOf Device1MenuItem_Click
			Case 2
				AddHandler deviceMenuItem.Click, AddressOf Device2MenuItem_Click
			Case 3
				AddHandler deviceMenuItem.Click, AddressOf Device3MenuItem_Click
			Case 4
				AddHandler deviceMenuItem.Click, AddressOf Device4MenuItem_Click
			Case Else
				Debug.WriteLine("too many devices!")
		End Select
	End Sub

	Private Sub OnDeviceNMenuItem_Click(ByVal N As Integer)
		Dim mnu As ToolStripMenuItem

		mnu = Me.DevicesToolStripMenuItem.DropDownItems.Item(N)
		If CheckState.Checked = mnu.CheckState Then
			CloseDevice()
			UpdateTitle()
			ClearGraph()
			UpdateUnits()
			UpdateChannelMenu(IntPtr.Zero, 0, False, 0)
			MeasBox.Text = ""
		Else
			Refresh()
			OpenDevice(mnu.Text)
			OnAnalogChannel(NGIO_ParmBlk.CHANNEL_ID_ANALOG1)
		End If
		UpdateControls()
	End Sub

	Private Sub OpenDevice(ByVal deviceName As String)
        If IntPtr.Zero <> NGIO_device_handle Then
            CloseDevice()
        End If
		NGIO_device_handle = NGIO.Device_Open(NGIO_lib_handle, deviceName, 0)
		If IntPtr.Zero <> NGIO_device_handle Then
			NGIO.GetDeviceTypeFromDeviceName(deviceName, openDeviceType)
            If (NGIO.DEVTYPE_LABQUEST = openDeviceType) Or (NGIO.DEVTYPE_LABQUEST2 = openDeviceType) Then
                Dim oldCursor As Cursor = Cursor
                Cursor = Cursors.WaitCursor
                Dim status As Integer = NGIO.Device_AcquireExclusiveOwnership(NGIO_device_handle, NGIO.GRAB_DAQ_TIMEOUT_MS)
                If status <> 0 Then
                    CloseDevice()
                End If
                Cursor = oldCursor
            ElseIf NGIO.DEVTYPE_LABQUEST_MINI = openDeviceType Then
                Dim ledParams As NGIOSetLedStateParams = New NGIOSetLedStateParams
                ledParams.color = NGIOSetLedStateParams.LED_COLOR_GREEN
                ledParams.brightness = NGIOSetLedStateParams.LED_BRIGHTNESS_MAX
                NGIO.Device_SendCmdAndGetResponse2(NGIO_device_handle, NGIO_ParmBlk.CMD_ID_SET_LED_STATE, ledParams, NGIO.TIMEOUT_MS_DEFAULT)
            End If
		End If
		If IntPtr.Zero <> NGIO_device_handle Then
			'Set desired sampling period.
			NGIO.Device_SetMeasurementPeriod(NGIO_device_handle, -1, measPeriodInSeconds, NGIO.TIMEOUT_MS_DEFAULT)
			'Find out what actual sampling period is.
			NGIO.Device_GetMeasurementPeriod(NGIO_device_handle, -1, measPeriodInSeconds, NGIO.TIMEOUT_MS_DEFAULT)
		End If

	End Sub

	Private Sub OpenAudioDevice()
		If IntPtr.Zero <> NGIO_device_handle Then
			If IntPtr.Zero = NGIO_audio_device_handle Then
				Dim signature As UInteger
				Dim status_mask As UInteger
				Dim hList As IntPtr
				Dim numDevices As UInteger
				Dim deviceName As StringBuilder = New StringBuilder(CInt(NGIO.MAX_SIZE_DEVICE_NAME))
				Dim deviceName2 As String
				Dim status As Integer

				NGIO.SearchForDevices(NGIO_lib_handle, NGIO.DEVTYPE_LABQUEST_AUDIO, NGIO.COMM_TRANSPORT_USB, IntPtr.Zero, signature)

				hList = NGIO.OpenDeviceListSnapshot(NGIO_lib_handle, NGIO.DEVTYPE_LABQUEST_AUDIO, numDevices, signature)
				If (hList <> IntPtr.Zero) Then
					status = NGIO.DeviceListSnapshot_GetNthEntry(hList, 0, deviceName, NGIO.MAX_SIZE_DEVICE_NAME, status_mask)
					If 0 = status Then
						deviceName2 = deviceName.ToString()
						NGIO_audio_device_handle = NGIO.Device_Open(NGIO_lib_handle, deviceName2, 1)
						If IntPtr.Zero <> NGIO_audio_device_handle Then
							'Set desired sampling period.
							measPeriodInSeconds = 0.000125	'Default to about 8,000 hz.
							NGIO.Device_SetMeasurementPeriod(NGIO_audio_device_handle, -1, measPeriodInSeconds, NGIO.TIMEOUT_MS_DEFAULT)
							'Find out what actual sampling period is.
							NGIO.Device_GetMeasurementPeriod(NGIO_audio_device_handle, -1, measPeriodInSeconds, NGIO.TIMEOUT_MS_DEFAULT)
						End If
					End If
					NGIO.CloseDeviceListSnapshot(hList)
				End If
			End If
		End If
	End Sub

	Private Sub CloseDevice()
		If IntPtr.Zero <> NGIO_audio_device_handle Then
			NGIO.Device_Close(NGIO_audio_device_handle)
		End If
		NGIO_audio_device_handle = IntPtr.Zero
		If IntPtr.Zero <> NGIO_device_handle Then
			NGIO.Device_Close(NGIO_device_handle)
		End If
		NGIO_device_handle = IntPtr.Zero
		bIsCollectingMeasurements = False
	End Sub

	Private Sub ActionToolStripMenuItem_DropDownOpening(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ActionToolStripMenuItem.DropDownOpening
		Dim device_handle As IntPtr = GetOpenDeviceHandle(False)
		ActionToolStripMenuItem.DropDownItems.Item(0).Enabled = (IntPtr.Zero <> device_handle)	'Get Status
		ActionToolStripMenuItem.DropDownItems.Item(1).Enabled = (IntPtr.Zero <> device_handle) And (Not bIsCollectingMeasurements)	'Set measurement period
		ActionToolStripMenuItem.DropDownItems.Item(2).Enabled = True	'Set display depth
		ActionToolStripMenuItem.DropDownItems.Item(3).Enabled = (IntPtr.Zero <> device_handle) And (Not bIsCollectingMeasurements)	'Start measurements
		ActionToolStripMenuItem.DropDownItems.Item(4).Enabled = bIsCollectingMeasurements	'Stop measurements
	End Sub

	Private Sub GetStatusToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles GetStatusToolStripMenuItem.Click
		Dim response As NGIOGetStatusCmdResponsePayload = New NGIOGetStatusCmdResponsePayload
		Dim response2 As NGIO_NVMEM_CHANNEL_ID1_rec = New NGIO_NVMEM_CHANNEL_ID1_rec
		Dim nRespBytes As UInteger
		Dim status As Integer

		Dim device_handle As IntPtr = GetOpenDeviceHandle(False)
		If (IntPtr.Zero <> device_handle) Then
			status = NGIO.Device_SendCmdAndGetResponse3(device_handle, NGIO_ParmBlk.CMD_ID_GET_STATUS, response, nRespBytes, NGIO.TIMEOUT_MS_DEFAULT)
			If (0 = status) Then
				If (device_handle <> NGIO_audio_device_handle) Then
					Dim params2 As NGIOReadNVMemParams = New NGIOReadNVMemParams
					params2.channel = NGIOReadNVMemParams.NGIO_NVMEM_CHANNEL_ID1
					params2.lsbyteLswordAddr = 0
					params2.msbyteLswordAddr = 0
					params2.lsbyteMswordAddr = 0
					params2.msbyteMswordAddr = 0
					params2.msByteCount = 0
					params2.lsByteCount = Marshal.SizeOf(response2)
					status = NGIO.Device_SendCmdAndGetResponse1(device_handle, NGIO_ParmBlk.CMD_ID_READ_NV_MEM, params2, response2, nRespBytes, NGIO.TIMEOUT_MS_DEFAULT)
				End If
			End If

			If (0 = status) Then
				Dim digit0 As Integer = response.majorVersionMasterCPU
				Dim digit1 As Integer = response.minorVersionMasterCPU >> 4
				Dim digit2 As Integer = response.minorVersionMasterCPU And 15
				Dim digit3 As Integer = response.majorVersionSlaveCPU >> 4
				Dim digit4 As Integer = response.majorVersionSlaveCPU And 15
				Dim digit5 As Integer = response.minorVersionSlaveCPU >> 4
				Dim digit6 As Integer = response.minorVersionSlaveCPU And 15
				Dim rVersion As Double = digit0 + digit1 * 0.1 + digit2 * 0.01 + digit3 * 0.001 + digit4 * 0.0001 _
				 + digit5 * 0.00001 + digit6 * 0.000001
				Dim sVersion As String = rVersion.ToString("F6")
				Dim deviceStatus As UInteger = response.status

				Dim msg As String = "Device status = " & deviceStatus.ToString("X") & " ; version = " & sVersion
				If (device_handle <> NGIO_audio_device_handle) Then
					Dim serialNum As UInteger = response2.serialNumber.msbyteMswordSerialCounter
					serialNum = (serialNum << 8) + response2.serialNumber.lsbyteMswordSerialCounter
					serialNum = (serialNum << 8) + response2.serialNumber.msbyteLswordSerialCounter
					serialNum = (serialNum << 8) + response2.serialNumber.lsbyteLswordSerialCounter
					msg = msg & " ; serial number(yy ww nnnnnnnn) is " & response2.serialNumber.yy.ToString("X2")
					msg = msg & " " & response2.serialNumber.ww.ToString("X2") & " " & serialNum.ToString("D8")
				End If

				MessageBox.Show(msg, "Get Device Status")
			End If
		End If
	End Sub

	Private Sub StartMeasurements()
		Dim device_handle As IntPtr = GetOpenDeviceHandle(False)
		If (IntPtr.Zero <> device_handle) And (Not bIsCollectingMeasurements) Then
			NGIO.Device_SendCmdAndGetResponse4(device_handle, NGIO_ParmBlk.CMD_ID_START_MEASUREMENTS, NGIO.TIMEOUT_MS_DEFAULT)
			bIsCollectingMeasurements = True
		End If
	End Sub

	Private Sub StopMeasurements()
		Dim device_handle As IntPtr = GetOpenDeviceHandle(False)
		If (IntPtr.Zero <> device_handle) And bIsCollectingMeasurements Then
			NGIO.Device_SendCmdAndGetResponse4(device_handle, NGIO_ParmBlk.CMD_ID_STOP_MEASUREMENTS, NGIO.TIMEOUT_MS_DEFAULT)
			'NGIO.Device_SendCmdAndGetResponse(device_handle, NGIO_ParmBlk.CMD_ID_STOP_MEASUREMENTS, IntPtr.Zero, 0, IntPtr.Zero, IntPtr.Zero, NGIO.TIMEOUT_MS_DEFAULT)
			bIsCollectingMeasurements = False
		End If
	End Sub

	Private Sub UpdateControls()
		If IntPtr.Zero = GetOpenDeviceHandle(False) Then
			Start_Button.Enabled = False
			Stop_Button.Enabled = False
		ElseIf bIsCollectingMeasurements Then
			Start_Button.Enabled = False
			Stop_Button.Enabled = True
		Else
			Start_Button.Enabled = True
			Stop_Button.Enabled = False
		End If
		Me.Refresh()
	End Sub

	Private Sub Start_Button_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Start_Button.Click
		ClearGraph()
		StartMeasurements()
		UpdateControls()
	End Sub

	Private Sub StartToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles StartToolStripMenuItem.Click
		ClearGraph()
		StartMeasurements()
		UpdateControls()
	End Sub

	Private Sub Stop_Button_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Stop_Button.Click
		StopMeasurements()
		UpdateControls()
	End Sub

	Private Sub StopMeasurementsToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles StopMeasurementsToolStripMenuItem.Click
		StopMeasurements()
		UpdateControls()
	End Sub

	Private Sub ClearGraph()
		If IntPtr.Zero <> GetOpenDeviceHandle(False) Then
			NGIO.Device_ClearIO(GetOpenDeviceHandle(False), -1)
		End If
		measCirBuf.Clear()
		graph_history_y_min = 1.0	'This clears the history.
		graph_history_y_max = -1.0
		Graph_Canvas.Invalidate()
	End Sub

	Private Sub Timer1_Tick(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Timer1.Tick
		Dim device_handle As IntPtr = GetOpenDeviceHandle(False)
		If IntPtr.Zero <> device_handle Then
			Dim numMeasurementsAvailable As Integer
            Dim volts As Single
			Dim rMeasurement As Double
			Dim rTime As Double
            Dim probeType As Integer = NGIO.Device_GetProbeType(device_handle, activeChannel)

			numMeasurementsAvailable = NGIO.Device_ReadRawMeasurements(device_handle, activeChannel, _
			 raw_measurements, raw_timestamps, NUM_MEASUREMENTS_IN_LOCAL_BIG_BUF)
			If numMeasurementsAvailable > 0 Then
                For i As Integer = 0 To (numMeasurementsAvailable - 1)
                    volts = NGIO.Device_ConvertToVoltage(device_handle, activeChannel, raw_measurements(i), probeType)
                    rMeasurement = NGIO.Device_CalibrateData(device_handle, activeChannel, volts)
                    If device_handle <> NGIO_audio_device_handle Then
                        rTime = raw_timestamps(i) * 0.000001
                    Else
                        rTime = raw_timestamps(i) * measPeriodInSeconds
                    End If
                    measCirBuf.AddElement(rMeasurement, rTime)
                Next i
				Graph_Canvas.Invalidate()

				MeasBox.Text = rMeasurement.ToString("G5")
			Else
				If Not bIsCollectingMeasurements Then
					MeasBox.Text = ""
				End If
			End If
		End If
	End Sub

	Private Sub OnAnalogChannel(ByVal channel As SByte)
		Dim device_handle As IntPtr = GetOpenDeviceHandle(True)
		If (IntPtr.Zero <> device_handle) Then
			Dim maskParams As NGIOSetSensorChannelEnableMaskParams = New NGIOSetSensorChannelEnableMaskParams
			Dim inputParams As NGIOSetAnalogInputParams = New NGIOSetAnalogInputParams
			Dim sensorId As Byte = 0

			activeChannel = channel
			tenVoltRangeFlag = False

			Select Case activeChannel
				Case NGIO_ParmBlk.CHANNEL_ID_ANALOG1
					maskParams.lsbyteLsword_EnableSensorChannels = NGIO_ParmBlk.CHANNEL_MASK_ANALOG1
				Case NGIO_ParmBlk.CHANNEL_ID_ANALOG2
					maskParams.lsbyteLsword_EnableSensorChannels = NGIO_ParmBlk.CHANNEL_MASK_ANALOG2
				Case NGIO_ParmBlk.CHANNEL_ID_ANALOG3
					maskParams.lsbyteLsword_EnableSensorChannels = NGIO_ParmBlk.CHANNEL_MASK_ANALOG3
				Case NGIO_ParmBlk.CHANNEL_ID_ANALOG4
					maskParams.lsbyteLsword_EnableSensorChannels = NGIO_ParmBlk.CHANNEL_MASK_ANALOG4
				Case Else
					maskParams.lsbyteLsword_EnableSensorChannels = 0
			End Select
			maskParams.msbyteLsword_EnableSensorChannels = 0
			maskParams.lsbyteMsword_EnableSensorChannels = 0
			maskParams.msbyteMsword_EnableSensorChannels = 0
			NGIO.Device_SendCmdAndGetResponse2(device_handle, NGIO_ParmBlk.CMD_ID_SET_SENSOR_CHANNEL_ENABLE_MASK, _
			 maskParams, NGIO.TIMEOUT_MS_DEFAULT)

			inputParams.channel = activeChannel
			inputParams.analogInput = NGIOSetAnalogInputParams.ANALOG_INPUT_5V_BUILTIN_12BIT_ADC
			NGIO.Device_SendCmdAndGetResponse2(device_handle, NGIO_ParmBlk.CMD_ID_SET_ANALOG_INPUT, _
			 inputParams, NGIO.TIMEOUT_MS_DEFAULT)

			sensorId = UpdateSensorIdAndDDSRec(activeChannel)

			If (0 = sensorId) Then
				'Force a default sensor.
				sensorId = 14	'5v voltage probe.
				NGIO.Device_DDSMem_SetSensorNumber(device_handle, activeChannel, sensorId)
				NGIO.Device_DDSMem_SetLongName(device_handle, activeChannel, "assumed")
				NGIO.Device_DDSMem_SetCalPage(device_handle, activeChannel, 0, 0.0, 1.0, 0.0, "volts")
			End If

			UpdateTitle()
			ClearGraph()
			UpdateUnits()
			UpdateChannelMenu(device_handle, activeChannel, tenVoltRangeFlag, 0)

			If NGIO.Device_GetProbeType(device_handle, activeChannel) <> VSTSensorDDSMemRec.kProbeTypeAnalog5V Then
				MessageBox.Show("Sensor plugged in is not compatible with selected mode.")
			End If

		End If
	End Sub

	Private Sub OnDigitalChannel(ByVal channel As SByte, ByVal sampling_mode As Byte)
		Dim device_handle As IntPtr = GetOpenDeviceHandle(True)
		If (IntPtr.Zero <> device_handle) Then
			Dim maskParams As NGIOSetSensorChannelEnableMaskParams = New NGIOSetSensorChannelEnableMaskParams
			Dim samplingModeParams As NGIOSetSamplingModeParams = New NGIOSetSamplingModeParams
			Dim sensorId As Byte = 0
			Dim desiredSensorId As Byte = 0
			Dim desiredProbeType As Integer = 0

			activeChannel = channel
			digitalSamplingMode = sampling_mode

			Select Case activeChannel
				Case NGIO_ParmBlk.CHANNEL_ID_DIGITAL1
					maskParams.lsbyteLsword_EnableSensorChannels = NGIO_ParmBlk.CHANNEL_MASK_DIGITAL1
				Case NGIO_ParmBlk.CHANNEL_ID_DIGITAL2
					maskParams.lsbyteLsword_EnableSensorChannels = NGIO_ParmBlk.CHANNEL_MASK_DIGITAL2
				Case Else
					maskParams.lsbyteLsword_EnableSensorChannels = 0
			End Select
			maskParams.msbyteLsword_EnableSensorChannels = 0
			maskParams.lsbyteMsword_EnableSensorChannels = 0
			maskParams.msbyteMsword_EnableSensorChannels = 0
			NGIO.Device_SendCmdAndGetResponse2(device_handle, NGIO_ParmBlk.CMD_ID_SET_SENSOR_CHANNEL_ENABLE_MASK, _
			 maskParams, NGIO.TIMEOUT_MS_DEFAULT)

			samplingModeParams.channel = activeChannel
			samplingModeParams.samplingMode = digitalSamplingMode
			NGIO.Device_SendCmdAndGetResponse2(device_handle, NGIO_ParmBlk.CMD_ID_SET_SAMPLING_MODE, _
			 samplingModeParams, NGIO.TIMEOUT_MS_DEFAULT)

			sensorId = UpdateSensorIdAndDDSRec(activeChannel)

			Select Case digitalSamplingMode
				Case NGIOSetSamplingModeParams.SAMPLING_MODE_APERIODIC_EDGE_DETECT
					desiredSensorId = 4	'photogate
					desiredProbeType = VSTSensorDDSMemRec.kProbeTypePhotoGate
				Case NGIOSetSamplingModeParams.SAMPLING_MODE_PERIODIC_MOTION_DETECT
					desiredSensorId = 2	'motion detector
					desiredProbeType = VSTSensorDDSMemRec.kProbeTypeMD
				Case NGIOSetSamplingModeParams.SAMPLING_MODE_PERIODIC_ROTATION_COUNTER, _
				  NGIOSetSamplingModeParams.SAMPLING_MODE_PERIODIC_ROTATION_COUNTER_X4
					desiredSensorId = 6	'rotary motion
					desiredProbeType = VSTSensorDDSMemRec.kProbeTypeRotary
				Case NGIOSetSamplingModeParams.SAMPLING_MODE_PERIODIC_PULSE_COUNT
					desiredSensorId = 7	'radiation monitor
					desiredProbeType = VSTSensorDDSMemRec.kProbeTypeDigitalCount
			End Select

			If (0 = sensorId) Then
				'Force a default sensor.
				NGIO.Device_DDSMem_SetSensorNumber(device_handle, activeChannel, desiredSensorId)
				NGIO.Device_DDSMem_SetLongName(device_handle, activeChannel, "assumed")
				NGIO.Device_DDSMem_SetCalPage(device_handle, activeChannel, 0, 0.0, 1.0, 0.0, "units")
			End If

			UpdateTitle()
			ClearGraph()
			UpdateUnits()
			UpdateChannelMenu(device_handle, activeChannel, False, digitalSamplingMode)

			If (desiredProbeType <> VSTSensorDDSMemRec.kProbeTypeDigitalCount) Then	'Do not complain when just counting pulses.
				If NGIO.Device_GetProbeType(device_handle, activeChannel) <> desiredProbeType Then
                    MessageBox.Show("Sensor plugged in is not compatible with selected mode.")
                ElseIf (measPeriodInSeconds < 0.02) Then    'Make sure measurement period is ok for motion detectors.
                    measPeriodInSeconds = 0.05
                    NGIO.Device_SetMeasurementPeriod(device_handle, -1, measPeriodInSeconds, NGIO.TIMEOUT_MS_DEFAULT)
                End If
			End If

		End If
	End Sub

	Private Sub Analog15vToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Analog15vToolStripMenuItem.Click
		OnAnalogChannel(NGIO_ParmBlk.CHANNEL_ID_ANALOG1)
	End Sub

	Private Sub Analog110vToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Analog110vToolStripMenuItem.Click
		Dim device_handle As IntPtr = GetOpenDeviceHandle(True)
		If (IntPtr.Zero <> device_handle) Then
			Dim maskParams As NGIOSetSensorChannelEnableMaskParams = New NGIOSetSensorChannelEnableMaskParams
			Dim inputParams As NGIOSetAnalogInputParams = New NGIOSetAnalogInputParams
			Dim sensorId As Byte = 0

			activeChannel = NGIO_ParmBlk.CHANNEL_ID_ANALOG1
			tenVoltRangeFlag = True

			maskParams.lsbyteLsword_EnableSensorChannels = NGIO_ParmBlk.CHANNEL_MASK_ANALOG1
			maskParams.msbyteLsword_EnableSensorChannels = 0
			maskParams.lsbyteMsword_EnableSensorChannels = 0
			maskParams.msbyteMsword_EnableSensorChannels = 0
			NGIO.Device_SendCmdAndGetResponse2(device_handle, NGIO_ParmBlk.CMD_ID_SET_SENSOR_CHANNEL_ENABLE_MASK, _
			 maskParams, NGIO.TIMEOUT_MS_DEFAULT)

			inputParams.channel = activeChannel
			inputParams.analogInput = NGIOSetAnalogInputParams.ANALOG_INPUT_PM10V_BUILTIN_12BIT_ADC
			NGIO.Device_SendCmdAndGetResponse2(device_handle, NGIO_ParmBlk.CMD_ID_SET_ANALOG_INPUT, _
			 inputParams, NGIO.TIMEOUT_MS_DEFAULT)

			sensorId = UpdateSensorIdAndDDSRec(activeChannel)

			If (0 = sensorId) Then
				'Force a default sensor.
				sensorId = 2	'+/- 10 volt probe.
				NGIO.Device_DDSMem_SetSensorNumber(device_handle, activeChannel, sensorId)
				NGIO.Device_DDSMem_SetLongName(device_handle, activeChannel, "assumed")
				NGIO.Device_DDSMem_SetCalPage(device_handle, activeChannel, 0, 0.0, 1.0, 0.0, "volts")
			End If

			UpdateTitle()
			ClearGraph()
			UpdateUnits()
			UpdateChannelMenu(device_handle, activeChannel, tenVoltRangeFlag, 0)

			If NGIO.Device_GetProbeType(device_handle, activeChannel) <> VSTSensorDDSMemRec.kProbeTypeAnalog10V Then
				MessageBox.Show("Sensor plugged in is not compatible with selected mode.")
			End If

		End If
	End Sub

	Private Sub Analog25vToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Analog25vToolStripMenuItem.Click
		OnAnalogChannel(NGIO_ParmBlk.CHANNEL_ID_ANALOG2)
	End Sub

	Private Sub Analog35vToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Analog35vToolStripMenuItem.Click
		OnAnalogChannel(NGIO_ParmBlk.CHANNEL_ID_ANALOG3)
	End Sub

	Private Sub Analog45vToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Analog45vToolStripMenuItem.Click
		OnAnalogChannel(NGIO_ParmBlk.CHANNEL_ID_ANALOG4)
	End Sub

	Private Sub Digital1photogateToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Digital1photogateToolStripMenuItem.Click
		OnDigitalChannel(NGIO_ParmBlk.CHANNEL_ID_DIGITAL1, NGIOSetSamplingModeParams.SAMPLING_MODE_APERIODIC_EDGE_DETECT)
	End Sub

	Private Sub Digital1MotionToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Digital1MotionToolStripMenuItem.Click
		OnDigitalChannel(NGIO_ParmBlk.CHANNEL_ID_DIGITAL1, NGIOSetSamplingModeParams.SAMPLING_MODE_PERIODIC_MOTION_DETECT)
	End Sub

	Private Sub Digital1RotaryToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Digital1RotaryToolStripMenuItem.Click
		OnDigitalChannel(NGIO_ParmBlk.CHANNEL_ID_DIGITAL1, NGIOSetSamplingModeParams.SAMPLING_MODE_PERIODIC_ROTATION_COUNTER)
	End Sub

	Private Sub Digital1RotaryX4ToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Digital1RotaryX4ToolStripMenuItem.Click
		OnDigitalChannel(NGIO_ParmBlk.CHANNEL_ID_DIGITAL1, NGIOSetSamplingModeParams.SAMPLING_MODE_PERIODIC_ROTATION_COUNTER_X4)
	End Sub

	Private Sub Digital1RadiationToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Digital1RadiationToolStripMenuItem.Click
		OnDigitalChannel(NGIO_ParmBlk.CHANNEL_ID_DIGITAL1, NGIOSetSamplingModeParams.SAMPLING_MODE_PERIODIC_PULSE_COUNT)
	End Sub

	Private Sub Digital2photogateToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Digital2photogateToolStripMenuItem.Click
		OnDigitalChannel(NGIO_ParmBlk.CHANNEL_ID_DIGITAL2, NGIOSetSamplingModeParams.SAMPLING_MODE_APERIODIC_EDGE_DETECT)
	End Sub

	Private Sub Digital2MotionToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Digital2MotionToolStripMenuItem.Click
		OnDigitalChannel(NGIO_ParmBlk.CHANNEL_ID_DIGITAL2, NGIOSetSamplingModeParams.SAMPLING_MODE_PERIODIC_MOTION_DETECT)
	End Sub

	Private Sub Digital2RotaryToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Digital2RotaryToolStripMenuItem.Click
		OnDigitalChannel(NGIO_ParmBlk.CHANNEL_ID_DIGITAL2, NGIOSetSamplingModeParams.SAMPLING_MODE_PERIODIC_ROTATION_COUNTER)
	End Sub

	Private Sub Digital2RotaryX4ToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Digital2RotaryX4ToolStripMenuItem.Click
		OnDigitalChannel(NGIO_ParmBlk.CHANNEL_ID_DIGITAL2, NGIOSetSamplingModeParams.SAMPLING_MODE_PERIODIC_ROTATION_COUNTER_X4)
	End Sub

	Private Sub Digital2RadiationToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Digital2RadiationToolStripMenuItem.Click
		OnDigitalChannel(NGIO_ParmBlk.CHANNEL_ID_DIGITAL2, NGIOSetSamplingModeParams.SAMPLING_MODE_PERIODIC_PULSE_COUNT)
	End Sub

	Private Sub AudiointernalToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles AudiointernalToolStripMenuItem.Click
		Dim maskParams As NGIOSetSensorChannelEnableMaskParams = New NGIOSetSensorChannelEnableMaskParams
		If (IntPtr.Zero = NGIO_audio_device_handle) Then
			OpenAudioDevice()
		End If
		If (IntPtr.Zero <> NGIO_audio_device_handle) Then
			activeChannel = NGIO_ParmBlk.CHANNEL_ID_AUDIO_INTERNAL

			maskParams.lsbyteLsword_EnableSensorChannels = NGIO_ParmBlk.CHANNEL_ID_MASK_AUDIO_INTERNAL
			maskParams.msbyteLsword_EnableSensorChannels = 0
			maskParams.lsbyteMsword_EnableSensorChannels = 0
			maskParams.msbyteMsword_EnableSensorChannels = 0
			NGIO.Device_SendCmdAndGetResponse2(NGIO_audio_device_handle, NGIO_ParmBlk.CMD_ID_SET_SENSOR_CHANNEL_ENABLE_MASK, _
			 maskParams, NGIO.TIMEOUT_MS_DEFAULT)

			UpdateSensorIdAndDDSRec(activeChannel)
			UpdateTitle()
			ClearGraph()
			UpdateUnits()
			UpdateChannelMenu(NGIO_audio_device_handle, activeChannel, False, 0)
		End If
	End Sub

	Private Function UpdateSensorIdAndDDSRec(ByVal channel As SByte) As Byte
		Dim id As Byte = 0

		Dim device_handle As IntPtr = GetOpenDeviceHandle(False)
		If (IntPtr.Zero <> device_handle) Then
			NGIO.Device_DDSMem_ClearRecord(device_handle, channel)
            NGIO.Device_DDSMem_SetCalibrationEquation(device_handle, channel, VSTSensorDDSMemRec.kEquationType_Linear)
			NGIO.Device_DDSMem_SetCalPage(device_handle, channel, 0, 0.0, 1.0, 0.0, "units")
			NGIO.Device_DDSMem_SetHighestValidCalPageIndex(device_handle, channel, 0)
			NGIO.Device_DDSMem_SetActiveCalPage(device_handle, channel, 0)

			If device_handle <> NGIO_audio_device_handle Then
				Dim status As Int32
				Dim sig As UInt32

				status = NGIO.Device_DDSMem_GetSensorNumber(device_handle, channel, id, 1, sig, NGIO.TIMEOUT_MS_DEFAULT)
				If 0 <> status Then
					id = 0
				Else
					If id >= VSTSensorDDSMemRec.kSensorIdNumber_FirstSmartSensor Then
						status = NGIO.Device_DDSMem_ReadRecord(device_handle, channel, 0, NGIO.READ_DDSMEMBLOCK_TIMEOUT_MS)
						If 0 <> status Then
							id = 0
							NGIO.Device_DDSMem_SetSensorNumber(device_handle, channel, id)
						End If
					End If
				End If
			Else
				NGIO.Device_DDSMem_SetCalPage(device_handle, channel, 0, 0.0, 1.0, 0.0, "volts")
			End If
		End If

		Return id
	End Function

	Private Sub UpdateTitle()
		Dim title As String = "NGIO_Measure"
		Dim device_handle As IntPtr = GetOpenDeviceHandle(False)
		If (IntPtr.Zero <> device_handle) Then
			Dim sensorId As Byte = 0
			Dim sig As UInt32
			Dim longName As StringBuilder = New StringBuilder(30)
			Dim longName2 As String
			Dim probeType As Int32

			title = title & " - Sensor in chan "
			If device_handle <> NGIO_audio_device_handle Then
				Select Case activeChannel
					Case NGIO_ParmBlk.CHANNEL_ID_ANALOG1
						title = title & "ANALOG1"
					Case NGIO_ParmBlk.CHANNEL_ID_ANALOG2
						title = title & "ANALOG2"
					Case NGIO_ParmBlk.CHANNEL_ID_ANALOG3
						title = title & "ANALOG3"
					Case NGIO_ParmBlk.CHANNEL_ID_ANALOG4
						title = title & "ANALOG4"
					Case NGIO_ParmBlk.CHANNEL_ID_DIGITAL1
						title = title & "DIGITAL1"
					Case NGIO_ParmBlk.CHANNEL_ID_DIGITAL2
						title = title & "DIGITAL2"
				End Select
			Else
				title = title & "AUDIO_INTERNAL"
			End If

			title = title & " has id = "
			NGIO.Device_DDSMem_GetSensorNumber(device_handle, activeChannel, sensorId, 0, sig, 0)
			title = title & sensorId.ToString()

			NGIO.Device_DDSMem_GetLongName(device_handle, activeChannel, longName, 30)
			longName2 = longName.ToString()
			If longName2.Length() > 0 Then
				title = title & " ( " & longName2 & " ) "
			End If

			title = title & ", probe type is "
			probeType = NGIO.Device_GetProbeType(device_handle, activeChannel)

			Select Case probeType
				Case VSTSensorDDSMemRec.kProbeTypeAnalog5V
					title = title & "ANALOG5V"
				Case VSTSensorDDSMemRec.kProbeTypeAnalog10V
					title = title & "ANALOG10V"
				Case VSTSensorDDSMemRec.kProbeTypeMD
					title = title & "Motion Detector"
				Case VSTSensorDDSMemRec.kProbeTypePhotoGate
					title = title & "Photogate"
				Case VSTSensorDDSMemRec.kProbeTypeDigitalCount
					title = title & "Digital Count"
				Case VSTSensorDDSMemRec.kProbeTypeRotary
					title = title & "Rotary"
				Case VSTSensorDDSMemRec.kProbeTypeLabquestAudio
					title = title & "LabQuest Audio"
				Case Else
					title = title & "?"
			End Select

		End If
		Me.Text = title
	End Sub

	Private Function GetUnits() As String
		Dim units As StringBuilder = New StringBuilder(20)
		Dim units2 As String
		Dim device_handle As IntPtr = GetOpenDeviceHandle(False)
		If (IntPtr.Zero <> device_handle) Then
			Dim CalPageIndex As Byte = 0
			Dim A, B, C As Single
			NGIO.Device_DDSMem_GetActiveCalPage(device_handle, activeChannel, CalPageIndex)
			NGIO.Device_DDSMem_GetCalPage(device_handle, activeChannel, CalPageIndex, A, B, C, units, 20)
			units2 = units.ToString()
		Else
			units2 = ""
		End If
		Return units2
	End Function


	Private Sub UpdateUnits()
		Dim units As String = GetUnits()
		UnitsLabel.Text = units
	End Sub

	Private Sub UpdateChannelMenu( _
	  ByVal device_handle As IntPtr, _
	  ByVal channel As SByte, _
	  ByVal tenVoltRange As Boolean, _
	  ByVal sampling_mode As Byte)
		Dim index As Integer = 0
		Dim listItem As ToolStripItem
		Dim menuItem As ToolStripMenuItem

		If IntPtr.Zero = device_handle Then
			For index = 0 To (ChannelsToolStripMenuItem.DropDownItems.Count() - 1)
				listItem = ChannelsToolStripMenuItem.DropDownItems(index)
				menuItem = CType(listItem, ToolStripMenuItem)
				menuItem.Enabled = False
			Next
		Else
			For index = 0 To (ChannelsToolStripMenuItem.DropDownItems.Count() - 2)
				listItem = ChannelsToolStripMenuItem.DropDownItems(index)
				menuItem = CType(listItem, ToolStripMenuItem)
				menuItem.Enabled = True
			Next

			'Handle audio device case.
			listItem = ChannelsToolStripMenuItem.DropDownItems(ChannelsToolStripMenuItem.DropDownItems.Count() - 1)
			menuItem = CType(listItem, ToolStripMenuItem)
            If (NGIO.DEVTYPE_LABQUEST = openDeviceType) Or (NGIO.DEVTYPE_LABQUEST2 = openDeviceType) Then
                menuItem.Enabled = True
            Else
                menuItem.Enabled = False
            End If
		End If

		'Clear all the checks.
		For index = 0 To (ChannelsToolStripMenuItem.DropDownItems.Count() - 1)
			listItem = ChannelsToolStripMenuItem.DropDownItems(index)
			menuItem = CType(listItem, ToolStripMenuItem)
			menuItem.Checked = False
		Next

		If (device_handle = NGIO_audio_device_handle) And (device_handle <> IntPtr.Zero) Then
			index = 15
		ElseIf IntPtr.Zero = device_handle Then
			index = ChannelsToolStripMenuItem.DropDownItems.Count()
		Else
			Select Case channel
				Case NGIO_ParmBlk.CHANNEL_ID_ANALOG1
					If tenVoltRange Then
						index = 1
					Else
						index = 0
					End If
				Case NGIO_ParmBlk.CHANNEL_ID_ANALOG2
					index = 2
				Case NGIO_ParmBlk.CHANNEL_ID_ANALOG3
					index = 3
				Case NGIO_ParmBlk.CHANNEL_ID_ANALOG4
					index = 4
				Case NGIO_ParmBlk.CHANNEL_ID_DIGITAL1
					Select Case sampling_mode
						Case NGIOSetSamplingModeParams.SAMPLING_MODE_APERIODIC_EDGE_DETECT
							index = 5
						Case NGIOSetSamplingModeParams.SAMPLING_MODE_PERIODIC_MOTION_DETECT
							index = 6
						Case NGIOSetSamplingModeParams.SAMPLING_MODE_PERIODIC_ROTATION_COUNTER
							index = 7
						Case NGIOSetSamplingModeParams.SAMPLING_MODE_PERIODIC_ROTATION_COUNTER_X4
							index = 8
						Case NGIOSetSamplingModeParams.SAMPLING_MODE_PERIODIC_PULSE_COUNT
							index = 9
					End Select
				Case NGIO_ParmBlk.CHANNEL_ID_DIGITAL2
					Select Case sampling_mode
						Case NGIOSetSamplingModeParams.SAMPLING_MODE_APERIODIC_EDGE_DETECT
							index = 10
						Case NGIOSetSamplingModeParams.SAMPLING_MODE_PERIODIC_MOTION_DETECT
							index = 11
						Case NGIOSetSamplingModeParams.SAMPLING_MODE_PERIODIC_ROTATION_COUNTER
							index = 12
						Case NGIOSetSamplingModeParams.SAMPLING_MODE_PERIODIC_ROTATION_COUNTER_X4
							index = 13
						Case NGIOSetSamplingModeParams.SAMPLING_MODE_PERIODIC_PULSE_COUNT
							index = 14
					End Select
			End Select
		End If

		If index < ChannelsToolStripMenuItem.DropDownItems.Count() Then
			listItem = ChannelsToolStripMenuItem.DropDownItems(index)
			menuItem = CType(listItem, ToolStripMenuItem)
			menuItem.Checked = True
		End If

	End Sub

	Private Sub SetMeasurementPeriodToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles SetMeasurementPeriodToolStripMenuItem.Click
		Dim DialogBox As New SetMeasurementPeriodDlg()
		Dim perioduS As Int32 = CInt((measPeriodInSeconds + 0.0000001) / 0.000001)

		DialogBox.period_uS_textbox.Text = perioduS.ToString()
		If DialogBox.ShowDialog() = Windows.Forms.DialogResult.OK Then
			perioduS = CInt(DialogBox.period_uS_textbox.Text)
			If perioduS >= 10 Then
				measPeriodInSeconds = perioduS * 0.000001
				If (IntPtr.Zero <> GetOpenDeviceHandle(False)) Then
					'Set desired sampling period.
					NGIO.Device_SetMeasurementPeriod(GetOpenDeviceHandle(False), -1, measPeriodInSeconds, NGIO.TIMEOUT_MS_DEFAULT)
					'Find out what actual sampling period is.
					NGIO.Device_GetMeasurementPeriod(GetOpenDeviceHandle(False), -1, measPeriodInSeconds, NGIO.TIMEOUT_MS_DEFAULT)
				End If
			End If
		End If
	End Sub

	Private Function GetOpenDeviceHandle(ByVal bMainDeviceOnly As Boolean) As IntPtr
		Dim device_handle As IntPtr = IntPtr.Zero

		If bMainDeviceOnly Then
			If IntPtr.Zero <> NGIO_audio_device_handle Then
				NGIO.Device_Close(NGIO_audio_device_handle)
			End If
			NGIO_audio_device_handle = IntPtr.Zero
			device_handle = NGIO_device_handle
		Else
			If IntPtr.Zero <> NGIO_audio_device_handle Then
				device_handle = NGIO_audio_device_handle
			Else
				device_handle = NGIO_device_handle
			End If
		End If
		Return device_handle
	End Function

	Private Sub SetDisplayDepthToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles SetDisplayDepthToolStripMenuItem.Click
		Dim DialogBox As New SetDisplayDepthDlg()
		Dim depth As Int32 = measCirBuf.MaxNumElementsAvailable()

		DialogBox.displayDepthTextBox.Text = depth.ToString()
		If DialogBox.ShowDialog() = Windows.Forms.DialogResult.OK Then
			depth = CInt(DialogBox.displayDepthTextBox.Text)
			If depth <> measCirBuf.MaxNumElementsAvailable() Then
				If (depth >= 10) And (depth <= 100000) Then
					measCirBuf = New MeasurementTuplesCirBuf(depth)
				End If
			End If
		End If
	End Sub

	Private Sub NGIOLibVerboseToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles NGIOLibVerboseToolStripMenuItem.Click
		Dim trace_threshold As Integer = 0
		NGIO.Diags_GetDebugTraceThreshold(NGIO_lib_handle, trace_threshold)
		If trace_threshold <= NGIO.TRACE_SEVERITY_LOWEST Then
			NGIO.Diags_SetDebugTraceThreshold(NGIO_lib_handle, NGIO.TRACE_SEVERITY_HIGH)
			NGIOLibVerboseToolStripMenuItem.Checked = False
		Else
			NGIO.Diags_SetDebugTraceThreshold(NGIO_lib_handle, NGIO.TRACE_SEVERITY_LOWEST)
			NGIOLibVerboseToolStripMenuItem.Checked = True
		End If
	End Sub

	Private Sub AboutNGIOMeasureToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles AboutNGIOMeasureToolStripMenuItem.Click
		Dim DialogBox As New AboutDlg()
		Dim sVersionLabel As String = "This app is linked to NGIO library version "
		sVersionLabel = sVersionLabel & lib_major_version.ToString("D1") & "." & lib_minor_version.ToString("D2")
		DialogBox.NGIO_lib_version_label.Text = sVersionLabel
		DialogBox.ShowDialog()
	End Sub

	Private Sub NewToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles NewToolStripMenuItem.Click
		StopMeasurements()
		UpdateControls()
		OnAnalogChannel(NGIO_ParmBlk.CHANNEL_ID_ANALOG1)
	End Sub

	Private Sub ExitToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ExitToolStripMenuItem.Click
		Me.Close()
	End Sub

	Private Sub Calib0ToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Calib0ToolStripMenuItem.Click
		Dim device_handle As IntPtr = GetOpenDeviceHandle(False)
		Dim activeCalPageIndex As Byte = 0
		If (IntPtr.Zero <> device_handle) And (device_handle <> NGIO_audio_device_handle) Then
			NGIO.Device_DDSMem_GetActiveCalPage(device_handle, activeChannel, activeCalPageIndex)
			If activeCalPageIndex <> 0 Then
				NGIO.Device_DDSMem_SetActiveCalPage(device_handle, activeChannel, 0)
				ClearGraph()
				UpdateUnits()
			End If
		End If
	End Sub

	Private Sub Calib1ToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Calib1ToolStripMenuItem.Click
		Dim device_handle As IntPtr = GetOpenDeviceHandle(False)
		If (IntPtr.Zero <> device_handle) And (device_handle <> NGIO_audio_device_handle) Then
			Dim highestValidCalPageIndex As Byte = 0
			Dim activeCalPageIndex As Byte = 0
			NGIO.Device_DDSMem_GetHighestValidCalPageIndex(device_handle, activeChannel, highestValidCalPageIndex)
			If highestValidCalPageIndex > 0 Then
				NGIO.Device_DDSMem_GetActiveCalPage(device_handle, activeChannel, activeCalPageIndex)
				If activeCalPageIndex <> 1 Then
					NGIO.Device_DDSMem_SetActiveCalPage(device_handle, activeChannel, 1)
					ClearGraph()
					UpdateUnits()
				End If
			End If
		End If
	End Sub

	Private Sub Calib2ToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Calib2ToolStripMenuItem.Click
		Dim device_handle As IntPtr = GetOpenDeviceHandle(False)
		If (IntPtr.Zero <> device_handle) And (device_handle <> NGIO_audio_device_handle) Then
			Dim highestValidCalPageIndex As Byte = 0
			Dim activeCalPageIndex As Byte = 0
			NGIO.Device_DDSMem_GetHighestValidCalPageIndex(device_handle, activeChannel, highestValidCalPageIndex)
			If highestValidCalPageIndex > 1 Then
				NGIO.Device_DDSMem_GetActiveCalPage(device_handle, activeChannel, activeCalPageIndex)
				If activeCalPageIndex <> 2 Then
					NGIO.Device_DDSMem_SetActiveCalPage(device_handle, activeChannel, 2)
					ClearGraph()
					UpdateUnits()
				End If
			End If
		End If
	End Sub

	Private Sub CalibrationsToolStripMenuItem_DropDownOpening(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CalibrationsToolStripMenuItem.DropDownOpening
		Me.CalibrationsToolStripMenuItem.DropDownItems.Clear()
		Dim device_handle As IntPtr = GetOpenDeviceHandle(False)
		If (IntPtr.Zero <> device_handle) And (device_handle <> NGIO_audio_device_handle) Then
			Dim activeCalPageIndex As Byte = 0
			Dim highestValidCalPageIndex As Byte = 0
			Dim units As StringBuilder = New StringBuilder(20)
			Dim offset, gain, dummy As Single

			NGIO.Device_DDSMem_GetHighestValidCalPageIndex(device_handle, activeChannel, highestValidCalPageIndex)
			NGIO.Device_DDSMem_GetActiveCalPage(device_handle, activeChannel, activeCalPageIndex)

			NGIO.Device_DDSMem_GetCalPage(device_handle, activeChannel, 0, offset, gain, dummy, units, 20)
			Me.Calib0ToolStripMenuItem.Text = "Calib 0 " & units.ToString()
			If 0 <> activeCalPageIndex Then
				Me.Calib0ToolStripMenuItem.Checked = False
			Else
				Me.Calib0ToolStripMenuItem.Checked = True
			End If
			Me.CalibrationsToolStripMenuItem.DropDownItems.Add(Me.Calib0ToolStripMenuItem)

			If highestValidCalPageIndex > 0 Then
				NGIO.Device_DDSMem_GetCalPage(device_handle, activeChannel, 1, offset, gain, dummy, units, 20)
				Me.Calib1ToolStripMenuItem.Text = "Calib 1 " & units.ToString()
				If 1 <> activeCalPageIndex Then
					Me.Calib1ToolStripMenuItem.Checked = False
				Else
					Me.Calib1ToolStripMenuItem.Checked = True
				End If
				Me.CalibrationsToolStripMenuItem.DropDownItems.Add(Me.Calib1ToolStripMenuItem)
			End If

			If highestValidCalPageIndex > 1 Then
				NGIO.Device_DDSMem_GetCalPage(device_handle, activeChannel, 2, offset, gain, dummy, units, 20)
				Me.Calib2ToolStripMenuItem.Text = "Calib 2 " & units.ToString()
				If 2 <> activeCalPageIndex Then
					Me.Calib2ToolStripMenuItem.Checked = False
				Else
					Me.Calib2ToolStripMenuItem.Checked = True
				End If
				Me.CalibrationsToolStripMenuItem.DropDownItems.Add(Me.Calib2ToolStripMenuItem)
			End If
		End If
	End Sub
End Class
