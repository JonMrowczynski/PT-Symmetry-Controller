/*********************************************************************************

Copyright (c) 2010, Vernier Software & Technology
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of Vernier Software & Technology nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL VERNIER SOFTWARE & TECHNOLOGY BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

**********************************************************************************/
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "NGIO_Measure.h"
#include "NGIO_MeasureDoc.h"
#include "NGIO_MeasureView.h"
#include "SetMeasPeriodDlg.h"
#include "DisplayDepthDlg.h"
#include "GVernierUSB.h"

#include "MainFrm.h"

#define TIMER_PERIOD_MS 70

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define NUM_MEASUREMENTS_IN_LOCAL_BIG_BUF 100000
gtype_int32 g_measurements[NUM_MEASUREMENTS_IN_LOCAL_BIG_BUF];
gtype_int64 g_times[NUM_MEASUREMENTS_IN_LOCAL_BIG_BUF];

static BYTE ConvertBCDStringToByte(LPCSTR str);
static void ConvertByteToBCDString(BYTE b, CString &str);

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_INITMENUPOPUP()
	ON_COMMAND(IDM_DEVICE0, OnDevice0)
	ON_COMMAND(IDM_DEVICE1, OnDevice1)
	ON_COMMAND(IDM_DEVICE2, OnDevice2)
	ON_COMMAND(IDM_DEVICE3, OnDevice3)
	ON_COMMAND(IDM_DEVICE4, OnDevice4)
	ON_COMMAND(IDM_DEVICE5, OnDevice5)
	ON_COMMAND(IDM_DEVICE6, OnDevice6)
	ON_COMMAND(IDM_DEVICE7, OnDevice7)
	ON_WM_TIMER()
	ON_UPDATE_COMMAND_UI(IDM_GET_STATUS, OnUpdateGetStatus)
	ON_COMMAND(IDM_GET_STATUS, OnGetStatus)
	ON_UPDATE_COMMAND_UI(IDM_SET_MEAS_PERIOD, OnUpdateSetMeasPeriod)
	ON_COMMAND(IDM_SET_MEAS_PERIOD, OnSetMeasPeriod)
	ON_UPDATE_COMMAND_UI(IDM_START_MEAS, OnUpdateStartMeas)
	ON_COMMAND(IDM_START_MEAS, OnStartMeas)
	ON_UPDATE_COMMAND_UI(IDM_STOP_MEAS, OnUpdateStopMeas)
	ON_COMMAND(IDM_STOP_MEAS, OnStopMeas)
	ON_COMMAND(IDM_CALIB0, OnCalib0)
	ON_COMMAND(IDM_CALIB1, OnCalib1)
	ON_COMMAND(IDM_CALIB2, OnCalib2)
	ON_COMMAND(IDC_ANALOG1, OnAnalog1)
	ON_COMMAND(IDC_ANALOG2, OnAnalog2)
	ON_COMMAND(IDC_ANALOG3, OnAnalog3)
	ON_COMMAND(IDC_ANALOG4, OnAnalog4)
	ON_COMMAND(ID_CHANNELS_DIGITAL1, OnChannelsDigital1)
	ON_COMMAND(ID_CHANNELS_DIGITAL2, OnChannelsDigital2)
	ON_COMMAND(IDC_ANALOG1_10V, OnAnalog110v)
	ON_COMMAND(ID_CHANNELS_DIGITAL1_MOTION, OnChannelsDigital1Motion)
	ON_COMMAND(ID_CHANNELS_DIGITAL2_MOTION, OnChannelsDigital2Motion)
	ON_COMMAND(ID_CHANNELS_DIGITAL1_ROTARY, OnChannelsDigital1Rotary)
	ON_COMMAND(ID_CHANNELS_DIGITAL1_RADIATION, OnChannelsDigital1Radiation)
	ON_COMMAND(ID_CHANNELS_DIGITAL2_ROTARY, OnChannelsDigital2Rotary)
	ON_COMMAND(ID_CHANNELS_DIGITAL2_RADIATION, OnChannelsDigital2Radiation)
	ON_COMMAND(ID_CHANNELS_DIGITAL1_ROTARY_X4, OnChannelsDigital1RotaryX4)
	ON_COMMAND(ID_CHANNELS_DIGITAL2_ROTARY_X4, OnChannelsDigital2RotaryX4)
//	ON_UPDATE_COMMAND_UI(ID_CHANNELS_AUDIO_INTERNAL, OnUpdateChannelsAudioInternal)
	ON_COMMAND(ID_CHANNELS_AUDIO_INTERNAL, OnChannelsAudioInternal)
	ON_UPDATE_COMMAND_UI(IDM_NGIO_LIB_VERBOSE, OnUpdateNgioLibVerbose)
	ON_COMMAND(IDM_NGIO_LIB_VERBOSE, OnNgioLibVerbose)
	ON_UPDATE_COMMAND_UI(ID_ACTION_SETDISPLAYDEPTH, OnUpdateActionSetdisplaydepth)
	ON_COMMAND(ID_ACTION_SETDISPLAYDEPTH, OnActionSetdisplaydepth)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_timerId = 0;
	m_bIsCollectingMeasurements = false;
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	m_wndSendBar.Create(this, IDD_PACKET_BAR, CBRS_TOP, IDD_PACKET_BAR);
	m_wndSendBar.EnableDocking(CBRS_ALIGN_ANY);

	EnableDocking(CBRS_ALIGN_ANY);

	DockControlBar(&m_wndSendBar);
	CString str;
	ConvertByteToBCDString(0, str);
	for (int i = IDC_PACKET_BYTE; i <= IDC_PACKET_BYTE8; i++)
	{
		CEdit *pEdit = (CEdit *) m_wndSendBar.GetDlgItem(i);
		if (pEdit)
		{
			pEdit->SetLimitText(2);
			pEdit->SetWindowText(str);
		}
	}

	m_timerId = SetTimer(777, TIMER_PERIOD_MS, NULL);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

// CMainFrame message handlers

