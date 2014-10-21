
// painter1.0Doc.h : interface of the Cpainter10Doc class
//


#pragma once
#include "afxcoll.h"
#include "DrawShape.h"


class Cpainter10Doc : public CDocument
{
protected: // create from serialization only
	Cpainter10Doc();
	DECLARE_DYNCREATE(Cpainter10Doc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~Cpainter10Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	CObArray m_ObArray;
	void AddShape(CDrawShape* pDrawShape);
//	int m_ShapeIndex;
};
