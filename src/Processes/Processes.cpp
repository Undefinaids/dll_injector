//
// Created by Antoine on 6/14/2020.
//

#include "Processes.h"
#include "../Process/Process.h"

Processes::Processes() {
    //TODO: Manage errors, Get the handle, Store all the process (array or list)
};

bool Processes::fetch_processes() {
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if(hProcessSnap == INVALID_HANDLE_VALUE) {
        std::cout << "Oupsi" << std::endl;
        return false;
    }
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hProcessSnap, &pe32)) {
        std::cout << "Oupsi" << std::endl;
        CloseHandle(hProcessSnap);
        return false;
    }
    do {
        Process process(&pe32);
        std::cout << process.getName() << std::endl;
    } while(Process32Next(hProcessSnap, &pe32));
    CloseHandle(hProcessSnap);
    return true;
}

Processes::~Processes() {
    //Free the handle and all the process
}