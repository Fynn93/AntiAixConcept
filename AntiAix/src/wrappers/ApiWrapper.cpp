#include "ApiWrapper.hpp"

#include "src/Globals.hpp"
#include "src/utils/HttpUtils.hpp"

// TODO: Check if these functions are actually working

void ApiWrapper::SetSurveillanceAllowance(const bool allowSurveillance) {
    HttpUtils::Get("api/SetSurveillanceAllowance?allowSurveillance=" + std::to_string(allowSurveillance));
}

void ApiWrapper::SetFilterInternet(const bool doActivateFilter) {
    HttpUtils::Get("api/SetFilterInternet?doActivateFilter=" +
        std::to_string(doActivateFilter) +
        "&actionId=0f8fad5b-d9cb-469f-a165-70867728950e"); // Just a random GUID
}