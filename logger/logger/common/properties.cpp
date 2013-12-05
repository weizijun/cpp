#include "properties.h"
#include <locale.h>
#include <clocale>
#include <iostream>
#include <fstream>

#include <new>

////////////////////////////////////////////////////////////////

CProperties::CProperties()
{
	pProperties = new (std::nothrow) PropertiesMap();
}

CProperties::~CProperties()
{
	delete pProperties;
}

CProperties::CProperties(const CProperties & rhs)
{
	pProperties = new (std::nothrow) PropertiesMap();
	*this->pProperties = *rhs.pProperties;
}

CProperties & CProperties::operator = (const CProperties & rhs)
{
	if(this != &rhs)
	{
		delete pProperties;
		pProperties = new (std::nothrow) PropertiesMap();
		*this->pProperties = *rhs.pProperties;
	}
	return *this;
}

void CProperties::AddProperties(const CProperties * pAnother)
{
	for(PropertiesMapItr itr = pAnother->pProperties->begin(); itr != pAnother->pProperties->end(); ++itr)
	{
		this->AddProperty(itr->first, itr->second);
	}
}

int CProperties::ImportProperties(const char * pFilePath, const char * pLocale)
{
	if(NULL == pFilePath)
	{
		return -1;
	}
	std::ifstream ifs(pFilePath);
	ifs.imbue(std::locale(pLocale));
	if(!ifs)
	{
		return -2;
	}
	std::string s;
	int count = 0;
	while(std::getline(ifs, s))
	{
		if('#' == s[0])
		{
			continue;
		}
		if('/' == s[0] && '/' == s[1])
		{
			continue;
		}
		std::string::size_type key_end_pos = s.find("=");
		if(std::string::npos == key_end_pos)
		{
			continue;
		}
		std::string key = s.substr(0, key_end_pos);
		std::string value = s.substr(key_end_pos + 1);
		if(0 == value.length())
		{
			continue;
		}
#if (defined(LINUX))
		if(value[value.length() - 1] == '\r')
		{
			value = value.substr(0, value.length() - 1);
		}
#endif
		this->AddProperty(key, value);
		count++;
	}
	return count;
}

bool CProperties::ExportProperties(const char * pFilePath, const char * pLocale)
{
	if(NULL == pFilePath)
	{
		return false;
	}
	std::locale locl = std::locale::global(std::locale(pLocale));
	std::ofstream ofs(pFilePath);
	ofs.imbue(std::locale(pLocale));
	if(!ofs)
	{
		return false;
	}
	ofs << this->toString() << std::endl;
	ofs.flush();
	return true;
}

void CProperties::AddProperty(const std::string & Key, const std::string & Value)
{
	this->pProperties->insert(std::make_pair(Key, Value));
}

bool CProperties::RemoveProperty(const std::string & Key)
{
	PropertiesMapItr itr = this->pProperties->find(Key);
	if(itr == this->pProperties->end())
	{
		return false;
	}
	this->pProperties->erase(itr);
	return true;
}

char * CProperties::GetProperty(const std::string & Key, char * pValueBuffer, unsigned int Length)
{
	PropertiesMapItr itr = this->pProperties->find(Key);
	if(itr == this->pProperties->end())
	{
		return NULL;
	}
	if(itr->second.length() + 1 > Length)
	{
		return NULL;
	}
	strncpy(pValueBuffer, itr->second.c_str(), Length);
	return pValueBuffer;
}

//template <typename T>
//T CProperties::GetProperty(const std::string & Key, const T & Def)
//{
//	std::string value = _GetProperty();
//	return value.empty() ? Def : ConvertTraits<T>(value);
//}

int CProperties::Size()
{
	return pProperties->size();
}

std::string CProperties::toString()
{
	std::string toString("");
	PropertiesMapItr itr = this->pProperties->begin();
	for(; itr != this->pProperties->end(); ++itr)
	{
		toString.append(itr->first);
		toString.append("=");
		toString.append(itr->second);
#if (defined(WIN32) || defined(WIN64))
		toString.append("\r\n");
#else
		toString.append("\n");
#endif
	}
	return toString;
}

std::string CProperties::_GetProperty(const std::string & Key)
{
	PropertiesMapItr itr = this->pProperties->find(Key);
	if(itr != this->pProperties->end())
	{
		return itr->second;
	}
	return "";
}
