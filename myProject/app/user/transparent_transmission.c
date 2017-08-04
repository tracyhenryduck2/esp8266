#include "transparent_transmission.h"
UDP_DATA_STATUS    udpDataStatus = SEND_TCP_UDP_IDLE;
uint8 data1; //有效数据1
uint8 data2; //有效数据2
sc_status wifi_mode;  //WiFi模块的工作方式，0代表正常模式，非0代表配网模式中
uint8 wifi_connect_status;  //WIFI模块的链接状态;
char length_data[200];  //串口的数据长度

/******************************************************************************
* FunctionName : DeviceToWifi
* Description  :
* Parameters   : char *input:
in autoload mode
                        50 ~ 0x7fffff;  for FRC1 source.
                        100 ~ 0x7fffff;  for NMI source.
in non autoload mode:
                        10 ~ 0x7fffff;
* Returns      : NONE
*******************************************************************************/
void deviceToWifi(uint8 *input,uint8 length){
	ERR_TYPE errcode = ERR_OK;
    uint8 header = *(input);
    uint8 reallength = *(input+1);
    uint8 type = *(input+2);
    uint8 idframe = *(input+3);
    uint8 i = 0;
    if( length == 0 ){
    	 //os_printf("无数据");
    	 return;
    }
    //数据超出范围 或者数据不足
    if( length<=5 || reallength != length || reallength > MAX_DATA_LENGTH || length > MAX_DATA_LENGTH ){
    	 os_printf("ERR_OUT_BANGDE");
    	 return;
    }

    uint8 checksum = *(input+length-1);
    uint8 calchek = calcrc_bytes(input,length-1);
    uint8 ds = *(input+4);
    if(calchek != checksum){
		 os_printf("ERR_Checksum%d,%d",calchek,checksum);
		 return;
    }

    switch(type){
      case REPORT_FRAME:
          data1 = *(input+5);
          data2 = *(input+6);
    	  udpDataStatus = ds;


    	  for(i=0;i<length;i++){
    		  uint8 ds_high = *(input+i)/16;
    		  uint8 ds_low = *(input+i)%16;
    		  if(ds_high>=0&&ds_high<10){
    			  length_data[2*i] = 48+ds_high;
    		  }else{
    			  length_data[2*i] = 55+ds_high;
    		  }
    		  if(ds_low>=0&&ds_low<10){
    			  length_data[2*i+1] = 48+ds_low;
    		  }else{
    			  length_data[2*i+1] = 55+ds_low;
    		  }

    	  }
    	  length_data[2*length] = '\0';
    	  char *tcp_content = getJsonTree();
    	  os_printf("tcp_content:%s",tcp_content);
    	  espconn_sent(&user_tcp_conn,tcp_content,strlen(tcp_content));
    	  user_udp_send_info();
    	  udpDataStatus = SEND_TCP_UDP_IDLE;
    	  HttpReadFile("http://192.168.3.140/beauty/app/SiterAction!emailtoMember.action","GET","");
    	  break;
      case REACT_FRAME:
          data1 = *(input+5);
          data2 = *(input+6);
    	  udpDataStatus = ds;


    	  for(i=0;i<length;i++){
    		  uint8 ds_high = *(input+i)/16;
    		  uint8 ds_low = *(input+i)%16;
    		  if(ds_high>=0&&ds_high<10){
    			  length_data[2*i] = 48+ds_high;
    		  }else{
    			  length_data[2*i] = 55+ds_high;
    		  }
    		  if(ds_low>=0&&ds_low<10){
    			  length_data[2*i+1] = 48+ds_low;
    		  }else{
    			  length_data[2*i+1] = 55+ds_low;
    		  }

    	  }
    	  length_data[2*length] = '\0';
    	  char *tcp_content2 = getAppResponse();
    	  os_printf("tcp_asp_content:%s",tcp_content2);
    	  espconn_sent(&user_tcp_conn,tcp_content2,strlen(tcp_content2));
    	  user_udp_send_resp();
    	  udpDataStatus = SEND_TCP_UDP_IDLE;
    	  break;
      case OPERATION_FRAME:
    	  if(ds == QUERY_STATUS){
    		  _Txbuf[0] = header;
    		  _Txbuf[1] = 0x0B;
    		  _Txbuf[2] = type;
    		  _Txbuf[3] = idframe;
    		  _Txbuf[4] = QUERY_STATUS;
    		  _Txbuf[5] = wifi_mode;
    		  _Txbuf[6] = wifi_connect_status;
    		  _Txbuf[7] = 1;
    		  _Txbuf[8] = 0;
    		  _Txbuf[9] = 0;

    		 uint8 checksum = calcrc_bytes(_Txbuf,10);
    		 _Txbuf[10] = checksum;
    		 uart0_tx_buffer(_Txbuf,11);
    	  }else if(ds == SMART_CONFIG){
    		  _Txbuf[0] = header;
    		  _Txbuf[1] = reallength;
    		  _Txbuf[2] = type;
    		  _Txbuf[3] = idframe;
    		  _Txbuf[4] = SMART_CONFIG;
    		  _Txbuf[5] = 0;
    		 uint8 checksum = calcrc_bytes(_Txbuf,6);
    		 _Txbuf[6] = checksum;
    		 uart0_tx_buffer(_Txbuf,7);
             system_os_post(0,2,0); //事件传递
    	  }
    	  break;
      case ERR_FRAME:

    	  break;
    }



}


/******************************************************************************
* FunctionName :calcrc_bytes
* Description  :CRC校验
* Parameters   : uint8 *p,uint8 len


* Returns      : *p
*******************************************************************************/
uint8 calcrc_bytes(uint8 *p,uint8 len)
{
	uint8 crc=0;
	uint8 i=0;
    for(i=0;i<len;i++){
    	crc = crc + *(p+i);
    }
	return crc;  //若最终返回的crc为0，则数据传输正确
}

/******************************************************************************
* FunctionName :calcrc_bytes
* Description  :CRC校验
* Parameters   : uint8 *p,uint8 len


* Returns      : *p
*******************************************************************************/
void DeviceOperation(uint8 comand){


   switch(comand){
   case QUERY_STATUS:
	   break;
   case SMART_CONFIG:
	   os_printf("进入配网模式");
	   break;
   default:
	   break;
   }
}

