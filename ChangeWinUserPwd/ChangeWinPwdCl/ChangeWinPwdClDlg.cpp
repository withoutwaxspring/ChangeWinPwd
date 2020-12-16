
// ChangeWinPwdClDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "ChangeWinPwdCl.h"
#include "ChangeWinPwdClDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChangeWinPwdClDlg 对话框



CChangeWinPwdClDlg::CChangeWinPwdClDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHANGEWINPWDCL_DIALOG, pParent)
	, strSrvName(_T(""))
	, strUserName(_T(""))
	, strOldPwd(_T(""))
	, strNewPwd(_T(""))
	, strComfirmpwd(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChangeWinPwdClDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, strSrvName);
	DDX_Text(pDX, IDC_EDIT2, strUserName);
	DDX_Text(pDX, IDC_EDIT3, strOldPwd);
	DDX_Text(pDX, IDC_EDIT4, strNewPwd);
	DDX_Text(pDX, IDC_EDIT5, strComfirmpwd);
	DDX_Control(pDX, IDC_BUTTON1, c_comfire);
}

BEGIN_MESSAGE_MAP(CChangeWinPwdClDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON2, &CChangeWinPwdClDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CChangeWinPwdClDlg::OnBnClickedButton1)
	ON_MESSAGE(WM_SRVRESPONSE, &CChangeWinPwdClDlg::OnSrvResponse)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CChangeWinPwdClDlg 消息处理程序

BOOL CChangeWinPwdClDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	//ShowWindow(SW_MINIMIZE);

	// TODO: 在此添加额外的初始化代码
	//strSrvName = L"\\\\FILESRV";
	strSrvName = L"192.168.0.14";
	UpdateData(FALSE);
	m_pClUser = nullptr;
	

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CChangeWinPwdClDlg::OnPaint()
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
HCURSOR CChangeWinPwdClDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CChangeWinPwdClDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnOK();
}


void CChangeWinPwdClDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void CChangeWinPwdClDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (strSrvName.IsEmpty())
	{
		CWnd::MessageBoxW(L"服务器名不能为空！");
		return;
	}
	if (strUserName.IsEmpty()) 
	{
		CWnd::MessageBoxW(L"用户名不能为空！");
		return;
	}
	if (strOldPwd.IsEmpty())
	{
		CWnd::MessageBoxW(L"旧密码不能为空！");
		return;
	}
	if (strNewPwd.IsEmpty())
	{
		CWnd::MessageBoxW(L"新密码不能为空！");
		return;
	}
	if (strComfirmpwd.IsEmpty())
	{
		CWnd::MessageBoxW(L"确认密码不能为空！");
		return;
	}
	if (strNewPwd != strComfirmpwd)
	{
		CWnd::MessageBoxW(L"新密码与确认密码不一致！");
		return;
	}

	if (nullptr != m_pClUser)
	{
		m_pClUser->Close();
		delete m_pClUser;
		m_pClUser = nullptr;
	}
	m_pClUser = new CUserClient();
	m_pClUser->m_strUsername = strUserName;
	m_pClUser->m_strOldpwd = strOldPwd;
	m_pClUser->m_strNewpwd = strNewPwd;
	m_pClUser->m_strSrvname = strSrvName;
	m_pClUser->m_hwnd = GetSafeHwnd();

	if (!m_pClUser->Create())
	{
		CWnd::MessageBoxW(L"网络未连接！");
		return;
	}


	//CWnd::MessageBoxW(strSrvName);
	c_comfire.EnableWindow(FALSE);
	m_pClUser->Connect(strSrvName, 8083);
	SetTimer(12, 6000, NULL);
	//DWORD errcode;
	//if (m_pClUser->Connect(strSrvName, 8083))
	//{
	//	errcode = GetLastError();
	//	CWnd::MessageBoxW(L"False！");
	//}
	//else
	//{
	//	errcode = GetLastError();
	//	CWnd::MessageBoxW(L"TURE！");
	//}
	//
	//CString msgstr;
	//msgstr.Format(L"%d", errcode);
	//CWnd::MessageBoxW(msgstr);
}


afx_msg LRESULT CChangeWinPwdClDlg::OnSrvResponse(WPARAM wParam, LPARAM lParam)
{
	// 0为未连接服务器，1为成功修改密码，2为密码不正确，3为修改失败,4为连接服务器失败
	switch (m_pClUser->GetErrCode())
	{
	case 0:
	{
		//CWnd::MessageBoxW(L"未连接服务器！");
	}
	break;
	case 1:
	{
		CWnd::MessageBoxW(L"成功修改密码！");
	}
	break;
	case 2:
	{
		CWnd::MessageBoxW(L"用户名或密码不正确！");
	}
	break;
	case 3:
	{
		CWnd::MessageBoxW(L"修改失败！");
	}
	break;
	case 4:
	{
		CWnd::MessageBoxW(L"连接服务器失败！");
	}
	break;
	default:
		break;
	}
	KillTimer(12);
	if (nullptr != m_pClUser)
	{
		m_pClUser->Close();
		delete m_pClUser;
		m_pClUser = nullptr;
	}
	c_comfire.EnableWindow(TRUE);
	return 0;
}


void CChangeWinPwdClDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnTimer(nIDEvent);
	switch (nIDEvent)
	{
	case 12:
	{
		KillTimer(12);
		if (nullptr != m_pClUser)
		{
			m_pClUser->Close();
			delete m_pClUser;
			m_pClUser = nullptr;
			CWnd::MessageBoxW(L"连接服务器超时！");
		}
		c_comfire.EnableWindow(TRUE);
		
	}
	break;
	default:
		break;
	}
}
