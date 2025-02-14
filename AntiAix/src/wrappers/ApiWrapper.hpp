#pragma once

#include <string>

class ApiWrapper {
public:
    static void SetSurveillanceAllowance(bool allowSurveillance);
    static void SetFilterInternet(bool doActivateFilter);
};

