#include "lock.h"

CNonLock::CNonLock()
{
}

CNonLock::~CNonLock()
{
}

void CNonLock::Lock()
{
}

void CNonLock::Unlock()
{
}

CLock::CLock()
{
	PPCool::IPPSystemAPI::Instance()->InitLock(m_csLock);
}

CLock::~CLock()
{
	PPCool::IPPSystemAPI::Instance()->DestroyLock(m_csLock);
}

void CLock::Lock()
{
	PPCool::IPPSystemAPI::Instance()->Lock(m_csLock);
}

void CLock::Unlock()
{
	PPCool::IPPSystemAPI::Instance()->Unlock(m_csLock);
}
