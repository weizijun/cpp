#ifndef __PP_SYSTEM_API_H__
#define __PP_SYSTEM_API_H__

#include "ppbase.h"
#include <string>

#if (defined(WIN32) || defined(WIN64))
#include <winsock2.h>
#include <windows.h>
#else
#include <pthread.h>
extern "C" {
#include "libcoro/coro.h"
}
typedef bool BOOL;
#endif

namespace PPCool
{

#if (defined(WIN32) || defined(WIN64))

//通用
typedef HANDLE PPHANDLE;
#define PPINVALID_HANDLE  (NULL)
//用于锁
typedef CRITICAL_SECTION LockID;
//用于线程
typedef unsigned int (WINAPI *PFThreadProc)(void *);
#define PPTHREAD_DECLARE(x)  unsigned int WINAPI  x
#define PPTHREAD_RETURN(x)   return x;
//用于协程
typedef void (WINAPI *PFCoroutineProc)(void *);
typedef void * PPCoroContext;

//用于条件变量
#ifdef _PPCOOL_USE_CONDITION_VARIABLE
typedef CONDITION_VARIABLE PPCondition;
#else
typedef HANDLE PPCondition;
#endif

#define vsnprintf _vsnprintf
#define snprintf _snprintf
#define strcasecmp stricmp

#else

//通用
#ifndef TRUE
#define TRUE (true)
#endif
#ifndef FALSE
#define FALSE (false)
#endif
//typedef unsigned long int PPHANDLE;
typedef pthread_t PPHANDLE;
#define PPINVALID_HANDLE  (0)
#define INFINITE 0xFFFFFFFF
//用于锁
typedef pthread_mutex_t LockID;
//用于线程
typedef void * (*PFThreadProc)(void *);
#define PPTHREAD_DECLARE(x)  void *  x
#define PPTHREAD_RETURN(x)   return x;
//用于协程
typedef void (*PFCoroutineProc)(void *);
typedef coro_context* PPCoroContext;
//用于条件变量
typedef pthread_cond_t PPCondition;

#define _vsnprintf vsnprintf
#define _snprintf snprintf
#define stricmp strcasecmp

#endif

class IPPSystemAPI
{
protected:
	IPPSystemAPI();
	virtual ~IPPSystemAPI();

private:
	static IPPSystemAPI * m_pInstance;
	
public:
	static IPPSystemAPI* Instance();

	static void DestroyInstance();

public:
	virtual std::string toString() = 0;

	virtual int AtomicRead32(volatile int *pValue) = 0;

    virtual void AtomicSet32(volatile int *pValue, int NewValue) = 0;

    virtual int AtomicAdd32(volatile int *pValue, int Addend) = 0;

    virtual int AtomicSub32(volatile int *pValue, int Subtrahend) = 0;

    virtual int AtomicInc32(volatile int *pValue) = 0;

    virtual int AtomicDec32(volatile int *pValue) = 0;

	virtual unsigned long GetTickCount() = 0;

	virtual void Sleep(unsigned int milliseconds) = 0;

	virtual void InitLock(LockID & id) = 0;

	virtual void DestroyLock(LockID & id) = 0;

	virtual void Lock(LockID & id) = 0;

	virtual void Unlock(LockID & id) = 0;

	virtual void InitCondition(PPCondition & hCondition) =0;

	virtual void DestroyCondition(PPCondition & hCondition) =0;

	virtual int WaitCondition(PPCondition & hCondition, LockID & hLock, unsigned int TimeOut) =0;

	virtual void NotifyCondition(PPCondition & hCondition) =0;

	virtual void NotifyAllCondition(PPCondition & hCondition) =0;

	virtual PPHANDLE CreateThread(PFThreadProc pfThrdProc, void *pArg, BOOL bSuspend = FALSE) = 0;

	virtual void JoinThread(PPHANDLE hThread, int timeout = INFINITE) = 0;

	virtual void TerminateThread(PPHANDLE hThread, unsigned long ExitCode = 0) = 0;

	virtual void ResumeThread(PPHANDLE hThread) = 0;

	virtual void SuspendThread(PPHANDLE hThread) = 0;

	virtual unsigned long GetCurrentThreadID() = 0;

	virtual PPCoroContext InitCoroutine(unsigned int StackSize = 4 * 1024 * 1024) = 0;

	virtual PPCoroContext CreateCoroutine(unsigned int StackSize, PFCoroutineProc pfCoroProc, void * pParam) =0;

	virtual void TerminateCoroutine(PPCoroContext hCoroContext) =0;

	virtual void SwitchCoroutine(PPCoroContext hOldCoroContext, PPCoroContext hNewCoroContext) =0;

	virtual void UninitCoroutine() = 0;

	virtual std::string GBKToUTF8(std::string& strGBK) =0;

	virtual std::string UTF8ToGBK(std::string& strUTF8) =0;

	virtual int SingleRun(const std::string& strLockFile) =0;
};

};

#endif
