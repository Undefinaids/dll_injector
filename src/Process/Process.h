//
// Created by Antoine on 6/14/2020.
//

#ifndef DLL_INJECTOR_PROCESS_H
#define DLL_INJECTOR_PROCESS_H

#include <windows.h>
#include <tlhelp32.h>

class Process {
public:
    Process(LPPROCESSENTRY32 lppe);
    ~Process();
    const char *getName() const;
private:
    LPPROCESSENTRY32 _lppe;
};


#endif //DLL_INJECTOR_PROCESS_H
