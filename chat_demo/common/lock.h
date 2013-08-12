#ifndef __PPLOCK_H__
#define __PPLOCK_H__

#include "ppbase.h"
#include "ppsystemapi.h"

class ILock
{
public:
	virtual ~ILock() {};

public:
	virtual void Lock() = 0;
	virtual void Unlock() = 0;
};

class CNonLock : public ILock
{
public:
	explicit CNonLock();
	~CNonLock();

public:
	void Lock();
	void Unlock();
};

class CLock : public ILock
{
public:
	explicit CLock();
	~CLock();

public:
	void Lock();
	void Unlock();

private:
	PPCool::LockID m_csLock;
};

class CLockGuard
{
private:
    ILock& m_lock;
public:
	CLockGuard(ILock & oLock) : m_lock(oLock)
    {
        m_lock.Lock();
    }

    ~CLockGuard()
    {
        m_lock.Unlock();
    }
};

#endif
