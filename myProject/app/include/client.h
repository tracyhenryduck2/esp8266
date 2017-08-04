/*
 * client.h
 *
 *  Created on: 2015Äê7ÔÂ22ÈÕ
 *      Author: Administrator
 */

#ifndef APP_INCLUDE_CLIENT_H_
#define APP_INCLUDE_CLIENT_H_
#include "user_main.h"
#include "espconn.h"
#include "mem.h"
#include "resolve_json.h"
#include "my_http.h"

#define SECTOR_USER1_ADDR 0x01
#define SECTOR_USER2_ADDR 0x81
struct espconn user_tcp_conn;
extern struct espconn user_tcp_conn;
extern bool flag_revtcp;
extern char revtcp_content[500];
char binbuffer[1024];
void my_station_init(struct ip_addr *remote_ip,struct ip_addr *local_ip,int remote_port);
int pre_addr;
int bufferlength;
int recvlength;
int erase_sector;
int datalength;
bool updataflag;
char write_buffer[4096];
#endif /* APP_INCLUDE_CLIENT_H_ */
