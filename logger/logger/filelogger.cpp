#include "filelogger.h"
#include <time.h>

#if (defined(WIN32) || defined(WIN64))

bool KM::CFileLogger::_FindFirstFileExists(LPCTSTR lpPath,DWORD dwFilter)
{
	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile(lpPath,&fd);
	bool bFilter = (FALSE == dwFilter) ? TRUE : fd.dwFileAttributes & dwFilter;
	bool RetValue = ((hFind != INVALID_HANDLE_VALUE) && bFilter) ? TRUE : FALSE;
	FindClose(hFind);
	return RetValue;
}

bool KM::CFileLogger::_FilePathExists(LPCTSTR lpPath)
{
	return _FindFirstFileExists(lpPath,FALSE);
}

bool KM::CFileLogger::_FolderExists(LPCTSTR lpPath)
{
	return _FindFirstFileExists(lpPath,FILE_ATTRIBUTE_DIRECTORY);
}

void KM::CFileLogger::_CreateFolder(const char* FilePath)
{
	char FilePath2[MAX_PATH];
	memset(FilePath2,0,MAX_PATH);
	for (int i = 0; i < MAX_PATH; ++i)
	{
		if (FilePath[i] == '\\' || FilePath[i] == '/')
		{
			CreateDirectory(FilePath2,NULL);
		}
		FilePath2[i] = FilePath[i];
	}
}

#endif

KM::CFileLogger::CFileLogger() :
m_strFilePath(""),
m_strFilePrefix("log"),
m_Rolling(24)
{
	m_LogType = 2;
}

KM::CFileLogger::~CFileLogger()
{

}

void KM::CFileLogger::SetFilePath(const char *pFilePath)
{
	if (NULL != pFilePath)
	{
		m_strFilePath = pFilePath;
		if (m_strFilePath.length() >= 1)
		{
#if (defined(WIN32) || defined(WIN64))
			if ('\\' == m_strFilePath[m_strFilePath.length() - 1] ||
				 '/' == m_strFilePath[m_strFilePath.length() - 1])
			{
				m_strFilePath[m_strFilePath.length()-1] = '\\';
			}
			else
			{
				m_strFilePath += '\\';
			}
#else
			if ('\\' == m_strFilePath[m_strFilePath.length() - 1] ||
				'/' == m_strFilePath[m_strFilePath.length() - 1])
			{
				m_strFilePath[m_strFilePath.length()-1] = '/';
			}
			else
			{
				m_strFilePath += '/';
			}
#endif

#if (defined(WIN32) || defined(WIN64))
			if (!_FolderExists(m_strFilePath.c_str()))
			{
				_CreateFolder(m_strFilePath.c_str());
			}
#endif
		}
	}
}

void KM::CFileLogger::SetFilePrefix(const char *pFilePrefix)
{
	if (NULL != pFilePrefix)
	{
		m_strFilePrefix = pFilePrefix;
	}
}

void KM::CFileLogger::SetFileRolling(int Rolling)
{
	m_Rolling = Rolling;
}

void KM::CFileLogger::_Log(const char * pMsg)
{
	time_t now;
	struct tm t;
	time(&now);
	t = *localtime(&now);

	char szFilename[MAX_PATH];
	if (1 == m_Rolling)
	{
		sprintf(szFilename,"%s%s.%4d%02d%02d",m_strFilePath.c_str(),m_strFilePrefix.c_str(),t.tm_year+1900, t.tm_mon+1, t.tm_mday);
	}
	else if (24 == m_Rolling)
	{
		sprintf(szFilename,"%s%s.%4d%02d%02d%02d",m_strFilePath.c_str(),m_strFilePrefix.c_str(),t.tm_year+1900, t.tm_mon+1, t.tm_mday,t.tm_year+1900, t.tm_mon+1, t.tm_mday, t.tm_hour);
	}

	FILE * fp = fopen(szFilename,"a+");
	if (NULL == fp)
	{
		return;
	}

#if (defined(WIN32) || defined(WIN64))
	fprintf(fp,"%s\r",pMsg);
#else
	fprintf(fp,"%s\n",pMsg);
#endif
	fclose(fp);
}