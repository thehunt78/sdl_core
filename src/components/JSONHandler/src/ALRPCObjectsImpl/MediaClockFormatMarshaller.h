#ifndef MEDIACLOCKFORMATMARSHALLER_INCLUDE
#define MEDIACLOCKFORMATMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>

#include "PerfectHashTable.h"

#include "../../include/JSONHandler/ALRPCObjects/MediaClockFormat.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Oct 24 15:41:28 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


//! marshalling class for MediaClockFormat

class MediaClockFormatMarshaller
{
public:

  static std::string toName(const MediaClockFormat& e) 	{ return getName(e.mInternal) ?: ""; }

  static bool fromName(MediaClockFormat& e,const std::string& s)
  { 
    return (e.mInternal=getIndex(s.c_str()))!=MediaClockFormat::INVALID_ENUM;
  }

  static bool checkIntegrity(MediaClockFormat& e)		{ return e.mInternal!=MediaClockFormat::INVALID_ENUM; } 
  static bool checkIntegrityConst(const MediaClockFormat& e)	{ return e.mInternal!=MediaClockFormat::INVALID_ENUM; } 

  static bool fromString(const std::string& s,MediaClockFormat& e);
  static const std::string toString(const MediaClockFormat& e);

  static bool fromJSON(const Json::Value& s,MediaClockFormat& e);
  static Json::Value toJSON(const MediaClockFormat& e);

  static const char* getName(MediaClockFormat::MediaClockFormatInternal e)
  {
     return (e>=0 && e<5) ? mHashTable[e].name : NULL;
  }

  static const MediaClockFormat::MediaClockFormatInternal getIndex(const char* s);

  static const PerfectHashTable mHashTable[5];
};

#endif
