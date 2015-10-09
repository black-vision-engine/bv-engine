#pragma once


#include "Tools/IncludeJSON.h"


namespace bv
{

class JsonDeserializeObjectImpl
{
public:
	Json::Value			m_root;
	Json::Value*		m_currentNode;


	JsonDeserializeObjectImpl( const std::string& jsonString );
	~JsonDeserializeObjectImpl();

	std::string					GetName();
	std::string					GetValue( const std::string& name ) const;
};

// ***************************************************************************************
//
inline JsonDeserializeObjectImpl::JsonDeserializeObjectImpl( const std::string& jsonString )
{
	Json::Reader reader;
	reader.parse( jsonString, m_root );
	m_currentNode = &m_root;
}


inline JsonDeserializeObjectImpl::~JsonDeserializeObjectImpl()
{
}

// ***************************************************************************************
//
inline std::string JsonDeserializeObjectImpl::GetName()
{

}

inline std::string JsonDeserializeObjectImpl::GetValue( const std::string& name ) const
{
	return m_currentNode->get( name, "").asString();
}

} //bv
