//#define _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING
//#define _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS
//#include <windows.h>
//#include <iostream>
//#include <fstream>
//#include <sstream>
//#include <string>
//#include <vector>
//#include <thread>
//#include <chrono>
//#include <regex>
//#include <cpprest/json.h>
//
//using namespace web;
//using namespace std;
//
//struct Schedule {
//    int year;
//    int month;
//    int day;
//    int hour;
//    int minute;
//    int second;
//    int action; // 1: 開啟, 0: 關閉
//    std::wstring planStartDate;
//    std::wstring planCloseDate;
//};
//
//std::vector<Schedule> scheduleList;
//HANDLE hSerial;
//std::atomic<bool> isScheduleRunning{ false };
//
// Function prototypes
//void OpenBreaker();
//void CloseBreaker();
//
//bool InitSerialPort() {
//    hSerial = CreateFile(L"COM6", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
//    if (hSerial == INVALID_HANDLE_VALUE) {
//        MessageBox(NULL, L"串口開啟失敗", L"錯誤", MB_OK | MB_ICONERROR);
//        return false;
//    }
//
//     設置串口參數
//    DCB dcbSerialParams = { 0 };
//    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
//    if (!GetCommState(hSerial, &dcbSerialParams)) {
//        MessageBox(NULL, L"無法獲取串口狀態", L"錯誤", MB_OK | MB_ICONERROR);
//        CloseHandle(hSerial);
//        hSerial = NULL;
//        return false;
//    }
//
//    dcbSerialParams.BaudRate = CBR_9600;
//    dcbSerialParams.ByteSize = 8;
//    dcbSerialParams.StopBits = ONESTOPBIT;
//    dcbSerialParams.Parity = NOPARITY;
//
//    if (!SetCommState(hSerial, &dcbSerialParams)) {
//        MessageBox(NULL, L"無法設定串口參數", L"錯誤", MB_OK | MB_ICONERROR);
//        CloseHandle(hSerial);
//        hSerial = NULL;
//        return false;
//    }
//
//     設置超時參數
//    COMMTIMEOUTS timeouts = { 0 };
//    timeouts.ReadIntervalTimeout = 50;
//    timeouts.ReadTotalTimeoutConstant = 50;
//    timeouts.ReadTotalTimeoutMultiplier = 10;
//    timeouts.WriteTotalTimeoutConstant = 50;
//    timeouts.WriteTotalTimeoutMultiplier = 10;
//
//    if (!SetCommTimeouts(hSerial, &timeouts)) {
//        MessageBox(NULL, L"無法設定超時", L"錯誤", MB_OK | MB_ICONERROR);
//        CloseHandle(hSerial);
//        hSerial = NULL;
//        return false;
//    }
//
//     清理緩衝區
//    PurgeComm(hSerial, PURGE_RXCLEAR | PURGE_TXCLEAR);
//    return true;
//}
//
//void ReadScheduleFromFile(const std::wstring& filePath) {
//    std::wcout << L"Reading schedule from file: " << filePath << std::endl;
//     清空舊的排程資料
//    scheduleList.clear();
//
//     讀取文件
//    std::wifstream file(filePath);
//    if (!file.is_open()) {
//        std::wcerr << L"無法打開文件: " << filePath << std::endl;
//        return;
//    }
//
//    std::wstringstream wss;
//    wss << file.rdbuf();
//    file.close();
//
//     剖析文件內容
//    json::value jsonResponse = json::value::parse(wss.str());
//
//    if (jsonResponse.has_array_field(U("responseData"))) {
//        auto data = jsonResponse[U("responseData")].as_array();
//
//        for (const auto& item : data) {
//            if (item.has_field(U("planStartDate")) && item.has_field(U("planCloseDate"))) {
//                Schedule schedule;
//                schedule.planStartDate = item.at(U("planStartDate")).as_string();
//                schedule.planCloseDate = item.at(U("planCloseDate")).as_string();
//
//                 剖析日期和時間
//                std::wregex timeRegex(L"(\\d+)-(\\d+)-(\\d+)T(\\d+):(\\d+):(\\d+)");
//                std::wsmatch match;
//                if (std::regex_search(schedule.planStartDate, match, timeRegex) && match.size() == 7) {
//                    schedule.year = std::stoi(match[1].str());
//                    schedule.month = std::stoi(match[2].str());
//                    schedule.day = std::stoi(match[3].str());
//                    schedule.hour = std::stoi(match[4].str());
//                    schedule.minute = std::stoi(match[5].str());
//                    schedule.second = std::stoi(match[6].str());
//                    schedule.action = 1; // 開啟
//                    scheduleList.push_back(schedule);
//                    std::wcout << L"Loaded schedule - Start: " << schedule.planStartDate << L", Action: " << (schedule.action == 1 ? L"Open" : L"Close") << std::endl;
//                }
//                if (std::regex_search(schedule.planCloseDate, match, timeRegex) && match.size() == 7) {
//                    Schedule closeSchedule = schedule;
//                    closeSchedule.hour = std::stoi(match[4].str());
//                    closeSchedule.minute = std::stoi(match[5].str());
//                    closeSchedule.second = std::stoi(match[6].str());
//                    closeSchedule.action = 0; // 關閉
//                    scheduleList.push_back(closeSchedule);
//                    std::wcout << L"Loaded schedule - Close: " << closeSchedule.planCloseDate << L", Action: Close" << std::endl;
//                }
//            }
//        }
//    }
//
//     排序 scheduleList，依照年、月、日、時、分、秒進行升序
//    std::sort(scheduleList.begin(), scheduleList.end(), [](const Schedule& a, const Schedule& b) {
//        if (a.year != b.year) return a.year < b.year;
//        if (a.month != b.month) return a.month < b.month;
//        if (a.day != b.day) return a.day < b.day;
//        if (a.hour != b.hour) return a.hour < b.hour;
//        if (a.minute != b.minute) return a.minute < b.minute;
//        return a.second < b.second;
//        });
//}
//
//
//void RunScheduleFromFile() {
//    while (true) {
//        if (isScheduleRunning) {
//            std::wcout << L"Schedule running..." << std::endl;
//             獲取當前時間
//            time_t now = time(0);
//            tm localTime;
//            localtime_s(&localTime, &now);
//
//            for (const auto& schedule : scheduleList) {
//                 檢查是否符合年月日時分秒條件
//                if (localTime.tm_year + 1900 == schedule.year &&
//                    localTime.tm_mon + 1 == schedule.month &&
//                    localTime.tm_mday == schedule.day &&
//                    localTime.tm_hour == schedule.hour &&
//                    localTime.tm_min == schedule.minute &&
//                    localTime.tm_sec == schedule.second) {
//
//                     根據動作開啟或關閉
//                    if (schedule.action == 1) {
//                        OpenBreaker();
//                        std::wcout << L"Executing action - Open at: " << localTime.tm_hour << L":" << localTime.tm_min << L":" << localTime.tm_sec << std::endl;
//                    }
//                    else {
//                        CloseBreaker();
//                        std::wcout << L"Executing action - Close at: " << localTime.tm_hour << L":" << localTime.tm_min << L":" << localTime.tm_sec << std::endl;
//                    }
//                    std::this_thread::sleep_for(std::chrono::seconds(1));
//                    break;
//                }
//            }
//        }
//         每秒檢查一次
//        std::this_thread::sleep_for(std::chrono::seconds(1));
//    }
//}
//
//
// 開啟/關閉空開的實際操作
//void OpenBreaker() {
//    char openCommand[] = { 0x06, 0x06, 0x00, 0x31, 0x55, 0x88, 0xE6, 0x84 };
//    DWORD bytesWritten;
//    if (!WriteFile(hSerial, openCommand, sizeof(openCommand), &bytesWritten, NULL)) {
//        std::cerr << "Failed to open breaker." << std::endl;
//    }
//}
//
//void CloseBreaker() {
//    char closeCommand[] = { 0x06, 0x06, 0x00, 0x31, 0x55, 0x66, 0x66, 0xC8 };
//    DWORD bytesWritten;
//    if (!WriteFile(hSerial, closeCommand, sizeof(closeCommand), &bytesWritten, NULL)) {
//        std::cerr << "Failed to close breaker." << std::endl;
//    }
//}
//
//int main() {
//    /*if (!InitSerialPort()) {
//        return 1;
//    }*/
//
//     讀取檔案中的排程資料
//    ReadScheduleFromFile(L"D:\\PRODUCTION_SCHEDULE_SAMPLE\\testing_schedule_json_format.txt");
//
//     啟動排程的執行線程
//    std::thread scheduleThread(RunScheduleFromFile);
//
//     分離執行線程
//    scheduleThread.detach();
//
//     使用無限循環來保持程式執行
//    while (true) {
//        std::this_thread::sleep_for(std::chrono::seconds(1));
//    }
//
//    return 0;
//}


