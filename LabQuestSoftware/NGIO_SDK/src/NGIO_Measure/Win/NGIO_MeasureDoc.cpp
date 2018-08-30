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
// NGIO_MeasureDoc.cpp : implementation of the CNGIO_MeasureDoc class
//

#include "stdafx.h"
#include "NGIO_Measure.h"
#include "MainFrm.h"

#include "NGIO_MeasureDoc.h"
#include "NGIO_MeasureView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNGIO_MeasureDoc

IMPLEMENT_DYNCREATE(CNGIO_MeasureDoc, CDocument)

BEGIN_MESSAGE_MAP(CNGIO_MeasureDoc, CDocument)
END_MESSAGE_MAP()


// CNGIO_MeasureDoc construction/destruction

CNGIO_MeasureDoc::CNGIO_MeasureDoc()
{
	m_pDevice = NULL;
	m_pAudioDevice = NULL;
	m_pMeasCirBuf = new GCircularBuffer(DEFAULT_MAX_NUM_MEASUREMENTS_IN_CIRBUF*sizeof(double));
	m_pTimeCirBuf = new GCircularBuffer(DEFAULT_MAX_NUM_MEASUREMENTS_IN_CIRBUF*sizeof(double));

	ClearMeasurementCirbuf();
	m_measPeriodInSeconds = 0.5;
	m_activeChannel = NGIO_CHANNEL_ID_ANALOG1;
	m_b10voltRange = false;
	m_digitalCollectionMode = NGIO_SAMPLING_MODE_APERIODIC_EDGE_DETECT;
}

CNGIO_MeasureDoc::~CNGIO_MeasureDoc()
{
	if (m_pDevice)
		CloseDevice();

	if (m_pTimeCirBuf)
		delete m_pTimeCirBuf;
	m_pTimeCirBuf = NULL;

	if (m_pMeasCirBuf)
		delete m_pMeasCirBuf;
	m_pMeasCirBuf = NULL;
}

BOOL CNGIO_MeasureDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	CMainFrame *pFrame = (CMainFrame *) AfxGetMainWnd();
	if (pFrame)
		pFrame->ResetDevice();

	return TRUE;
}




// CNGIO_MeasureDoc serialization

void CNGIO_MeasureDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CNGIO_MeasureDoc diagnostics

#ifdef _DEBUG
void CNGIO_MeasureDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CNGIO_MeasureDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


NGIO_DEVICE_HANDLE CNGIO_MeasureDoc::OpenDevice(LPCSTR pDeviceName)
{
	NGIO_LIBRARY_HANDLE hNGIO_lib = ((CNGIO_MeasureApp *) AfxGetApp())->m_hNGIO_lib;
	if (m_pDevice)
		NGIO_Device_Close(m_pDevice);
	m_pDevice = NULL;
	m_pDevice = NGIO_Device_Open(hNGIO_lib, pDeviceName, 0);
	if (m_pDevice)
	{
		double period = 1.0;
		gtype_uint32 deviceType = 0;
		NGIO_GetDeviceTypeFromDeviceName(pDeviceName, &deviceType);
		if ((NGIO_DEVTYPE_LABQUEST == deviceType) || (NGIO_DEVTYPE_LABQUEST2 == deviceType))
			NGIO_Device_AcquireExclusiveOwnership(m_pDevice, NGIO_GRAB_DAQ_TIMEOUT);
		else if (NGIO_DEVTYPE_LABQUEST_MINI == deviceType)
		{
			NGIOSetLedStateParams ledParams;
			ledParams.brightness = NGIO_LED_BRIGHTNESS_MAX;
			ledParams.color = NGIO_LED_COLOR_GREEN;
			NGIO_Device_SendCmdAndGetResponse(m_pDevice, NGIO_CMD_ID_SET_LED_STATE, &ledParams, sizeof(ledParams), 
				NULL, NULL, NGIO_TIMEOUT_MS_DEFAULT);
		}
		NGIO_Device_SetMeasurementPeriod(m_pDevice, -1, 0.005, NGIO_TIMEOUT_MS_DEFAULT);
		NGIO_Device_GetMeasurementPeriod(m_pDevice, -1, &period, NGIO_TIMEOUT_MS_DEFAULT);
		SetMeasurementPeriodInSeconds(period);
	}
	return m_pDevice;
}

