#include "stdafxCore.h"

#include "DeserializeContext.h"

#include "UseLoggerLibCoreModule.h"

namespace bv
{

// ***********************
//
Exceptions                          DeserializeContext::GetWarnings             () {
    return m_warnings;
}

// ***********************
//
void                                DeserializeContext::AddWarning              ( ExceptionPtr warning ) {
    m_warnings.push_back( warning );
    LOG_MESSAGE( SeverityLevel::warning ) << warning->GetReason();
}


}	// bv