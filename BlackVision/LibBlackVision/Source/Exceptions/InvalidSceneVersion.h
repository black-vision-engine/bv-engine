#pragma once

#include "Exception.h"

namespace bv
{

class InvalidSceneVersion : public RuntimeException
{
    static ExceptionType        type;

public:
    static ExceptionType        Type() { return type; }

public:
                                InvalidSceneVersion( const std::string & detailedReason );


    virtual ExceptionType       GetType         () const override { return Type(); }
    virtual bool                IsDerivedFrom   ( ExceptionType fromType ) const override   { return IsDerivedImpl< InvalidSceneVersion, Exception >( fromType ); }
};

}	// bv