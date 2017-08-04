/*
 * my_json.c
 *
 *  Created on: 2016年9月3日
 *      Author: Administrator
 */


#include "resolve_json.h"

LOCAL uint16 recvtcpAppMsgId;
char recvtcpAppImei[80];
char recvtcpAppRaw[64];
char upgradebinUrl[100];
char binVer[32];
LOCAL char recvtcptoken[96];
LOCAL uint8 msgId = 1;
LOCAL int jsonTree_set(struct jsontree_context *js_ctx, struct jsonparse_state *parser);
void setJsonTree(char *json);
LOCAL int jsonObject_set(struct jsonparse_state *parser);
LOCAL bool flag_end = false;

LOCAL int ICACHE_FLASH_ATTR
jsonTree_get(struct jsontree_context *js_ctx)
{
    const char *path = jsontree_path_name(js_ctx, js_ctx->depth - 1);


    if (os_strncmp(path, "devTid", os_strlen("devTid")) == 0) {

        char hwaddr[6];
        os_bzero(hwaddr,6);
        wifi_get_macaddr(STATION_IF,hwaddr);
    	char DeviceBuffer[12]={0};
    	wifi_get_macaddr(STATION_IF,hwaddr);
    	os_sprintf(DeviceBuffer,"ETC_"MACSTR,MAC2STR(hwaddr));
    	jsontree_write_string(js_ctx, DeviceBuffer);

    } else if (os_strncmp(path, "action", os_strlen("action")) == 0) {
    	  	jsontree_write_string(js_ctx, "devSend");
    }else if(os_strncmp(path, "msgId", os_strlen("msgId"))==0){

    	jsontree_write_int(js_ctx,msgId++);

    }else if(os_strncmp(path, "appTid", os_strlen("appTid"))==0){

    	jsontree_write_atom(js_ctx, "[");
    	jsontree_write_atom(js_ctx, "]");

    }else if(os_strncmp(path, "data", os_strlen("data"))==0){
    	jsontree_write_atom(js_ctx, "{");
    	jsontree_write_string(js_ctx, "raw");
    	jsontree_write_atom(js_ctx, ":");
    	jsontree_write_string(js_ctx, length_data);
      	jsontree_write_atom(js_ctx, "}");

    }


    return 0;
}

LOCAL int ICACHE_FLASH_ATTR
jsonTree_reportDevInfo_get(struct jsontree_context *js_ctx)
{
	char hwaddr[6];
    char DeviceBuffer[12]={0};
    const char *path = jsontree_path_name(js_ctx, js_ctx->depth - 1);
    if (os_strncmp(path, "devTid", os_strlen("devTid")) == 0) {


        os_bzero(hwaddr,6);
    	wifi_get_macaddr(STATION_IF,hwaddr);
    	os_sprintf(DeviceBuffer,"ETC_"MACSTR,MAC2STR(hwaddr));
    	jsontree_write_string(js_ctx, DeviceBuffer);

    }else if(os_strncmp(path, "msgId", os_strlen("msgId"))==0){

    	jsontree_write_int(js_ctx,msgId++);

    }else if (os_strncmp(path, "action", os_strlen("action")) == 0) {
	  	jsontree_write_string(js_ctx, "reportDevInfo");
    }else if (os_strncmp(path, "mid", os_strlen("mid")) == 0) {
	  	jsontree_write_string(js_ctx, "Re6VCbDQ6cNE");
    }else if (os_strncmp(path, "workMode", os_strlen("workMode")) == 0) {
    	 jsontree_write_int(js_ctx, 0);
    }else if (os_strncmp(path, "MAC", os_strlen("MAC")) == 0) {
        os_bzero(hwaddr,6);
    	wifi_get_macaddr(STATION_IF,hwaddr);
    	os_sprintf(DeviceBuffer,MACSTR,MAC2STR(hwaddr));
    	jsontree_write_string(js_ctx, DeviceBuffer);
    }else if (os_strncmp(path, "tokenType", os_strlen("tokenType")) == 0) {
   	 jsontree_write_int(js_ctx, 2);
    }else if (os_strncmp(path, "binVer", os_strlen("binVer")) == 0) {
    	Readflash_VER();
    	jsontree_write_string(js_ctx, dev_ver);
	}else if (os_strncmp(path, "binType", os_strlen("binType")) == 0) {
		   uint8 d = system_upgrade_userbin_check();
		 	jsontree_write_string(js_ctx, d==0?"A":"B");
    }

	return 0;
}


