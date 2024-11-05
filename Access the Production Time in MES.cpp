//
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
//
//
//void callScheduleAPI() {
//    uri api_uri(U("http://CNDGNMESIMQ001.delta.corp:10101/schedule/queryScheduleMo"));
//    http_client client(api_uri);
//
//    http_request request(methods::POST);
//    request.headers().add(U("Content-Type"), U("application/json"));
//    request.headers().add(U("tokenID"), U("107574C7FAAA0EA5E0630ECA940AF1FE"));
//
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
//    client.request(request).then([](http_response response) {
//        if (response.status_code() == status_codes::OK) {
//            response.extract_string().then([](utility::string_t responseString) {
//                // 將回應字串轉換為 JSON 格式
//                json::value jsonResponse = json::value::parse(responseString);
//
//                // 確認 responseData 是否存在並且是陣列格式
//                if (jsonResponse.has_array_field(U("responseData"))) {
//                    auto data = jsonResponse[U("responseData")].as_array();
//                    for (const auto& item : data) {
//                        if (item.has_field(U("planStartDate")) && item.has_field(U("planCloseDate"))) {
//                            auto startDate = item.at(U("planStartDate")).as_string();
//                            auto closeDate = item.at(U("planCloseDate")).as_string();
//                            std::wcout << L"planStartDate: " << startDate << L", planCloseDate: " << closeDate << std::endl;
//                        }
//                        else {
//                            std::wcerr << L"Missing planStartDate or planCloseDate in item" << std::endl;
//                        }
//                    }
//                }
//                else {
//                    std::wcerr << L"responseData array not found in JSON response" << std::endl;
//                }
//                }).wait();
//        }
//        else {
//            std::wcerr << L"Request failed with status code: " << response.status_code() << std::endl;
//        }
//        }).then([](pplx::task<void> t) {
//            try {
//                t.get();
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
//
