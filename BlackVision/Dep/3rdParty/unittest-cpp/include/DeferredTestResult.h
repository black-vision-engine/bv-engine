#ifndef UNITTEST_DEFERREDTESTRESULT_H
#define UNITTEST_DEFERREDTESTRESULT_H

#include "Config.h"
#ifndef UNITTEST_NO_DEFERRED_REPORTER

#include "HelperMacros.h"
#include <string>
#include <vector>

#define MAX_FAILURE_STRING 8192


namespace UnitTest
{

   class UNITTEST_LINKAGE DeferredTestFailure
   {
   public:
      DeferredTestFailure();
      DeferredTestFailure(int lineNumber_, const char* failureStr_);

      int lineNumber;
      char failureStr[ MAX_FAILURE_STRING ];
   };

}

UNITTEST_STDVECTOR_LINKAGE(UnitTest::DeferredTestFailure);

namespace UnitTest
{

   class UNITTEST_LINKAGE DeferredTestResult
   {
   public:
      DeferredTestResult();
      DeferredTestResult(char const* suite, char const* test);
      ~DeferredTestResult();

      std::string suiteName;
      std::string testName;
      std::string failureFile;

      typedef std::vector< DeferredTestFailure > FailureVec;
      FailureVec failures;

      float timeElapsed;
      bool failed;
   };

}

#endif
#endif
