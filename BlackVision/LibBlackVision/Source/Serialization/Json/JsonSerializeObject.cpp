#include "JsonSerializeObject.h"

#include <cassert>

namespace bv
{

// ******************************************************************************************
//
JsonSerializeObject::JsonSerializeObject()
{
    m_currentNode = &m_root;
}
JsonSerializeObject::~JsonSerializeObject()
{}

// ***********************
//
void JsonSerializeObject::Save( const std::string& filename )
{
	std::ofstream file;
	file.open( filename, std::ios_base::out );
	Save( file );
	file.close();
}

// ***********************
//
void JsonSerializeObject::Save( std::ostream& out )
{
	out << m_root;
}

// ***********************
//
Json::Value JsonSerializeObject::GetJson()
{
    return m_root;
}

// ***********************
//
void JsonSerializeObject::EnterChild( const std::string& name )
{
	m_nodeStack.push( m_currentNode );

    if( (*m_currentNode)[ name ].isArray() )
    {
        auto size = (*m_currentNode)[ name ].size();
        
        //(*m_currentNode)[ name ][ size ];
        m_currentNode = &(*m_currentNode)[ name ][ size ];
        
    }
    else if( (*m_currentNode)[ name ].isObject() )
    {
        auto tempNode = Json::Value( (*m_currentNode)[ name ] );       //Remember node. (deep copy)
        
        (*m_currentNode)[ name ] = Json::Value( Json::ValueType::arrayValue );
        (*m_currentNode)[ name ][ 0 ] = tempNode;

        m_currentNode = &(*m_currentNode)[ name ][ 1 ];
    }
    else
        m_currentNode = &(*m_currentNode)[ name ];
}

// ***********************
//
void JsonSerializeObject::SetAttribute( const std::string& name, const std::string& value )
{
	(*m_currentNode)[ name ] = value;
}

// ***********************
//
std::string JsonSerializeObject::GetAttribute( const std::string& name )
{
    return (*m_currentNode)[ name ].asString();
}

// ***********************
//
bool JsonSerializeObject::ExitChild()
{
    if( m_nodeStack.empty() )
        return false;

	m_currentNode = m_nodeStack.top();
	m_nodeStack.pop();

    return true;
}

// ***********************
//
void                JsonSerializeObject::SetAttribute        ( const std::wstring& /*name*/, const std::wstring& /*value*/ )
{    assert( !"This serializer doesn't supports wstrings" );    }

// ***********************
//
std::wstring        JsonSerializeObject::GetAttribute        ( const std::wstring& /*name*/ )
{    assert( !"This serializer doesn't supports wstrings" ); return L"";   }

// ***********************
//
void                JsonSerializeObject::EnterChild          ( const std::wstring& /*name*/ )
{    assert( !"This serializer doesn't supports wstrings" );    }

} //bv
