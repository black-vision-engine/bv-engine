#include "stdafx.h"

#include "JsonDeserializeObject.h"

#include "../BV/BVDeserializeContext.h"
#include "Assets/AssetDescsWithUIDs.h"

#include <fstream>
#include <cassert>

namespace bv
{


// ******************************************************************************************
//

JsonDeserializeObject::JsonDeserializeObject()
    : m_context( std::unique_ptr< DeserializeContext >( new BVDeserializeContext( nullptr, nullptr ) ) )
{
    m_currentNode = nullptr;
}
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
void JsonDeserializeObject::Load                ( const std::string& jsonString )
{
	Json::Reader reader;
	reader.parse( jsonString, m_root );
    OnRootInit();
}

// ***********************
//
void JsonDeserializeObject::Load                ( std::istream& stream )
{
	Json::Reader reader;
	reader.parse( stream, m_root );
    OnRootInit();
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
std::string JsonDeserializeObject::GetAttribute        ( const std::string& name ) const
{
    return (*m_currentNode)[ name ].asString();
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
        if( m_currentNode->size() == 0 )
            return false;

        m_nodeStack.push( m_currentNode );

        m_indexStack.push( 0 );                     //After EnterChild we are always in first array element.
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

std::wstring        JsonDeserializeObject::GetAttribute        ( const std::wstring& /*name*/ ) const
{    assert( !"This serializer doesn't supports wstrings" ); return L"";    }

std::wstring        JsonDeserializeObject::GetParentAttribute	( const std::wstring& /*parentName*/, const std::wstring& /*attName*/ ) const
{    assert( !"This serializer doesn't supports wstrings" ); return L"";    }

bool                JsonDeserializeObject::EnterChild          ( const std::wstring& /*name*/ ) const
{    assert( !"This serializer doesn't supports wstrings" ); return false;   }

} //bv
