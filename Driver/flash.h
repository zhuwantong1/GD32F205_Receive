#ifndef __GDFLASH_H__
#define __GDFLASH_H__
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////
//�û������Լ�����Ҫ����
#define GD32_FLASH_SIZE 256 	 					//��ѡGD32��FLASH������С(��λΪK)
#define GD32_FLASH_WREN 1              	//ʹ��FLASHд��(0����ʹ��;1��ʹ��)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH��ʼ��ַ
#define GD32_FLASH_BASE 0x08000000 			//GD32 FLASH����ʼ��ַ
//FLASH������ֵ
 
void GDFLASH_Write_NoCheck(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite);   
uint16_t GDFLASH_ReadHalfWord(uint32_t faddr);		  												//��������  
void GDFLASH_WriteLenByte(uint32_t WriteAddr,uint32_t DataToWrite,uint16_t Len);	//ָ����ַ��ʼд��ָ�����ȵ�����
uint32_t GDFLASH_ReadLenByte(uint32_t ReadAddr,uint16_t Len);										//ָ����ַ��ʼ��ȡָ����������
void GDFLASH_Write(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite);		//��ָ����ַ��ʼд��ָ�����ȵ�����
void GDFLASH_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead);   		//��ָ����ַ��ʼ����ָ�����ȵ�����

//����д��
void Test_Write(uint32_t WriteAddr,uint16_t WriteData);								   
#endif

















