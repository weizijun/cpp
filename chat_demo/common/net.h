#ifndef __PPCOOL_NET_H__
#define __PPCOOL_NET_H__

#include <string>

int GetLocalIP(unsigned long ip[], int count);

std::string GetMacAddr();

//将一个点分十进制的IP转换成一个长整型数(u_long类型)
//in_addr_t inet_addr(const char *cp);

//将一个长整型数转化为一个点分十进制的IP
//const char * inet_ntoa()

#endif
