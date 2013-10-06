// �ļ��� ���� ������ 2011��
// huipeng_lhp@163.com �޸��� 2013-09-18


#include "StdAfx.h"
#include ".\crc32.h"
#include <string.h>
#include <assert.h>


CCrc32::CCrc32(void)
{
	InitCrc32Table();
}

CCrc32::~CCrc32(void)
{
}

//////////////////////////////////////////////////////////////////////////
//  CCrc32::Reflect
//		��ref���ݵ�ÿ��λ�ߵ�����λ����λ����λ����λ  
//
//	Parameters:
//		ref [in]    Ҫ�ߵ�������
//		ch  [in]	Ҫ�ƶ���λ��
//
//	Return:
//		�ߵ��������
//////////////////////////////////////////////////////////////////////////
unsigned long CCrc32::Reflect( unsigned long ref, char ch )
{
	unsigned long nValue = 0;

	// Swap bit 0 for bit 7
	// bit 1 for bit 6, etc.
	for(int i = 1; i < (ch + 1); i++)
	{
		if (ref & 1)
			nValue |= 1 << (ch - i);
		ref >>= 1;
	}

	return nValue;
}

//////////////////////////////////////////////////////////////////////////
//	CCrc32::InitCrc32Table
//		��ʼ��Crc32�������ѯ��
//////////////////////////////////////////////////////////////////////////
void CCrc32::InitCrc32Table()
{
	memset(m_Crc32Table,0,sizeof(m_Crc32Table));

	//CRC-32����ʽ(��WinRARʹ�õ�ͬһ�ֱ�׼)
	unsigned long ulPolynomial = 0x04C11DB7;

	// 256 values representing ASCII character codes.
	for (int i = 0; i <= 0xFF; i++)
	{
		m_Crc32Table[i] = Reflect(i, 8) << 24;

		for (int j = 0; j < 8; j++)
			m_Crc32Table[i] = (m_Crc32Table[i] << 1) ^ (m_Crc32Table[i] & (1 << 31) ? ulPolynomial : 0);

		m_Crc32Table[i] = Reflect(m_Crc32Table[i], 32);
	}
}

//////////////////////////////////////////////////////////////////////////
//	CCrc32::CalculateCrc32
//		����crc32
//
//	Parameters
//		pbuffer	[in]	Ҫ����Crc32������ָ��
//		size	[in]	pbuffer�Ĵ�С
//
//	Return
//		�����Crc32ֵ
//////////////////////////////////////////////////////////////////////////
unsigned long CCrc32::CalculateCrc32( const void* pbuffer, unsigned long size )
{
	assert(pbuffer);
	assert(size>0);

	const unsigned char* pbyte = (const unsigned char*)pbuffer;
	unsigned long Crc32 = 0xFFFFFFFF;
	while (size--)
		Crc32 = (Crc32 >> 8) ^ m_Crc32Table[(Crc32 & 0xFF) ^ *pbyte++];
	Crc32 ^= 0xFFFFFFFF;

	return Crc32;
}
