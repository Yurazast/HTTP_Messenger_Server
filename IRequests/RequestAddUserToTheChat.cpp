#pragma once
#include "IRequests.h"
#include "RequestAddUserToTheChat.h"
#include <cpprest/json.h>
#include "../stringtowstring.h"
using namespace web;

RequestAddUserToTheChat::RequestAddUserToTheChat(IDatabase* db, const std::string& userAccessToken, const unsigned long& chatId, const std::string& userLogin) : IRequests(db),
user_access_token(userAccessToken),
chat_id(chatId),user_login(userLogin) {}

void RequestAddUserToTheChat::DoRequest() {
    json::value result;
    try {
        if (this->db->AddUserToChat(this->user_access_token, this->user_login,this->chat_id)) {
            result[L"status"] = json::value::string(L"OK");
        }
        else {
            result[L"what"] = json::value::string(to_wstring("Cannot join to this chat"));
            this->answercontainer->SetStatusCode(status_codes::Forbidden);
        }
    }
    catch (const QueryException& e) {
        result[L"what"] = json::value::string(to_wstring("No such user"));
        this->answercontainer->SetStatusCode(status_codes::Unauthorized);
    }
    catch (const std::exception& e) {
        result[L"what"] = json::value::string(to_wstring("Database error"));
        this->answercontainer->SetStatusCode(status_codes::InternalError);
    }
    this->answercontainer->SetAnswer(result);
    this->answercontainer->MakeDone();
}