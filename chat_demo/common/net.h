#ifndef __PPCOOL_NET_H__
#define __PPCOOL_NET_H__

#include <string>

int GetLocalIP(unsigned long ip[], int count);

std::string GetMacAddr();

//��һ�����ʮ���Ƶ�IPת����һ����������(u_long����)
//in_addr_t inet_addr(const char *cp);

//��һ����������ת��Ϊһ�����ʮ���Ƶ�IP
//const char * inet_ntoa()

#endif
