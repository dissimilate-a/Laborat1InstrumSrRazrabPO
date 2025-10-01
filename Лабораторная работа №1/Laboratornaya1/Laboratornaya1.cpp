// Laboratornaya1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <windows.h>
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

//int main()
//{
//    setlocale(0, "Russian");
//    ULONGLONG ticks = GetTickCount64(); // Возвращает количество миллисекунд
//    ULONGLONG remainsSeconds = ticks / 1000;
//
//    ULONGLONG days = remainsSeconds / (24 * 3600);
//    remainsSeconds %= (24 * 3600);
//
//    ULONGLONG hours = remainsSeconds / 3600;
//    remainsSeconds %= 3600;
//
//    ULONGLONG minutes = remainsSeconds / 60;
//    ULONGLONG seconds = remainsSeconds % 60;
//
//    cout << "Время работы ОС: " << days << " дней, " << hours << " часов, " << minutes << " минут, " << seconds << " секунд" << endl;
//    return 0;
//}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Получаем текущее системное время
    SYSTEMTIME st;
    GetLocalTime(&st);

    // Буфер для форматированной строки с датой и временем
    wchar_t timeBuffer[256];
    swprintf_s(timeBuffer, 256,
        L"Текущая дата и время:\n\n"
        L"Дата: %02d.%02d.%04d\n"
        L"Время: %02d:%02d:%02d\n",
        st.wDay, st.wMonth, st.wYear,
        st.wHour, st.wMinute, st.wSecond);

    // Показываем MessageBox с текущим временем
    MessageBoxW(NULL, timeBuffer, L"Системное время", MB_OK | MB_ICONINFORMATION);

    return 0;
}
