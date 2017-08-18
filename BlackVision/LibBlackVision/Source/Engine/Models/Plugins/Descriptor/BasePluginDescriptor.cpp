#include "stdafx.h"

#include "BasePluginDescriptor.h"



#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {

// *********************************
//
BasePluginDescriptor::BasePluginDescriptor                      ( const std::string & uid, const std::string & defaultName, const std::string & abbrv )
    : m_uid( uid )
    , m_defaultName( defaultName )
    , m_abbrv( abbrv )
{
}

BasePluginDescriptor::BasePluginDescriptor                      ( const std::string & uid, const std::string & defaultName )
    : m_uid( uid )
    , m_defaultName( defaultName )
    , m_abbrv( "" )
{
}

// *********************************
//
const std::string &     BasePluginDescriptor::GetPluginTypeUID  () const
{
    return m_uid;
}

// *********************************
//
const std::string &     BasePluginDescriptor::DefaultPluginName () const
{
    return m_defaultName;
}

// *********************************
//
const std::string &     BasePluginDescriptor::GetPluginTypeAbbrv() const
{
    return m_abbrv;
}

// ***********************
//
std::string             BasePluginDescriptor::QueryGizmoLogicName( const std::string & functionalityName ) const
{
    return m_gizmosMapping.QueryGizmoLogicName( functionalityName );
}

// ***********************
//
std::vector< std::string >  BasePluginDescriptor::ListGizmoFunctions() const
{
    return m_gizmosMapping.ListFunctionalities();
}

// *********************************
//
IPluginParamValModelPtr BasePluginDescriptor::CreateModel       ( ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreateDefaultModel( timeEvaluator );
}

} //model
} //bv
