#include "Config.h"
#ifndef UNITTEST_NO_DEFERRED_REPORTER

#include "DeferredTestResult.h"
#include <cstring>
#include <algorithm>

namespace UnitTest
{

   DeferredTestFailure::DeferredTestFailure()
      : lineNumber(-1)
   {
      failureStr[0] = '\0';
   }

   DeferredTestFailure::DeferredTestFailure(int lineNumber_, const char* failureStr_)
      : lineNumber(lineNumber_)
   {
       auto length = strlen( failureStr_ ) + 1;
       auto lenToCopy = std::min( (int)length, MAX_FAILURE_STRING - 1 );

       memcpy( failureStr, failureStr_, lenToCopy );
       failureStr[ MAX_FAILURE_STRING - 1 ] = '\0';
   }

   DeferredTestResult::DeferredTestResult()
      : suiteName("")
      , testName("")
      , failureFile("")
      , timeElapsed(0.0f)
      , failed(false)
   {}

   DeferredTestResult::DeferredTestResult(char const* suite, char const* test)
      : suiteName(suite)
      , testName(test)
      , failureFile("")
      , timeElapsed(0.0f)
      , failed(false)
   {}

   DeferredTestResult::~DeferredTestResult()
   {}

}

#endif
