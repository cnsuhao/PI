#ifndef EASYPR_BASEDEF_H_
#define EASYPR_BASEDEF_H_

#ifdef EASYPR_BUILD
#	ifdef EASYPR_STATIC
#		define EASYPR_API 
#	else
#		define EASYPR_API  _declspec(dllexport)
#	endif // !EASYPR_STATIC
#else
#	ifdef EASYPR_STATIC
#		define EASYPR_API 
#	else
#		define EASYPR_API  _declspec(dllimport)
#	endif // !EASYPR_STATIC
#endif // !EASYPR_BUILD

#endif // !EASYPR_BASEDEF_H_
