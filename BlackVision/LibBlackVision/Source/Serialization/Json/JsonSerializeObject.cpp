#include "JsonSerializeObject.h"


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

void JsonSerializeObject::Save( std::ostream& out )
{
	out << m_root;
}

// ***********************
//
void JsonSerializeObject::EnterChild( const std::string& name )
{
	m_nodeStack.push( m_currentNode );

    if( (*m_currentNode)[ name ].isArray() )
    {
        auto size = (*m_currentNode)[ name ].size();
        m_currentNode = &((*m_currentNode)[ name ][ size ]);
    }
    else if( (*m_currentNode).isObject() )
    {
        auto tempNode = Json::Value( (*m_currentNode)[ name ] );       //Remember node.
        (*m_currentNode)[ name ][ 0 ] = tempNode;
        m_currentNode = &(*m_currentNode)[ name ][ 1 ];
    }
    else
        m_currentNode = &(*m_currentNode)[ name ];


	//auto size = (*m_currentNode)[ name ].size();
 //   if( size == 0 )
 //       m_currentNode = &((*m_currentNode)[ name ]);
 //   else if( size == 1 )
 //   {
 //       auto tempNode = (*m_currentNode)[ name ][ 0 ];  // Remember first element.

 //   }
 //   else
 //       m_currentNode = &((*m_currentNode)[ name ][ size ]);
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


} //bv
