#ifndef GENERICRESPONSE_RESPONSEMARSHALLER_INCLUDE
#define GENERICRESPONSE_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>

#include "../../include/JSONHandler/ALRPCObjects/GenericResponse_response.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Oct 24 15:41:28 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


struct GenericResponse_responseMarshaller
{
  static bool checkIntegrity(GenericResponse_response& e);
  static bool checkIntegrityConst(const GenericResponse_response& e);

  static bool fromString(const std::string& s,GenericResponse_response& e);
  static const std::string toString(const GenericResponse_response& e);

  static bool fromJSON(const Json::Value& s,GenericResponse_response& e);
  static Json::Value toJSON(const GenericResponse_response& e);
};
#endif
