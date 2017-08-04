/*
 * client_http.h
 *
 *  Created on: 2017Äê7ÔÂ13ÈÕ
 *      Author: Administrator
 */

#ifndef APP_INCLUDE_CLIENT_HTTP_H_
#define APP_INCLUDE_CLIENT_HTTP_H_

#include "user_main.h"
#include "espconn.h"
#include "mem.h"
char buffer[1024];
#define GET "GET /%s HTTP/1.1\r\nAccept: */*\r\nHost: %s\r\nConnection: Keep-Alive\r\n\r\n"
#define POST "POST /%s HTTP/1.1\r\nAccept: */*\r\nContent-Length: %d\r\nContent-Type: application/x-www-form-urlencoded\r\nHost: %s\r\nConnection: Keep-Alive\r\n\r\n%s"
struct espconn user_tcp2_conn;
void my_station2_init(struct ip_addr *remote_ip,struct ip_addr *local_ip,int remote_port);



#endif /* APP_INCLUDE_CLIENT_HTTP_H_ */
