#include "StdAfx.h"
#include "DrawEllipse.h"


CDrawEllipse::CDrawEllipse(void)
{
	m_ptFirst.x = m_ptFirst.y = 0;
	m_ptSecond.x = m_ptSecond.y = 0;
	m_ptTemp.x = m_ptTemp.y = 0;
	m_bStart = FALSE;
}

void CDrawEllipse::Draw(CDC* pDC,bool isFast)
{
	CRect	rcDraw;
	CBrush*	pBrush		= NULL;
	CBrush*	pOldBrush	= NULL;
	pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	pOldBrush = pDC->SelectObject(pBrush);
	rcDraw.left = m_ptCenter.x - m_nRadius;
	rcDraw.top = m_ptCenter.y  - m_nRadius;
	rcDraw.right = m_ptCenter.x + m_nRadius;
	rcDraw.bottom = m_ptCenter.y + m_nRadius;
	pDC->Ellipse(&rcDraw);
	pDC->SelectObject(pOldBrush);
}

void CDrawEllipse::OnMouseMove(CDC* pDC, UINT nFlags, CPoint point)
{
	int		iDrawMode	= 0;
	CRect	rcDraw;
	CBrush*	pBrush		= NULL;
	CBrush*	pOldBrush	= NULL;
	if(m_bStart)
	{
		if(pDC)
		{
			pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
			pOldBrush = pDC->SelectObject(pBrush);
			iDrawMode = pDC->SetROP2(R2_NOTXORPEN);
			rcDraw.left = m_ptCenter.x - m_nRadius;
			rcDraw.top = m_ptCenter.y  - m_nRadius;
			rcDraw.right = m_ptCenter.x + m_nRadius;
			rcDraw.bottom = m_ptCenter.y + m_nRadius;
			pDC->Ellipse(&rcDraw);
			m_ptTemp = point;
			m_nRadius = CalcRadius(m_ptCenter, m_ptTemp);
			rcDraw.left = m_ptCenter.x - m_nRadius;
			rcDraw.top = m_ptCenter.y  - m_nRadius;
			rcDraw.right = m_ptCenter.x + m_nRadius;
			rcDraw.bottom = m_ptCenter.y + m_nRadius;
			pDC->Ellipse(&rcDraw);
			pDC->SetROP2(iDrawMode);
			pDC->SelectObject(pOldBrush);
		}
	}
}

BOOL CDrawEllipse::PtInShape(CPoint point)
{
	CRgn	rgn;
	CRect	rcDraw;
	BOOL	m_bRetVal	= FALSE;
	rcDraw.left = m_ptCenter.x - m_nRadius;
	rcDraw.top = m_ptCenter.y  - m_nRadius;
	rcDraw.right = m_ptCenter.x + m_nRadius;
	rcDraw.bottom = m_ptCenter.y + m_nRadius;
	rgn.CreateEllipticRgnIndirect(&rcDraw);
	m_bRetVal = rgn.PtInRegion(point);
	rgn.DeleteObject();
	return m_bRetVal;
}

void CDrawEllipse::OnLButtonDown(CDC* pDC, UINT nFlags, CPoint point)
{
	m_ptFirst = point;
	m_ptTemp = point;
	m_bStart = TRUE;
}

LONG CDrawEllipse::CalcRadius(CPoint& ptFirst, CPoint& ptSecond)
{
	double	dRadius	= 0.0;
	dRadius = pow((float)ptSecond.x-ptFirst.x, 2) + pow((float)ptSecond.y-ptFirst.y, 2);
	dRadius = sqrt(dRadius);
	return (LONG)dRadius;
}

BOOL CDrawEllipse::OnLButtonUp(CDC* pDC, UINT nFlags, CPoint point)
{
	int		iDrawMode	= 0;
	CRect	rcDraw;
	CBrush*	pBrush		= NULL;
	CBrush*	pOldBrush	= NULL;
	if(m_bStart)
	{
		if(pDC)
		{
			pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
			pOldBrush = pDC->SelectObject(pBrush);
			iDrawMode = pDC->SetROP2(R2_NOTXORPEN);
			rcDraw.left = m_ptFirst.x;
			rcDraw.top = m_ptFirst.y;
			rcDraw.right = m_ptTemp.x;
			rcDraw.bottom = m_ptTemp.y;
			pDC->Ellipse(&rcDraw);
			m_ptSecond = point;
			m_ptTemp = point;
			rcDraw.left = MIN(m_ptFirst.x, m_ptSecond.x);
			rcDraw.top = MIN(m_ptFirst.y, m_ptSecond.y);
			rcDraw.right = MAX(m_ptFirst.x, m_ptSecond.x);
			rcDraw.bottom = MAX(m_ptFirst.y, m_ptSecond.y);
			m_ptFirst.x = rcDraw.left;
			m_ptFirst.y = rcDraw.top;
			m_ptSecond.x = rcDraw.right;
			m_ptSecond.y = rcDraw.bottom;
			pDC->SetROP2(R2_COPYPEN);
			Draw(pDC,false);
			pDC->SetROP2(iDrawMode);
			pDC->SelectObject(pOldBrush);
		}
		m_bStart = FALSE;
	}
	return (!rcDraw.IsRectEmpty());
}

CDrawEllipse::~CDrawEllipse(void)
{
}
