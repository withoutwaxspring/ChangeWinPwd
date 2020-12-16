#pragma once
#include <afxsock.h>
#define WM_SRVRESPONSE WM_USER+1006
class CUserClient :
	public CAsyncSocket
{
public:
	CUserClient();
	CUserClient(HWND hWind, CString strSrvname, CString strUsername, CString strOldpwd, CString strNewpwd);
	~CUserClient();

	HWND m_hwnd;
	CString m_strSrvname;
	CString m_strUsername;
	CString m_strOldpwd;
	CString m_strNewpwd;
	int GetErrCode();
	bool SendUserinfo();


public:
	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual void OnClose(int nErrorCode);
private:
	void SetErrcode(CString strbuff);
	// 0Ϊδ���ӷ�������1Ϊ�ɹ��޸����룬2Ϊ���벻��ȷ��3Ϊ�޸�ʧ��,4Ϊ���ӷ�����ʧ��
	int m_errcode;

};

