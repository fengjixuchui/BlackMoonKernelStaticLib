#include "stdafx.h"

// ���ø�ʽ�� ���ֽڼ��� �ı���UTF8 ���ı��� ��ת�����ı��� - ϵͳ����֧�ֿ�->�ı�����
// Ӣ�����ƣ�StrToUTF8
// ����ָ���ı�ת����UTF8��ʽ�󷵻�,ע��������UTF8�ı����ݰ����������ַ�.������Ϊ�������
// ����<1>������Ϊ����ת�����ı���������Ϊ���ı��ͣ�text�������ṩ��ת����UTF8��ʽ���ı���
// 
// ����ϵͳ���� Windows��Linux
LIBAPI(void*, krnln_StrToUTF8)
{
	PMDATA_INF pArgInf = &ArgInf;
	char* pSrc = pArgInf[0].m_pText;
	if (!pSrc || !*pSrc) return NULL;

	//�Ƚ�ASCIIת��UNICODE
	int nNum = MultiByteToWideChar(CP_ACP, 0, pSrc, -1, NULL, 0);
	if (nNum <=0) return NULL;

	WCHAR* wcsUnicode = new WCHAR[nNum];
	nNum = MultiByteToWideChar(CP_ACP, 0, pSrc, -1, wcsUnicode, nNum);
	if (nNum <= 0)
	{
		delete []wcsUnicode;
		return NULL;
	}
	wcsUnicode[nNum - 1] = 0;

	//����UNICODEתUTF8
	nNum = WideCharToMultiByte(CP_UTF8, 0, wcsUnicode, -1, NULL, 0, NULL, NULL);
	pSrc = NULL;
	if (nNum > 0)
	{
		pSrc = (char*)E_MAlloc_Nzero(nNum + 2*sizeof(int));
		if (pSrc)
		{
			*(LPINT)pSrc = 1;
			char* pDes = pSrc + 2*sizeof(int);
			nNum = WideCharToMultiByte(CP_UTF8, 0, wcsUnicode, -1, pDes, nNum, NULL, NULL);
			*(LPINT)(pSrc + sizeof(int)) = nNum - 1; // ȥ����β��'\0' �������Ա���һ��
		}
	}
	delete []wcsUnicode;
	return pSrc;
}

//    ���ø�ʽ�� ���ֽڼ��� �ı���UTF16 ���ı��� ��ת�����ı��� - ϵͳ����֧�ֿ�->�ı�����
//    Ӣ�����ƣ�StrToUTF16
//    ����ָ���ı�ת����UTF16��ʽ�󷵻�,ע��������UTF16�ı����ݰ����������ַ�.������Ϊ�������
//    ����<1>������Ϊ����ת�����ı���������Ϊ���ı��ͣ�text�������ṩ��ת����UTF16��ʽ���ı���
//
//    ����ϵͳ���� Windows
LIBAPI(void*, krnln_fnStrToUTF16)
{
	PMDATA_INF pArgInf = &ArgInf;
	char* pSrc = pArgInf[0].m_pText;
	if (!pSrc || !*pSrc) return NULL;
	
	//�Ƚ�ASCIIת��UNICODE
	int nNum = MultiByteToWideChar(CP_ACP, 0, pSrc, -1, NULL, 0);
	if (nNum <=0) return NULL;
	
	WCHAR* wcsUnicode = new WCHAR[nNum];
	nNum = MultiByteToWideChar(CP_ACP, 0, pSrc, -1, wcsUnicode, nNum);
	if (nNum <= 0)
	{
		delete []wcsUnicode;
		return NULL;
	}
	wcsUnicode[nNum - 1] = 0;
	
	//����UNICODEתUTF8
	nNum = WideCharToMultiByte(CP_UTF8, 0, wcsUnicode, -1, NULL, 0, NULL, NULL);
	if (nNum <= 0)
	{
		delete []wcsUnicode;
		return NULL;
	}

	char* pszUtf8 = new char[nNum];
	nNum = WideCharToMultiByte(CP_UTF8, 0, wcsUnicode, -1, pszUtf8, nNum, NULL, NULL);

	//����UTF8תUTF16
	nNum = MultiByteToWideChar(CP_UTF8, 0, pszUtf8, -1, NULL, 0);
	if (nNum > 0)
	{
		pSrc = (char*)E_MAlloc_Nzero(nNum * 2 + 2*sizeof(int));
		if (pSrc)
		{
			*(LPINT)pSrc = 1;
			char* pDes = pSrc + 2*sizeof(int);
			nNum = MultiByteToWideChar(CP_UTF8, 0, pszUtf8, -1, (LPWSTR)pDes, nNum);
			*(LPINT)(pSrc + sizeof(int)) = nNum * 2; // ������β��'\0' �������Ա���һ��
		}
	}
	delete []wcsUnicode;
	delete []pszUtf8;
	return pSrc;
}