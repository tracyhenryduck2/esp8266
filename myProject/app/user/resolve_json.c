/*
 * my_json.c
 *
 *  Created on: 2021/05/21
 *      Author: Administrator
 */
#define JSONTEST "jsonTest"
#define DEVSEND "DEVSEND"
#define APPRESP "appResp"
#define LOGIN "Login"
#define HEARTBEAT "HeartBeat"
#define REPORTDEVINFO "reportDevInfo"

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
    	os_sprintf(DeviceBuffer,"ESP_%s","34ABE");
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
jsonTree_devSend_get(struct jsontree_context *js_ctx)
{
    const char *path = jsontree_path_name(js_ctx, js_ctx->depth - 1);


    if (os_strncmp(path, "devTid", os_strlen("devTid")) == 0) {

        char hwaddr[6];
        os_bzero(hwaddr,6);
        wifi_get_macaddr(STATION_IF,hwaddr);
    	char DeviceBuffer[12]={0};
    	wifi_get_macaddr(STATION_IF,hwaddr);
    	os_sprintf(DeviceBuffer,"ESP_%s","34ABE");
    	jsontree_write_string(js_ctx, DeviceBuffer);

    } else if (os_strncmp(path, "action", os_strlen("action")) == 0) {
    	  	jsontree_write_string(js_ctx, "devSend");
    }else if(os_strncmp(path, "msgId", os_strlen("msgId"))==0){

    	jsontree_write_int(js_ctx,msgId++);

    }else if(os_strncmp(path, "appTid", os_strlen("appTid"))==0){

    	jsontree_write_atom(js_ctx, "[");
    	jsontree_write_atom(js_ctx, "]");

    }else if(os_strncmp(path, "vol", os_strlen("vol"))==0){
    	jsontree_write_int(js_ctx,167);
    }else if(os_strncmp(path, "cur", os_strlen("cur"))==0){
    	jsontree_write_int(js_ctx,167);
    }else if(os_strncmp(path,"cmdId",os_strlen("cmdId"))==0){
		jsontree_write_int(js_ctx,1);
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
    	os_sprintf(DeviceBuffer,"ESP_%s","34ABE");
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
    	os_sprintf(DeviceBuffer,"ESP_%s","34ABE");
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
    	os_sprintf(DeviceBuffer,"ESP_%s","34ABE");
    	jsontree_write_string(js_ctx, DeviceBuffer);

    }else if(os_strncmp(path, "action", os_strlen("action")) == 0) {
    	jsontree_write_string(js_ctx, "devLogin");
    }else if(os_strncmp(path, "msgId", os_strlen("msgId"))==0){
    	jsontree_write_int(js_ctx,msgId++);
    }else if(os_strncmp(path, "prodKey", os_strlen("prodKey"))==0){
    	jsontree_write_string(js_ctx, "e15ecc3ee7ddc4527885a6576410de86");
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
                JSONTREE_PAIR(REPORTDEVINFO, &jsonReportDevInfoTrees));

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
                JSONTREE_PAIR(JSONTEST, &jsonTestTrees));


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
                JSONTREE_PAIR(APPRESP, &jsonAppRespTrees));

//============================

LOCAL struct jsontree_callback jsondevSendCallback = 
JSONTREE_CALLBACK(jsonTree_devSend_get,NULL);


JSONTREE_OBJECT(jsonObject2,
                JSONTREE_PAIR("cmdId",&jsondevSendCallback),
                JSONTREE_PAIR("vol", &jsondevSendCallback),
		         JSONTREE_PAIR("cur", &jsondevSendCallback));
JSONTREE_OBJECT(jsonADevSendTrees2,
                JSONTREE_PAIR("devTid", &jsondevSendCallback),
                JSONTREE_PAIR("appTid", &jsondevSendCallback),
				JSONTREE_PAIR("data", &jsonObject2));
JSONTREE_OBJECT(jsonDevSendTrees,
                JSONTREE_PAIR("msgId", &jsondevSendCallback),
                JSONTREE_PAIR("action", &jsondevSendCallback),
				JSONTREE_PAIR("params", &jsonADevSendTrees2));

