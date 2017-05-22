#include "stdafx.h"

#include "JsonDeserializeObject.h"
#include "JsonSerializeObject.h"

#include "../BV/BVDeserializeContext.h"

#include "Assets/AssetDescsWithUIDs.h"

#include "Engine/Models/Timeline/TimelineManager.h"

#include <fstream>
#include <cassert>



#include "Memory/MemoryLeaks.h"



namespace bv
{


// ******************************************************************************************
//

JsonDeserializeObject::JsonDeserializeObject()
    : m_context( std::unique_ptr< DeserializeContext >( new BVDeserializeContext( nullptr, nullptr ) ) )
    , m_currentNode( nullptr )
{}

// ***********************
//
JsonDeserializeObject::JsonDeserializeObject    ( JsonSerializeObject && serializer )
    :   m_root( Json::nullValue )
    ,   m_currentNode( nullptr )
    ,   m_context( std::unique_ptr< DeserializeContext >( new BVDeserializeContext( nullptr, nullptr ) ) )
{
    auto & steal = serializer.StealJson();
    m_root.swap( steal );

    OnRootInit();
}

// ***********************
//
JsonDeserializeObject::JsonDeserializeObject       ( Json::Value && initValue )
    :   m_root( initValue )
    ,   m_currentNode( nullptr )
    ,   m_context( std::unique_ptr< DeserializeContext >( new BVDeserializeContext( nullptr, nullptr ) ) )
{
    OnRootInit();
}

// ***********************
//
JsonDeserializeObject::~JsonDeserializeObject()
{}

// *******************************
//
DeserializeContext* JsonDeserializeObject::GetDeserializeContext() const
{
    return m_context.get();
}

// ***********************
//
bool JsonDeserializeObject::LoadFile        ( const std::string& fileName )
{
    std::ifstream file;
    file.open( fileName, std::ios_base::out );
    if( !file.fail() )
    {
        Load( file );
        return true;
    }
    return false;
}

// ***********************
//
bool JsonDeserializeObject::Load                ( const std::string& jsonString )
{
    Json::Reader reader;
    if( reader.parse( jsonString, m_root ) )
    {
        OnRootInit();
        return true;;
    }
    return false;
}

// ***********************
//
bool JsonDeserializeObject::Load                ( std::istream& stream )
{
    Json::Reader reader;
    if( reader.parse( stream, m_root ) )
    {
        OnRootInit();
        return true;
    }
    return false;
}

// ***********************
//
void JsonDeserializeObject::OnRootInit          ()
{
    m_nodeStack.push( &m_root );

    if( m_root.isObject() )
        m_currentNode = &m_root;
    else if( m_root.isArray() && m_root.size() )
    {
        m_currentNode = &m_root[ 0 ];
        m_indexStack.push( 0 );
    }
    else
        m_root = Json::ValueType::nullValue;

}

// ***********************
//
Json::Value JsonDeserializeObject::GetJson() const
{
    return m_root;
}


// ***********************
//
bool JsonDeserializeObject::HasAttribute                ( const std::string& name ) const
{
    auto & attribute = ( *m_currentNode )[ name ];

    return !attribute.isNull();
}

// ***********************
//
std::string JsonDeserializeObject::GetAttribute        ( const std::string& name ) const
{
    auto & attribute = (*m_currentNode)[ name ];
    
    if( attribute.isNull() || attribute.isObject() || attribute.isArray() )
        return "";

    return attribute.asString();
}

// ***********************
//
std::string JsonDeserializeObject::GetParentAttribute        ( const std::string& /*parentName*/, const std::string& /*attName*/ ) const
{
    assert( !"FIXME" ); return "";
}


// ***********************
//
bool JsonDeserializeObject::EnterChild          ( const std::string& name ) const
{
    if( m_currentNode->isArray() )
    {
        // After EnterChild we are always in first array element.
        // Even with array is empty we should push 0.
        m_indexStack.push( 0 );

        if( m_currentNode->size() == 0 )
            return false;

        m_nodeStack.push( m_currentNode );
        m_currentNode = &( (*m_currentNode)[ 0 ] );
    }
    else
    {
        m_nodeStack.push( m_currentNode );

        auto& node = (*m_currentNode)[ name ];
        m_currentNode = &node;
    }

    if( m_currentNode->isNull() )
    {
        ExitChild();        // Return to previous node. Always true.
        return false;
    }
    return true;
}

// ***********************
//
bool JsonDeserializeObject::ExitChild           () const
{
    assert( !m_nodeStack.empty() );

    if( m_nodeStack.empty() )
        return false;

    if( m_currentNode->isArray() )
        m_indexStack.pop();         // Restore last index, before we entered array.

    m_currentNode = m_nodeStack.top();
    m_nodeStack.pop();

    return true;
}

// ***********************
//
bool JsonDeserializeObject::NextChild           () const
{
    assert( !m_nodeStack.empty() );

    if( m_nodeStack.empty() )
        return false;

    auto arrayNode = m_nodeStack.top();
    if( arrayNode->isArray() )
    {
        auto nodeIndex = m_indexStack.top();
        if( ++nodeIndex < arrayNode->size() )
        {
            m_indexStack.pop();
            m_indexStack.push( nodeIndex );
            m_currentNode = &(*arrayNode)[ nodeIndex ];
            return true;
        }
    }
    return false;
}

// ***********************
//
IDeserializer*      JsonDeserializeObject::DetachBranch        ( const std::string & name )
{
    assert( m_currentNode->isObject() );

    auto& branch = (*m_currentNode)[ name ];
    
    // Uncomment this asert in future
    //assert( branch.isObject() || branch.isNull() );

    if( branch.isNull() || !branch.isObject() )
        return nullptr;

    Json::Value nullValue( Json::nullValue );
    branch.swap( nullValue );

    return new JsonDeserializeObject( std::move( nullValue ) );
}

// ***********************
//
bool                JsonDeserializeObject::AttachBranch        ( const std::string & name, ISerializer * ser )
{
    assert( typeid( *ser ) == typeid( *this ) );
    auto typedSer = static_cast< JsonSerializeObject * >( ser );

    assert( m_currentNode->isObject() );
    assert( (*m_currentNode)[ name ] == Json::Value( Json::nullValue ) );

    (*m_currentNode)[ name ] = typedSer->GetJson();

    return true;
}

// ***********************
//
ISerializer *       JsonDeserializeObject::CreateSerializer    () const
{
    Json::Value copyRoot = m_root;
    JsonSerializeObject * newSer = new JsonSerializeObject( std::move( copyRoot ) );
    
    return newSer;
}

// ***********************
//
bool		        JsonDeserializeObject::HasAttribute        ( const std::wstring & ) const
{ assert( !"This serializer doesn't supports wstrings" ); return false; }


std::wstring        JsonDeserializeObject::GetAttribute        ( const std::wstring& /*name*/ ) const
{    assert( !"This serializer doesn't supports wstrings" ); return L"";    }

std::wstring        JsonDeserializeObject::GetParentAttribute	( const std::wstring& /*parentName*/, const std::wstring& /*attName*/ ) const
{    assert( !"This serializer doesn't supports wstrings" ); return L"";    }

bool                JsonDeserializeObject::EnterChild          ( const std::wstring& /*name*/ ) const
{    assert( !"This serializer doesn't supports wstrings" ); return false;   }

} //bv
