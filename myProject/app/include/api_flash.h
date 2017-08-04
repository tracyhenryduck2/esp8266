/*
 * api_flash.h
 *
 *  Created on: 2017Äê6ÔÂ4ÈÕ
 *      Author: Administrator
 */

#ifndef APP_INCLUDE_API_FLASH_H_
#define APP_INCLUDE_API_FLASH_H_

#include "user_interface.h"
#include "spi_flash.h"
#include "osapi.h"
#include "mem.h"

#define ADDRESS_TOKEN 0
#define ADDRESS_CTRLKEY 32
#define ADDRESS_BINDKEY 64
#define ADDRESS_VER     96

extern char dev_token[33];
extern char dev_ctrlkey[33];
extern char dev_bindkey[33];
extern char dev_ver[32];
extern void ICACHE_FLASH_ATTR Readflash_token();
extern void ICACHE_FLASH_ATTR Readflash_ctrlkey();
extern void ICACHE_FLASH_ATTR Readflash_bindkey();
extern void ICACHE_FLASH_ATTR writeflash_VER(char * ver);
#endif /* APP_INCLUDE_API_FLASH_H_ */
