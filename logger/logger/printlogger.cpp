#include "printlogger.h"

KM::CPrintLogger::CPrintLogger()
{
	m_LogType = 1;
}

KM::CPrintLogger::~CPrintLogger()
{

}

void KM::CPrintLogger::_Log(const char* pMsg)
{
#if (defined(WIN32) || defined(WIN64))
	printf("%s\r\n",pMsg);
#else
	printf("%s\n",pMsg);
#endif
}