#include "ppwindowsapi.h"

#if (defined(WIN32) || defined(WIN64))

#include <process.h>

using namespace PPCool;

CPPWindowsAPI::CPPWindowsAPI()
{
}

CPPWindowsAPI::~CPPWindowsAPI()
{
}

std::string CPPWindowsAPI::toString()
{
	return "PPWindowsAPI";
}

int CPPWindowsAPI::AtomicRead32(volatile int *pValue)
{
	 return *pValue;
}

void CPPWindowsAPI::AtomicSet32(volatile int *pValue, int NewValue)
{
	InterlockedExchange((volatile LONG *)pValue, NewValue);
}

int CPPWindowsAPI::AtomicAdd32(volatile int *pValue, int Addend)
{
	return InterlockedExchangeAdd((volatile LONG *)pValue, Addend);
}

int CPPWindowsAPI::AtomicSub32(volatile int *pValue, int Subtrahend)
{
	return InterlockedExchangeAdd((volatile LONG *)pValue, -Subtrahend);
}

int CPPWindowsAPI::AtomicInc32(volatile int *pValue)
{
	return InterlockedIncrement((volatile LONG *)pValue) - 1;
}

int CPPWindowsAPI::AtomicDec32(volatile int *pValue)
{
	return InterlockedDecrement((volatile LONG *)pValue) + 1;
}

unsigned long CPPWindowsAPI::GetTickCount()
{
	return ::GetTickCount();
}

void CPPWindowsAPI::Sleep(unsigned int milliseconds)
{
	::Sleep(milliseconds);
}

void CPPWindowsAPI::InitLock(LockID & id)
{
	InitializeCriticalSection(&id);
}

void CPPWindowsAPI::DestroyLock(LockID & id)
{
	DeleteCriticalSection(&id);
}

void CPPWindowsAPI::Lock(LockID & id)
{
	EnterCriticalSection(&id);
}

void CPPWindowsAPI::Unlock(LockID & id)
{
	LeaveCriticalSection(&id);
}

void CPPWindowsAPI::InitCondition(PPCondition & hCondition)
{
#ifdef _PPCOOL_USE_CONDITION_VARIABLE
	InitializeConditionVariable(&hCondition);
#endif
}

void CPPWindowsAPI::DestroyCondition(PPCondition & hCondition)
{
}

int CPPWindowsAPI::WaitCondition(PPCondition & hCondition, LockID & hLock, unsigned int TimeOut)
{
#ifdef _PPCOOL_USE_CONDITION_VARIABLE
	return TRUE == SleepConditionVariableCS(&hCondition, &hLock, TimeOut);
#else
	return -1;
#endif
}

void CPPWindowsAPI::NotifyCondition(PPCondition & hCondition)
{
#ifdef _PPCOOL_USE_CONDITION_VARIABLE
	WakeConditionVariable(&hCondition);
#endif
}

void CPPWindowsAPI::NotifyAllCondition(PPCondition & hCondition)
{
#ifdef _PPCOOL_USE_CONDITION_VARIABLE
	WakeAllConditionVariable(&hCondition);
#endif
}

PPHANDLE CPPWindowsAPI::CreateThread(PFThreadProc pfThrdProc, void *pArg, BOOL bSuspend)
{
	unsigned int InitFlag = bSuspend ? CREATE_SUSPENDED : 0;
	return (PPHANDLE)_beginthreadex(NULL, 0, pfThrdProc, pArg, InitFlag, NULL);
}

void CPPWindowsAPI::JoinThread(PPHANDLE hThread, int timeout)
{
	::WaitForSingleObject(hThread, timeout);
}

void CPPWindowsAPI::TerminateThread(PPHANDLE hThread, unsigned long ExitCode)
{
	if(::TerminateThread(hThread, ExitCode))
	{
		::CloseHandle(hThread);
	}
}

void CPPWindowsAPI::ResumeThread(PPHANDLE hThread)
{
	::ResumeThread(hThread);
}

void CPPWindowsAPI::SuspendThread(PPHANDLE hThread)
{
	::SuspendThread(hThread);
}

unsigned long CPPWindowsAPI::GetCurrentThreadID()
{
	return ::GetCurrentThreadId();
}

PPCool::PPCoroContext CPPWindowsAPI::InitCoroutine(unsigned int)
{
	return ConvertThreadToFiber(NULL);
}

PPCool::PPCoroContext CPPWindowsAPI::CreateCoroutine(unsigned int StackSize, PFCoroutineProc pfCoroProc, void * pParam)
{
	return CreateFiber(StackSize, pfCoroProc, pParam);
}

void CPPWindowsAPI::TerminateCoroutine(PPCool::PPCoroContext hCoroContext)
{
	DeleteFiber(hCoroContext);
}

void CPPWindowsAPI::SwitchCoroutine(PPCool::PPCoroContext, PPCool::PPCoroContext hNewCoroContext)
{
	SwitchToFiber(hNewCoroContext);
}

void CPPWindowsAPI::UninitCoroutine()
{
	ConvertFiberToThread();
}

std::string CPPWindowsAPI::GBKToUTF8(std::string& strGBK)
{
	std::string strOutUTF8 = "";
	WCHAR * str1;
	int n = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0);
	str1 = new WCHAR[n];
	MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, str1, n);
	n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);
	char * str2 = new char[n];
	WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL);
	strOutUTF8 = str2;
	delete [] str1;
	str1 = NULL;
	delete [] str2;
	str2 = NULL;
	return strOutUTF8;
}

std::string CPPWindowsAPI::UTF8ToGBK(std::string& strUTF8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);
	unsigned short * wszGBK = new unsigned short[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUTF8.c_str(), -1, (LPWSTR)wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
	char * szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wszGBK, -1, (LPSTR)szGBK, len, NULL, NULL);
	std::string strTemp(szGBK);
	delete [] szGBK;
	delete [] wszGBK;
	return strTemp;
}

int CPPWindowsAPI::SingleRun(const std::string& strLockFile)
{
	HANDLE hFile = CreateFile(strLockFile.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(INVALID_HANDLE_VALUE == hFile)
	{
		if(ERROR_ALREADY_EXISTS == GetLastError())
		{
			return 1;
		}
		return 2;
	}
	return 0;
}

#endif
