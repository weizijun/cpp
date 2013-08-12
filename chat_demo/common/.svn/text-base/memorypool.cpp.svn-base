#include "memorypool.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

CVarMemoryPool::CVarMemoryPool()
    :m_pHeadPage(NULL), m_pWorkPage(NULL), m_pPageBuf(NULL)
{
    for (unsigned int i = 0; i < UNIT_TYPE_COUNT; ++ i)
    {
        m_pFreeHead[i] = NULL;
        m_nFreeCount[i] = 0;
    }
}

CVarMemoryPool::~CVarMemoryPool()
{
    MemoryPage* pMemoryPage = m_pHeadPage;
    while (m_pHeadPage != NULL)
    {
        pMemoryPage = m_pHeadPage->Next;
        free(m_pHeadPage);
        m_pHeadPage = pMemoryPage;
    }
}

void* CVarMemoryPool::Malloc(unsigned int Len)
{
    assert(Len > 0);

    Len ++;
    if (Len > MAX_UNIT_SIZE)
    {
        // allocate memory from system if requery Len is too large
        void* buf = malloc(Len);
        if (buf == NULL)
        {
            return NULL;
        }

        //if content of 1 byte before memory means allocate form system
        *(char*)buf = 0;

        return (char*)buf + 1;
    }
    else
    {
        return GetPoolMemory(Len);
    }
}

void CVarMemoryPool::Free(void* p)
{
    assert(p != NULL);

    char* temp = (char*)p - 1;
    unsigned char type = *temp;
    if (type == 0)	//if content of 1 byte before memory means allocate form system
    {
        free(temp);
    }
    else
    {
        FreePoolMemory(temp, type);
    }
}

void* CVarMemoryPool::GetPoolMemory(unsigned int Len)
{
    Len = (Len + (ALIGNMENT-1)) & ~(ALIGNMENT-1);
    int idx = (Len - 1) / ALIGNMENT;

    //if free memory unit is not enough, first get some free units
    if (m_nFreeCount[idx] == 0
            && !AddFreeMemory(idx))
    {
        return NULL;
    }

    -- m_nFreeCount[idx];
    char* buf = m_pFreeHead[idx];
    m_pFreeHead[idx] = (char*)(*((long*)m_pFreeHead[idx]));
    *buf = idx + 1;

    return buf + 1;
}

void CVarMemoryPool::FreePoolMemory(void* memblock, unsigned char type)
{
    int idx = type - 1;
    *(long*)memblock = (long)m_pFreeHead[idx];
    m_pFreeHead[idx] = (char*)memblock;
    ++ m_nFreeCount[idx];
}

bool CVarMemoryPool::AddFreeMemory(int idx)
{
    const int UNIT_SIZE = (idx + 1) * ALIGNMENT;

    if ((m_pPageBuf + UNIT_SIZE ) > GetPageBufEnd(m_pWorkPage)
            && !SetMemoryPage())
    {
        return false;
    }

    char* page_end = GetPageBufEnd(m_pWorkPage);
    for (unsigned int i = 0; i < ALLOC_COUNT; ++ i)
    {
        *(long*)m_pPageBuf = (long)m_pFreeHead[idx];
        m_pFreeHead[idx] = m_pPageBuf;

        m_pPageBuf += UNIT_SIZE;
        ++ m_nFreeCount[idx];

        if (m_pPageBuf + UNIT_SIZE > page_end)
            break;
    }

    return true;
}

bool CVarMemoryPool::SetMemoryPage()
{
    if(m_pWorkPage->Next != NULL)
    {
        m_pWorkPage = m_pWorkPage->Next;
    }
    else
    {
        void* buf = malloc(sizeof(MemoryPage) + m_nPageSize);
        if (buf == NULL)
        {
            return false;
        }
        else
        {
            MemoryPage* pMemoryPage = (MemoryPage*)(buf);
            pMemoryPage->Next = NULL;
            m_pWorkPage->Next = pMemoryPage;
            m_pWorkPage = pMemoryPage;
        }
    }
    m_pPageBuf = GetPageBufGegin(m_pWorkPage);
    return true;
}

int CVarMemoryPool::GetMemUsed()
{
    int used = 0;
    const int PAGE_SIZE = sizeof(MemoryPage) + m_nPageSize;

    MemoryPage* pMemoryPage = m_pHeadPage;
    while (pMemoryPage != NULL)
    {
        pMemoryPage = pMemoryPage->Next;
        used += PAGE_SIZE;
    }

    return used;
}

void CVarMemoryPool::Clear()
{
    m_pWorkPage = m_pHeadPage;
    m_pPageBuf = GetPageBufGegin(m_pWorkPage);
}

