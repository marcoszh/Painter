#include "StdAfx.h"
#include "DrawCircle.h"


CDrawCircle::CDrawCircle(void)
{
	m_ptCenter.x = m_ptCenter.y = 0;
	m_ptTemp.x = m_ptTemp.y = 0;
	m_nRadius = 0;
	m_bStart = FALSE;
}

void CDrawCircle::Draw(CDC* pDC,bool isFast)
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
	if(!isFast) 
	{
		rcDraw.left = m_ptCenter.x - m_nRadius;
		rcDraw.top = m_ptCenter.y  - m_nRadius;
		rcDraw.right = m_ptCenter.x + m_nRadius;
		rcDraw.bottom = m_ptCenter.y + m_nRadius;
		pDC->Ellipse(&rcDraw);
	}
	else
	{
		CPoint mPoint;
		pDC->MoveTo(m_ptCenter.x,m_ptCenter.y - m_nRadius);
		mPoint.x=m_ptCenter.x;
		int offset=0;
		int x,y;
		while(offset<=m_nRadius)
		{
			offset+=5;
			
			x=m_ptCenter.x+offset;
			y=m_ptCenter.y-(int)sqrt((float)(m_nRadius*m_nRadius-offset*offset));
			pDC->LineTo(x,y);
			//TRACE("0");
			Sleep(8*CSharedPara::m_Speed);
		}
		pDC->LineTo(m_ptCenter.x+m_nRadius,m_ptCenter.y);
		offset=m_nRadius;
		while(offset>=0)
		{
			offset-=5;
			
			x=m_ptCenter.x+offset;
			y=m_ptCenter.y+(int)sqrt((float)(m_nRadius*m_nRadius-offset*offset));
			pDC->LineTo(x,y);
			//TRACE("0");
			Sleep(8*CSharedPara::m_Speed);
		}
		pDC->LineTo(m_ptCenter.x,m_ptCenter.y+m_nRadius);
		offset=0;
		while(offset<=m_nRadius)
		{
			offset+=5;
			
			x=m_ptCenter.x-offset;
			y=m_ptCenter.y+(int)sqrt((float)(m_nRadius*m_nRadius-offset*offset));
			pDC->LineTo(x,y);
			//TRACE("0");
			Sleep(8*CSharedPara::m_Speed);
		}
		pDC->LineTo(m_ptCenter.x-m_nRadius,m_ptCenter.y);
		offset=m_nRadius;
		while(offset>=0)
		{
			offset-=5;
			
			x=m_ptCenter.x-offset;
			y=m_ptCenter.y-(int)sqrt((float)(m_nRadius*m_nRadius-offset*offset));
			pDC->LineTo(x,y);
			//TRACE("0");
			Sleep(8*CSharedPara::m_Speed);
		}
		pDC->LineTo(m_ptCenter.x,m_ptCenter.y-m_nRadius);
		
		//pDC->PolyBezier(pointArray,4);
	}
	pDC->SelectObject(pOldBrush);
}

void CDrawCircle::OnMouseMove(CDC* pDC, UINT nFlags, CPoint point)
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

void CDrawCircle::OnLButtonDown(CDC* pDC, UINT nFlags, CPoint point)
{
	m_ptCenter = point;
	m_ptTemp = point;
	m_bStart = TRUE;
}

BOOL CDrawCircle::OnLButtonUp(CDC* pDC, UINT nFlags, CPoint point)
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
			pDC->SetROP2(R2_COPYPEN);
			Draw(pDC,true);
			pDC->SetROP2(iDrawMode);
			pDC->SelectObject(pOldBrush);
		}
		m_bStart = FALSE;
	}
	return (!rcDraw.IsRectEmpty());
}

LONG CDrawCircle::CalcRadius(CPoint& ptFirst, CPoint& ptSecond)
{
	double	dRadius	= 0.0;
	dRadius = pow((float)ptSecond.x-ptFirst.x, 2) + pow((float)ptSecond.y-ptFirst.y, 2);
	dRadius = sqrt(dRadius);
	return (LONG)dRadius;
}

BOOL CDrawCircle::PtInShape(CPoint point)
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


CDrawCircle::~CDrawCircle(void)
{
}
