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
#define COMPANY "TEST COMPANY"

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
    ServiceProviderProxy *serviceProvider = new ServiceProviderProxy;
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
    serviceProvider->soap->header->ns3__serverInfo = NULL;
    serviceProvider->soap->header->ns3__userCredentials =
            new _ns3__userCredentials;

    if (NULL == serviceProvider->soap->header->ns3__userCredentials) {
        std::cerr << "Failed creating an _ns3__userCredentials object" << std::endl;
        exit(EXIT_FAILURE);
    }
    serviceProvider->soap->header->ns3__userCredentials->accessToken =
            accessToken;

    // creating 2 objects for the request and for the response
    _ns6__AddServiceProvider *request = new _ns6__AddServiceProvider;
    if (NULL == request) {
        std::cerr << "Failed creating an _ns6__AddServiceProvider object" << std::endl;
        exit(EXIT_FAILURE);
    }

    _ns6__AddServiceProviderResponse response;

    /* 
     * information about the new Service Provider
     * (name, login, password, country and charging plan)
     **/
    std::stringstream name_ss;
    std::stringstream login_ss;
    std::stringstream firstname_ss;
    std::stringstream lastname_ss;
    std::stringstream email_ss;

    std::string name;
    std::string login;
    std::string firstname;
    std::string lastname;
    std::string email;
    std::string country(COUNTRY);
    std::string company(COMPANY);
    std::string chargingPlanID;
    bool passAuto = true;

    srand(time(NULL));


    // filling in the information about the new organization (name, login, password, country and charging plan)
    name_ss << "ServiceProviderCPP_" << rand() % 1000;
    login_ss << "Admin_" << rand() % 1000;
    firstname_ss << "FirstnameCPP_" << rand() % 1000;
    lastname_ss << "LastnameCPP_" << rand() % 1000;
    email_ss << "Email_" << rand() % 1000 << "@example.com";

    name_ss >> name;
    login_ss >> login;
    firstname_ss >> firstname;
    lastname_ss >> lastname;
    email_ss >> email;

    request->__AddServiceProvider_sequence = new __ns6__AddServiceProvider_sequence;
    if (NULL == request->__AddServiceProvider_sequence) {
        std::cerr << "Failed creating an __AddServiceProvider_sequence object" << std::endl;
        exit(EXIT_FAILURE);
    }

    request->__AddServiceProvider_sequence->ns8__name = &name;
    request->__AddServiceProvider_sequence->ns8__login = &login;
    request->__AddServiceProvider_sequence->ns8__firstName = &firstname;
    request->__AddServiceProvider_sequence->ns8__lastName = &lastname;
    request->__AddServiceProvider_sequence->ns8__email = &email;
    request->__AddServiceProvider_sequence->ns8__passwordAuto = &passAuto;
    request->__AddServiceProvider_sequence->ns8__country = &country;
    request->__AddServiceProvider_sequence->ns8__company = &company;


    // get a random charging plan ID using RandomChargingPlan class

    RandomChargingPlan *rcp = new RandomChargingPlan(accessToken);
    if (NULL == rcp) {
        std::cerr << "Failed creating a RandomChargingPlan object" << std::endl;
        exit(EXIT_FAILURE);
    }

    chargingPlanID = rcp->getRandomChargingPlan();
    if (chargingPlanID != NO_CHARGING_PLAN_FOUND)
        request->__AddServiceProvider_sequence->ns8__chargingIdentifier = &chargingPlanID;

    delete rcp;

    // making the request and getting the response

    int errCode = serviceProvider->AddServiceProvider (request, response);
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
        delete s;
    }

    request->~_ns6__AddServiceProvider();
    delete request;

    return 0;
}
