#include "nulllogger.h"

KM::CNullLogger::CNullLogger()
{
	m_LogType = 0;
}

KM::CNullLogger::~CNullLogger()
{

}

void KM::CNullLogger::_Log(const char * pMsg)
{

}