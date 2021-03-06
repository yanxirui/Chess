
// XChessDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "XChess.h"
#include "XChessDlg.h"
#include "afxdialogex.h"
#include <cstring>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define BORDERWIDTH      13   //棋盘左右边缘的宽度
#define BORDERHEIGHT    15   //棋盘上下边缘的宽度
#define GRILLEWIDTH     35   //棋盘上每个格子的高度
#define GRILLEHEIGHT    35   //棋盘上每个格子的宽度


#define RedTime 1
#define BlkTime 2

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();


	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

														// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CXChessDlg 对话框



CXChessDlg::CXChessDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_XCHESS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_SelectMoveFrom = NOMOVE;
	m_SelectMoveTo = NOMOVE;
}


void CXChessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RED_TIMEPASS, m_RedTimePass_Ctr);
	DDX_Control(pDX, IDC_RED_TIMELEFT, m_RedTimeLeft_Ctr);
	DDX_Control(pDX, IDC_BLK_TIMEPASS, m_BlkTimePass_Ctr);
	DDX_Control(pDX, IDC_BLK_TIMELEFT, m_BlkTimeLeft_Ctr);
	DDX_Control(pDX, IDC_BUT_CLOSE, m_ButExit);
	DDX_Control(pDX, IDC_BUT_BEGIN, m_ButBegin);
}

BEGIN_MESSAGE_MAP(CXChessDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUT_BEGIN, &CXChessDlg::OnBnClickedButBegin)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(IDM_LET_COMPUTERTHINK, OnLetComputerThink)
	//ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUT_CLOSE, &CXChessDlg::OnBnClickedButClose)
END_MESSAGE_MAP()


// CXChessDlg 消息处理程序

BOOL CXChessDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

									// TODO: Add extra initialization here
	BITMAP BitMap;
	m_BoardBmp.LoadBitmap(IDB_CHESSBOARD);
	m_BoardBmp.GetBitmap(&BitMap);
	m_nBoardWidth = BitMap.bmWidth;
	m_nBoardHeight = BitMap.bmHeight;
	m_BoardBmp.DeleteObject();

	m_Chessman.Create(IDB_CHESSMAN, 31, 15, RGB(0, 128, 128));

	rectBoard.left = BORDERWIDTH;
	rectBoard.right = BORDERWIDTH + GRILLEWIDTH * 9;
	rectBoard.top = BORDERHEIGHT;
	rectBoard.bottom = BORDERHEIGHT + GRILLEHEIGHT * 10;

	m_BlkTimeLeft_Ctr.SetWindowText(_T(""));
	m_BlkTimePass_Ctr.SetWindowText(_T(""));
	m_RedTimeLeft_Ctr.SetWindowText(_T(""));
	m_RedTimePass_Ctr.SetWindowText(_T(""));


	InitData();
	m_TotalTime = CTimeSpan(0, 0, 30, 0);
	m_BlkTimer = 0;
	m_RedTimer = 0;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CXChessDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CXChessDlg::OnPaint()
{
	/*	if (IsIconic())
	{
	CPaintDC dc(this); // device context for painting

	SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

	// Center icon in client rectangle
	int cxIcon = GetSystemMetrics(SM_CXICON);
	int cyIcon = GetSystemMetrics(SM_CYICON);
	CRect rect;
	GetClientRect(&rect);
	int x = (rect.Width() - cxIcon + 1) / 2;
	int y = (rect.Height() - cyIcon + 1) / 2;

	// Draw the icon
	dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
	CDialog::OnPaint();
	}
	*/

	CPaintDC dc(this);
	CDC MemDC;
	POINT pt;
	CBitmap *pOldBmp;
	int z;

	MemDC.CreateCompatibleDC(&dc);
	m_BoardBmp.LoadBitmap(IDB_CHESSBOARD);
	pOldBmp = MemDC.SelectObject(&m_BoardBmp);
	for (short i = 0; i<90; i++)
	{
		if (m_interface[i] == 0)
		{
			if (i == m_SelectMoveFrom)
			{
				pt.x = (i % 9)*GRILLEHEIGHT + BORDERWIDTH;
				pt.y = (i / 9)*GRILLEWIDTH + BORDERHEIGHT;
				m_Chessman.Draw(&MemDC, 14, pt, ILD_TRANSPARENT);
			}
			continue;
		}
		pt.x = (i % 9)*GRILLEHEIGHT + BORDERWIDTH;
		pt.y = (i / 9)*GRILLEWIDTH + BORDERHEIGHT;

		z = IntToSubscript(m_interface[i]);

		m_Chessman.Draw(&MemDC, z, pt, ILD_TRANSPARENT);
		if (i == m_SelectMoveFrom)
			m_Chessman.Draw(&MemDC, 14, pt, ILD_TRANSPARENT);
		if (i == m_SelectMoveTo)
			m_Chessman.Draw(&MemDC, 14, pt, ILD_TRANSPARENT);
	}

	dc.BitBlt(0, 0, m_nBoardWidth, m_nBoardHeight, &MemDC, 0, 0, SRCCOPY);
	MemDC.SelectObject(&pOldBmp);
	MemDC.DeleteDC();
	m_BoardBmp.DeleteObject();
}



