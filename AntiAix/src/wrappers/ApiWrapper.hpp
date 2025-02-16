#pragma once

#include <string>

class ApiWrapper {
public:
    static std::string SetSurveillanceAllowance(bool allowSurveillance);
    static std::string SetFilterInternet(bool doActivateFilter);
};

