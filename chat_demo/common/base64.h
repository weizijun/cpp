#ifndef ___BASE64_H___
#define ___BASE64_H___
#include <string> 
  
class CBase64
{
public:
    CBase64();
    ~CBase64();

    /*********************************************************
    * 函数说明：将输入数据进行base64编码
    * 参数说明：[in]pIn      需要进行编码的数据
    *           [in]uInLen   输入参数的字节数
    *           [out]strOut  输出的进行base64编码之后的字符串
    * 返回值  ：true处理成功,false失败
    **********************************************************/
	bool static Encode(const unsigned char *pIn, unsigned long uInLen, std::string& strOut);

    /*********************************************************
    * 函数说明：将输入数据进行base64编码
    * 参数说明：[in]pIn          需要进行编码的数据
    *           [in]uInLen       输入参数的字节数
    *           [out]pOut        输出的进行base64编码之后的字符串
    *           [out]uOutLen     输出的进行base64编码之后的字符串长度
    * 返回值  ：true处理成功,false失败
    **********************************************************/
    bool static Encode(const unsigned char *pIn, unsigned long uInLen, unsigned char *pOut, unsigned long *uOutLen);

    /*********************************************************
    * 函数说明：将输入数据进行base64解码
    * 参数说明：[in]strIn        需要进行解码的数据
    *           [out]pOut        输出解码之后的节数数据
    *           [out]uOutLen     输出的解码之后的字节数长度
    * 返回值  ：true处理成功,false失败
    **********************************************************/
	bool static Decode(const std::string& strIn, unsigned char *pOut, unsigned long *uOutLen);

    /*********************************************************
    * 函数说明：将输入数据进行base64解码
    * 参数说明：[in]strIn        需要进行解码的数据
    *           [out]pOut        输出解码之后的节数数据
    *           [out]uOutLen     输出的解码之后的字节数长度
    * 返回值  ：true处理成功,false失败
    **********************************************************/
    bool static Decode(const unsigned char *pIn, unsigned long uInLen, unsigned char *pOut, unsigned long *uOutLen);
};

#endif // ___BASE64_H___
