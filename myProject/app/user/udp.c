/*
 * udp.c
 *
 *  Created on: 2017��5��25��
 *      Author: gc-0001
 */
#include "udp.h"

struct espconn user_udp_espconn;
char *udp_receive;
bool flag_revudp = false;
/******************************************************************************
 * FunctionName : user_udp_send_info
 * Description  : udp�����������ݱ��ķ���
 * Parameters   :
 * Returns      :
*******************************************************************************/
void ICACHE_FLASH_ATTR user_udp_send_info(void){

	char *ds;
	ds = getJsonTree();
	espconn_sent(&user_udp_espconn,ds,os_strlen(ds));
	os_free(ds);
}

/******************************************************************************
 * FunctionName : user_udp_send_resp
 * Description  : udp��ӦappsendResp
 * Parameters   :
 * Returns      :
*******************************************************************************/
void ICACHE_FLASH_ATTR user_udp_send_resp(void){

	char *ds;
	ds = getAppResponse();
	espconn_sent(&user_udp_espconn,ds,os_strlen(ds));
	os_free(ds);
}

/******************************************************************************
 * FunctionName : user_udp_send_mac
 * Description  : udp���ַ���mac
 * Parameters   :
 * Returns      :
*******************************************************************************/
void ICACHE_FLASH_ATTR user_udp_send_mac(void){
char DeviceBuffer[40]={0};
char hwaddr[6];
wifi_get_macaddr(STATION_IF,hwaddr);
os_sprintf(DeviceBuffer,"ESP_"MACSTR"\r\n",MAC2STR(hwaddr));
espconn_sent(&user_udp_espconn,DeviceBuffer,os_strlen(DeviceBuffer));
}

/******************************************************************************
 * FunctionName : user_udp_sent_cb
 * Description  : udp������ɻص�����
 * Parameters   :
 * Returns      :
*******************************************************************************/
void ICACHE_FLASH_ATTR user_udp_sent_cb(void *arg){
  os_printf("������ɻص�");
}

/******************************************************************************
 * FunctionName : user_udp_sent_cb
 * Description  : udp���ջص�����
 * Parameters   :
 * Returns      :
*******************************************************************************/
void ICACHE_FLASH_ATTR user_udp_recv_cb(void *arg,
char *pdata,
unsigned short len){
 flag_revudp = true;
 udp_receive = (char*)os_malloc(sizeof(char)*len);
 //os_free(udp_receive);
 //os_memcpy(udp_receive,pdata,len);
 int i=0;
 for(i=0;i<len;i++){
	 *(udp_receive+i) = *(pdata+i);
 }
 *(udp_receive+i) = '\0';
 os_printf("udp�Ѿ��������ݣ�%s",udp_receive);
 //setJsonTree(pdata);

}