LOCAL int ICACHE_FLASH_ATTR
jsonTree_appResp_get(struct jsontree_context *js_ctx)
{
    const char *path = jsontree_path_name(js_ctx, js_ctx->depth - 1);


    if (os_strncmp(path, "devTid", os_strlen("devTid")) == 0) {

        char hwaddr[6];
        os_bzero(hwaddr,6);
        wifi_get_macaddr(STATION_IF,hwaddr);
    	char DeviceBuffer[12]={0};
    	wifi_get_macaddr(STATION_IF,hwaddr);
    	os_sprintf(DeviceBuffer,"ETC_"MACSTR,MAC2STR(hwaddr));
    	jsontree_write_string(js_ctx, DeviceBuffer);

    } else if (os_strncmp(path, "action", os_strlen("action")) == 0) {
    		jsontree_write_string(js_ctx, "appSendResp");
    }else if(os_strncmp(path, "msgId", os_strlen("msgId"))==0){

    	jsontree_write_int(js_ctx,recvtcpAppMsgId);

    }else if(os_strncmp(path, "appTid", os_strlen("appTid"))==0){

    	jsontree_write_string(js_ctx, recvtcpAppImei);

    }else if(os_strncmp(path, "data", os_strlen("data"))==0){
    	jsontree_write_atom(js_ctx, "{");
    	jsontree_write_string(js_ctx, "raw");
    	jsontree_write_atom(js_ctx, ":");
    	jsontree_write_string(js_ctx, recvtcpAppRaw);
      	jsontree_write_atom(js_ctx, "}");

    }else if(os_strncmp(path, "code", os_strlen("code"))==0){
    	jsontree_write_int(js_ctx,200);
    }else if(os_strncmp(path, "desc", os_strlen("desc"))==0){
    	jsontree_write_string(js_ctx,"success");
    }else if(os_strncmp(path, "ctrlKey", os_strlen("ctrlKey"))==0){
    	Readflash_ctrlkey();
    	jsontree_write_string(js_ctx, dev_ctrlkey);
    }


    return 0;
}

LOCAL int ICACHE_FLASH_ATTR
jsonTree_get_Login(struct jsontree_context *js_ctx)
{
    const char *path = jsontree_path_name(js_ctx, js_ctx->depth - 1);


    if (os_strncmp(path, "devTid", os_strlen("devTid")) == 0) {

        char hwaddr[6];
        os_bzero(hwaddr,6);
        wifi_get_macaddr(STATION_IF,hwaddr);
    	char DeviceBuffer[12]={0};
    	wifi_get_macaddr(STATION_IF,hwaddr);
    	os_sprintf(DeviceBuffer,"ETC_"MACSTR,MAC2STR(hwaddr));
    	jsontree_write_string(js_ctx, DeviceBuffer);

    }else if(os_strncmp(path, "action", os_strlen("action")) == 0) {
    	jsontree_write_string(js_ctx, "devLogin");
    }else if(os_strncmp(path, "msgId", os_strlen("msgId"))==0){
    	jsontree_write_int(js_ctx,msgId++);
    }else if(os_strncmp(path, "prodKey", os_strlen("prodKey"))==0){
    	jsontree_write_string(js_ctx, "bbb775c29abae5235384342d2f67027b");
    }else if(os_strncmp(path, "token", os_strlen("token"))==0){
    	Readflash_token();
    	jsontree_write_string(js_ctx, dev_token);
    }
    return 0;
}

