
// ChangeWinPwdClDlg.h: 头文件
//
#include "CUserClient.h"
#pragma once


// CChangeWinPwdClDlg 对话框
class CChangeWinPwdClDlg : public CDialogEx
{
// 构造
public:
	CChangeWinPwdClDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHANGEWINPWDCL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CString strSrvName;
	CString strUserName;
	CString strOldPwd;
	CString strNewPwd;
	virtual void OnOK();
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
private:
	CString strComfirmpwd;
protected:
	CUserClient* m_pClUser;
	afx_msg LRESULT OnSrvResponse(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
private:
	CButton c_comfire;
};
