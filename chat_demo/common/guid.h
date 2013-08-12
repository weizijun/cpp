#ifndef __GUID_H__
#define __GUID_H__

#include "singleton.h"
#include <string>

class CGuid
{
DEFINE_SINGLETON(CGuid)

private:
	CGuid();
	~CGuid();
	CGuid(const CGuid & rhs);
	CGuid& operator=(const CGuid & rhs);

public:
	bool Init();

	std::string GetGuid();

private:
	static int GetLocalIP(unsigned long ip[], int count);
	static std::string GetMacAddr();
	static int GetRandom(unsigned int seed);

private:
	static unsigned int m_SeqID;
	unsigned long m_LocalIP;
	std::string m_strMacAddr;
};

#endif