LOCAL int ICACHE_FLASH_ATTR
jsonTree_get_HeartBeat(struct jsontree_context *js_ctx)
{
    const char *path = jsontree_path_name(js_ctx, js_ctx->depth - 1);


    if(os_strncmp(path, "action", os_strlen("action")) == 0) {
    	jsontree_write_string(js_ctx, "heartbeat");
    }else if(os_strncmp(path, "msgId", os_strlen("msgId"))==0){
    	jsontree_write_int(js_ctx,msgId++);
    }
    return 0;
}

//============================
LOCAL struct jsontree_callback jsonReportDevInfoCallback =
    JSONTREE_CALLBACK(jsonTree_reportDevInfo_get, NULL);


JSONTREE_OBJECT(jsonReportDevInfoObject,
                JSONTREE_PAIR("devTid", &jsonReportDevInfoCallback),
                JSONTREE_PAIR("mid", &jsonReportDevInfoCallback),
		        JSONTREE_PAIR("workMode", &jsonReportDevInfoCallback),
		        JSONTREE_PAIR("MAC", &jsonReportDevInfoCallback),
		        JSONTREE_PAIR("tokenType", &jsonReportDevInfoCallback),
		        JSONTREE_PAIR("binVer", &jsonReportDevInfoCallback),
		        JSONTREE_PAIR("binType", &jsonReportDevInfoCallback));
JSONTREE_OBJECT(jsonReportDevInfoTrees,
                JSONTREE_PAIR("msgId", &jsonReportDevInfoCallback),
                JSONTREE_PAIR("action", &jsonReportDevInfoCallback),
				JSONTREE_PAIR("params", &jsonReportDevInfoObject));

JSONTREE_OBJECT(jsonReportDevInfoTree,
                JSONTREE_PAIR("reportDevInfo", &jsonReportDevInfoTrees));

//============================
LOCAL struct jsontree_callback jsonCallback =
    JSONTREE_CALLBACK(jsonTree_get, jsonTree_set);


JSONTREE_OBJECT(jsonObject,
                JSONTREE_PAIR("devTid", &jsonCallback),
                JSONTREE_PAIR("appTid", &jsonCallback),
				JSONTREE_PAIR("data", &jsonCallback));
JSONTREE_OBJECT(jsonTestTrees,
                JSONTREE_PAIR("msgId", &jsonCallback),
                JSONTREE_PAIR("action", &jsonCallback),
				JSONTREE_PAIR("params", &jsonObject));

JSONTREE_OBJECT(jsonTestTree,
                JSONTREE_PAIR("jsonTest", &jsonTestTrees));


//============================
LOCAL struct jsontree_callback jsonAppCallback =
    JSONTREE_CALLBACK(jsonTree_appResp_get, jsonTree_set);


JSONTREE_OBJECT(jsonAppObject,
                JSONTREE_PAIR("devTid", &jsonAppCallback),
		         JSONTREE_PAIR("ctrlKey", &jsonAppCallback),
                JSONTREE_PAIR("appTid", &jsonAppCallback),
				JSONTREE_PAIR("data", &jsonAppCallback));
JSONTREE_OBJECT(jsonAppRespTrees,
                JSONTREE_PAIR("msgId", &jsonAppCallback),
                JSONTREE_PAIR("action", &jsonAppCallback),
                JSONTREE_PAIR("code", &jsonAppCallback),
                JSONTREE_PAIR("desc", &jsonAppCallback),
				JSONTREE_PAIR("params", &jsonAppObject));

JSONTREE_OBJECT(jsonAppRespTree,
                JSONTREE_PAIR("appResp", &jsonAppRespTrees));

//============================




LOCAL struct jsontree_callback jsonLoginCallback =
    JSONTREE_CALLBACK(jsonTree_get_Login, jsonTree_set);

JSONTREE_OBJECT(jsonLoginObject,
                JSONTREE_PAIR("devTid", &jsonLoginCallback),
                JSONTREE_PAIR("prodKey", &jsonLoginCallback),
				JSONTREE_PAIR("token", &jsonLoginCallback));

JSONTREE_OBJECT(jsonLoginTrees,
                JSONTREE_PAIR("msgId", &jsonLoginCallback),
                JSONTREE_PAIR("action", &jsonLoginCallback),
				JSONTREE_PAIR("params", &jsonLoginObject));

