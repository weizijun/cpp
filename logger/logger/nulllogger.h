#ifndef __NULL_LOGGER_H__
#define __NULL_LOGGER_H__
#include "baselogger.h"

namespace KM
{
	class CNullLogger : public CBaseLogger
	{
	public:
		CNullLogger();
		virtual ~CNullLogger(); 

	protected:
		void _Log(const char * pMsg);
	};
}

#endif