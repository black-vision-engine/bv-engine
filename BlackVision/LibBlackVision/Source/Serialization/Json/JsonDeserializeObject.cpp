#include "Serialization/Json/JsonDeserializeObject.h"

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
void JsonDeserializeObject::Load                ( const std::string& jsonString )
{
	Json::Reader reader;
	reader.parse( jsonString, m_root );
	m_currentNode =&m_root;
	m_nodeStack.push(&m_root );
}

// ***********************
//
void JsonDeserializeObject::Load                ( std::istream& stream )
{
	Json::Reader reader;
	reader.parse( stream, m_root );
	m_currentNode = &m_root;
	m_nodeStack.push( &m_root );
}

// ***********************
//
void JsonDeserializeObject::SetAttribute        ( const std::string& name, const std::string& value )
{
	(*m_currentNode)[ name ] = value;
}

// ***********************
//
std::string JsonDeserializeObject::GetAttribute        ( const std::string& name )
{
    return (*m_currentNode)[ name ].asString();
}

// ***********************
//
bool JsonDeserializeObject::EnterChild          ( const std::string& name )
{
	m_nodeStack.push( m_currentNode );
	m_currentNode = &((*m_currentNode)[ name ]);

	if( m_currentNode->isNull() )
    {
        ExitChild();        // Return to previous node.
		return false;
    }
	return true;
}

// ***********************
//
bool JsonDeserializeObject::ExitChild           ()
{
    if( m_nodeStack.empty() )
        return false;

	m_currentNode = m_nodeStack.top();
	m_nodeStack.pop();

    return true;
}



} //bv