void CNGIO_MeasureDoc::CloseDevice()
{
	if (m_pAudioDevice)
		NGIO_Device_Close(m_pAudioDevice);
	m_pAudioDevice = NULL;

	if (m_pDevice)
		NGIO_Device_Close(m_pDevice);
	m_pDevice = NULL;

	ClearMeasurementCirbuf();

	CMainFrame *pFrame = (CMainFrame *) AfxGetMainWnd();
	if (pFrame)
	{
		pFrame->m_wndSendBar.SetDlgItemText(IDC_MEAS, "");
		pFrame->m_wndSendBar.SetDlgItemText(IDC_UNITS, "");
		pFrame->ClearGraph();
	}

	UpdateTitle();
}

NGIO_DEVICE_HANDLE CNGIO_MeasureDoc::OpenAudioDevice()
{
	NGIO_DEVICE_HANDLE hDevice = m_pAudioDevice;
	if (!m_pAudioDevice)
	{
		if (m_pDevice)
		{
			gtype_uint32 sig, numAudios, status;
			NGIO_LIBRARY_HANDLE hNGIO_lib = ((CNGIO_MeasureApp *) AfxGetApp())->m_hNGIO_lib;
			NGIO_DEVICE_LIST_HANDLE hAudioList;

			NGIO_SearchForDevices(hNGIO_lib, NGIO_DEVTYPE_LABPRO2_AUDIO, NGIO_COMM_TRANSPORT_USB, NULL, &sig);//spam
			hAudioList = NGIO_OpenDeviceListSnapshot(hNGIO_lib, NGIO_DEVTYPE_LABPRO2_AUDIO, &numAudios, &sig);
			if (hAudioList)
			{
				char newDeviceName[NGIO_MAX_SIZE_DEVICE_NAME];
				if (0 == NGIO_DeviceListSnapshot_GetNthEntry(hAudioList, 0, newDeviceName, NGIO_MAX_SIZE_DEVICE_NAME, &status))
				{
					gtype_real64 period = 1.0;
					NGIO_Device_GetMeasurementPeriod(m_pDevice, -1, &period, NGIO_TIMEOUT_MS_DEFAULT);
					m_pAudioDevice = NGIO_Device_Open(hNGIO_lib, newDeviceName, 1);
					if (m_pAudioDevice)
					{
						NGIO_Device_SetMeasurementPeriod(m_pAudioDevice, -1, period, NGIO_TIMEOUT_MS_DEFAULT);
						NGIO_Device_GetMeasurementPeriod(m_pAudioDevice, -1, &period, NGIO_TIMEOUT_MS_DEFAULT);
						SetMeasurementPeriodInSeconds(period);
					}
				}

				NGIO_CloseDeviceListSnapshot(hAudioList);
			}
		}
	}

	return m_pAudioDevice;
}

void CNGIO_MeasureDoc::CloseAudioDevice()
{
	if (m_pAudioDevice)
		NGIO_Device_Close(m_pAudioDevice);
	m_pAudioDevice = NULL;
}


void CNGIO_MeasureDoc::AddMeasurementToCirbuf(double measurement, double time)
{
	m_pMeasCirBuf->AddBytes((unsigned char *) &measurement, sizeof(measurement));
	m_pTimeCirBuf->AddBytes((unsigned char *) &time, sizeof(time));
}

void CNGIO_MeasureDoc::ClearMeasurementCirbuf()
{
	m_pMeasCirBuf->Clear();
	m_pTimeCirBuf->Clear();
}

bool CNGIO_MeasureDoc::GetNthMeasurementInCirbuf(int N, double *pMeasurement, double *pTime) //(N == 0) => first measurement.
{
	bool bSuccess = false;
	double measurement = 0.0;
	double time = 0.0;
	int count = m_pMeasCirBuf->CopyBytes((unsigned char *) &measurement, N*sizeof(double), sizeof(double));
	if (sizeof(double) == count)
	{
		count = m_pTimeCirBuf->CopyBytes((unsigned char *) &time, N*sizeof(double), sizeof(double));
		if (sizeof(double) == count)
		{
			bSuccess = true;
			*pMeasurement = measurement;
			*pTime = time;
		}
	}

	return bSuccess;
}

