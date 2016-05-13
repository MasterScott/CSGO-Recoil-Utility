
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif

#include <windows.h>
#include <TlHelp32.h>
#include <string>
#include <Psapi.h>
#include <stdarg.h>


class ShootMem {

public:
	ShootMem();
	~ShootMem();

	bool Attach();


	DWORD GetModule();

	template <class T>
	T Read(DWORD addr) {
		T _read;
		ReadProcessMemory(_process, (LPVOID)addr, &_read, sizeof(T), NULL);
		return _read;
	}

	HANDLE _process;
	DWORD pID;

private:
	int isExe(WCHAR * program);
	int isClient(WCHAR * program);


};