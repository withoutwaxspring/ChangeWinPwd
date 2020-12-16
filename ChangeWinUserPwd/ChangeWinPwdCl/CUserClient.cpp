#include "pch.h"
#include "CUserClient.h"

CUserClient::CUserClient()
{
	m_hwnd = NULL;
	m_strSrvname = L"";
	m_strUsername = L"";
	m_strOldpwd = L"";
	m_strNewpwd = L"";
	m_errcode = 0;
}

CUserClient::CUserClient(HWND hWind, CString strSrvname, CString strUsername, CString strOldpwd, CString strNewpwd)
{
	m_hwnd = hWind;
	m_strSrvname = strSrvname;
	m_strUsername = strUsername;
	m_strOldpwd = strOldpwd;
	m_strNewpwd = strNewpwd;
}

CUserClient::~CUserClient()
{
}

int CUserClient::GetErrCode()
{
	return m_errcode;
}

void CUserClient::OnConnect(int nErrorCode)
{
	if (!nErrorCode == 0)
	{
		m_errcode = 4;
		SendMessage(m_hwnd, WM_SRVRESPONSE, 0, 0);
		Close();
	}
	CAsyncSocket::OnConnect(nErrorCode);
}

void CUserClient::OnReceive(int nErrorCode)
{
	if (nErrorCode == 0)
	{
		int ncode;
		CString revstr;

		TCHAR buff[4096];
		ZeroMemory(buff, sizeof(buff));
		ncode = Receive(buff, 4096);
		switch (ncode)
		{
		case 0:
		{
			Close();
		}
		break;
		case SOCKET_ERROR:
		{
			if (GetLastError() != WSAEWOULDBLOCK)
			{
				//显示错误
				m_errcode = 3;
				SendMessage(m_hwnd, WM_SRVRESPONSE,0, 0);
			}
		}
		break;
		default:
		{
			buff[ncode] = L'\0';
			SetErrcode(buff);
			SendMessage(m_hwnd, WM_SRVRESPONSE, 0, 0);
		}
		break;
		}

	}
	Close();
	CAsyncSocket::OnReceive(nErrorCode);
}

void CUserClient::OnSend(int nErrorCode)
{
	CAsyncSocket::OnSend(nErrorCode);
	if (0 == nErrorCode)
	{
		if(!SendUserinfo())
		{
			m_errcode = 4;
			SendMessage(m_hwnd, WM_SRVRESPONSE, 0, 0);
		}
	}
	else
	{
		CString msgstr;
		msgstr.Format(L"%d", nErrorCode);
		AfxMessageBox(msgstr);
		m_errcode = 4;
		SendMessage(m_hwnd, WM_SRVRESPONSE, 0, 0);
		//AfxMessageBox(L"连接服务器失败！");
		//OnClose(nErrorCode);
	}
}

void CUserClient::OnClose(int nErrorCode)
{
	//m_errcode = 4;
	//SendMessage(m_hwnd, WM_SRVRESPONSE, 0, 0);
	Close();
	CAsyncSocket::OnClose(nErrorCode);

}

void CUserClient::SetErrcode(CString strbuff)
{
	if (strbuff == L"1")
	{
		m_errcode = 1;
		return;
	}
	if (strbuff == L"2")
	{
		m_errcode = 2;
		return;
	}

	m_errcode = 3;
	return;

}

bool CUserClient::SendUserinfo()
{
	CString strMsg = m_strUsername + L"\n" + m_strOldpwd + L"\n" + m_strNewpwd;
	int nBytesSended = 0;
	int nMsgBytesLength = strMsg.GetLength() * 2;
	int n_err;
	int dwbytes;
	while (nBytesSended < nMsgBytesLength)
	{
		dwbytes = Send((LPCTSTR)strMsg + nBytesSended, nMsgBytesLength - nBytesSended);
		if (dwbytes == SOCKET_ERROR)
		{
			n_err = GetLastError();
			if (n_err == WSAEWOULDBLOCK)
			{
				break;
			}
			else
			{
				CString msgstr;
				msgstr.Format(L"%d", n_err);
				AfxMessageBox(msgstr);
				return false;
				
			}
		}
		else
		{
			nBytesSended += dwbytes;
		}
	}
	return true;
}
