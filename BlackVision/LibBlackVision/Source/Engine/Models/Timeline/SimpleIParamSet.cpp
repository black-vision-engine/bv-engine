#include "stdafx.h"

#include "SimpleIParamSet.h"




#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {

// *********************************
//
std::vector< IParameterPtr > &      SimpleIParamSet::GetParameters       ()
{
    return m_parameters;
}

// *********************************
//
IParameterPtr                       SimpleIParamSet::GetParameter        ( const std::string & name )
{
    for( auto param : m_parameters )
    {
        if( param->GetName() == name )
        {
            return param;
        }
    }

    return nullptr;
}

// *********************************
//Name duplicates are allowed, but stored pointers must me be unique
bool                             SimpleIParamSet::AddParameter        ( IParameterPtr param )
{
    if( std::find( m_parameters.begin(), m_parameters.end(), param ) == m_parameters.end() )
    {
        m_parameters.push_back( param );
        
        return true;
    }

    return false;
}

// *********************************
//
bool                                SimpleIParamSet::RemoveParameter     ( IParameterPtr param )
{
    auto it = std::find( m_parameters.begin(), m_parameters.end(), param );

    if( it != m_parameters.end() )
    {
        m_parameters.erase( it );

        return true;
    }

    return false;    
}

// *********************************
//
unsigned int                        SimpleIParamSet::RemoveParameters    ( const std::string & name )
{
    unsigned int erasedElements = 0;

    for( auto elt = m_parameters.begin(); elt != m_parameters.end(); )
    {
        if( name == (*elt)->GetName() )
        {
            elt = m_parameters.erase( elt );

            ++erasedElements;
        }
        else
        {
            ++elt;
        }
    }

    return erasedElements;
}

} //model
} //bv