JSONTREE_OBJECT(jsonLoginTree,
                JSONTREE_PAIR("Login", &jsonLoginTrees));


LOCAL struct jsontree_callback jsonHeatbeatCallback =
    JSONTREE_CALLBACK(jsonTree_get_HeartBeat, jsonTree_set);

JSONTREE_OBJECT(jsonHeartbeatTrees,
                JSONTREE_PAIR("msgId", &jsonHeatbeatCallback),
                JSONTREE_PAIR("action", &jsonHeatbeatCallback));

JSONTREE_OBJECT(jsonHeartbeatTree,
                JSONTREE_PAIR("HeartBeat", &jsonHeartbeatTrees));


#define LENGTH 512
static char jsonbuf[LENGTH];
char* ICACHE_FLASH_ATTR
getJsonTree(void)
{

    os_memset(jsonbuf, 0, LENGTH);      //初始化字符串
	json_ws_send((struct jsontree_value *)&jsonTestTree, "jsonTest", jsonbuf);
	normal_replace(jsonbuf,"\n","");
	strcat(jsonbuf,"\n");
	return jsonbuf;
}

char* ICACHE_FLASH_ATTR
getAppResponse(void)
{
    os_memset(jsonbuf, 0, LENGTH);      //初始化字符串
	json_ws_send((struct jsontree_value *)&jsonAppRespTree, "appResp", jsonbuf);
	normal_replace(jsonbuf,"\n","");
	strcat(jsonbuf,"\n");
	return jsonbuf;
}

char* ICACHE_FLASH_ATTR
getReportDevInfo(void)
{
    os_memset(jsonbuf, 0, LENGTH);      //初始化字符串
	json_ws_send((struct jsontree_value *)&jsonReportDevInfoTree, "reportDevInfo", jsonbuf);
	normal_replace(jsonbuf,"\n","");
	strcat(jsonbuf,"\n");
	return jsonbuf;
}


char* ICACHE_FLASH_ATTR
getLogin(void)
{
    os_memset(jsonbuf, 0, LENGTH);      //初始化字符串
	json_ws_send((struct jsontree_value *)&jsonLoginTree, "Login", jsonbuf);
	normal_replace(jsonbuf,"\n","");
	strcat(jsonbuf,"\n");
	return jsonbuf;
}

char* ICACHE_FLASH_ATTR
getHeartBeat(void)
{
    os_memset(jsonbuf, 0, LENGTH);      //初始化字符串
	json_ws_send((struct jsontree_value *)&jsonHeartbeatTree, "HeartBeat", jsonbuf);
	normal_replace(jsonbuf,"\n","");
	strcat(jsonbuf,"\n");
	return jsonbuf;
}


