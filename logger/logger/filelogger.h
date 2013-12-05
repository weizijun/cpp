#ifndef __FILE_LOGGER_H__
#define __FILE_LOGGER_H__

#include "config.h"
#include "baselogger.h"

namespace KM
{
	class CFileLogger : public CBaseLogger
	{
	public:
		CFileLogger();
		~CFileLogger();

		void SetFilePath(const char *pFilePath);
		void SetFilePrefix(const char *pFilePrefix);
		void SetFileRolling(int Rolling);

#if (defined(WIN32) || defined(WIN64))
	private:
		bool _FindFirstFileExists(LPCTSTR lpPath,DWORD dwFilter);
		bool _FilePathExists(LPCTSTR lpPath);
		bool _FolderExists(LPCTSTR lpPath);
		void _CreateFolder(const char* FilePath);
#endif
	protected:
		void _Log(const char * pMsg);

	private:
		std::string m_strFilePath;
		std::string m_strFilePrefix;
		int m_Rolling;
	};
}

#endif