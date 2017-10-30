#include "stdafx.h"

#include "ModelHelper.h"

namespace bv { namespace model {

// *********************************
//
ModelHelper::ModelHelper(  ITimeEvaluatorPtr te, DefaultPluginParamValModelPtr model )
    : m_lastTimeEvaluator( te )
    , m_model( model )
{}

// *********************************
//
ModelHelper::ModelHelper(  ITimeEvaluatorPtr te )
    : m_lastTimeEvaluator( te )
    , m_model( std::make_shared< DefaultPluginParamValModel >( te ) )
{}

// *********************************
//
void ModelHelper::SetOrCreateVacModel      ()
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
void ModelHelper::SetOrCreateVSModel       ()
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
void ModelHelper::SetOrCreateGSModel       ()
{
    DefaultParamValModelPtr gsModel = std::static_pointer_cast< DefaultParamValModel >( m_model->GetGeometryShaderChannelModel() );

    if( !gsModel )
    {
        gsModel = std::make_shared< DefaultParamValModel >();
        m_model->SetGeometryShaderChannelModel( gsModel );
    }

    m_lastParamValModel = gsModel;
}

// *********************************
//
void ModelHelper::SetOrCreatePSModel       ()
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
void ModelHelper::SetOrCreatePluginModel   ()
{
    DefaultParamValModelPtr plModel = std::static_pointer_cast< DefaultParamValModel >( m_model->GetPluginModel() );

    if( !plModel )
    {
        plModel = std::make_shared< DefaultParamValModel >();
        m_model->SetPluginModel( plModel );
    }

    m_lastParamValModel = plModel;
}

} // model
} // bv