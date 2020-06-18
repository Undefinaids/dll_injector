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

int handle_error_int(const char *error, int ret) {
    printf("%s\n", error);
    return (ret);
}

int alloc_and_inject(unsigned long pid, const char *dll) {
    HANDLE process;
    HANDLE dll_fd;
    char *dll_full_path;
    void *dll_addr;
    unsigned long file_size = 0;
    unsigned long byte_read = 0;
    void *local_dll_buffer;

    process = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, pid);
    if (process == INVALID_HANDLE_VALUE)
        return handle_error_int("Failed to open process", 0);
    if ((dll_full_path = calloc(sizeof(char), MAX_PATH + 1)) == NULL)
        return handle_error_int("Failed to calloc the full path var", 0);
    if (!GetFullPathName(TEXT(dll), MAX_PATH, dll_full_path, NULL))
        return handle_error_int("Failed to get full path name", 0);
    dll_fd = CreateFileA(dll_full_path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (dll_fd == INVALID_HANDLE_VALUE)
        return handle_error_int("Failed to open dll file", 0);
    if ((file_size = GetFileSize(dll_fd, NULL)) == INVALID_FILE_SIZE)
        return handle_error_int("Invalid file size", 0);
    printf("File size: %ld\n", file_size);
    if ((dll_addr = VirtualAllocEx(process, NULL, file_size, MEM_RESERVE|MEM_COMMIT, PAGE_EXECUTE_READWRITE)) == NULL)
        return handle_error_int("Failed to virtualAlloc the victim process", 0);
    // Retrieve the starting point address
    if ((local_dll_buffer = HeapAlloc(GetProcessHeap(), 0, file_size)) == NULL)
        return handle_error_int("Failed to alloc the Heap", 0);
    if ((ReadFile(dll_fd, local_dll_buffer, file_size, &byte_read, NULL)) == FALSE)
        return handle_error_int("Failed to read dll file", 0);
    // https://github.com/stephenfewer/ReflectiveDLLInjection/blob/master/inject/src/LoadLibraryR.c
    // Think about using his method to get the reflective loader offset
    // GetReflectiveLoaderOffset()
    // WIP
}

/**
 * Avoid the call to loadlibrary /!\ (use reflective injection)
 */
int main(int ac, char **av) {
    unsigned long pid = retrieve_process_id("dll_injector.exe");
    alloc_and_inject(pid, "dummy.dll");

}
