#include "stdafx.h"
#include "Myfunctions.h"

//�ֽڼ����� - Ѱ���ֽڼ�
/*
    ���ø�ʽ�� �������͡� Ѱ���ֽڼ� ���ֽڼ� ����Ѱ���ֽڼ����ֽڼ� ��Ѱ�ҵ��ֽڼ����������� ��ʼ��Ѱλ�ãݣ� - ϵͳ����֧�ֿ�->�ֽڼ�����
    Ӣ�����ƣ�InBin
    ����һ�ֽڼ�����һ�ֽڼ������ȳ��ֵ�λ�ã�λ��ֵ�� 1 ��ʼ�����δ�ҵ������� -1��������Ϊ�������
    ����<1>������Ϊ������Ѱ���ֽڼ���������Ϊ���ֽڼ���bin������
    ����<2>������Ϊ����Ѱ�ҵ��ֽڼ���������Ϊ���ֽڼ���bin������
    ����<3>������Ϊ����ʼ��Ѱλ�á�������Ϊ�������ͣ�int���������Ա�ʡ�ԡ�1Ϊ��λ�ã�2Ϊ��2��λ�ã�������ƣ������ʡ�ԣ���Ѱ���ֽڼ�������Ĭ�ϴ��ײ���ʼ���������ֽڼ�������Ĭ�ϴ�β����ʼ��

*/
LIBAPI(int, krnln_InBin)
{ // �����ߺ� ��
 	PMDATA_INF pArgInf = &ArgInf;
	if(pArgInf[0].m_pBin==NULL || pArgInf[1].m_pBin==NULL)
 		return -1;

	INT nLen = pArgInf[0].m_pInt[1];
	INT nSubLen = pArgInf[1].m_pInt[1];

	register unsigned char* pSrc;
	int off;
	if(pArgInf[2].m_dtDataType == _SDT_NULL || pArgInf[2].m_int <= 1)
	{
		off = 1;
		pSrc = pArgInf[0].m_pBin + 2*sizeof(INT);
	}
	else
	{
		off = pArgInf[2].m_int;
		pSrc = pArgInf[0].m_pBin + 2*sizeof(INT) + off - 1;
		nLen -= off - 1;
	}

 	if(nLen<=0 || nSubLen<=0 || nSubLen > nLen)
		return -1;

	register unsigned char* pDes = pArgInf[1].m_pBin + 2*sizeof(INT);
	int i;

	// ���Ӵ�ֱ�ӱ�������
	switch (nSubLen)
	{
	case 1:
		i = mymemchr(pSrc, nLen, pDes[0]);
		if (i >= 0)
			return i + off;
		else
			return -1;
	case 2:
		for (i=0; i < nLen - 1; i++)
			if (((short*)(pSrc+i))[0] == ((short*)pDes)[0])
				return i + off;
		return -1;
	case 3:
		for (i=0; i < nLen - 2; i++)
			if (((short*)(pSrc+i))[0] == ((short*)pDes)[0])
				if (pSrc[i+2] == pDes[2])
					return i + off;	
		return -1;
	case 4:
		for (i=0; i < nLen - 3; i++)
			if (((int*)(pSrc+i))[0] == ((int*)pDes)[0])
				return i + off;
		return -1;
	case 5:
		for (i=0; i < nLen - 4; i++)
			if (((int*)(pSrc+i))[0] == ((int*)pDes)[0])
				if (pSrc[i+4] == pDes[4])
					return i + off;		
		return -1;
	case 6:
		for (i=0; i < nLen - 5; i++)
			if (((int*)(pSrc+i))[0] == ((int*)pDes)[0])
				if (((short*)(pSrc+i+4))[0] == ((short*)(pDes+4))[0])
					return i + off;
		return -1;
	default:  // ���Ӵ�ʹ��BM�㷨,
		int naddr = boyer_moore(pSrc, nLen, pDes, nSubLen);
		if (naddr >= 0)
			return naddr + off;
	}

// ���Ӵ�ʹ�� Sunday�㷨,
// 	unsigned int next[256];
// 	for (i=0; i < 256; i++)
// 		next[i] = nSubLen;
// 	for (i = 0; i < nSubLen; i++)
// 		next[pDes[i]] = nSubLen - i;
// 
// 	register unsigned char* naddr;
// 	for (naddr = pSrc; naddr <= pSrc + nLen - nSubLen; naddr += next[naddr[nSubLen]])
// 		if (MyMemCmp(naddr, pDes, nSubLen)==0)
// 			return naddr - pSrc + off;
		
	return -1;
}
// { // ����ԭ��
// 	PMDATA_INF pArgInf = &ArgInf;
// 	if(pArgInf[0].m_pBin==NULL || pArgInf[1].m_pBin==NULL)
// 		return -1;
// 
// 	INT nLen = pArgInf[0].m_pInt[1];
// 	INT nSubLen = pArgInf[1].m_pInt[1];
// 	if(nLen==0 || nSubLen==0 || nSubLen > nLen)return -1;
// 	
// 	INT nStart;
// 	if(pArgInf[2].m_dtDataType ==_SDT_NULL || pArgInf[2].m_int <=0)
// 		nStart = 1;
// 	else
// 		nStart = pArgInf[2].m_int;
// 
// 	INT nEnd = nLen - nSubLen;
// 	if(nStart > (nEnd+1))return -1;
// 	LPBYTE pSrc = pArgInf[0].m_pBin + 2*sizeof(INT);
// 	LPBYTE pStart = pSrc + nStart -1;
// 	LPBYTE pEnd = pSrc + nEnd;
// 	LPBYTE pObj = pArgInf[1].m_pBin + 2*sizeof(INT);
// 
// 	while(pStart <= pEnd)
// 	{
// 		if(memcmp(pStart,pObj,nSubLen)==0)
// 		{
// 			nStart = pStart - pSrc +1;
// 			return nStart;
// 		}
// 		pStart++;
// 	}
// 
// 
// 	return -1;	
// }
