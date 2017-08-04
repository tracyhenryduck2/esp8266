/*
 * transparent_transmission.h
 *
 *  Created on: 2017��4��30��
 *      Author: Administrator
 */

#ifndef APP_INCLUDE_TRANSPARENT_TRANSMISSION_H_
#define APP_INCLUDE_TRANSPARENT_TRANSMISSION_H_
#include "c_types.h"
#include "driver/uart.h"
#include "user_interface.h"
#include "osapi.h"
#include "udp.h"
#include "smartconfig.h"
#include "client.h"
#include "resolve_json.h"

#define MAX_DATA_LENGTH  0x80



typedef enum{
	ERR_OK = 0,             //����
    ERR_OPERATION = 1,		//��������
	ERR_CHECKSUM = 2,	//У�������
	ERR_OUT_BANGDE=3,//���ݳ�����Χ
    ERR_COMMAND = 4,	//�������
	ERR_DATA_NOT_IN_PROTOCOL = 5, //���ݲ���Э��涨��
	ERR_HEADER = 6   //֡ͷ������
}ERR_TYPE;

typedef enum{
	REPORT_FRAME = 1, //�ϱ�������֡
	REACT_FRAME = 2,  //�·�Ӧ��֡
	OPERATION_FRAME = 0xfe, //�豸������ѯ֡
	ERR_FRAME = 0xff  //�豸����֡

}FRAM_TRYPE;


typedef enum{
	QUERY_STATUS = 1,  //ģ��״̬��ѯ
    RESTART_WIFI = 2,  //ģ��������
    SMART_CONFIG = 3,  //��������ģʽ
    SLEEP_WIFI   = 5,  //ģ���������ģʽ
	AWAKE_WIFI   = 6,  //ģ��˯�߻���
    QUERY_VERSION = 10, //ģ��汾��ѯ

}OPERATION_DEIVCE;

typedef enum{
	SEND_TCP_UDP_IDLE = 0, //����
	SEND_TCP_UDP_DEFINE1   = 1, //�Զ����ϱ����ݸ�ʽ1
	SEND_TCP_UDP_DEFINE2   = 2, //�Զ����ϱ����ݸ�ʽ2
	SEND_TCP_UDP_APPSENDRESP1 = 101,  //�ط�����id=101
	SEND_TCP_UDP_APPSENDRESP2 = 102,  //�ط�����id=102
}UDP_DATA_STATUS;

uint8 calcrc_1byte(uint8 abyte);
uint8 calcrc_bytes(uint8 *p,uint8 len);
extern void deviceToWifi(uint8 *input,uint8 length);
void DeviceOperation(uint8 comand);
static uint8  _Txbuf[128];

extern UDP_DATA_STATUS    udpDataStatus;
extern uint8 data1; //��Ч����1
extern uint8 data2; //��Ч����2
extern sc_status wifi_mode;  //WiFiģ��Ĺ�����ʽ��0��������ģʽ����0��������ģʽ��
extern uint8 wifi_connect_status;  //WIFIģ�������״̬;
extern char length_data[200];  //���ڵ����ݳ���

#endif /* APP_INCLUDE_TRANSPARENT_TRANSMISSION_H_ */
