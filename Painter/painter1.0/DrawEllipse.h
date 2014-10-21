#pragma once
#include "drawshape.h"
#include <math.h>
class CDrawEllipse :
	public CDrawShape
{
private:
	CPoint		m_ptFirst;
	CPoint      m_ptCenter;
	int         m_nRadius;
	CPoint		m_ptSecond;
	CPoint		m_ptTemp;
	BOOL		m_bStart;
public:
	CDrawEllipse(void);
    virtual ~CDrawEllipse();
	virtual void Draw(CDC* pDC,bool isFalse);
	virtual void OnMouseMove(CDC* pDC, UINT nFlags, CPoint point);
	virtual void OnLButtonDown(CDC* pDC, UINT nFlags, CPoint point);
	virtual BOOL OnLButtonUp(CDC* pDC, UINT nFlags, CPoint point);
	LONG CalcRadius(CPoint& ptFirst, CPoint& ptSecond);
	virtual BOOL PtInShape(CPoint point);

};

