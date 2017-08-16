#ifndef __JZ_LOADER_H__
#define __JZ_LOADER_H__
#include <Windows.h>

// dll�ͷź���
inline void JZDLL_Unload(HMODULE hDLL)
{
	if (NULL != hDLL)
	{
		FreeLibrary(hDLL);
		hDLL = NULL;
	}
}

// dll���غ���
inline HMODULE JZDLL_Load(LPCTSTR szDLLParentPathLPCTSTR, 
	LPCTSTR* pszEnvRefDebug,		// ���ؿ��� CoreDLL Ŀ¼����Ҫ���õĻ������������·��
	intptr_t nEnvDebugCount,		// �����������·���ĸ������������ؿ��������ڵ����·����
	LPCTSTR *pszEnvRefRelease,
	intptr_t nEnvReleaseCount,
	LPCTSTR szDLLName,			// ���ؿ������
	LPCSTR szAPIExtName,			// ���ؿ⵼���ӿڵ�����
	void* *ppAPI)					// ���������ӿ�ָ��
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
			_Moudle = JZDLL_Load(szDLLParentPath);
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

