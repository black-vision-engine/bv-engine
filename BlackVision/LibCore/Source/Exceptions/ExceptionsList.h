#pragma once

#include "Exception.h"

#include <vector>


namespace bv
{

class ExceptionsList;
DEFINE_PTR_TYPE( ExceptionsList )


// ***********************
//
class ExceptionsList : public Exception
{
    static ExceptionType        type;
public:
    static ExceptionType        Type();

protected:

    std::vector< ExceptionPtr >     m_exceptions;

public:

    ExceptionsList      () {}
    ExceptionsList      ( const std::vector< ExceptionPtr > & list );
    ExceptionsList      ( std::vector< ExceptionPtr > && list );

    virtual std::string         GetReason       () const override;
    virtual ExceptionType       GetType         () const override;
    virtual bool                IsDerivedFrom   ( ExceptionType fromType ) const override { return IsDerivedImpl< RuntimeException, Exception >( fromType ); }

public:

    void                        AddException        ( ExceptionPtr exception );
    void                        AddException        ( const std::string & errorMessage );
    void                        Merge               ( ExceptionsListPtr list );

    bool                        IsEmpty             () { return m_exceptions.empty(); }
};




}	// bv