void CMainFrame::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu) 
{
	CFrameWnd::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
	CNGIO_MeasureDoc *pDoc = (CNGIO_MeasureDoc *) GetActiveDocument();
	NGIO_LIBRARY_HANDLE hNGIO_lib = ((CNGIO_MeasureApp *) AfxGetApp())->m_hNGIO_lib;

	if ((1 == nIndex) && pDoc && hNGIO_lib)
	{
		gtype_uint32 sig, numLabpro2s, numLabQuestMinis, numLabquest2s, status;
		NGIO_DEVICE_LIST_HANDLE hLabpro2List, hLabQuestMiniList, hLabquest2List;

		NGIO_SearchForDevices(hNGIO_lib, NGIO_DEVTYPE_LABQUEST, NGIO_COMM_TRANSPORT_USB, NULL, &sig);//spam
		hLabpro2List = NGIO_OpenDeviceListSnapshot(hNGIO_lib, NGIO_DEVTYPE_LABQUEST, &numLabpro2s, &sig);
		if (!hLabpro2List)
			numLabpro2s = 0;

		char openDeviceName[NGIO_MAX_SIZE_DEVICE_NAME];
		char newDeviceName[NGIO_MAX_SIZE_DEVICE_NAME];
		if (pDoc->GetOpenDevicePtr())
		{
			NGIO_Device_GetOpenDeviceName(pDoc->GetOpenDevicePtr(), openDeviceName, NGIO_MAX_SIZE_DEVICE_NAME);
		}
		else
			openDeviceName[0] = 0;

		while (pPopupMenu->GetMenuItemCount() > 0)
			pPopupMenu->DeleteMenu(0, MF_BYPOSITION);

		unsigned int i, j;
		i = 0;
		for (j = 0; j < numLabpro2s; j++)
		{
			if (0 == NGIO_DeviceListSnapshot_GetNthEntry(hLabpro2List, j, newDeviceName, NGIO_MAX_SIZE_DEVICE_NAME, &status))
			{
				pPopupMenu->AppendMenu(MF_STRING, IDM_DEVICE0 + i, newDeviceName);
				if (0 == lstrcmp(openDeviceName, newDeviceName))
					pPopupMenu->CheckMenuItem(IDM_DEVICE0 + i, MF_BYCOMMAND | MF_CHECKED);
				i++;
			}
		}

		NGIO_CloseDeviceListSnapshot(hLabpro2List);

		NGIO_SearchForDevices(hNGIO_lib, NGIO_DEVTYPE_LABQUEST_MINI, NGIO_COMM_TRANSPORT_USB, NULL, &sig);//spam
		hLabQuestMiniList = NGIO_OpenDeviceListSnapshot(hNGIO_lib, NGIO_DEVTYPE_LABQUEST_MINI, &numLabQuestMinis, &sig);
		if (!hLabQuestMiniList)
			numLabQuestMinis = 0;

		for (j = 0; j < numLabQuestMinis; j++)
		{
			if (0 == NGIO_DeviceListSnapshot_GetNthEntry(hLabQuestMiniList, j, newDeviceName, NGIO_MAX_SIZE_DEVICE_NAME, &status))
			{
				pPopupMenu->AppendMenu(MF_STRING, IDM_DEVICE0 + i, newDeviceName);
				if (0 == lstrcmp(openDeviceName, newDeviceName))
					pPopupMenu->CheckMenuItem(IDM_DEVICE0 + i, MF_BYCOMMAND | MF_CHECKED);
				i++;
			}
		}

		NGIO_CloseDeviceListSnapshot(hLabQuestMiniList);

		NGIO_SearchForDevices(hNGIO_lib, NGIO_DEVTYPE_LABQUEST2, NGIO_COMM_TRANSPORT_USB, NULL, &sig);//spam
		hLabquest2List = NGIO_OpenDeviceListSnapshot(hNGIO_lib, NGIO_DEVTYPE_LABQUEST2, &numLabquest2s, &sig);
		if (!hLabquest2List)
			numLabquest2s = 0;

		for (j = 0; j < numLabquest2s; j++)
		{
			if (0 == NGIO_DeviceListSnapshot_GetNthEntry(hLabquest2List, j, newDeviceName, NGIO_MAX_SIZE_DEVICE_NAME, &status))
			{
				pPopupMenu->AppendMenu(MF_STRING, IDM_DEVICE0 + i, newDeviceName);
				if (0 == lstrcmp(openDeviceName, newDeviceName))
					pPopupMenu->CheckMenuItem(IDM_DEVICE0 + i, MF_BYCOMMAND | MF_CHECKED);
				i++;
			}
		}

		NGIO_CloseDeviceListSnapshot(hLabquest2List);
	}
	else
	if ((2 == nIndex) && pDoc)
	{
		int ctrlId, enable_flags, enable_audio_flags;
		if (m_bIsCollectingMeasurements || (NULL == pDoc->GetOpenDevicePtr()))
			enable_flags = MF_GRAYED | MF_BYCOMMAND;
		else
			enable_flags = MF_ENABLED | MF_BYCOMMAND;

		enable_audio_flags = MF_GRAYED | MF_BYCOMMAND;
		if ((!m_bIsCollectingMeasurements) && (NULL != pDoc->GetParentDevicePtr()))
		{
			char parent_devstring[NGIO_MAX_SIZE_DEVICE_NAME];
			gtype_uint32 deviceType;
			NGIO_DEVICE_HANDLE hParentDev = pDoc->GetParentDevicePtr();
			NGIO_Device_GetOpenDeviceName(hParentDev, parent_devstring, NGIO_MAX_SIZE_DEVICE_NAME);
			if (NGIO_GetDeviceTypeFromDeviceName(parent_devstring, &deviceType) >= 0)
			{
				if ((NGIO_DEVTYPE_LABQUEST == deviceType) || (NGIO_DEVTYPE_LABQUEST2 == deviceType))
					enable_audio_flags = MF_ENABLED | MF_BYCOMMAND;
			}
		}
		pPopupMenu->EnableMenuItem(IDC_ANALOG1, enable_flags);
		pPopupMenu->EnableMenuItem(IDC_ANALOG1_10V, enable_flags);
		pPopupMenu->EnableMenuItem(IDC_ANALOG2, enable_flags);
		pPopupMenu->EnableMenuItem(IDC_ANALOG3, enable_flags);
		pPopupMenu->EnableMenuItem(IDC_ANALOG4, enable_flags);
		pPopupMenu->EnableMenuItem(ID_CHANNELS_DIGITAL1, enable_flags);
		pPopupMenu->EnableMenuItem(ID_CHANNELS_DIGITAL1_MOTION, enable_flags);
		pPopupMenu->EnableMenuItem(ID_CHANNELS_DIGITAL1_ROTARY, enable_flags);
		pPopupMenu->EnableMenuItem(ID_CHANNELS_DIGITAL1_ROTARY_X4, enable_flags);
		pPopupMenu->EnableMenuItem(ID_CHANNELS_DIGITAL1_RADIATION, enable_flags);
		pPopupMenu->EnableMenuItem(ID_CHANNELS_DIGITAL2, enable_flags);
		pPopupMenu->EnableMenuItem(ID_CHANNELS_DIGITAL2_MOTION, enable_flags);
		pPopupMenu->EnableMenuItem(ID_CHANNELS_DIGITAL2_ROTARY, enable_flags);
		pPopupMenu->EnableMenuItem(ID_CHANNELS_DIGITAL2_ROTARY_X4, enable_flags);
		pPopupMenu->EnableMenuItem(ID_CHANNELS_DIGITAL2_RADIATION, enable_flags);
		pPopupMenu->EnableMenuItem(ID_CHANNELS_AUDIO_INTERNAL, enable_audio_flags);

		pPopupMenu->CheckMenuItem(IDC_ANALOG1, MF_BYCOMMAND | MF_UNCHECKED);
		pPopupMenu->CheckMenuItem(IDC_ANALOG1_10V, MF_BYCOMMAND | MF_UNCHECKED);
		pPopupMenu->CheckMenuItem(IDC_ANALOG2, MF_BYCOMMAND | MF_UNCHECKED);
		pPopupMenu->CheckMenuItem(IDC_ANALOG3, MF_BYCOMMAND | MF_UNCHECKED);
		pPopupMenu->CheckMenuItem(IDC_ANALOG4, MF_BYCOMMAND | MF_UNCHECKED);
		pPopupMenu->CheckMenuItem(ID_CHANNELS_DIGITAL1, MF_BYCOMMAND | MF_UNCHECKED);
		pPopupMenu->CheckMenuItem(ID_CHANNELS_DIGITAL1_MOTION, MF_BYCOMMAND | MF_UNCHECKED);
		pPopupMenu->CheckMenuItem(ID_CHANNELS_DIGITAL1_ROTARY, MF_BYCOMMAND | MF_UNCHECKED);
		pPopupMenu->CheckMenuItem(ID_CHANNELS_DIGITAL1_ROTARY_X4, MF_BYCOMMAND | MF_UNCHECKED);
		pPopupMenu->CheckMenuItem(ID_CHANNELS_DIGITAL1_RADIATION, MF_BYCOMMAND | MF_UNCHECKED);
		pPopupMenu->CheckMenuItem(ID_CHANNELS_DIGITAL2, MF_BYCOMMAND | MF_UNCHECKED);
		pPopupMenu->CheckMenuItem(ID_CHANNELS_DIGITAL2_MOTION, MF_BYCOMMAND | MF_UNCHECKED);
		pPopupMenu->CheckMenuItem(ID_CHANNELS_DIGITAL2_ROTARY, MF_BYCOMMAND | MF_UNCHECKED);
		pPopupMenu->CheckMenuItem(ID_CHANNELS_DIGITAL2_ROTARY_X4, MF_BYCOMMAND | MF_UNCHECKED);
		pPopupMenu->CheckMenuItem(ID_CHANNELS_DIGITAL2_RADIATION, MF_BYCOMMAND | MF_UNCHECKED);
		pPopupMenu->CheckMenuItem(ID_CHANNELS_AUDIO_INTERNAL, MF_BYCOMMAND | MF_UNCHECKED);
		if (pDoc->GetAudioDevicePtr())
			ctrlId = ID_CHANNELS_AUDIO_INTERNAL;
		else
		{
			switch (pDoc->GetActiveChannel())
			{
				case NGIO_CHANNEL_ID_ANALOG1:
					if (pDoc->Get10voltRangeFlag())
						ctrlId = IDC_ANALOG1_10V;
					else
						ctrlId = IDC_ANALOG1;
					break;
				case NGIO_CHANNEL_ID_ANALOG2:
					ctrlId = IDC_ANALOG2;
					break;
				case NGIO_CHANNEL_ID_ANALOG3:
					ctrlId = IDC_ANALOG3;
					break;
				case NGIO_CHANNEL_ID_ANALOG4:
					ctrlId = IDC_ANALOG4;
					break;
				case NGIO_CHANNEL_ID_DIGITAL1:
					switch (pDoc->GetDigitalCollectionMode())
					{
						case NGIO_SAMPLING_MODE_PERIODIC_PULSE_COUNT:
							ctrlId = ID_CHANNELS_DIGITAL1_RADIATION;
							break;
						case NGIO_SAMPLING_MODE_PERIODIC_MOTION_DETECT:
							ctrlId = ID_CHANNELS_DIGITAL1_MOTION;
							break;
						case NGIO_SAMPLING_MODE_PERIODIC_ROTATION_COUNTER:
							ctrlId = ID_CHANNELS_DIGITAL1_ROTARY;
							break;
						case NGIO_SAMPLING_MODE_PERIODIC_ROTATION_COUNTER_X4:
							ctrlId = ID_CHANNELS_DIGITAL1_ROTARY_X4;
							break;
						default:
							ctrlId = ID_CHANNELS_DIGITAL1;
							break;
					}
					break;
				case NGIO_CHANNEL_ID_DIGITAL2:
					switch (pDoc->GetDigitalCollectionMode())
					{
						case NGIO_SAMPLING_MODE_PERIODIC_PULSE_COUNT:
							ctrlId = ID_CHANNELS_DIGITAL2_RADIATION;
							break;
						case NGIO_SAMPLING_MODE_PERIODIC_MOTION_DETECT:
							ctrlId = ID_CHANNELS_DIGITAL2_MOTION;
							break;
						case NGIO_SAMPLING_MODE_PERIODIC_ROTATION_COUNTER:
							ctrlId = ID_CHANNELS_DIGITAL2_ROTARY;
							break;
						case NGIO_SAMPLING_MODE_PERIODIC_ROTATION_COUNTER_X4:
							ctrlId = ID_CHANNELS_DIGITAL2_ROTARY_X4;
							break;
						default:
							ctrlId = ID_CHANNELS_DIGITAL2;
							break;
					}
					break;
				default:
					ctrlId = IDC_ANALOG1;
					break;
			}
		}
		pPopupMenu->CheckMenuItem(ctrlId, MF_BYCOMMAND | MF_CHECKED);
	}
	else
	if ((3 == nIndex) && pDoc)
	{
		NGIO_DEVICE_HANDLE hDevice = pDoc->GetOpenDevicePtr();
		while (pPopupMenu->GetMenuItemCount() > 0)
			pPopupMenu->DeleteMenu(0, MF_BYPOSITION);

		if (hDevice)
		{
			signed char channel = pDoc->GetActiveChannel();
			unsigned char nActiveIndex;
			NGIO_Device_DDSMem_GetActiveCalPage(hDevice, channel, &nActiveIndex);
			unsigned char numCalibrations;
			NGIO_Device_DDSMem_GetHighestValidCalPageIndex(hDevice, channel, &numCalibrations);
			numCalibrations++;
			char units[30];
			gtype_real32 coeffs[3];
			cppsstream ss;
			for (unsigned int k = 0; k < numCalibrations; k++)
			{
				NGIO_Device_DDSMem_GetCalPage(hDevice, channel, k, &coeffs[0], &coeffs[1], &coeffs[2], units, sizeof(units));
				ss.str("");
				ss << "Calib " << k << " " << cppstring(units);
				pPopupMenu->AppendMenu(MF_STRING, IDM_CALIB0 + k, ss.str().c_str());
				if (k == nActiveIndex)
					pPopupMenu->CheckMenuItem(IDM_CALIB0 + k, MF_BYCOMMAND | MF_CHECKED);
			}
		}
	}
}

