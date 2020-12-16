
// ChangeWinPwdSrvDlg.h: 头文件
//
#include "CListenSrv.h"
#pragma once


// CChangeWinPwdSrvDlg 对话框
class CChangeWinPwdSrvDlg : public CDialogEx
{
// 构造
public:
	CChangeWinPwdSrvDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHANGEWINPWDSRV_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CString strState;
	CListenSrv lstnsrv;
	virtual void OnOK();
public:
	afx_msg void OnBnClickedButton1();
protected:
	afx_msg LRESULT OnLstnSrvStatus(WPARAM wParam, LPARAM lParam);
private:
	int intCounts;
};
