/**
 * 4PSA VoipNow Service Providers Edition SystemAPI Client for C++
 *	
 * Copyright (c) 2012, Rack-Soft (www.4psa.com). All rights reserved.
 * VoipNow is a Trademark of Rack-Soft, Inc
 * 4PSA is a Registered Trademark of Rack-Soft, Inc.
 * All rights reserved.
 *
 * This demo program adds an user using the SystemAPI
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
#include "User.nsmap"
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
        std::cerr << "Usage: ./<executable_name> \"<access_token>\" [\"<Organization_ID>\"]" << std::endl;
        std::cerr << "If no organization ID is provided, a random ID will be generated;" << std::endl;
        std::cerr << "	example: ./demoadduser \"1|V_pmPvEm25-HrqAzERx_nvJbBvNs~q3F|1|v-gntT4GFH-UCUX0EM2_r9XTVDtw~qCF\" [\"125\"]" << std::endl;
        exit(EXIT_FAILURE);
    }

	// if an organization ID is provided, make sure the format is correct
    if (3 == argc && (false == isPositiveInteger(argv[2]))) {
        std::cerr << "Organization ID must be a positive integer number" << std::endl;
        exit(EXIT_FAILURE);
    }


    // We will add a new user, so we need an User object
    User *user = new User;
    if (NULL == user) {
        std::cerr << "Failed creating a User object" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Authentication Data - passed from command line
    std::string accessToken(argv[1]);

    // filling in the header with the user credentials
    user->soap->header = new SOAP_ENV__Header;
    if (NULL == user->soap->header) {
        std::cerr << "Failed creating a SOAP_ENV__Header object" << std::endl;
        exit(EXIT_FAILURE);
    }
    user->soap->header->ns4__serverInfo = NULL;
    user->soap->header->ns4__userCredentials = new _ns4__userCredentials;
    if (NULL == user->soap->header->ns4__userCredentials) {
        std::cerr << "Failed creating an _ns4__userCredentials object" << std::endl;
        exit(EXIT_FAILURE);
    }
    user->soap->header->ns4__userCredentials->accessToken = accessToken;

    // creating 2 objects for the request and for the response
    _ns11__AddUser *request = new _ns11__AddUser;
    if (NULL == request) {
        std::cerr << "Failed creating an AddUser object" << std::endl;
        exit(EXIT_FAILURE);
    }

    _ns11__AddUserResponse *response = new _ns11__AddUserResponse;
    if (NULL == response) {
        std::cerr << "Failed creating an AddUserResponse object" << std::endl;
        exit(EXIT_FAILURE);
    }


    // information about the new user (name, login, password, country and charging plan)
    std::stringstream name_ss;
    std::stringstream login_ss;
    std::stringstream pass_ss;

    std::string name;
    std::string login;
    std::string pass;
    std::string country(COUNTRY);
    std::string chargingPlanID;

    srand(time(NULL));

    // filling in the information about the new user (name, login, password and country)
    name_ss << "UserCPP_" << rand() % 1000;
    login_ss << "User_" << rand() % 1000;
    pass_ss << "Pass_" << rand() % 1000;

    name_ss >> name;
    login_ss >> login;
    pass_ss >> pass;


    request->name = &name;
    request->login = &login;
    request->password = &pass;
    request->country = &country;

    // Organization ID - passed from command line or randomly generated
    std::string parentID;

    if (3 == argc) {
        // organization ID was passed from command line
        std::string temp(argv[2]);
        parentID = temp;
    } else {
        // random organization ID
        std::cout << "No organization ID was provided, will randomly generate one" << std::endl;

        // getting all organizations

        // we need an Organization object 
        Organization *organization = new Organization;
        if (NULL == organization) {
            std::cerr << "Failed creating an Organization object" << std::endl;
            exit(EXIT_FAILURE);
        }

        // filling in the header with the user credentials
        organization->soap->header = new SOAP_ENV__Header;
        if (NULL == organization->soap->header) {
            std::cerr << "Failed creating a SOAP_ENV__Header object" << std::endl;
            exit(EXIT_FAILURE);
        }
        organization->soap->header->ns4__serverInfo = NULL;
        organization->soap->header->ns4__userCredentials = new _ns4__userCredentials;
        if (NULL == organization->soap->header->ns4__userCredentials) {
            std::cerr << "Failed creating a _ns4__userCredentials object" << std::endl;
            exit(EXIT_FAILURE);
        }
        organization->soap->header->ns4__userCredentials->accessToken = accessToken;

        // creating 2 objects for the request and for the response
        _ns10__GetOrganizations * orgRequest = new _ns10__GetOrganizations;
        if (NULL == orgRequest) {
            std::cerr << "Failed creating a GetOrganizations object" << std::endl;
            exit(EXIT_FAILURE);
        }

        _ns10__GetOrganizationsResponse *orgResponse = new _ns10__GetOrganizationsResponse;
        if (NULL == orgResponse) {
            std::cerr << "Failed creating a GetOrganizationsResponse object" << std::endl;
            exit(EXIT_FAILURE);
        }


        // making the request for getting the organizations and getting the response
        int errCodeOrg = organization->__ns26__GetOrganizations(orgRequest, orgResponse);
        if (SOAP_OK == errCodeOrg) {
            // no error
            std::cout << "OK retrieving organizations" << std::endl;

            if (0 == orgResponse->organization.size()) {
                // no organizations found
                std::cerr << "No organizations are defined" << std::endl;
                exit(EXIT_FAILURE);
            }

            // found organizations

            // randomly choosing an organization
            int randomIndex = rand() % orgResponse->organization.size();

            // getting the id of the organization
            std::string randomID = *orgResponse->organization.at(randomIndex)->ns6__ID;
            std::cout << "Using random organization ID " << randomID << std::endl;
            parentID = randomID;

        } else {
            // error found
            soap *s = new soap;
            if (NULL == s) {
                std::cerr << "Failed creating a soap object" << std::endl;
                exit(EXIT_FAILURE);
            }
            s->error = errCodeOrg;
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


    // making the request and getting the response
    int errCode = user->__ns27__AddUser(request, response);
    if (SOAP_OK == errCode) {
        // no error
        std::cout << "OK adding user" << std::endl;
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