void CMainFrame::OnDeviceN(unsigned int N) 
{
	CMenu *pMenu = GetMenu();
	CNGIO_MeasureDoc *pDoc = (CNGIO_MeasureDoc *) GetActiveDocument();
	if (pMenu && pDoc)
	{
		int count = pMenu->GetMenuItemCount();
		CMenu *pSubMenu = pMenu->GetSubMenu(1);
		if (pSubMenu)
		{
			unsigned int menuState = pSubMenu->GetMenuState(N, MF_BYPOSITION);
			if (menuState & MF_CHECKED)
				pDoc->CloseDevice();
			else
			{
				char tmpstring[NGIO_MAX_SIZE_DEVICE_NAME];
				gtype_uint32 deviceType;
				bool bSuccess = true;

				pSubMenu->GetMenuString(N, tmpstring, sizeof(tmpstring), MF_BYPOSITION);

				if (NGIO_GetDeviceTypeFromDeviceName(tmpstring, &deviceType) < 0)
					bSuccess = false;

				if (bSuccess)
				{
					pDoc->OpenDevice(tmpstring);
					OnAnalog1();
				}
			}
			m_bIsCollectingMeasurements = false;
		}
	}
}

void CMainFrame::OnDevice0() 
{
	OnDeviceN(0);
}

void CMainFrame::OnDevice1() 
{
	OnDeviceN(1);
}

void CMainFrame::OnDevice2() 
{
	OnDeviceN(2);
}

void CMainFrame::OnDevice3() 
{
	OnDeviceN(3);
}

void CMainFrame::OnDevice4() 
{
	OnDeviceN(4);
}

void CMainFrame::OnDevice5() 
{
	OnDeviceN(5);
}

void CMainFrame::OnDevice6() 
{
	OnDeviceN(6);
}

void CMainFrame::OnDevice7() 
{
	OnDeviceN(7);
}

BOOL CMainFrame::DestroyWindow() 
{
	if (m_timerId != 0)
		KillTimer(m_timerId);
	m_timerId = 0;
	
	return CFrameWnd::DestroyWindow();
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent) 
{
	CNGIO_MeasureDoc *pDoc = (CNGIO_MeasureDoc *) GetActiveDocument();
	CNGIO_MeasureView *pView = (CNGIO_MeasureView *) GetActiveView();
	bool bNewMeasurementAvailable = false;
	short iRawMeasVal = 0;
	if (pDoc && pView)
	{
		NGIO_DEVICE_HANDLE hDevice = pDoc->GetOpenDevicePtr();
		if (hDevice)
		{
			gtype_real32 rMeasurement, rTime, volts;
			int numMeasurementsAvailable = NGIO_Device_GetNumMeasurementsAvailable(hDevice, pDoc->GetActiveChannel());
			if (numMeasurementsAvailable > 0)
			{
				if (numMeasurementsAvailable > 39000)
					OutputDebugString("Measurement buffer is filling up.\n");

				numMeasurementsAvailable = NGIO_Device_ReadRawMeasurements(hDevice, pDoc->GetActiveChannel(),
					g_measurements, g_times, NUM_MEASUREMENTS_IN_LOCAL_BIG_BUF);
			}

			if (numMeasurementsAvailable > 0)
			{
				signed char channel = pDoc->GetActiveChannel();
				unsigned char probeType = NGIO_Device_GetProbeType(hDevice, channel);

				//Stuff the new measurements in a circular buffer.
				for (int k = 0; k < numMeasurementsAvailable; k++)
				{
					volts = NGIO_Device_ConvertToVoltage(hDevice, channel, g_measurements[k], probeType);
					rMeasurement = NGIO_Device_CalibrateData(hDevice, channel, volts);
					if (pDoc->GetAudioDevicePtr())
						rTime = (gtype_real32) (g_times[k]*pDoc->GetMeasurementPeriodInSeconds());
					else
						rTime = (gtype_real32) (g_times[k]*0.000001);
					pDoc->AddMeasurementToCirbuf(rMeasurement, rTime);
				}
				pView->Invalidate();
				pView->UpdateWindow();

				//Display latest measurement value on the toolbar.
				cppsstream ss;
				ss << fixed << uppercase << showpoint << setprecision(3) << rMeasurement;
				cppstring strMeas = ss.str();
				m_wndSendBar.SetDlgItemText(IDC_MEAS, strMeas.c_str());
			}
			else
			if (!m_bIsCollectingMeasurements)
				//Clear latest measurement value.
				m_wndSendBar.SetDlgItemText(IDC_MEAS, "");
		}
	}
}

void CMainFrame::OnUpdateGetStatus(CCmdUI* pCmdUI) 
{
	BOOL bEnable = FALSE;
	CNGIO_MeasureDoc *pDoc = (CNGIO_MeasureDoc *) GetActiveDocument();
	if (pDoc)
		bEnable = (pDoc->GetOpenDevicePtr() != NULL);
	pCmdUI->Enable(bEnable);
}

void CMainFrame::OnGetStatus() 
{
	CNGIO_MeasureDoc *pDoc = (CNGIO_MeasureDoc *) GetActiveDocument();
	CNGIO_MeasureView *pView = (CNGIO_MeasureView *) GetActiveView();
	if (pDoc && pView)
	{
		NGIO_DEVICE_HANDLE pDevice = pDoc->GetOpenDevicePtr();
		if (pDevice)
		{
			//Normally, you would do the following:
			NGIOGetStatusCmdResponsePayload statusRec;
			memset(&statusRec, 0, sizeof(statusRec));//Do this because Go! Temp does not set the slave CPU fields.
			unsigned int nBytesRead = sizeof(statusRec);
			int nResult = NGIO_Device_SendCmdAndGetResponse(pDevice, NGIO_CMD_ID_GET_STATUS, NULL, 0, &statusRec, 
				&nBytesRead, NGIO_TIMEOUT_MS_DEFAULT);
			if (0 == nResult)
			{
				int statusSummary = statusRec.status;
 				double version = (statusRec.majorVersionMasterCPU/0x10)*10.0 + (statusRec.majorVersionMasterCPU & 0xf)*1.0 + 
 						(statusRec.minorVersionMasterCPU/0x10)*0.1 + (statusRec.minorVersionMasterCPU & 0xf)*0.01 +
 					  (statusRec.majorVersionSlaveCPU/0x10)*0.001 + (statusRec.majorVersionSlaveCPU & 0xf)*0.0001 + 
 						(statusRec.minorVersionSlaveCPU/0x10)*0.00001 + (statusRec.minorVersionSlaveCPU & 0xf)*0.000001; 
			
				cppsstream ss;
				ss << "Status byte = " << hex << statusSummary << " ; DAQ firmware version = ";
				ss << fixed << uppercase << showpoint << setprecision(6) << version;
				cppstring str = ss.str();
				MessageBox(str.c_str(), "NGIO_CMD_ID_GET_STATUS");
			}
		}
	}
}

void CMainFrame::OnUpdateGetSensorId(CCmdUI* pCmdUI) 
{
	BOOL bEnable = FALSE;
	CNGIO_MeasureDoc *pDoc = (CNGIO_MeasureDoc *) GetActiveDocument();
	if (pDoc)
	{
		NGIO_DEVICE_HANDLE pDevice = pDoc->GetOpenDevicePtr();
		if (pDevice)
		{
			bEnable = TRUE;
		}
	}
	pCmdUI->Enable(bEnable);
}

void CMainFrame::OnGetSensorId() 
{
	CNGIO_MeasureDoc *pDoc = (CNGIO_MeasureDoc *) GetActiveDocument();
	CNGIO_MeasureView *pView = (CNGIO_MeasureView *) GetActiveView();
	if (pDoc && pView)
	{
		NGIO_DEVICE_HANDLE pDevice = pDoc->GetOpenDevicePtr();
		if (pDevice)
		{
			unsigned char sensorNumber;
			gtype_uint32 sig;
			NGIO_Device_DDSMem_GetSensorNumber(pDevice, pDoc->GetActiveChannel(), &sensorNumber, 1, &sig, NGIO_TIMEOUT_MS_DEFAULT);
		}
	}
}


void CMainFrame::OnUpdateSetMeasPeriod(CCmdUI* pCmdUI) 
{
	BOOL bEnable = FALSE;
	CNGIO_MeasureDoc *pDoc = (CNGIO_MeasureDoc *) GetActiveDocument();
	if (pDoc)
		bEnable = (pDoc->GetOpenDevicePtr() != NULL) && 
			(!m_bIsCollectingMeasurements);//GoIO_Sensor_SetMeasurementPeriod() fails while collecting.
	pCmdUI->Enable(bEnable);
}

