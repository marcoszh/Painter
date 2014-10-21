#include "StdAfx.h"
#include "DrawSpecial.h"
using namespace std;

CDrawSpecial::CDrawSpecial(void)
{
	m_ptFirst.x = m_ptFirst.y = 0;
	m_ptSecond.x = m_ptSecond.y = 0;
	m_ptTemp.x = m_ptTemp.y = 0;
	m_bStart = FALSE;
	shapeIndex=CSharedPara::m_ShapeIndex;
}



CDrawSpecial::~CDrawSpecial(void)
{
}

/*void CDrawSpecial::setShape(int index)
{
	shapeIndex=index;
}*/

void CDrawSpecial::Draw(CDC* pDC,bool isFast)
{
	sprintf(fileName,"%x",shapeIndex);
	fileName[1]='.';
	fileName[2]='d';
	fileName[3]='a';
	fileName[4]='t';
	fileName[5]='\0';

	CRect	rcDraw;
	CBrush*	pBrush		= NULL;
	CBrush*	pOldBrush	= NULL;
	pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	pOldBrush = pDC->SelectObject(pBrush);

	TRACE("DRAWSPECIAL");
	ifstream fp(fileName,ios::in);
	if(!fp)
	{
		printf("file open fail!");
		exit(0);
	}
	string str;
	char ch[10];
	int x=0,y=0;
	bool nextStroke=false;
	bool firstPoint = true;
	pDC->MoveTo(m_ptSecond);
	while(!fp.eof())
	{
		getline(fp,str);
		istringstream istr(str);

		istr>>ch;
		x = atoi(ch)+m_ptSecond.x;

		istr>>ch;
		y = atoi(ch)+m_ptSecond.y;

		if(nextStroke)
		{
			pDC->MoveTo(x,y);
			nextStroke=false;
			continue;
		}
		if((x==-1+m_ptSecond.x)&&(y==-1+m_ptSecond.y))
		{
			nextStroke=true;
			continue;
		}
		if(firstPoint)
		{
			firstPoint=false;
			pDC->MoveTo(x,y);
			continue;
			
		}

		rcDraw.left=x-1;
		rcDraw.right=x+1;
		rcDraw.top=y-1;
		rcDraw.bottom=y+1;

		pDC->Rectangle(&rcDraw);
		//pDC->LineTo(x,y);
		
		if(isFast)
			Sleep(CSharedPara::m_Speed);
	}
	pDC->SelectObject(pOldBrush);
	fp.close();
}


void CDrawSpecial::OnMouseMove(CDC* pDC, UINT nFlags, CPoint point)
{
	
}

BOOL CDrawSpecial::PtInShape(CPoint point)
{
	CRect	rcDraw;
	rcDraw.left = MIN(m_ptFirst.x, m_ptSecond.x);
	rcDraw.top = MIN(m_ptFirst.y, m_ptSecond.y);
	rcDraw.right = MAX(m_ptFirst.x, m_ptSecond.x);
	rcDraw.bottom = MAX(m_ptFirst.y, m_ptSecond.y);
	return rcDraw.PtInRect(point);
}


void CDrawSpecial::OnLButtonDown(CDC* pDC, UINT nFlags, CPoint point)
{
	m_ptFirst = point;
	m_ptTemp = point;
	m_bStart = TRUE;
}

BOOL CDrawSpecial::OnLButtonUp(CDC* pDC, UINT nFlags, CPoint point)
{
	int		iDrawMode	= 0;
	if(m_bStart)
	{
		if(pDC)
		{
			
			iDrawMode = pDC->SetROP2(R2_NOTXORPEN);
			//pDC->MoveTo(m_ptFirst);
			//pDC->LineTo(m_ptTemp);
			m_ptSecond = point;
			m_ptTemp = point;
			pDC->SetROP2(R2_COPYPEN);
			Draw(pDC,true);
			pDC->SetROP2(iDrawMode);
		}
		m_bStart = FALSE;
	}
	return true;
}