#include "logger.h"
#include "common/Unitity.h"

KM::CLogger::CLogger() :
m_PrintFlag(false),
m_FileFlag(false)
{

}
KM::CLogger::~CLogger()
{
	delete m_PrintLogProxy;
	delete m_FileLogProxy;
}

KM::CLogger* KM::CLogger::Instance()
{
	static CLogger LogMgr;
	return &LogMgr;
}

int KM::CLogger::Init(const char * pConfigFile)
{
	CProperties t_Config;
	if (0 >= t_Config.ImportProperties(pConfigFile))
	{
		//m_LogProxy = new (std::nothrow) CNullLogger;
		return -1;
	}

	std::string strLoggerType = t_Config.GetProperty<std::string>("LoggerType", "Print");
	std::vector<std::string> t_TypeVec;
	Spliter(strLoggerType,t_TypeVec,'|');
	for (std::vector<std::string>::iterator it = t_TypeVec.begin(); it != t_TypeVec.end(); ++it)
	{
		if (it->compare("Print") == 0)
		{
			m_PrintLogProxy = new (std::nothrow) CPrintLogger();
			if (NULL != m_PrintLogProxy)
			{
				m_PrintLogProxy->SetColor(t_Config.GetProperty<bool>("useColor", true));
				m_PrintLogProxy->SetDate(t_Config.GetProperty<bool>("useDate", true));
				m_PrintLogProxy->SetLocation(t_Config.GetProperty<bool>("useLocation",true));
				m_PrintLogProxy->SetLogLevel(t_Config.GetProperty<bool>("useLevel",true));
				m_PrintLogProxy->SetLowestLevel(t_Config.GetProperty<int>("LowestLevel",5));
				m_PrintFlag = true;
			}
			
		}
		else if (it->compare("File") == 0)
		{
			m_FileLogProxy = new (std::nothrow) CFileLogger();
			if (NULL != m_FileLogProxy)
			{
				m_FileLogProxy->SetColor(t_Config.GetProperty<bool>("useColor", true));
				m_FileLogProxy->SetDate(t_Config.GetProperty<bool>("useDate", true));
				m_FileLogProxy->SetLocation(t_Config.GetProperty<bool>("useLocation",true));
				m_FileLogProxy->SetLogLevel(t_Config.GetProperty<bool>("useLevel",true));
				m_FileLogProxy->SetLowestLevel(t_Config.GetProperty<int>("LowestLevel",5));
				m_FileLogProxy->SetFilePath(t_Config.GetProperty<std::string>("FilePath","").c_str());
				m_FileLogProxy->SetFilePrefix(t_Config.GetProperty<std::string>("FilePrefix","log").c_str());
				m_FileLogProxy->SetFileRolling(t_Config.GetProperty<int>("Rolling", 24));
				m_FileFlag = true;
			}
		}
	}

	return 0;
}

void KM::CLogger::Log(int Level,const char *pFile,int iLine,const char *pFunction,const char* pFormat,...)
{
	va_list ap;
	va_start(ap,pFormat);
	if (m_PrintFlag == true)
	{
		m_PrintLogProxy->Log(Level,pFile,iLine,pFunction,pFormat,ap);
	}
	
	if (m_FileFlag == true)
	{
		m_FileLogProxy->Log(Level,pFile,iLine,pFunction,pFormat,ap);
	}	
	va_end(ap);
}