void CMainFrame::OnSetMeasPeriod() 
{
	CNGIO_MeasureDoc *pDoc = (CNGIO_MeasureDoc *) GetActiveDocument();
	CNGIO_MeasureView *pView = (CNGIO_MeasureView *) GetActiveView();
	if (pDoc && pView)
	{
		NGIO_DEVICE_HANDLE pDevice = pDoc->GetOpenDevicePtr();
		if (pDevice)
		{
			CSetMeasPeriodDlg dlg;
			dlg.SetUnits("us");
			double period = 1.0;
			NGIO_Device_GetMeasurementPeriod(pDevice, -1, &period, NGIO_TIMEOUT_MS_DEFAULT);
			dlg.m_period = 
				(unsigned int) floor(1000000.0*period + 0.5);
			if (IDOK == dlg.DoModal())
			{
				NGIO_Device_SetMeasurementPeriod(pDevice, -1, dlg.m_period/1000000.0, NGIO_TIMEOUT_MS_DEFAULT);
				NGIO_Device_GetMeasurementPeriod(pDevice, -1, &period, NGIO_TIMEOUT_MS_DEFAULT);
				pDoc->SetMeasurementPeriodInSeconds(period);
			}
		}
	}
}

void CMainFrame::OnUpdateStartMeas(CCmdUI* pCmdUI) 
{
	BOOL bEnable = FALSE;
	CNGIO_MeasureDoc *pDoc = (CNGIO_MeasureDoc *) GetActiveDocument();
	if (pDoc)
		bEnable = (pDoc->GetOpenDevicePtr() != NULL) && (!m_bIsCollectingMeasurements);
	pCmdUI->Enable(bEnable);
}

void CMainFrame::OnStartMeas() 
{
	CNGIO_MeasureDoc *pDoc = (CNGIO_MeasureDoc *) GetActiveDocument();
	CNGIO_MeasureView *pView = (CNGIO_MeasureView *) GetActiveView();
	if (pDoc && pView)
	{
		NGIO_DEVICE_HANDLE pDevice = pDoc->GetOpenDevicePtr();
		if (pDevice)
		{
			ClearGraph();
			NGIO_Device_SendCmdAndGetResponse(pDevice, NGIO_CMD_ID_START_MEASUREMENTS, NULL, 0, NULL, NULL, 
				NGIO_TIMEOUT_MS_DEFAULT);

			m_bIsCollectingMeasurements = true;
		}
	}
}

void CMainFrame::OnUpdateStopMeas(CCmdUI* pCmdUI) 
{
	BOOL bEnable = FALSE;
	CNGIO_MeasureDoc *pDoc = (CNGIO_MeasureDoc *) GetActiveDocument();
	if (pDoc)
		bEnable = (pDoc->GetOpenDevicePtr() != NULL) && m_bIsCollectingMeasurements;
	pCmdUI->Enable(bEnable);
}

void CMainFrame::OnStopMeas() 
{
	CNGIO_MeasureDoc *pDoc = (CNGIO_MeasureDoc *) GetActiveDocument();
	CNGIO_MeasureView *pView = (CNGIO_MeasureView *) GetActiveView();
	if (pDoc && pView)
	{
		NGIO_DEVICE_HANDLE pDevice = pDoc->GetOpenDevicePtr();
		if (pDevice)
		{
			NGIO_Device_SendCmdAndGetResponse(pDevice, NGIO_CMD_ID_STOP_MEASUREMENTS, NULL, 0, NULL, NULL, 
				NGIO_TIMEOUT_MS_DEFAULT);

			m_bIsCollectingMeasurements = false;
		}
	}
}

static BYTE ConvertBCDStringToByte(LPCSTR pStr)
{
	WORD val = 0;
	BYTE testByte = (*pStr);
	while (testByte != 0)
	{
		if ((testByte >= 'a') && (testByte <= 'f'))
			testByte = 10 + testByte - 'a';
		else
		if ((testByte >= 'A') && (testByte <= 'F'))
			testByte = 10 + testByte - 'A';
		else
		if ((testByte >= '0') && (testByte <= '9'))
			testByte = testByte - '0';
		else
			testByte = 0;

		val = val*16 + testByte;
		
		pStr++;
		testByte = (*pStr);
	}

	return ((BYTE) val);
}

static void ConvertByteToBCDString(BYTE b, CString &str)
{
	str = "";
	BYTE firstDigit = b/16;
	BYTE secondDigit = b % 16;
	BYTE firstChar, secondChar;
	firstChar = (firstDigit < 10) ? ('0' + firstDigit) : ('A' + firstDigit - 10);
	str += firstChar;
	secondChar = (secondDigit < 10) ? ('0' + secondDigit) : ('A' + secondDigit - 10);
	str += secondChar;
}


void CMainFrame::OnCalib0() 
{
	OnCalibN(0);
}

void CMainFrame::OnCalib1() 
{
	OnCalibN(1);
}

void CMainFrame::OnCalib2() 
{
	OnCalibN(2);
}

void CMainFrame::OnCalibN(unsigned int N)
{
	CNGIO_MeasureDoc *pDoc = (CNGIO_MeasureDoc *) GetActiveDocument();
	if (pDoc)
	{
		NGIO_DEVICE_HANDLE pDevice = pDoc->GetOpenDevicePtr();
		if (pDevice)
		{
			signed char channel = pDoc->GetActiveChannel();
			unsigned char numCalibrations;
			NGIO_Device_DDSMem_GetHighestValidCalPageIndex(pDevice, channel, &numCalibrations);
			numCalibrations++;
			unsigned char oldN;
			NGIO_Device_DDSMem_GetActiveCalPage(pDevice, channel, &oldN);
			if ((N < numCalibrations) && (oldN != N))
			{
				NGIO_Device_DDSMem_SetActiveCalPage(pDevice, channel, N);
				ClearGraph();
				UpdateUnits();
			}
		}
	}
}

void CMainFrame::OnAnalog1() 
{
	CNGIO_MeasureDoc *pDoc = (CNGIO_MeasureDoc *) GetActiveDocument();
	NGIO_DEVICE_HANDLE pDevice = NULL;
	if (pDoc)
		pDevice = pDoc->GetOpenDevicePtr(true);
	if (pDevice)
	{
		pDoc->SetActiveChannel(NGIO_CHANNEL_ID_ANALOG1);
		pDoc->Set10voltRangeFlag(false);

		NGIOSetSensorChannelEnableMaskParams params;
		memset(&params, 0, sizeof(params));
		params.lsbyteLsword_EnableSensorChannels = NGIO_CHANNEL_MASK_ANALOG1;
		NGIO_Device_SendCmdAndGetResponse(pDevice, NGIO_CMD_ID_SET_SENSOR_CHANNEL_ENABLE_MASK, &params, sizeof(params), NULL, NULL, 
			NGIO_TIMEOUT_MS_DEFAULT);

		NGIOSetAnalogInputParams inputParams;
		inputParams.channel = NGIO_CHANNEL_ID_ANALOG1;
		inputParams.analogInput = NGIO_ANALOG_INPUT_5V_BUILTIN_12BIT_ADC;
		NGIO_Device_SendCmdAndGetResponse(pDevice, NGIO_CMD_ID_SET_ANALOG_INPUT, &inputParams, sizeof(inputParams), NULL, NULL, 
			NGIO_TIMEOUT_MS_DEFAULT);

		unsigned char sensorId = pDoc->UpdateSensorIdAndDDSRec(pDoc->GetActiveChannel());
		if (0 == sensorId)
		{
			//Force a default sensor.
			sensorId = 14; //5v voltage probe.
			NGIO_Device_DDSMem_SetSensorNumber(pDevice, pDoc->GetActiveChannel(), sensorId);
			NGIO_Device_DDSMem_SetLongName(pDevice, pDoc->GetActiveChannel(), "assumed");//Mark this as bogus.
			NGIO_Device_DDSMem_SetCalPage(pDevice, pDoc->GetActiveChannel(), 0, 0.0, 1.0, 0.0, "volts");//default calb.
		}
		pDoc->UpdateTitle();
		ClearGraph();
		UpdateUnits();
	
		if (NGIO_Device_GetProbeType(pDevice, pDoc->GetActiveChannel()) != kProbeTypeAnalog5V)
			MessageBox("Sensor plugged in is not compatible with selected mode.");
	}
}

void CMainFrame::OnAnalog2() 
{
	CNGIO_MeasureDoc *pDoc = (CNGIO_MeasureDoc *) GetActiveDocument();
	NGIO_DEVICE_HANDLE pDevice = NULL;
	if (pDoc)
		pDevice = pDoc->GetOpenDevicePtr(true);
	if (pDevice)
	{
		pDoc->SetActiveChannel(NGIO_CHANNEL_ID_ANALOG2);
		pDoc->Set10voltRangeFlag(false);

		NGIOSetSensorChannelEnableMaskParams params;
		memset(&params, 0, sizeof(params));
		params.lsbyteLsword_EnableSensorChannels = NGIO_CHANNEL_MASK_ANALOG2;
		NGIO_Device_SendCmdAndGetResponse(pDevice, NGIO_CMD_ID_SET_SENSOR_CHANNEL_ENABLE_MASK, &params, sizeof(params), NULL, NULL, 
			NGIO_TIMEOUT_MS_DEFAULT);

		NGIOSetAnalogInputParams inputParams;
		inputParams.channel = NGIO_CHANNEL_ID_ANALOG2;
		inputParams.analogInput = NGIO_ANALOG_INPUT_5V_BUILTIN_12BIT_ADC;
		NGIO_Device_SendCmdAndGetResponse(pDevice, NGIO_CMD_ID_SET_ANALOG_INPUT, &inputParams, sizeof(inputParams), NULL, NULL, 
			NGIO_TIMEOUT_MS_DEFAULT);

		unsigned char sensorId = pDoc->UpdateSensorIdAndDDSRec(pDoc->GetActiveChannel());
		if (0 == sensorId)
		{
			//Force a default sensor.
			sensorId = 14; //5v voltage probe.
			NGIO_Device_DDSMem_SetSensorNumber(pDevice, pDoc->GetActiveChannel(), sensorId);
			NGIO_Device_DDSMem_SetLongName(pDevice, pDoc->GetActiveChannel(), "assumed");//Mark this as bogus.
			NGIO_Device_DDSMem_SetCalPage(pDevice, pDoc->GetActiveChannel(), 0, 0.0, 1.0, 0.0, "volts");//default calb.
		}
		pDoc->UpdateTitle();
		ClearGraph();
		UpdateUnits();
	
		if (NGIO_Device_GetProbeType(pDevice, pDoc->GetActiveChannel()) != kProbeTypeAnalog5V)
			MessageBox("Sensor plugged in is not compatible with selected mode.");
	}
}

