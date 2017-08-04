/*
 * user_main.c
 *
 *  Created on: 2015年7月13日
 *      Author: Administrator
 */
#include "driver/uart.h"
#include "user_interface.h"
#include "osapi.h"
#include "user_main.h"
#include "my_json.h"

    char str[96];
static uint8 count=0;
//static uint8 count_sntp = 0;
//ETSTimer sntp_timer;

//void ICACHE_FLASH_ATTR sntp_read_timer_callback(void *arg){
//
//os_timer_disarm(&sntp_timer);
//uint32 current_stamp;
//current_stamp = sntp_get_current_timestamp();
//os_printf("sntp: %d, %s \n",current_stamp, sntp_get_real_time(current_stamp));
//count_sntp++;
//if(current_stamp != 0){
//	os_free(current_stamp);
//	return;
//}else{
//	if(count_sntp>=0x7){
//		count_sntp = 0;
//		os_printf("sntp获取结束");
//	return;
//	}
//
//}
//os_timer_arm(&sntp_timer,2000,NULL);
//}

void ICACHE_FLASH_ATTR HeartBeat(void *arg){
char *ds;
os_timer_disarm(&heartbeat_timer);


  ds =getHeartBeat();
  os_printf("heartbeat:%s",ds);
  espconn_sent(&user_tcp_conn,ds,strlen(ds));
  os_free(ds);
  os_timer_arm(&heartbeat_timer,10000,NULL);





  Readflash_token();

  Readflash_ctrlkey();

  Readflash_bindkey();
}



void ICACHE_FLASH_ATTR Wifi_conned(void *arg){
uint8 status;
os_timer_disarm(&connect_timer);
count++;
status=wifi_station_get_connect_status();
if(status==STATION_GOT_IP){
os_printf("Wifi connect success!");
wifi_set_broadcast_if(STATIONAP_MODE);
user_udp_espconn.type=ESPCONN_UDP;
user_udp_espconn.proto.udp=(esp_udp *)os_zalloc(sizeof(esp_udp));
user_udp_espconn.proto.udp->local_port=2525;
user_udp_espconn.proto.udp->remote_port=1112;
const char udp_remote_ip[4]={255,255,255,255};
os_memcpy(user_udp_espconn.proto.udp->remote_ip,udp_remote_ip,4);
espconn_regist_recvcb(&user_udp_espconn,user_udp_recv_cb);
espconn_regist_sentcb(&user_udp_espconn,user_udp_sent_cb);
espconn_create(&user_udp_espconn);


//os_timer_setfn(&sntp_timer,sntp_read_timer_callback,NULL);
//os_timer_arm(&sntp_timer,2000,NULL);

struct ip_info info;
const char remote_ip[4]={106,75,138,150};//hekr服务器
wifi_get_ip_info(STATION_IF,&info);
my_station_init((struct ip_addr *)remote_ip,&info.ip,83);
os_timer_setfn(&heartbeat_timer,HeartBeat,NULL);
os_timer_arm(&heartbeat_timer,2000,NULL);
return;
}else{
if(count>=0x7){
	count = 0;
os_printf("Wifi connect fail！");
return;
}
}

os_timer_arm(&connect_timer,2000,NULL);
}


void ICACHE_FLASH_ATTR
smartconfig_done(sc_status status, void *pdata)
{
    switch(status) {
        case SC_STATUS_WAIT:
            os_printf("SC_STATUS_WAIT\n");
            break;
        case SC_STATUS_FIND_CHANNEL:
        	wifi_mode = SC_STATUS_GETTING_SSID_PSWD;
            os_printf("SC_STATUS_FIND_CHANNEL\n");
            break;
        case SC_STATUS_GETTING_SSID_PSWD:
        	wifi_mode = SC_STATUS_GETTING_SSID_PSWD;
            os_printf("SC_STATUS_GETTING_SSID_PSWD\n");
			sc_type *type = pdata;
            if (*type == SC_TYPE_ESPTOUCH) {
                os_printf("SC_TYPE:SC_TYPE_ESPTOUCH\n");
            } else {
                os_printf("SC_TYPE:SC_TYPE_AIRKISS\n");
            }
            break;
        case SC_STATUS_LINK:
        	wifi_mode = SC_STATUS_LINK;
            os_printf("SC_STATUS_LINK\n");
            struct station_config *sta_conf = pdata;
            wifi_set_opmode(0x03);
	        wifi_station_set_config(sta_conf);
	        wifi_station_disconnect();
	        wifi_station_connect();
	        count = 0;
		    os_timer_setfn(&connect_timer,Wifi_conned,NULL);
		    os_timer_arm(&connect_timer,2000,NULL);
            break;
        case SC_STATUS_LINK_OVER:
            os_printf("SC_STATUS_LINK_OVER\n");
            wifi_mode = SC_STATUS_WAIT;
            if (pdata != NULL) {
                uint8 phone_ip[4] = {0};

                os_memcpy(phone_ip, (uint8*)pdata, 4);
                os_printf("Phone ip: %d.%d.%d.%d\n",phone_ip[0],phone_ip[1],phone_ip[2],phone_ip[3]);
            }
            smartconfig_stop();
            break;
    }

}
void ICACHE_FLASH_ATTR done_cb1(void){
	struct station_config config;

	wifi_station_disconnect();
	if(wifi_station_get_config_default(&config)){
		if(config.bssid_set==1){
		    wifi_set_opmode(0x03);
	        wifi_station_connect();
		    os_timer_setfn(&connect_timer,Wifi_conned,NULL);
		    os_timer_arm(&connect_timer,2000,NULL);
		}else{
		    wifi_set_opmode(0x01);
			smartconfig_start(smartconfig_done);
		}
	}else{
		 wifi_set_opmode(0x01);
		smartconfig_start(smartconfig_done);
	}


}

