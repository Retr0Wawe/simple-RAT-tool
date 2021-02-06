#pragma once
#include <windows.h>
#include <Tlhelp32.h>
#include <psapi.h>
#include <iostream>

namespace Win_Api
{
	class Procces {
		PROCESSENTRY32 pe{ 0 };
		MODULEENTRY32 modEntry{ 0 };
	public:
		char* Pwd()
		{
			char pwd[MAX_PATH];

			int pwd_dir = GetCurrentDirectory(MAX_PATH, pwd);

			if (!pwd_dir) {
				std::cout << "Failed to get the current directory: " << GetLastError() << std::endl;
				exit(1);
			}
			return pwd;
		}

		char* Get_Procc_Directory(const char* name)
		{
			char buf[MAX_PATH];

			int pid = Find_Proc_Id(name);
			HANDLE hprocess = OpenProcess(PROCESS_ALL_ACCESS, false, pid);

			if (!hprocess) {
				std::cout << "Error getting the current directory of the process : " << std::endl;
				exit(1);
			}
			GetModuleFileNameEx(hprocess, 0, buf, MAX_PATH);
			CloseHandle(hprocess);
			return buf;
		}

		void Get_Processes()
		{
			HANDLE hproc = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			pe.dwSize = sizeof(PROCESSENTRY32);

			if (!hproc) {
				std::cout << "Can't get handle of snapshot process!" << std::endl;
				return;
			}

			do {
				std::cout << pe.szExeFile << '\t' << pe.th32ProcessID << '\t' << pe.cntThreads << std::endl;
			} while (Process32Next(hproc, &pe));

			CloseHandle(hproc);
		}

		bool Exit_Proc(const char* name)
		{
			HANDLE hprocess = OpenProcess(PROCESS_TERMINATE, false, Find_Proc_Id(name));

			if (!hprocess) {
				return false;
			}

			BOOL result = TerminateProcess(hprocess, 0);
			return result;
		}

		DWORD Find_Proc_Id(const char* name)
		{
			DWORD dwResult = 0;
			HANDLE hproc = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			pe.dwSize = sizeof(PROCESSENTRY32);

			if (!hproc) {
				std::cout << "Can't get handle of snapshot process!" << std::endl;
				return 1;
			}

			if (Process32First(hproc, &pe)) {
				do {
					if (!strcmp(pe.szExeFile, name)) {
						dwResult = pe.th32ProcessID;
					}
				} while (Process32Next(hproc, &pe));
			}
			CloseHandle(hproc);
			return dwResult;
		}

		DWORD Get_Module_Base_Address(const char* modName, DWORD procId)
		{
			DWORD modBaseAddr = 0;
			HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, procId);
			modEntry.dwSize = sizeof(modEntry);

			if (hSnap != INVALID_HANDLE_VALUE)
			{
				MODULEENTRY32 modEntry;
				modEntry.dwSize = sizeof(modEntry);
				if (Module32First(hSnap, &modEntry))
				{
					do
					{
						if (!strcmp(modEntry.szModule, modName))
						{
							modBaseAddr = DWORD(modEntry.modBaseAddr);
							break;
						}
					} while (Module32Next(hSnap, &modEntry));
				}
			}
			CloseHandle(hSnap);
			return modBaseAddr;
		}
	};
}