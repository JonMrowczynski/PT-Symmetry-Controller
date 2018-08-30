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
// NGIO_MeasureDoc.h : interface of the CNGIO_MeasureDoc class
//
#pragma once

#include "GCircularBuffer.h"
#define DEFAULT_MAX_NUM_MEASUREMENTS_IN_CIRBUF 1001

class CNGIO_MeasureDoc : public CDocument
{
protected: // create from serialization only
	CNGIO_MeasureDoc();
	DECLARE_DYNCREATE(CNGIO_MeasureDoc)

// Attributes
public:
	NGIO_DEVICE_HANDLE GetOpenDevicePtr(bool bDAQOnly = false) 
	{
		if (!bDAQOnly)
		{
			if (m_pAudioDevice)
				return m_pAudioDevice; 
			else
				return m_pDevice;
		}
		else
		{
			if (m_pAudioDevice)
				NGIO_Device_Close(m_pAudioDevice);
			m_pAudioDevice = NULL;
			return m_pDevice;
		}
	}
	NGIO_DEVICE_HANDLE GetParentDevicePtr() { return m_pDevice; }
	NGIO_DEVICE_HANDLE GetAudioDevicePtr() { return m_pAudioDevice; }
	unsigned char GetActiveChannel() { return m_activeChannel; }
	void SetActiveChannel(unsigned char channel) { m_activeChannel = channel; }

	void Set10voltRangeFlag(bool flag)
	{
		m_b10voltRange = flag;
	}
	bool Get10voltRangeFlag()
	{
		return m_b10voltRange;
	}

	void SetDigitalCollectionMode(int mode)
	{
		m_digitalCollectionMode = mode;
	}
	int GetDigitalCollectionMode()
	{
		return m_digitalCollectionMode;
	}

	unsigned char UpdateSensorIdAndDDSRec(unsigned char channel);//Returns sensorId after querying device.
	void UpdateTitle();

// Operations
public:
	NGIO_DEVICE_HANDLE OpenDevice(LPCSTR pDeviceName);
	void CloseDevice();
	NGIO_DEVICE_HANDLE OpenAudioDevice();
	void CloseAudioDevice();

	void AddMeasurementToCirbuf(double measurement, double time);
	int GetNumMeasurementsInCirbuf() { return m_pMeasCirBuf->NumBytesAvailable()/sizeof(double); }
	void ClearMeasurementCirbuf();
	void SetMaxNumMeasurementsInCirBuf(int maxNum);
	int GetMaxNumMeasurementsInCirbuf() { return m_pMeasCirBuf->MaxNumBytesAvailable()/sizeof(double); }
	bool GetNthMeasurementInCirbuf(int N, double *pMeasurement, double *pTime); //(N == 0) => first measurement.
	void SetMeasurementPeriodInSeconds(double period) { m_measPeriodInSeconds = period; }
	double GetMeasurementPeriodInSeconds() { return m_measPeriodInSeconds; }

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CNGIO_MeasureDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

	NGIO_DEVICE_HANDLE m_pDevice;
	NGIO_DEVICE_HANDLE m_pAudioDevice;
//	double m_measurements_cirbuf[MAX_NUM_MEASUREMENTS_IN_CIRBUF];
//	int m_numMeasurementsInCirbuf;
//	int m_firstCirbufMeasurementIndex;
	double m_measPeriodInSeconds;
	GCircularBuffer *m_pMeasCirBuf;
	GCircularBuffer *m_pTimeCirBuf;
	unsigned char m_activeChannel;
	bool m_b10voltRange;
	int m_digitalCollectionMode;
};


