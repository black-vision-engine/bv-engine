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
bool JsonSerializeObject::EnterChild( const std::string& name, unsigned int /*index*/ )
{
	m_nodeStack.push( m_currentNode );
	auto size = (*m_currentNode)[ name ].size();
	m_currentNode = &((*m_currentNode)[ name ][ size ]);
    return true;
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
