#include "DefaultPluginParamValModel.h"

#include <cassert>

#include "Engine/Models/Plugins/Interfaces/IParamValModel.h"
#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"


namespace bv { namespace model {

// *******************************
//
DefaultPluginParamValModel::DefaultPluginParamValModel      ()
{
    m_pluginModel                   = nullptr;
    m_transformChannelModel         = nullptr;
    m_vertexAttributesChannelModel  = nullptr;
    m_pixelShaderChannelModel       = nullptr;
    m_vertexShaderChannelModel      = nullptr;
    m_geometryShaderChannelModel    = nullptr;
}

// *******************************
//
DefaultPluginParamValModel::~DefaultPluginParamValModel     ()
{
}

// *******************************
//
void                DefaultPluginParamValModel::Update      ()
{
    if( m_pluginModel )
        m_pluginModel->Update();

    if( m_transformChannelModel )
        m_transformChannelModel->Update();

    if( m_vertexAttributesChannelModel )
        m_vertexAttributesChannelModel->Update();

    if( m_pixelShaderChannelModel )
        m_pixelShaderChannelModel->Update();

    if( m_vertexShaderChannelModel )
        m_vertexShaderChannelModel->Update();

    if( m_geometryShaderChannelModel )
        m_geometryShaderChannelModel->Update();
}

// *******************************
//
IParamValModelPtr   DefaultPluginParamValModel::GetPluginModel                  ()
{
    return PluginModelImpl();
}

// *******************************
//
IParamValModelPtr   DefaultPluginParamValModel::GetTransformChannelModel        ()
{
    return TransformChannelModelImpl();
}

// *******************************
//
IParamValModelPtr   DefaultPluginParamValModel::GetVertexAttributesChannelModel ()
{
    return VertexAttributesChannelModelImpl();
}

// *******************************
//
IParamValModelPtr   DefaultPluginParamValModel::GetPixelShaderChannelModel      ()
{
    return PixelShaderChannelModelImpl();
}

// *******************************
//
IParamValModelPtr   DefaultPluginParamValModel::GetVertexShaderChannelModel     ()
{
    return VertexShaderChannelModelImpl();
}

// *******************************
//
IParamValModelPtr   DefaultPluginParamValModel::GetGeometryShaderChannelModel   ()
{
    return GeometryShaderChannelModelImpl();
}

// *******************************
//
DefaultParamValModelPtr   DefaultPluginParamValModel::PluginModelImpl           ()
{
    return m_pluginModel;
}

// *******************************
//
DefaultParamValModelPtr   DefaultPluginParamValModel::TransformChannelModelImpl ()
{
    return m_transformChannelModel;
}

// *******************************
//
DefaultParamValModelPtr   DefaultPluginParamValModel::VertexAttributesChannelModelImpl()
{
    return m_vertexAttributesChannelModel;
}

// *******************************
//
DefaultParamValModelPtr   DefaultPluginParamValModel::PixelShaderChannelModelImpl   ()
{
    return m_pixelShaderChannelModel;
}

// *******************************
//
DefaultParamValModelPtr   DefaultPluginParamValModel::VertexShaderChannelModelImpl  ()
{
    return m_vertexShaderChannelModel;
}

// *******************************
//
DefaultParamValModelPtr   DefaultPluginParamValModel::GeometryShaderChannelModelImpl()
{
    return m_geometryShaderChannelModel;
}

// *******************************
//
void                DefaultPluginParamValModel::SetPluginModel                  ( DefaultParamValModelPtr model )
{
    assert( m_pluginModel == nullptr );

    m_pluginModel = model;
}

// *******************************
//
void                DefaultPluginParamValModel::SetTransformChannelModel        ( DefaultParamValModelPtr model )
{
    assert( m_transformChannelModel == nullptr );

    m_transformChannelModel = model;
}

// *******************************
//
void                DefaultPluginParamValModel::SetVertexAttributesChannelModel ( DefaultParamValModelPtr model )
{
    assert( m_vertexAttributesChannelModel == nullptr );

    m_vertexAttributesChannelModel = model;
}

// *******************************
//
void                DefaultPluginParamValModel::SetPixelShaderChannelModel      ( DefaultParamValModelPtr model )
{
    assert( m_pixelShaderChannelModel == nullptr );

    m_pixelShaderChannelModel = model;
}

// *******************************
//
void                DefaultPluginParamValModel::SetVertexShaderChannelModel     ( DefaultParamValModelPtr model )
{
    assert( m_vertexShaderChannelModel == nullptr );

    m_vertexShaderChannelModel = model;
}

// *******************************
//
void                DefaultPluginParamValModel::SetGeometryShaderChannelModel   ( DefaultParamValModelPtr model )
{
    assert( m_geometryShaderChannelModel == nullptr );

    m_geometryShaderChannelModel = model;
}

} //model
} //bv
