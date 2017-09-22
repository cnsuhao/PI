#ifndef EASYPR_UTIL_KV_H_
#define EASYPR_UTIL_KV_H_

#include <map>
#include <string>
#include "basedef.h"
namespace easypr
{
	// 中文映射器
	class EASYPR_API Kv
	{
	public:
		// 构造函数
		Kv();
		// 根据xml文件读取中文映射表
		void load(const std::string &file);
		// 从中文映射表中，得到键-值对
		std::string get(const std::string &key);
		// 从中文映射表中，清除某一个键-值对
		void remove(const std::string &key);
		// 清除中文映射表
		void clear();

	private:
		// 将中文映射对加入中文映射表
		void _add(const std::string &key, const std::string &value);
	private:
		std::map<std::string, std::string>	 m_data;	// 中文映射表
	};

}

#endif // EASYPR_UTIL_KV_H_