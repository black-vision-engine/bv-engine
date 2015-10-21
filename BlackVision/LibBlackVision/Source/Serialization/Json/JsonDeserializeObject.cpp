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
	m_currentNode = &m_root;
	m_nodeStack.push( &m_root );
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
std::string JsonDeserializeObject::GetAttribute        ( const std::string& name ) const
{
    return (*m_currentNode)[ name ].asString();
}

// ***********************
//
bool JsonDeserializeObject::EnterChild          ( const std::string& name ) const
{
	m_nodeStack.push( m_currentNode );
	
    auto node = (*m_currentNode)[ name ];
    if( node.isArray() )
        m_currentNode = &(node[ 0 ]);
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

    return true;
}

// ***********************
//
bool JsonDeserializeObject::NextChild           () const
{
    return false;
}



} //bv
