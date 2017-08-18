#ifndef __JZ_LOADER_H__
#define __JZ_LOADER_H__
#include <Windows.h>
#include <tchar.h>
#include <io.h>
#include <JZAutoSetEnv.h>

// dll释放函数
inline void JZDLL_Unload(HMODULE hDLL)
{
	if (NULL != hDLL)
	{
		FreeLibrary(hDLL);
		hDLL = NULL;
	}
}

// 加载库子函数
inline HMODULE _JZDLL_LoadLibrary(LPCTSTR szDLLName, // 动态库名，不包含扩展名 
								  LPCTSTR szPostfix) // 后缀名，包含了扩展名
{
	TCHAR s_name[MAX_PATH] = { 0 };
	s_name[0] = '\0';
	_tcscpy(s_name, szDLLName);
	_tcscat(s_name, szPostfix);
	return LoadLibrary(s_name);
}


typedef void* (*DefGetAPIStuPtr)();
// 获取API指针
inline void _JZDLL_GetAPI(HMODULE& hDLL,			// 已加载的库句柄
						  LPCSTR szAPIExportName,	// 加载库导出接口的名字
						  void** ppAPI)				// 输出功能组接口指针
{
	DefGetAPIStuPtr pfnGet = reinterpret_cast<DefGetAPIStuPtr>(GetProcAddress(hDLL, szAPIExportName));
	*ppAPI = pfnGet();
	if (NULL == *ppAPI)
	{
		JZDLL_Unload(hDLL);	// 如果导出接口不成功，则释放 DLL
		hDLL = NULL;
	}
}

// dll加载函数
inline HMODULE JZDLL_Load(
	LPCTSTR szDLLName,				// 加载库的名字
	LPCSTR szAPIExtName,			// 加载库导出接口的名字
	void** ppAPI)					// 输出功能组接口指针
{
	// 参数检查
	if (NULL == szDLLName || NULL == szAPIExtName)
	{
		return NULL;
	}

	// 将Import_dll目录下的所有子目录设置为环境变量
	JZDLL_SetEnv(NULL);

	HMODULE hDLL = NULL;
	// 预先加载一次，避免重复的搜索缺省路径，尤其是单件加载器被多个库调用时
#ifdef _DEBUG
	hDLL = _JZDLL_LoadLibrary(szDLLName, JZDLL_POSTFIX_DEBUG); // 如果系统处于 DEBUG 模式下时，尝试导入 DEBUG 版本库
#endif // _DEBUG
	if (NULL == hDLL) // 如果不在 DEBUG 模式下或导入 DEBUG 版本库失败，则尝试导入 RELEASE 版本库
	{
		hDLL = _JZDLL_LoadLibrary(szDLLName, JZDLL_POSTFIX_RELEASE);
	}

	if (NULL != hDLL) // 如果加载成功则直接返回，否则继续执行下面代码进行搜索
	{
		_JZDLL_GetAPI(hDLL, szAPIExtName, ppAPI);
		return hDLL;
	}

	TCHAR s_top[MAX_PATH] = { 0 };
	TCHAR s_env[JZENV_MAX_LENGTH] = { 0 };
	s_top[0] = '\0';
	s_env[0] = '\0';

	LPCTSTR p_separator = NULL;
	GetModuleFileName(NULL, s_top, MAX_PATH - 1);
	for (int i = 0; i < 4; i++)
	{
		p_separator = _tcsrchr(s_top, JZDIR_SEPARATOR);
		if (NULL == p_separator)
		{
			return NULL;
		}
		*(LPTSTR)p_separator = '\0';
	}
	_tcscat(s_top, JZ_DLL);
	_tcscat(s_top, JZDLL_DIR);

	GetEnvironmentVariable(_T("Path"), s_env, JZENV_MAX_LENGTH - 1);
	_tcscat(s_env, JZENV_SEPARATOR_S);

	// 如果系统环境变量拥有当前环境变量则不再加入
	if (NULL == _tcsstr(s_env, s_top))
	{
		_tcscat(s_env, s_top);
	}
	SetEnvironmentVariable(_T("Path"), s_env);

#ifdef _DEBUG
	hDLL = _JZDLL_LoadLibrary(szDLLName, JZDLL_POSTFIX_DEBUG); // 如果系统处于 DEBUG 模式下时，尝试导入 DEBUG 版本库
#endif // _DEBUG
	if (NULL == hDLL) // 如果不在 DEBUG 模式下或导入 DEBUG 版本库失败，则尝试导入 RELEASE 版本库
	{
		hDLL = _JZDLL_LoadLibrary(szDLLName, JZDLL_POSTFIX_DEBUG);
	}

	_JZDLL_GetAPI(hDLL, szAPIExtName, ppAPI);
	return hDLL;
}

// 自动加载器
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
			_Pos _Pval;
			_Moudle = JZDLL_Load(_Pval.DLLName(), _Pval.APIExtName(), (void**)&_Fpval);
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
	JZLoader<_Func, _Pos>& operator=(JZLoader<_Func, _Pos> const&) throw();
	bool _IsLoaded()
	{
		return (NULL != _Moudle);
	}

private:
	HMODULE _Moudle;
	_Func* _Fpval;
};
#endif // __JZ_LOADER_H__

