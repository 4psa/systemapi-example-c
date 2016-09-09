/**
 * 4PSA VoipNow Service Providers Edition SystemAPI Client for C++
 *	
 * Copyright (c) 2012, Rack-Soft (www.4psa.com). All rights reserved.
 * VoipNow is a Trademark of Rack-Soft, Inc
 * 4PSA is a Registered Trademark of Rack-Soft, Inc.
 * All rights reserved.
 *
 * This demo program makes a request for a call report using the SystemAPI
 *
 **/

#include<sstream>
#include<string> 
#include<fstream>
#include<time.h>

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
#include "stdsoap2.h"
#include "Report.nsmap"
#include "utils.h"

#define RESPONSE_BEGIN "HTTP/1.1 200 OK"
#define START_DATE "2012-01-01"
#define END_DATE "2016-01-01"

int main(int argc, char *argv[])
{
    // make sure that each time you run your application, a valid access token is provided
    if (argc < 2 || argc > 3) {
        std::cerr << "Usage: ./<executable_name> \"<access_token>\" [\"<Extension_identifier\"]" << std::endl;
        std::cerr << "	example: ./democallcosts \"1|V_pmPvEm25-HrqAzERx_nvJbBvNs~q3F|1|D~nbBUf87k~7I12F79T-nJnHU12Y.4Aq\"" << std::endl;
        exit(EXIT_FAILURE);
    }

    // We will make a Call report, so we need a Report object
    ReportProxy *report = new ReportProxy;
    if (NULL == report) {
        std::cerr << "Failed creating a Report object" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Authentication Data - passed from command line
    std::string accessToken(argv[1]);

    // filling in the header with the user credentials
    report->soap->header = new SOAP_ENV__Header;
    if (NULL == report->soap->header) {
        std::cerr << "Failed creating a SOAP_ENV__Header object" << std::endl;
        exit(EXIT_FAILURE);
    }
    report->soap->header->ns3__serverInfo = NULL;
    report->soap->header->ns3__userCredentials = new _ns3__userCredentials;
    if (NULL == report->soap->header->ns3__userCredentials) {
        std::cerr << "Failed creating an _ns3__userCredentials  object" << std::endl;
        exit(EXIT_FAILURE);
    }
    report->soap->header->ns3__userCredentials->accessToken = accessToken;

    // creating 2 objects for the request and for the response
    _ns20__CallCosts *request = new _ns20__CallCosts;
    if (NULL == request) {
        std::cerr << "Failed creating a CallReport object" << std::endl;
        exit(EXIT_FAILURE);
    }

    _ns20__CallCostsResponse response;

    // User ID - passed from the command line or randomly generated
    std::string parentID;

    if (3 == argc) {
        // User ID was passed from command line
        std::string temp(argv[2]);
        parentID = temp;
    } else {
        // random extension ID
        std::cout << "No extension ID was provided, will randomly generate one" << std::endl;

        // getting all extensions

        // we need an Extension object
        ExtensionProxy *extension = new ExtensionProxy;
        if (NULL == extension) {
            std::cerr << "Failed creating an Extension object" << std::endl;
            exit(EXIT_FAILURE);
        }

        // filling in the header with the user credentials
        extension->soap->header = new SOAP_ENV__Header;
        if (NULL == extension->soap->header) {
            std::cerr << "Failed creating a SOAP_ENV__Header object" << std::endl;
            exit(EXIT_FAILURE);
        }
        extension->soap->header->ns3__serverInfo = NULL;
        extension->soap->header->ns3__userCredentials = new _ns3__userCredentials;
        if (NULL == extension->soap->header->ns3__userCredentials) {
            std::cerr << "Failed creating a _ns3__userCredentials object" << std::endl;
            exit(EXIT_FAILURE);
        }
        extension->soap->header->ns3__userCredentials->accessToken = accessToken;

        // creating 2 objects for the request and for the response
        _ns13__GetExtensions *extRequest = new _ns13__GetExtensions;
        if (NULL == extRequest) {
            std::cerr << "Failed creating a _ns13__GetExtensions object" << std::endl;
            exit(EXIT_FAILURE);
        }

        _ns13__GetExtensionsResponse extResponse;

        // making the request for getting extensions and getting the response
        int errCodeExt = extension->GetExtensions(extRequest, extResponse);
        if (SOAP_OK == errCodeExt) {
            // no error
            std::cout << "OK retrieving extensions" << std::endl;

            if (0 == extResponse.extension.size()) {
               // no extensions exist
                std::cerr << "No extensions are defined" << std::endl;
                exit(EXIT_FAILURE);
            }

            // found extensions

            // randomly choosing an extension
            srand(time(NULL));
            int randomIndex = rand() % extResponse.extension.size();

            // getting the id of the user
            std::string randomID = *extResponse.extension.at(randomIndex)->identifier;
            std::cout << "Using random extension identifier " << randomID << std::endl;
            parentID = randomID;

        } else {
            // error found
            soap *s = new soap;
            if (NULL == s) {
                std::cerr << "Failed creating a soap object" << std::endl;
                exit(EXIT_FAILURE);
            }
            s->error = errCodeExt;
            soap_print_fault(s, stderr);
            delete s;
        }
    }

    request->__CallCosts_sequence = new __ns20__CallCosts_sequence;
    if (NULL == request->__CallCosts_sequence) {
        std::cerr << "Failed creating a __ns20__CallCosts_sequence" << std::endl;
        exit(EXIT_FAILURE);
    }

    request->__CallCosts_sequence->ns21__userIdentifier = &parentID;

    //get current date for the endDate parameter of the interval for the report
    time_t the_time = time(NULL);
    struct tm *a_time = localtime(&the_time);
    char *end_date = (char*)malloc(12 * sizeof(char));
    if (NULL == end_date) {
        std::cerr << "Failed creating string for end_date" << std::endl;
        exit(EXIT_FAILURE);
    }
    sprintf(end_date, "%d-%02d-%d", a_time->tm_year + 1900, a_time->tm_mon + 1, a_time->tm_mday);

    // information about the call report(interval)
    std::string startDate(START_DATE);
    std::string endDate(end_date);

    free(end_date);

    request->__CallCosts_sequence->ns21__interval = new _ns21__interval;
    if (NULL == request->__CallCosts_sequence->ns21__interval) {
        std::cerr << "Failed creating a _ns21__interval object" << std::endl;
        exit(EXIT_FAILURE);
    }

    // filling in the information about the call report (interval)
    request->__CallCosts_sequence->ns21__interval->startDate = &startDate;
    request->__CallCosts_sequence->ns21__interval->endDate = &endDate;

    // making the request and getting the response
    int errCode = report->CallCosts(request, response);
    if (SOAP_OK == errCode) {
        // no error
        std::cout << "OK retrieving call costs" << std::endl;

        std::cout << *response.ns21__totalCalls << " calls have been made between " << startDate << " and " << endDate;
        std::cout << " with a total cost of " << *response.ns21__cost << " " << *response.ns21__currency << std::endl;

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

    delete request->__CallCosts_sequence->ns21__interval;
    delete request->__CallCosts_sequence;
    request->~_ns20__CallCosts();
    delete request;

    return 0;
}