void ICACHE_FLASH_ATTR smart_config(void){
	 wifi_set_opmode(0x01);
	os_timer_disarm(&connect_timer);
	os_timer_disarm(&heartbeat_timer);
	smartconfig_stop();
	espconn_disconnect(&user_tcp_conn);
	wifi_station_disconnect();
	smartconfig_start(smartconfig_done);
}
void user_init(){
    uart_init(115200,115200);
//    ip_addr_t *addr = (ip_addr_t *)os_zalloc(sizeof(ip_addr_t));
//    sntp_setservername(0, "us.pool.ntp.org"); // set server 0 by domain name
//    sntp_setservername(1, "ntp.sjtu.edu.cn"); // set server 1 by domain name
//    addr->addr=210<<24 | 72<<16 | 145<<8 | 44;
//    sntp_setserver(2, addr); // set server 2 by IP address
//    sntp_init();
//    os_free(addr);

    system_init_done_cb(done_cb1);

//    char dsa[32] = "4d97ed3fbae140ea927791435140b269";
//    char dsa1[32] = "ac5a368c89644f60a89fbd8966bedfe0";
//    char dsa2[32] = "cfe4ab4364e14cb9a5ea4c9bfe736784";
//
//    uint8 i=0;
//    for(i=0;i<96;i++){
//    	if(i<32){
//    		str[i] = dsa[i];
//    	}else if(i>=32&&i<64){
//    		str[i] = dsa1[i-32];
//    	}else if(i>=64){
//    		str[i] = dsa2[i-64];
//    	}
//    }
//	spi_flash_erase_sector(0x3C);
//	spi_flash_write(0x3C*SPI_FLASH_SEC_SIZE,(uint32 *)&str,96);

}
void user_rf_pre_init(){}


/******************************************************************************
 * FunctionName : uart_init
 * Description  : user interface for init uart
 * Parameters   : UartBautRate uart0_br - uart0 bautrate
 *                UartBautRate uart1_br - uart1 bautrate
 * Returns      : NONE
*******************************************************************************/
LOCAL int itimer = 0;
#if UART_SELFTEST&UART_BUFF_EN
void ICACHE_FLASH_ATTR uart_test_rx()
{

	uint8 buffer[128];
    uint16 len = 0;
    char * send;

    itimer++;
    if(itimer%3==0){

    	itimer = 0;
    }


	wifi_connect_status = wifi_station_get_connect_status();
    len = rx_buff_deq(buffer, 128 );
    deviceToWifi(buffer,len);

    if(itimer%3==0){
    	if(flag_revudp){
    		flag_revudp = false;
        	if(os_strcmp(udp_receive,"IOT?")==0){
        		user_udp_send_mac();
        	}else{
        		send= getJsonTree();
        		espconn_sent(&user_udp_espconn, send,os_strlen(send));
                os_free(send);
        	}
            os_free(udp_receive);
    	}

    }
    else if(itimer%3==1){
    	if(flag_revtcp){
    		flag_revtcp = false;
    		setJsonTree(revtcp_content);
             if(os_strlen(recvtcpAppImei)>0&&os_strlen(recvtcpAppRaw)>0){
             	  char *tcp_content = getAppResponse();
             	  espconn_sent(&user_tcp_conn,tcp_content,strlen(tcp_content));
             	  os_free(tcp_content);
             	  os_memset(recvtcpAppImei,0,80);
            	  os_memset(recvtcpAppRaw,0,64);
             }
    	}
    }

}
#endif
