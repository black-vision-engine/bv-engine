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

// *********************************
//
IPluginParamValModelPtr BasePluginDescriptor::CreateModel       ( ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreateDefaultModel( timeEvaluator );
}

// *********************************
//
BasePluginDescriptor::ModelHelper::ModelHelper(  ITimeEvaluatorPtr te, DefaultPluginParamValModelPtr model )
    : m_lastTimeEvaluator( te )
    , m_model( model )
{}

// *********************************
//
BasePluginDescriptor::ModelHelper::ModelHelper(  ITimeEvaluatorPtr te )
    : m_lastTimeEvaluator( te )
    , m_model( std::make_shared< DefaultPluginParamValModel >( te ) )
{}

// *********************************
//
void BasePluginDescriptor::ModelHelper::SetOrCreateVacModel      ()
{
    DefaultParamValModelPtr vacModel = std::static_pointer_cast< DefaultParamValModel >( m_model->GetVertexAttributesChannelModel() );

    if( !vacModel )
    {
        vacModel = std::make_shared< DefaultParamValModel >();
        m_model->SetVertexAttributesChannelModel( vacModel );
    }

    m_lastParamValModel = vacModel;
}

// *********************************
//
void BasePluginDescriptor::ModelHelper::SetOrCreateVSModel       ()
{
    DefaultParamValModelPtr vsModel = std::static_pointer_cast< DefaultParamValModel >( m_model->GetVertexShaderChannelModel() );

    if( !vsModel )
    {
        vsModel = std::make_shared< DefaultParamValModel >();
        m_model->SetVertexShaderChannelModel( vsModel );
    }

    m_lastParamValModel = vsModel;
}

// ***********************
//
void BasePluginDescriptor::ModelHelper::SetOrCreateGSModel       ()
{
    assert( false );
}

// *********************************
//
void BasePluginDescriptor::ModelHelper::SetOrCreatePSModel       ()
{
    DefaultParamValModelPtr psModel = std::static_pointer_cast< DefaultParamValModel >( m_model->GetPixelShaderChannelModel() );

    if( !psModel )
    {
        psModel = std::make_shared< DefaultParamValModel >();
        m_model->SetPixelShaderChannelModel( psModel );
    }

    m_lastParamValModel = psModel;
}

// ***********************
//
void BasePluginDescriptor::ModelHelper::SetOrCreatePluginModel   ()
{
    DefaultParamValModelPtr plModel = std::static_pointer_cast< DefaultParamValModel >( m_model->GetPluginModel() );

    if( !plModel )
    {
        plModel = std::make_shared< DefaultParamValModel >();
        m_model->SetPluginModel( plModel );
    }

    m_lastParamValModel = plModel;
}


} //model
} //bv
