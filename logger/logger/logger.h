#ifndef __LOGGER_H__
#define __LOGGER_H__

#include "common/properties.h"
#include "baselogger.h"
#include "printlogger.h"
#include "filelogger.h"
#include "nulllogger.h"

#if (defined(WIN32) || defined(WIN64))
#define LOG_MESSAGE(Level, pFormat, ...) \
	KM::CLogger::Instance()->Log((Level), __FILE__, __LINE__, __FUNCTION__, pFormat, __VA_ARGS__)
#elif (defined(LINUX))
#define LOG_MESSAGE(pILogger, Level, ...) \
	KM::CLogger::Instance()->Log((Level), __FILE__, __LINE__, __PRETTY_FUNCTION__, __VA_ARGS__)
#endif

#define LOG_FATAL(...) \
	LOG_MESSAGE( 1, __VA_ARGS__)

#define LOG_ERROR(...) \
	LOG_MESSAGE( 3, __VA_ARGS__)

#define LOG_WARN(...) \
	LOG_MESSAGE( 4, __VA_ARGS__)

#define LOG_INFO(...) \
	LOG_MESSAGE(5, __VA_ARGS__)

#define LOG_DEBUG(...) \
	LOG_MESSAGE(6, __VA_ARGS__)


namespace KM
{
	class CLogger
	{
	public:
		CLogger();
		~CLogger();

	public:
		static CLogger* Instance();
		int Init(const char * pConfigFile);
		void LOGGERAPI Log(int Level,const char *pFile,int iLine,const char *pFunction,const char* pFormat,...);

	private:
		CPrintLogger *m_PrintLogProxy;
		CFileLogger *m_FileLogProxy;
		bool m_PrintFlag;
		bool m_FileFlag;
	};
}

#endif