#ifndef __COUNT_DOWN_LATCH_H__
#define __COUNT_DOWN_LATCH_H__

#include "ppsystemapi.h"
#if (defined(WIN32) || defined(WIN64))
#else
#include <semaphore.h>
#endif

class CountDownLatch
{
public:
	explicit CountDownLatch(int count);
	~CountDownLatch();

private:
	CountDownLatch(CountDownLatch& rhs);
	CountDownLatch();
	CountDownLatch& operator=(CountDownLatch& rhs);

public:
	/**
	* @brief 等待一批事务完成，最多等TimeOut时间
	* @param TimeOut 超时时间，单位为毫秒
	* @return 0为所有事务完成，正数为超时，数量为未完成的事务，负数为错误
	*/
	int Wait(unsigned int TimeOut);

	/**
	* @brief 完成一个事务，就调用一次此方法
	*/
	void CountDown();

	/**
	* @brief 剩余未完成的事务
	*/
	int GetCount();

private:
	PPCool::LockID m_Lock;
	//PPCool::PPCondition m_Condition;
#if (defined(WIN32) || defined(WIN64))
	HANDLE m_hSemaphore;
#else
	sem_t m_hSemaphore;
#endif
	int m_Count;
};

#endif
