#ifndef __PROPERTIES_H__
#define __PROPERTIES_H__

#include <string>
#include <map>

template <typename T>
inline T ConvertTrait(const std::string & value)
{
	return value;
}

template <>
inline int ConvertTrait<int>(const std::string & value)
{
	return atoi(value.c_str());
}

template <>
inline long ConvertTrait<long>(const std::string & value)
{
	return atol(value.c_str());
}

template <>
inline unsigned int ConvertTrait<unsigned int>(const std::string & value)
{
	return atoi(value.c_str());
}

template <>
inline unsigned long ConvertTrait<unsigned long>(const std::string & value)
{
	return atol(value.c_str());
}

template <>
inline float ConvertTrait<float>(const std::string & value)
{
	return (float)atof(value.c_str());
}

template <>
inline double ConvertTrait<double>(const std::string & value)
{
	return atof(value.c_str());
}

template <>
inline bool ConvertTrait<bool>(const std::string & value)
{
	if(value == "true" 
		|| value == "1" 
		|| value == "TRUE"
		|| value == "True"
		|| value == "yes"
		|| value == "YES"
		|| value == "Yes"
		)
	{
		return true;
	}
	return false;
}

/**
* @brief һ��Key-Value��������Ϣ��,�����ڶ�ȡKey-Value�ļ�(.ini),
* Ҳ���Խ��ڴ��е�Key-Value��Ϣ�洢���ļ��в���,�����������,ɾ��,
* ��ȡKey-Value��Ϣ.����,���������Ϣ��,����ʹ����ģ��,֧�ֽ��ļ��е�����,
* ��ȡΪint,long,float,double�ȸ�������.
*
*/
class CProperties
{
public:
	CProperties();
	~CProperties();
	CProperties(const CProperties & rhs);
	CProperties & operator = (const CProperties & rhs);
	
	/**
	* @brief ����һ��������Ϣ�����ӵ���������Ϣ���У������Key��ͬ���µĽ����Ǿɵ�
	* @param pAnother : �����ӵ�������Ϣ��
	*/
	void AddProperties(const CProperties * pAnother);
	
	/**
	* @brief �����ı�������Ϣ
	* @param pFilePath : ������ļ�·��
	* @return ���ص����Key-Value����
	*/
	int ImportProperties(const char * pFilePath, const char * pLocale = "");
	
	/**
	* @brief ����������Ϣ���ı�
	* @param pFilePath : �������ļ�·��
	* @return �Ƿ񵼳��ɹ�
	*/
	bool ExportProperties(const char * pFilePath, const char * pLocale = "");
	
	/**
	* @brief ����һ��������Ϣ����������Ϣ����
	* @param Key : ���ӵ�Key
	* @param Value : ���ӵ�Value
	*/
	void AddProperty(const std::string & Key, const std::string & Value);
	
	/**
	* @brief ɾ��һ��������Ϣ
	* @param Key : ɾ����Key
	* @return �Ƿ�ɾ���ɹ�
	*/
	bool RemoveProperty(const std::string & Key);
	
	/**
	* @brief ��ȡ�ַ������͵�������Ϣ
	* @param Key : Keyֵ
	* @param pValueBuffer : Value��ŵ�Buffer
	* @param Length : Buffer�ĳ���
	* @return ��ȡ��Valueֵ����ֵ�Ǵ����pValueBufferָ��Ŀռ��еģ�����ʧ�ܣ�����NULL
	*/
	char * GetProperty(const std::string & Key, char * pValueBuffer, unsigned int Length);
	
	/**
	* @brief ��ȡ�κ������������Ϣ
	* @param Key : Keyֵ
	* @param Def : Value��ȱʡֵ
	* @return ����Value,Value������,����Def
	*/
	template <typename T>
	inline T GetProperty(const std::string & Key, const T & Def = T())
	{
		std::string value = _GetProperty(Key);
		return value.empty() ? Def : ConvertTrait<T>(value);
	}
	
	/**
	* @brief Ŀǰ������Ϣ�����ж��ٸ�Key-Value������Ϣ��
	* @return ���ص�Value
	*/
	int Size();

	/**
	* @brief ��ȡ������Ϣ��������������Ϣ��string
	* @return ��ȡ������Ϣ�������������Ϣ��string
	*/
	std::string toString();

private:
	std::string _GetProperty(const std::string & Key);

private:
	typedef std::map<std::string, std::string> PropertiesMap;
	typedef PropertiesMap::iterator PropertiesMapItr;
	PropertiesMap * pProperties;
};
#endif