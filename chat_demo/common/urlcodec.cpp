#include "urlcodec.h"

static unsigned char toHex(unsigned char x)
{
	return x > 9 ? x + 55 : x + 48;
}

std::string UrlEncode(const std::string & sIn)
{
	std::string sOut;
	for(size_t i=0; i < sIn.size(); ++i)
	{
		unsigned char buf[4];
		memset(buf, 0, 4);
		if(isalnum((unsigned char)sIn[i]))
		{
			buf[0] = sIn[i];
		}
		else if(isspace((unsigned char)sIn[i]))
		{
			buf[0] = '+';
		}
		else
		{
			buf[0] = '%';
			buf[1] = toHex((unsigned char)sIn[i] >> 4);
			buf[2] = toHex((unsigned char)sIn[i] % 16);
		}
		sOut += (char *)buf;
	}
	return sOut;
}

std::string UrlDecode(const std::string & sIn)
{
	std::string sOut;
	for(size_t i = 0; i < sIn.size(); i++)
	{
		unsigned char buf[4];
		memset(buf, 0, 4);
		if( isalnum( sIn[i] ) )
		{
			buf[0] = sIn[i];
		}
		else if ( '+'==( sIn[i] ) )
		{
			buf[0] = ' ';
		}
		else
		{
			buf[0] = toHex( sIn[i + 1] << 4 );
			buf[1] = toHex( sIn[i]);
		}
		sOut += (char *)buf;
	}
	return sOut;
}