unsigned char CNGIO_MeasureDoc::UpdateSensorIdAndDDSRec(unsigned char channel)
{
	unsigned char id = 0;
	NGIO_DEVICE_HANDLE hDevice = GetOpenDevicePtr();
	if (hDevice)
	{
		GSensorDDSRec rec;
		gtype_uint32 sig;

		//Force defaults unless we are able to auto id the sensor.
		memset(&rec, 0, sizeof(rec));

		rec.CalibrationEquation = kEquationType_Linear;
		rec.CalibrationPage[0].CalibrationCoefficientA = 0.0;
		rec.CalibrationPage[0].CalibrationCoefficientB = 1.0;
		rec.CalibrationPage[0].CalibrationCoefficientC = 0.0;

		if (hDevice != GetAudioDevicePtr())
		{
			NGIO_Device_DDSMem_SetRecord(hDevice, channel, &rec);
			int status = NGIO_Device_DDSMem_GetSensorNumber(hDevice, channel, &id, 1, &sig, NGIO_TIMEOUT_MS_DEFAULT);
			if (0 == status)
			{
				if (id >= kSensorIdNumber_FirstSmartSensor)
				{
					status = NGIO_Device_DDSMem_ReadRecord(hDevice, channel, 0, NGIO_TIMEOUT_MS_READ_DDSMEMBLOCK);
					if (0 != status)
						NGIO_Device_DDSMem_SetSensorNumber(hDevice, channel, 0);//Clear id to flag error.
				}
			}
		}
		else
		{
			strcpy(rec.CalibrationPage[0].Units, "volts");
			id = 74;//Sound level meter.
			rec.SensorNumber = id;
			NGIO_Device_DDSMem_SetRecord(hDevice, channel, &rec);
		}
	}

	return id;
}

void CNGIO_MeasureDoc::UpdateTitle()
{
	NGIO_DEVICE_HANDLE hDevice = GetOpenDevicePtr();
	if (!hDevice)
		SetTitle("");
	else
	{
		cppsstream ss;
		unsigned char sensorId = 0;
		char tmpstring[100];
		unsigned char chan = GetActiveChannel();
		gtype_uint32 sig;

		NGIO_Device_DDSMem_GetSensorNumber(hDevice, chan, &sensorId, 0, &sig, NGIO_TIMEOUT_MS_DEFAULT);

		ss << "Sensor in chan ";
		if (hDevice != GetAudioDevicePtr())
		{
			switch (chan)
			{
				case NGIO_CHANNEL_ID_ANALOG1:
					ss << "ANALOG1";
					break;
				case NGIO_CHANNEL_ID_ANALOG2:
					ss << "ANALOG2";
					break;
				case NGIO_CHANNEL_ID_ANALOG3:
					ss << "ANALOG3";
					break;
				case NGIO_CHANNEL_ID_ANALOG4:
					ss << "ANALOG4";
					break;
				case NGIO_CHANNEL_ID_DIGITAL1:
					ss << "DIGITAL1";
					break;
				case NGIO_CHANNEL_ID_DIGITAL2:
					ss << "DIGITAL2";
					break;
				default:
					ss << "?";
					break;
			}
		}
		else
			ss << "AUDIO_INTERNAL";

		ss << " has id = " << (int) sensorId;

		NGIO_Device_DDSMem_GetLongName(hDevice, chan, tmpstring, sizeof(tmpstring));
		if (lstrlen(tmpstring) != 0)
		{
			cppstring sensorName = tmpstring;
			ss << " ( " << sensorName << " ) ";
		}

		ss << ", probe type is ";
		switch (NGIO_Device_GetProbeType(hDevice, chan))
		{
			case kProbeTypeAnalog5V:
				ss << "ANALOG5V";
				break;
			case kProbeTypeAnalog10V:
				ss << "ANALOG10V";
				break;
			case kProbeTypeMD:
				ss << "Motion Detector";
				break;
			case kProbeTypePhotoGate:
				ss << "Photogate";
				break;
			case kProbeTypeDigitalCount:
				ss << "Digital Count";
				break;
			case kProbeTypeRotary:
				ss << "Rotary";
				break;
			case kProbeTypeLabquestAudio:
				ss << "LabQuest Audio";
				break;
			default:
				ss << "?";
				break;
		}
		SetTitle(ss.str().c_str());
	}
}

void CNGIO_MeasureDoc::SetMaxNumMeasurementsInCirBuf(int maxNum)
{
	if (m_pMeasCirBuf)
	{
		if (m_pMeasCirBuf->MaxNumBytesAvailable() != (maxNum*sizeof(double)))
		{
			delete m_pMeasCirBuf;
			m_pMeasCirBuf = NULL;
		}
	}
	if (m_pTimeCirBuf)
	{
		if (m_pTimeCirBuf->MaxNumBytesAvailable() != (maxNum*sizeof(double)))
		{
			delete m_pTimeCirBuf;
			m_pTimeCirBuf = NULL;
		}
	}
	if (!m_pMeasCirBuf)
		m_pMeasCirBuf = new GCircularBuffer(maxNum*sizeof(double));
	if (!m_pTimeCirBuf)
		m_pTimeCirBuf = new GCircularBuffer(maxNum*sizeof(double));
}
// CNGIO_MeasureDoc commands