void CMainFrame::OnAnalog3() 
{
	CNGIO_MeasureDoc *pDoc = (CNGIO_MeasureDoc *) GetActiveDocument();
	NGIO_DEVICE_HANDLE pDevice = NULL;
	if (pDoc)
		pDevice = pDoc->GetOpenDevicePtr(true);
	if (pDevice)
	{
		pDoc->SetActiveChannel(NGIO_CHANNEL_ID_ANALOG3);
		pDoc->Set10voltRangeFlag(false);

		NGIOSetSensorChannelEnableMaskParams params;
		memset(&params, 0, sizeof(params));
		params.lsbyteLsword_EnableSensorChannels = NGIO_CHANNEL_MASK_ANALOG3;
		NGIO_Device_SendCmdAndGetResponse(pDevice, NGIO_CMD_ID_SET_SENSOR_CHANNEL_ENABLE_MASK, &params, sizeof(params), NULL, NULL, 
			NGIO_TIMEOUT_MS_DEFAULT);

		NGIOSetAnalogInputParams inputParams;
		inputParams.channel = NGIO_CHANNEL_ID_ANALOG3;
		inputParams.analogInput = NGIO_ANALOG_INPUT_5V_BUILTIN_12BIT_ADC;
		NGIO_Device_SendCmdAndGetResponse(pDevice, NGIO_CMD_ID_SET_ANALOG_INPUT, &inputParams, sizeof(inputParams), NULL, NULL, 
			NGIO_TIMEOUT_MS_DEFAULT);

		unsigned char sensorId = pDoc->UpdateSensorIdAndDDSRec(pDoc->GetActiveChannel());
		if (0 == sensorId)
		{
			//Force a default sensor.
			sensorId = 14; //5v voltage probe.
			NGIO_Device_DDSMem_SetSensorNumber(pDevice, pDoc->GetActiveChannel(), sensorId);
			NGIO_Device_DDSMem_SetLongName(pDevice, pDoc->GetActiveChannel(), "assumed");//Mark this as bogus.
			NGIO_Device_DDSMem_SetCalPage(pDevice, pDoc->GetActiveChannel(), 0, 0.0, 1.0, 0.0, "volts");//default calb.
		}
		pDoc->UpdateTitle();
		ClearGraph();
		UpdateUnits();
	
		if (NGIO_Device_GetProbeType(pDevice, pDoc->GetActiveChannel()) != kProbeTypeAnalog5V)
			MessageBox("Sensor plugged in is not compatible with selected mode.");
	}
}

void CMainFrame::OnAnalog4() 
{
	CNGIO_MeasureDoc *pDoc = (CNGIO_MeasureDoc *) GetActiveDocument();
	NGIO_DEVICE_HANDLE pDevice = NULL;
	if (pDoc)
		pDevice = pDoc->GetOpenDevicePtr(true);
	if (pDevice)
	{
		pDoc->SetActiveChannel(NGIO_CHANNEL_ID_ANALOG4);
		pDoc->Set10voltRangeFlag(false);

		NGIOSetSensorChannelEnableMaskParams params;
		memset(&params, 0, sizeof(params));
		params.lsbyteLsword_EnableSensorChannels = NGIO_CHANNEL_MASK_ANALOG4;
		NGIO_Device_SendCmdAndGetResponse(pDevice, NGIO_CMD_ID_SET_SENSOR_CHANNEL_ENABLE_MASK, &params, sizeof(params), NULL, NULL, 
			NGIO_TIMEOUT_MS_DEFAULT);

		NGIOSetAnalogInputParams inputParams;
		inputParams.channel = NGIO_CHANNEL_ID_ANALOG4;
		inputParams.analogInput = NGIO_ANALOG_INPUT_5V_BUILTIN_12BIT_ADC;
		NGIO_Device_SendCmdAndGetResponse(pDevice, NGIO_CMD_ID_SET_ANALOG_INPUT, &inputParams, sizeof(inputParams), NULL, NULL, 
			NGIO_TIMEOUT_MS_DEFAULT);

		unsigned char sensorId = pDoc->UpdateSensorIdAndDDSRec(pDoc->GetActiveChannel());
		if (0 == sensorId)
		{
			//Force a default sensor.
			sensorId = 14; //5v voltage probe.
			NGIO_Device_DDSMem_SetSensorNumber(pDevice, pDoc->GetActiveChannel(), sensorId);
			NGIO_Device_DDSMem_SetLongName(pDevice, pDoc->GetActiveChannel(), "assumed");//Mark this as bogus.
			NGIO_Device_DDSMem_SetCalPage(pDevice, pDoc->GetActiveChannel(), 0, 0.0, 1.0, 0.0, "volts");//default calb.
		}
		pDoc->UpdateTitle();
		ClearGraph();
		UpdateUnits();
	
		if (NGIO_Device_GetProbeType(pDevice, pDoc->GetActiveChannel()) != kProbeTypeAnalog5V)
			MessageBox("Sensor plugged in is not compatible with selected mode.");
	}
}

void CMainFrame::OnChannelsDigital1() 
{
	CNGIO_MeasureDoc *pDoc = (CNGIO_MeasureDoc *) GetActiveDocument();
	NGIO_DEVICE_HANDLE pDevice = NULL;
	if (pDoc)
		pDevice = pDoc->GetOpenDevicePtr(true);
	if (pDevice)
	{
		pDoc->SetActiveChannel(NGIO_CHANNEL_ID_DIGITAL1);
		pDoc->SetDigitalCollectionMode(NGIO_SAMPLING_MODE_APERIODIC_EDGE_DETECT);

		NGIOSetSensorChannelEnableMaskParams params;
		memset(&params, 0, sizeof(params));
		params.lsbyteLsword_EnableSensorChannels = NGIO_CHANNEL_MASK_DIGITAL1;
		NGIO_Device_SendCmdAndGetResponse(pDevice, NGIO_CMD_ID_SET_SENSOR_CHANNEL_ENABLE_MASK, &params, sizeof(params), NULL, NULL, 
			NGIO_TIMEOUT_MS_DEFAULT);

		NGIOSetSamplingModeParams samplingModeParams;
		samplingModeParams.channel = NGIO_CHANNEL_ID_DIGITAL1;
		samplingModeParams.samplingMode = NGIO_SAMPLING_MODE_APERIODIC_EDGE_DETECT;
		NGIO_Device_SendCmdAndGetResponse(pDevice, NGIO_CMD_ID_SET_SAMPLING_MODE, &samplingModeParams, sizeof(samplingModeParams), NULL, NULL, 
			NGIO_TIMEOUT_MS_DEFAULT);

		unsigned char sensorId = pDoc->UpdateSensorIdAndDDSRec(pDoc->GetActiveChannel());
		if (0 == sensorId)
		{
			//Force a default sensor.
			sensorId = 4; //photogate
			NGIO_Device_DDSMem_SetSensorNumber(pDevice, pDoc->GetActiveChannel(), sensorId);
			NGIO_Device_DDSMem_SetLongName(pDevice, pDoc->GetActiveChannel(), "assumed");//Mark this as bogus.
			NGIO_Device_DDSMem_SetCalPage(pDevice, pDoc->GetActiveChannel(), 0, 0.0, 1.0, 0.0, "units");//default calb.
		}
		pDoc->UpdateTitle();
		ClearGraph();
		UpdateUnits();
	
		if (NGIO_Device_GetProbeType(pDevice, pDoc->GetActiveChannel()) != kProbeTypePhotoGate)
			MessageBox("Sensor plugged in is not compatible with selected mode.");
	}
}

void CMainFrame::OnChannelsDigital2() 
{
	CNGIO_MeasureDoc *pDoc = (CNGIO_MeasureDoc *) GetActiveDocument();
	NGIO_DEVICE_HANDLE pDevice = NULL;
	if (pDoc)
		pDevice = pDoc->GetOpenDevicePtr(true);
	if (pDevice)
	{
		pDoc->SetActiveChannel(NGIO_CHANNEL_ID_DIGITAL2);
		pDoc->SetDigitalCollectionMode(NGIO_SAMPLING_MODE_APERIODIC_EDGE_DETECT);

		NGIOSetSensorChannelEnableMaskParams params;
		memset(&params, 0, sizeof(params));
		params.lsbyteLsword_EnableSensorChannels = NGIO_CHANNEL_MASK_DIGITAL2;
		NGIO_Device_SendCmdAndGetResponse(pDevice, NGIO_CMD_ID_SET_SENSOR_CHANNEL_ENABLE_MASK, &params, sizeof(params), NULL, NULL, 
			NGIO_TIMEOUT_MS_DEFAULT);

		NGIOSetSamplingModeParams samplingModeParams;
		samplingModeParams.channel = NGIO_CHANNEL_ID_DIGITAL1;
		samplingModeParams.samplingMode = NGIO_SAMPLING_MODE_APERIODIC_EDGE_DETECT;
		NGIO_Device_SendCmdAndGetResponse(pDevice, NGIO_CMD_ID_SET_SAMPLING_MODE, &samplingModeParams, sizeof(samplingModeParams), NULL, NULL, 
			NGIO_TIMEOUT_MS_DEFAULT);

		unsigned char sensorId = pDoc->UpdateSensorIdAndDDSRec(pDoc->GetActiveChannel());
		if (0 == sensorId)
		{
			//Force a default sensor.
			sensorId = 4; //photogate
			NGIO_Device_DDSMem_SetSensorNumber(pDevice, pDoc->GetActiveChannel(), sensorId);
			NGIO_Device_DDSMem_SetLongName(pDevice, pDoc->GetActiveChannel(), "assumed");//Mark this as bogus.
			NGIO_Device_DDSMem_SetCalPage(pDevice, pDoc->GetActiveChannel(), 0, 0.0, 1.0, 0.0, "units");//default calb.
		}
		pDoc->UpdateTitle();
		ClearGraph();
		UpdateUnits();
	
		if (NGIO_Device_GetProbeType(pDevice, pDoc->GetActiveChannel()) != kProbeTypePhotoGate)
			MessageBox("Sensor plugged in is not compatible with selected mode.");
	}
}

