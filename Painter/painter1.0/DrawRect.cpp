#include "StdAfx.h"
#include "DrawRect.h"


CDrawRect::CDrawRect(void)
{
	m_ptFirst.x = m_ptFirst.y = 0;
	m_ptSecond.x = m_ptSecond.y = 0;
	m_ptTemp.x = m_ptTemp.y = 0;
	m_bStart = FALSE;
}

void CDrawRect::Draw(CDC* pDC,bool isFast)
{
	CRect	rcDraw;
	CBrush*	pBrush		= NULL;
	CBrush*	pOldBrush	= NULL;
	pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	pOldBrush = pDC->SelectObject(pBrush);
	rcDraw.left = m_ptFirst.x;
	rcDraw.top = m_ptFirst.y;
	rcDraw.right = m_ptSecond.x;		
	rcDraw.bottom = m_ptSecond.y;
	if(!isFast)
	{
		pDC->Rectangle(&rcDraw);
	}
	else
	{
		int x,y,offset=0;
		x=rcDraw.left;
		y=rcDraw.top;
		pDC->MoveTo(x,y);
		while(offset<=(rcDraw.right-rcDraw.left))
		{
			offset+=5;
			x=rcDraw.left+offset;
			pDC->LineTo(x,y);
			Sleep(8*CSharedPara::m_Speed);
		}
		x=rcDraw.right;
		pDC->MoveTo(x,y);
		offset=0;
		while(offset<=(rcDraw.bottom-rcDraw.top))
		{
			offset+=5;
			y=rcDraw.top+offset;
			pDC->LineTo(x,y);
			Sleep(8*CSharedPara::m_Speed);
		}
		y=rcDraw.bottom;
		pDC->MoveTo(x,y);
		offset=rcDraw.right-rcDraw.left;
		while(offset>=0)
		{
			offset-=5;
			x=rcDraw.left+offset;
			pDC->LineTo(x,y);
			Sleep(8*CSharedPara::m_Speed);
		}
		x=rcDraw.left;
		pDC->MoveTo(x,y);
		offset=0;
		while(offset<=(rcDraw.bottom-rcDraw.top))
		{
			offset+=5;
			y=rcDraw.bottom-offset;
			pDC->LineTo(x,y);
			Sleep(8*CSharedPara::m_Speed);
		}
		y=rcDraw.top;
		pDC->MoveTo(x,y);
	}
	pDC->SelectObject(pOldBrush);
}

void CDrawRect::OnMouseMove(CDC* pDC, UINT nFlags, CPoint point)
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
			pDC->Rectangle(&rcDraw);
			m_ptTemp = point;
			rcDraw.right = m_ptTemp.x;
			rcDraw.bottom = m_ptTemp.y;
			pDC->Rectangle(&rcDraw);
			pDC->SetROP2(iDrawMode);
			pDC->SelectObject(pOldBrush);
		}
	}
}

void CDrawRect::OnLButtonDown(CDC* pDC, UINT nFlags, CPoint point)
{
	m_ptFirst = point;
	m_ptTemp = point;
	m_bStart = TRUE;
}

BOOL CDrawRect::OnLButtonUp(CDC* pDC, UINT nFlags, CPoint point)
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
			pDC->Rectangle(&rcDraw);
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
			Draw(pDC,true);
			pDC->SetROP2(iDrawMode);
			pDC->SelectObject(pOldBrush);
		}
		m_bStart = FALSE;
	}
	return (!rcDraw.IsRectEmpty());
}

BOOL CDrawRect::PtInShape(CPoint point)
{
	CRect	rcDraw;
	rcDraw.left = MIN(m_ptFirst.x, m_ptSecond.x);
	rcDraw.top = MIN(m_ptFirst.y, m_ptSecond.y);
	rcDraw.right = MAX(m_ptFirst.x, m_ptSecond.x);
	rcDraw.bottom = MAX(m_ptFirst.y, m_ptSecond.y);
	return rcDraw.PtInRect(point);
}


CDrawRect::~CDrawRect(void)
{
}
