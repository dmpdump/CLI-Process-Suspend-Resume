//Process Suspend

#include <windows.h>
#include <stdio.h>
#include <ntstatus.h>

typedef NTSTATUS(NTAPI* pNtSuspendProcess)(IN HANDLE ProcessHandle);

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		printf("[+] Usage: s.exe <pid> to suspend the process using that pid\n");
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
	pNtSuspendProcess NtSuspendProcess = (pNtSuspendProcess)GetProcAddress(hNtDll, "NtSuspendProcess");
	
	if (NtSuspendProcess(hTProcess) != STATUS_SUCCESS)
	{
		printf("[x] Error suspending process. Check if you are running as admin.\n");
	}
	printf("[+] Successfully suspended pid %d\n", pid);

	CloseHandle(hTProcess);

	return 0;
}	


