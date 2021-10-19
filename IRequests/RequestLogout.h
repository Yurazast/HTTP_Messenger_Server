#pragma once
#include <cpprest/json.h>
using namespace web;

class RequestLogout : public IRequests {
private:
    std::string user_access_token;
public:
    RequestLogout(IDatabase* db, const std::string& userAccessToken);
    void DoRequest();
};
