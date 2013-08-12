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
	* @brief �ȴ�һ��������ɣ�����TimeOutʱ��
	* @param TimeOut ��ʱʱ�䣬��λΪ����
	* @return 0Ϊ����������ɣ�����Ϊ��ʱ������Ϊδ��ɵ����񣬸���Ϊ����
	*/
	int Wait(unsigned int TimeOut);

	/**
	* @brief ���һ�����񣬾͵���һ�δ˷���
	*/
	void CountDown();

	/**
	* @brief ʣ��δ��ɵ�����
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
