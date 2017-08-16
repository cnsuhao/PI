#ifndef __JZ_LOADER_H__
#define __JZ_LOADER_H__
#include <Windows.h>

// dll释放函数
inline void JZDLL_Unload(HMODULE hDLL)
{
	if (NULL != hDLL)
	{
		FreeLibrary(hDLL);
		hDLL = NULL;
	}
}

// dll加载函数
inline HMODULE JZDLL_Load(LPCTSTR szDLLParentPathLPCTSTR, 
	LPCTSTR* pszEnvRefDebug,		// 加载库在 CoreDLL 目录下需要设置的环境变量的相对路径
	intptr_t nEnvDebugCount,		// 环境变量相对路径的个数（包括加载库自身所在的相对路径）
	LPCTSTR *pszEnvRefRelease,
	intptr_t nEnvReleaseCount,
	LPCTSTR szDLLName,			// 加载库的名字
	LPCSTR szAPIExtName,			// 加载库导出接口的名字
	void* *ppAPI)					// 输出功能组接口指针
{
	if (NULL == szDLLName || NULL == szAPIExtName)
	{
		return NULL;
	}

	HMODULE hDLL = NULL;

}

template<class _Func, class _Pos>
class JZLoader
{
public:
	JZLoader() throw() : _Moudle(NULL), _Fpval(NULL)
	{
		// 加载库，并获取接口指针
		Load(NULL);
	}

	~JZLoader() throw()
	{
		// 释放库
		if (NULL != _Moudle)
		{
			JZDLL_Unload(_Moudle);
		}
	}

	bool Load(LPCTSTR szDLLParentPath)
	{
		if (_IsLoaded())
		{
			return true;
		}
		else
		{
			_Moudle = JZDLL_Load(szDLLParentPath);
			return _IsLoaded();
		}	
	}

	// 重载本类型对象到bool的隐式转换
	operator bool() const
	{
		return (NULL != _Fpval);
	}

	operator _Func* ()
	{
		return _Fpval;
	}

	// 重载->指针运算符
	_Func* operator ->() const
	{
		return _Fpval;
	}

private:
	JZLoader(JZLoader<_Func, _Pos> const&) throw();
	// JZLoader<_Func, _Pos>& operator=(JZLoader<_Func, _Pos> const&) throw();
	bool _IsLoaded()
	{
		return (NULL != _Moudle);
	}

private:
	HMODULE _Moudle;
	_Func* _Fpval;
};
#endif // __JZ_LOADER_H__

