#include "StdAfx.h"
#include "DrawLine.h"


CDrawLine::CDrawLine(void)
{
	m_ptFirst.x = m_ptFirst.y = 0;
	m_ptSecond.x = m_ptSecond.y = 0;
	m_ptTemp.x = m_ptTemp.y = 0;
	m_bStart = FALSE;
}


CDrawLine::~CDrawLine(void)
{
}

void CDrawLine::Draw(CDC* pDC, bool isFast)
{
	if(!isFast)
	{
		pDC->MoveTo(m_ptFirst);
		pDC->LineTo(m_ptSecond);
	}
	else
	{
		//CPoint* cpp;
		pDC->MoveTo(m_ptFirst);
		int xGap=(m_ptSecond.x - m_ptFirst.x)/30;
		int yGap=(m_ptSecond.y - m_ptFirst.y)/30;
		for(int i=0;i<30;i++)
		{
			//cpp = new CPoint(m_ptFirst.x+i*xGap,m_ptFirst.y+i*yGap);
			pDC->LineTo(CPoint(m_ptFirst.x+i*xGap,m_ptFirst.y+i*yGap));
			Sleep(8*CSharedPara::m_Speed);
		}
	}
}

void CDrawLine::OnMouseMove(CDC* pDC, UINT nFlags, CPoint point)
{
	int		iDrawMode	= 0;
	if(m_bStart)
	{
		if(pDC)
		{
			iDrawMode = pDC->SetROP2(R2_NOTXORPEN);
			pDC->MoveTo(m_ptFirst);
			pDC->LineTo(m_ptTemp);
			m_ptTemp = point;
			pDC->MoveTo(m_ptFirst);
			pDC->LineTo(m_ptTemp);
			pDC->SetROP2(iDrawMode);
		}
	}
	TRACE("MM");
}

void CDrawLine::OnLButtonDown(CDC* pDC, UINT nFlags, CPoint point)
{
	m_ptFirst = point;
	m_ptTemp = point;
	m_bStart = TRUE;
	TRACE("LBD");
}

BOOL CDrawLine::OnLButtonUp(CDC* pDC, UINT nFlags, CPoint point)
{
	int		iDrawMode	= 0;
	if(m_bStart)
	{
		if(pDC)
		{
			iDrawMode = pDC->SetROP2(R2_NOTXORPEN);
			pDC->MoveTo(m_ptFirst);
			pDC->LineTo(m_ptTemp);
			m_ptSecond = point;
			m_ptTemp = point;
			pDC->SetROP2(R2_COPYPEN);
			Draw(pDC,true);
			pDC->SetROP2(iDrawMode);
		}
		m_bStart = FALSE;
	}
	TRACE("LBU");
	return (m_ptFirst!=m_ptSecond);
}

BOOL CDrawLine::PtInShape(CPoint point)
{
	CRect	rcDraw;
	rcDraw.left = MIN(m_ptFirst.x, m_ptSecond.x);
	rcDraw.top = MIN(m_ptFirst.y, m_ptSecond.y);
	rcDraw.right = MAX(m_ptFirst.x, m_ptSecond.x);
	rcDraw.bottom = MAX(m_ptFirst.y, m_ptSecond.y);
	return rcDraw.PtInRect(point);
}