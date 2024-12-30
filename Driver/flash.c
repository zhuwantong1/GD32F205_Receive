#include "flash.h"
#include "usart.h"

// ��ȡָ����ַ�İ���(16λ����)
// faddr:����ַ(�˵�ַ����Ϊ2�ı���!!)
// ����ֵ:��Ӧ����.
uint16_t GDFLASH_ReadHalfWord(uint32_t faddr)
{
	return *(volatile uint16_t *)faddr;
}

#if GD32_FLASH_WREN // ���ʹ����д
// ������д��
// WriteAddr:��ʼ��ַ
// pBuffer:����ָ��
// NumToWrite:����(16λ)��
void GDFLASH_Write_NoCheck(uint32_t WriteAddr, uint16_t *pBuffer, uint16_t NumToWrite)
{
	uint16_t i;
	for (i = 0; i < NumToWrite; i++)
	{
		fmc_halfword_program(WriteAddr, pBuffer[i]);
		WriteAddr += 2; // ��ַ����2.
	}
}
// ��ָ����ַ��ʼд��ָ�����ȵ�����
// WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)
// pBuffer:����ָ��
// NumToWrite:����(16λ)��(����Ҫд���16λ���ݵĸ���.)
#if GD32_FLASH_SIZE < 256
#define GD_SECTOR_SIZE 1024 // �ֽ�
#else
#define GD_SECTOR_SIZE 2048
#endif
uint16_t GDFLASH_BUF[GD_SECTOR_SIZE / 2]; // �����2K�ֽ�
void GDFLASH_Write(uint32_t WriteAddr, uint16_t *pBuffer, uint16_t NumToWrite)
{
	uint32_t secpos;	// ������ַ
	uint16_t secoff;	// ������ƫ�Ƶ�ַ(16λ�ּ���)
	uint16_t secremain; // ������ʣ���ַ(16λ�ּ���)
	uint16_t i;
	uint32_t offaddr; // ȥ��0X08000000��ĵ�ַ
	if (WriteAddr < GD32_FLASH_BASE || (WriteAddr >= (GD32_FLASH_BASE + 1024 * GD32_FLASH_SIZE)))
		return;								  // �Ƿ���ַ
	fmc_unlock();							  // ����
	offaddr = WriteAddr - GD32_FLASH_BASE;	  // ʵ��ƫ�Ƶ�ַ.
	secpos = offaddr / GD_SECTOR_SIZE;		  // ������ַ  0~127 for GD32F103RBT6
	secoff = (offaddr % GD_SECTOR_SIZE) / 2; // �������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
	secremain = GD_SECTOR_SIZE / 2 - secoff; // ����ʣ��ռ��С
	if (NumToWrite <= secremain)
		secremain = NumToWrite; // �����ڸ�������Χ
	while (1)
	{
		// secpos*GD_SECTOR_SIZE+GD32_FLASH_BASE
		// ������������������
		GDFLASH_Read(secpos * GD_SECTOR_SIZE + GD32_FLASH_BASE, GDFLASH_BUF, GD_SECTOR_SIZE / 2);
		 
		for (i = 0; i < secremain; i++)																   // У������
		{
			if (GDFLASH_BUF[secoff + i] != 0XFFFF)
				break; // ��Ҫ����
		}
		if (i < secremain) // ��Ҫ����
		{
			fmc_page_erase(secpos * GD_SECTOR_SIZE + GD32_FLASH_BASE); // �����������
			for (i = 0; i < secremain; i++)								 // ����
			{
				GDFLASH_BUF[i + secoff] = pBuffer[i];
			}
			GDFLASH_Write_NoCheck(secpos * GD_SECTOR_SIZE + GD32_FLASH_BASE, GDFLASH_BUF, GD_SECTOR_SIZE / 2); // д����������
		}
		else
			GDFLASH_Write_NoCheck(WriteAddr, pBuffer, secremain); // д�Ѿ������˵�,ֱ��д������ʣ������.
		if (NumToWrite == secremain)
			break; // д�������
		else	   // д��δ����
		{
			secpos++;				 // ������ַ��1
			secoff = 0;				 // ƫ��λ��Ϊ0
			pBuffer += secremain;	 // ָ��ƫ��
			WriteAddr += secremain;	 // д��ַƫ��
			NumToWrite -= secremain; // �ֽ�(16λ)���ݼ�
			if (NumToWrite > (GD_SECTOR_SIZE / 2))
				secremain = GD_SECTOR_SIZE / 2; // ��һ����������д����
			else
				secremain = NumToWrite; // ��һ����������д����
		}
	};
	fmc_lock(); // ����
}
#endif

// ��ָ����ַ��ʼ����ָ�����ȵ�����
// ReadAddr:��ʼ��ַ
// pBuffer:����ָ��
// NumToWrite:����(16λ)��
void GDFLASH_Read(uint32_t ReadAddr, uint16_t *pBuffer, uint16_t NumToRead)
{
	uint16_t i;
	for (i = 0; i < NumToRead; i++)
	{
		pBuffer[i] = GDFLASH_ReadHalfWord(ReadAddr); // ��ȡ2���ֽ�.
		ReadAddr += 2;								  // ƫ��2���ֽ�.
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// WriteAddr:��ʼ��ַ
// WriteData:Ҫд�������
void Test_Write(uint32_t WriteAddr, uint16_t WriteData)
{
	GDFLASH_Write(WriteAddr, &WriteData, 1); // д��һ����
}

