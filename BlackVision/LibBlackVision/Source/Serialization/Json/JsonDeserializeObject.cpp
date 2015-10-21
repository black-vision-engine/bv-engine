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
	
    auto& node = (*m_currentNode)[ name ];
    if( node.isArray() )
    {
        // Always push both node's when making an array.
        // Array node can never be the current node.
        m_nodeStack.push( &node );
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
    }

    return true;
}

// ***********************
//
bool JsonDeserializeObject::NextChild           () const
{
    return false;
}



} //bv
