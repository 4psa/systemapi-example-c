/**
 * 4PSA VoipNow Service Providers Edition SystemAPI Client for C++
 *
 * Copyright (c) 2012, Rack-Soft (www.4psa.com). All rights reserved.
 * VoipNow is a Trademark of Rack-Soft, Inc
 * 4PSA is a Registered Trademark of Rack-Soft, Inc.
 * All rights reserved.
 *
 * This file contains utilitary functions that are used by
 * the demo clients
 *
 **/

#include "utils.h"
#include<string>


bool isPositiveInteger(char* chinput)
{
    std::string input = chinput;
    return (input.find_first_not_of(DIGITS) == std::string::npos);
}

