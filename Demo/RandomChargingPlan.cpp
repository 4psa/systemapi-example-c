/**
 * 4PSA VoipNow Service Providers Edition SystemAPI Client for C++
 *
 * Copyright (c) 2012, Rack-Soft (www.4psa.com). All rights reserved.
 * VoipNow is a Trademark of Rack-Soft, Inc
 * 4PSA is a Registered Trademark of Rack-Soft, Inc.
 * All rights reserved.
 *
 * This utilitary class is used to generate a random charging plan ID
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
#include "stdsoap2.h"
#include "RandomChargingPlan.h"



RandomChargingPlan::RandomChargingPlan(std::string token)
{
    accessToken = token;
	parent_id = NO_PARENT_ID;
}

RandomChargingPlan::RandomChargingPlan(std::string token, std::string id)
{
    accessToken = token;
    parent_id = id;
}

std::string RandomChargingPlan::getRandomChargingPlan()
{

    srand(time(NULL));
    // random charging plan ID
    std::cout << "Will randomly generate a charging plan" << std::endl;

    // getting all charging plans

    // we need a Billing Object
    Billing *billing = new Billing;
    if (NULL == billing) {
        std::cerr << "Failed creating a Billing object" << std::endl;
        exit(EXIT_FAILURE);
    }

    // filling in the header with the user credentials
    billing->soap->header = new SOAP_ENV__Header;
	if (NULL == billing->soap->header) {
            std::cerr << "Failed creating a SOAP_ENV__Header object" << std::endl;
            exit(EXIT_FAILURE);
        }
    billing->soap->header->ns4__serverInfo = NULL;
    billing->soap->header->ns4__userCredentials = new _ns4__userCredentials;
	if (NULL == billing->soap->header->ns4__userCredentials) {
            std::cerr << "Failed creating an _ns4__userCredentials object" << std::endl;
            exit(EXIT_FAILURE);
        }
    billing->soap->header->ns4__userCredentials->accessToken = accessToken;

    // creating 2 objects for the request and for the response
    _ns19__GetChargingPlans * cpRequest = new _ns19__GetChargingPlans;
	if (NULL == cpRequest) {
        std::cerr << "Failed creating a GetChargingPlans object" << std::endl;
        exit(EXIT_FAILURE);
    }
	
    _ns19__GetChargingPlansResponse *cpResponse = new _ns19__GetChargingPlansResponse;
    if (NULL == cpResponse) {
        std::cerr << "Failed creating a GetChargingPlansResponse object" << std::endl;
        exit(EXIT_FAILURE);
    }

	if (parent_id != NO_PARENT_ID)
        cpRequest->ns6__userID = &parent_id;
	

    // making the request for getting charging plans and getting the response
    int errCodeCP = billing->__ns31__GetChargingPlans(cpRequest, cpResponse);
    if (SOAP_OK == errCodeCP) {
        // no error
        std::cout << "OK retrieving charging plans" << std::endl;
        if (0 == cpResponse->chargingPlan.size()) {
            std::cout << "Couldn`t find a charging plan" << std::endl;
			return NO_CHARGING_PLAN_FOUND;
        } else {
            // randomly choosing a charging plan
            int randomIndex = rand() % cpResponse->chargingPlan.size();

            // getting the id of the charging plang
            std::string randomID = *cpResponse->chargingPlan.at(randomIndex)->ID;
            std::cout << "Using random charging plan ID " << randomID << std::endl;

            return randomID;
        }

    } else {
        // error found
        soap *s = new soap;
		if (NULL == s) {
        std::cerr << "Failed creating a soap object" << std::endl;
        exit(EXIT_FAILURE);
		}
        s->error = errCodeCP;
        soap_print_fault(s, stderr);
    }

    return NO_CHARGING_PLAN_FOUND;
}
