/**
 * 4PSA VoipNow Service Providers Edition SystemAPI Client for C++
 *	
 * Copyright (c) 2012, Rack-Soft (www.4psa.com). All rights reserved.
 * VoipNow is a Trademark of Rack-Soft, Inc
 * 4PSA is a Registered Trademark of Rack-Soft, Inc.
 * All rights reserved.
 *
 * This demo program adds an organization using the SystemAPI
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
#include "utils.h"

// CHANGEME
#define COUNTRY "ro"

int main(int argc, char *argv[])
{
    /* 
     * make sure that each time you run your application, a valid access token is provided
     * a service provider id can also be passed from command line
     **/
    if (argc < 2 || argc > 3) {
        std::cerr << "Usage: ./<executable_name> \"<access_token>\" [\"<Service_Provider_ID>\"]" << std::endl;
        std::cerr << "If no service provider ID is provided, a random ID will be generated;" << std::endl;
        std::cerr << "	example: ./demoaddorganization \"1|V_pmPvEm25-HrqAzERx_nvJbBvNs~q3F|1|v-gntT4GFH-UCUX0EM2_r9XTVDtw~qCF\" [\"72\"]" << std::endl;
        exit(EXIT_FAILURE);
    }

	// if a service provider ID is provided, make sure the format is correct
    if (3 == argc && (false == isPositiveInteger(argv[2]))) {
        std::cerr << "Service provider ID must be a positive integer number" << std::endl;
        exit(EXIT_FAILURE);
    }


    // We will add a new Organization, so we need an Organization object
    Organization *organization = new Organization;
    if (NULL == organization) {
        std::cerr << "Failed creating an Organization object" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Authentication Data - passed from command line
    std::string accessToken(argv[1]);

    // filling in the header with the user credentials
    organization->soap->header = new SOAP_ENV__Header;
    if (NULL == organization->soap->header) {
        std::cerr << "Failed creating a SOAP_ENV__Header object" << std::endl;
        exit(EXIT_FAILURE);
    }
    organization->soap->header->ns4__serverInfo = NULL;
    organization->soap->header->ns4__userCredentials =
            new _ns4__userCredentials;
    if (NULL == organization->soap->header->ns4__userCredentials) {
        std::cerr << "Failed creating an _ns4__userCredentials object" << std::endl;
        exit(EXIT_FAILURE);
    }
    organization->soap->header->ns4__userCredentials->accessToken =
            accessToken;

    // creating 2 objects for the request and for the response
    _ns10__AddOrganization *request = new _ns10__AddOrganization;
    if (NULL == request) {
        std::cerr << "Failed creating an AddOrganization object" << std::endl;
        exit(EXIT_FAILURE);
    }
    _ns10__AddOrganizationResponse *response =
            new _ns10__AddOrganizationResponse;
    if (NULL == response) {
        std::cerr << "Failed creating an AddOrganizationResponse object" <<
                std::endl;
        exit(EXIT_FAILURE);
    }


    // information about the new organization (name, login, password, country and charging plan)
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
    name_ss << "OrganizationCPP_" << rand() % 1000;
    login_ss << "Organization_" << rand() % 1000;
    pass_ss << "Pass_" << rand() % 1000;


    name_ss >> name;
    login_ss >> login;
    pass_ss >> pass;

    request->name = &name;
    request->login = &login;
    request->password = &pass;
    request->country = &country;


    // Service Provider ID - passed from command line or randomly generated
    std::string parentID;

    if (3 == argc) {
        // service provider ID was passed from command line
        std::string temp(argv[2]);
        parentID = temp;
    } else {
        // random service provider ID
        std::cout << "No service provider ID was provided, will randomly generate one" << std::endl;

        // getting all service providers

        // we need a ServiceProvider object 
        ServiceProvider *serviceProvider = new ServiceProvider;
        if (NULL == serviceProvider) {
            std::cerr << "Failed creating a ServiceProvider object" << std::endl;
            exit(EXIT_FAILURE);
        }

        // filling in the header with the user credentials
        serviceProvider->soap->header = new SOAP_ENV__Header;
        if (NULL == serviceProvider->soap->header) {
            std::cerr << "Failed creating a SOAP_ENV__Header object" << std::endl;
            exit(EXIT_FAILURE);
        }
        serviceProvider->soap->header->ns4__serverInfo = NULL;
        serviceProvider->soap->header->ns4__userCredentials = new _ns4__userCredentials;
        if (NULL == serviceProvider->soap->header->ns4__userCredentials) {
            std::cerr << "Failed creating an _ns4__userCredentials object" << std::endl;
            exit(EXIT_FAILURE);
        }
        serviceProvider->soap->header->ns4__userCredentials->accessToken = accessToken;

        // creating 2 objects for the request and for the response
        _ns7__GetServiceProviders * spRequest = new _ns7__GetServiceProviders;
        if (NULL == spRequest) {
            std::cerr << "Failed creating a GetServiceProviders object" << std::endl;
            exit(EXIT_FAILURE);
        }
        _ns7__GetServiceProvidersResponse *spResponse = new _ns7__GetServiceProvidersResponse;
        if (NULL == spResponse) {
            std::cerr << "Failed creating a GetServiceProvidersResponse object" << std::endl;
            exit(EXIT_FAILURE);
        }


        // making the request for getting service providers and getting the response
        int errCodeSP = serviceProvider->__ns25__GetServiceProviders(spRequest, spResponse);
        if (SOAP_OK == errCodeSP) {
            // no error
            std::cout << "OK retrieving service providers" << std::endl;

            // if no service providers exist, will exit
            if (0 == spResponse->serviceProvider.size()) {
                std::cerr << "No service providers are defined" << std::endl;
                exit(EXIT_FAILURE);
            }

            // found service providers

            // randomly choosing a service provider
            int randomIndex = rand() % spResponse->serviceProvider.size();

            // getting the id of the service provider
            std::string randomID = *spResponse->serviceProvider.at(randomIndex)->ns6__ID;
            std::cout << "Using random service provider ID " << randomID << std::endl;
            parentID = randomID;

        } else {
            // error found
            soap *s = new soap;
            if (s == NULL) {
                std::cerr << "Failed creating a soap object" << std::endl;
                exit(EXIT_FAILURE);
            }
            s->error = errCodeSP;
            soap_print_fault(s, stderr);
        }
    }

    request->ns5__parentID = &parentID;

    // get a random charging plan ID using RandomChargingPlan class
    RandomChargingPlan *rcp = new RandomChargingPlan(accessToken, parentID);
    if (NULL == rcp) {
        std::cerr << "Failed creating a RandomChargingPlan object" << std::endl;
        exit(EXIT_FAILURE);
    }

    chargingPlanID = rcp->getRandomChargingPlan();
    if (chargingPlanID != NO_CHARGING_PLAN_FOUND) {
        request->ns5__chargingPlanID = &chargingPlanID;
    }


    // making the request for adding organization and getting the response
    int errCode = organization->__ns26__AddOrganization(request, response);
    if (SOAP_OK == errCode) {
        // no error
        std::cout << "OK adding organization" << std::endl;
    } else {
        // error found
        soap *s = new soap;
        if (NULL == s) {
            std::cerr << "Failed creating a soap object" << std::endl;
            exit(EXIT_FAILURE);
        }
        s->error = errCode;
        soap_print_fault(s, stderr);
        std::cerr << "Please check the log files for more information" << std::endl;
    }

    return 0;
}
