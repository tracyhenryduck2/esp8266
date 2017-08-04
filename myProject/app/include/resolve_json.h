/*
 * resolve_json.h
 *
 *  Created on: 2017Äê5ÔÂ31ÈÕ
 *      Author: gc-0001
 */

#ifndef APP_INCLUDE_RESOLVE_JSON_H_
#define APP_INCLUDE_RESOLVE_JSON_H_

#include "user_json.h"
#include "ets_sys.h"
#include "os_type.h"
#include "osapi.h"
#include "mem.h"
#include "user_interface.h"
#include "transparent_transmission.h"
#include "api_flash.h"
extern char upgradebinUrl[100];
extern char binVer[32];
extern char recvtcpAppImei[80];
extern char recvtcpAppRaw[64];
extern uint32 length_of_bin;
extern char* ICACHE_FLASH_ATTR getLogin(void);
extern char* ICACHE_FLASH_ATTR getAppResponse(void);
extern char* ICACHE_FLASH_ATTR getHeartBeat(void);
extern char* ICACHE_FLASH_ATTR getReportDevInfo(void);
int ICACHE_FLASH_ATTR _count_string(char *data, char *key);
char ICACHE_FLASH_ATTR *malloc_replace(char *data, char *rep, char *to, int free_data);
void ICACHE_FLASH_ATTR normal_replace(char *data, char *rep, char *to);
#endif /* APP_INCLUDE_RESOLVE_JSON_H_ */
