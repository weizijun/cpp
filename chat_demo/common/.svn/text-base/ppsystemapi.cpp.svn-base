#include "ppbase.h"
#include "ppsystemapi.h"
#include "ppwindowsapi.h"
#include "pplinuxapi.h"
#include <new>

using namespace PPCool;

IPPSystemAPI * IPPSystemAPI::m_pInstance = NULL;

IPPSystemAPI::IPPSystemAPI()
{
}

IPPSystemAPI::~IPPSystemAPI()
{
}

IPPSystemAPI* IPPSystemAPI::Instance()
{
	if(NULL == m_pInstance)
	{
#if (defined(WIN32) || defined(WIN64))
		m_pInstance = new (std::nothrow) CPPWindowsAPI();
#else
		m_pInstance = new (std::nothrow) CPPLinuxAPI();
#endif
	}
	return m_pInstance;
}

void IPPSystemAPI::DestroyInstance()
{
	if(NULL != m_pInstance)
	{
		delete m_pInstance;
	}
	m_pInstance = NULL;
}
