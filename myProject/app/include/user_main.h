/*
 * user_main.h
 *
 *  Created on: 2017��4��27��
 *      Author: xuejunjun
 */

#ifndef APP_INCLUDE_USER_MAIN_H_
#define APP_INCLUDE_USER_MAIN_H_
#include "mem.h"
#include "hw_timer.h"
#include "smartconfig.h"
#include "espconn.h"
#include "udp.h"
#include "transparent_transmission.h"
#include "client.h"
#include "api_flash.h"
#include "resolve_json.h"

ETSTimer connect_timer;
ETSTimer heartbeat_timer;
ETSTimer sendInfo_timer;

extern void ICACHE_FLASH_ATTR smart_config(void);
extern void ICACHE_FLASH_ATTR uart_test_rx(void);
#endif /* APP_INCLUDE_USER_MAIN_H_ */