//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CXChessDlg::OnQueryDragIcon()
{
	return (HCURSOR)m_hIcon;
}



void CXChessDlg::OnBnClickedButBegin()
{
	// TODO: 在此添加控件通知处理程序代码
	InitData();
	m_SelectMoveFrom = NOMOVE;
	m_SelectMoveTo = NOMOVE;
	InvalidateRect(&rectBoard, false);
	UpdateWindow();

	m_Board.ClearBoard();
	m_Board.StringToArray("rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR w - - 0 1");
	m_tsBlkTimeLeft = m_TotalTime;
	m_BlkTimeLeft_Ctr.SetWindowText(m_tsBlkTimeLeft.Format("%H:%M:%S"));
	m_tsBlkTimePass = m_TotalTime - m_tsBlkTimeLeft;
	m_BlkTimePass_Ctr.SetWindowText(m_tsBlkTimePass.Format("%H:%M:%S"));
	m_tsRedTimeLeft = m_tsBlkTimeLeft;
	m_RedTimeLeft_Ctr.SetWindowText(m_tsRedTimeLeft.Format("%H:%M:%S"));
	m_tsRedTimePass = m_TotalTime - m_tsRedTimeLeft;
	m_RedTimePass_Ctr.SetWindowText(m_tsRedTimePass.Format("%H:%M:%S"));

	m_RedTimer = SetTimer(1, 1000, NULL);
	m_gameState = REDTHINKING;
}


BOOL CXChessDlg::IsPtInBoard(CPoint point)	//鼠标点是否在棋盘内
{
	return rectBoard.PtInRect(point);
}

void CXChessDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	// TODO: Add your message handler code here and/or call default
	if (!IsPtInBoard(point) || m_gameState != REDTHINKING)
		return;

	int SideTag = 16 + m_HumanSide * 16;

	short dest, from;
	int num;

	//清空高亮显示
	from = m_SelectMoveFrom;
	dest = m_SelectMoveTo;
	m_SelectMoveTo = NOMOVE;
	m_SelectMoveFrom = NOMOVE;
	if (from != NOMOVE)
		RequireDrawCell(from);
	if (dest != NOMOVE)
		RequireDrawCell(dest);

	dest = GetPiecePos(point);

	BYTE piece = m_interface[dest];

	if (piece & SideTag) //选手选中本方棋子
	{
		if (from != NOMOVE) {	//开始已经选中有本方其他棋子,则取消高亮显示原来选中的棋子
			m_SelectMoveFrom = NOMOVE;
			RequireDrawCell(from);
		}
		m_SelectMoveFrom = dest;	//高亮显示新选中的棋子
		RequireDrawCell(dest);
	}
	else //棋子落在空处或者他方的棋子上
		if (from != NOMOVE)	//如果已经先选中本方棋子
		{
			Move mv;
			mv.from = ((from / 9 + 3) * 16 + from % 9 + 3);	//将10*9的棋盘位置转换成16*16的棋盘位置
			mv.to = ((dest / 9 + 3) * 16 + dest % 9 + 3);

			if (m_Board.LegalMove(mv)) //走法合理性检验，源位置z，目的位置k
			{
				m_Board.MakeMove(mv);
				m_interface[dest] = m_interface[from];
				m_interface[from] = 0;

				m_SelectMoveTo = dest;
				RequireDrawCell(dest);
				m_SelectMoveFrom = from;	//着重显示走法起始点
				RequireDrawCell(from);		//将源点及目的点重新显示
				Beep(200, 300);

				num = m_Board.HasLegalMove(); //判断胜负
				if (!num) {
					KillTimer(m_RedTimer);
					m_gameState = GAMEOVER;
					MessageBox(_T("红方获胜"), _T("系统消息"));
					return;
				}

				KillTimer(m_RedTimer);
				m_gameState = BLACKTHINKING;

				PostMessage(WM_COMMAND, IDM_LET_COMPUTERTHINK);
			}
		}

	CDialog::OnLButtonDown(nFlags, point);
}

