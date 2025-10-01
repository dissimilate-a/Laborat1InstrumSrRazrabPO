#include <iostream>
#include <cwctype>
#include <cwchar>
#include <algorithm>
#include <string>
#include <cctype>
#include <windows.h>

using namespace std;

// Прототипы функций
int countwords(const char* str);
int countwords(const wchar_t* str);
void reversestring(char* str);
void reversestring(wchar_t* str);
bool isstringpalindrome(const char* str);
bool isstringpalindrome(const wchar_t* str);
string UnicodeToUTF8(const std::wstring& wstr);

// Функция для получения текста последней ошибки Windows и вывода с контекстом
string GetLastErrorAsString() {
    DWORD errorMessageID = ::GetLastError();
    if (errorMessageID == 0)
        return string();

    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        errorMessageID,
        MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), 
        (LPSTR)&messageBuffer,
        0, NULL);

    string message(messageBuffer, size);
    LocalFree(messageBuffer);

    // Удаляем пробелы и символы новой строки в начале и конце
    size_t start = message.find_first_not_of(" \r\n");
    size_t end = message.find_last_not_of(" \r\n");
    if (start != string::npos && end != string::npos)
        message = message.substr(start, end - start + 1);

    return message;
}

void HandleWinError(const char* context) {
    DWORD err = GetLastError();
    if (err == 0) return;
    string msg = GetLastErrorAsString();
    cerr << "ERROR in [" << context << "]: Code " << err << " - " << msg << endl;
}

// Функция установки кодировки с использованием HandleWinError
bool SetConsoleEncoding() {
    bool success = true;

    if (!SetConsoleCP(65001)) {
        HandleWinError("SetConsoleCP");
        success = false;
    }
    if (!SetConsoleOutputCP(65001)) {
        HandleWinError("SetConsoleOutputCP");
        success = false;
    }

    return success;
}

string UnicodeToUTF8(const wstring& wstr) {
    if (wstr.empty())
        return {};

    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(),
        nullptr, 0, nullptr, nullptr);
    if (size_needed == 0) {
        HandleWinError("WideCharToMultiByte get size");
        return {};
    }
    string strTo(size_needed, 0);
    int result = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(),
        &strTo[0], size_needed, nullptr, nullptr);
    if (result == 0) {
        HandleWinError("WideCharToMultiByte convert");
        return {};
    }
    return strTo;
}


int main() {
    // Настройка кодировки консоли
    SetConsoleCP(65001); // Входная кодировка консоли (UTF-8)
    SetConsoleOutputCP(65001); // Выходная кодировка консоли (UTF-8)

    // Устанавливаем локаль UTF-8 для потоков
    std::locale utf8_locale(".UTF-8");
    std::cout.imbue(utf8_locale);
    std::wcout.imbue(utf8_locale);


    // ===== ДЕМОНСТРАЦИЯ ОБРАБОТКИ И ВЫВОДА ОШИБОК =====
    cout << "DEMONSTRATION OF ERROR PROCESSING AND OUTPUT\n" << endl;

    if (!SetConsoleCP(123456)) { // Некорректный код страницы -> ошибка
        HandleWinError("Forced SetConsoleCP error");
    }

    // ===== ДЕМОНСТРАЦИЯ ANSI-ФУНКЦИЙ =====
    cout << "\n\nANSI-FUNCTIONS\n" << endl;

    // Используем ASCII-строку для демонстрации ANSI
    char ansi_str[] = "Hello, world! This is a test.";
    cout << "Original line: " << ansi_str << endl;
    cout << "Number of words: " << countwords(ansi_str) << endl;

    reversestring(ansi_str);
    cout << "The inverted line: " << ansi_str << endl;

    // Для кириллицы используем Unicode
    const wchar_t* palindrom_ansi_str = L"No x in Nixon";
    wcout << L"Checking the palindrome: \"" << palindrom_ansi_str << L"\" - ";
    wcout << (isstringpalindrome(palindrom_ansi_str) ? L"PALINDROME" : L"NOT A PALINDROME") << endl;

    // ===== ДЕМОНСТРАЦИЯ UNICODE-ФУНКЦИЙ =====
    cout << "\n\nUNICODE-FUNCTIONS\n" << endl;
    wchar_t unicode_str[] = L"Hello, world! This is a test.";
    wcout << L"Original line: " << unicode_str << endl;
    cout << "Number of words: " << countwords(unicode_str) << endl;

    reversestring(unicode_str);
    wcout << L"The inverted line: " << unicode_str << endl;

    const wchar_t* palindrom_uni_str = L"No x in Nixon";
    wcout << L"Checking the palindrome: \"" << palindrom_uni_str << L"\" - ";
    wcout << (isstringpalindrome(palindrom_uni_str) ? L"PALINDROME" : L"NOT A PALINDROME") << endl;

    // ===== ДЕМОНСТРАЦИЯ ПРЕОБРАЗОВАНИЯ КОДИРОВОК =====
    cout << "\n\nDEMONSTRATION OF CODE CONVERSION\n" << endl;
    wstring unicode_convert_str = L"This is a Unicode string.";
    wcout << L"The original Unicode string: " << unicode_convert_str << endl;

    // Преобразование в UTF-8
    string utf8_str = UnicodeToUTF8(unicode_convert_str);
    if (!utf8_str.empty()) {
        cout << "Converted UTF-8 string: " << utf8_str << endl;
    }
    else {
        cout << "Error converting to UTF-8" << endl;
    }

    
    return 0;
}


