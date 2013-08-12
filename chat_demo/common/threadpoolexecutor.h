#ifndef __THREAD_POOL_EXECUTOR__
#define __THREAD_POOL_EXECUTOR__

#include "lock.h"
#include "thread.h"
#include <set>
#include <list>

class CThreadPoolExecutor
{
public:
	CThreadPoolExecutor(void);
	~CThreadPoolExecutor(void);

	/**
	  ��ʼ���̳߳أ�����minThreads���߳�
	**/
	bool Init(unsigned int minThreads, unsigned int maxThreads, unsigned int maxPendingTasks);

	/**
	  ִ����������ǰ�����б�û����������������뵽�����б�����true
	  ����ǰ�����б����ˣ�����ǰ�߳�����С������߳��������������߳�ִ�д����񣬷���true
	  ����ǰ�����б����ˣ�����ǰ�߳�������������߳����������������񣬷���false
	**/
	bool Execute(Runnable * pRunnable);

	/**
	  ��ֹ�̳߳أ�����ֹ��������
	  Ȼ��ȴ�ֱ�������б�Ϊ�գ�
	  Ȼ��������С�߳�����Ϊ0��
	  �ȴ�ֱ���߳�����Ϊ�գ�
	  ����������е�����
	**/
	void Terminate();

	/**
	  �����̳߳��е�ǰ���߳�����
	**/
	unsigned int GetThreadPoolSize();

private:
	/**
	  ��ȡ�����б��е������������б�Ϊ�գ�����NULL
	**/
	Runnable * GetTask();
	unsigned int GetTaskLockSize();
	unsigned int GetThreadPoolLockSize();

private:
	class CWorker : public CThread
	{
	public:
		CWorker(CThreadPoolExecutor * pThreadPool, Runnable * pFirstTask = NULL);
		~CWorker();
		void Run();

	private:
		CThreadPoolExecutor * m_pThreadPool;
		Runnable * m_pFirstTask;
		volatile bool m_bRun;
	};

	typedef std::set<CWorker *> ThreadPool;
	typedef std::list<Runnable *> Tasks;
	typedef Tasks::iterator TasksItr;
	typedef ThreadPool::iterator ThreadPoolItr;

	ThreadPool m_ThreadPool;
	ThreadPool m_TrashThread;
	Tasks m_Tasks;

	CLock m_csTasksLock;
	CLock m_csThreadPoolLock;

	volatile bool m_bRun;
	volatile bool m_bEnableInsertTask;
	volatile unsigned int m_minThreads;
	volatile unsigned int m_maxThreads;
	volatile unsigned int m_maxPendingTasks;
};

#endif