LOCAL int ICACHE_FLASH_ATTR
jsonTree_set(struct jsontree_context *js_ctx, struct jsonparse_state *parser)
{
    int type;
    while ((type = jsonparse_next(parser)) != 0) {
    	//如果是KEY类型
        if (type == JSON_TYPE_PAIR_NAME) {
            char buffer[64];
            os_bzero(buffer, 64);

            if (jsonparse_strcmp_value(parser, "action") == 0) {
                jsonparse_next(parser);	//返回的是冒号字符
                type = jsonparse_next(parser);	//返回的是双引号字符
                os_printf("action Value type = %c\n", type);	// = "

                //如果Value是字符串类型，则读取数据到buffer
                if (JSON_TYPE_STRING == type){	//#define JSON_TYPE_STRING '"'
                	jsonparse_copy_value(parser, buffer, sizeof(buffer));
                	os_printf("action Value = %s\n", buffer);
                }

            }else if (jsonparse_strcmp_value(parser, "msgId") == 0) {
                jsonparse_next(parser);	//返回的是冒号字符
                type = jsonparse_next(parser);	//返回的是双引号字符
                os_printf("action msgId type = %c\n", type);	// = "

                //如果Value是字符串类型，则读取数据到buffer
                if (JSON_TYPE_NUMBER == type){	//#define JSON_TYPE_STRING '"'

                	recvtcpAppMsgId = jsonparse_get_value_as_int(parser);

                }

            }
#if 0
            else if (jsonparse_strcmp_value(parser, "Integer") == 0) {
                jsonparse_next(parser);
                type = jsonparse_next(parser);


                os_printf("Integer Value type = %c\n", type);	// = 0
                //如果Value是数值类型
                if(JSON_TYPE_NUMBER == type){	//#define JSON_TYPE_NUMBER '0'
                	//jsonparse_copy_value(parser, buffer, sizeof(buffer));
                	int num = 0;
                	num = jsonparse_get_value_as_int(parser);
                	os_printf("Integer Value = %d\n", num);		// = 1
                }
            } else if (jsonparse_strcmp_value(parser, "Array") == 0) {
                jsonparse_next(parser);	//跳过冒号
                type = jsonparse_next(parser);
                os_printf("Array Value type = %c\n", type);		// = [

                //如果Value是数组类型
                if(JSON_TYPE_ARRAY == type){	//#define JSON_TYPE_ARRAY '['
                	//jsonparse_copy_value(parser, buffer, sizeof(buffer));
                	int length = jsonparse_get_len(parser);
                	os_printf("Array Length = %d\n", length);	//= 5, 数据是[0,1,2]，可能把逗号也算在内

                	int i;
                	int num = 100;
                	//循环读取数组的数据
                	for(i=0; i<length; i++){
                		type = jsonparse_next(parser);
                		// 如果是逗号，则直接打印, 如果是数值则打印0
                		os_printf("Array[%d] type = %c ", i, type);

                		//如果是数值类型，则转换成int并打印出来
                		if(JSON_TYPE_NUMBER==type){
                        	num = jsonparse_get_value_as_int(parser);
                        	os_printf("Array[%d] = %d\n", i, num);
                		}
                		//后面可以添加else if判断是否是其他类型
                		//比如 else if(JSON_TYPE_OBJECT==type),判断是否是Json对象
                		else{
                			os_printf("\n");
                		}
                	}
                }
            }
#endif
#if 1
            else if (jsonparse_strcmp_value(parser, "params") == 0) {
                jsonparse_next(parser);	//跳过冒号
                type = jsonparse_next(parser);
                os_printf("params Value type = %c\n", type);		// = {

                if(JSON_TYPE_OBJECT == type){	//#define JSON_TYPE_OBJECT '{'
                	int length = jsonparse_get_len(parser);
                	os_printf("params Length = %d\n", length);

                	//char temp[128] = {0};
                	//jsonparse_copy_value(parser, temp, 128);
                	//os_printf("JsonObject Value = %s\n", temp);

                	//循环读取数据
                	//int i = 0;
                	//for(i=0; i<length; i++){
                	while(type != '}'){
                	//	i++;
                		type = jsonparse_next(parser);

                		//os_memset(temp, 0, 128);
                    	//jsonparse_copy_value(parser, temp, 128);
                    	//os_printf("JsonObject Value = %s\n", temp);
                		//读取第二层的Json对象的数据
                		if(!flag_end){
                        	jsonObject_set(parser);
                		}else{
                			flag_end = false;
                			break;
                		}

                	}
                }

            }
#endif
        }
    }

    return 0;
}



