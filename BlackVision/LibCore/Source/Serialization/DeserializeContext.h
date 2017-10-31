#pragma once

#include "Exception.h"

namespace bv {

class DeserializeContext
{
    Exceptions                          m_warnings;

public:

    virtual                             ~DeserializeContext() {}

    Exceptions                          GetWarnings             ();
    void                                AddWarning              ( ExceptionPtr warning );

};

} //bv
