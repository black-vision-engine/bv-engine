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
    delete m_pluginModel;
    delete m_transformChannelModel;
    delete m_vertexAttributesChannelModel;
    delete m_pixelShaderChannelModel;
    delete m_vertexShaderChannelModel;
    delete m_geometryShaderChannelModel;
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
IParamValModel *    DefaultPluginParamValModel::GetPluginModel                  ()
{
    return PluginModelImpl();
}

// *******************************
//
IParamValModel *    DefaultPluginParamValModel::GetTransformChannelModel        ()
{
    return TransformChannelModelImpl();
}

// *******************************
//
IParamValModel *    DefaultPluginParamValModel::GetVertexAttributesChannelModel ()
{
    return VertexAttributesChannelModelImpl();
}

// *******************************
//
IParamValModel *    DefaultPluginParamValModel::GetPixelShaderChannelModel      ()
{
    return PixelShaderChannelModelImpl();
}

// *******************************
//
IParamValModel *    DefaultPluginParamValModel::GetVertexShaderChannelModel     ()
{
    return VertexShaderChannelModelImpl();
}

// *******************************
//
IParamValModel *    DefaultPluginParamValModel::GetGeometryShaderChannelModel   ()
{
    return GeometryShaderChannelModelImpl();
}

// *******************************
//
DefaultParamValModel *    DefaultPluginParamValModel::PluginModelImpl           ()
{
    return m_pluginModel;
}

// *******************************
//
DefaultParamValModel *    DefaultPluginParamValModel::TransformChannelModelImpl ()
{
    return m_transformChannelModel;
}

// *******************************
//
DefaultParamValModel *    DefaultPluginParamValModel::VertexAttributesChannelModelImpl()
{
    return m_vertexAttributesChannelModel;
}

// *******************************
//
DefaultParamValModel *    DefaultPluginParamValModel::PixelShaderChannelModelImpl   ()
{
    return m_pixelShaderChannelModel;
}

// *******************************
//
DefaultParamValModel *    DefaultPluginParamValModel::VertexShaderChannelModelImpl  ()
{
    return m_vertexShaderChannelModel;
}

// *******************************
//
DefaultParamValModel *    DefaultPluginParamValModel::GeometryShaderChannelModelImpl()
{
    return m_geometryShaderChannelModel;
}

// *******************************
//
void                DefaultPluginParamValModel::SetPluginModel                  ( DefaultParamValModel * model )
{
    assert( m_pluginModel == nullptr );

    m_pluginModel = model;
}

// *******************************
//
void                DefaultPluginParamValModel::SetTransformChannelModel        ( DefaultParamValModel * model )
{
    assert( m_transformChannelModel == nullptr );

    m_transformChannelModel = model;
}

// *******************************
//
void                DefaultPluginParamValModel::SetVertexAttributesChannelModel ( DefaultParamValModel * model )
{
    assert( m_vertexAttributesChannelModel == nullptr );

    m_vertexAttributesChannelModel = model;
}

// *******************************
//
void                DefaultPluginParamValModel::SetPixelShaderChannelModel      ( DefaultParamValModel * model )
{
    assert( m_pixelShaderChannelModel == nullptr );

    m_pixelShaderChannelModel = model;
}

// *******************************
//
void                DefaultPluginParamValModel::SetVertexShaderChannelModel     ( DefaultParamValModel * model )
{
    assert( m_vertexShaderChannelModel == nullptr );

    m_vertexShaderChannelModel = model;
}

// *******************************
//
void                DefaultPluginParamValModel::SetGeometryShaderChannelModel   ( DefaultParamValModel * model )
{
    assert( m_geometryShaderChannelModel == nullptr );

    m_geometryShaderChannelModel = model;
}

} //model
} //bv
