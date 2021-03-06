#include "thread.h"

CThread::CThread(void) : 
m_pRunnable(NULL),
m_bRun(false)
{
}

CThread::~CThread(void)
{
}

CThread::CThread(Runnable * pRunnable) : 
m_pRunnable(pRunnable),
m_ThreadName(""),
m_bRun(false)
{
}

CThread::CThread(const char * ThreadName, Runnable * pRunnable) : 
m_pRunnable(pRunnable),
m_ThreadName(ThreadName),
m_bRun(false)
{
}

CThread::CThread(std::string ThreadName, Runnable * pRunnable) : 
m_pRunnable(pRunnable),
m_ThreadName(ThreadName),
m_bRun(false)
{
}

bool CThread::Start(bool bSuspend)
{
	if(m_bRun)
	{
		return true;
	}

	m_handle = PPCool::IPPSystemAPI::Instance()->CreateThread(StaticThreadFunc, this, bSuspend);
	if(PPINVALID_HANDLE == m_handle)
	{
		m_bRun = false;
	}
	else
	{
		m_bRun = true;
	}
	return m_bRun;
}

void CThread::Run()
{
	if(!m_bRun)
	{
		return;
	}
	if(NULL != m_pRunnable)
	{
		m_pRunnable->Run();
	}
	m_bRun = false;
}

void CThread::Join(int timeout)
{
	if(!m_bRun)
	{
		return;
	}
	PPCool::IPPSystemAPI::Instance()->JoinThread(m_handle, timeout);
}

void CThread::Resume()
{
#ifdef WIN32
	if(NULL == m_handle || !m_bRun)
	{
		return;
	}
	::ResumeThread(m_handle);
#endif
}

void CThread::Suspend()
{
#ifdef WIN32
	if(NULL == m_handle || !m_bRun)
	{
		return;
	}
	::SuspendThread(m_handle);
#endif
}

void CThread::Terminate(unsigned long ExitCode)
{
	if(!m_bRun)
	{
		return;
	}
	PPCool::IPPSystemAPI::Instance()->TerminateThread(m_handle, ExitCode);
}

std::string CThread::GetThreadName()
{
	return m_ThreadName;
}

void CThread::SetThreadName(std::string ThreadName)
{
	m_ThreadName = ThreadName;
}

void CThread::SetThreadName(const char * ThreadName)
{
	if(NULL == ThreadName)
	{
		m_ThreadName = "";
	}
	else
	{
		m_ThreadName = ThreadName;
	}
}

PPTHREAD_DECLARE(CThread::StaticThreadFunc) (void * arg)
{
	CThread * pThread = (CThread *)arg;
	pThread->Run();
	PPTHREAD_RETURN(0);
}
