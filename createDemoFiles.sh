# 4PSA VoipNow Service Providers Edition SystemAPI Client for C++
#	
# Copyright (c) 2012, Rack-Soft (www.4psa.com). All rights reserved.
# VoipNow is a Trademark of Rack-Soft, Inc
# 4PSA is a Registered Trademark of Rack-Soft, Inc.
# All rights reserved.
#
# This script is used to generate all files necessary for running
# SystemAPI applications
# 
# Please check Makefile for running demo applications


#!/bin/bash

# check number of arguments
EXPECTED_ARGS=3
BADARGS=65

if [ $# -ne $EXPECTED_ARGS ]
then
	echo "Usage: ./createDemoFiles.sh server_hostname PATH_TO_GSOAP PATH_TO_SCHEMA_FILES"
	exit $BADARGS
fi



# building path names
# path to current directory
PATH_TO_BASE="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
# path to gsoap directory
PATH_TO_GSOAP="$2/gsoap"
# path to schema directory
PATH_TO_SCHEMA=$3
# path to demo files
PATH_TO_DEMO="${PATH_TO_BASE}/Demo"
# path to gsoap's wsdl2h
PATH_TO_WSDL2H="wsdl2h"
# path to gsoap's soapcpp2
PATH_TO_SOAPCPP2="soapcpp2"
# path to voipnowservice.wsdl
PATH_TO_WSDL="${PATH_TO_SCHEMA}/voipnowservice.wsdl"
# path to gsoap schema files
PATH_TO_WS="${PATH_TO_GSOAP}/WS/"
# path to gsoap imports
PATH_TO_IMPORT="${PATH_TO_GSOAP}/import/"

# replace endpoint with provided hostname
find ${PATH_TO_SCHEMA} -iname "*.wsdl" -exec sed -i "s#<soap:address location=\"https://[^/]*#<soap:address location=\"https://$1#g" {} \;

# compile the WSDL
# please check README for parameter significance
${PATH_TO_WSDL2H} -I ${PATH_TO_WS} -f -u -k -o voip.h ${PATH_TO_WSDL}

# generate C++ stub files
# please check README for parameter significance
${PATH_TO_SOAPCPP2} -C -L -I ${PATH_TO_IMPORT} -w -x -j voip.h

# copy generated files to Demo folder
cp -f ${PATH_TO_BASE}/soap*.h \
      ${PATH_TO_BASE}/*.cpp \
      ${PATH_TO_BASE}/*.nsmap \
      ${PATH_TO_GSOAP}/stdsoap2.cpp \
      ${PATH_TO_GSOAP}/stdsoap2.h \
      ${PATH_TO_DEMO}

# add some lines after soap_new()
# please check README for more info
OLD_TEXT="soap_new();"
NEW_TEXT="soap_new();soap_ssl_init();if(soap_ssl_client_context(soap, SOAP_SSL_SKIP_HOST_CHECK, NULL, NULL, NULL, NULL, NULL)) soap_print_fault(soap, stderr);"

# add lines to all proxy files
for proxyFile in ${PATH_TO_DEMO}/soap*Proxy.h; do
 sed "s/$OLD_TEXT/$NEW_TEXT/g" ${proxyFile}> ${proxyFile}copy;
 cp -f ${proxyFile}copy ${proxyFile};
 rm -f ${proxyFile}copy;
done

