short CXChessDlg::GetPiecePos(POINT pt)	//鼠标点对应的棋盘位置
{
	if (!rectBoard.PtInRect(pt)) return -1;
	short x = (pt.x - rectBoard.left) / GRILLEWIDTH;
	short y = (pt.y - rectBoard.top) / GRILLEHEIGHT;

	return x + y * 9;
}

void CXChessDlg::RequireDrawCell(short pos)	//重绘棋盘上的一点
{
	CRect rect = GetPieceRect(pos);
	InvalidateRect(&rect, false);
	UpdateWindow();
}

CRect CXChessDlg::GetPieceRect(short pos)		//棋盘位置对应的矩形区域
{
	short x = BORDERWIDTH + (pos % 9) * GRILLEWIDTH;
	short y = BORDERHEIGHT + (pos / 9) * GRILLEHEIGHT;
	CRect rect(x, y, x + GRILLEWIDTH, y + GRILLEHEIGHT);
	return rect;
}


void CXChessDlg::OnLetComputerThink()
{
	if (m_gameState == GAMEOVER)
		return;
	CTime t1 = CTime::GetCurrentTime();


	m_Board.BestMove.from = 0;
	m_Board.BestMove.to = 0;

	m_Board.StackTop = 0;
	//m_Board.DepthNow = 0;
	m_Board.MaxDepth = 3;
	m_Board.NegaMaxSearch(m_Board.MaxDepth, -m_Board.MaxValue, m_Board.MaxValue);


	m_tsBlkTimePass = m_tsBlkTimePass + (CTime::GetCurrentTime() - t1);
	if (m_tsBlkTimePass > m_TotalTime)
	{
		m_gameState = GAMEOVER;
		if (m_RedTimer)
			KillTimer(m_RedTimer);
		MessageBox(_T("黑方超时判负"), _T("系统提示"));
		return;
	}

	int z, k;
	z = m_Board.BestMove.from;
	k = m_Board.BestMove.to;



	if (z == 0)
	{
		m_gameState = GAMEOVER;
		if (m_RedTimer)
			KillTimer(m_RedTimer);
		MessageBox(_T("黑方认输，红方获胜"), _T("系统提示"));
		return;
	}

	int num;
	m_Board.MakeMove(m_Board.BestMove);

	short zz, kk;
	//清空高亮显示
	zz = m_SelectMoveFrom;
	kk = m_SelectMoveTo;
	m_SelectMoveFrom = NOMOVE;
	m_SelectMoveTo = NOMOVE;
	RequireDrawCell(zz);
	RequireDrawCell(kk);

	zz = ((z / 16 - 3) * 9 + z % 16 - 3);	//将16*16的棋盘位置转换成10*9的棋盘位置
	kk = ((k / 16 - 3) * 9 + k % 16 - 3);

	m_interface[kk] = m_interface[zz];
	m_interface[zz] = 0;

	//高亮显示电脑走法
	m_SelectMoveFrom = zz;
	m_SelectMoveTo = kk;
	RequireDrawCell(zz);
	RequireDrawCell(kk);
	Beep(500, 300);

	m_tsBlkTimeLeft = m_TotalTime - m_tsBlkTimePass;
	m_BlkTimeLeft_Ctr.SetWindowText(m_tsBlkTimeLeft.Format("%H:%M:%S"));
	m_BlkTimePass_Ctr.SetWindowText(m_tsBlkTimePass.Format("%H:%M:%S"));

	num = m_Board.HasLegalMove(); //
	if (!num) {
		KillTimer(m_RedTimer);
		m_gameState = GAMEOVER;
		Beep(700, 1000);
		MessageBox(_T("黑方获胜"), _T("系统消息"));
		return;
	}

	m_gameState = REDTHINKING;
	m_RedTimer = SetTimer(1, 1000, NULL);
}

