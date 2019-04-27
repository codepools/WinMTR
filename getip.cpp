
//#include "stdafx.h"
#include<afxinet.h>

//GB2312 转换成 Unicode
wchar_t* GB2312ToUnicode(const char* szGBString)
{
	UINT nCodePage = 936; //GB2312

	int nLength = MultiByteToWideChar(nCodePage, 0, szGBString, -1, NULL, 0);

	wchar_t* pBuffer = new wchar_t[nLength + 1];

	MultiByteToWideChar(nCodePage, 0, szGBString, -1, pBuffer, nLength);

	pBuffer[nLength] = 0;

	return pBuffer;
}

CString GetNetIp()
{
	CString internetIp = _T("");
	char chSource[4096] = { 0 };
	CString strAddress;
	CInternetSession mySession(NULL, 0);
	CHttpFile* myHttpFile = NULL;

	strAddress = _T("http://2019.ip138.com/ic.asp");//ip138网页
	//strAddress = _T("http://www.apnic.net/");
	TRY
	{
		myHttpFile = (CHttpFile*)mySession.OpenURL(strAddress);
	//while(myHttpFile->Read(chSource, 4096));
	while (myHttpFile->Read(chSource, 4096))
	{
		//循环读取下载来的网页文本
		int begin = 0;
		// 目前只处理字符集为gb2312的情况
		CString sSourece(chSource);
		begin = sSourece.Find(_T("<center>"),0);
		if (begin != -1)
		{
			WCHAR* wchSource = GB2312ToUnicode(chSource);
			internetIp = wchSource;
			delete[] wchSource;
		}
		begin = internetIp.Find(_T("<center>"),0);
		if (begin != -1)//如果找到"["， 则找"]"  中括号内的文本则是 你的外网ip
		{
			int end = internetIp.Find(_T("</center>"));
			internetIp = internetIp.Mid(begin + 8,end - begin - 8);//提取外网ip
			break;
		}
	}
	myHttpFile->Close();
	mySession.Close();
	}
		CATCH(CInternetException, e)
	{

	}
	END_CATCH

		TRACE(internetIp);

	return internetIp;
}