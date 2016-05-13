#include "MemoryManager.h"
#include <iostream>
#include <stdlib.h>


using namespace std;

ShootMem::ShootMem() {}
ShootMem::~ShootMem() {}

//returns whether a given char* is csgo.exe
int ShootMem::isExe(WCHAR * program) {
	bool answer = (
		program[0] + program[0] == 198 &&
		program[0] + program[1] == 214 &&
		program[0] + program[2] == 202 &&
		program[0] + program[3] == 210 &&
		program[0] + program[4] == 145
		);
	if (answer)
		return 5;
	return -2;
}

//returns whether a given char* is the client.dll
int ShootMem::isClient(WCHAR * program) {
	//cout << program;
	//cout << "\t";
	bool answer = (
		program[0] + program[0] == 198 &&
		program[0] + program[1] == 207 &&
		program[0] + program[2] == 204 &&
		program[0] + program[3] == 200 &&
		program[0] + program[7] == 199
		);
	//cout << answer;
	//cout << "\n";
	if (answer)
		return 7;
	return -3;
}

//does a process walk and attaches to the csgo exe
bool ShootMem::Attach() {
	HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(entry);
	do
		if (isExe(entry.szExeFile) == 5) {
			//cout << entry.szExeFile;
			pID = entry.th32ProcessID;
			CloseHandle(handle);

			_process = OpenProcess(PROCESS_ALL_ACCESS, false, pID);


			return true;
		}
	while (Process32Next(handle, &entry));

	return false;
}

//does a dll walk and finds client.dll
DWORD ShootMem::GetModule() {
	HANDLE module = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID);
	MODULEENTRY32 mEntry;
	mEntry.dwSize = sizeof(mEntry);
	do {
		if (isClient(mEntry.szModule) > 1) {
			CloseHandle(module);
			return (DWORD)mEntry.hModule;
		}
	} while (Module32Next(module, &mEntry));
	;
	return (DWORD)false;
}