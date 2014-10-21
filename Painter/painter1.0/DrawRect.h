#pragma once
#include "drawshape.h"
class CDrawRect :
	public CDrawShape
{
private:
	CPoint		m_ptFirst;
	CPoint		m_ptSecond;
	CPoint		m_ptTemp;
	BOOL		m_bStart;
public:
	CDrawRect(void);
	virtual ~CDrawRect();
	virtual void Draw(CDC* pDC, bool isFast);
	virtual void OnMouseMove(CDC* pDC, UINT nFlags, CPoint point);
	virtual void OnLButtonDown(CDC* pDC, UINT nFlags, CPoint point);
	virtual BOOL OnLButtonUp(CDC* pDC, UINT nFlags, CPoint point);

	virtual BOOL PtInShape(CPoint point);
};

