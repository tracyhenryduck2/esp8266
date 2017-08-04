/*
 * udp.h
 *
 *  Created on: 2017Äê5ÔÂ25ÈÕ
 *      Author: gc-0001
 */

#ifndef APP_INCLUDE_UDP_H_
#define APP_INCLUDE_UDP_H_

#include "mem.h"
#include "espconn.h"
#include "user_interface.h"
#include "osapi.h"
#include "my_json.h"
#include "resolve_json.h"


extern struct espconn user_udp_espconn;
extern char *udp_receive;
extern bool flag_revudp;

extern void ICACHE_FLASH_ATTR user_udp_send_info(void);
extern void ICACHE_FLASH_ATTR user_udp_send_resp(void);

extern void ICACHE_FLASH_ATTR user_udp_send_mac(void);
extern void ICACHE_FLASH_ATTR user_udp_sent_cb(void *arg);
extern void ICACHE_FLASH_ATTR user_udp_recv_cb(void *arg,char *pdata,unsigned short len);
#endif /* APP_INCLUDE_UDP_H_ */
