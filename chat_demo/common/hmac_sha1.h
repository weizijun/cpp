#ifndef __HMAC_SHA1_H__
#define __HMAC_SHA1_H__
/*
	CHMAC_SHA1.h
	HMAC-SHA1 algorithm
	@author: Josh Wolfhead

	100% free public domain implementation of the HMAC-SHA-1 algorithm

	algorithm:
	Let:

    * H(¡¤) be a cryptographic hash function
    * K be a secret key padded to the right with extra zeros to the input block size of the hash function, or the hash of the original key if it's longer than that block size
    * m be the message to be authenticated
    * ¡Î denote concatenation
    * ¨’ denote exclusive or (XOR)
    * opad be the outer padding (0x5c5c5c¡­5c5c, one-block-long hexadecimal constant)
    * ipad be the inner padding (0x363636¡­3636, one-block-long hexadecimal constant)

	Then HMAC(K,m) is mathematically defined by

    HMAC(K,m) = H((K ¨’ opad) ¡Î H((K ¨’ ipad) ¡Î m)). 

	The following pseudocode demonstrates how HMAC may be implemented.

	function hmac (key, message)
		if (length(key) > blocksize) then
			key = hash(key) // keys longer than blocksize are shortened
		end if
		if (length(key) < blocksize) then
			key = key || zeroes(blocksize - length(key)) // keys shorter than blocksize are zero-padded('||' is concatenation) 
		end if
    
		o_key_pad = [0x5c * blocksize] ¨’ key // Where blocksize is that of the underlying hash function
		i_key_pad = [0x36 * blocksize] ¨’ key // Where ¨’ is exclusive or (XOR)
    
		return hash(o_key_pad || hash(i_key_pad || message)) // Where '||' is concatenation
	end function


*/
#include "sha1.h"
#include <string>

#define STRING_TYPE_LEN (-1)

class CHMAC_SHA1
{
public:

	static void binary(const char * data, const char * key, unsigned char * digest, int dataLen = STRING_TYPE_LEN, int keyLen = STRING_TYPE_LEN); 

	static std::string hex(const char * data, const char * key, int dataLen = STRING_TYPE_LEN, int keyLen = STRING_TYPE_LEN);

protected:
	enum{
		SHA1_DIGEST_LENGTH	= 20,
		SHA1_BLOCK_SIZE		= 64,
	};

	static void _generate(const char * _data, const char * _key, int _dataLen, int _keyLen, unsigned char * digest);
	static std::string _toHex(const unsigned char *);
};

#endif
