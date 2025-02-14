#pragma once

#include <httplib.h>

#include "src/Globals.hpp"

class HttpUtils {
public:
    static std::string Get(const std::string& url) {
        httplib::Client client(Globals::g_ClientServiceUrl);
        auto res = client.Get(url);
        if (!res) {
            return {};
        }

        return res->body;
    }
};
