#include "JsonSpiritDeserializeObject.h"

#include <cassert>
#include <fstream>

namespace bv {

const std::wstring EMPTY_STRING = L"";

// ***********************
//
JsonSpiritDeserializeObject::JsonSpiritDeserializeObject()
{
    m_currentNode = nullptr;
}
JsonSpiritDeserializeObject::~JsonSpiritDeserializeObject()
{}

// ***********************
//
bool JsonSpiritDeserializeObject::LoadFile           ( const std::string& fileName )
{
    std::wifstream file( fileName );
    bool result = json_spirit::read( file, m_root );
    if( result )
        return OnRootInit();
    return result;
}

// ***********************
//
bool JsonSpiritDeserializeObject::LoadWString         ( const std::wstring& jsonString )
{
    bool result = json_spirit::read( jsonString, m_root );
    if( result )
        return OnRootInit();
    return result;
}

// ***********************
//
bool JsonSpiritDeserializeObject::OnRootInit          ()
{
    m_nodeStack.push( &m_root );
    if( m_root.type() == json_spirit::Value_type::obj_type )
        m_currentNode = &m_root;
    else if( m_root.type() == json_spirit::Value_type::array_type )
    {
        auto& nodeArray = m_root.get_array();
        if( nodeArray.empty() )
            return false;

        m_currentNode = &nodeArray[ 0 ];
        m_indexStack.push( 0 );
    }
    return true;
}

// ***********************
//
std::string			JsonSpiritDeserializeObject::GetAttribute        ( const std::string& /*name*/ ) const
{    assert( !"This serializer supports only wstrings" ); return "";   }

// ***********************
//
std::wstring		JsonSpiritDeserializeObject::GetAttribute        ( const std::wstring& name ) const
{
    auto& JsonObject = m_currentNode->get_obj();
    auto value = FindValue( JsonObject, name );
    if( value == nullptr )
        return EMPTY_STRING;

    return value->get_str();
}

// ***********************
//
bool                JsonSpiritDeserializeObject::EnterChild          ( const std::string& /*name*/ ) const
{    assert( !"This serializer supports only wstrings" ); return "";   }

// ***********************
//
bool                JsonSpiritDeserializeObject::EnterChild          ( const std::wstring& name ) const
{
	m_nodeStack.push( m_currentNode );
	
    auto& object = m_currentNode->get_obj();
    auto node = FindValue( object, name );

    if( node != nullptr )
    {
        if( node->type() == json_spirit::Value_type::array_type )
        {
            auto& arrayNode = node->get_array();
            if( arrayNode.empty() )
                return false;
            // Always push both node's when making an array.
            // Array node can never be the current node.
            m_nodeStack.push( node );
            m_indexStack.push( 0 );         //After EnterChild we are always in first array element.
            m_currentNode = &( arrayNode[ 0 ] );
        }
        else
            m_currentNode = node;
    }
    else
        return false;
	return true;
}

// ***********************
//
bool                JsonSpiritDeserializeObject::NextChild           () const
{
    if( m_nodeStack.empty() )
        return false;

    auto arrayNode = m_nodeStack.top();
    if( arrayNode->type() == json_spirit::Value_type::array_type )
    {
        auto& arrayNodeTyped = arrayNode->get_array();
        auto nodeIndex = m_indexStack.top();
        if( ++nodeIndex < arrayNodeTyped.size() )
        {
            m_indexStack.pop();
            m_indexStack.push( nodeIndex );
            m_currentNode = &(arrayNodeTyped[ nodeIndex ]);
            return true;
        }
    }
    return false;
}

// ***********************
//
bool                JsonSpiritDeserializeObject::ExitChild           () const
{
    if( m_nodeStack.empty() )
        return false;

	m_currentNode = m_nodeStack.top();
    m_nodeStack.pop();

    // If we took array node from stack, we have to take next top node too.
    // Array node can never be the current node.
    if( (*m_currentNode).type() == json_spirit::Value_type::array_type )
    {
        if( m_nodeStack.empty() )
            return false;       // Stack corrupted. It's very very bad.

        m_currentNode = m_nodeStack.top();
        m_nodeStack.pop();
        m_indexStack.pop();     // Restore last index, before we entered array.
    }

    return true;
}

// ***********************
//
json_spirit::wObject::value_type::Value_type* JsonSpiritDeserializeObject::FindValue ( const json_spirit::wObject& obj, const std::wstring& name ) const
{
    for( size_t i = 0; i < obj.size(); ++i )
        if( obj[ i ].name_ == name )
        {
            return const_cast<json_spirit::wObject::value_type::Value_type*>( &( obj[ i ].value_ ) );
        }

    return nullptr;
}


} //bv