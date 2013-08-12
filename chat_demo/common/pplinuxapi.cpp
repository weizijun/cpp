#include "pplinuxapi.h"

#if (defined(LINUX))

#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include "libcoro/coro.h"
#include <new>
#include <sys/time.h>
#include <sys/times.h>
#include <fcntl.h>

using namespace PPCool;

CPPLinuxAPI::CPPLinuxAPI()
{
}

CPPLinuxAPI::~CPPLinuxAPI()
{
}

std::string CPPLinuxAPI::toString()
{
	return "PPLinuxAPI";
}

int CPPLinuxAPI::AtomicRead32(volatile int *pValue)
{
	return *pValue;
}

void CPPLinuxAPI::AtomicSet32(volatile int *pValue, int NewValue)
{
	*pValue = NewValue;
}

int CPPLinuxAPI::AtomicAdd32(volatile int *pValue, int Addend)
{
	asm volatile ("lock; xaddl %0,%1"
                  : "=r" (Addend), "=m" (*pValue)
                  : "0" (Addend), "m" (*pValue)
                  : "memory", "cc");
    return *pValue - 1;
}

int CPPLinuxAPI::AtomicSub32(volatile int *pValue, int Subtrahend)
{
	asm volatile ("lock; subl %1, %0"
                  : /* no output */
                  : "m" (*(pValue)), "r" (Subtrahend)
                  : "memory", "cc");
    return *pValue + 1;
}

int CPPLinuxAPI::AtomicInc32(volatile int *pValue)
{
	return AtomicAdd32(pValue, 1);
}

int CPPLinuxAPI::AtomicDec32(volatile int *pValue)
{
	unsigned char prev;
    asm volatile ("lock; decl %0; setnz %1"
                  : "=m" (*pValue), "=qm" (prev)
                  : "m" (*pValue)
                  : "memory");
    return prev + 1;
}

unsigned long CPPLinuxAPI::GetTickCount()
{
	struct timeval cateq_v;
    struct timezone cateq_z;

    gettimeofday(&cateq_v, &cateq_z);

    return (unsigned int)(1000.0 * (double)(cateq_v.tv_sec) + (double)(cateq_v.tv_usec) / 1000.0);
}

void CPPLinuxAPI::Sleep(unsigned int milliseconds)
{
	usleep(milliseconds * 1000);
}

void CPPLinuxAPI::InitLock(LockID & id)
{
	pthread_mutex_init(&id, 0);
}

void CPPLinuxAPI::DestroyLock(LockID & id)
{
	int rc = pthread_mutex_destroy(&id);
	assert( rc != EBUSY );
	assert( rc == 0 );
}

void CPPLinuxAPI::Lock(LockID & id)
{
	int rc = pthread_mutex_lock(&id);
	assert( rc != EINVAL );
	assert( rc != EDEADLK );
	assert( rc == 0 );
}

void CPPLinuxAPI::Unlock(LockID & id)
{
	int rc = pthread_mutex_unlock(&id);
	assert( rc != EINVAL );
	assert( rc != EPERM );
	assert( rc == 0 );
}

void CPPLinuxAPI::InitCondition(PPCondition & hCondition)
{
	int rc = pthread_cond_init(&hCondition, NULL);
	assert( rc == 0 );
}

void CPPLinuxAPI::DestroyCondition(PPCondition & hCondition)
{
	if (pthread_cond_destroy(&hCondition) == EBUSY)
	{
		assert(0);
	}
}

int CPPLinuxAPI::WaitCondition(PPCondition & hCondition, LockID & hLock, unsigned int TimeOut)
{
	struct timespec abstime;
	abstime.tv_sec = time(NULL) + TimeOut/1000;
	abstime.tv_nsec = TimeOut%1000 * 1000000;
	return pthread_cond_timedwait(&hCondition, &hLock, &abstime);
}

void CPPLinuxAPI::NotifyCondition(PPCondition & hCondition)
{
	int rc = pthread_cond_signal(&hCondition);
	assert( rc == 0);
}

void CPPLinuxAPI::NotifyAllCondition(PPCondition & hCondition)
{
	pthread_cond_broadcast(&hCondition);
}

PPHANDLE CPPLinuxAPI::CreateThread(PFThreadProc pfThrdProc, void *pArg, BOOL)
{
	PPHANDLE hThread;
	if(0 == pthread_create((pthread_t*)&hThread, NULL, pfThrdProc, pArg))
	{
		return hThread;
	}
	return PPINVALID_HANDLE;
}

