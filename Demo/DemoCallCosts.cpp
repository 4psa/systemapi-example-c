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

#define RESPONSE_BEGIN "HTTP/1.1 200 OK"

int main(int argc, char *argv[])
{
    // make sure that each time you run your application, a valid access token is provided
    if (argc != 2) {
        std::cerr << "Usage: ./<executable_name> \"<access_token>\"" << std::endl;
        std::cerr << "	example: ./democallcosts \"1|V_pmPvEm25-HrqAzERx_nvJbBvNs~q3F|1|D~nbBUf87k~7I12F79T-nJnHU12Y.4Aq\"" << std::endl;
        exit(EXIT_FAILURE);
    }


    // We will make a Call report, so we need a Report object
    Report *report = new Report;
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
    report->soap->header->ns4__serverInfo = NULL;
    report->soap->header->ns4__userCredentials = new _ns4__userCredentials;
    if (NULL == report->soap->header->ns4__userCredentials) {
        std::cerr << "Failed creating an _ns4__userCredentials  object" << std::endl;
        exit(EXIT_FAILURE);
    }
    report->soap->header->ns4__userCredentials->accessToken = accessToken;


    // creating 2 objects for the request and for the response
    _ns21__CallReport *request = new _ns21__CallReport;
    if (NULL == request) {
        std::cerr << "Failed creating a CallReport object" << std::endl;
        exit(EXIT_FAILURE);
    }

    _ns21__CallReportResponse *response = new _ns21__CallReportResponse;
    if (NULL == response) {
        std::cerr << "Failed creating a CallReportResponse object" << std::endl;
        exit(EXIT_FAILURE);
    }



    // making the request and getting the response
    int errCode = report->__ns32__CallReport(request, response);
    if (SOAP_OK == errCode) {
        // no error
        std::cout << "OK retrieving call report\n";
        std::ifstream logfile("RECV.log");
        std::stringstream ssBuffer;
        std::string fileContent;

        ssBuffer << logfile.rdbuf();
        fileContent = ssBuffer.str();

        size_t posResult = fileContent.rfind(RESPONSE_BEGIN);
        std::cout << std::endl << std::endl << "Result is " << std::endl <<
                fileContent.substr(posResult) << std::endl;

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