void CMainFrame::OnAnalog110v() 
{
	CNGIO_MeasureDoc *pDoc = (CNGIO_MeasureDoc *) GetActiveDocument();
	NGIO_DEVICE_HANDLE pDevice = NULL;
	if (pDoc)
		pDevice = pDoc->GetOpenDevicePtr(true);
	if (pDevice)
	{
		pDoc->SetActiveChannel(NGIO_CHANNEL_ID_ANALOG1);
		pDoc->Set10voltRangeFlag(true);

		NGIOSetSensorChannelEnableMaskParams params;
		memset(&params, 0, sizeof(params));
		params.lsbyteLsword_EnableSensorChannels = NGIO_CHANNEL_MASK_ANALOG1;
		NGIO_Device_SendCmdAndGetResponse(pDevice, NGIO_CMD_ID_SET_SENSOR_CHANNEL_ENABLE_MASK, &params, sizeof(params), NULL, NULL, 
			NGIO_TIMEOUT_MS_DEFAULT);

		NGIOSetAnalogInputParams inputParams;
		inputParams.channel = NGIO_CHANNEL_ID_ANALOG1;
		inputParams.analogInput = NGIO_ANALOG_INPUT_PM10V_BUILTIN_12BIT_ADC;
		NGIO_Device_SendCmdAndGetResponse(pDevice, NGIO_CMD_ID_SET_ANALOG_INPUT, &inputParams, sizeof(inputParams), NULL, NULL, 
			NGIO_TIMEOUT_MS_DEFAULT);

		unsigned char sensorId = pDoc->UpdateSensorIdAndDDSRec(pDoc->GetActiveChannel());
		if (0 == sensorId)
		{
			//Force a default sensor.
			sensorId = 2; //+/- 10 volt probe
			NGIO_Device_DDSMem_SetSensorNumber(pDevice, pDoc->GetActiveChannel(), sensorId);
			NGIO_Device_DDSMem_SetLongName(pDevice, pDoc->GetActiveChannel(), "assumed");//Mark this as bogus.
			NGIO_Device_DDSMem_SetCalPage(pDevice, pDoc->GetActiveChannel(), 0, 0.0, 1.0, 0.0, "volts");//default calb.
		}
		pDoc->UpdateTitle();
		ClearGraph();
		UpdateUnits();
	
		if (NGIO_Device_GetProbeType(pDevice, pDoc->GetActiveChannel()) != kProbeTypeAnalog10V)
			MessageBox("Sensor plugged in is not compatible with selected mode.");
	}
}

void CMainFrame::OnChannelsDigital1Motion() 
{
	CNGIO_MeasureDoc *pDoc = (CNGIO_MeasureDoc *) GetActiveDocument();
	NGIO_DEVICE_HANDLE pDevice = NULL;
	if (pDoc)
		pDevice = pDoc->GetOpenDevicePtr(true);
	if (pDevice)
	{
		pDoc->SetActiveChannel(NGIO_CHANNEL_ID_DIGITAL1);
		pDoc->SetDigitalCollectionMode(NGIO_SAMPLING_MODE_PERIODIC_MOTION_DETECT);

		NGIOSetSensorChannelEnableMaskParams params;
		memset(&params, 0, sizeof(params));
		params.lsbyteLsword_EnableSensorChannels = NGIO_CHANNEL_MASK_DIGITAL1;
		NGIO_Device_SendCmdAndGetResponse(pDevice, NGIO_CMD_ID_SET_SENSOR_CHANNEL_ENABLE_MASK, &params, sizeof(params), NULL, NULL, 
			NGIO_TIMEOUT_MS_DEFAULT);

		NGIOSetSamplingModeParams samplingModeParams;
		samplingModeParams.channel = NGIO_CHANNEL_ID_DIGITAL1;
		samplingModeParams.samplingMode = NGIO_SAMPLING_MODE_PERIODIC_MOTION_DETECT;
		NGIO_Device_SendCmdAndGetResponse(pDevice, NGIO_CMD_ID_SET_SAMPLING_MODE, &samplingModeParams, sizeof(samplingModeParams), NULL, NULL, 
			NGIO_TIMEOUT_MS_DEFAULT);

		unsigned char sensorId = pDoc->UpdateSensorIdAndDDSRec(pDoc->GetActiveChannel());
		if (0 == sensorId)
		{
			//Force a default sensor.
			sensorId = 2; //motion detector
			NGIO_Device_DDSMem_SetSensorNumber(pDevice, pDoc->GetActiveChannel(), sensorId);
			NGIO_Device_DDSMem_SetLongName(pDevice, pDoc->GetActiveChannel(), "assumed");//Mark this as bogus.
			NGIO_Device_DDSMem_SetCalPage(pDevice, pDoc->GetActiveChannel(), 0, 0.0, 1.0, 0.0, "units");//default calb.
		}
		pDoc->UpdateTitle();
		ClearGraph();
		UpdateUnits();
	
		if (NGIO_Device_GetProbeType(pDevice, pDoc->GetActiveChannel()) != kProbeTypeMD)
			MessageBox("Sensor plugged in is not compatible with selected mode.");
		else if (pDoc->GetMeasurementPeriodInSeconds() < 0.020)	//Make sure measurement period is ok for motion detectors.
		{
			NGIO_Device_SetMeasurementPeriod(pDevice, -1, 0.050, NGIO_TIMEOUT_MS_DEFAULT);
			pDoc->SetMeasurementPeriodInSeconds(0.050);
		}
	}
}

void CMainFrame::OnChannelsDigital2Motion() 
{
	CNGIO_MeasureDoc *pDoc = (CNGIO_MeasureDoc *) GetActiveDocument();
	NGIO_DEVICE_HANDLE pDevice = NULL;
	if (pDoc)
		pDevice = pDoc->GetOpenDevicePtr(true);
	if (pDevice)
	{
		pDoc->SetActiveChannel(NGIO_CHANNEL_ID_DIGITAL2);
		pDoc->SetDigitalCollectionMode(NGIO_SAMPLING_MODE_PERIODIC_MOTION_DETECT);

		NGIOSetSensorChannelEnableMaskParams params;
		memset(&params, 0, sizeof(params));
		params.lsbyteLsword_EnableSensorChannels = NGIO_CHANNEL_MASK_DIGITAL2;
		NGIO_Device_SendCmdAndGetResponse(pDevice, NGIO_CMD_ID_SET_SENSOR_CHANNEL_ENABLE_MASK, &params, sizeof(params), NULL, NULL, 
			NGIO_TIMEOUT_MS_DEFAULT);

		NGIOSetSamplingModeParams samplingModeParams;
		samplingModeParams.channel = NGIO_CHANNEL_ID_DIGITAL2;
		samplingModeParams.samplingMode = NGIO_SAMPLING_MODE_PERIODIC_MOTION_DETECT;
		NGIO_Device_SendCmdAndGetResponse(pDevice, NGIO_CMD_ID_SET_SAMPLING_MODE, &samplingModeParams, sizeof(samplingModeParams), NULL, NULL, 
			NGIO_TIMEOUT_MS_DEFAULT);
		unsigned char sensorId = pDoc->UpdateSensorIdAndDDSRec(pDoc->GetActiveChannel());
		if (0 == sensorId)
		{
			//Force a default sensor.
			sensorId = 2; //motion detector
			NGIO_Device_DDSMem_SetSensorNumber(pDevice, pDoc->GetActiveChannel(), sensorId);
			NGIO_Device_DDSMem_SetLongName(pDevice, pDoc->GetActiveChannel(), "assumed");//Mark this as bogus.
			NGIO_Device_DDSMem_SetCalPage(pDevice, pDoc->GetActiveChannel(), 0, 0.0, 1.0, 0.0, "units");//default calb.
		}
		pDoc->UpdateTitle();
		ClearGraph();
		UpdateUnits();
	
		if (NGIO_Device_GetProbeType(pDevice, pDoc->GetActiveChannel()) != kProbeTypeMD)
			MessageBox("Sensor plugged in is not compatible with selected mode.");
		else if (pDoc->GetMeasurementPeriodInSeconds() < 0.020)	//Make sure measurement period is ok for motion detectors.
		{
			NGIO_Device_SetMeasurementPeriod(pDevice, -1, 0.050, NGIO_TIMEOUT_MS_DEFAULT);
			pDoc->SetMeasurementPeriodInSeconds(0.050);
		}
	}
}

