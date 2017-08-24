#ifndef __JZ_AUTOSETENV_H__
#define __JZ_AUTOSETENV_H__
#include <Windows.h>
#include <tchar.h>
#include <io.h>
// 最大字符串长度定义
#ifndef MAX_PATH
#	define MAX_PATH		260
#endif // !MAX_PATH

// 环境变量的缺省最大长度，如果系统环境变量长度大于该值时，可选择修改此值。
#ifndef JZENV_MAX_LENGTH
#	define JZENV_MAX_LENGTH	4096
#endif // !JZENV_MAX_LENGTH

//	库相对路径与库后缀名声明
#define JZ_DLL						_T("\\dll")
#define JZDLL_POSTFIX_DEBUG			_T("d.dll")
#define JZDLL_POSTFIX_RELEASE		_T(".dll")
#define JZDIR_SEPARATOR				_T('\\')
#define JZDIR_SEPARATOR_S			_T("\\")
#define JZENV_SEPARATOR				_T(';')
#define JZENV_SEPARATOR_S			_T(";")
#ifdef _DEBUG
#		ifdef _WIN64
#			define JZDLL_DIR		_T("\\x64\\Debug")
#		else
#			define JZDLL_DIR		_T("\\Win32\\Debug")
#		endif // _WIN64
#	else
#		ifdef _WIN64
#			define JZDLL_DIR		_T("\\x64\\Release")
#		else
#			define JZDLL_DIR		_T("\\Win32\\Release")
#		endif // _WIN64
#endif // _DEBUG


// 将指定目录下的所有子目录设置为环境变量
inline void JZDLL_SetEnv(LPCTSTR szImportDLLPath)
{
	if (NULL == szImportDLLPath) // 当传入的路径为空时，自动寻找Import_dll目录
	{
		TCHAR s_top[MAX_PATH] = { 0 };
		s_top[0] = '\0';
		GetModuleFileName(NULL, s_top, MAX_PATH - 1);
		LPCTSTR p_separator = NULL;
		GetModuleFileName(NULL, s_top, MAX_PATH - 1);
		for (int i = 0; i < 4; i++)
		{
			p_separator = _tcsrchr(s_top, JZDIR_SEPARATOR);
			if (NULL == p_separator)
			{
				return ;
			}
			*(LPTSTR)p_separator = '\0';
		}
		_tcscat(s_top, JZDIR_SEPARATOR_S);
		_tcscat(s_top, _T("dll\\Import_dll\\"));
		szImportDLLPath = s_top;
	}
	
	// 获取环境变量
	TCHAR s_env[JZENV_MAX_LENGTH] = { 0 };
	s_env[0] = '\0';
	GetEnvironmentVariable(_T("Path"), s_env, JZENV_MAX_LENGTH - 1);
	_tcscat(s_env, JZENV_SEPARATOR_S);

	// 查找指定目录下的所有子目录
	TCHAR s_path[MAX_PATH] = { 0 };
	s_path[0] = '\0';
	_tfinddata_t info_file;
	intptr_t h_file;
	TCHAR s_search[MAX_PATH] = { 0 };
	s_search[0] = '\0';
	_tcscpy(s_search, szImportDLLPath);
	_tcscat(s_search, _T("*"));
	h_file = _tfindfirst(s_search, &info_file);
	if (-1 == h_file) // 如果没有搜索到文件和子文件夹
	{
		return;
	}
	else
	{
		// 如果查找的第一时文件夹则将其作添加到环境变量
		if (0 != (info_file.attrib & _A_SUBDIR)
			&& _tcscmp(info_file.name, _T("."))
			&& _tcscmp(info_file.name, _T("..")))
		{
			_tcscat(s_path, szImportDLLPath);
			_tcscat(s_path, info_file.name);
			_tcscat(s_path, JZDLL_DIR);
			_tcscat(s_path, JZENV_SEPARATOR_S);
			if (NULL == _tcsstr(s_env, s_path))
			{
				SetEnvironmentVariable(_T("Path"), s_path);
			}
			s_path[0] = '\0';
		}
		else // 如果找到的第一是文件，则忽略不管
		{	
		}

		// 查找剩下的文件夹或文件
		while (-1 != _tfindnext(h_file, &info_file))
		{
			if (0 != (info_file.attrib & _A_SUBDIR)
				&& _tcscmp(info_file.name, _T("."))
				&& _tcscmp(info_file.name, _T(".."))) // 如果找到了子文件夹则将其作为将要添加到环境变量的目录
			{
				_tcscat(s_path, szImportDLLPath);
				_tcscat(s_path, info_file.name);
				_tcscat(s_path, JZDLL_DIR);
				_tcscat(s_path, JZENV_SEPARATOR_S);
				if (NULL == _tcsstr(s_env, s_path))
				{
					SetEnvironmentVariable(_T("Path"), s_path);
				}
				s_path[0] = '\0';
			}
			else // 如果找到的是子文件，则忽略不管，继续往下查找
			{
			}
		}
		_findclose(h_file);
	}

	

}


// 自动设置环境变量
class JZAutoSetEnv
{
public:
	JZAutoSetEnv() throw()
	{
		// 缺省调用，自动设置环境变量
		JZDLL_SetEnv(NULL);
	}

	~JZAutoSetEnv() 
	{
	}
};

//JZAutoSetEnv g_autoSet;
#endif // __JZ_AUTOSETENV_H__

