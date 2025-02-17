#include "ApiWrapper.hpp"

#include "src/Globals.hpp"
#include "src/utils/HttpUtils.hpp"

// TODO: Check if these functions are actually working

std::string ApiWrapper::SetSurveillanceAllowance(const bool allowSurveillance)
{
    return HttpUtils::Get("/api/SetSurveillanceAllowance?allowSurveillance=" + std::to_string(allowSurveillance));
}

std::string ApiWrapper::SetFilterInternet(const bool doActivateFilter)
{
    return HttpUtils::Get("/api/SetFilterInternet?doActivateFilter=" +
        std::to_string(doActivateFilter) +
        "&actionId=0f8fad5b-d9cb-469f-a165-70867728950e" // Just a random GUID
    );
}
ApiWrapper::SurveillanceInformation ApiWrapper::GetSurveillanceState()
{

}