JSONTREE_OBJECT(jsonDevSendTree,
                JSONTREE_PAIR(DEVSEND, &jsonDevSendTrees));

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
                JSONTREE_PAIR(LOGIN, &jsonLoginTrees));


LOCAL struct jsontree_callback jsonHeatbeatCallback =
    JSONTREE_CALLBACK(jsonTree_get_HeartBeat, jsonTree_set);

JSONTREE_OBJECT(jsonHeartbeatTrees,
                JSONTREE_PAIR("msgId", &jsonHeatbeatCallback),
                JSONTREE_PAIR("action", &jsonHeatbeatCallback));

JSONTREE_OBJECT(jsonHeartbeatTree,
                JSONTREE_PAIR(HEARTBEAT, &jsonHeartbeatTrees));


#define LENGTH 512
static char jsonbuf[LENGTH];
char* ICACHE_FLASH_ATTR
getJsonTree(void)
{

    os_memset(jsonbuf, 0, LENGTH);      //��ʼ���ַ���
	json_ws_send((struct jsontree_value *)&jsonTestTree, JSONTEST, jsonbuf);
	normal_replace(jsonbuf,"\n","");
	strcat(jsonbuf,"\n");
	return jsonbuf;
}

char* ICACHE_FLASH_ATTR
getDevSendTree(void)
{

    os_memset(jsonbuf, 0, LENGTH);      //主动发送数据定义
	json_ws_send((struct jsontree_value *)&jsonDevSendTree, DEVSEND, jsonbuf);
	normal_replace(jsonbuf,"\n","");
	strcat(jsonbuf,"\n");
	return jsonbuf;
}

char* ICACHE_FLASH_ATTR
getAppResponse(void)
{
    os_memset(jsonbuf, 0, LENGTH);      //��ʼ���ַ���
	json_ws_send((struct jsontree_value *)&jsonAppRespTree, APPRESP, jsonbuf);
	normal_replace(jsonbuf,"\n","");
	strcat(jsonbuf,"\n");
	return jsonbuf;
}

char* ICACHE_FLASH_ATTR
getReportDevInfo(void)
{
    os_memset(jsonbuf, 0, LENGTH);      //��ʼ���ַ���
	json_ws_send((struct jsontree_value *)&jsonReportDevInfoTree, REPORTDEVINFO, jsonbuf);
	normal_replace(jsonbuf,"\n","");
	strcat(jsonbuf,"\n");
	return jsonbuf;
}


char* ICACHE_FLASH_ATTR
getLogin(void)
{
    os_memset(jsonbuf, 0, LENGTH);      //��ʼ���ַ���
	json_ws_send((struct jsontree_value *)&jsonLoginTree, LOGIN, jsonbuf);
	normal_replace(jsonbuf,"\n","");
	strcat(jsonbuf,"\n");
	return jsonbuf;
}

char* ICACHE_FLASH_ATTR
getHeartBeat(void)
{
    os_memset(jsonbuf, 0, LENGTH);      //��ʼ���ַ���
	json_ws_send((struct jsontree_value *)&jsonHeartbeatTree, HEARTBEAT, jsonbuf);
	normal_replace(jsonbuf,"\n","");
	strcat(jsonbuf,"\n");
	return jsonbuf;
}


