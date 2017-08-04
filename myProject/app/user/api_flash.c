/*
 * api_flah.c
 *
 *  Created on: 2017年6月4日
 *      Author: Administrator
 */
#include "api_flash.h"
char dev_token[33];
char dev_ctrlkey[33];
char dev_bindkey[33];
char dev_ver[32];
LOCAL char newb[128];

/******************************************************************************
 * FunctionName : Readflash_token
 * Description  : 读取token (32个字节)
 * Parameters   : json -- A pointer to a JSON set up
 *                path -- A pointer to the JSON format tree's path
 * Returns      : A pointer to the JSON format tree
*******************************************************************************/
void ICACHE_FLASH_ATTR Readflash_token(){
    uint32 value[8];
    uint8 i;
    uint8* dev_tok = (uint8 *)&value[0];
    char  *pos_end;

    spi_flash_read((0xFC * SPI_FLASH_SEC_SIZE + ADDRESS_TOKEN),(uint32 *)dev_tok,32);



    if (*(dev_tok)==255 && *(dev_tok+1)==255&& *(dev_tok+2)==255&& *(dev_tok+3)==255) {
      for(i=0;i<33;i++){
            dev_token[i] = '\0';
        }
    }else {
        for(i=0;i<32;i++){
        	dev_token[i] = *(dev_tok+i);
        }
        dev_token[32] = '\0';
    }
    os_printf("Readflash_token:%s\r\n",dev_token);
}


/******************************************************************************
 * FunctionName : Readflash_ctrlkey
 * Description  : 读取ctrlkey (32个字节)
 * Parameters   : json -- A pointer to a JSON set up
 *                path -- A pointer to the JSON format tree's path
 * Returns      : A pointer to the JSON format tree
*******************************************************************************/
void ICACHE_FLASH_ATTR Readflash_ctrlkey(){
    uint32 value[8];
    uint8 i;
    char* dev_tok = (char *)&value[0];

    spi_flash_read((0xFC * SPI_FLASH_SEC_SIZE + ADDRESS_CTRLKEY),(uint32 *)dev_tok,32);

    for(i=0;i<32;i++){
    	dev_ctrlkey[i] = *(dev_tok+i);
    }
    dev_ctrlkey[32] = '\0';
    os_printf("Readflash_ctrlkey:%s\r\n",dev_ctrlkey);


}

/******************************************************************************
 * FunctionName : Readflash_bindkey
 * Description  : 读取bindkey (32个字节)
 * Parameters   : json -- A pointer to a JSON set up
 *                path -- A pointer to the JSON format tree's path
 * Returns      : A pointer to the JSON format tree
*******************************************************************************/
void ICACHE_FLASH_ATTR Readflash_bindkey(){
    uint32 value[8];
    uint8 i;
    char* dev_tok = (char *)&value[0];

    spi_flash_read((0xFC * SPI_FLASH_SEC_SIZE + ADDRESS_BINDKEY),(uint32 *)dev_tok,32);
    for(i=0;i<32;i++){
    	dev_bindkey[i] = *(dev_tok+i);
    }
    dev_bindkey[32] = '\0';
    os_printf("Readflash_bindkey:%s\r\n",dev_bindkey);

}


/******************************************************************************
 * FunctionName : Readflash_VER
 * Description  : 读取软件版本号 (32个字节)
 * Parameters   : json -- A pointer to a JSON set up
 *                path -- A pointer to the JSON format tree's path
 * Returns      : A pointer to the JSON format tree
*******************************************************************************/
void ICACHE_FLASH_ATTR Readflash_VER(){
    uint32 value[8];
    uint8 i;
    char* dev_tok = (char *)&value[0];

    spi_flash_read((0xFC * SPI_FLASH_SEC_SIZE + ADDRESS_VER),(uint32 *)dev_tok,32);
    //起初版本为1.1.1.1
    if (*(dev_tok)==255 && *(dev_tok+1)==255&& *(dev_tok+2)==255&& *(dev_tok+3)==255) {

    	dev_ver[0] = '1';
    	dev_ver[1] = '.';
    	dev_ver[2] = '1';
    	dev_ver[3] = '.';
    	dev_ver[4] = '1';
    	dev_ver[5] = '.';
    	dev_ver[6] = '1';
    	dev_ver[7] = '\0';
    }else {
        for(i=0;i<32;i++){
        	dev_ver[i] = *(dev_tok+i);
        }
    }


    os_printf("Readflash_VER:%s\r\n",dev_ver);
}

/******************************************************************************
 * FunctionName : writeflash_VER
 * Description  : 读取软件版本号 (32个字节)
 * Parameters   : json -- A pointer to a JSON set up
 *                path -- A pointer to the JSON format tree's path
 * Returns      : A pointer to the JSON format tree
*******************************************************************************/
void ICACHE_FLASH_ATTR writeflash_VER(char * ver){
    uint32 value[24];
    char* buffer = (char *)&value[0];

    spi_flash_read((0xFC * SPI_FLASH_SEC_SIZE + ADDRESS_TOKEN),(uint32 *)buffer,96);

     int i=0;
     for(i=0;i<96;i++){
    	 newb[i] =  *(buffer+i);
     }
     for(i=0;i<os_strlen(ver);i++){
    	 newb[96+i] = *(ver+i);
     }
      newb[96+os_strlen(ver)] = '\0';
     os_printf("要写入的newb:%s",newb);
	 spi_flash_erase_sector(0xFC);
	 spi_flash_write((0xFC*SPI_FLASH_SEC_SIZE+ ADDRESS_TOKEN),(uint32 *)&newb,os_strlen(newb));
}
