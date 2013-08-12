#include "fifo.h"

CFifo::CFifo()
{
}

CFifo::~CFifo()
{
}

void CFifo::PushBack(void * entity)
{
	m_csLock.Lock();
	m_fifo.push_back(entity);
	m_csLock.Unlock();
}

void * CFifo::PopFront()
{
	void * entity = NULL;
	m_csLock.Lock();
	if(m_fifo.empty())
	{
		m_csLock.Unlock();
		return entity;
	}
	entity = m_fifo.front();
	m_fifo.pop_front();
	m_csLock.Unlock();
	return entity;
}
