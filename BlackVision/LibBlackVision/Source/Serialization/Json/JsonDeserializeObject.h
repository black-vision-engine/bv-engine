#pragma once

#include "Tools/IncludeJSON.h"
#include "Serialization/ISerializer.h"

#include <fstream>
#include <stack>

namespace bv
{



class JsonDeserializeObject : public ISerializer
{
private:
	Json::Value					m_root;
	Json::Value*				m_currentNode;
	std::stack<Json::Value*>	m_nodeStack;
public:
	JsonDeserializeObject()
    {
            m_currentNode = nullptr;
    }
    virtual ~JsonDeserializeObject() {}

	void						Load                ( const std::string& jsonString )
        {
	Json::Reader reader;
	reader.parse( jsonString, m_root );
	m_currentNode =&m_root;
	m_nodeStack.push(&m_root );
}
	void						Load                ( std::istream& stream )
        {
	Json::Reader reader;
	reader.parse( stream, m_root );
	m_currentNode = &m_root;
	m_nodeStack.push( &m_root );
}

	void						SetAttribute        ( const std::string& name, const std::string& value )
        {
	(*m_currentNode)[ name ] = value;
}

    std::string                 GetAttribute        ( const std::string& name )
        {
    return (*m_currentNode)[ name ].asString();
}


    bool						EnterChild          ( const std::string& name )
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

	bool						ExitChild           ()
        {
    if( m_nodeStack.empty() )
        return false;

	m_currentNode = m_nodeStack.top();
	m_nodeStack.pop();

    return true;
}




};


} //bv

//#include "Serialization/Json/JsonDeserializeObject.h"


//
//class JsonDeserializeObjectImpl
//{
//public:
//	Json::Value					m_root;
//	Json::Value*				m_currentNode;
//	std::stack<Json::Value*>	m_nodeStack;
//
//
//	JsonDeserializeObjectImpl( const std::string& jsonString );
//	JsonDeserializeObjectImpl( std::istream& stream );
//	~JsonDeserializeObjectImpl();
//
//	std::string					GetName			();
//	std::string					GetValue		( const std::string& name ) const;
//	bool						Push			( const std::string& name );
//	void						Pop				();
//};
//
//// ***************************************************************************************
////
//inline JsonDeserializeObjectImpl::JsonDeserializeObjectImpl( const std::string& jsonString )
//{
//	Json::Reader reader;
//	reader.parse( jsonString, m_root );
//	m_currentNode = &m_root;
//	m_nodeStack.push( &m_root );
//}
//
//inline JsonDeserializeObjectImpl::JsonDeserializeObjectImpl( std::istream& stream )
//{
//	Json::Reader reader;
//	reader.parse( stream, m_root );
//	m_currentNode = &m_root;
//	m_nodeStack.push( &m_root );
//}
//
//
//inline JsonDeserializeObjectImpl::~JsonDeserializeObjectImpl()
//{
//}
//
//// ***************************************************************************************
////
//inline std::string JsonDeserializeObjectImpl::GetName()
//{
//	//m_currentNode = &((*m_currentNode)[ name ]);
//	//m_nodeStack.push( m_currentNode );
//	return "";
//}
//
//inline std::string JsonDeserializeObjectImpl::GetValue( const std::string& name ) const
//{
//	return m_currentNode->get( name, "" ).asString();
//}
//
//inline bool JsonDeserializeObjectImpl::Push( const std::string& name )
//{
//	m_nodeStack.push( m_currentNode );
//	m_currentNode = &((*m_currentNode)[ name ]);
//
//	if( m_currentNode->isNull() )
//		return false;
//	return true;
//}
//
//inline void JsonDeserializeObjectImpl::Pop()
//{
//	m_currentNode = m_nodeStack.top();
//	m_nodeStack.pop();
//}



//
//
//
//class JsonSerializeObjectImpl
//{
//private:
//	Json::Value					m_root;
//	Json::Value*				m_currentNode;
//	std::stack<Json::Value*>	m_nodeStack;
//
//public:
//	JsonSerializeObjectImpl();
//	~JsonSerializeObjectImpl();
//
//	void				Save			( const std::string& filename );
//	void				Save			( std::ostream& out );
//
//	void				SetName			( const std::string& name );
//	void				SetValue		( const std::string& name, const std::string & value );
//	void				SetContent		( const std::string& value );
//	void				Pop				();
//
//};
//
//// ***************************************************************************************
////s
//inline JsonSerializeObjectImpl::JsonSerializeObjectImpl()
//{
//	m_currentNode = &m_root;
//	m_nodeStack.push( &m_root );
//}
//
//
//inline JsonSerializeObjectImpl::~JsonSerializeObjectImpl()
//{}
//
//// ***************************************************************************************
////
//inline void JsonSerializeObjectImpl::Save( const std::string& filename )
//{
//	std::ofstream file;
//	file.open( filename, std::ios_base::out );
//	Save( file );
//	file.close();
//}
//
//inline void JsonSerializeObjectImpl::Save( std::ostream& out )
//{
//	out << m_root;
//}
//
//inline void JsonSerializeObjectImpl::SetName( const std::string& name )
//{
//	m_nodeStack.push( m_currentNode );
//	auto size = (*m_currentNode)[ name ].size();
//	m_currentNode = &((*m_currentNode)[ name ][ size ]);
//}
//
//inline void JsonSerializeObjectImpl::SetValue( const std::string& name, const std::string& value )
//{
//	(*m_currentNode)[ name ] = value;
//}
//
//inline void JsonSerializeObjectImpl::SetContent( const std::string& /*value*/ )
//{
//	assert( !"Implement me" );
//}
//
//inline void JsonSerializeObjectImpl::Pop()
//{
//	m_currentNode = m_nodeStack.top();
//	m_nodeStack.pop();
//}

