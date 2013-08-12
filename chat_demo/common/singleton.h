#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include <new>

#define DEFINE_SINGLETON(cls) \
\
private:\
	static cls * m_pInstance;\
public:\
	static bool CreateInstance()\
	{\
		if(NULL == m_pInstance)\
		{\
			m_pInstance = new (std::nothrow) cls();\
		}\
		return NULL != m_pInstance;\
	}\
	static cls* Instance()\
	{\
		return m_pInstance;\
	}\
	static void DestroyInstance()\
	{\
		if(NULL != m_pInstance)\
		{\
			delete m_pInstance;\
		}\
		m_pInstance = NULL;\
	}

#define IMPLEMENT_SINGLETON(cls) \
	cls * cls::m_pInstance = NULL;

#endif
