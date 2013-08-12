#ifndef __MD5_H__
#define __MD5_H__

#include <string>

/**
* @brief
* get md5 for a string buffer ��ȡһ���ַ�����md5��
* @param acDigest : [in/out]array to store md5 val [�����������]�洢md5�������
* @param pInput : input string �����ַ���
* @param dwInLen : the length fo input string �����ַ����ĳ���
* @return void
*/
void MD5(unsigned char acDigest[16], unsigned char* pInput, unsigned int dwInLen);

std::string GetMD5(const char * str, int len);

std::string GetBigMD5(const char * str, int len);

#endif

