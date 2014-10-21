
// painter1.0View.h : interface of the Cpainter10View class
//

#pragma once
#include "atltypes.h"
#include "drawshape.h"
#include "SharedPara.h"
#include "MainFrm.h"
#include "DrawCircle.h"
#include "DrawRect.h"
#include "DrawSpecial.h"



class Cpainter10View : public CView
{
protected: // create from serialization only
	Cpainter10View();
	DECLARE_DYNCREATE(Cpainter10View)

// Attributes
public:
	Cpainter10Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~Cpainter10View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
private:
	bool m_IsDraw;
	CPoint m_StartPoint;
	CPoint m_EndPoint;
	HCURSOR m_hCross;
	CPen mPen;
	bool saveTag;
	CString saveFilePath;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	static int m_ShapeIndex;
	CDrawShape* m_pDrawing;
	int m_iMode;
	UINT m_nTimer;
	DWORD			m_dwNewSel;
	DWORD			m_dwOldSel;

	BOOL			m_bNeed;		//记录闪烁信息，是否需要恢复
public:
	void MyKillTimer();
	void FlashShape();

	afx_msg void OnButtonCircle();
	afx_msg void OnButtonClock();
	afx_msg void OnButtonFlower();
	afx_msg void OnButtonLine();
	afx_msg void OnButtonOrange();
	afx_msg void OnButtonRect();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnButtonRollback();
	afx_msg void OnButtonSave();
	afx_msg void OnButtonBird();
	afx_msg void OnButtonLamp();
	afx_msg void OnButtonSnow();
	afx_msg void OnDrawspeedFast();
	afx_msg void OnDrawspeedMedium();
	afx_msg void OnDrawspeedSlow();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#ifndef _DEBUG  // debug version in painter1.0View.cpp
inline Cpainter10Doc* Cpainter10View::GetDocument() const
   { return reinterpret_cast<Cpainter10Doc*>(m_pDocument); }
#endif

