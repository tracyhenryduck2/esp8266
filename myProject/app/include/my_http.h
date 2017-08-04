/*
 * my_http.h
 *
 *  Created on: 2015Äê7ÔÂ24ÈÕ
 *      Author: Administrator
 */

#ifndef APP_INCLUDE_MY_HTTP_H_
#define APP_INCLUDE_MY_HTTP_H_
#include "c_types.h"
#include "client_http.h"
#include "client.h"

void HttpReadFile(char *URL,char *method,char *postdata);
void ICACHE_FLASH_ATTR HttpReadBinUrl(char *URL,char *method,char *postdata);

extern void ICACHE_FLASH_ATTR HttpReadBinUrl(char *URL,char *method,char *postdata);
char host[32];
char filename[128];
unsigned short port;



#endif /* APP_INCLUDE_MY_HTTP_H_ */
