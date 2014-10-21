#pragma once
#include "drawshape.h"
#include "SharedPara.h"
#include <iostream>
#include <fstream> 
#include <sstream>
class CDrawSpecial :
	public CDrawShape
{
public:
	CDrawSpecial(void);
	virtual ~CDrawSpecial(void);
	//void setShape(int index);
private:
	CPoint		m_ptFirst;
	CPoint		m_ptSecond;
	CPoint		m_ptTemp;
	BOOL		m_bStart;
	char        fileName[20]; 
	int         shapeIndex;
    virtual void Draw(CDC* pDC, bool isFast);
	virtual void OnMouseMove(CDC* pDC, UINT nFlags, CPoint point);
	virtual void OnLButtonDown(CDC* pDC, UINT nFlags, CPoint point);
	virtual BOOL OnLButtonUp(CDC* pDC, UINT nFlags, CPoint point);
	virtual BOOL PtInShape(CPoint point);

};

