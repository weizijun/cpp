#ifndef __OBJECT_POOL_H__
#define __OBJECT_POOL_H__

#include "memorypool.h"

template<typename TO>
class CObjectPool
{
public:
	CObjectPool()
	{
	}

	~CObjectPool()
	{
	}

	/**
    * @brief
    * ��ʼ�����������
    * @param dwGrowCount : ÿ�������ķ�������
    * @return  �����ɹ�����true��ʧ�ܷ���false
    **/
    bool PPAPI Create(unsigned int dwGrowCount = 1)
	{
		unsigned int dwUnitSize = sizeof(TO);
		bool ret = false;
		ret = m_Pool.Create(dwUnitSize, dwGrowCount * dwUnitSize);
		return ret;
	}

    /**
    * @brief
    * ����һ���µĶ���
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
    * �ͷ�һ������
    * @param p : ָ������ָ��
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
		return m_Pool.Malloc();
	}

    void Free(void* p)
	{
		m_Pool.Free(p);
	}

private:
	CFixMemoryPool m_Pool;
};

#endif