bool CVarMemoryPool::Create( unsigned int PageSize /*= 0x80000*/ )
{
    PageSize = (PageSize + (ALIGNMENT-1)) & ~(ALIGNMENT-1);
    if (PageSize < MIN_PAGESIZE)
    {
        m_nPageSize = MIN_PAGESIZE;
    }
    else
    {
        m_nPageSize = PageSize;
    }

    void* buf = malloc(sizeof(MemoryPage) + m_nPageSize);
    if (buf == NULL)
    {
        return false;
    }
    else
    {
        MemoryPage* pMemoryPage = (MemoryPage*)(buf);
        pMemoryPage->Next = NULL;
        m_pWorkPage = pMemoryPage;
        m_pPageBuf = GetPageBufGegin(m_pWorkPage);
        m_pHeadPage = m_pWorkPage;
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////

CFixMemoryPool::CFixMemoryPool()
    :m_pHeadPage(NULL), m_nUnitSize(0), m_nPageSize(0)
{
}

CFixMemoryPool::~CFixMemoryPool()
{
    MemoryPage* pMemoryPage = m_pHeadPage;
    while (m_pHeadPage != NULL)
    {
        pMemoryPage = m_pHeadPage->Next;
        free(m_pHeadPage);
        m_pHeadPage = pMemoryPage;
    }
}

void* CFixMemoryPool::Malloc()
{
    MemoryPage* pMemoryPage = m_pHeadPage;
    while (pMemoryPage != NULL && pMemoryPage->nFreecount == 0)
    {
        pMemoryPage = pMemoryPage->Next;
    }

    // add new page if space is not enough
    if (pMemoryPage == NULL)
    {
        if(!AddMemoryPage())
        {
            return NULL;
        }
        pMemoryPage = m_pHeadPage;
    }

    // get unused memory
    -- pMemoryPage->nFreecount;
    char* buf = GetPageBuf(pMemoryPage) + pMemoryPage->nFreeHead * m_nUnitSize;
    pMemoryPage->nFreeHead = *(int*)(buf);

    return buf;
}

void CFixMemoryPool::Free(void* p)
{
    // don't check null point for fast
    MemoryPage* pMemoryPage = m_pHeadPage;
    char* buf = GetPageBuf(m_pHeadPage);

    // find point in which page
    while((p < buf ||
            p > buf + m_nPageSize) &&
            pMemoryPage != NULL)
    {
        pMemoryPage = pMemoryPage->Next;
        buf = GetPageBuf(pMemoryPage);
    }

    // do not in any page
    if (pMemoryPage == NULL)
    {
        return;
    }

    *(int*)p = pMemoryPage->nFreeHead;
    pMemoryPage->nFreeHead = ((char*)p - buf) / m_nUnitSize;
    ++ pMemoryPage->nFreecount;

    return;
}

bool CFixMemoryPool::AddMemoryPage()
{
    void* buf = malloc(sizeof(MemoryPage) + m_nPageSize);
    if (buf == NULL)
    {
        return false;
    }

    MemoryPage* pMemoryPage = (MemoryPage*)(buf);
    InitPage(pMemoryPage);

    if (m_pHeadPage == NULL)
    {
        pMemoryPage->Next = NULL;
        m_pHeadPage = pMemoryPage;
    }
    else
    {
        pMemoryPage->Next = m_pHeadPage;
        m_pHeadPage = pMemoryPage;
    }

    return true;
}

int CFixMemoryPool::GetMemUsed()
{
    int used = 0;
    const int PAGE_SIZE = sizeof(MemoryPage) + m_nPageSize;

    MemoryPage* pMemoryPage = m_pHeadPage;
    while (pMemoryPage != NULL)
    {
        pMemoryPage = pMemoryPage->Next;
        used += PAGE_SIZE;
    }

    return used;
}

void CFixMemoryPool::Clear()
{
    MemoryPage* pMemoryPage = m_pHeadPage;
    while (pMemoryPage != NULL)
    {
        InitPage(pMemoryPage);
        pMemoryPage = pMemoryPage->Next;
    }
}

void CFixMemoryPool::InitPage(MemoryPage *Page)
{
    Page->nFreecount = m_nPageSize / m_nUnitSize;
    Page->nFreeHead = 0;

    void* head = GetPageBuf(Page);
    for (int i = 1; i < Page->nFreecount; ++i)
    {
        *(int*)head = i;
        head = (int*)((char*)head + m_nUnitSize);
    }
}

bool CFixMemoryPool::Create( unsigned int UnitSize, unsigned int PageSize /*= 0x40000*/ )
{
    if (UnitSize < 4)
    {
        m_nUnitSize = 4;
    }
    {
        m_nUnitSize = (UnitSize + (ALIGNMENT-1)) & ~(ALIGNMENT-1);
    }

    if (PageSize < MIN_PAGESIZE)
    {
        m_nPageSize = MIN_PAGESIZE;
    }
    else
    {
        m_nPageSize = (PageSize / m_nUnitSize) * m_nUnitSize;
    }

    return AddMemoryPage();
}
