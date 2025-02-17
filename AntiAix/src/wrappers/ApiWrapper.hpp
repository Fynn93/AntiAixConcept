#pragma once

#include <string>

class ApiWrapper {
public:
    struct SurveillanceInformation
    {
        bool UnderSurveillance;
        std::string Surveillants;
        bool AllowanceNeeded;
        bool AllowanceGranted;
    };

public:
    static std::string SetSurveillanceAllowance(bool allowSurveillance);
    static std::string SetFilterInternet(bool doActivateFilter);
    static SurveillanceInformation GetSurveillanceState();
};

