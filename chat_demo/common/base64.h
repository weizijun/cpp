#ifndef ___BASE64_H___
#define ___BASE64_H___
#include <string> 
  
class CBase64
{
public:
    CBase64();
    ~CBase64();

    /*********************************************************
    * ����˵�������������ݽ���base64����
    * ����˵����[in]pIn      ��Ҫ���б��������
    *           [in]uInLen   ����������ֽ���
    *           [out]strOut  ����Ľ���base64����֮����ַ���
    * ����ֵ  ��true����ɹ�,falseʧ��
    **********************************************************/
	bool static Encode(const unsigned char *pIn, unsigned long uInLen, std::string& strOut);

    /*********************************************************
    * ����˵�������������ݽ���base64����
    * ����˵����[in]pIn          ��Ҫ���б��������
    *           [in]uInLen       ����������ֽ���
    *           [out]pOut        ����Ľ���base64����֮����ַ���
    *           [out]uOutLen     ����Ľ���base64����֮����ַ�������
    * ����ֵ  ��true����ɹ�,falseʧ��
    **********************************************************/
    bool static Encode(const unsigned char *pIn, unsigned long uInLen, unsigned char *pOut, unsigned long *uOutLen);

    /*********************************************************
    * ����˵�������������ݽ���base64����
    * ����˵����[in]strIn        ��Ҫ���н��������
    *           [out]pOut        �������֮��Ľ�������
    *           [out]uOutLen     ����Ľ���֮����ֽ�������
    * ����ֵ  ��true����ɹ�,falseʧ��
    **********************************************************/
	bool static Decode(const std::string& strIn, unsigned char *pOut, unsigned long *uOutLen);

    /*********************************************************
    * ����˵�������������ݽ���base64����
    * ����˵����[in]strIn        ��Ҫ���н��������
    *           [out]pOut        �������֮��Ľ�������
    *           [out]uOutLen     ����Ľ���֮����ֽ�������
    * ����ֵ  ��true����ɹ�,falseʧ��
    **********************************************************/
    bool static Decode(const unsigned char *pIn, unsigned long uInLen, unsigned char *pOut, unsigned long *uOutLen);
};

#endif // ___BASE64_H___
