//
// Created by Antoine on 6/14/2020.
//

#include "Processes.h"
#include "../Process/Process.h"

Processes::Processes() {
    //TODO: Manage errors, Get the handle, Store all the process
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;

    hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hProcessSnap, &pe32)) {
        std::cout << "Oupsi" << std::endl;
        CloseHandle(hProcessSnap);
    }
    do {
        Process process(&pe32);
        std::cout << process.getName() << std::endl;

    } while(Process32Next(hProcessSnap, &pe32));
    CloseHandle(hProcessSnap);
};

Processes::~Processes() {
    //Free the handle and all the process
}