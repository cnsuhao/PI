#ifndef __JZ_LOADER_H__
#define __JZ_LOADER_H__
#include <Windows.h>
#include <tchar.h>
#include <io.h>
// ����ַ������ȶ���
#ifndef MAX_PATH
#	define MAX_PATH		260
#endif // !MAX_PATH

// ����������ȱʡ��󳤶ȣ����ϵͳ�����������ȴ��ڸ�ֵʱ����ѡ���޸Ĵ�ֵ��
#ifndef JZENV_MAX_LENGTH
#	define JZENV_MAX_LENGTH	4096
#endif // !JZENV_MAX_LENGTH

//	�����·������׺������
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

// dll�ͷź���
inline void JZDLL_Unload(HMODULE hDLL)
{
	if (NULL != hDLL)
	{
		FreeLibrary(hDLL);
		hDLL = NULL;
	}
}

// ���ؿ��Ӻ���
inline HMODULE _JZDLL_LoadLibrary(LPCTSTR szDLLName, // ��̬��������������չ�� 
								  LPCTSTR szPostfix) // ��׺������������չ��
{
	TCHAR s_name[MAX_PATH] = { 0 };
	s_name[0] = '\0';
	_tcscpy(s_name, szDLLName);
	_tcscat(s_name, szPostfix);
	return LoadLibrary(s_name);
}

typedef void* (*DefGetAPIStuPtr)();
// ��ȡAPIָ��
inline void _JZDLL_GetAPI(HMODULE& hDLL,			// �Ѽ��صĿ���
						  LPCSTR szAPIExportName,	// ���ؿ⵼���ӿڵ�����
						  void** ppAPI)				// ���������ӿ�ָ��
{
	DefGetAPIStuPtr pfnGet = reinterpret_cast<DefGetAPIStuPtr>(GetProcAddress(hDLL, szAPIExportName));
	*ppAPI = pfnGet();
	if (NULL == *ppAPI)
	{
		JZDLL_Unload(hDLL);	// ��������ӿڲ��ɹ������ͷ� DLL
		hDLL = NULL;
	}
}

// ��ָ��Ŀ¼�µ�������Ŀ¼����Ϊ��������
inline void JZDLL_SetEnv(LPCTSTR szImportDLLPath)
{
	if (NULL == szImportDLLPath) // �������·��Ϊ��ʱ���Զ�Ѱ��Import_dllĿ¼
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
	
	// ��ȡ��������
	TCHAR s_env[JZENV_MAX_LENGTH] = { 0 };
	s_env[0] = '\0';
	GetEnvironmentVariable(_T("Path"), s_env, JZENV_MAX_LENGTH - 1);
	_tcscat(s_env, JZENV_SEPARATOR_S);

	// ����ָ��Ŀ¼�µ�������Ŀ¼
	TCHAR s_path[MAX_PATH] = { 0 };
	s_path[0] = '\0';
	_tfinddata_t info_file;
	intptr_t h_file;
	TCHAR s_search[MAX_PATH] = { 0 };
	s_search[0] = '\0';
	_tcscpy(s_search, szImportDLLPath);
	_tcscat(s_search, _T("*"));
	h_file = _tfindfirst(s_search, &info_file);
	if (-1 == h_file) // ���û���������ļ������ļ���
	{
		return;
	}
	else
	{
		// ������ҵĵ�һʱ�ļ�����������ӵ���������
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
		else // ����ҵ��ĵ�һ���ļ�������Բ���
		{	
		}

		// ����ʣ�µ��ļ��л��ļ�
		while (-1 != _tfindnext(h_file, &info_file))
		{
			if (0 != (info_file.attrib & _A_SUBDIR)
				&& _tcscmp(info_file.name, _T("."))
				&& _tcscmp(info_file.name, _T(".."))) // ����ҵ������ļ���������Ϊ��Ҫ��ӵ�����������Ŀ¼
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
			else // ����ҵ��������ļ�������Բ��ܣ��������²���
			{
			}
		}
		_findclose(h_file);
	}

	

}

// dll���غ���
inline HMODULE JZDLL_Load(
	LPCTSTR szDLLName,				// ���ؿ������
	LPCSTR szAPIExtName,			// ���ؿ⵼���ӿڵ�����
	void** ppAPI)					// ���������ӿ�ָ��
{
	// �������
	if (NULL == szDLLName || NULL == szAPIExtName)
	{
		return NULL;
	}

	JZDLL_SetEnv(NULL);// ��ָ��Ŀ¼�µ�������Ŀ¼����Ϊ��������

	HMODULE hDLL = NULL;
	// Ԥ�ȼ���һ�Σ������ظ�������ȱʡ·���������ǵ�������������������ʱ
#ifdef _DEBUG
	hDLL = _JZDLL_LoadLibrary(szDLLName, JZDLL_POSTFIX_DEBUG); // ���ϵͳ���� DEBUG ģʽ��ʱ�����Ե��� DEBUG �汾��
#endif // _DEBUG
	if (NULL == hDLL) // ������� DEBUG ģʽ�»��� DEBUG �汾��ʧ�ܣ����Ե��� RELEASE �汾��
	{
		hDLL = _JZDLL_LoadLibrary(szDLLName, JZDLL_POSTFIX_RELEASE);
	}

	if (NULL != hDLL) // ������سɹ���ֱ�ӷ��أ��������ִ����������������
	{
		_JZDLL_GetAPI(hDLL, szAPIExtName, ppAPI);
		return hDLL;
	}

	TCHAR s_top[MAX_PATH] = { 0 };
	TCHAR s_env[JZENV_MAX_LENGTH] = { 0 };
	s_top[0] = '\0';
	s_env[0] = '\0';

	TCHAR s_search[MAX_PATH] = { 0 };
	s_search[0] = '\0';


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

	// ���ϵͳ��������ӵ�е�ǰ�����������ټ���
	if (NULL == _tcsstr(s_env, s_top))
	{
		_tcscat(s_env, s_top);
	}
	SetEnvironmentVariable(_T("Path"), s_env);

#ifdef _DEBUG
	hDLL = _JZDLL_LoadLibrary(szDLLName, JZDLL_POSTFIX_DEBUG); // ���ϵͳ���� DEBUG ģʽ��ʱ�����Ե��� DEBUG �汾��
#endif // _DEBUG
	if (NULL == hDLL) // ������� DEBUG ģʽ�»��� DEBUG �汾��ʧ�ܣ����Ե��� RELEASE �汾��
	{
		hDLL = _JZDLL_LoadLibrary(szDLLName, JZDLL_POSTFIX_DEBUG);
	}

	_JZDLL_GetAPI(hDLL, szAPIExtName, ppAPI);
	return hDLL;
}

template<class _Func, class _Pos>
class JZLoader
{
public:
	JZLoader() throw() : _Moudle(NULL), _Fpval(NULL)
	{
		// ���ؿ⣬����ȡ�ӿ�ָ��
		Load(NULL);
	}

	~JZLoader() throw()
	{
		// �ͷſ�
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

	// ���ر����Ͷ���bool����ʽת��
	operator bool() const
	{
		return (NULL != _Fpval);
	}

	operator _Func* ()
	{
		return _Fpval;
	}

	// ����->ָ�������
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

