/**
 * 4PSA VoipNow Service Providers Edition SystemAPI Client for C++
 *	
 * Copyright (c) 2012, Rack-Soft (www.4psa.com). All rights reserved.
 * VoipNow is a Trademark of Rack-Soft, Inc
 * 4PSA is a Registered Trademark of Rack-Soft, Inc.
 * All rights reserved.
 *
 * This demo program adds an extension using the SystemAPI
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
#include "Extension.nsmap"
#include "stdsoap2.h"
#include "utils.h"

int main(int argc, char *argv[])
{
    /* 
     * make sure that each time you run your application, a valid access token is provided
     * a user id can also be passed from command line
     **/
    if (argc < 2 || argc > 3) {
        std::cerr << "Usage: ./<executable_name> \"<access_token>\" [\"<User_ID>\"]" << std::endl;
        std::cerr << "If no user ID is provided, a random ID will be generated;" << std::endl;
        std::cerr << "	example: ./demoaddextension \"1|V_pmPvEm25-HrqAzERx_nvJbBvNs~q3F|1|v-gntT4GFH-UCUX0EM2_r9XTVDtw~qCF\" [\"16\"]" << std::endl;
        exit(EXIT_FAILURE);
    }

	// if a user ID is provided, make sure the format is correct
    if (3 == argc && (false == isPositiveInteger(argv[2]))) {
        std::cerr << "User ID must be a positive integer number" << std::endl;
        exit(EXIT_FAILURE);
    }



    // We will add a new extension, so we need an Extension Object
    Extension *extension = new Extension;
    if (NULL == extension) {
        std::cerr << "Failed creating an Extension object" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Authentication Data - passed from command line
    std::string accessToken(argv[1]);

    // filling in the header with the user credentials
    extension->soap->header = new SOAP_ENV__Header;
    if (NULL == extension->soap->header) {
        std::cerr << "Failed creating a SOAP_ENV__Header object" << std::endl;
        exit(EXIT_FAILURE);
    }
    extension->soap->header->ns4__serverInfo = NULL;
    extension->soap->header->ns4__userCredentials =
            new _ns4__userCredentials;
    if (NULL == extension->soap->header->ns4__userCredentials) {
        std::cerr << "Failed creating an ns4__userCredentials object" << std::endl;
        exit(EXIT_FAILURE);
    }
    extension->soap->header->ns4__userCredentials->accessToken =
            accessToken;

    // creating two objects for the request and for the response
    _ns14__AddExtension *request = new _ns14__AddExtension;
    if (NULL == request) {
        std::cerr << "Failed creating an AddExtension object" << std::endl;
        exit(EXIT_FAILURE);
    }

    _ns14__AddExtensionResponse *response =
            new _ns14__AddExtensionResponse;
    if (NULL == response) {
        std::cerr << "Failed creating an AddExtensionResponse object" << std::endl;
        exit(EXIT_FAILURE);
    }



    // information about the new extension (label and password)
    std::stringstream label_ss;
    std::stringstream pass_ss;

    std::string label;
    std::string pass;

    srand(time(NULL));

    // filling in the information about the new extension (label and password)
    label_ss << "ExtensionCPP_" << rand() % 1000;
    pass_ss << "Pass_" << rand() % 1000;

    label_ss >> label;
    pass_ss >> pass;

    request->ns15__label = label;
    request->ns15__password = &pass;


    // User ID - passed from command line or randomly generated
    std::string parentID;

    if (3 == argc) {
        // user ID was passed from command line
        std::string temp(argv[2]);
        parentID = temp;
    } else {
        // random user ID
        std::cout << "No user ID was provided, will randomly generate one" << std::endl;

        // getting all users

        // we need a User object
        User *user = new User;
        if (NULL == user) {
            std::cerr << "Failed creating a User object" << std::endl;
            exit(EXIT_FAILURE);
        }

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
        _ns11__GetUsers * usrRequest = new _ns11__GetUsers;
        if (NULL == usrRequest) {
            std::cerr << "Failed creating a GetUsers object" << std::endl;
            exit(EXIT_FAILURE);
        }

        _ns11__GetUsersResponse *usrResponse = new _ns11__GetUsersResponse;
        if (NULL == usrResponse) {
            std::cerr << "Failed creating a GetUsersResponse object" << std::endl;
            exit(EXIT_FAILURE);
        }


        // making the request for getting users and getting the response
        int errCodeUsr = user->__ns27__GetUsers(usrRequest, usrResponse);
        if (SOAP_OK == errCodeUsr) {
            // no error
            std::cout << "OK retrieving users" << std::endl;

            if (0 == usrResponse->user.size()) {
                // no users exist
                std::cerr << "No users are defined" << std::endl;
                exit(EXIT_FAILURE);
            }

            // found users

            // randomly choosing a user
            int randomIndex = rand() % usrResponse->user.size();

            // getting the id of the service provider
            std::string randomID = *usrResponse->user.at(randomIndex)->ns6__ID;
            std::cout << "Using random user ID " << randomID << std::endl;
            parentID = randomID;

        } else {
            // error found
            soap *s = new soap;
            if (NULL == s) {
                std::cerr << "Failed creating a soap object" << std::endl;
                exit(EXIT_FAILURE);
            }
            s->error = errCodeUsr;
            soap_print_fault(s, stderr);
        }
    }

    request->ns5__parentID = &parentID;

    // making the request and getting the response
    int errCode = extension->__ns28__AddExtension(request, response);
    if (SOAP_OK == errCode) {
        // no error
        std::cout << "OK adding extension" << std::endl;
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
