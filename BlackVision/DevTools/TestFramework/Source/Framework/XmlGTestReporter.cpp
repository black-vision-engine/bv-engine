#include "Config.h"
#ifndef UNITTEST_NO_DEFERRED_REPORTER

#include "XmlGTestReporter.h"

#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>


#include "CoreDEF.h"

using std::string;
using std::ostringstream;
using std::ostream;

namespace {

   void ReplaceChar(string& str, char c, string const& replacement)
   {
      for (size_t pos = str.find(c); pos != string::npos; pos = str.find(c, pos + 1))
         str.replace(pos, 1, replacement);
   }

   string XmlEscape(string const& value)
   {
      string escaped = value;

      ReplaceChar(escaped, '&', "&amp;");
      ReplaceChar(escaped, '<', "&lt;");
      ReplaceChar(escaped, '>', "&gt;");
      ReplaceChar(escaped, '\'', "&apos;");
      ReplaceChar(escaped, '\"', "&quot;");

      return escaped;
   }

   string BuildFailureMessage(string const& file, int line, string const& message)
   {
      ostringstream failureMessage;
      failureMessage << file << "(" << line << ") : " << message;
      return failureMessage.str();
   }


   bool Compare       ( const UnitTest::DeferredTestResult & first, const UnitTest::DeferredTestResult & second )
   {
       return first.suiteName < second.suiteName;
   }

}

namespace UnitTest {

    XmlGTestReporter::XmlGTestReporter(ostream& ostream)
      : m_ostream(ostream)
   {}

   void XmlGTestReporter::ReportSummary(int totalTestCount, int failedTestCount,
                                       int failureCount, float secondsElapsed)
   {
      AddXmlElement(m_ostream, NULL);

      BeginResults(m_ostream, totalTestCount, failedTestCount, failureCount, secondsElapsed);

      DeferredTestResultList & results = GetResults();
      
      // Sort all elements by test suite name.
      std::sort( results.begin(), results.end(), &Compare );

      for( DeferredTestResultList::const_iterator j = results.begin(); j != results.end(); )
      {
          DeferredTestResultList suite;
          std::string suiteName = j->suiteName;
          bv::SizeType numFailures = 0;
          float timeElapsed = 0.0f;

          while( j != results.end() && suiteName == j->suiteName )
          {
              numFailures += j->failures.size();
              timeElapsed += j->timeElapsed;

              suite.push_back( *j );
              ++j;
          }

          BeginSuite( m_ostream, suiteName, (int)suite.size(), (int)numFailures, timeElapsed );

          for( DeferredTestResultList::const_iterator i = suite.begin(); i != suite.end(); ++i )
          {
              BeginTest( m_ostream, *i );

              if( i->failed )
                  AddFailure( m_ostream, *i );

              EndTest( m_ostream, *i );
          }

          EndSuite( m_ostream );
      }

      EndResults(m_ostream);
   }

   void XmlGTestReporter::AddXmlElement(ostream& os, char const* encoding)
   {
      os << "<?xml version=\"1.0\"";

      if (encoding != NULL)
         os << " encoding=\"" << encoding << "\"";

      os << "?>";
   }

   void XmlGTestReporter::BeginResults(std::ostream& os, int totalTestCount, int /*failedTestCount*/,
                                      int failureCount, float secondsElapsed)
   {
      os << "<testsuites"
         << " tests=\"" << totalTestCount << "\""
         //<< " failedtests=\"" << failedTestCount << "\""
         << " failures=\"" << failureCount << "\""
         << " errors=\"0\""                                  // Dummy attribute
         << " time=\"" << secondsElapsed << "\""
         << " name=\"AllTests\""
         << ">";
   }

   void XmlGTestReporter::EndResults(std::ostream& os)
   {
      os << "</testsuites>";
   }

   void XmlGTestReporter::BeginSuite( std::ostream & os, const std::string & name, int totalTestCount, int failureCount, float secondsElapsed )
   {
       os << "<testsuite"
           << " name=\"" << name << "\""
           << " tests=\"" << totalTestCount << "\""
           << " failures=\"" << failureCount << "\""
           << " errors=\"0\""                                  // Dummy attribute
           << " time=\"" << secondsElapsed << "\""
           << ">";
   }

   void XmlGTestReporter::EndSuite( std::ostream & os )
   {
       os << "</testsuite>";
   }

   void XmlGTestReporter::BeginTest(std::ostream& os, DeferredTestResult const& result)
   {
      os << "<testcase"
         << " name=\"" << result.testName << "\""
         << " status=\"run\""
         << " time=\"" << result.timeElapsed << "\""
         << " classname=\"\"";
   }

   void XmlGTestReporter::EndTest(std::ostream& os, DeferredTestResult const& result)
   {
      if (result.failed)
         os << "</testcase>";
      else
         os << "/>";
   }

   void XmlGTestReporter::AddFailure(std::ostream& os, DeferredTestResult const& result)
   {
      os << ">"; // close <test> element

      for (DeferredTestResult::FailureVec::const_iterator it = result.failures.begin();
           it != result.failures.end();
           ++it)
      {
         string const escapedMessage = XmlEscape(std::string(it->failureStr));
         string const message = BuildFailureMessage(result.failureFile, it->lineNumber, escapedMessage);

         os << "<failure" << " message=\"" << message << "\"" << "/>";
      }
   }

}

#endif
