#ifndef __MEMORY_ALLOCATOR_H__
#define __MEMORY_ALLOCATOR_H__

#include "lock.h"
#include "memorypool.h"

template<typename T = CNonLock>
class CVarMemoryAllocator
{
public:
	CVarMemoryAllocator()
	{
	}

	~CVarMemoryAllocator()
	{
	}

	/**
    * @brief
    * 创建可变内存池
    * @param dwPageSize : 内部分配的内存页大小，内存不够时，内存池会申请一块新的内存页
    * @return 创建成功返回TRUE，否则返回FALSE
    **/
    bool PPAPI Create(unsigned int dwPageSize = 0x80000)
	{
		m_Lock.Lock();
		m_Pool.Create(dwPageSize);
		m_Lock.Unlock();
	}

    /**
    * @brief
    * 分配Len长度的Buffer
    * @param dwLen : 获得的内存块大小长度
    * @return  返回的内存，如果返回为NULL，则代表分配失败
    **/
    void* PPAPI Malloc(unsigned int dwLen)
	{
		void* p = NULL;
		m_Lock.Lock();
		p = m_Pool.Malloc(dwLen);
		m_Lock.Unlock();
		return p;
	}

    /**
    * @brief
    * 回收内存
    * @param p : 指向需要回收的内存
    * @return void
    **/
    void PPAPI Free(void* p)
	{
		m_Lock.Lock();
		m_Pool.Free(p);
		m_Lock.Unlock();
	}

    /**
    * @brief
    * 清空内存池，使所有的内存都可以使用，此方法不会将内存返回给操作系统
    * @return void
    **/
    void PPAPI Clear()
	{
		m_Lock.Lock();
		m_Pool.Clear();
		m_Lock.Unlock();
	}

    /**
    * @brief
    * 获取当前内存使用量
    * @return 当前内存使用量
    **/
    int PPAPI GetMemUsed()
	{
		int t_MemUsed = 0;
		m_Lock.Lock();
		t_MemUsed = m_Pool.GetMemUsed();
		m_Lock.Unlock();
		return t_MemUsed;
	}

private:
	CVarMemoryPool m_Pool;
	T m_Lock;
};

template<typename T = CNonLock>
class CFixMemoryAllocator
{
public:
	CFixMemoryAllocator()
	{
	}

	~CFixMemoryAllocator()
	{
	}

	/**
    * @brief
    * 初始化内存池
    * @param dwUnitSize : 每一个分配的内存块大小
    * @param dwPageSize : 内部分配的内存页大小，内存不够时，内存池会申请一块新的内存页
    * @return  创建成功返回true，失败返回false
    **/
    bool PPAPI Create(unsigned int dwUnitSize, unsigned int dwPageSize = 0x40000)
	{
		bool ret = false;
		m_Lock.Lock();
		ret = m_Pool.Create(dwUnitSize, dwPageSize);
		m_Lock.Unlock();
		return ret;
	}

    /**
    * @brief
    * 得到一块新的内存
    * @return  void*
    **/
    void* PPAPI Malloc()
	{
		void * p = NULL;
		m_Lock.Lock();
		p = m_Pool.Malloc()
		m_Lock.Unlock();
		return p;
	}

    /**
    * @brief
    * 归还一块分配的内存
    * @param p : 内存的地址
    * @return  void
    **/
    void PPAPI Free(void* p)
	{
		m_Lock.Lock();
		m_Pool.Free(p);
		m_Lock.Unlock();
	}

    /**
    * @brief
    * 清空内存池，使所有的内存都可以使用，此方法不会将内存返回给操作系统
    * @return void
    **/
    void PPAPI Clear()
	{
		m_Lock.Lock();
		m_Pool.Clear();
		m_Lock.Unlock();
	}

    /**
    * @brief
    * 获取当前内存使用量
    * @return 当前内存使用量
    **/
    int PPAPI GetMemUsed()
	{
		int t_MemUsed = 0;
		m_Lock.Lock();
		t_MemUsed = m_Pool.GetMemUsed();
		m_Lock.Unlock();
		return t_MemUsed;
	}

private:
	CFixMemoryPool m_Pool;
	T m_Lock;
};

#endif
