#ifndef __PPMEMORYPOOL_H__
#define __PPMEMORYPOOL_H__

#include "ppbase.h"

/**
* @brief 可变内存块大小内存池，
* 内存池申请的内存不会自动释放，只在内存池销毁时释放。
* 注意：此类“非”线程安全
* 使用示例：
* CVarMemoryPool pool;
* pool.Create();
* 某一线程：
* char * p1 = pool.Malloc(512);//记得加锁
* char * p2 = pool.Malloc(31);//记得加锁
* char * p3 = pool.Malloc(128);//记得加锁
*
* 另外一个线程：
* pool.Free(p3);//记得加锁
* pool.Free(p2);//记得加锁
* pool.Free(p1);//记得加锁
*/
class CVarMemoryPool
{
    struct MemoryPage
    {
        MemoryPage* Next;	// next memory page
    };

public:
    CVarMemoryPool();
    ~CVarMemoryPool();

    /**
    * @brief
    * 创建可变内存池
    * @param dwPageSize : 内部分配的内存页大小，内存不够时，内存池会申请一块新的内存页
    * @return 创建成功返回TRUE，否则返回FALSE
    **/
    bool PPAPI Create(unsigned int dwPageSize = 0x80000);

    /**
    * @brief
    * 分配Len长度的Buffer
    * @param dwLen : 获得的内存块大小长度
    * @return  返回的内存，如果返回为NULL，则代表分配失败
    **/
    void* PPAPI Malloc(unsigned int dwLen);

    /**
    * @brief
    * 回收内存
    * @param p : 指向需要回收的内存
    * @return void
    **/
    void PPAPI Free(void* p);

    /**
    * @brief
    * 清空内存池，使所有的内存都可以使用，此方法不会将内存返回给操作系统
    * @return void
    **/
    void PPAPI Clear();

    /**
    * @brief
    * 获取当前内存使用量
    * @return 当前内存使用量
    **/
    int PPAPI GetMemUsed();

private:
    void* GetPoolMemory(unsigned int dwLen);
    void FreePoolMemory(void* pMemBlock, unsigned char dwType);

    bool AddFreeMemory(int dwIndex);
    bool SetMemoryPage();

    inline char* GetPageBufGegin(MemoryPage *pPage)
    {
        return (char*)(pPage + 1);
    }

    inline char* GetPageBufEnd(MemoryPage *pPage)
    {
        return (char*)(pPage + 1) + m_nPageSize;
    }

private:
    static const unsigned int ALIGNMENT = 8;
    static const unsigned int ALLOC_COUNT = 16;
    static const unsigned int MIN_PAGESIZE = 0x40000;	// min pPage size
    static const unsigned int MAX_UNIT_SIZE = 128;
    static const unsigned int UNIT_TYPE_COUNT = 16;

    char* m_pFreeHead[UNIT_TYPE_COUNT];
    int m_nFreeCount[UNIT_TYPE_COUNT];

    MemoryPage* m_pHeadPage;
    MemoryPage* m_pWorkPage;
    char* m_pPageBuf;
    unsigned int m_nPageSize;
};




/**
*@brief 固定内存块大小内存池，用于分配固定大小的内存块
* 内存池申请的内存不会自动释放，只在内存池销毁时释放
* 注意：此类“非”线程安全
* 使用示例：
* CFixMemoryPool pool;
* pool.Create(128);
* 某一线程：
* char * p1 = pool.Malloc();//记得加锁
* char * p2 = pool.Malloc();//记得加锁
* char * p3 = pool.Malloc();//记得加锁
*
* 另外一个线程：
* pool.Free(p3);//记得加锁
* pool.Free(p2);//记得加锁
* pool.Free(p1);//记得加锁
*/
class CFixMemoryPool
{
    struct MemoryPage
    {
        MemoryPage* Next;	// next memory page
        int nFreeHead;		// the first free unit in page
        int nFreecount;		// free unit in page
    };

public:
    CFixMemoryPool();
    ~CFixMemoryPool();

    /**
    * @brief
    * 初始化内存池
    * @param dwUnitSize : 每一个分配的内存块大小
    * @param dwPageSize : 内部分配的内存页大小，内存不够时，内存池会申请一块新的内存页
    * @return  创建成功返回true，失败返回false
    **/
    bool PPAPI Create(unsigned int dwUnitSize, unsigned int dwPageSize = 0x40000);

    /**
    * @brief
    * 得到一块新的内存
    * @return  void*
    **/
    void* PPAPI Malloc();

    /**
    * @brief
    * 归还一块分配的内存
    * @param p : 内存的地址
    * @return  void
    **/
    void PPAPI Free(void* p);

    /**
    * @brief
    * 清空内存池，使所有的内存都可以使用，此方法不会将内存返回给操作系统
    * @return void
    **/
    void PPAPI Clear();

    /**
    * @brief
    * 获取当前内存使用量
    * @return 当前内存使用量
    **/
    int PPAPI GetMemUsed();

private:
    bool AddMemoryPage();
    void InitPage(MemoryPage *pPage);

    inline char* GetPageBuf(MemoryPage *pPage)
    {
        return (char*)(pPage + 1);
    }

private:
    static const int ALIGNMENT = 4;
    static const unsigned int MIN_PAGESIZE = 0x40000;	// min page size

    MemoryPage* m_pHeadPage;					// first page
    unsigned int m_nUnitSize;					// unit memory size
    unsigned int m_nPageSize;					// total memory in page
};

#endif
