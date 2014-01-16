//////////////////////////////////////////////////////////////////////////////////////
//
//						The Bohge Engine License (BEL)
//
//	Copyright (c) 2011-2014 Peng Zhao
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in 
//	all copies or substantial portions of the Software. And the logo of 
//	Bohge Engine shall be displayed full screen for more than 3 seconds 
//	when the software is started. Copyright holders are allowed to develop 
//	game edit based on Bohge Engine, The edit must be released under the MIT 
//	open source license if it is going to be published. In no event shall 
//	copyright holders be prohibited from using any code of Bohge Engine 
//	to develop any other analogous game engines.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//
//////////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "BattleServer.h"
#include "MainDlg.h"
#include "Base.h"
#include "ServerConfig.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TIMERID 1

CMainDlg* smd;
void ClientConnect()
{
	if( smd!=NULL )
	{
		((CMainDlg*)smd)->Connects();
	}
}
void ClientDisonnect(int num)
{
	if( smd!=NULL )
	{
		((CMainDlg*)smd)->Disconnect(num);
	}
}
/////////////////////////////////////////////////////////////////////
// 在主界面中显示提示信息
void ShowMessage(const CString szFormat,...)
{
	// 根据传入的参数格式化字符串
	CString   strMessage;
	va_list   arglist;

	// 处理变长参数
	va_start(arglist, szFormat);
	strMessage.FormatV(szFormat,arglist);
	va_end(arglist);

	// 在主界面中显示
	if( smd!=NULL )
	{
		((CMainDlg*)smd)->AddInformation(strMessage);
		TRACE( strMessage+_T("\n") );
	}	
}
void DebugMessage(const CString szFormat,...)
{
	if( ServerConfig::Instance().isShowLog() )
	{
		// 根据传入的参数格式化字符串
		CString   strMessage;
		va_list   arglist;

		// 处理变长参数
		va_start(arglist, szFormat);
		strMessage.FormatV(szFormat,arglist);
		va_end(arglist);

		// 在主界面中显示
		if( smd!=NULL )
		{
			((CMainDlg*)smd)->AddInformation(strMessage);
			TRACE( strMessage+_T("\n") );
		}	
	}
}

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CMainDlg 对话框




CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
: CDialog(CMainDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CMainDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_STOP, &CMainDlg::OnBnClickedStop)
	ON_WM_CLOSE()
	//ON_MESSAGE(WM_MSG_NEW_MSG,OnNewMsg)
	ON_BN_CLICKED(IDCANCEL, &CMainDlg::OnBnClickedCancel)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CMainDlg 消息处理程序

BOOL CMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// 初始化界面信息
	this->Init();	

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMainDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMainDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//////////////////////////////////////////////////////////////////////
// 初始化Socket库以及界面信息
void CMainDlg::Init()
{
	// 初始化Socket库
	if( false==m_IOCP.LoadSocketLib() )
	{
		AfxMessageBox(_T("load Winsock 2.2 fail, server can not start"));
		PostQuitMessage(0);
	}

	// 设置本机IP地址
	SetDlgItemText( IDC_STATIC_SERVERIP,m_IOCP.GetLocalIP().c_str() );
	// 设置默认端口
	SetDlgItemInt( IDC_EDIT_PORT,DEFAULT_PORT );
	// 初始化列表
	this->InitListCtrl();
	// 绑定主界面指针(为了方便在界面中显示信息 )
	smd = this;

	m_Connects = 0;
	this->ChageConnectInfo();

}

///////////////////////////////////////////////////////////////////////
//	开始监听
void CMainDlg::OnBnClickedOk()
{
	if( false==m_IOCP.Start() )
	{
		AfxMessageBox(_T("Fail to start server！"));
		return;	
	}
	this->SetTimer( TIMERID, ServerConfig::Instance().Timer(), NULL );

	GetDlgItem(IDOK)->EnableWindow(FALSE);
	GetDlgItem(IDC_STOP)->EnableWindow(TRUE);
}

//////////////////////////////////////////////////////////////////////
//	结束监听
void CMainDlg::OnBnClickedStop()
{
	m_IOCP.Stop();
	this->KillTimer( TIMERID );

	GetDlgItem(IDC_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDOK)->EnableWindow(TRUE);
}

///////////////////////////////////////////////////////////////////////
//	初始化List Control
void CMainDlg::InitListCtrl()
{
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_INFO);
	pList->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	pList->InsertColumn(0,"INFORMATION",LVCFMT_LEFT,1000);
}

///////////////////////////////////////////////////////////////////////
//	点击“退出”的时候，停止监听，清空Socket类库
void CMainDlg::OnBnClickedCancel()
{
	// 停止监听
	m_IOCP.Stop();
	this->KillTimer( TIMERID );
	CDialog::OnCancel();
}

///////////////////////////////////////////////////////////////////////
//	系统退出的时候，为确保资源释放，停止监听，清空Socket类库
void CMainDlg::OnDestroy()
{
	OnBnClickedCancel();

	CDialog::OnDestroy();
}

void CMainDlg::OnTimer( UINT_PTR nIDEvent )
{
	m_IOCP.OnTimer();
}
