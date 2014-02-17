#pragma once

#include "Engine/Models/Plugins/Interfaces/IPluginParamValModel.h"


namespace bv { namespace model {

class DefaultParamValModel;
class IParamValModel;

class DefaultPluginParamValModel : public IPluginParamValModel
{
private:

    DefaultParamValModel *  m_pluginModel;

    DefaultParamValModel *  m_transformChannelModel;
    DefaultParamValModel *  m_vertexAttributesChannelModel;
    DefaultParamValModel *  m_pixelShaderChannelModel;
    DefaultParamValModel *  m_vertexShaderChannelModel;
    DefaultParamValModel *  m_geometryShaderChannelModel;

public:

                                    DefaultPluginParamValModel      ();
                                    ~DefaultPluginParamValModel     ();
    
    virtual void                    Update                          ( TimeType t ) override;

    virtual     IParamValModel *    GetPluginModel                  () override;

    virtual     IParamValModel *    GetTransformChannelModel        () override;
    virtual     IParamValModel *    GetVertexAttributesChannelModel () override;
    virtual     IParamValModel *    GetPixelShaderChannelModel      () override;
    virtual     IParamValModel *    GetVertexShaderChannelModel     () override;
    virtual     IParamValModel *    GetGeometryShaderChannelModel   () override;


    IParamValModel *                PluginModelImpl                 ();

    IParamValModel *                TransformChannelModelImpl       ();
    IParamValModel *                VertexAttributesChannelModelImpl();
    IParamValModel *                PixelShaderChannelModelImpl     ();
    IParamValModel *                VertexShaderChannelModelImpl    ();
    IParamValModel *                GeometryShaderChannelModelImpl  ();

    
    void                            SetPluginModel                  ( DefaultParamValModel * model );

    void                            SetTransformChannelModel        ( DefaultParamValModel * model );
    void                            SetVertexAttributesChannelModel ( DefaultParamValModel * model );
    void                            SetPixelShaderChannelModel      ( DefaultParamValModel * model );
    void                            SetVertexShaderChannelModel     ( DefaultParamValModel * model );
    void                            SetGeometryShaderChannelModel   ( DefaultParamValModel * model );

};

} //model
} //bv
