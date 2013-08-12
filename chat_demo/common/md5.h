#ifndef __MD5_H__
#define __MD5_H__

#include <string>

/**
* @brief
* get md5 for a string buffer 获取一个字符串的md5码
* @param acDigest : [in/out]array to store md5 val [输入输出参数]存储md5码的数组
* @param pInput : input string 输入字符串
* @param dwInLen : the length fo input string 输入字符串的长度
* @return void
*/
void MD5(unsigned char acDigest[16], unsigned char* pInput, unsigned int dwInLen);

std::string GetMD5(const char * str, int len);

std::string GetBigMD5(const char * str, int len);

#endif

