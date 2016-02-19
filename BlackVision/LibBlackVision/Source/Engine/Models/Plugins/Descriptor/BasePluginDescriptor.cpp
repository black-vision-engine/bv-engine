#include "stdafx.h"

#include "BasePluginDescriptor.h"

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

// *********************************
//
IPluginParamValModelPtr BasePluginDescriptor::CreateModel       ( ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreateDefaultModel( timeEvaluator );
}

// *********************************
//
void BasePluginDescriptor::ModelHelper::CreateVacModel         ()
{
    auto vacModel = std::make_shared< DefaultParamValModel >();
    m_model->SetVertexAttributesChannelModel( vacModel );
    m_lastParamValModel = vacModel;
}

// *********************************
//
void BasePluginDescriptor::ModelHelper::CreateVSModel         ()
{
    auto psModel = std::make_shared< DefaultParamValModel >();
    m_model->SetVertexShaderChannelModel( psModel );
    m_lastParamValModel = psModel;
}

// *********************************
//
void BasePluginDescriptor::ModelHelper::CreatePSModel         ()
{
    auto psModel = std::make_shared< DefaultParamValModel >();
    m_model->SetPixelShaderChannelModel( psModel );
    m_lastParamValModel = psModel;
}

// ***********************
//
void BasePluginDescriptor::ModelHelper::CreatePluginModel   ()
{
    auto psModel = std::make_shared< DefaultParamValModel >();
    m_model->SetPluginModel( psModel );
    m_lastParamValModel = psModel;
}


} //model
} //bv
