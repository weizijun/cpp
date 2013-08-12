#ifndef __PP_WINDOWS_API_H__
#define __PP_WINDOWS_API_H__

#include "ppsystemapi.h"

#if (defined(WIN32) || defined(WIN64))

class CPPWindowsAPI : public PPCool::IPPSystemAPI
{
public:
	CPPWindowsAPI();
	~CPPWindowsAPI();

public:
	std::string toString();

	int AtomicRead32(volatile int *pValue);

    void AtomicSet32(volatile int *pValue, int NewValue);

    int AtomicAdd32(volatile int *pValue, int Addend);

    int AtomicSub32(volatile int *pValue, int Subtrahend);

    int AtomicInc32(volatile int *pValue);

    int AtomicDec32(volatile int *pValue);

	unsigned long GetTickCount();

	void Sleep(unsigned int milliseconds);

	void InitLock(PPCool::LockID & id);

	void DestroyLock(PPCool::LockID & id);

	void Lock(PPCool::LockID & id);

	void Unlock(PPCool::LockID & id);

	void InitCondition(PPCool::PPCondition & hCondition);

	void DestroyCondition(PPCool::PPCondition & hCondition);

	int WaitCondition(PPCool::PPCondition & hCondition, PPCool::LockID & hLock, unsigned int TimeOut);

	void NotifyCondition(PPCool::PPCondition & hCondition);

	void NotifyAllCondition(PPCool::PPCondition & hCondition);

	PPCool::PPHANDLE CreateThread(PPCool::PFThreadProc pfThrdProc, void *pArg, BOOL bSuspend);

	void JoinThread(PPCool::PPHANDLE hThread, int timeout);

	void TerminateThread(PPCool::PPHANDLE hThread, unsigned long ExitCode);

	void ResumeThread(PPCool::PPHANDLE hThread);

	void SuspendThread(PPCool::PPHANDLE hThread);

	unsigned long GetCurrentThreadID();

	PPCool::PPCoroContext InitCoroutine(unsigned int StackSize);

	PPCool::PPCoroContext CreateCoroutine(unsigned int StackSize, PPCool::PFCoroutineProc pfCoroProc, void * pParam);

	void TerminateCoroutine(PPCool::PPCoroContext hCoroContext);

	void SwitchCoroutine(PPCool::PPCoroContext hOldCoroContext, PPCool::PPCoroContext hNewCoroContext);

	void UninitCoroutine();
	
	std::string GBKToUTF8(std::string& strGBK);

	std::string UTF8ToGBK(std::string& strUTF8);

	int SingleRun(const std::string& strLockFile);
};

#endif

#endif
