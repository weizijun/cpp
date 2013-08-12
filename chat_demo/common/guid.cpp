#include "guid.h"
#if (defined(WIN32) || defined(WIN64))
#include <WinSock2.h>
#include <iphlpapi.h>
#pragma comment(lib, "ws2_32.lib") 
#pragma comment(lib, "Iphlpapi.lib") 
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#endif
#include <time.h>

IMPLEMENT_SINGLETON(CGuid)

unsigned int CGuid::m_SeqID = 1;

CGuid::CGuid()
{
}

CGuid::~CGuid()
{
}

CGuid::CGuid(const CGuid &)
{
}

CGuid& CGuid::operator=(const CGuid & rhs)
{
	if(&rhs != this)
	{
		//参数不是本身时，才有操作意义
	}
	return *this;
}

bool CGuid::Init()
{
	unsigned long ip[10];
	int ipcount = CGuid::GetLocalIP(ip, 10);
	if(ipcount <= 0)
	{
		return false;
	}
	m_LocalIP = ip[0];
	m_strMacAddr = CGuid::GetMacAddr();
	if("" == m_strMacAddr)
	{
		return false;
	}
	return true;
}

std::string CGuid::GetGuid()
{
	time_t now;
	time(&now);

	int randcount = CGuid::GetRandom((unsigned int)now + m_SeqID++);

	char GUID[100];
	sprintf(GUID, "%12s%012ld%06d%02d", m_strMacAddr.c_str(), (long)now, randcount, (m_SeqID++)%99);
	return GUID;
}

int CGuid::GetLocalIP(unsigned long ip[], int count)
{
	int ipcount = 0;
#if (defined(WIN32) || defined(WIN64))
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
	char hostname[1024];
	if (gethostname(hostname, sizeof(hostname)) != 0)
	{
		return 0;
	}

	LPHOSTENT lphost = gethostbyname(hostname);
	if (lphost == NULL)
	{
		return 0;
	}

	struct in_addr *addr;
	while(lphost->h_addr_list[ipcount] != NULL && count-- > 0)
	{
		addr=(struct in_addr *)lphost->h_addr_list[ipcount];
		ip[ipcount++] = addr->S_un.S_addr;
	}
#else
	int fd;
	int interface;
	struct ifreq buf[10];

	struct ifconf ifc;

	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		return 0;
	}

	ifc.ifc_len = sizeof(buf);
	ifc.ifc_buf = (char*) buf;
	if (ioctl(fd, SIOCGIFCONF, (char *) &ifc))
	{
		close(fd);
		return 0;
	}

	interface = ifc.ifc_len / sizeof(struct ifreq);
	interface = interface < 10 ? interface : 10;
	int iplocal = inet_addr("127.0.0.1");
	while (interface-- > 0 && count-- > 0)
	{
		/*Get IP of the net card */
		if ((ioctl(fd, SIOCGIFADDR, (char *) &buf[interface])))
		{
			continue;
		}

		long tmp_ip = ((struct sockaddr_in*)(&buf[interface].ifr_addr))->sin_addr.s_addr;
		if(tmp_ip == iplocal)
		{
			continue;
		}
		ip[ipcount++] = tmp_ip;
	}
	close(fd);
#endif
	return ipcount;
}

std::string CGuid::GetMacAddr()
{
#if (defined(WIN32) || defined(WIN64))
	DWORD Err;

    PIP_ADAPTER_INFO pAdapterInfo, pAdapt;
    DWORD AdapterInfoSize;

	//获取网卡适配器信息的大小
    AdapterInfoSize = 0;
    if ((Err = GetAdaptersInfo(NULL, &AdapterInfoSize)) != 0)
    {
        if (Err != ERROR_BUFFER_OVERFLOW)
        {
            return "";
        }
    }

    //为网卡适配器信息分配空间
    if (NULL == (pAdapterInfo = (PIP_ADAPTER_INFO)malloc(AdapterInfoSize)))
    {
        return "";
    }

    //获取网卡适配器信息
    if ((Err = GetAdaptersInfo(pAdapterInfo, &AdapterInfoSize)) != 0)
    {
        return "";
    }

    pAdapt = pAdapterInfo;

	char t_MacAddr[33];
	char * pMacAddr = t_MacAddr;
    while (pAdapt)
    {
        for (UINT i=0; i<pAdapt->AddressLength; i++)
        {
			sprintf(pMacAddr, "%.2X",(int)pAdapt->Address[i]);
			pMacAddr += 2;
        }
		break;
        pAdapt = pAdapt->Next;
    }
	return t_MacAddr;
#else
	struct ifreq ifreq;
	int fd;

	if((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		return "";
	}
	strcpy(ifreq.ifr_name, "eth0");
	if(ioctl(fd, SIOCGIFHWADDR, &ifreq) < 0)
	{
		close(fd);
		return "";
	}
	char t_MacAddr[33];
	sprintf(t_MacAddr, "%02X%02X%02X%02X%02X%02X", 
		(unsigned char)ifreq.ifr_hwaddr.sa_data[0],
		(unsigned char)ifreq.ifr_hwaddr.sa_data[1],
		(unsigned char)ifreq.ifr_hwaddr.sa_data[2],
		(unsigned char)ifreq.ifr_hwaddr.sa_data[3],
		(unsigned char)ifreq.ifr_hwaddr.sa_data[4],
		(unsigned char)ifreq.ifr_hwaddr.sa_data[5]
	);
	close(fd);
	return t_MacAddr;
#endif
}

int CGuid::GetRandom(unsigned int seed)
{
	srand(seed);
	return rand()%65535;
}
