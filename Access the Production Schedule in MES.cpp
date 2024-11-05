//
//#define _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING
//#define _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS
//#include <cpprest/http_client.h>
//#include <cpprest/json.h>
//#include <iostream>
//
//using namespace web;
//using namespace web::http;
//using namespace web::http::client;
//
//void callScheduleAPI() {
//    // 定義 API 的 URI
//    uri api_uri(U("http://CNDGNMESIMQ001.delta.corp:10101/schedule/queryScheduleMo"));
//
//    // 建立 HTTP 客戶端
//    http_client client(api_uri);
//
//    // 設置請求頭
//    http_request request(methods::POST);
//    request.headers().add(U("Content-Type"), U("application/json"));
//    request.headers().add(U("tokenID"), U("107574C7FAAA0EA5E0630ECA940AF1FE"));
//
//    // 設置請求的 JSON 主體
//    json::value body;
//    body[U("orgInfo")][U("plant")] = json::value::string(U("WJ3"));
//    body[U("clientInfo")][U("user")] = json::value::string(U("55319938"));
//    body[U("clientInfo")][U("pcName")] = json::value::string(U("CNWJxxxx"));
//    body[U("clientInfo")][U("program")] = json::value::string(U("test"));
//    body[U("clientInfo")][U("programVer")] = json::value::string(U("1.2"));
//    body[U("parameters")][U("prodArea")] = json::value::string(U("WJ3_SMT"));
//    body[U("parameters")][U("lineName")] = json::value::string(U("SMT-21"));
//    body[U("parameters")][U("modelName")] = json::value::string(U(""));
//    body[U("parameters")][U("startDate")] = json::value::string(U("2024-11-04"));
//
//    request.set_body(body);
//
//    // 發送請求
//    client.request(request).then([](http_response response) {
//        if (response.headers().content_type() == U("application/json")) {
//            response.extract_json().then([](json::value jsonResponse) {
//                auto result = jsonResponse[U("result")].as_string();
//                if (result == U("FAIL")) {
//                    auto errorMsg = jsonResponse[U("errorMsg")].is_null() ? U("No error message") : jsonResponse[U("errorMsg")].as_string();
//                    std::wcout << L"API Response: FAIL - " << errorMsg << std::endl;
//                }
//                else {
//                    std::wcout << L"API Response (JSON): " << jsonResponse.serialize() << std::endl;
//                }
//                }).wait();
//        }
//        else {
//            response.extract_string().then([](utility::string_t responseString) {
//                std::wcout << L"API Response (String): " << responseString << std::endl;
//                }).wait();
//        }
//        }).then([](pplx::task<void> t) {
//            try {
//                t.get(); // 用來捕捉異常
//            }
//            catch (const std::exception& e) {
//                std::cerr << "Exception occurred: " << e.what() << std::endl;
//            }
//            }).wait();
//}
//
//int main() {
//    callScheduleAPI();
//    return 0;
//}