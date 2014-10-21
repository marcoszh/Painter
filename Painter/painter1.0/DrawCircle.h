#pragma once
#include "drawshape.h"
#include <math.h>
class CDrawCircle :
	public CDrawShape
{
private:
	CPoint		m_ptCenter;
	LONG		m_nRadius;
	CPoint		m_ptTemp;
	BOOL		m_bStart;
public:
	CDrawCircle(void);
	virtual ~CDrawCircle();
	virtual void Draw(CDC* pDC,bool isFast);
	virtual void OnMouseMove(CDC* pDC, UINT nFlags, CPoint point);
	virtual void OnLButtonDown(CDC* pDC, UINT nFlags, CPoint point);
	virtual BOOL OnLButtonUp(CDC* pDC, UINT nFlags, CPoint point);

	virtual BOOL PtInShape(CPoint point);
private:
	LONG CalcRadius(CPoint& ptFirst, CPoint& ptSecond);
};

