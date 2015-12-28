#include "NodeEffect.h"

#include <cassert>


namespace bv {


// *********************************
//
NodeEffect::NodeEffect  ( NodeEffect::Type type )
    : m_type( type )
{
}

// *********************************
//
NodeEffect::~NodeEffect ()
{
}

// *********************************
//
SizeType    NodeEffect::GetNumValues() const
{
    return m_modelValues.size();
}

// *********************************
//
IValuePtr   NodeEffect::GetValue    ( unsigned int i )
{
    assert( i < GetNumValues() );

    return m_modelValues[ i ];
}

// *********************************
//
IValuePtr   NodeEffect::GetValue    ( const std::string & name )
{
    for( auto & v : m_modelValues )
    {
        if( v->GetName() == name )
        {
            return v;
        }
    }

    return nullptr;
}

// *********************************
//
NodeEffect::Type    NodeEffect::GetType     () const
{
    return m_type;
}

// *********************************
//
std::vector< IValuePtr > &  NodeEffect::AccessValues()
{
    return m_modelValues;
}

} //bv
