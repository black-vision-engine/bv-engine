#pragma once


#include "Tools/IncludeJSON.h"
#include <fstream>
#include <stack>
#include <cassert>


namespace bv
{

class JsonSerializeObjectImpl
{
private:
	Json::Value					m_root;
	Json::Value*				m_currentNode;
	std::stack<Json::Value*>	m_nodeStack;

public:
	JsonSerializeObjectImpl();
	~JsonSerializeObjectImpl();

	void				Save			( const std::string& filename );
	void				Save			( std::ostream& out );

	void				SetName			( const std::string& name );
	void				SetValue		( const std::string& name, const std::string & value );
	void				SetContent		( const std::string& value );
	void				Pop				();

};

// ***************************************************************************************
//s
inline JsonSerializeObjectImpl::JsonSerializeObjectImpl()
{
	m_currentNode = &m_root;
	m_nodeStack.push( &m_root );
}


inline JsonSerializeObjectImpl::~JsonSerializeObjectImpl()
{}

// ***************************************************************************************
//
inline void JsonSerializeObjectImpl::Save( const std::string& filename )
{
	std::ofstream file;
	file.open( filename, std::ios_base::out );
	Save( file );
	file.close();
}

inline void JsonSerializeObjectImpl::Save( std::ostream& out )
{
	out << m_root;
}

inline void JsonSerializeObjectImpl::SetName( const std::string& name )
{
	m_nodeStack.push( m_currentNode );
	auto size = (*m_currentNode)[ name ].size();
	m_currentNode = &((*m_currentNode)[ name ][ size ]);
}

inline void JsonSerializeObjectImpl::SetValue( const std::string& name, const std::string& value )
{
	(*m_currentNode)[ name ] = value;
}

inline void JsonSerializeObjectImpl::SetContent( const std::string& /*value*/ )
{
	assert( !"Implement me" );
}

inline void JsonSerializeObjectImpl::Pop()
{
	m_currentNode = m_nodeStack.top();
	m_nodeStack.pop();
}

} //bv
