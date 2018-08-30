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
// NGIO_MeasureView.cpp : implementation of the CNGIO_MeasureView class
//

#include "stdafx.h"
#include "NGIO_Measure.h"

#include "NGIO_MeasureDoc.h"
#include "NGIO_MeasureView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNGIO_MeasureView

IMPLEMENT_DYNCREATE(CNGIO_MeasureView, CView)

BEGIN_MESSAGE_MAP(CNGIO_MeasureView, CView)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CNGIO_MeasureView construction/destruction

CNGIO_MeasureView::CNGIO_MeasureView()
{
	// TODO: add construction code here

}

CNGIO_MeasureView::~CNGIO_MeasureView()
{
}

BOOL CNGIO_MeasureView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CNGIO_MeasureView drawing

void CNGIO_MeasureView::OnDraw(CDC* pDC)
{
	CNGIO_MeasureDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	char tmpstring[100];

	CRect clientRect;
	GetClientRect(&clientRect);

	NGIO_DEVICE_HANDLE hDevice = pDoc->GetOpenDevicePtr();
	if (hDevice && (pDoc->GetNumMeasurementsInCirbuf() > 0))
	{
		CRect graphRect = clientRect;
		int i;
		double meas_x_range, meas_x_min, meas_x_max;
		double meas_y_range, meas_y_min, meas_y_max, testy, testx;
		int x, y, deltaX, deltaY, old_x, old_y;

		//Calculate what portion of the client area will hold the graph.
		if ((graphRect.Width() > 200) && (graphRect.Height() > 200))
		{
			graphRect.top += 25;
			graphRect.bottom -= 50;
			graphRect.left += 100;
			graphRect.right -= 25;
		}

		//Calculate full graph time range.
		int numMeasurements = pDoc->GetNumMeasurementsInCirbuf();
		meas_x_min = 0.0;
		meas_x_max = 1.0;
		if (numMeasurements > 0)
		{
			pDoc->GetNthMeasurementInCirbuf(0, &testy, &meas_x_min);
			pDoc->GetNthMeasurementInCirbuf(numMeasurements - 1, &testy, &meas_x_max);
		}
		meas_x_range = meas_x_max - meas_x_min;

		//Calculate full graph y range.
		pDoc->GetNthMeasurementInCirbuf(0, &meas_y_min, &testx);
		meas_y_max = meas_y_min;
		for (i = 1; i < numMeasurements; i++)
		{
			pDoc->GetNthMeasurementInCirbuf(i, &testy, &testx);
			if (testy < meas_y_min)
				meas_y_min = testy;
			if (testy > meas_y_max)
				meas_y_max = testy;
		}
		meas_y_range = meas_y_max - meas_y_min;

		//Make sure that meas_y_range corresponds to a voltage delta of at least 0.1 volts.
		if (meas_y_range < 0.1)
		{
			meas_y_range = 0.1;
			meas_y_max = meas_y_min + meas_y_range;
		}
/*
		double y_0_volts, y_dot1_volts, deltay_dot1_volts;
		y_0_volts = GoIO_Sensor_CalibrateData(hDevice, 0.0);
		y_dot1_volts = GoIO_Sensor_CalibrateData(hDevice, 0.1);
		deltay_dot1_volts = y_dot1_volts - y_0_volts;
		if (deltay_dot1_volts < 0.0)
			deltay_dot1_volts = -deltay_dot1_volts;
		if (deltay_dot1_volts > meas_y_range)
		{
			meas_y_range = deltay_dot1_volts;
			meas_y_max = meas_y_min + meas_y_range;
		}
*/
		double history_y_min;
		double history_y_max;
		GetGraphHistory(history_y_min, history_y_max);
		if (history_y_max > history_y_min)
		{
			if (history_y_max > meas_y_max)
				meas_y_max = history_y_max;
			if (history_y_min < meas_y_min)
				meas_y_min = history_y_min;
			meas_y_range = meas_y_max - meas_y_min;
		}
		SetGraphHistory(meas_y_min, meas_y_max);

		//Do some drawing now.
		pDC->FillRect(&clientRect, CBrush::FromHandle((HBRUSH) ::GetStockObject(WHITE_BRUSH)));

		CPen *pOldPen = pDC->SelectObject(CPen::FromHandle((HPEN) ::GetStockObject(BLACK_PEN)));

		pDC->MoveTo(graphRect.left, clientRect.top);
		pDC->LineTo(graphRect.left, clientRect.bottom);
		pDC->MoveTo(graphRect.right, clientRect.top);
		pDC->LineTo(graphRect.right, clientRect.bottom);

		CPen bluePen;
		bluePen.CreatePen(PS_SOLID, 0, RGB(0, 0, 200));
		pDC->SelectObject(&bluePen);

		pDC->MoveTo(clientRect.left, graphRect.bottom);
		pDC->LineTo(clientRect.right, graphRect.bottom);
		pDC->MoveTo(clientRect.left, graphRect.top);
		pDC->LineTo(clientRect.right, graphRect.top);

		cppsstream ss;
		ss << fixed << showpoint << setprecision(3) << meas_x_min << " secs";
		cppstring str = ss.str();
		lstrcpy(tmpstring, str.c_str());
		x = graphRect.left + 10;
		y = graphRect.bottom + 10;
		pDC->TextOut(x, y, tmpstring, lstrlen(tmpstring));

		ss.str("");
		ss << fixed << showpoint << setprecision(3) << meas_x_max << " secs";
		str = ss.str();
		lstrcpy(tmpstring, str.c_str());
		CRect textRect;
		pDC->DrawText(tmpstring, lstrlen(tmpstring), &textRect, DT_CALCRECT);
		x = graphRect.right - textRect.Width() - 10;
		y = graphRect.bottom + 10;
		pDC->TextOut(x, y, tmpstring, lstrlen(tmpstring));

		char units[40];
		unsigned char calPageIndex;
		GCalibrationPage calPage;
		signed char channel = pDoc->GetActiveChannel();
		NGIO_Device_DDSMem_GetActiveCalPage(hDevice, channel, &calPageIndex);
		NGIO_Device_DDSMem_GetCalPage(hDevice, channel, calPageIndex,
			&calPage.CalibrationCoefficientA, &calPage.CalibrationCoefficientB, &calPage.CalibrationCoefficientC, 
			units, sizeof(units));

		pDC->SetTextColor(RGB(0, 0, 200));
		pDC->SetBkMode(TRANSPARENT);

		ss.str("");
		ss << fixed << showpoint << setprecision(3) << meas_y_min << " " << cppstring(units);
//		ss << fixed << showpoint << setprecision(3) << meas_y_min;
		str = ss.str();
		lstrcpy(tmpstring, str.c_str());
		pDC->DrawText(tmpstring, lstrlen(tmpstring), &textRect, DT_CALCRECT);
		x = clientRect.left + 10;
		y = graphRect.bottom - textRect.Height() - 10;
		pDC->TextOut(x, y, tmpstring, lstrlen(tmpstring));

		ss.str("");
		ss << fixed << showpoint << setprecision(3) << meas_y_max << " " << cppstring(units);
//		ss << fixed << showpoint << setprecision(3) << meas_y_max;
		str = ss.str();
		lstrcpy(tmpstring, str.c_str());
		x = clientRect.left + 10;
		y = graphRect.top + 10;
		pDC->TextOut(x, y, tmpstring, lstrlen(tmpstring));


		CPen redPen;
		redPen.CreatePen(PS_SOLID, 3, RGB(200, 0, 0));
		pDC->SelectObject(&redPen);

		double meas_x_frac, meas_y_frac;

		for (i = 0; i < numMeasurements; i++)
		{
			pDoc->GetNthMeasurementInCirbuf(i, &meas_y_frac, &meas_x_frac);
			meas_y_frac = (meas_y_frac - meas_y_min)/meas_y_range;
			deltaY = (int) floor(meas_y_frac*graphRect.Height() + 0.5);
			meas_x_frac = (meas_x_frac - meas_x_min)/meas_x_range;
			deltaX = (int) floor(meas_x_frac*graphRect.Width() + 0.5);
			x = graphRect.left + deltaX;
			y = graphRect.bottom - deltaY;
			if (0 == i)
			{
				pDC->MoveTo(x, y);
				pDC->LineTo(x, y);
			}
			else if ((x != old_x) || (y != old_y))
				pDC->LineTo(x, y);

			old_x = x;
			old_y = y;
		}

		if (pOldPen)
			pDC->SelectObject(pOldPen);
	}
	else
		pDC->FillRect(&clientRect, CBrush::FromHandle((HBRUSH) ::GetStockObject(WHITE_BRUSH)));
}


// CNGIO_MeasureView diagnostics

#ifdef _DEBUG
void CNGIO_MeasureView::AssertValid() const
{
	CView::AssertValid();
}

void CNGIO_MeasureView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CNGIO_MeasureDoc* CNGIO_MeasureView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNGIO_MeasureDoc)));
	return (CNGIO_MeasureDoc*)m_pDocument;
}
#endif //_DEBUG


BOOL CNGIO_MeasureView::OnEraseBkgnd(CDC* pDC) 
{
	return FALSE;
}
// CNGIO_MeasureView message handlers