void CPPLinuxAPI::JoinThread(PPHANDLE hThread, int)
{
	pthread_join(hThread, NULL);
}

void CPPLinuxAPI::TerminateThread(PPHANDLE hThread, unsigned long)
{
	pthread_cancel(hThread);
}

void CPPLinuxAPI::ResumeThread(PPHANDLE)
{
}

void CPPLinuxAPI::SuspendThread(PPHANDLE)
{
}

unsigned long CPPLinuxAPI::GetCurrentThreadID()
{
	return (unsigned long)pthread_self();
}

PPCool::PPCoroContext CPPLinuxAPI::InitCoroutine(unsigned int StackSize)
{
	if(0 == StackSize)
	{
		StackSize = 4 * 1024 * 1024;
	}
	char* mem = new char[StackSize];
	coro_context * pRealCoroContext = (coro_context*)mem;
	coro_create(pRealCoroContext, NULL, NULL, NULL, 0);
	return (PPCool::PPCoroContext)pRealCoroContext;
}

PPCool::PPCoroContext CPPLinuxAPI::CreateCoroutine(unsigned int StackSize, PFCoroutineProc pfCoroProc, void * pParam)
{
	if(0 == StackSize)
	{
		StackSize = 4 * 1024 * 1024;
	}
	char* mem = new char[StackSize];
	coro_context * pRealCoroContext = (coro_context*)mem;
	coro_create(pRealCoroContext, pfCoroProc, (void*)pParam, (char*)pRealCoroContext + sizeof(coro_context), StackSize - sizeof(coro_context));
	return (PPCool::PPCoroContext)pRealCoroContext;
}

void CPPLinuxAPI::TerminateCoroutine(PPCool::PPCoroContext hCoroContext)
{
	coro_destroy(hCoroContext);
	char* mem = (char *)hCoroContext;
	delete [] mem;
}

void CPPLinuxAPI::SwitchCoroutine(PPCool::PPCoroContext hOldCoroContext, PPCool::PPCoroContext hNewCoroContext)
{
	::coro_transfer(hOldCoroContext, hNewCoroContext);
}

void CPPLinuxAPI::UninitCoroutine()
{

}

std::string CPPLinuxAPI::GBKToUTF8(std::string& strGBK)
{
	setlocale(LC_ALL, "zh_CN.gbk");
	size_t wdestlen = mbstowcs(NULL, strGBK.c_str(), 0);
	wchar_t* wdest = new wchar_t[wdestlen + 1];
	mbstowcs(wdest, strGBK.c_str(), wdestlen + 1);
	
	setlocale(LC_ALL, "zh_CN.utf8");
	size_t destlen = wcstombs(NULL, wdest, 0);
	char* dest = new char[destlen + 1];
	wcstombs(dest, wdest, destlen + 1);
	delete [] wdest;
	std::string strDest(dest);
	delete [] dest;
	setlocale(LC_ALL, "C");
	return strDest;
}

std::string CPPLinuxAPI::UTF8ToGBK(std::string& strUTF8)
{
	setlocale(LC_ALL, "zh_CN.utf8");
	size_t wdestlen = mbstowcs(NULL, strUTF8.c_str(), 0);
	wchar_t* wdest = new wchar_t[wdestlen + 1];
	mbstowcs(wdest, strUTF8.c_str(), wdestlen + 1);
	
	setlocale(LC_ALL, "zh_CN.gbk");
	size_t destlen = wcstombs(NULL, wdest, 0);
	char* dest = new char[destlen + 1];
	wcstombs(dest, wdest, destlen + 1);
	delete [] wdest;
	std::string strDest(dest);
	delete [] dest;
	setlocale(LC_ALL, "C");
	return strDest;
}

static int lockfile(int fd)
{
	struct flock fl;
	fl.l_type = F_WRLCK;
	fl.l_start = 0;
	fl.l_whence = SEEK_SET;
	fl.l_len = 0;
	return (fcntl(fd, F_SETLK, &fl));
}

int CPPLinuxAPI::SingleRun(const std::string& strLockFile)
{
	int fd;
	char buf[16];

	fd = open(strLockFile.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if(fd < 0)
	{
		return 2;
	}
	if(lockfile(fd) < 0)
	{
		if (errno == EACCES || errno == EAGAIN)
		{
			close(fd);
			return 1;
		}
		return 3;
	}
	ftruncate(fd, 0);
	sprintf(buf, "%ld", (long)getpid());
	write(fd, buf, strlen(buf) + 1);
	return 0;
}

#endif
