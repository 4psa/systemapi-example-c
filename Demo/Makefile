#
# 4PSA VoipNow Service Providers Edition SystemAPI Client for C++
	
# Copyright (c) 2012, Rack-Soft (www.4psa.com). All rights reserved.
# VoipNow is a Trademark of Rack-Soft, Inc
# 4PSA is a Registered Trademark of Rack-Soft, Inc.
# All rights reserved.

# This Makefile can be used for compiling and running demo System API cients

# compiler
CC = g++

# flags
FLAGS = -lssl -DDEBUG -DWITH_OPENSSL

# executables for 'all' target
EXECS = demoaddserviceprovider demoaddorganization demoadduser demoaddextension democallcosts

# all sources
SOURCES = DemoAddServiceProvider.cpp DemoAddOrganization DemoAddUser DemoAddExtension DemoCallCosts RandomChargingPlan.cpp soapC.cpp soapClient.cpp stdsoap2.cpp utils.cpp

# all objects
OBJECTS = $(SOURCES:.c=.o)

GEN_SOURCES = soapC.cpp soapClient.cpp stdsoap2.cpp

#dependencies

# general dependencies
GEN_OBJ = soapC.o soapClient.o stdsoap2.o

# demoaddserviceprovider dependencies
SP_OBJ = DemoAddServiceProvider.o $(GEN_OBJ) RandomChargingPlan.o
# demoaddorganization dependencies 
ORG_OBJ = DemoAddOrganization.o $(GEN_OBJ) RandomChargingPlan.o utils.o
# demoadduser dependencies
USR_OBJ = DemoAddUser.o $(GEN_OBJ) RandomChargingPlan.o utils.o
# demoaddextension dependencies
EXT_OBJ = DemoAddExtension.o $(GEN_OBJ) utils.o
# democallcosts dependencies
CST_OBJ = DemoCallCosts.o

#default target
all: $(EXECS)

$(OBJECTS): $(@:.o=.c)

demoaddserviceprovider: $(SP_OBJ)
	 $(CC) $(FLAGS) -o demoaddserviceprovider DemoAddServiceProvider.cpp $(GEN_SOURCES) RandomChargingPlan.cpp

demoaddorganization: $(ORG_OBJ)
	$(CC) $(FLAGS) -o demoaddorganization DemoAddOrganization.cpp $(GEN_SOURCES) RandomChargingPlan.cpp utils.cpp

demoadduser: $(USR_OBJ)
	$(CC) $(FLAGS) -o demoadduser DemoAddUser.cpp $(GEN_SOURCES) RandomChargingPlan.cpp utils.cpp

demoaddextension: $(EXT_OBJ)
	$(CC) $(FLAGS) -o demoaddextension DemoAddExtension.cpp $(GEN_SOURCES) utils.cpp

democallcosts: $(CST_OBJ)
	$(CC) $(FLAGS) -o democallcosts DemoCallCosts.cpp $(GEN_SOURCES)

clean:
	rm -f *.o
	rm -f demoaddserviceprovider demoaddorganization demoadduser demoaddextension democallcosts
