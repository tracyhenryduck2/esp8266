/*
 * client.c
 *
 *  Created on: 2015年7月22日
 *      Author: Administrator
 */

#include "client.h"

ETSTimer report_timer;
char revtcp_content[500];
bool flag_revtcp = false;


void ICACHE_FLASH_ATTR report(void *arg){
char *ds;
os_timer_disarm(&report_timer);
  ds =getReportDevInfo();
  os_printf("getReportDevInfo:%s",ds);
  espconn_sent(&user_tcp_conn,ds,strlen(ds));
  os_free(ds);
}


void ICACHE_FLASH_ATTR user_tcp_sent_cb(void *arg){
	os_printf("发送数据成功！");
}
void ICACHE_FLASH_ATTR user_tcp_discon_cb(void *arg){
	os_printf("断开连接成功！");
	   uint8 flag = system_upgrade_flag_check();
	   os_printf("是否处于更新：%d\r\n",flag);
	if(flag ==0x01){
		HttpReadBinUrl(upgradebinUrl,"GET","");
		updataflag=false;
	    pre_addr = (system_upgrade_userbin_check() == 0?SECTOR_USER2_ADDR:SECTOR_USER1_ADDR);
	}
}

bool ICACHE_FLASH_ATTR parse_http_head_length(char *pdata,int *length,char *pdata2,unsigned short *t){
	char *PA;
	char *PB;
	char buffer[10];
	os_memset(buffer,0,10);
	PA=pdata;
	if(strncmp(PA,"HTTP/1.1 200 OK",strlen("HTTP/1.1 200 OK")))
			return false;
	PB=strstr(PA,"Content-Length: ");
	if(PB==NULL)
		return false;
	PA=PB+strlen("Content-Length: ");
	PB=strstr(PA,"\r\n");
	if(PB==NULL)
		return false;
	os_memcpy(buffer,PA,PB-PA);
	*length=atoi(buffer);
	PA=strstr(PB,"\r\n\r\n");
	if(PA==NULL)
		*t=0;
	else{
		PA=PA+4;
		*t=*t-(PA-pdata);
		memcpy(pdata2,PA,*t);
	}
	return true;
}


void ICACHE_FLASH_ATTR user_tcp_recv_cb(void *arg,
		char *pdata,
		unsigned short len){

	os_printf("tcp收到数据：%s\r\n",pdata);
	os_printf("tcp收到数据长度：%d\r\n",len);
   uint8 flag = system_upgrade_flag_check();
	if(flag!=0x01){
		flag_revtcp = true;
		if(len<1000){
			os_strcpy(revtcp_content,pdata);
		}else{
			os_printf("tcp收到数据过长！");
		}
	}else{


		if(updataflag==true){
			int pre_bufferlength=bufferlength;
			bufferlength+=len;
			recvlength+=len;
			int i=0;
			int s=0;
			if(bufferlength>4096){
				os_memcpy(write_buffer+pre_bufferlength,pdata+i*4096-s,4096-pre_bufferlength);
				i++;
				if(i==1)
					s=pre_bufferlength;
				spi_flash_erase_sector(pre_addr+erase_sector);
				spi_flash_write((pre_addr+erase_sector)*4096,(uint32 *)write_buffer,4096);
				erase_sector++;
				os_memset(write_buffer,0,4096);
				bufferlength-=4096;
				pre_bufferlength=0;

			}
			os_memcpy(write_buffer+pre_bufferlength,pdata+i*4096-s,bufferlength-pre_bufferlength);
			if(recvlength>=datalength){
				os_printf("ota完成!!!");
				writeflash_VER(binVer);
				updataflag=false;
				spi_flash_erase_sector(pre_addr+erase_sector);
				spi_flash_write((pre_addr+erase_sector)*4096,(uint32 *)write_buffer,bufferlength);
				system_upgrade_flag_set(0x02);
				system_upgrade_reboot();
			}

			return;
		}

		char *pdata2=(char *)os_malloc(len);

		unsigned short t=len;
		if(parse_http_head_length(pdata,&datalength,pdata2,&t)){
			updataflag=true;

			int k=datalength/4096+(datalength%4096!=0?1:0);
			int i;
			erase_sector=0;
			os_printf("第一个扇区%d",pre_addr);
			if(t!=0){
				os_memset(write_buffer,0,4096);
				bufferlength=t;
				recvlength=t;
				os_memcpy(write_buffer,pdata2,bufferlength);
			}

		}
		os_free(pdata2);
	}



}
void ICACHE_FLASH_ATTR user_tcp_recon_cb(void *arg, sint8 err){
	os_printf("连接错误，错误代码为%d\r\n",err);
	//espconn_connect((struct espconn *)arg);
}


void ICACHE_FLASH_ATTR user_tcp_connect_cb(void *arg){
	struct espconn *pespconn=arg;
	espconn_regist_recvcb(pespconn,user_tcp_recv_cb);
	 espconn_regist_sentcb(pespconn,user_tcp_sent_cb);
	 espconn_regist_disconcb(pespconn,user_tcp_discon_cb);

	 if(system_upgrade_flag_check()!=0x01){
		 char* ds;
		 ds = getLogin();
		 os_printf("ds1:%s",ds);
		 espconn_sent(pespconn,ds,strlen(ds));
		 os_free(ds);
		 os_timer_setfn(&report_timer,report,NULL);
		 os_timer_arm(&report_timer,3000,NULL);
	 }else{
		 espconn_sent(pespconn,binbuffer,strlen(binbuffer));
	 }


}

void ICACHE_FLASH_ATTR user_tcp_sent(void *arg,char *content){
	struct espconn *pespconn=arg;
	 os_printf("content:%s",content);
	 espconn_sent(pespconn,content,strlen(content));
	 os_free(pespconn);
}



void ICACHE_FLASH_ATTR my_station_init(struct ip_addr *remote_ip,struct ip_addr *local_ip,int remote_port){
	//espconn参数配置
	user_tcp_conn.type=ESPCONN_TCP;
	user_tcp_conn.state=ESPCONN_NONE;
	user_tcp_conn.proto.tcp=(esp_tcp *)os_zalloc(sizeof(esp_tcp));
	os_memcpy(user_tcp_conn.proto.tcp->local_ip,local_ip,4);
	os_memcpy(user_tcp_conn.proto.tcp->remote_ip,remote_ip,4);
	user_tcp_conn.proto.tcp->local_port=espconn_port();
	user_tcp_conn.proto.tcp->remote_port=remote_port;
	//注册连接回调函数和重连回调函数
	espconn_regist_connectcb(&user_tcp_conn,user_tcp_connect_cb);
	espconn_regist_reconcb(&user_tcp_conn,user_tcp_recon_cb);
	//启用连接
	espconn_connect(&user_tcp_conn);
}

