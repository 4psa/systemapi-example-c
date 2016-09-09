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
#define COMPANY "TEST COMPANY"

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
    OrganizationProxy *organization = new OrganizationProxy;
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
    organization->soap->header->ns3__serverInfo = NULL;
    organization->soap->header->ns3__userCredentials = new _ns3__userCredentials;
    if (NULL == organization->soap->header->ns3__userCredentials) {
        std::cerr << "Failed creating an _ns3__userCredentials object" << std::endl;
        exit(EXIT_FAILURE);
    }
    organization->soap->header->ns3__userCredentials->accessToken = accessToken;

    // creating 2 objects for the request and for the response
    _ns9__AddOrganization *request = new _ns9__AddOrganization;
    if (NULL == request) {
        std::cerr << "Failed creating an AddOrganization object" << std::endl;
        exit(EXIT_FAILURE);
    }
    _ns9__AddOrganizationResponse response;


    // information about the new organization (name, login, password, country and charging plan)
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
    name_ss << "OrganizationCPP_" << rand() % 1000;
    login_ss << "Organization_" << rand() % 1000;
    firstname_ss << "FirstnameCPP_" << rand() % 1000;
    lastname_ss << "LastnameCPP_" << rand() % 1000;
    email_ss << "EmailOrganization_" << rand() % 1000 << "@example.com";

    name_ss >> name;
    login_ss >> login;
    firstname_ss >> firstname;
    lastname_ss >> lastname;
    email_ss >> email;

    request->__AddOrganization_sequence = new __ns9__AddOrganization_sequence;
    if (NULL == request->__AddOrganization_sequence) {
        std::cerr << "Failed creatin an __AddOrganization_sequence object" << std::endl;
        exit(EXIT_FAILURE);
    }

    request->__AddOrganization_sequence->ns8__name = &name;
    request->__AddOrganization_sequence->ns8__login = &login;
    request->__AddOrganization_sequence->ns8__firstName = &firstname;
    request->__AddOrganization_sequence->ns8__lastName = &lastname;
    request->__AddOrganization_sequence->ns8__email = &email;
    request->__AddOrganization_sequence->ns8__passwordAuto = &passAuto;
    request->__AddOrganization_sequence->ns8__country = &country;
    request->__AddOrganization_sequence->ns8__company = &company;

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
        ServiceProviderProxy *serviceProvider = new ServiceProviderProxy;
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
        serviceProvider->soap->header->ns3__serverInfo = NULL;
        serviceProvider->soap->header->ns3__userCredentials = new _ns3__userCredentials;
        if (NULL == serviceProvider->soap->header->ns3__userCredentials) {
            std::cerr << "Failed creating an _ns3__userCredentials object" << std::endl;
            exit(EXIT_FAILURE);
        }
        serviceProvider->soap->header->ns3__userCredentials->accessToken = accessToken;

        // creating 2 objects for the request and for the response
        _ns6__GetServiceProviders * spRequest = new _ns6__GetServiceProviders;
        if (NULL == spRequest) {
            std::cerr << "Failed creating a _ns6__GetServiceProviders object" << std::endl;
            exit(EXIT_FAILURE);
        }
        _ns6__GetServiceProvidersResponse spResponse;

        // making the request for getting service providers and getting the response
        int errCodeSP = serviceProvider->GetServiceProviders(spRequest, spResponse);
        if (SOAP_OK == errCodeSP) {
            // no error
            std::cout << "OK retrieving service providers" << std::endl;

            // if no service providers exist, will exit
            if (0 == spResponse.serviceProvider.size()) {
                std::cerr << "No service providers are defined" << std::endl;
                exit(EXIT_FAILURE);
            }

            // found service providers

            // randomly choosing a service provider
            srand(time(NULL));
            int randomIndex = rand() % spResponse.serviceProvider.size();

            // getting the id of the service provider
            std::string randomID = *spResponse.serviceProvider.at(randomIndex)->ns5__ID;
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
            delete s;
        }

        spRequest->~_ns6__GetServiceProviders();
        delete spRequest;
    }

    request->ns4__parentID = &parentID;

    // get a random charging plan ID using RandomChargingPlan class
    RandomChargingPlan *rcp = new RandomChargingPlan(accessToken, parentID);
    if (NULL == rcp) {
        std::cerr << "Failed creating a RandomChargingPlan object" << std::endl;
        exit(EXIT_FAILURE);
    }

    chargingPlanID = rcp->getRandomChargingPlan();
    if (chargingPlanID != NO_CHARGING_PLAN_FOUND) {
        request->__AddOrganization_sequence->ns8__chargingIdentifier = &chargingPlanID;
    }

    delete rcp;

    // making the request for adding organization and getting the response
    int errCode = organization->AddOrganization(request, response);
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
        delete s;
    }

    request->~_ns9__AddOrganization();
    delete request;

    return 0;
}