void CMainFrame::OnChannelsDigital1Rotary() 
{
	CNGIO_MeasureDoc *pDoc = (CNGIO_MeasureDoc *) GetActiveDocument();
	NGIO_DEVICE_HANDLE pDevice = NULL;
	if (pDoc)
		pDevice = pDoc->GetOpenDevicePtr(true);
	if (pDevice)
	{
		pDoc->SetActiveChannel(NGIO_CHANNEL_ID_DIGITAL1);
		pDoc->SetDigitalCollectionMode(NGIO_SAMPLING_MODE_PERIODIC_ROTATION_COUNTER);

		NGIOSetSensorChannelEnableMaskParams params;
		memset(&params, 0, sizeof(params));
		params.lsbyteLsword_EnableSensorChannels = NGIO_CHANNEL_MASK_DIGITAL1;
		NGIO_Device_SendCmdAndGetResponse(pDevice, NGIO_CMD_ID_SET_SENSOR_CHANNEL_ENABLE_MASK, &params, sizeof(params), NULL, NULL, 
			NGIO_TIMEOUT_MS_DEFAULT);

		NGIOSetSamplingModeParams samplingModeParams;
		samplingModeParams.channel = NGIO_CHANNEL_ID_DIGITAL1;
		samplingModeParams.samplingMode = NGIO_SAMPLING_MODE_PERIODIC_ROTATION_COUNTER;
		NGIO_Device_SendCmdAndGetResponse(pDevice, NGIO_CMD_ID_SET_SAMPLING_MODE, &samplingModeParams, sizeof(samplingModeParams), NULL, NULL, 
			NGIO_TIMEOUT_MS_DEFAULT);

		unsigned char sensorId = pDoc->UpdateSensorIdAndDDSRec(pDoc->GetActiveChannel());
		if (0 == sensorId)
		{
			//Force a default sensor.
			sensorId = 6; //rotary motion
			NGIO_Device_DDSMem_SetSensorNumber(pDevice, pDoc->GetActiveChannel(), sensorId);
			NGIO_Device_DDSMem_SetLongName(pDevice, pDoc->GetActiveChannel(), "assumed");//Mark this as bogus.
			NGIO_Device_DDSMem_SetCalPage(pDevice, pDoc->GetActiveChannel(), 0, 0.0, 1.0, 0.0, "units");//default calb.
		}
		pDoc->UpdateTitle();
		ClearGraph();
		UpdateUnits();
	
		if (NGIO_Device_GetProbeType(pDevice, pDoc->GetActiveChannel()) != kProbeTypeRotary)
			MessageBox("Sensor plugged in is not compatible with selected mode.");
	}
}

void CMainFrame::OnChannelsDigital1RotaryX4() 
{
	CNGIO_MeasureDoc *pDoc = (CNGIO_MeasureDoc *) GetActiveDocument();
	NGIO_DEVICE_HANDLE pDevice = NULL;
	if (pDoc)
		pDevice = pDoc->GetOpenDevicePtr(true);
	if (pDevice)
	{
		pDoc->SetActiveChannel(NGIO_CHANNEL_ID_DIGITAL1);
		pDoc->SetDigitalCollectionMode(NGIO_SAMPLING_MODE_PERIODIC_ROTATION_COUNTER_X4);

		NGIOSetSensorChannelEnableMaskParams params;
		memset(&params, 0, sizeof(params));
		params.lsbyteLsword_EnableSensorChannels = NGIO_CHANNEL_MASK_DIGITAL1;
		NGIO_Device_SendCmdAndGetResponse(pDevice, NGIO_CMD_ID_SET_SENSOR_CHANNEL_ENABLE_MASK, &params, sizeof(params), NULL, NULL, 
			NGIO_TIMEOUT_MS_DEFAULT);

		NGIOSetSamplingModeParams samplingModeParams;
		samplingModeParams.channel = NGIO_CHANNEL_ID_DIGITAL1;
		samplingModeParams.samplingMode = NGIO_SAMPLING_MODE_PERIODIC_ROTATION_COUNTER_X4;
		NGIO_Device_SendCmdAndGetResponse(pDevice, NGIO_CMD_ID_SET_SAMPLING_MODE, &samplingModeParams, sizeof(samplingModeParams), NULL, NULL, 
			NGIO_TIMEOUT_MS_DEFAULT);

		unsigned char sensorId = pDoc->UpdateSensorIdAndDDSRec(pDoc->GetActiveChannel());
		if (0 == sensorId)
		{
			//Force a default sensor.
			sensorId = 6; //rotary motion
			NGIO_Device_DDSMem_SetSensorNumber(pDevice, pDoc->GetActiveChannel(), sensorId);
			NGIO_Device_DDSMem_SetLongName(pDevice, pDoc->GetActiveChannel(), "assumed");//Mark this as bogus.
			NGIO_Device_DDSMem_SetCalPage(pDevice, pDoc->GetActiveChannel(), 0, 0.0, 1.0, 0.0, "units");//default calb.
		}
		pDoc->UpdateTitle();
		ClearGraph();
		UpdateUnits();
	
		if (NGIO_Device_GetProbeType(pDevice, pDoc->GetActiveChannel()) != kProbeTypeRotary)
			MessageBox("Sensor plugged in is not compatible with selected mode.");
	}
}

void CMainFrame::OnChannelsDigital1Radiation() 
{
	CNGIO_MeasureDoc *pDoc = (CNGIO_MeasureDoc *) GetActiveDocument();
	NGIO_DEVICE_HANDLE pDevice = NULL;
	if (pDoc)
		pDevice = pDoc->GetOpenDevicePtr(true);
	if (pDevice)
	{
		pDoc->SetActiveChannel(NGIO_CHANNEL_ID_DIGITAL1);
		pDoc->SetDigitalCollectionMode(NGIO_SAMPLING_MODE_PERIODIC_PULSE_COUNT);

		NGIOSetSensorChannelEnableMaskParams params;
		memset(&params, 0, sizeof(params));
		params.lsbyteLsword_EnableSensorChannels = NGIO_CHANNEL_MASK_DIGITAL1;
		NGIO_Device_SendCmdAndGetResponse(pDevice, NGIO_CMD_ID_SET_SENSOR_CHANNEL_ENABLE_MASK, &params, sizeof(params), NULL, NULL, 
			NGIO_TIMEOUT_MS_DEFAULT);

		NGIOSetSamplingModeParams samplingModeParams;
		samplingModeParams.channel = NGIO_CHANNEL_ID_DIGITAL1;
		samplingModeParams.samplingMode = NGIO_SAMPLING_MODE_PERIODIC_PULSE_COUNT;
		NGIO_Device_SendCmdAndGetResponse(pDevice, NGIO_CMD_ID_SET_SAMPLING_MODE, &samplingModeParams, sizeof(samplingModeParams), NULL, NULL, 
			NGIO_TIMEOUT_MS_DEFAULT);
	}
}

void CMainFrame::OnChannelsDigital2Rotary() 
{
	CNGIO_MeasureDoc *pDoc = (CNGIO_MeasureDoc *) GetActiveDocument();
	NGIO_DEVICE_HANDLE pDevice = NULL;
	if (pDoc)
		pDevice = pDoc->GetOpenDevicePtr(true);
	if (pDevice)
	{
		pDoc->SetActiveChannel(NGIO_CHANNEL_ID_DIGITAL2);
		pDoc->SetDigitalCollectionMode(NGIO_SAMPLING_MODE_PERIODIC_ROTATION_COUNTER);

		NGIOSetSensorChannelEnableMaskParams params;
		memset(&params, 0, sizeof(params));
		params.lsbyteLsword_EnableSensorChannels = NGIO_CHANNEL_MASK_DIGITAL2;
		NGIO_Device_SendCmdAndGetResponse(pDevice, NGIO_CMD_ID_SET_SENSOR_CHANNEL_ENABLE_MASK, &params, sizeof(params), NULL, NULL, 
			NGIO_TIMEOUT_MS_DEFAULT);

		NGIOSetSamplingModeParams samplingModeParams;
		samplingModeParams.channel = NGIO_CHANNEL_ID_DIGITAL2;
		samplingModeParams.samplingMode = NGIO_SAMPLING_MODE_PERIODIC_ROTATION_COUNTER;
		NGIO_Device_SendCmdAndGetResponse(pDevice, NGIO_CMD_ID_SET_SAMPLING_MODE, &samplingModeParams, sizeof(samplingModeParams), NULL, NULL, 
			NGIO_TIMEOUT_MS_DEFAULT);

		unsigned char sensorId = pDoc->UpdateSensorIdAndDDSRec(pDoc->GetActiveChannel());
		if (0 == sensorId)
		{
			//Force a default sensor.
			sensorId = 6; //rotary motion
			NGIO_Device_DDSMem_SetSensorNumber(pDevice, pDoc->GetActiveChannel(), sensorId);
			NGIO_Device_DDSMem_SetLongName(pDevice, pDoc->GetActiveChannel(), "assumed");//Mark this as bogus.
			NGIO_Device_DDSMem_SetCalPage(pDevice, pDoc->GetActiveChannel(), 0, 0.0, 1.0, 0.0, "units");//default calb.
		}
		pDoc->UpdateTitle();
		ClearGraph();
		UpdateUnits();
	
		if (NGIO_Device_GetProbeType(pDevice, pDoc->GetActiveChannel()) != kProbeTypeRotary)
			MessageBox("Sensor plugged in is not compatible with selected mode.");
	}
}

