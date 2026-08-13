#ifndef __MSJEXHND_H__
#define __MSJEXHND_H__

/*
struct tm
{
	int tm_sec;		// 초					범위[0, 59]
	int tm_min;		// 분					범위[0, 59]
	int tm_hour;	// 시간					범위[0, 23]
	int tm_mday;	// 날짜					범위[0, 31]
	int tm_mon;		// 월					범위[0, 11]
	int tm_year;	// 1900년 이후 년도수 
	int tm_wday;	// 요일					범위[0, 6]
	int tm_yday;	// 연중날짜				범위[0, 365]
	int tm_isdst;	// 일광절약시간			범위[0, 59]
};
//*/

#include <windows.h>
/*
#if _MSC_VER < 1300
#define DECLSPEC_DEPRECATED
// VC6: change this path to your Platform SDK headers
#include "M:\\dev7\\vs\\devtools\\common\\win32sdk\\include\\dbghelp.h"   // must be XP version of file
#else
// VC7: ships with updated headers
#include "dbghelp.h"
#endif
//*/
// based on dbghelp.h
typedef BOOL (WINAPI *MINIDUMPWRITEDUMP)(HANDLE hProcess, DWORD dwPid, HANDLE hFile, MINIDUMP_TYPE DumpType,
                                         CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
                                        CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
                                         CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam
                                         );




class MSJExceptionHandler
{
	public:
	
	MSJExceptionHandler( );
	~MSJExceptionHandler( );
	
	void SetLogFileName( PTSTR pszLogFileName );

	private:

	// entry point where control comes on an unhandled exception
	static LONG WINAPI MSJUnhandledExceptionFilter(
								PEXCEPTION_POINTERS pExceptionInfo );

	// where report info is extracted and generated	
	static void GenerateExceptionReport( PEXCEPTION_POINTERS pExceptionInfo );

	// Helper functions
	static LPTSTR GetExceptionString( DWORD dwCode );
	static BOOL GetLogicalAddress( 	PVOID addr, PTSTR szModule, DWORD len,
									DWORD& section, DWORD& offset );
	static void IntelStackWalk( PCONTEXT pContext );

	static void FileCopy(CString strSourceFileName, CString strTargetFileName);

	#if 1
	static void ImagehlpStackWalk( PCONTEXT pContext );
	#endif
	static int __cdecl _tprintf(const TCHAR * format, ...);

	#if 1
	static BOOL InitImagehlpFunctions( void );
	#endif
		
	// Variables used by the class
	static TCHAR m_szLogFileName[MAX_PATH];
	static LPTOP_LEVEL_EXCEPTION_FILTER m_previousFilter;
	static HANDLE m_hReportFile;

	#if 1
	// Make typedefs for some IMAGEHLP.DLL functions so that we can use them
	// with GetProcAddress
	typedef BOOL (__stdcall * SYMINITIALIZEPROC)( HANDLE, LPSTR, BOOL );
	typedef BOOL (__stdcall *SYMCLEANUPPROC)( HANDLE );

	typedef BOOL (__stdcall * STACKWALKPROC)
					( DWORD, HANDLE, HANDLE, LPSTACKFRAME, LPVOID,
					PREAD_PROCESS_MEMORY_ROUTINE,PFUNCTION_TABLE_ACCESS_ROUTINE,
					PGET_MODULE_BASE_ROUTINE, PTRANSLATE_ADDRESS_ROUTINE );

	typedef LPVOID (__stdcall *SYMFUNCTIONTABLEACCESSPROC)( HANDLE, DWORD );

	typedef DWORD (__stdcall *SYMGETMODULEBASEPROC)( HANDLE, DWORD );

	typedef BOOL (__stdcall *SYMGETSYMFROMADDRPROC)
								( HANDLE, DWORD, PDWORD, PIMAGEHLP_SYMBOL );

	static SYMINITIALIZEPROC _SymInitialize;
	static SYMCLEANUPPROC _SymCleanup;
	static STACKWALKPROC _StackWalk;
	static SYMFUNCTIONTABLEACCESSPROC _SymFunctionTableAccess;
	static SYMGETMODULEBASEPROC _SymGetModuleBase;
	static SYMGETSYMFROMADDRPROC _SymGetSymFromAddr;

	#endif

};

extern MSJExceptionHandler g_MSJExceptionHandler;	//  global instance of class

#endif