LOCAL int ICACHE_FLASH_ATTR
jsonObject_set(struct jsonparse_state *parser)
{
	int type = jsonparse_get_type(parser);
	int vtype = parser->vtype;
	char buffer[64];
	char binType[2];
	int i;
	os_bzero(buffer, 64);
	os_bzero(binType, 2);
	//如果是KEY类型
	if (vtype == JSON_TYPE_PAIR_NAME) {
	   if (jsonparse_strcmp_value(parser, "devTid") == 0) {
			jsonparse_next(parser);	//返回的是冒号字符
			type = jsonparse_next(parser);	//返回的是双引号字符
			//os_printf("jsonObject String Value type = %c\n", type);	// = "

			//如果Value是字符串类型，则读取数据到buffer
			if (JSON_TYPE_STRING == type){	//#define JSON_TYPE_STRING '"'
				jsonparse_copy_value(parser, buffer, sizeof(buffer));
				os_printf("devTid  Value = %s\n", buffer);
			}

		}else if(jsonparse_strcmp_value(parser,"appTid")==0){
			jsonparse_next(parser);	//返回的是冒号字符
			type = jsonparse_next(parser);	//返回的是双引号字符

			//如果Value是字符串类型，则读取数据到buffer
			if (JSON_TYPE_STRING == type){	//#define JSON_TYPE_STRING '"'
				jsonparse_copy_value(parser, buffer, sizeof(buffer));
				os_printf("appTid  Value = %s\n", buffer);
	            os_strcpy(recvtcpAppImei,buffer);
			}

		}else if (jsonparse_strcmp_value(parser, "data") == 0) {
			jsonparse_next(parser);
			type = jsonparse_next(parser);
			//os_printf("jsonObject Integer Value type = %c\n", type);	// = 0
			//如果Value是数值类型
			if(JSON_TYPE_OBJECT == type){
				//jsonparse_copy_value(parser, buffer, sizeof(buffer));
				//os_printf("buffer = %s\n", buffer);
            	int length = jsonparse_get_len(parser);
            	int vlen = parser->vlen;


            	//循环读取Json对象数据

				//for(i=0; i<length; i++){
				while(type != '}'){	//用'}'判断是否达到二级Object末尾

					type = jsonparse_next(parser);


					//如果是KEY类型
					if (type == JSON_TYPE_PAIR_NAME) {
					   if (jsonparse_strcmp_value(parser, "cmdId") == 0) {
							jsonparse_next(parser);	//返回的是冒号字符
							type = jsonparse_next(parser);	//返回的是双引号字符
							//os_printf("K1 Value type = %c\n", type);	// = "

							//如果Value是字符串类型，则读取数据到buffer
							if (JSON_TYPE_NUMBER == type){	//#define JSON_TYPE_STRING '"'

			                	int num = 0;
			                	num = jsonparse_get_value_as_int(parser);
			                	os_printf("cmdId Value = %d\n", num);		// = 1

							}
						}else if(jsonparse_strcmp_value(parser, "raw") == 0){
							jsonparse_next(parser);	//返回的是冒号字符
							type = jsonparse_next(parser);	//返回的是双引号字符

							//如果Value是字符串类型，则读取数据到buffer
							if (JSON_TYPE_STRING == type){	//#define JSON_TYPE_STRING '"'

								jsonparse_copy_value(parser, buffer, sizeof(buffer));
					            os_strcpy(recvtcpAppRaw,buffer);
							}
						}
					}
				}
			}
		}
		else if(jsonparse_strcmp_value(parser, "token") == 0){
			jsonparse_next(parser);
			type = jsonparse_next(parser);

			if(JSON_TYPE_STRING == type){

				jsonparse_copy_value(parser, buffer, sizeof(buffer));
                      for(i=0;i<32;i++){
                      		recvtcptoken[i] = buffer[i];
                      }
			}
		}else if(jsonparse_strcmp_value(parser, "ctrlKey") == 0){
			jsonparse_next(parser);
			type = jsonparse_next(parser);
			if(JSON_TYPE_STRING == type){
				jsonparse_copy_value(parser, buffer, sizeof(buffer));
                  for(i=32;i<64;i++){
                  		recvtcptoken[i] = buffer[i-32];
                  }
			}
		}else if(jsonparse_strcmp_value(parser, "bindKey") == 0){
			jsonparse_next(parser);
			type = jsonparse_next(parser);
			if(JSON_TYPE_STRING == type){
				jsonparse_copy_value(parser, buffer, sizeof(buffer));

		              flag_end = true;
	                  for(i=64;i<96;i++){
	                  		recvtcptoken[i] = buffer[i-64];
	                  }
	                  Readflash_token();
                    if(os_strlen(dev_token)==0){
                		spi_flash_erase_sector(0xFC);
               			spi_flash_write(0xFC*SPI_FLASH_SEC_SIZE,(uint32 *)&recvtcptoken,96);
                    }

			}

		}else if(jsonparse_strcmp_value(parser, "binUrl") == 0){
			jsonparse_next(parser);
			type = jsonparse_next(parser);
			if(JSON_TYPE_STRING == type){
				jsonparse_copy_value(parser, upgradebinUrl, sizeof(upgradebinUrl));
            	os_printf("下载的bin地址:%s",upgradebinUrl);
			}

		}else if(jsonparse_strcmp_value(parser, "binType") == 0){
			jsonparse_next(parser);
			type = jsonparse_next(parser);
			if(JSON_TYPE_STRING == type){
				jsonparse_copy_value(parser, binType, sizeof(binType));
				uint8 d = system_upgrade_userbin_check();

                if((d == 0 && os_strncmp(binType, "B", os_strlen("B")) == 0)
                   ||(d == 1 && os_strncmp(binType, "A", os_strlen("A")) == 0)){


                	espconn_disconnect(&user_tcp_conn);
                	system_upgrade_flag_set(0x01);
                }
			}

		}else if(jsonparse_strcmp_value(parser, "binVer") == 0){
			jsonparse_next(parser);
			type = jsonparse_next(parser);
			if(JSON_TYPE_STRING == type){
				jsonparse_copy_value(parser, binVer, sizeof(binVer));
				binVer[os_strlen(binVer)] = '\0';
			}

		}
	}
}



