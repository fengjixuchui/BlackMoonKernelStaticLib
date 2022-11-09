#include "stdafx.h"

// ���ø�ʽ�� ���ı��͡� UTF8���ı� ���ֽڼ� ��ת����UTF8�ı����ݣ� - ϵͳ����֧�ֿ�->�ı�����
// Ӣ�����ƣ�UTF8ToStr
// ����ָ��UTF8�ı�����ת����ͨ���ı��󷵻ء�������Ϊ�������
// ����<1>������Ϊ����ת����UTF8�ı����ݡ�������Ϊ���ֽڼ���bin�������ṩ��ת����ͨ���ı���UTF8�ı����ݡ�
// 
// ����ϵͳ���� Windows��Linux
LIBAPI(char*, krnln_UTF8ToStr)
{
	char* pSrc;
	char* pRet = NULL;
	int nUnicodeLen, nAnsiLen;
	WCHAR *unicodetext;

	if (ArgInf.m_pBin == NULL || ArgInf.m_pInt[1] <= 0) 
		return NULL;

	pSrc = (char*)ArgInf.m_pBin;
	pSrc += 2*sizeof(INT);

	//�ȴ�UTF-8ת��UNICODE
	nUnicodeLen = MultiByteToWideChar(CP_UTF8, 0, pSrc, ArgInf.m_pInt[1], NULL, 0);
	if (nUnicodeLen <= 0) 
		return NULL;

	unicodetext = new WCHAR[nUnicodeLen + 1];
	MultiByteToWideChar(CP_UTF8, 0, pSrc, ArgInf.m_pInt[1], unicodetext, nUnicodeLen);
	unicodetext[nUnicodeLen] = '\0';

	//����UNICDOEת��ANSI
	nAnsiLen = WideCharToMultiByte(936, 0, unicodetext, -1, NULL, 0, NULL, NULL);
	if (nAnsiLen > 0)
	{
		pRet = (char*)E_MAlloc_Nzero(nAnsiLen); //nAnsiLen��������ֹ
		if (pRet)
			WideCharToMultiByte(936, 0, unicodetext, -1, pRet, nAnsiLen, NULL, NULL);
	}
	delete []unicodetext;
	return pRet;
}

//    ���ø�ʽ�� ���ı��͡� UTF16���ı� ���ֽڼ� ��ת����UTF16�ı����ݣ� - ϵͳ����֧�ֿ�->�ı�����
//    Ӣ�����ƣ�UTF16ToStr
//    ����ָ��UTF16�ı�����ת����ͨ���ı��󷵻ء�������Ϊ�������
//    ����<1>������Ϊ����ת����UTF16�ı����ݡ�������Ϊ���ֽڼ���bin�������ṩ��ת����ͨ���ı���UTF16�ı����ݡ�
//
//    ����ϵͳ���� Windows
LIBAPI(char*, krnln_fnUTF16ToStr)
{
	char* pSrc;
	int nl, al;
	WCHAR *unicodetext;
	
	if(ArgInf.m_pBin == NULL || ArgInf.m_pInt[1] <= 0)
		return NULL;
	
	pSrc = (char*)ArgInf.m_pBin;
	pSrc += 2*sizeof(INT);
	
	//�ȴ�UTF-16ת��UTF-8
	nl = WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)pSrc, ArgInf.m_pInt[1], NULL, 0, NULL, NULL);
	if (nl <= 0) return NULL;

	char* pszUtf8 = new char[nl + 1];
	nl = WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)pSrc, ArgInf.m_pInt[1], pszUtf8, nl, NULL, NULL);
	pszUtf8[nl] = '\0';

	pSrc = pszUtf8;

	//��UTF-8ת��UNICODE
	nl = MultiByteToWideChar(CP_UTF8, 0, pSrc, -1, NULL, 0);
	if (nl <= 0) {
		delete []pszUtf8;
		return NULL;
	}
	
	unicodetext = new WCHAR[nl];
	nl = MultiByteToWideChar(CP_UTF8, 0, pSrc, -1, unicodetext, nl);
	if (0 >= nl)
	{
		delete []pszUtf8;
		delete []unicodetext;
		return NULL;
	}
	
	//����UNICDOEת��ANSI
	al = WideCharToMultiByte(936, 0, unicodetext, -1, NULL, 0, NULL, NULL);
	pSrc = NULL;
	if (al > 0)
	{
		pSrc = (char*)E_MAlloc_Nzero(al);
		if (pSrc)
			WideCharToMultiByte(936, 0, unicodetext, -1, pSrc, al, NULL, NULL);
	}
	delete []pszUtf8;
	delete []unicodetext;
	return pSrc;
}
