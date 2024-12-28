# Resume a process fromt the command line

#include <windows.h>
#include <stdio.h>
#include <ntstatus.h>

typedef NTSTATUS(NTAPI* pNtResumeProcess)(IN HANDLE ProcessHandle);

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		printf("[+] Usage: r.exe <pid> to resume the suspended process using that pid\n");
		return 1;
	}

	int pid = atoi(argv[1]);

	HANDLE hTProcess = NULL;
	hTProcess = OpenProcess(PROCESS_SUSPEND_RESUME, TRUE, pid);
	if (hTProcess == NULL)
	{
		printf("Error opening target process: %d\n", GetLastError());
		return 1;
	}

	HMODULE hNtDll = GetModuleHandleW(L"ntdll.dll");
	if (hNtDll == NULL)
	{
		printf("Error getting a handle to ntdll: %d\n", GetLastError());
		return 1;
	}
	pNtResumeProcess NtResumeProcess = (pNtResumeProcess)GetProcAddress(hNtDll, "NtResumeProcess");
	
	if (NtResumeProcess(hTProcess) != STATUS_SUCCESS)
	{
		printf("[x] Error resuming process. Check if you are running as admin.\n");
	}
	printf("[+] Successfully resumed pid %d\n", pid);

	CloseHandle(hTProcess);

	return 0;
}	