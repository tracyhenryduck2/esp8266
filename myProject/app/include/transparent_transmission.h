/*
 * transparent_transmission.h
 *
 *  Created on: 2017年4月30日
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
	ERR_OK = 0,             //正常
    ERR_OPERATION = 1,		//操作错误
	ERR_CHECKSUM = 2,	//校验码错误
	ERR_OUT_BANGDE=3,//数据超出范围
    ERR_COMMAND = 4,	//命令错误
	ERR_DATA_NOT_IN_PROTOCOL = 5, //数据不是协议规定的
	ERR_HEADER = 6   //帧头部错误
}ERR_TYPE;

typedef enum{
	REPORT_FRAME = 1, //上报服务器帧
	REACT_FRAME = 2,  //下发应答帧
	OPERATION_FRAME = 0xfe, //设备操作查询帧
	ERR_FRAME = 0xff  //设备错误帧

}FRAM_TRYPE;


typedef enum{
	QUERY_STATUS = 1,  //模块状态查询
    RESTART_WIFI = 2,  //模块软启动
    SMART_CONFIG = 3,  //进入配网模式
    SLEEP_WIFI   = 5,  //模块进入休眠模式
	AWAKE_WIFI   = 6,  //模块睡眠唤醒
    QUERY_VERSION = 10, //模块版本查询

}OPERATION_DEIVCE;

typedef enum{
	SEND_TCP_UDP_IDLE = 0, //空闲
	SEND_TCP_UDP_DEFINE1   = 1, //自定义上报数据格式1
	SEND_TCP_UDP_DEFINE2   = 2, //自定义上报数据格式2
	SEND_TCP_UDP_APPSENDRESP1 = 101,  //回发数据id=101
	SEND_TCP_UDP_APPSENDRESP2 = 102,  //回发数据id=102
}UDP_DATA_STATUS;

uint8 calcrc_1byte(uint8 abyte);
uint8 calcrc_bytes(uint8 *p,uint8 len);
extern void deviceToWifi(uint8 *input,uint8 length);
void DeviceOperation(uint8 comand);
static uint8  _Txbuf[128];

extern UDP_DATA_STATUS    udpDataStatus;
extern uint8 data1; //有效数据1
extern uint8 data2; //有效数据2
extern sc_status wifi_mode;  //WiFi模块的工作方式，0代表正常模式，非0代表配网模式中
extern uint8 wifi_connect_status;  //WIFI模块的链接状态;
extern char length_data[200];  //串口的数据长度

#endif /* APP_INCLUDE_TRANSPARENT_TRANSMISSION_H_ */
