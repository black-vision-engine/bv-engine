#include "JsonSpiritSerializeObject.h"

#include <cassert>
#include <fstream>

namespace bv
{

// ******************************************************************************************
//
JsonSpiritSerializeObject::JsonSpiritSerializeObject()
{
    m_root = json_spirit::wObject();
    m_currentNode = &m_root;
}
JsonSpiritSerializeObject::~JsonSpiritSerializeObject()
{}

// ***********************
//
void JsonSpiritSerializeObject::Save                ( const std::string& filename, FormatStyle style  )
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
    auto& JsonObject = (*m_currentNode).get_obj();
    JsonObject.push_back( json_spirit::wPair( name, value ) );
}

// ***********************
//
std::wstring		JsonSpiritSerializeObject::GetAttribute        ( const std::wstring& name )
{
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
        auto nodeType = node->type();
        if( nodeType == json_spirit::Value_type::array_type )
        {// Add element to array
            auto& array = const_cast<json_spirit::wArray&>( node->get_array() );

            json_spirit::wObject newObject;
            array.push_back( newObject );

            m_currentNode = &( array.back() );
        }
        else
        {// Save object, insert array, create new object and add created and saved to the array.
            auto& prevObject = const_cast<json_spirit::wObject&>( node->get_obj() );
            
            json_spirit::wObject newObject;
            json_spirit::wArray newArray;
            newArray.push_back( prevObject );
            newArray.push_back( newObject );

            // delete prevObject from vector
            for( int i = 0; i < object.size(); ++i )
                if( object[i].name_ == name )
                {
                    object.erase( object.begin() + i );
                    break;
                }
                 
           object.push_back( json_spirit::wPair( name, newArray ) );

           m_currentNode = &( object.back().value_.get_array().back() );
        }
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
