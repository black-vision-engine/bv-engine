#include "Serialization/Json/JsonDeserializeObject.h"

#include <fstream>
#include <cassert>

namespace bv
{


// ******************************************************************************************
//

JsonDeserializeObject::JsonDeserializeObject()
{
    m_currentNode = nullptr;
}
JsonDeserializeObject::~JsonDeserializeObject()
{}

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
	m_nodeStack.push( m_currentNode );
	
    auto& node = (*m_currentNode)[ name ];
    if( node.isArray() )
    {
        if( node.size() == 0 )
            return false;
        // Always push both node's when making an array.
        // Array node can never be the current node.
        m_nodeStack.push( &node );
        m_indexStack.push( 0 );         //After EnterChild we are always in first array element.
        m_currentNode = &( node[ 0 ] );
    }
    else
        m_currentNode = &node;

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
    if( m_nodeStack.empty() )
        return false;

	m_currentNode = m_nodeStack.top();
    m_nodeStack.pop();

    // If we took array node from stack, we have to take next top node too.
    // Array node can never be the current node.
    if( (*m_currentNode).isArray() )
    {
        if( m_nodeStack.empty() )
            return false;       // Stack corrupted. It's very very bad.

        m_currentNode = m_nodeStack.top();
        m_nodeStack.pop();
        m_indexStack.pop();     // Restore last index, before we entered array.
    }

    return true;
}

// ***********************
//
bool JsonDeserializeObject::NextChild           () const
{
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
