//
// Created by Antoine on 6/14/2020.
//

#include "Process.h"

Process::Process(LPPROCESSENTRY32 lppe): _lppe(lppe) {
}

Process::~Process() {
}

const char * Process::getName() const {
    return _lppe->szExeFile;
}