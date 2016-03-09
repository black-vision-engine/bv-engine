#include "stdafx.h"
#include "SceneVariables.h"

#include "Serialization/IDeserializer.h"
#include "Serialization/ISerializer.h"



namespace bv
{

// ***********************
//
SceneVariables::SceneVariables()
{}

// ***********************
//
SceneVariables::~SceneVariables()
{
    for( auto iter : m_varCollection )
    {
        delete iter.second;
    }
}

// ***********************
//
void                SceneVariables::Serialize       ( ISerializer & /*ser*/ ) const
{

}

// ***********************
//
ISerializablePtr    SceneVariables::Create          ( const IDeserializer & /*deser*/ )
{
    return nullptr;
}


// ***********************
//
bool                SceneVariables::AddVariable     ( const std::string & varName, IDeserializer & deser )
{
    auto iter = m_varCollection.find( varName );
    if( iter != m_varCollection.end() )
        return false;

    ISerializer * ser = deser.CreateSerializer();
    m_varCollection[ varName ] = ser;

    return true;
}

// ***********************
//
bool                SceneVariables::DeleteVariable  ( const std::string & varName )
{
    auto iter = m_varCollection.find( varName );
    if( iter == m_varCollection.end() )
        return false;

    delete iter->second;
    m_varCollection.erase( iter );

    return true;
}

// ***********************
//
const ISerializer * SceneVariables::GetVariable     ( const std::string & varName ) const
{
    auto iter = m_varCollection.find( varName );
    if( iter == m_varCollection.end() )
        return nullptr;

    return iter->second;
}

}	// bv