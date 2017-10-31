#pragma once

#include "Exception.h"


namespace bv
{

class FileNotFoundException : public Exception
{
    static ExceptionType        type;

public:
    static ExceptionType        Type() { return type; }

public:

    virtual std::string         GetReason       () const override { return "File not found."; }
    virtual ExceptionType       GetType         () const override { return Type(); }
    virtual bool                IsDerivedFrom   ( ExceptionType fromType ) const override   { return IsDerivedImpl< FileNotFoundException, Exception >( fromType ); }
};


}	// bv
