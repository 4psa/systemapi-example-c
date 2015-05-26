/**
 * 4PSA VoipNow Service Providers Edition SystemAPI Client for C++
 *
 * Copyright (c) 2012, Rack-Soft (www.4psa.com). All rights reserved.
 * VoipNow is a Trademark of Rack-Soft, Inc
 * 4PSA is a Registered Trademark of Rack-Soft, Inc.
 * All rights reserved.
 *
 * This utilitary class is used to generate a random charging plan ID
 * If you need to use it, include "RandomChargingPlan.h" in your program
 *
 **/
 
#ifndef RANDOMCHARGINGPLAN_H
#define RANDOMCHARGINGPLAN_H

#define NO_CHARGING_PLAN_FOUND ""
#define NO_PARENT_ID ""

class RandomChargingPlan{
	public:
		/* 
		* RandomChargingPlan constructor
		* @token: accessToken
		**/
		RandomChargingPlan(std::string token);
		
		/*
		* RandomChargingPlan constructor
		* @token: accessToken
		* @id: parentID
		**/
		RandomChargingPlan(std::string token, std::string id);
		
		/*
		* returns a random charging plan ID
		**/
		std::string getRandomChargingPlan();

	private:
		std::string parent_id;
		std::string accessToken;
};

#endif

