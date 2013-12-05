#include "baselogger.h"
#include <time.h>
#include <string>
#include <sstream>
#include <stdarg.h>
#include <stdio.h>


KM::CBaseLogger::CBaseLogger() :
m_LogDate(true),
m_LowestLevel(5),
m_LogLevel(true),
m_Location(false)
{
	m_LogPrefix[0] = "";
	m_LogPrefix[1] = "[LOG_FATAL]";
	m_LogPrefix[2] = "[LOG_CRIT]";
	m_LogPrefix[3] = "[ERROR]";
	m_LogPrefix[4] = "[LOG_WARN]";
	m_LogPrefix[5] = "[LOG_INFO]";
	m_LogPrefix[6] = "[LOG_DEBUG]";
	m_LogPrefix[7] = "[LOG_DETAIL]";
}

KM::CBaseLogger::~CBaseLogger()
{

}

int KM::CBaseLogger::_FormatColor(int Level,std::string &str)
{
	return 0;
}

int KM::CBaseLogger::_FormatDate(std::string &str)
{
	time_t now;
	struct tm t;
	time(&now);
	t = *localtime(&now);

	char szFormatTime[24];
	sprintf(szFormatTime,"[%4d-%02d%02d %02d:%02d:%02d]",t.tm_year+1900,t.tm_mon+1,t.tm_mday,t.tm_hour,t.tm_min,t.tm_sec);

	str.append(szFormatTime);
	return 0;
}

int KM::CBaseLogger::_FormatFile(const char * pFile,std::string &str)
{
	str.append(pFile);
	return 0;
}

int KM::CBaseLogger::_FormatLine(int iLine,std::string &str)
{
	std::ostringstream os;
	os << iLine;
	str.append(os.str());
	return 0;
}

int KM::CBaseLogger::_FormatFuncion(const char *pFunction,std::string &str)
{
	str.append(pFunction);
	return 0;
}

void  KM::CBaseLogger::Release()
{
	delete this;
}

void KM::CBaseLogger::SetLowestLevel(int Level)
{
	m_LowestLevel = Level;
}

void KM::CBaseLogger::SetLogLevel(bool Level)
{
	m_LogLevel = Level;
}

void KM::CBaseLogger::SetDate(bool Date)
{
	m_LogDate = Date;
}

void KM::CBaseLogger::SetLocation(bool Location)
{
	m_Location = Location;
}

void KM::CBaseLogger::SetColor(bool Color)
{
	m_Color = Color;
}

void KM::CBaseLogger::Log(int Level,const char *pFile,int iLine,const char *pFunction,const char* pFormat,...)
{
	if (m_LowestLevel < Level || Level < 0)
	{
		return ;
	}

	std::string strLog;
	strLog.reserve(64);

#if (defined(WIN32) || defined(WIN64))
	if (m_Color && 1 == m_LogType)
	{
		_FormatColor(Level,strLog);
	}
#else
	if (m_Color && (1 == m_LogType || 2 == m_LogType))
	{
		_FormatColor(Level,strLog);
	}

#endif
	if (m_LogDate)
	{
		_FormatDate(strLog);
	}

	if (m_LogLevel)
	{
		strLog.append(m_LogPrefix[Level]);
	}

	char msg[2048];
	msg[sizeof(msg) - 1] = '\0';
	va_list ap;
	va_start(ap,pFormat);
	vsnprintf(msg,sizeof(msg),pFormat,ap);
	va_end(ap);
	strLog.append(msg);

	if (m_Location)
	{
		strLog.append("(");
		_FormatFile(pFile,strLog);		
		strLog.append(":");
		_FormatLine(iLine,strLog);
		strLog.append(" ");
		_FormatFuncion(pFunction,strLog);
		strLog.append(")");
	}

	_Log(strLog.c_str());
}