#define _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING
#define _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <algorithm>
#include <ctime>
#include <fstream>
#include <cpprest/json.h>
#include <thread>

using namespace web;

struct Schedule {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    int action; // 1: 開啟, 0: 關閉
    std::wstring planStartDate;
    std::wstring planCloseDate;
};

std::vector<Schedule> scheduleList;

// 讀取排程文件
void ReadScheduleFromFile(const std::wstring& filePath) {
    scheduleList.clear();
    std::wifstream file(filePath);
    if (!file.is_open()) {
        std::wcerr << L"無法打開文件: " << filePath << std::endl;
        return;
    }

    std::wstringstream wss;
    wss << file.rdbuf();
    file.close();

    json::value jsonResponse = json::value::parse(wss.str());
    if (jsonResponse.has_array_field(U("responseData"))) {
        auto data = jsonResponse[U("responseData")].as_array();

        for (const auto& item : data) {
            if (item.has_field(U("planStartDate")) && item.has_field(U("planCloseDate"))) {
                Schedule schedule;
                schedule.planStartDate = item.at(U("planStartDate")).as_string();
                schedule.planCloseDate = item.at(U("planCloseDate")).as_string();

                // 解析日期和時間
                std::wregex timeRegex(L"(\\d+)-(\\d+)-(\\d+)T(\\d+):(\\d+):(\\d+)");
                std::wsmatch match;
                if (std::regex_search(schedule.planStartDate, match, timeRegex) && match.size() == 7) {
                    schedule.year = std::stoi(match[1].str());
                    schedule.month = std::stoi(match[2].str());
                    schedule.day = std::stoi(match[3].str());
                    schedule.hour = std::stoi(match[4].str());
                    schedule.minute = std::stoi(match[5].str());
                    schedule.second = std::stoi(match[6].str());
                    schedule.action = 1; // 開啟
                    scheduleList.push_back(schedule);
                }
                if (std::regex_search(schedule.planCloseDate, match, timeRegex) && match.size() == 7) {
                    Schedule closeSchedule = schedule;
                    closeSchedule.hour = std::stoi(match[4].str());
                    closeSchedule.minute = std::stoi(match[5].str());
                    closeSchedule.second = std::stoi(match[6].str());
                    closeSchedule.action = 0; // 關閉
                    scheduleList.push_back(closeSchedule);
                }
            }
        }
    }

    // 排序 scheduleList
    std::sort(scheduleList.begin(), scheduleList.end(), [](const Schedule& a, const Schedule& b) {
        if (a.year != b.year) return a.year < b.year;
        if (a.month != b.month) return a.month < b.month;
        if (a.day != b.day) return a.day < b.day;
        if (a.hour != b.hour) return a.hour < b.hour;
        if (a.minute != b.minute) return a.minute < b.minute;
        return a.second < b.second;
        });

    // 印出讀取的生產計劃時間
    for (const auto& schedule : scheduleList) {
        std::wcout << L"讀取到的排程: " << (schedule.action == 1 ? L"開啟" : L"關閉") << L" 時間: " << schedule.planStartDate << L" / " << schedule.planCloseDate << std::endl;
    }
}

// 執行排程的機制
void RunSchedule() {
    while (true) {
        time_t now = time(0);
        tm localTime;
        localtime_s(&localTime, &now);

        for (const auto& schedule : scheduleList) {
            if (localTime.tm_year + 1900 == schedule.year &&
                localTime.tm_mon + 1 == schedule.month &&
                localTime.tm_mday == schedule.day &&
                localTime.tm_hour == schedule.hour &&
                localTime.tm_min == schedule.minute &&
                localTime.tm_sec == schedule.second) {
                if (schedule.action == 1) {
                    std::wcout << L"開啟新的任務: " << schedule.planStartDate << std::endl;
                }
                else {
                    std::wcout << L"關閉任務: " << schedule.planCloseDate << std::endl;
                }
                std::this_thread::sleep_for(std::chrono::seconds(1));
                break;
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

// 主程序
int main() {
    // 讀取檔案中的排程資料
    ReadScheduleFromFile(L"D:\\PRODUCTION_SCHEDULE_SAMPLE\\testing_schedule_json_format.txt");

    // 執行排程機制
    RunSchedule();

    return 0;
}
