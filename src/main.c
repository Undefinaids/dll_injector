//
// Created by antoine on 16/06/2020.
//

#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>

#include <stdio.h>

unsigned long retrieve_process_id(const char *p_name) {
  HANDLE hProcessSnap;
  PROCESSENTRY32 pe32;

  hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if(hProcessSnap == INVALID_HANDLE_VALUE)
	return (0);
  pe32.dwSize = sizeof(PROCESSENTRY32);
  if (!Process32First(hProcessSnap, &pe32)) {
	CloseHandle(hProcessSnap);
	return (0);
  }
  do {
	if (strcasecmp(pe32.szExeFile, p_name) == 0) {
	  CloseHandle(hProcessSnap);
	  return (pe32.th32ProcessID);
	}
  } while(Process32Next(hProcessSnap, &pe32));
  CloseHandle(hProcessSnap);
  return (0);
}

int handle_error_int(const char *error) {
    printf("%s\n", error);
    return (0);
}

int alloc_and_inject(unsigned long pid, const char *dll) {
    HANDLE process;
    HANDLE remote_thread;
    FARPROC loadlib_addr;
    char *dll_full_path;
    void *dll_addr;
    unsigned int dll_name_len = 0;

    process = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, pid);
    if (process == INVALID_HANDLE_VALUE)
        return handle_error_int("Failed to open process");
    if ((dll_full_path = calloc(sizeof(char), MAX_PATH + 1)) == NULL)
        return handle_error_int("Failed to calloc the full path var");
    if (!(dll_name_len = GetFullPathName(TEXT(dll), MAX_PATH, dll_full_path, NULL)))
        return handle_error_int("Failed to get full path name");
    if ((dll_addr = VirtualAllocEx(process, 0, dll_name_len, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE)) == NULL)
        return handle_error_int("Failed to alloc enough space to write the library name in the victim process");
    if (WriteProcessMemory(process, dll_addr, dll_full_path, dll_name_len, NULL) == 0)
        return handle_error_int("Failed to write in the victim process memory");
    if ((loadlib_addr = GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "LoadLibraryA")) == NULL)
        return handle_error_int("Failed to get proc address of LoadLibraryA in kernel32.dll");
    if ((remote_thread = CreateRemoteThread(process, NULL, 0, (LPTHREAD_START_ROUTINE) loadlib_addr, dll_addr, 0, NULL)) == NULL)
        return handle_error_int("Failed to create remote thread");
    WaitForSingleObject(remote_thread, INFINITE);
    return (0);
}

/**
 * Dummy Main (example)
 */
int main(int ac, char **av) {
    unsigned long pid = retrieve_process_id("Battle.net.exe");
    alloc_and_inject(pid, "dummy.dll");
}
