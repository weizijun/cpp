#ifndef __FIFO_H__
#define __FIFO_H__

#include "ppbase.h"
#include <deque>
#include "lock.h"

class CFifo
{
public:
	explicit CFifo();
	~CFifo();

private:
	CFifo& operator = (const CFifo& rhs);
	CFifo(const CFifo& rhs);

public:
	void PushBack(void * entity);
	void * PopFront();

private:
	std::deque<void *> m_fifo;

	CLock m_csLock;
};

#endif
