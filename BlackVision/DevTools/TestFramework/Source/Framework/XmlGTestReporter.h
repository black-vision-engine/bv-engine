#ifndef UNITTEST_XMLTESTREPORTER_H
#define UNITTEST_XMLTESTREPORTER_H

#include "Config.h"
#ifndef UNITTEST_NO_DEFERRED_REPORTER

#include "DeferredTestReporter.h"

#include <iosfwd>

namespace UnitTest
{

   class UNITTEST_LINKAGE XmlGTestReporter : public DeferredTestReporter
   {
   public:
      explicit XmlGTestReporter(std::ostream& ostream);

      virtual void ReportSummary(int totalTestCount, int failedTestCount, int failureCount, float secondsElapsed);

   private:
       XmlGTestReporter( XmlGTestReporter const&);
      XmlGTestReporter& operator=( XmlGTestReporter const&);

      void AddXmlElement(std::ostream& os, char const* encoding);
      void BeginResults(std::ostream& os, int totalTestCount, int failedTestCount, int failureCount, float secondsElapsed);
      void EndResults(std::ostream& os);

      void BeginSuite( std::ostream& os, const std::string & name, int totalTestCount, int failureCount, float secondsElapsed );
      void EndSuite( std::ostream& os );

      void BeginTest(std::ostream& os, DeferredTestResult const& result);
      void AddFailure(std::ostream& os, DeferredTestResult const& result);
      void EndTest(std::ostream& os, DeferredTestResult const& result);

      std::ostream& m_ostream;
   };

}

#endif
#endif
