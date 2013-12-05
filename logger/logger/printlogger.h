#ifndef __PRINT_LOGGER_H__
#define __PRINT_LOGGER_H__

#include "config.h"
#include "baselogger.h"

namespace KM
{
	class CPrintLogger : public CBaseLogger
	{
	public:
		CPrintLogger();
		virtual ~CPrintLogger();

	protected:
		void _Log(const char* pMsg);
	};
}

#endif