// ----- Функции подсчета количества слов в строке -----
// ANSI-версия
int countwords(const char* str) {
    if (!str || *str == '\0') return 0;
    int count = 0;
    bool in_word = false;
    for (size_t i = 0; str[i] != '\0'; ++i) {
        // Приведение к unsigned char для безопасных вызовов is* функций
        if (isspace((unsigned char)str[i]) || ispunct((unsigned char)str[i])) {
            if (in_word) {
                count++;
                in_word = false;
            }
        }
        else {
            in_word = true;
        }
    }
    if (in_word) count++;
    return count;
}
// UNICODE-версия
int countwords(const wchar_t* str) {
    if (!str || *str == L'\0') return 0;
    int count = 0;
    bool in_word = false;
    for (size_t i = 0; str[i] != L'\0'; ++i) {
        if (iswspace(str[i]) || iswpunct(str[i])) {  // заменено на iswpunct для wchar_t
            if (in_word) {
                count++;
                in_word = false;
            }
        }
        else {
            in_word = true;
        }
    }
    if (in_word) count++;
    return count;
}




// ----- Функции для изменения порядка символов на противоположный -----
// ANSI-версия
void reversestring(char* str) {
    if (!str || *str == '\0') return;
    reverse(str, str + strlen(str));
}
// Unicode-версия
void reversestring(wchar_t* str) {
    if (!str || *str == L'\0') return;
    reverse(str, str + wcslen(str));
}



// ----- Функции для проверки является ли строка палиндромом -----
// ANSI-версия
bool isstringpalindrome(const char* str) {
    if (!str || *str == '\0') return false;
    string cleaned;
    size_t length = strlen(str);
    for (size_t i = 0; i < length; ++i) {
        unsigned char c = (unsigned char)str[i];
        if (isalnum(c)) {
            cleaned += tolower(c);
        }
    }
    return equal(
        cleaned.begin(),
        cleaned.begin() + cleaned.size() / 2,
        cleaned.rbegin()
    );
}

// Unicode-версия
bool isstringpalindrome(const wchar_t* str) {
    if (!str || *str == L'\0') return false;

    // Оставляем только алфавитно-цифровые символы в нижнем регистре
    wstring cleaned;
    size_t length = wcslen(str);
    for (size_t i = 0; i < length; ++i) {
        wchar_t c = str[i];
        if (iswalnum(c)) {
            cleaned += towlower(c);
        }
    }
    // Сравниваем символы с начала и с конца
    size_t left = 0;
    size_t right = cleaned.size();
    if (right == 0) return false; // пустая строка не палиндром
    right--;

    while (left < right) {
        if (cleaned[left] != cleaned[right]) {
            return false;
        }
        left++;
        right--;
    }
    return true;
}