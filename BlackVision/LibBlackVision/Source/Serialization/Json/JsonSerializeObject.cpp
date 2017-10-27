#include "stdafx.h"

#include "JsonSerializeObject.h"
#include "JsonDeserializeObject.h"
#include "Serialization/BV/BVSerializeContext.h"
#include "Assets/AssetDescsWithUIDs.h"

#include <cassert>



#include "Memory/MemoryLeaks.h"



namespace bv
{

// ***********************
//
JsonSerializeObject::JsonSerializeObject( SerializeContext * context )
    :   m_context( std::make_unique< SerializeContext >( *context ) ),
        m_root( Json::objectValue )
{
    m_currentNode = &m_root;
}

// ***********************
//
JsonSerializeObject::JsonSerializeObject( Json::Value && initValue )
    :   m_context( new SerializeContext() ),
        m_root( initValue )
{
    m_currentNode = &m_root;
}

// ***********************
//
JsonSerializeObject::~JsonSerializeObject()
{}

// ***********************
//
void JsonSerializeObject::Save( const std::string& filename, FormatStyle /*style*/ )
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

// *******************************
//
SerializeContext* JsonSerializeObject::GetSerializeContext() const
{
    return m_context.get();
}

// ***********************
//
Json::Value JsonSerializeObject::GetJson() const
{
    return m_root;
}

// ***********************
//
Json::Value & JsonSerializeObject::StealJson()
{
    m_currentNode = nullptr;
    
    while( !m_nodeStack.empty() )
        m_nodeStack.pop();

    return m_root;
}

// ***********************
//
std::string JsonSerializeObject::GetString()
{
    return m_root.toStyledString();
}

// ***********************
//
void JsonSerializeObject::EnterChild( const std::string& name )
{
	m_nodeStack.push( m_currentNode );

//    assert( !(*m_currentNode)[ name ].isArray() );

    if( m_currentNode->isArray() )
    {
        m_currentNode = &( ( *m_currentNode ).append( Json::ValueType::objectValue ) );
    }
    else
    {
        assert( !m_currentNode->isMember( name ) ); // if this assert fails, we've got two EnterChild's with the same name which leads to destroyed JSON!!!
        (*m_currentNode)[ name ] = Json::ValueType::objectValue;
        m_currentNode = &(*m_currentNode)[ name ];
    }

    
}

// ***********************
//
void JsonSerializeObject::SetAttribute( const std::string& name, const std::string& value )
{
    //assert( (*m_currentNode).isObject() || (*m_currentNode).isArray() );

    if( (*m_currentNode).isObject() )
	    (*m_currentNode)[ name ] = value;
    else if( (*m_currentNode).isArray() )
        (*m_currentNode).append( value );
    else
    {
        assert( false );
    }
}

// ***********************
//
std::string JsonSerializeObject::GetAttribute( const std::string& name )
{
    assert( (*m_currentNode).isObject() );

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
void                JsonSerializeObject::EnterArray          ( const std::string& name )
{
	m_nodeStack.push( m_currentNode );

    bool isArray = m_currentNode->isArray();
    if( isArray )
    {
        m_currentNode = &( ( *m_currentNode ).append( Json::ValueType::arrayValue ) );
    }
    else if( !isArray && m_currentNode->isMember( name ) )
    {
        assert( false );
    }
    else
    {
        ( *m_currentNode )[ name ] = Json::ValueType::arrayValue;
        m_currentNode = &( ( *m_currentNode )[ name ] );
    }
}


// ***********************
//
bool                JsonSerializeObject::AttachBranch        ( const std::string & name, const ISerializer * ser )
{
    assert( typeid( *ser ) == typeid( *this ) );
    auto typedSer = static_cast< const JsonSerializeObject * >( ser );

    assert( m_currentNode->isObject() );
    assert( (*m_currentNode)[ name ] == Json::Value( Json::nullValue ) );

    (*m_currentNode)[ name ] = typedSer->GetJson();

    return true;
}

// ***********************
//
bool                JsonSerializeObject::AttachBranch        ( const std::string & name, const IDeserializer * ser )
{
    assert( typeid( *ser ) == typeid( *this ) );
    auto typedSer = static_cast< const JsonDeserializeObject * >( ser );

    assert( m_currentNode->isObject() );
    assert( (*m_currentNode)[ name ] == Json::Value( Json::nullValue ) );

    (*m_currentNode)[ name ] = typedSer->GetJson();

    return true;
}

// ***********************
//
void                JsonSerializeObject::EnterArray          ( const std::wstring& /*name*/ )
{    assert( !"This serializer doesn't supports wstrings" );    }

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