LOCAL int ICACHE_FLASH_ATTR
jsonTree_set(struct jsontree_context *js_ctx, struct jsonparse_state *parser)
{
    int type;
    while ((type = jsonparse_next(parser)) != 0) {
    	//�����KEY����
        if (type == JSON_TYPE_PAIR_NAME) {
            char buffer[64];
            os_bzero(buffer, 64);

            if (jsonparse_strcmp_value(parser, "action") == 0) {
                jsonparse_next(parser);	//���ص���ð���ַ�
                type = jsonparse_next(parser);	//���ص���˫�����ַ�
                os_printf("action Value type = %c\n", type);	// = "

                //���Value���ַ������ͣ����ȡ���ݵ�buffer
                if (JSON_TYPE_STRING == type){	//#define JSON_TYPE_STRING '"'
                	jsonparse_copy_value(parser, buffer, sizeof(buffer));
                	os_printf("action Value = %s\n", buffer);
                }

            }else if (jsonparse_strcmp_value(parser, "msgId") == 0) {
                jsonparse_next(parser);	//���ص���ð���ַ�
                type = jsonparse_next(parser);	//���ص���˫�����ַ�
                os_printf("action msgId type = %c\n", type);	// = "

                //���Value���ַ������ͣ����ȡ���ݵ�buffer
                if (JSON_TYPE_NUMBER == type){	//#define JSON_TYPE_STRING '"'

                	recvtcpAppMsgId = jsonparse_get_value_as_int(parser);

                }

            }
#if 0
            else if (jsonparse_strcmp_value(parser, "Integer") == 0) {
                jsonparse_next(parser);
                type = jsonparse_next(parser);


                os_printf("Integer Value type = %c\n", type);	// = 0
                //���Value����ֵ����
                if(JSON_TYPE_NUMBER == type){	//#define JSON_TYPE_NUMBER '0'
                	//jsonparse_copy_value(parser, buffer, sizeof(buffer));
                	int num = 0;
                	num = jsonparse_get_value_as_int(parser);
                	os_printf("Integer Value = %d\n", num);		// = 1
                }
            } else if (jsonparse_strcmp_value(parser, "Array") == 0) {
                jsonparse_next(parser);	//����ð��
                type = jsonparse_next(parser);
                os_printf("Array Value type = %c\n", type);		// = [

                //���Value����������
                if(JSON_TYPE_ARRAY == type){	//#define JSON_TYPE_ARRAY '['
                	//jsonparse_copy_value(parser, buffer, sizeof(buffer));
                	int length = jsonparse_get_len(parser);
                	os_printf("Array Length = %d\n", length);	//= 5, ������[0,1,2]�����ܰѶ���Ҳ������

                	int i;
                	int num = 100;
                	//ѭ����ȡ���������
                	for(i=0; i<length; i++){
                		type = jsonparse_next(parser);
                		// ����Ƕ��ţ���ֱ�Ӵ�ӡ, �������ֵ���ӡ0
                		os_printf("Array[%d] type = %c ", i, type);

                		//�������ֵ���ͣ���ת����int����ӡ����
                		if(JSON_TYPE_NUMBER==type){
                        	num = jsonparse_get_value_as_int(parser);
                        	os_printf("Array[%d] = %d\n", i, num);
                		}
                		//�����������else if�ж��Ƿ�����������
                		//���� else if(JSON_TYPE_OBJECT==type),�ж��Ƿ���Json����
                		else{
                			os_printf("\n");
                		}
                	}
                }
            }
#endif
#if 1
            else if (jsonparse_strcmp_value(parser, "params") == 0) {
                jsonparse_next(parser);	//����ð��
                type = jsonparse_next(parser);
                os_printf("params Value type = %c\n", type);		// = {

                if(JSON_TYPE_OBJECT == type){	//#define JSON_TYPE_OBJECT '{'
                	int length = jsonparse_get_len(parser);
                	os_printf("params Length = %d\n", length);

                	//char temp[128] = {0};
                	//jsonparse_copy_value(parser, temp, 128);
                	//os_printf("JsonObject Value = %s\n", temp);

                	//ѭ����ȡ����
                	//int i = 0;
                	//for(i=0; i<length; i++){
                	while(type != '}'){
                	//	i++;
                		type = jsonparse_next(parser);

                		//os_memset(temp, 0, 128);
                    	//jsonparse_copy_value(parser, temp, 128);
                    	//os_printf("JsonObject Value = %s\n", temp);
                		//��ȡ�ڶ����Json���������
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
	//�����KEY����
	if (vtype == JSON_TYPE_PAIR_NAME) {
	   if (jsonparse_strcmp_value(parser, "devTid") == 0) {
			jsonparse_next(parser);	//���ص���ð���ַ�
			type = jsonparse_next(parser);	//���ص���˫�����ַ�
			//os_printf("jsonObject String Value type = %c\n", type);	// = "

			//���Value���ַ������ͣ����ȡ���ݵ�buffer
			if (JSON_TYPE_STRING == type){	//#define JSON_TYPE_STRING '"'
				jsonparse_copy_value(parser, buffer, sizeof(buffer));
				os_printf("devTid  Value = %s\n", buffer);
			}

		}else if(jsonparse_strcmp_value(parser,"appTid")==0){
			jsonparse_next(parser);	//���ص���ð���ַ�
			type = jsonparse_next(parser);	//���ص���˫�����ַ�

			//���Value���ַ������ͣ����ȡ���ݵ�buffer
			if (JSON_TYPE_STRING == type){	//#define JSON_TYPE_STRING '"'
				jsonparse_copy_value(parser, buffer, sizeof(buffer));
				os_printf("appTid  Value = %s\n", buffer);
	            os_strcpy(recvtcpAppImei,buffer);
			}

		}else if (jsonparse_strcmp_value(parser, "data") == 0) {
			jsonparse_next(parser);
			type = jsonparse_next(parser);
			//os_printf("jsonObject Integer Value type = %c\n", type);	// = 0
			//���Value����ֵ����
			if(JSON_TYPE_OBJECT == type){
				//jsonparse_copy_value(parser, buffer, sizeof(buffer));
				//os_printf("buffer = %s\n", buffer);
            	int length = jsonparse_get_len(parser);
            	int vlen = parser->vlen;


            	//ѭ����ȡJson��������

				//for(i=0; i<length; i++){
				while(type != '}'){	//��'}'�ж��Ƿ�ﵽ����Objectĩβ

					type = jsonparse_next(parser);


					//�����KEY����
					if (type == JSON_TYPE_PAIR_NAME) {
					   if (jsonparse_strcmp_value(parser, "cmdId") == 0) {
							jsonparse_next(parser);	//���ص���ð���ַ�
							type = jsonparse_next(parser);	//���ص���˫�����ַ�
							//os_printf("K1 Value type = %c\n", type);	// = "

							//���Value���ַ������ͣ����ȡ���ݵ�buffer
							if (JSON_TYPE_NUMBER == type){	//#define JSON_TYPE_STRING '"'

			                	int num = 0;
			                	num = jsonparse_get_value_as_int(parser);
			                	os_printf("cmdId Value = %d\n", num);		// = 1

							}
						}else if(jsonparse_strcmp_value(parser, "raw") == 0){
							jsonparse_next(parser);	//���ص���ð���ַ�
							type = jsonparse_next(parser);	//���ص���˫�����ַ�

							//���Value���ַ������ͣ����ȡ���ݵ�buffer
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
            	os_printf("���ص�bin��ַ:%s",upgradebinUrl);
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
 * ͳ��key��data�г��ֵĴ���
 * @param data �����ҵ��ַ���
 * @param key  Ҫ���ҵ��ַ���
 * @return key��data�г��ֵĴ���
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
 * ��data�е�rep�ַ����滻��to�ַ������Զ�̬�����ڴ淽ʽ�������ַ���
 * �����������Ҫ��֤data�ܱ�֤������
 * @param data ���滻ĳЩ�ַ���������
 * @param rep  ���滻���ַ���
 * @param to   �滻�ɵ��ַ���
 * @param free_data ��Ϊ0ʱҪ�ͷ�data���ڴ�
 * @return �����·����ڴ���滻��ɵ��ַ�����ע���ͷš�
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
 * ��data�е�rep�ַ����滻��to�ַ���
 * ע�Ᵽ֤data�ռ��㹻�滻��ɺ���ַ�������
 * @param data ���滻ĳЩ�ַ���������
 * @param rep  ���滻���ַ���
 * @param to   �滻�ɵ��ַ���
 * @return ��
 */
void ICACHE_FLASH_ATTR normal_replace(char *data, char *rep, char *to)
{
    char *new_buf = malloc_replace(data, rep, to, 1);
    if (NULL != new_buf) {
        strcpy(data, new_buf);
        os_free(new_buf);
    }
}



