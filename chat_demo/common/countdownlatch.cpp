#include "countdownlatch.h"
#include <errno.h>
/**
CountDownLatch::CountDownLatch(int count) : 
m_Count(count)
{
	PPCool::IPPSystemAPI::Instance()->InitLock(m_Lock);
	PPCool::IPPSystemAPI::Instance()->InitCondition(m_Condition);
}

CountDownLatch::~CountDownLatch()
{
	PPCool::IPPSystemAPI::Instance()->DestroyCondition(m_Condition);
	PPCool::IPPSystemAPI::Instance()->DestroyLock(m_Lock);
}

void CountDownLatch::Wait(unsigned int TimeOut)
{
	PPCool::IPPSystemAPI::Instance()->Lock(m_Lock);
	while (m_Count > 0)
	{
		PPCool::IPPSystemAPI::Instance()->WaitCondition(m_Condition, m_Lock, TimeOut);
	}
	PPCool::IPPSystemAPI::Instance()->Unlock(m_Lock);
}

void CountDownLatch::CountDown()
{
	PPCool::IPPSystemAPI::Instance()->Lock(m_Lock);
	--m_Count;
	if (m_Count == 0)
	{
		PPCool::IPPSystemAPI::Instance()->NotifyAllCondition(m_Condition);
	}
	PPCool::IPPSystemAPI::Instance()->Unlock(m_Lock);
}

int CountDownLatch::GetCount()
{
	PPCool::IPPSystemAPI::Instance()->Lock(m_Lock);
	int Count = m_Count;
	PPCool::IPPSystemAPI::Instance()->Unlock(m_Lock);
	return Count;
}
**/

/**
CountDownLatch::CountDownLatch(int count) : 
m_Count(count)
{
	PPCool::IPPSystemAPI::Instance()->InitLock(m_Lock);
}

CountDownLatch::~CountDownLatch()
{
	PPCool::IPPSystemAPI::Instance()->DestroyLock(m_Lock);
}

int CountDownLatch::Wait(unsigned int TimeOut)
{
	unsigned long t_Start = PPCool::IPPSystemAPI::Instance()->GetTickCount();
	unsigned long t_Now = t_Start;
	while (GetCount() > 0 && TimeOut > 0)
	{
		PPCool::IPPSystemAPI::Instance()->Sleep(1);
		t_Now = PPCool::IPPSystemAPI::Instance()->GetTickCount();
		TimeOut -= (t_Now - t_Start);
		//--TimeOut;
	}
	if(TimeOut > 0)
	{
		return 0;
	}
	else if(TimeOut <= 0)
	{
		return GetCount();
	}
	return -1;
}

void CountDownLatch::CountDown()
{
	PPCool::IPPSystemAPI::Instance()->Lock(m_Lock);
	--m_Count;
	PPCool::IPPSystemAPI::Instance()->Unlock(m_Lock);
}

int CountDownLatch::GetCount()
{
	PPCool::IPPSystemAPI::Instance()->Lock(m_Lock);
	int Count = m_Count;
	PPCool::IPPSystemAPI::Instance()->Unlock(m_Lock);
	return Count;
}
**/


CountDownLatch::CountDownLatch(int count) : 
m_Count(count)
{
	PPCool::IPPSystemAPI::Instance()->InitLock(m_Lock);
#if (defined(WIN32) || defined(WIN64))
	m_hSemaphore = CreateSemaphore(NULL, 0, 1, NULL);
#else
	sem_init(&m_hSemaphore, 0, 0);
#endif
}

CountDownLatch::~CountDownLatch()
{
#if (defined(WIN32) || defined(WIN64))
	CloseHandle(m_hSemaphore);
#else
	sem_destroy(&m_hSemaphore);
#endif
	PPCool::IPPSystemAPI::Instance()->DestroyLock(m_Lock);
}

int CountDownLatch::Wait(unsigned int TimeOut)
{
#if (defined(WIN32) || defined(WIN64))
	DWORD dw = WaitForSingleObject(m_hSemaphore, TimeOut);
	switch(dw) {
		case WAIT_OBJECT_0:
			{
				return 0;
			}
			break;
		case WAIT_TIMEOUT:
			{
				return GetCount();
			}
			break;
		case WAIT_FAILED:
			{
				DWORD dwErr = GetLastError();
				return -1;
			}
			break;
	}
#else
	struct timespec abstime;
	abstime.tv_sec = time(NULL) + TimeOut/1000;
	abstime.tv_nsec = TimeOut%1000 * 1000000;

	int ret = sem_timedwait(&m_hSemaphore, &abstime);
	if(0 == ret)
	{
		return 0;
	}
	else
	{
		if(ETIMEDOUT == errno)
		{
			return GetCount();
		}
		else
		{
			return -1;
		}
	}
#endif
	return -1;
}

void CountDownLatch::CountDown()
{
	PPCool::IPPSystemAPI::Instance()->Lock(m_Lock);
	if(--m_Count <= 0)
	{
#if (defined(WIN32) || defined(WIN64))
		ReleaseSemaphore(m_hSemaphore, 1, NULL);
#else
		sem_post(&m_hSemaphore);
#endif
	}
	PPCool::IPPSystemAPI::Instance()->Unlock(m_Lock);
}

int CountDownLatch::GetCount()
{
	PPCool::IPPSystemAPI::Instance()->Lock(m_Lock);
	int Count = m_Count;
	PPCool::IPPSystemAPI::Instance()->Unlock(m_Lock);
	return Count;
}

