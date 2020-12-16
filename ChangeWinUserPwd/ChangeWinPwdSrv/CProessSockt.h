#pragma once
#include <afxsock.h>
class CProessSockt :
	public CAsyncSocket
{
public:
	CProessSockt();
	~CProessSockt();

	void SetHwnd(HWND hwnd);
	HWND GetHwnd();
	bool SendMsg(CString strMsg);


private:
	HWND m_hwnd;
	int ChangeUserPwd(CString strUserinfo);

public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnSend(int nErrorCode);
};

