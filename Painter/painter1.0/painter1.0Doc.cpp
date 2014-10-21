
// painter1.0Doc.cpp : implementation of the Cpainter10Doc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "painter1.0.h"
#endif

#include "painter1.0Doc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Cpainter10Doc

IMPLEMENT_DYNCREATE(Cpainter10Doc, CDocument)

BEGIN_MESSAGE_MAP(Cpainter10Doc, CDocument)
END_MESSAGE_MAP()


// Cpainter10Doc construction/destruction

Cpainter10Doc::Cpainter10Doc()
{
	// TODO: add one-time construction code here

}

Cpainter10Doc::~Cpainter10Doc()
{
}

void Cpainter10Doc::AddShape(CDrawShape* pDrawShape)
{
	if(pDrawShape)
	{
		m_ObArray.Add((CObject*)pDrawShape);
	}
}

BOOL Cpainter10Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// Cpainter10Doc serialization

void Cpainter10Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


#ifdef SHARED_HANDLERS

// Support for thumbnails
void Cpainter10Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void Cpainter10Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void Cpainter10Doc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// Cpainter10Doc diagnostics

#ifdef _DEBUG
void Cpainter10Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void Cpainter10Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Cpainter10Doc commands
