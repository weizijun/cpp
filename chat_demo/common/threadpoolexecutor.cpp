#include "threadpoolexecutor.h"
#include <new>

CThreadPoolExecutor::CWorker::CWorker(CThreadPoolExecutor * pThreadPool, Runnable * pFirstTask) : 
m_pThreadPool(pThreadPool),
m_pFirstTask(pFirstTask),
m_bRun(true)
{
	
}

CThreadPoolExecutor::CWorker::~CWorker()
{
	m_pThreadPool = NULL;
}

/**
  执行任务的工作线程。
  当前没有任务时，
  如果当前线程数量大于最小线程数量，减少线程，
  否则，执行清理程序，将线程类给释放掉
**/
void CThreadPoolExecutor::CWorker::Run()
{
	Runnable * pTask = NULL;
	while(m_bRun)
	{
		if(NULL == m_pFirstTask)
		{
			pTask = m_pThreadPool->GetTask();
		}
		else
		{
			pTask = m_pFirstTask;
			m_pFirstTask = NULL;
		}

		if(NULL == pTask)
		{
			m_pThreadPool->m_csThreadPoolLock.Lock();
			if(m_pThreadPool->GetThreadPoolSize() > m_pThreadPool->m_minThreads)
			{
				ThreadPoolItr itr = m_pThreadPool->m_ThreadPool.find(this);
				if(itr != m_pThreadPool->m_ThreadPool.end())
				{
					m_pThreadPool->m_ThreadPool.erase(itr);
					m_pThreadPool->m_TrashThread.insert(this);
				}
				m_bRun = false;
			}
			else
			{
				ThreadPoolItr itr = m_pThreadPool->m_TrashThread.begin();
				while(itr != m_pThreadPool->m_TrashThread.end())
				{
					(*itr)->Join();
					delete (*itr);
					m_pThreadPool->m_TrashThread.erase(itr);
					itr = m_pThreadPool->m_TrashThread.begin();
				}
			}
			m_pThreadPool->m_csThreadPoolLock.Unlock();
			PPCool::IPPSystemAPI::Instance()->Sleep(1);
			continue;
		}
		else
		{
			pTask->Run();
			pTask = NULL;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////

CThreadPoolExecutor::CThreadPoolExecutor(void) : 
m_bRun(false),
m_bEnableInsertTask(false)
{
}

CThreadPoolExecutor::~CThreadPoolExecutor(void)
{
	Terminate();
}

bool CThreadPoolExecutor::Init(unsigned int minThreads, unsigned int maxThreads, unsigned int maxPendingTasks)
{
	if(minThreads == 0)
	{
		return false;
	}
	if(maxThreads < minThreads)
	{
		return false;
	}
	m_minThreads = minThreads;
	m_maxThreads = maxThreads;
	m_maxPendingTasks = maxPendingTasks;
	unsigned int i = GetThreadPoolLockSize();
	for(; i<minThreads; i++)
	{
		//创建线程
		CWorker * pWorker = new (std::nothrow) CWorker(this);
		if(NULL == pWorker)
		{
			return false;
		}
		m_csThreadPoolLock.Lock();
		m_ThreadPool.insert(pWorker);
		m_csThreadPoolLock.Unlock();
		pWorker->Start();
	}
	m_bRun = true;
	m_bEnableInsertTask = true;
	return true;
}

bool CThreadPoolExecutor::Execute(Runnable * pRunnable)
{
	if(!m_bEnableInsertTask)
	{
		return false;
	}
	if(NULL == pRunnable)
	{
		return false;
	}
	m_csTasksLock.Lock();
	if(m_Tasks.size() >= m_maxPendingTasks)
	{
		m_csTasksLock.Unlock();
		if(GetThreadPoolLockSize() < m_maxThreads)
		{
			CWorker * pWorker = new (std::nothrow) CWorker(this, pRunnable);
			if(NULL == pWorker)
			{
				return false;
			}
			m_csThreadPoolLock.Lock();
			m_ThreadPool.insert(pWorker);
			m_csThreadPoolLock.Unlock();
			pWorker->Start();
		}
		else
		{
			return false;
		}
	}
	else
	{
		m_Tasks.push_back(pRunnable);
		m_csTasksLock.Unlock();
	}
	return true;
}

Runnable * CThreadPoolExecutor::GetTask()
{
	Runnable * Task = NULL;
	m_csTasksLock.Lock();
	if(!m_Tasks.empty())
	{
		Task = m_Tasks.front();
		m_Tasks.pop_front();
	}
	m_csTasksLock.Unlock();
	return Task;
}

unsigned int CThreadPoolExecutor::GetTaskLockSize()
{
	unsigned int size = 0;
	m_csTasksLock.Lock();
	size = m_Tasks.size();
	m_csTasksLock.Unlock();
	return size;
}

unsigned int CThreadPoolExecutor::GetThreadPoolLockSize()
{
	unsigned int size = 0;
	m_csThreadPoolLock.Lock();
	size = m_ThreadPool.size();
	m_csThreadPoolLock.Unlock();
	return size;
}

unsigned int CThreadPoolExecutor::GetThreadPoolSize()
{
	return m_ThreadPool.size();
}

void CThreadPoolExecutor::Terminate()
{
	m_bEnableInsertTask = false;
	while(GetTaskLockSize() > 0)
	{
		PPCool::IPPSystemAPI::Instance()->Sleep(1);
	}
	m_bRun = false;
	m_minThreads = 0;
	m_maxThreads = 0;
	m_maxPendingTasks = 0;
	while(GetThreadPoolLockSize() > 0)
	{
		PPCool::IPPSystemAPI::Instance()->Sleep(1);
	}
	m_csThreadPoolLock.Lock();
	ThreadPoolItr itr = m_TrashThread.begin();
	while(itr != m_TrashThread.end())
	{
		(*itr)->Join();
		delete (*itr);
		m_TrashThread.erase(itr);
		itr = m_TrashThread.begin();
	}
	m_csThreadPoolLock.Unlock();
}
