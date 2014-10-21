#pragma once
#include "afx.h"
#include "SharedPara.h"
class CDrawShape :
	public CObject
{
public:
	CTime	m_tmCreate;
	int		m_iID;
	CDrawShape(void);
	virtual ~CDrawShape(void);
public:
	virtual void Draw(CDC* pDC, bool isFast) {};
	virtual void OnMouseMove(CDC* pDC, UINT nFlags, CPoint point)
	{
	}
	virtual void OnLButtonDown(CDC* pDC, UINT nFlags, CPoint point)
	{
	}
	virtual BOOL OnLButtonUp(CDC* pDC, UINT nFlags, CPoint point)
	{
		return FALSE;
	}
	virtual BOOL PtInShape(CPoint point)
	{
		return FALSE;
	}
	LONG MIN(LONG num1, LONG num2);
	LONG MAX(LONG num1, LONG num2);
};

