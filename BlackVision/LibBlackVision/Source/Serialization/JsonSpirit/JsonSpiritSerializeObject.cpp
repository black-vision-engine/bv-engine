#include "JsonSpiritSerializeObject.h"
#include "Serialization/BV/BVSerializeContext.h"

#include "Assets/AssetDescsWithUIDs.h"

#include <cassert>
#include <fstream>

namespace bv
{

// ******************************************************************************************
//
JsonSpiritSerializeObject::JsonSpiritSerializeObject()
    : m_context( std::unique_ptr< SerializeContext >( new BVSerializeContext() ) )
{
    m_root = json_spirit::wObject();
    m_currentNode = &m_root;
}
JsonSpiritSerializeObject::~JsonSpiritSerializeObject()
{}

// ***********************
//
void JsonSpiritSerializeObject::Save                ( const std::string& filename, FormatStyle style )
{
    std::wofstream file( filename );
    if( style == FormatStyle::FORMATSTYLE_SPARING )
        json_spirit::write( m_root, file, json_spirit::none );
    else if( style == FormatStyle::FORMATSTYLE_READABLE )
        json_spirit::write( m_root, file, json_spirit::pretty_print );
    file.close();
}

// ***********************
//
std::wstring JsonSpiritSerializeObject::Save                ( FormatStyle style )
{
    if( style == FormatStyle::FORMATSTYLE_SPARING )
        return json_spirit::write( m_root, json_spirit::none );
    else if( style == FormatStyle::FORMATSTYLE_READABLE )
        return json_spirit::write( m_root, json_spirit::pretty_print );
    return L"";
}

// *******************************
//
SerializeContext* JsonSpiritSerializeObject::GetSerializeContext() const
{
    return m_context.get();
}

// ***********************
//
void				JsonSpiritSerializeObject::SetAttribute        ( const std::string& /*name*/, const std::string& /*value*/ )
{    assert( !"This serializer supports only wstrings" );   }

// ***********************
//
std::string			JsonSpiritSerializeObject::GetAttribute        ( const std::string& /*name*/ )
{    assert( !"This serializer supports only wstrings" ); return "";   }

// ***********************
//
void				JsonSpiritSerializeObject::SetAttribute        ( const std::wstring& name, const std::wstring& value )
{
    assert( (*m_currentNode).type() == json_spirit::Value_type::obj_type );

    auto& JsonObject = (*m_currentNode).get_obj();
    JsonObject.push_back( json_spirit::wPair( name, value ) );
}

// ***********************
//
std::wstring		JsonSpiritSerializeObject::GetAttribute        ( const std::wstring& name )
{
    assert( (*m_currentNode).type() == json_spirit::Value_type::obj_type );

    auto& JsonObject = (*m_currentNode).get_obj();
    auto value = FindValue( JsonObject, name );
    if( value == nullptr )
        return L"";

    return value->get_str();
}

// ***********************
//
void                JsonSpiritSerializeObject::EnterChild          ( const std::string& /*name*/ )
{    assert( !"This serializer supports only wstrings" );   }

// ***********************
//
void                JsonSpiritSerializeObject::EnterChild          ( const std::wstring& name )
{
    m_nodeStack.push( m_currentNode );

    auto& object = (*m_currentNode).get_obj();
    auto node = FindValue( object, name );

    if( node == nullptr )
    {// Add node
        json_spirit::wObject newObject;
        object.push_back( json_spirit::wPair( name, newObject ) );

        m_currentNode = &( object.back().value_ );
    }
    else
    {
        assert( node->type() == json_spirit::Value_type::obj_type );
        m_currentNode = node;
    }
}

// ***********************
//
bool                JsonSpiritSerializeObject::ExitChild           ()
{
    if( m_nodeStack.empty() )
        return false;

	m_currentNode = m_nodeStack.top();
	m_nodeStack.pop();

    return true;
}

// ***********************
//
void JsonSpiritSerializeObject::EnterArray          ( const std::string& /*name*/ )
{    assert( !"This serializer supports only wstrings" );   }

// ***********************
//
void JsonSpiritSerializeObject::EnterArray          ( const std::wstring& name )
{
    m_nodeStack.push( m_currentNode );

    auto& object = (*m_currentNode).get_obj();
    auto node = FindValue( object, name );

    if( node == nullptr )
    {// Add node
        json_spirit::wArray newArray;
        json_spirit::wObject newObject;
        newArray.push_back( newObject );

        object.push_back( json_spirit::wPair( name, newArray ) );

        m_currentNode = &( object.back().value_.get_array().back() );
    }
    else
    {
        assert( node->type() == json_spirit::Value_type::array_type );
        auto& array = const_cast<json_spirit::wArray&>( node->get_array() );

        json_spirit::wObject newObject;
        array.push_back( newObject );

        m_currentNode = &( array.back() );
    }
}

// ***********************
//
json_spirit::wObject::value_type::Value_type* JsonSpiritSerializeObject::FindValue ( const json_spirit::wObject& obj, const std::wstring& name )
{
    for( size_t i = 0; i < obj.size(); ++i )
        if( obj[ i ].name_ == name )
        {
            return const_cast<json_spirit::wObject::value_type::Value_type*>( &( obj[ i ].value_ ) );
        }

    return nullptr;
}

} //bv
