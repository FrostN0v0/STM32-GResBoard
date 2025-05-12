#include "flash.h"
#include <string.h>
#include "usart.h"
#include "stm32l4xx_hal.h"

HAL_StatusTypeDef  HAL_FLASH_Unlock(void);//解锁
HAL_StatusTypeDef  HAL_FLASH_Lock(void);  //上锁

HAL_StatusTypeDef  HAL_FLASHEx_Erase(FLASH_EraseInitTypeDef *pEraseInit, uint32_t *PageError);//擦除的时候，调用
HAL_StatusTypeDef  HAL_FLASH_Program(uint32_t TypeProgram, uint32_t Address, uint64_t Data);//写入操作



uint64_t remote_buf[LEN],remote_buf_1[LEN];
void Flash_Test(void)
{
	int i;
	uint32_t PageError = 0;
	FLASH_EraseInitTypeDef FlashSet;
	HAL_StatusTypeDef status;
	
	uint32_t addr = 0x0803F800;
	
	uint32_t data_buf[LEN],remote_buf[LEN];
	
	/* 读取Flash内容 */
	memcpy(data_buf, (uint32_t*)addr, sizeof(uint32_t)*LEN);
	printf("read before erase:\r\n\t");//擦除前读取
	for(i = 0;i < LEN;i++)
	{
		printf("0x%08x ", data_buf[i]);
		 HAL_Delay(100);
	}
	printf("\r\n");
	 HAL_Delay(5000);
	
	/* 写入新的数据 */
	//擦除最后一页
	FlashSet.TypeErase = FLASH_TYPEERASE_PAGES;
	FlashSet.Banks = FLASH_BANK_1;
	FlashSet.Page = 127;
	FlashSet.NbPages = 1;
	//解锁Flash操作
	HAL_FLASH_Unlock();//解锁
	status = HAL_FLASHEx_Erase(&FlashSet, &PageError);
	HAL_FLASH_Lock();
	if(status != HAL_OK)
	{
		printf("erase fail, PageError = %d\r\n", PageError);
	}
	printf("erase success\r\n");//擦除成功
	 HAL_Delay(5000);
	
	/* 读取Flash内容 */
	memcpy(data_buf, (uint32_t*)addr, sizeof(uint32_t)*LEN);
	printf("read after erase:\r\n\t");//擦除后读取
	for(i = 0;i < LEN;i++)
	{
		printf("0x%08x ", data_buf[i]);
	}
	printf("\r\n");
   
	//写入Flash内容
	uint32_t data_test[LEN]={0,1,2,3,4,5,6,7,8,9},data=128;
	HAL_FLASH_Unlock();
//	for (i = 0; i < LEN * sizeof(uint32_t); i+=8)
//	{
			//一个字是32位，一次写入两个字，64位，8个字节
		status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, addr+0, (uint64_t)data);//+ i
	  status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, addr+8, (uint64_t)data_test[2]);//+ i
		status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, addr+16, (uint64_t)data_test[3]);//+ i
		status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, addr+24, (uint64_t)data_test[4]);//+ i
		status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, addr+32, (uint64_t)data_test[5]);//+ i
	
			if(status != HAL_OK)
//			{
//				break;
//			}
//	}
	HAL_FLASH_Lock();
		if(i < LEN)
	{
		printf("write fail\r\n");
	}
	else
	{
		printf("write success\r\n");
	}
	 HAL_Delay(5000);
	
	
	

	/* 读取Flash内容 */
	addr = 0x0803F800;//remote_buf[LEN]
//	remote_addr = 0x0803F900;
	memcpy(data_buf, (uint32_t*)addr, sizeof(uint32_t)*LEN);
//	memcpy(remote_buf, (uint32_t*)remote_addr, sizeof(uint32_t)*LEN);
	printf("read after write:\r\n\t");
	for(i = 0;i < LEN;i++)
	{
		printf("0x%08x ", data_buf[i]);
		  HAL_Delay(50);
	}
	for(i = 0;i < LEN;i++)
	{
		printf("0x%08x ", remote_buf[i]);
		  HAL_Delay(50);
	}
	printf("\r\n");
	 HAL_Delay(5000);
}

void Flash_Erase(void)
{
  uint32_t PageError = 0;
	HAL_StatusTypeDef status;
	FLASH_EraseInitTypeDef FlashSet;
	
	//擦除最后一页
	FlashSet.TypeErase = FLASH_TYPEERASE_PAGES;
	FlashSet.Banks = FLASH_BANK_1;
	FlashSet.Page = 127;
	FlashSet.NbPages = 1;
	//解锁Flash操作
	HAL_FLASH_Unlock();//解锁
	status = HAL_FLASHEx_Erase(&FlashSet, &PageError);
	HAL_FLASH_Lock();
}


void remote_data(uint64_t data_0,uint64_t data_1,uint64_t data_2,uint64_t data_3)//偏移地址+数据
{
	uint32_t WriteAddr = 0x0803F800;//主地址
	HAL_StatusTypeDef status;

  HAL_FLASH_Unlock();//=解锁
	status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, WriteAddr+0, (uint64_t)data_0);//存储
	status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, WriteAddr+8, (uint64_t)data_1);//存储
	status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, WriteAddr+16, (uint64_t)data_2);//存储
	status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, WriteAddr+24, (uint64_t)data_3);//存储
	if(status != HAL_OK)
	{
	
	
	}
	HAL_FLASH_Lock();//上锁
}
extern uint8_t Sleep_Time;
void remote_read()//偏移地址+数据
{	
		/* 读取Flash内容 */
  uint32_t remote_addr = 0x0803F800;//主地址
//  uint32_t remote_buf[LEN]; //数据存放数组
	int i;
	memcpy(remote_buf, (uint32_t*)remote_addr, sizeof(uint32_t)*10);
  
	
	
//	printf("read after write:\r\n\t");
//	for(i = 0;i < 10;i++)
//	{
//		printf("0x%08x ", remote_buf[i]);
//		HAL_Delay(50);
//	}
//	printf("\r\n");
  
}


/* USER CODE END 0 */

