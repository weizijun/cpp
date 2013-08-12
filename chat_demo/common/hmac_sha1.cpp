#include "hmac_sha1.h"
#include <stdlib.h>
#include <stdio.h>
using namespace std;


void CHMAC_SHA1::_generate(const char * _data, const char * _key, int _dataLen, int _keyLen, unsigned char * digest)
{
	CSHA1 csha1;

	unsigned char ipad[SHA1_BLOCK_SIZE] = {0};
	unsigned char opad[SHA1_BLOCK_SIZE] = {0};
	unsigned char key[SHA1_BLOCK_SIZE] = {0};

	if (_keyLen > SHA1_BLOCK_SIZE)
	{
		csha1.Reset();
		csha1.Update((unsigned char*)_key, _keyLen);
		csha1.Final();

		csha1.GetHash((UINT_8 *)key);
	}
	else
	{
		memcpy(key, _key, _keyLen); // leave 0 at the end of the key buffer
	}

	/* repeated SHA1_BLOCK_SIZE times for values in ipad and opad */
	memset(ipad, 0x36, SHA1_BLOCK_SIZE);
	memset(opad, 0x5c, SHA1_BLOCK_SIZE);

	for (int i = 0; i != SHA1_BLOCK_SIZE ; ++i)
	{
		ipad[i] ^= key[i];		
	}

	for (int j = 0; j != SHA1_BLOCK_SIZE ; ++j)
	{
		opad[j] ^= key[j];
	}
		
	string buffer;
	buffer.append((const char*)ipad, SHA1_BLOCK_SIZE);
	buffer.append((const char*)_data, _dataLen);

	csha1.Reset();
	csha1.Update((UINT_8 *)buffer.data(), SHA1_BLOCK_SIZE + _dataLen);
	csha1.Final();

	csha1.GetHash((UINT_8 *)digest);

	buffer.clear();
	buffer.append((const char*)opad, SHA1_BLOCK_SIZE);
	buffer.append((const char*)digest, SHA1_DIGEST_LENGTH);
	csha1.Reset();
	csha1.Update((UINT_8 *)buffer.data(), SHA1_BLOCK_SIZE + SHA1_DIGEST_LENGTH);
	csha1.Final();

	csha1.GetHash((UINT_8 *)digest);

	return;
}

std::string CHMAC_SHA1::_toHex(const unsigned char* digest)
{
	char buffer[41] = {0};
	char high = 0, low = 0;

	for (int i = 0; i != SHA1_DIGEST_LENGTH; ++i)
	{
		high = digest[i] >> 4;
		low = digest[i] & 0x0F;
		buffer[i * 2] = (high > 9) ? (high - 10 + 'a') : high + '0';
		buffer[i * 2 + 1] = (low > 9) ? (low - 10 + 'a') : low + '0';
	}

	return string(buffer);
}

std::string CHMAC_SHA1::hex( const char * data, const char * key, int dataLen /*= STRING_TYPE_LEN*/, int keyLen /*= STRING_TYPE_LEN*/ )
{
	int _dataLen = dataLen == STRING_TYPE_LEN ? strlen(data) : dataLen;
	int _keyLen = keyLen == STRING_TYPE_LEN ? strlen(key) : keyLen;

	unsigned char digest[20] = {0};
	_generate(data, key, _dataLen, _keyLen, digest);

	return _toHex(digest);
}

void CHMAC_SHA1::binary( const char * data, const char * key, unsigned char * digest, int dataLen /*= STRING_TYPE_LEN*/, int keyLen /*= STRING_TYPE_LEN*/ )
{
	int _dataLen = dataLen == STRING_TYPE_LEN ? strlen(data) : dataLen;
	int _keyLen = keyLen == STRING_TYPE_LEN ? strlen(key) : keyLen;
	_generate(data, key, _dataLen, _keyLen, digest);
	return;
}
