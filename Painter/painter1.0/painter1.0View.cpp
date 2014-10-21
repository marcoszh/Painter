
// painter1.0View.cpp : implementation of the Cpainter10View class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "painter1.0.h"
#endif

#include "painter1.0Doc.h"
#include "DrawShape.h"
#include "DrawLine.h"
#include "painter1.0View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int CSharedPara::m_ShapeIndex=0;//特殊图形的索引
int CSharedPara::m_Speed=2;//默认的动态绘制速度
// Cpainter10View

IMPLEMENT_DYNCREATE(Cpainter10View, CView)

	BEGIN_MESSAGE_MAP(Cpainter10View, CView)
		// Standard printing commands
		ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_PREVIEW, &Cpainter10View::OnFilePrintPreview)
		ON_WM_CONTEXTMENU()
		ON_WM_RBUTTONUP()
		ON_WM_LBUTTONDOWN()
		ON_WM_LBUTTONUP()
		ON_COMMAND(ID_BUTTON_CIRCLE, &Cpainter10View::OnButtonCircle)
		ON_COMMAND(ID_BUTTON_CLOCK, &Cpainter10View::OnButtonClock)
		ON_COMMAND(ID_BUTTON_FLOWER, &Cpainter10View::OnButtonFlower)
		ON_COMMAND(ID_BUTTON_LINE, &Cpainter10View::OnButtonLine)
		ON_COMMAND(ID_BUTTON_ORANGE, &Cpainter10View::OnButtonOrange)
		ON_COMMAND(ID_BUTTON_RECT, &Cpainter10View::OnButtonRect)
		ON_WM_MOUSEMOVE()
		ON_COMMAND(ID_BUTTON_ROLLBACK, &Cpainter10View::OnButtonRollback)
		ON_COMMAND(ID_BUTTON_SAVE, &Cpainter10View::OnButtonSave)
		ON_COMMAND(ID_BUTTON_BIRD, &Cpainter10View::OnButtonBird)
		ON_COMMAND(ID_BUTTON_LAMP, &Cpainter10View::OnButtonLamp)
		ON_COMMAND(ID_BUTTON_SNOW, &Cpainter10View::OnButtonSnow)
		ON_COMMAND(ID_DRAWSPEED_FAST, &Cpainter10View::OnDrawspeedFast)
		ON_COMMAND(ID_DRAWSPEED_MEDIUM, &Cpainter10View::OnDrawspeedMedium)
		ON_COMMAND(ID_DRAWSPEED_SLOW, &Cpainter10View::OnDrawspeedSlow)
		ON_WM_TIMER()
	END_MESSAGE_MAP()

	// Cpainter10View construction/destruction

	Cpainter10View::Cpainter10View()
	{
		// TODO: add construction code here
		m_pDrawing	= new CDrawLine();
		m_iMode		= -1;//当前绘制的图形
		m_nTimer	= 0;//没用上
		m_dwNewSel	= 0;
		m_dwOldSel	= 0;
		saveTag=false;//是否需要保存到图形数组
		m_bNeed		= FALSE;
		mPen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
		m_hCross=AfxGetApp()->LoadStandardCursor(IDC_CROSS);//获得十字鼠标光标
	}

	Cpainter10View::~Cpainter10View()
	{
		if(m_pDrawing)
		{
			delete m_pDrawing;
			m_pDrawing = NULL;
		}
	}

	BOOL Cpainter10View::PreCreateWindow(CREATESTRUCT& cs)
	{
		// TODO: Modify the Window class or styles here by modifying
		//  the CREATESTRUCT cs

		return CView::PreCreateWindow(cs);
	}

	// Cpainter10View drawing

	void Cpainter10View::OnDraw(CDC* pDC)//绘制用户区
	{
		Cpainter10Doc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		int arraySize = pDoc->m_ObArray.GetSize();//将已保存的对象取出绘制，因为客户区在窗口变化时会被销毁，每次销毁后需要我们将之前的内容还原
		if (!pDoc)
			return;
		//m_pDrawing=new CDrawLine();

		for(int i=0; i<arraySize; i++)//将图形恢复
		{
			((CDrawShape*)pDoc->m_ObArray.GetAt(i))->Draw(pDC,false);
		}

		// TODO: add draw code for native data here;
	}


	// Cpainter10View printing


	void Cpainter10View::OnFilePrintPreview()
	{
#ifndef SHARED_HANDLERS
		AFXPrintPreview(this);
#endif
	}

	BOOL Cpainter10View::OnPreparePrinting(CPrintInfo* pInfo)
	{
		// default preparation
		return DoPreparePrinting(pInfo);
	}

	void Cpainter10View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
	{
		// TODO: add extra initialization before printing
	}

	void Cpainter10View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
	{
		// TODO: add cleanup after printing
	}

	void Cpainter10View::OnRButtonUp(UINT /* nFlags */, CPoint point)
	{
		ClientToScreen(&point);
		OnContextMenu(this, point);
	}

	void Cpainter10View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
	{
#ifndef SHARED_HANDLERS
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
	}


	// Cpainter10View diagnostics

