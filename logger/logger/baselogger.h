#ifndef __BASE_LOGGER_H__
#define __BASE_LOGGER_H__

#include "config.h"
#include <string>

namespace KM
{
	enum {
		LOG_FATAL = 1,
		LOG_CRIT,
		LOG_ERROR,
		LOG_WARN,
		LOG_INFO,
		LOG_DEBUG,
		LOG_DETAIL
	};

	class CBaseLogger
	{
	public:
		CBaseLogger();
		virtual ~CBaseLogger();

		void Release();

		void SetLowestLevel(int Level);
		void SetLogLevel(bool Level);
		void SetDate(bool Date);
		void SetLocation(bool Location);
		void SetColor(bool Color);

		void Log(int Level,const char *pFile,int iLine,const char *pFunction,const char* pFormat,...);

	private:
		int _FormatColor(int Level,std::string &str);
		int _FormatDate(std::string &str);
		int _FormatFile(const char * pFile,std::string &str);
		int _FormatLine(int iLine,std::string &str);
		int _FormatFuncion(const char *pFunction,std::string &str);

	protected:
		virtual void _Log(const char * pMsg) = 0;
		int m_LogType;

	private:
		const char * m_LogPrefix[8];
		bool m_LogDate;
		int m_LowestLevel;
		bool m_LogLevel;
		bool m_Location;
		bool m_Color;
	};
}

#endif