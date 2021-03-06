#ifndef __OBJECT_ALLOCATOR_H__
#define __OBJECT_ALLOCATOR_H__

#include "lock.h"
#include "memorypool.h"

template<typename TO, typename T = CNonLock>
class CObjectAllocator
{
public:
	CObjectAllocator()
	{
	}

	~CObjectAllocator()
	{
	}

	/**
    * @brief
    * 初始化对象分配器
    * @param dwGrowCount : 每次增长的分配数量
    * @return  创建成功返回true，失败返回false
    **/
    bool PPAPI Create(unsigned int dwGrowCount = 1)
	{
		unsigned int dwUnitSize = sizeof(TO);
		bool ret = false;
		m_Lock.Lock();
		ret = m_Pool.Create(dwUnitSize, dwGrowCount * dwUnitSize);
		m_Lock.Unlock();
		return ret;
	}

    /**
    * @brief
    * 构造一个新的对象
    * @return  void*
    **/
    TO* PPAPI New()
	{
		return new (Malloc()) TO;
	}

	template<typename P1>
	TO* PPAPI New(P1 p1)
	{
		return new (Malloc()) TO(p1);
	}

	template<typename P1, typename P2>
	TO* PPAPI New(P1 p1, P2 p2)
	{
		return new (Malloc()) TO(p1, p2);
	}

	template<typename P1, typename P2, typename P3>
	TO* PPAPI New(P1 p1, P2 p2, P3 p3)
	{
		return new (Malloc()) TO(p1, p2, p3);
	}

	template<typename P1, typename P2, typename P3, typename P4>
	TO* PPAPI New(P1 p1, P2 p2, P3 p3, P4 p4)
	{
		return new (Malloc()) TO(p1, p2, p3, p4);
	}

	template<typename P1, typename P2, typename P3, typename P4, typename P5>
	TO* PPAPI New(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
	{
		return new (Malloc()) TO(p1, p2, p3, p4, p5);
	}

    /**
    * @brief
    * 释放一个对象
    * @param p : 指向对象的指针
    * @return  void
    **/
    void PPAPI Delete(TO* p)
	{
		p->~TO();
		Free(p);
	}

private:
    void* Malloc()
	{
		void * p = NULL;
		m_Lock.Lock();
		p = m_Pool.Malloc();
		m_Lock.Unlock();
		return p;
	}

    void Free(void* p)
	{
		m_Lock.Lock();
		m_Pool.Free(p);
		m_Lock.Unlock();
	}

private:
	CFixMemoryPool m_Pool;
	T m_Lock;
};

#endif
