#include "client_manger.h"

CClient::CClient() :
_connected(false)
{ }

CClient::CClient(const CClient & rhs)
{
	if (this == &rhs)
	{
		return ;
	}

	_bvt = rhs._bvt;
	_client_ip = rhs._client_ip;
	_client_port = rhs._client_port;
	_connected = rhs._connected;
}

CClient::~CClient()
{

}

CClient& CClient::operator = (const CClient & rhs)
{
	if (this == &rhs)
	{
		return *this;
	}

	_bvt = rhs._bvt;
	_client_ip = rhs._client_ip;
	_client_port = rhs._client_port;
	_connected = rhs._connected;

	return *this;
}