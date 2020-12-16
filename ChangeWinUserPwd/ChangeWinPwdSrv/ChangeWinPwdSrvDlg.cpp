
// ChangeWinPwdSrvDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "ChangeWinPwdSrv.h"
#include "ChangeWinPwdSrvDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CChangeWinPwdSrvDlg 对话框



CChangeWinPwdSrvDlg::CChangeWinPwdSrvDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHANGEWINPWDSRV_DIALOG, pParent)
	, strState(_T(""))
	, intCounts(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChangeWinPwdSrvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATE, strState);
	DDX_Text(pDX, IDC_COUNT, intCounts);
}

BEGIN_MESSAGE_MAP(CChangeWinPwdSrvDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CChangeWinPwdSrvDlg::OnBnClickedButton1)
	ON_MESSAGE(WM_LSTN_SRV_STATUS, &CChangeWinPwdSrvDlg::OnLstnSrvStatus)
END_MESSAGE_MAP()


// CChangeWinPwdSrvDlg 消息处理程序

BOOL CChangeWinPwdSrvDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	lstnsrv.SetHwnd(GetSafeHwnd());
	lstnsrv.SetListenPort(8083);
	if (lstnsrv.OpenListen())
	{
		intCounts = lstnsrv.m_AcceptedCounts;
		strState = L"正在运行中";
		UpdateData(FALSE);
	}
	else
	{
		CWnd::MessageBoxW(L"端口打开失败");
		exit(1);
	}
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CChangeWinPwdSrvDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CChangeWinPwdSrvDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CChangeWinPwdSrvDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CChangeWinPwdSrvDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnOK();
}


void CChangeWinPwdSrvDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


afx_msg LRESULT CChangeWinPwdSrvDlg::OnLstnSrvStatus(WPARAM wParam, LPARAM lParam)
{
	intCounts = lstnsrv.m_AcceptedCounts;
	if (lstnsrv.IsListening)
	{
		strState = L"正在运行中";
	}
	else
	{
		strState = L"已停止运行，请重启！";
	}
	UpdateData(FALSE);
	return 0;
}
