#include "stdafxCore.h"
#include "ExceptionsList.h"



namespace bv
{

ExceptionType ExceptionsList::type = Exception::RegisterType();

// ***********************
//
ExceptionType        ExceptionsList::Type()
{
    return type;
}


// ***********************
//
ExceptionsList::ExceptionsList      ( const std::vector< ExceptionPtr > & list )
    :   m_exceptions( list )
{}

// ***********************
//
ExceptionsList::ExceptionsList      ( std::vector< ExceptionPtr > && list )
    : m_exceptions( std::move( list ) )
{}

// ***********************
//
std::string                 ExceptionsList::GetReason       () const
{
    std::string reason;

    if( m_exceptions.empty() )
        return "";

    for( int i = 0; i < m_exceptions.size() - 1; ++i )
    {
        reason += m_exceptions[ i ]->GetReason() + "\n";
    }

    // Add last exception string without end of line.
    reason += m_exceptions[ m_exceptions.size() - 1 ]->GetReason();

    return reason;
}

// ***********************
//
ExceptionType               ExceptionsList::GetType         () const
{
    return Type();
}

// ***********************
//
void                        ExceptionsList::AddException    ( ExceptionPtr exception )
{
    m_exceptions.push_back( exception );
}

// ***********************
//
void                        ExceptionsList::AddException    ( const std::string & errorMessage )
{
    AddException( std::make_shared< RuntimeException >( errorMessage ) );
}

// ***********************
/// Adds exceptions from ExceptionsList object in parameter. It is valid behavior to pass nullptr as parameter.
void                        ExceptionsList::Merge           ( ExceptionsListPtr list )
{
    if( list && !list->IsEmpty() )
    {
        for( auto exception : list->m_exceptions )
        {
            AddException( exception );
        }
    }
}



}	// bv