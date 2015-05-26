/**
 * 4PSA VoipNow Service Providers Edition SystemAPI Client for C++
 *
 * Copyright (c) 2012, Rack-Soft (www.4psa.com). All rights reserved.
 * VoipNow is a Trademark of Rack-Soft, Inc
 * 4PSA is a Registered Trademark of Rack-Soft, Inc.
 * All rights reserved.
 *
 * This demo program adds a service provider using the SystemAPI
 *
 **/

#include<sstream>
#include<string>
#include "soapAccountProxy.h"
#include "soapBillingProxy.h"
#include "soapChannelProxy.h"
#include "soapOrganizationProxy.h"
#include "soapExtensionProxy.h"
#include "soapUserProxy.h"
#include "soapGlobalOpProxy.h"
#include "soapPBXProxy.h"
#include "soapReportProxy.h"
#include "soapServiceProviderProxy.h"
#include "Organization.nsmap"
#include "stdsoap2.h"
#include "RandomChargingPlan.h"

// CHANGEME
#define COUNTRY "ro"

int main(int argc, char *argv[])
{
    /* make sure that each time you run your application, a valid access token is provided
     **/
    if (argc != 2) {
        std::cerr << "Usage: ./<executable_name> \"<access_token>\"" << std::endl;
        std::cerr << "	example: ./demoaddserviceprovider \"1|V_pmPvEm25-HrqAzERx_nvJbBvNs~q3F|1|v-gntT4GFH-UCUX0EM2_r9XTVDtw~qCF\"" << std::endl;
        exit(EXIT_FAILURE);
    }


    // We will add a new Service Provider, so we need a ServiceProvider object
    ServiceProvider *serviceProvider = new ServiceProvider;
    if (NULL == serviceProvider) {
        std::cerr << "Failed creating a ServiceProvider object" << std::endl;
        exit(EXIT_FAILURE);
    }


    // Authentication Data - passed from command line
    std::string accessToken(argv[1]);

    // filling in the header with the user credentials
    serviceProvider->soap->header = new SOAP_ENV__Header;
    if (NULL == serviceProvider->soap->header) {
        std::cerr << "Failed creating a SOAP_ENV_HEADER object" << std::endl;
        exit(EXIT_FAILURE);
    }
    serviceProvider->soap->header->ns4__serverInfo = NULL;
    serviceProvider->soap->header->ns4__userCredentials =
            new _ns4__userCredentials;
    if (NULL == serviceProvider->soap->header->ns4__userCredentials) {
        std::cerr << "Failed creating an _ns4__userCredentials object" << std::endl;
        exit(EXIT_FAILURE);
    }
    serviceProvider->soap->header->ns4__userCredentials->accessToken =
            accessToken;

    // creating 2 objects for the request and for the response
    _ns7__AddServiceProvider *request = new _ns7__AddServiceProvider;
    if (NULL == request) {
        std::cerr << "Failed creating an AddServiceProvider object" <<
                std::endl;
        exit(EXIT_FAILURE);
    }

    _ns7__AddServiceProviderResponse *response =
            new _ns7__AddServiceProviderResponse;
    if (NULL == response) {
        std::cerr << "Failed creating an AddServiceProviderResponse object" <<
                std::endl;
        exit(EXIT_FAILURE);
    }

    /* 
     * information about the new Service Provider
     * (name, login, password, country and charging plan)
     **/
    std::stringstream name_ss;
    std::stringstream login_ss;
    std::stringstream pass_ss;

    std::string name;
    std::string login;
    std::string pass;
    std::string country(COUNTRY);
    std::string chargingPlanID;

    srand(time(NULL));


    // filling in the information about the new organization (name, login, password, country and charging plan)
    name_ss << "ServiceProviderCPP_" << rand() % 1000;
    login_ss << "Admin_" << rand() % 1000;
    pass_ss << "Pass_" << rand() % 1000;

    name_ss >> name;
    login_ss >> login;
    pass_ss >> pass;

    request->name = &name;
    request->login = &login;
    request->password = &pass;
    request->country = &country;

    // get a random charging plan ID using RandomChargingPlan class

    RandomChargingPlan *rcp = new RandomChargingPlan(accessToken);
    if (NULL == rcp) {
        std::cerr << "Failed creating a RandomChargingPlan object" <<
                std::endl;
        exit(EXIT_FAILURE);
    }

    chargingPlanID = rcp->getRandomChargingPlan();
    if (chargingPlanID != NO_CHARGING_PLAN_FOUND)
        request->ns5__chargingPlanID = &chargingPlanID;

    // making the request and getting the response
    int errCode = serviceProvider->
            __ns25__AddServiceProvider(request, response);
    if (SOAP_OK == errCode) {
        // no error
        std::cout << "OK adding service provider" << std::endl;
    } else {
        // error found
        soap *s = new soap;
        if (NULL == s) {
            std::cerr << "Failed creating a soap object" <<
                    std::endl;
            exit(EXIT_FAILURE);
        }
        s->error = errCode;
        soap_print_fault(s, stderr);
        std::cerr << "Please check the log files for more information" <<
                std::endl;
    }

    return 0;
}