void CXChessDlg::OnTimer(UINT nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	switch (nIDEvent)
	{
	case 2:
		m_tsBlkTimePass = m_tsBlkTimePass + CTimeSpan(0, 0, 0, 1);
		m_tsBlkTimeLeft = m_TotalTime - m_tsBlkTimePass;
		m_BlkTimeLeft_Ctr.SetWindowText(m_tsBlkTimeLeft.Format("%H:%M:%S"));
		m_BlkTimePass_Ctr.SetWindowText(m_tsBlkTimePass.Format("%H:%M:%S"));
		break;
	case 1:
		m_tsRedTimePass = m_tsRedTimePass + CTimeSpan(0, 0, 0, 1);
		if (m_tsRedTimePass > m_TotalTime)
		{
			m_gameState = GAMEOVER;
			KillTimer(m_RedTimer);
			MessageBox(_T("红方超时判负"), _T("系统提示"));
		}
		else
		{
			m_tsRedTimeLeft = m_TotalTime - m_tsRedTimePass;
			m_RedTimeLeft_Ctr.SetWindowText(m_tsRedTimeLeft.Format("%H:%M:%S"));
			m_RedTimePass_Ctr.SetWindowText(m_tsRedTimePass.Format("%H:%M:%S"));
		}
		break;
	default:
		break;
	}
	CDialog::OnTimer(nIDEvent);
}

void CXChessDlg::InitData()
{
	static BYTE board[Board_Size] = {
		39,37,35,33,32,34,36,38,40,
		0,0,0,0,0,0,0,0,0,
		0,41,0,0,0,0,0,42,0,
		43,0,44,0,45,0,46,0,47,
		0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,
		27,0,28,0,29,0,30,0,31,
		0,25,0,0,0,0,0,26,0,
		0,0,0,0,0,0,0,0,0,
		23,21,19,17,16,18,20,22,24
	};
	for (int i = 0; i < Board_Size; i++)
	{
		m_interface[i] = board[i];
	}

	m_ComputerSide = 1;
	m_HumanSide = 0;
	m_gameState = GAMEOVER;
}

int CXChessDlg::IntToSubscript(int a)
{
	if (a<16 && a >= 48)
		return 14;

	switch (a)
	{
		//红方棋子
	case 16:	return 0;
	case 17:
	case 18:	return 1;
	case 19:
	case 20:	return 2;
	case 21:
	case 22:	return 3;
	case 23:
	case 24:	return 4;
	case 25:
	case 26:	return 5;
	case 27:
	case 28:
	case 29:
	case 30:
	case 31:	return 6;

		//黑方棋子
	case 32:	return 7;
	case 33:
	case 34:	return 8;
	case 35:
	case 36:	return 9;
	case 37:
	case 38:	return 10;
	case 39:
	case 40:	return 11;
	case 41:
	case 42:	return 12;
	case 43:
	case 44:
	case 45:
	case 46:
	case 47:	return 13;

	default:	return 14;
	}
}

void CXChessDlg::OnBnClickedButClose()
{
	// TODO: 在此添加控件通知处理程序代码
	if (MessageBox(_T("确定要退出吗？"), _T("系统提示"), MB_OKCANCEL) == IDOK)
	{
		DestroyWindow();
		delete this;
	}
}