#ifdef _DEBUG
	void Cpainter10View::AssertValid() const
	{
		CView::AssertValid();
	}

	void Cpainter10View::Dump(CDumpContext& dc) const
	{
		CView::Dump(dc);
	}

	Cpainter10Doc* Cpainter10View::GetDocument() const // non-debug version is inline
	{
		ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cpainter10Doc)));
		return (Cpainter10Doc*)m_pDocument;
	}
#endif //_DEBUG


	// Cpainter10View message handlers


	void Cpainter10View::OnLButtonDown(UINT nFlags, CPoint point)//鼠标左键按下
	{
		// TODO: Add your message handler code here and/or call default
		//MyKillTimer();
		CDC*	pDC	= NULL;
		if(m_pDrawing)
		{
			pDC = GetDC();
			OnPrepareDC(pDC);
			pDC->DPtoLP(&point);
			m_pDrawing->OnLButtonDown(pDC, nFlags, point);//所有的图形绘制类继承自CDrawShape，可以通过统一接口调用函数
			ReleaseDC(pDC);
		}
		//Cpainter10View::OnLButtonDown(nFlags, point);
	}


	void Cpainter10View::OnLButtonUp(UINT nFlags, CPoint point)//鼠标左键抬起
	{
		// TODO: Add your message handler code here and/or call default
		Cpainter10Doc* pDoc = NULL;
		CDC*			pDC		= NULL;
		int				iIndex	= 0;
		BOOL			bSave	= FALSE;
		if(m_pDrawing)//若当前是画东西状态，将当前新画的对象保存
		{
			pDC = GetDC();
			OnPrepareDC(pDC);
			pDC->DPtoLP(&point);
			bSave = m_pDrawing->OnLButtonUp(pDC, nFlags, point);
			ReleaseDC(pDC);

			if(bSave)
			{
				//存储对象
				m_pDrawing->m_tmCreate = CTime::GetCurrentTime();
				pDoc = GetDocument();
				
				pDoc->AddShape(m_pDrawing);
				TRACE("SAVEIT");
			}
			else
			{
				//释放对象
				delete m_pDrawing;
			}
			m_pDrawing = NULL;

			//m_pDrawing = new CDrawLine();
			//创建新对象
			switch(m_iMode)
			{
			case 0:
				m_pDrawing = new CDrawLine();
				break;
			case 1:
				m_pDrawing = new CDrawCircle();
				break;
			case 2:
				m_pDrawing = new CDrawRect();
				break;
			case 3:
				CSharedPara::m_ShapeIndex=3;
				m_pDrawing = new CDrawSpecial();
				
				break;
			case 4://特殊图形统一处理，所以在CSharedPara类里加入一个变量用来代表画的哪个特殊图形
				CSharedPara::m_ShapeIndex=4;
				m_pDrawing = new CDrawLine();
				
				break;
			case 5:
				CSharedPara::m_ShapeIndex=5;
				m_pDrawing = new CDrawLine();				
				break;
			case 6:
				CSharedPara::m_ShapeIndex=6;
				m_pDrawing = new CDrawLine();
				
				break;
			case 7:
				CSharedPara::m_ShapeIndex=7;
				m_pDrawing = new CDrawLine();
				
				break;
			case 8:
				CSharedPara::m_ShapeIndex=8;
				m_pDrawing = new CDrawLine();
				break;

			default:
				//选择模式
				break;
			}
		}
		//Cpainter10View::OnLButtonUp(nFlags, point);
	}


	

	void Cpainter10View::OnMouseMove(UINT nFlags, CPoint point)//鼠标移动
	{
		// TODO: Add your message handler code here and/or call default
		CDC*	pDC	= NULL;
		if(m_pDrawing)
		{
			pDC = GetDC();
			OnPrepareDC(pDC);
			pDC->DPtoLP(&point);
			m_pDrawing->OnMouseMove(pDC, nFlags, point);
			ReleaseDC(pDC);
		}
		//CView::OnMouseMove(nFlags, point);
	}

	void Cpainter10View::MyKillTimer()
	{
		if(m_nTimer)
		{
			KillTimer(m_nTimer);
			m_nTimer = 0;
		}
		//清楚以前闪烁
		if(m_bNeed)
		{
			FlashShape();
		}

		m_dwNewSel = 0;
		m_dwOldSel = 0;
		m_bNeed = FALSE;
	}

	void Cpainter10View::FlashShape()
	{
		CDC*			pDC			= NULL;
		Cpainter10Doc*	pDoc		= NULL;
		int				iDrawMode	= 0;
		CPen			pen(PS_SOLID, 3, RGB(255, 0, 0));
		CPen*			pOldPen		= NULL;
		if(false)
		{
			pDoc = GetDocument();
			pDC = GetDC();
			OnPrepareDC(pDC);
			iDrawMode = pDC->SetROP2(R2_NOTXORPEN);
			pOldPen = pDC->SelectObject(&pen);
			((CDrawShape*)(pDoc->m_ObArray.GetAt(m_dwNewSel-1)))->Draw(pDC,false);
			pDC->SelectObject(pOldPen);
			pen.DeleteObject();
			pDC->SetROP2(iDrawMode);
			ReleaseDC(pDC);
			pDC = NULL;

			m_bNeed = !m_bNeed;
		}
	}

	void Cpainter10View::OnButtonRollback()//实现撤销功能，删掉对象数组最后一个对象，强制重绘客户区
	{
		// TODO: Add your command handler code here
		Cpainter10Doc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		int arraySize = pDoc->m_ObArray.GetSize();
		if(arraySize>0)
		{
			pDoc->m_ObArray.RemoveAt(arraySize-1);
			Invalidate();
		}
	}


	void Cpainter10View::OnButtonSave()//保存图片，可以保存多种格式
	{
		CClientDC dc(this);    

		CRect rect; 

		BOOL  showMsgTag;                  //是否要弹出”图像保存成功对话框" 

		GetClientRect(&rect);                  //获取画布大小

		HBITMAP hbitmap = CreateCompatibleBitmap(dc, rect.right-rect.left, rect. bottom-rect.top);     

		//创建兼容位图

		HDC hdc = CreateCompatibleDC(dc);      //创建兼容DC，以便将图像保存为不同的格式

		HBITMAP hOldMap = (HBITMAP)SelectObject(hdc,hbitmap); 

		//将位图选入DC，并保存返回值 

		BitBlt(hdc,0,0,rect.right-rect.left,rect.bottom-rect.top,dc,0,0,SRCCOPY); 

		//将屏幕DC的图像复制到内存DC中



		CImage image;

		image.Attach(hbitmap);                //将位图转化为一般图像



		if (!saveTag)                          //如果图像是第一次被写入,则打开对话框

		{

			saveTag = TRUE;

			showMsgTag = TRUE;

			CString  strFilter = _T("位图文件(*.bmp)|*.bmp|JPEG 图像文件|*.jpg|GIF图像文件|*.gif|PNG图像文件|*.png|其他格式(*.*)|*.*||");

			CFileDialog dlg(FALSE,_T("bmp"),_T("iPaint1.bmp"),NULL,strFilter);

			if (dlg.DoModal() != IDOK)

				return;



			CString strFileName;          //如果用户没有指定文件扩展名，则为其添加一个

			CString strExtension;

			strFileName = dlg.m_ofn.lpstrFile;

			if (dlg.m_ofn.nFileExtension = 0)               //扩展名项目为0

			{

				switch (dlg.m_ofn.nFilterIndex)

				{

				case 1:

					strExtension = "bmp";break;

				case 2:

					strExtension = "jpg";break;

				case 3:

					strExtension = "gif";break;

				case 4:

					strExtension = "png";break;

				default:

					break;

				}

				strFileName = strFileName + "." + strExtension;  

			}

			saveFilePath = strFileName;     //saveFilePath为视类中的全局变量,类型为CString

		} 

		else

		{

			showMsgTag = FALSE;

		}



		//AfxMessageBox(saveFilePath);               //显示图像保存的全路径(包含文件名)

		HRESULT hResult = image.Save(saveFilePath);     //保存图像

		if (FAILED(hResult))

		{

			MessageBox(_T("保存图像文件失败！"));

		}

		else

		{

			if(showMsgTag)

				MessageBox(_T("文件保存成功！"));

		}

		image.Detach();

		SelectObject(hdc,hOldMap);
	}

	void Cpainter10View::OnButtonCircle()
	{
		// TODO: Add your command handler code here
		if(m_pDrawing)
		{
			delete m_pDrawing;
			m_pDrawing = NULL;
		}
		m_pDrawing = new CDrawCircle();
		m_iMode = 1;
		//MyKillTimer();
	}





	//以下的都是对选择图形的按钮进行相应
	void Cpainter10View::OnButtonLine()
	{
		// TODO: Add your command handler code here
		CSharedPara::m_ShapeIndex=3;
		if(m_pDrawing)
		{
			delete m_pDrawing;
			m_pDrawing = NULL;
		}
		m_pDrawing = new CDrawLine();
		m_iMode = 0;
		//MyKillTimer();
	}





	void Cpainter10View::OnButtonRect()
	{
		// TODO: Add your command handler code here
		CSharedPara::m_ShapeIndex=5;
		if(m_pDrawing)
		{
			delete m_pDrawing;
			m_pDrawing = NULL;
		}
		m_pDrawing = new CDrawRect();
		m_iMode = 2;
		//MyKillTimer();
	}


	void Cpainter10View::OnButtonBird()
	{
		// TODO: Add your command handler code here
		if(m_pDrawing)
		{
			delete m_pDrawing;
			m_pDrawing = NULL;
		}
		CSharedPara::m_ShapeIndex=3;
		m_pDrawing = new CDrawSpecial();
		
		(CDrawShape*)m_pDrawing;
		m_iMode = 3;
	}


	void Cpainter10View::OnButtonLamp()
	{
		// TODO: Add your command handler code here
		if(m_pDrawing)
		{
			delete m_pDrawing;
			m_pDrawing = NULL;
		}
		CSharedPara::m_ShapeIndex=4;
		m_pDrawing = new CDrawSpecial();
		
		(CDrawShape*)m_pDrawing;
		m_iMode = 4;
	}


	void Cpainter10View::OnButtonSnow()
	{
		// TODO: Add your command handler code here
		if(m_pDrawing)
		{
			delete m_pDrawing;
			m_pDrawing = NULL;
		}
		CSharedPara::m_ShapeIndex=5;
		m_pDrawing = new CDrawSpecial();
		
		(CDrawShape*)m_pDrawing;
		m_iMode = 5;
	}

	void Cpainter10View::OnButtonClock()
	{
		// TODO: Add your command handler code here
		if(m_pDrawing)
		{
			delete m_pDrawing;
			m_pDrawing = NULL;
		}
		CSharedPara::m_ShapeIndex=6;
		m_pDrawing = new CDrawSpecial();
		(CDrawShape*)m_pDrawing;
		TRACE("CLOCKCHANGED");
		m_iMode = 6;
	}

	void Cpainter10View::OnButtonFlower()
	{
		// TODO: Add your command handler code here
		if(m_pDrawing)
		{
			delete m_pDrawing;
			m_pDrawing = NULL;
		}
		CSharedPara::m_ShapeIndex=7;
		m_pDrawing = new CDrawSpecial();
		
		(CDrawShape*)m_pDrawing;
		m_iMode = 7;
	}

	void Cpainter10View::OnButtonOrange()
	{
		// TODO: Add your command handler code here
		if(m_pDrawing)
		{
			delete m_pDrawing;
			m_pDrawing = NULL;
		}
		CSharedPara::m_ShapeIndex=8;
		m_pDrawing = new CDrawSpecial();
		

		(CDrawShape*)m_pDrawing;
		m_iMode = 8;
	}


	void Cpainter10View::OnDrawspeedFast()
	{
		// TODO: Add your command handler code here
		CSharedPara::m_Speed=1;
		MyKillTimer();
		m_nTimer=SetTimer(0,100,NULL);
	}


	void Cpainter10View::OnDrawspeedMedium()
	{
		// TODO: Add your command handler code here
		CSharedPara::m_Speed=3;
		MyKillTimer();
		m_nTimer=SetTimer(0,500,NULL);
	}


	void Cpainter10View::OnDrawspeedSlow()
	{
		// TODO: Add your command handler code here
		CSharedPara::m_Speed=6;
		MyKillTimer();
		m_nTimer=SetTimer(0,1000,NULL);
	}


	void Cpainter10View::OnTimer(UINT_PTR nIDEvent)
	{
		// TODO: Add your message handler code here and/or call default
		FlashShape();
		CView::OnTimer(nIDEvent);
	}
