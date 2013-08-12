#ifndef __CLIENT_MANAGER_H__
#define __CLIENT_MANAGER_H__

#include <string>
#include <set>
#include "common.h"
#include "lock.h"

class CClient
{
public:
	CClient();
	CClient(const CClient & rhs);
	~CClient();
	CClient& operator = (const CClient& rhs);

public:
	std::string _client_ip;
	int _client_port;

	bool _connected;
	bufferevent *_bvt;
	
};
class CClientManger
{
public:
	CClientManger();
	~CClientManger();

	bool ClientConnected(bufferevent* bvt);
	bool CClientDisConnected(bufferevent* bvt);
	bool ReConnectClient(std::string ip,int port,bufferevent* bvt);

	void CheckReConnectAllClient();
	bool AddClient(std::string ip,int port,bufferevent* bvt);
	bool RemoveClient(bufferevent* bvt);
	std::set<bufferevent*> GetConnections();
	bool GetClient(bufferevent* bvt,CClient &client);

private:
	CLock _lock;
	std::set<bufferevent *> _connections;
	std::map<bufferevent *, CClient> _clientsConns;

};


#endif