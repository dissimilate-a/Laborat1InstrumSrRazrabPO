#include <windows.h>
#include <wchar.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Текущее время
    SYSTEMTIME st;
    GetLocalTime(&st);

    // Время работы системы
    ULONGLONG ticks = GetTickCount64();
    ULONGLONG totalSeconds = ticks / 1000;

    ULONGLONG days = totalSeconds / (24 * 3600);
    totalSeconds %= (24 * 3600);

    ULONGLONG hours = totalSeconds / 3600;
    totalSeconds %= 3600;

    ULONGLONG minutes = totalSeconds / 60;
    ULONGLONG seconds = totalSeconds % 60;

    // Компьютер
    wchar_t computerName[256]; // Буфер для имени компьютера(Unicode)
    DWORD compNameSize = 256; // Переменная для хранения размера буфера
    GetComputerNameW(computerName, &compNameSize);

    // Имя пользователя
    wchar_t userName[256];
    DWORD userNameSize = 256;
    GetUserNameW(userName, &userNameSize);


    wchar_t message[512]; // Буфер для итогового текста
    swprintf_s(message, 512,
        L"Текущая дата и время:\n"
        L"%02d.%02d.%04d  %02d:%02d:%02d\n\n"
        L"Время работы системы:\n"
        L"%llu дней, %02llu часов, %02llu минут, %02llu секунд\n\n"
        L"Компьютер: %s\n"
        L"Имя пользователя: %s",
        st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond, days, hours, minutes, seconds, computerName, userName);

    MessageBoxW(NULL, message, L"Информация о времени и пользователе", MB_OK | MB_ICONINFORMATION);
    return 0;
}