void ICACHE_FLASH_ATTR
setJsonTree(char *json)
{
    struct jsontree_context js;

    jsontree_setup(&js, (struct jsontree_value *)&jsonTestTree, json_putchar);
    json_parse(&js, json);
}


/**
 * 统计key在data中出现的次数
 * @param data 待查找的字符串
 * @param key  要查找的字符串
 * @return key在data中出现的次数
 */
int ICACHE_FLASH_ATTR _count_string(char *data, char *key)
{
    int count = 0;
    int klen = os_strlen(key);
    char *pos_start = data, *pos_end;
    while (NULL != (pos_end = strstr(pos_start, key))) {
        pos_start = pos_end + klen;
        count++;
    }
    return count;
}

/**
 * 将data中的rep字符串替换成to字符串，以动态分配内存方式返回新字符串
 * 这个函数不需要保证data能保证容量。
 * @param data 待替换某些字符串的数据
 * @param rep  待替换的字符串
 * @param to   替换成的字符串
 * @param free_data 不为0时要释放data的内存
 * @return 返回新分配内存的替换完成的字符串，注意释放。
 */
char ICACHE_FLASH_ATTR *malloc_replace(char *data, char *rep, char *to, int free_data)
{
    int rep_len = strlen(rep);
    int to_len  = strlen(to);
    int counts  = _count_string(data, rep);
    os_printf("counts = %d\n", counts);
    int m = strlen(data) + counts * (to_len - rep_len);
    char *new_buf = (char *)os_malloc(m + 1);
    if (NULL == new_buf) {
        os_free(data);
        return NULL;
    }
    memset(new_buf, 0, m + 1);
    char *pos_start = data, *pos_end, *pbuf = new_buf;
    int copy_len;
    while (NULL != (pos_end = strstr(pos_start, rep))) {
        copy_len = pos_end - pos_start;
        strncpy(pbuf, pos_start, copy_len);
        pbuf += copy_len;
        strcpy(pbuf, to);
        pbuf += to_len;
        pos_start  = pos_end + rep_len;
    }
    strcpy(pbuf, pos_start);
    if (free_data)
    	os_free(data);
    return new_buf;
}

/**
 * 将data中的rep字符串替换成to字符串
 * 注意保证data空间足够替换完成后的字符串长度
 * @param data 待替换某些字符串的数据
 * @param rep  待替换的字符串
 * @param to   替换成的字符串
 * @return 无
 */
void ICACHE_FLASH_ATTR normal_replace(char *data, char *rep, char *to)
{
    char *new_buf = malloc_replace(data, rep, to, 1);
    if (NULL != new_buf) {
        strcpy(data, new_buf);
        os_free(new_buf);
    }
}



