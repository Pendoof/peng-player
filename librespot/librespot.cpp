#include "librespot.hpp"

#include <iostream>

namespace librespot {
    PROCESS_INFORMATION pi;
    bool isRunning = false;

    bool start() {
        if (isRunning) {
            return true;
        }

        STARTUPINFOW si{};
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        wchar_t cmd[] = L"librespot.exe -n PengPlayer -j -c .";

        if (!CreateProcessW(nullptr, cmd, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
            return false;
        }

        isRunning = true;
        return true;
    }

    bool stop() {
        if (!isRunning) {
            return true;
        }

        TerminateProcess(pi.hProcess, 0);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        isRunning = false;
        return true;
    }
}