void CMainFrame::OnChannelsDigital2RotaryX4() 
{
	CNGIO_MeasureDoc *pDoc = (CNGIO_MeasureDoc *) GetActiveDocument();
	NGIO_DEVICE_HANDLE pDevice = NULL;
	if (pDoc)
		pDevice = pDoc->GetOpenDevicePtr(true);
	if (pDevice)
	{
		pDoc->SetActiveChannel(NGIO_CHANNEL_ID_DIGITAL2);
		pDoc->SetDigitalCollectionMode(NGIO_SAMPLING_MODE_PERIODIC_ROTATION_COUNTER_X4);

		NGIOSetSensorChannelEnableMaskParams params;
		memset(&params, 0, sizeof(params));
		params.lsbyteLsword_EnableSensorChannels = NGIO_CHANNEL_MASK_DIGITAL2;
		NGIO_Device_SendCmdAndGetResponse(pDevice, NGIO_CMD_ID_SET_SENSOR_CHANNEL_ENABLE_MASK, &params, sizeof(params), NULL, NULL, 
			NGIO_TIMEOUT_MS_DEFAULT);

		NGIOSetSamplingModeParams samplingModeParams;
		samplingModeParams.channel = NGIO_CHANNEL_ID_DIGITAL2;
		samplingModeParams.samplingMode = NGIO_SAMPLING_MODE_PERIODIC_ROTATION_COUNTER_X4;
		NGIO_Device_SendCmdAndGetResponse(pDevice, NGIO_CMD_ID_SET_SAMPLING_MODE, &samplingModeParams, sizeof(samplingModeParams), NULL, NULL, 
			NGIO_TIMEOUT_MS_DEFAULT);

		unsigned char sensorId = pDoc->UpdateSensorIdAndDDSRec(pDoc->GetActiveChannel());
		if (0 == sensorId)
		{
			//Force a default sensor.
			sensorId = 6; //rotary motion
			NGIO_Device_DDSMem_SetSensorNumber(pDevice, pDoc->GetActiveChannel(), sensorId);
			NGIO_Device_DDSMem_SetLongName(pDevice, pDoc->GetActiveChannel(), "assumed");//Mark this as bogus.
			NGIO_Device_DDSMem_SetCalPage(pDevice, pDoc->GetActiveChannel(), 0, 0.0, 1.0, 0.0, "units");//default calb.
		}
		pDoc->UpdateTitle();
		ClearGraph();
		UpdateUnits();
	
		if (NGIO_Device_GetProbeType(pDevice, pDoc->GetActiveChannel()) != kProbeTypeRotary)
			MessageBox("Sensor plugged in is not compatible with selected mode.");
	}
}

void CMainFrame::OnChannelsDigital2Radiation() 
{
	CNGIO_MeasureDoc *pDoc = (CNGIO_MeasureDoc *) GetActiveDocument();
	NGIO_DEVICE_HANDLE pDevice = NULL;
	if (pDoc)
		pDevice = pDoc->GetOpenDevicePtr(true);
	if (pDevice)
	{
		pDoc->SetActiveChannel(NGIO_CHANNEL_ID_DIGITAL2);
		pDoc->SetDigitalCollectionMode(NGIO_SAMPLING_MODE_PERIODIC_PULSE_COUNT);

		NGIOSetSensorChannelEnableMaskParams params;
		memset(&params, 0, sizeof(params));
		params.lsbyteLsword_EnableSensorChannels = NGIO_CHANNEL_MASK_DIGITAL2;
		NGIO_Device_SendCmdAndGetResponse(pDevice, NGIO_CMD_ID_SET_SENSOR_CHANNEL_ENABLE_MASK, &params, sizeof(params), NULL, NULL, 
			NGIO_TIMEOUT_MS_DEFAULT);

		NGIOSetSamplingModeParams samplingModeParams;
		samplingModeParams.channel = NGIO_CHANNEL_ID_DIGITAL2;
		samplingModeParams.samplingMode = NGIO_SAMPLING_MODE_PERIODIC_PULSE_COUNT;
		NGIO_Device_SendCmdAndGetResponse(pDevice, NGIO_CMD_ID_SET_SAMPLING_MODE, &samplingModeParams, sizeof(samplingModeParams), NULL, NULL, 
			NGIO_TIMEOUT_MS_DEFAULT);
	}
}

/*
void CMainFrame::OnUpdateChannelsAudioInternal(CCmdUI* pCmdUI) 
{
	BOOL bEnable = FALSE;
	CNGIO_MeasureDoc *pDoc = (CNGIO_MeasureDoc *) GetActiveDocument();
	if (pDoc)
		bEnable = (pDoc->GetOpenDevicePtr() != NULL);
	pCmdUI->Enable(bEnable);
}
*/
void CMainFrame::OnChannelsAudioInternal() 
{
	CNGIO_MeasureDoc *pDoc = (CNGIO_MeasureDoc *) GetActiveDocument();
	NGIO_DEVICE_HANDLE pDevice = NULL;
	if (pDoc)
		pDevice = pDoc->GetOpenDevicePtr();
	if (pDevice)
	{
		NGIO_DEVICE_HANDLE pAudioDevice = pDoc->GetAudioDevicePtr();
		if (!pAudioDevice)
			pDoc->OpenAudioDevice();
		pAudioDevice = pDoc->GetAudioDevicePtr();
		if (pAudioDevice)
		{
			NGIOSetSensorChannelEnableMaskParams params;
			memset(&params, 0, sizeof(params));
			params.lsbyteLsword_EnableSensorChannels = NGIO_CHANNEL_ID_MASK_AUDIO_INTERNAL;
			NGIO_Device_SendCmdAndGetResponse(pAudioDevice, NGIO_CMD_ID_SET_SENSOR_CHANNEL_ENABLE_MASK, &params, sizeof(params), NULL, NULL, 
				NGIO_TIMEOUT_MS_DEFAULT);

			pDoc->SetActiveChannel(NGIO_CHANNEL_ID_AUDIO_INTERNAL);
			pDoc->UpdateTitle();
			ClearGraph();
			UpdateUnits();
		}
	}
}

void CMainFrame::OnUpdateNgioLibVerbose(CCmdUI* pCmdUI) 
{
	BOOL bEnable = TRUE;
	pCmdUI->Enable(bEnable);
	NGIO_LIBRARY_HANDLE hNGIO_lib = ((CNGIO_MeasureApp *) AfxGetApp())->m_hNGIO_lib;
	if (hNGIO_lib)
	{
		gtype_int32 thresh = NGIO_TRACE_SEVERITY_HIGH;
		NGIO_Diags_GetDebugTraceThreshold(hNGIO_lib, &thresh);
		pCmdUI->SetCheck((thresh <= NGIO_TRACE_SEVERITY_LOW) ? 1 : 0);
	}
}

void CMainFrame::OnNgioLibVerbose() 
{
	NGIO_LIBRARY_HANDLE hNGIO_lib = ((CNGIO_MeasureApp *) AfxGetApp())->m_hNGIO_lib;
	if (hNGIO_lib)
	{
		gtype_int32 thresh = NGIO_TRACE_SEVERITY_HIGH;
		NGIO_Diags_GetDebugTraceThreshold(hNGIO_lib, &thresh);
		if (thresh <= NGIO_TRACE_SEVERITY_LOW)
			thresh = NGIO_TRACE_SEVERITY_HIGH;
		else
			thresh = NGIO_TRACE_SEVERITY_LOW;
		NGIO_Diags_SetDebugTraceThreshold(hNGIO_lib, thresh);
	}
}

void CMainFrame::UpdateUnits() 
{
	CNGIO_MeasureDoc *pDoc = (CNGIO_MeasureDoc *) GetActiveDocument();
	NGIO_DEVICE_HANDLE pDevice = NULL;
	if (pDoc)
		pDevice = pDoc->GetOpenDevicePtr();
	if (pDevice)
	{
		char units[20];
		GCalibrationPage calPage;
		unsigned char CalPageIndex;
		signed char channel = pDoc->GetActiveChannel();
		NGIO_Device_DDSMem_GetActiveCalPage(pDevice, channel, &CalPageIndex);
		NGIO_Device_DDSMem_GetCalPage(pDevice, channel, CalPageIndex,
			&calPage.CalibrationCoefficientA, &calPage.CalibrationCoefficientB, &calPage.CalibrationCoefficientC, 
			units, sizeof(units));
		m_wndSendBar.SetDlgItemText(IDC_UNITS, units);
	}
	else
		m_wndSendBar.SetDlgItemText(IDC_UNITS, "");
}

void CMainFrame::ClearGraph() 
{
	CNGIO_MeasureDoc *pDoc = (CNGIO_MeasureDoc *) GetActiveDocument();
	CNGIO_MeasureView *pView = (CNGIO_MeasureView *) GetActiveView();
	if (pDoc && pView)
	{
		if (pDoc->GetOpenDevicePtr())
			NGIO_Device_ClearIO(pDoc->GetOpenDevicePtr(), -1);//Flush any old measurements laying around.
		pDoc->ClearMeasurementCirbuf();
		pView->SetGraphHistory(1.0, -1.0);//This clears the history.
		pView->Invalidate();
	}
}

void CMainFrame::OnUpdateActionSetdisplaydepth(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnActionSetdisplaydepth() 
{
	CNGIO_MeasureDoc *pDoc = (CNGIO_MeasureDoc *) GetActiveDocument();
	CNGIO_MeasureView *pView = (CNGIO_MeasureView *) GetActiveView();
	if (pDoc && pView)
	{
		CDisplayDepthDlg dlg;
		dlg.m_displayDepth = pDoc->GetMaxNumMeasurementsInCirbuf();
		if (IDOK == dlg.DoModal())
		{
			if (dlg.m_displayDepth != pDoc->GetMaxNumMeasurementsInCirbuf())
			{
				pDoc->SetMaxNumMeasurementsInCirBuf(dlg.m_displayDepth);
				pView->SetGraphHistory(1.0, -1.0);//This clears the history.
				pView->Invalidate();
			}
		}
	}
}


