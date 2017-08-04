/*
 * client_http.c
 *
 *  Created on: 2017��7��13��
 *      Author: Administrator
 */

#include "client_http.h"
void ICACHE_FLASH_ATTR user_tcp2_sent_cb(void *arg){
	os_printf("tcp2�������ݳɹ���");
}
void ICACHE_FLASH_ATTR user_tcp2_discon_cb(void *arg){
	os_printf("tcp2�Ͽ����ӳɹ���");
}
void ICACHE_FLASH_ATTR user_tcp2_recv_cb(void *arg,
		char *pdata,
		unsigned short len){
	os_printf("tcp2�յ����ݣ�%s\r\n",pdata);
}
void ICACHE_FLASH_ATTR user_tcp2_recon_cb(void *arg, sint8 err){
	os_printf("tcp2���Ӵ��󣬴������Ϊ%d\r\n",err);
	espconn_connect((struct espconn *)arg);
}
void ICACHE_FLASH_ATTR user_tcp2_connect_cb(void *arg){
	struct espconn *pespconn=arg;
	espconn_regist_recvcb(pespconn,user_tcp2_recv_cb);
	 espconn_regist_sentcb(pespconn,user_tcp2_sent_cb);
	 espconn_regist_disconcb(pespconn,user_tcp2_discon_cb);
	 espconn_sent(pespconn,buffer,strlen(buffer));
}
void ICACHE_FLASH_ATTR my_station2_init(struct ip_addr *remote_ip,struct ip_addr *local_ip,int remote_port){
	//espconn��������
	user_tcp2_conn.type=ESPCONN_TCP;
	user_tcp2_conn.state=ESPCONN_NONE;
	user_tcp2_conn.proto.tcp=(esp_tcp *)os_zalloc(sizeof(esp_tcp));
	os_memcpy(user_tcp2_conn.proto.tcp->local_ip,local_ip,4);
	os_memcpy(user_tcp2_conn.proto.tcp->remote_ip,remote_ip,4);
	user_tcp2_conn.proto.tcp->local_port=espconn_port();
	user_tcp2_conn.proto.tcp->remote_port=remote_port;
	//ע�����ӻص������������ص�����
	os_printf("remote_ip:%d",remote_ip->addr);
	espconn_regist_connectcb(&user_tcp2_conn,user_tcp2_connect_cb);
	espconn_regist_reconcb(&user_tcp2_conn,user_tcp2_recon_cb);
	//��������
	espconn_connect(&user_tcp2_conn);
}


