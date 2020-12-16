#pragma once
#include <afxsock.h>
#define WM_LSTN_SRV_STATUS WM_USER+1007
class CListenSrv :
	public CAsyncSocket
{
public:
	//CString GetHostIP();
	void SetListenPort(UINT port);
	UINT GetListenPort();

	void SetHwnd(HWND hwnd);
	HWND GetHwnd();

	bool OpenListen();
	bool IsListening;
	int m_AcceptedCounts;
	CListenSrv();
	~CListenSrv();

private:
	UINT m_listenport;
	HWND m_hwnd;
public:
	virtual void OnClose(int nErrorCode);
	virtual void OnAccept(int nErrorCode);
};

