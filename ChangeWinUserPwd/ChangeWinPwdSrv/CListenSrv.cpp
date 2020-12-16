#include "pch.h"
#include "CListenSrv.h"
#include "CProessSockt.h"
//CString CListenSrv::GetHostIP()
//{
//	CString strIP;
//	hostent* plocalhost;
//	char *localIP;
//	plocalhost = gethostbyname("");
//	localIP = inet_ntoa(*(in_addr*)*plocalhost->h_addr_list);
//	for (size_t i = 0; i < strlen(localIP); i++)
//	{
//		strIP.Format(TEXT("%s%c"), strIP, *(localIP + i));
//	}
//
//	return strIP;
//}

void CListenSrv::SetListenPort(UINT port)
{
	m_listenport = port;
}

UINT CListenSrv::GetListenPort()
{
	return m_listenport;
}

void CListenSrv::SetHwnd(HWND hwnd)
{
	m_hwnd = hwnd;
}

HWND CListenSrv::GetHwnd()
{
	return m_hwnd;
}

bool CListenSrv::OpenListen()
{
	if (!Create(m_listenport))
		return false;

	IsListening = Listen();
	return IsListening;
}

CListenSrv::CListenSrv()
{
	m_listenport = 8083;
	m_hwnd = NULL;
	IsListening = false;
	m_AcceptedCounts = 0;
}


CListenSrv::~CListenSrv()
{
}


void CListenSrv::OnClose(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	IsListening = false;
	CAsyncSocket::OnClose(nErrorCode);
	SendMessage(m_hwnd, WM_LSTN_SRV_STATUS, 0, 0);
}


void CListenSrv::OnAccept(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	CProessSockt* accpetsocket = new CProessSockt();
	if (Accept(*accpetsocket))
	{
		m_AcceptedCounts += 1;
		SendMessage(m_hwnd, WM_LSTN_SRV_STATUS, 0, 0);
	}
	else
	{
		delete accpetsocket;
	}
	CAsyncSocket::OnAccept(nErrorCode);
}
