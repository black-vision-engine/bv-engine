#pragma once

#include <stack>
#include "Tools/IncludeJSON.h"


namespace bv
{

class JsonDeserializeObjectImpl
{
public:
	Json::Value					m_root;
	Json::Value*				m_currentNode;
	std::stack<Json::Value*>	m_nodeStack;


	JsonDeserializeObjectImpl( const std::string& jsonString );
	JsonDeserializeObjectImpl( std::istream& stream );
	~JsonDeserializeObjectImpl();

	std::string					GetName			();
	std::string					GetValue		( const std::string& name ) const;
	bool						Push			( const std::string& name );
	void						Pop				();
};

// ***************************************************************************************
//
inline JsonDeserializeObjectImpl::JsonDeserializeObjectImpl( const std::string& jsonString )
{
	Json::Reader reader;
	reader.parse( jsonString, m_root );
	m_currentNode = &m_root;
	m_nodeStack.push( &m_root );
}

inline JsonDeserializeObjectImpl::JsonDeserializeObjectImpl( std::istream& stream )
{
	Json::Reader reader;
	reader.parse( stream, m_root );
	m_currentNode = &m_root;
	m_nodeStack.push( &m_root );
}


inline JsonDeserializeObjectImpl::~JsonDeserializeObjectImpl()
{
}

// ***************************************************************************************
//
inline std::string JsonDeserializeObjectImpl::GetName()
{
	//m_currentNode = &((*m_currentNode)[ name ]);
	//m_nodeStack.push( m_currentNode );
	return "";
}

inline std::string JsonDeserializeObjectImpl::GetValue( const std::string& name ) const
{
	return m_currentNode->get( name, "" ).asString();
}

inline bool JsonDeserializeObjectImpl::Push( const std::string& name )
{
	m_nodeStack.push( m_currentNode );
	m_currentNode = &((*m_currentNode)[ name ]);

	if( m_currentNode->isNull() )
		return false;
	return true;
}

inline void JsonDeserializeObjectImpl::Pop()
{
	m_currentNode = m_nodeStack.top();
	m_nodeStack.pop();
}

} //bv
