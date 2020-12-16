#include "pch.h"
#include "CProessSockt.h"
#include <lm.h>

CProessSockt::CProessSockt()
{
	m_hwnd = NULL;
}

CProessSockt::~CProessSockt()
{
}

void CProessSockt::SetHwnd(HWND hwnd)
{
	m_hwnd = hwnd;
}

HWND CProessSockt::GetHwnd()
{
	return m_hwnd;
}

bool CProessSockt::SendMsg(CString strMsg)
{
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
				OnClose(n_err);
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

//strUserinfo =  username + \n + oldpwd + \n + Newpwd
// return 0为未连接服务器，1为成功修改密码，2为密码不正确，3为修改失败,4为连接服务器失败
int CProessSockt::ChangeUserPwd(CString strUserinfo)
{
	CString strusername;
	CString strOldpwd;
	CString strNewpwd;
	HANDLE hToken;
	int index;

	index = strUserinfo.Find('\n');
	if (index==-1)
	{
		return 3;
	}
	strusername = strUserinfo.Left(index);
	strOldpwd = strUserinfo.Right(strUserinfo.GetAllocLength() - index - 1);
	index = strOldpwd.Find('\n');
	if (index == -1)
	{
		return 3;
	}
	strNewpwd = strOldpwd.Right(strOldpwd.GetLength() - index - 1);
	strOldpwd = strOldpwd.Left(index);

	
	BOOL boolResult = LogonUserW(
		strusername,
		L".",
		strOldpwd,
		LOGON32_LOGON_NETWORK,
		LOGON32_PROVIDER_DEFAULT,
		&hToken);
	if (boolResult)
	{
		CloseHandle(hToken);
		USER_INFO_1003 user1003;
		user1003.usri1003_password = strNewpwd.GetBuffer();
		NET_API_STATUS nStatus = NetUserSetInfo(NULL, strusername, 1003, (LPBYTE)&user1003, NULL);
		if (nStatus == NERR_Success)
		{
			return 1;
		}
		else
		{
			return 3;
		}
		//switch (nStatus)
		//{
		//case NERR_Success:
		//{
		//	CWnd::MessageBoxW(L"修改成功！", L"提示");
		//}
		//break;
		//case ERROR_ACCESS_DENIED:
		//{
		//	CWnd::MessageBoxW(L"修改失败！ERROR_ACCESS_DENIED", L"提示");
		//}
		//break;
		//case ERROR_INVALID_PARAMETER:
		//{
		//	CWnd::MessageBoxW(L"修改失败！ERROR_INVALID_PARAMETER", L"提示");
		//}
		//break;
		//case NERR_InvalidComputer:
		//{
		//	CWnd::MessageBoxW(L"修改失败！NERR_InvalidComputer", L"提示");
		//}
		//break;
		//case NERR_NotPrimary:
		//{
		//	CWnd::MessageBoxW(L"修改失败！NERR_NotPrimary", L"提示");
		//}
		//break;
		//case NERR_UserNotFound:
		//{
		//	CWnd::MessageBoxW(L"修改失败！NERR_UserNotFound", L"提示");
		//}
		//break;
		//case NERR_PasswordTooShort:
		//{
		//	CWnd::MessageBoxW(L"修改失败！NERR_PasswordTooShort", L"提示");
		//}
		//break;
		//case NERR_BadPassword:
		//{
		//	CWnd::MessageBoxW(L"修改失败！NERR_PasswordTooShort", L"提示");
		//}
		//break;
		//case NERR_LastAdmin:
		//{
		//	CWnd::MessageBoxW(L"修改失败！NERR_PasswordTooShort", L"提示");
		//}
		//break;
		//case NERR_SpeGroupOp:
		//{
		//	CWnd::MessageBoxW(L"修改失败！NERR_PasswordTooShort", L"提示");
		//}
		//break;
		//default:
		//	CWnd::MessageBoxW(L"修改失败！", L"提示");
		//	break;
		//}
	}
	else
	{
		return 2;
	}
}

void CProessSockt::OnReceive(int nErrorCode)
{
	CString revstr;
	if (nErrorCode == 0)
	{
		int ncode;

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
				Close();
			}
		}
		break;
		default:
		{
			buff[ncode] = L'\0';
			revstr = buff;
		}
		break;
		}

	}
	CAsyncSocket::OnReceive(nErrorCode);

	//返回处理结果给客户端
	if (!revstr.IsEmpty())
	{
		int errcode = ChangeUserPwd(revstr);
		switch (errcode)
		{
		case 1:
		{
			SendMsg(L"1");	
		}
		break;
		case 2:
		{
			SendMsg(L"2");
		}
		break;
		case 3:
		{
			SendMsg(L"3");
		}
		break;
	

		default:
			break;
		}
	}
}

void CProessSockt::OnClose(int nErrorCode)
{
	Close();
	//SendMessage(m_hwnd, AccpeterMsgID, FD_CLOSE, (LPARAM)lpstrname);
	CAsyncSocket::OnClose(nErrorCode);
	delete this;
}

void CProessSockt::OnSend(int nErrorCode)
{
	if (nErrorCode != 0)
	{
		OnClose(nErrorCode);
	}
	CAsyncSocket::OnSend(nErrorCode);
}
