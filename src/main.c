//
// Created by antoine on 16/06/2020.
//

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
	if (strcasecmp(pe32->szExeFile, p_name) == 0) {
	  CloseHandle(hProcessSnap);
	  return (pe32->th32ProcessID);
	}
  } while(Process32Next(hProcessSnap, &pe32));
  CloseHandle(hProcessSnap);
  return (0);
}

int main(int ac, char **av) {

}
