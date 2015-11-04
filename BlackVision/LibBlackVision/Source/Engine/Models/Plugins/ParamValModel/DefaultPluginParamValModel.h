#pragma once

#include <memory>

#include "Engine/Models/Plugins/Interfaces/IPluginParamValModel.h"
#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"

namespace bv { namespace model {

class IParamValModel;

class DefaultPluginParamValModel : public IPluginParamValModel
{
private:
	ITimeEvaluatorPtr		m_timeEvaluator;

    DefaultParamValModelPtr m_pluginModel;

    DefaultParamValModelPtr m_transformChannelModel;
    DefaultParamValModelPtr m_vertexAttributesChannelModel;
    DefaultParamValModelPtr m_pixelShaderChannelModel;
    DefaultParamValModelPtr m_vertexShaderChannelModel;
    DefaultParamValModelPtr m_geometryShaderChannelModel;

public:

                                    DefaultPluginParamValModel      ( ITimeEvaluatorPtr timeEvaluator );
                                    ~DefaultPluginParamValModel     ();
    
    virtual void                    Update                          () override;

    virtual     IParamValModelPtr   GetPluginModel                  () override;

    virtual     IParamValModelPtr   GetTransformChannelModel        () override;
    virtual     IParamValModelPtr   GetVertexAttributesChannelModel () override;
    virtual     IParamValModelPtr   GetPixelShaderChannelModel      () override;
    virtual     IParamValModelPtr   GetVertexShaderChannelModel     () override;
    virtual     IParamValModelPtr   GetGeometryShaderChannelModel   () override;

	virtual		ITimeEvaluatorPtr	GetTimeEvaluator				() const override;

    DefaultParamValModelPtr        PluginModelImpl                 ();

    DefaultParamValModelPtr        TransformChannelModelImpl       ();
    DefaultParamValModelPtr        VertexAttributesChannelModelImpl();
    DefaultParamValModelPtr        PixelShaderChannelModelImpl     ();
    DefaultParamValModelPtr        VertexShaderChannelModelImpl    ();
    DefaultParamValModelPtr        GeometryShaderChannelModelImpl  ();

    
    void                            SetPluginModel                  ( DefaultParamValModelPtr model );

    void                            SetTransformChannelModel        ( DefaultParamValModelPtr model );
    void                            SetVertexAttributesChannelModel ( DefaultParamValModelPtr model );
    void                            SetPixelShaderChannelModel      ( DefaultParamValModelPtr model );
    void                            SetVertexShaderChannelModel     ( DefaultParamValModelPtr model );
    void                            SetGeometryShaderChannelModel   ( DefaultParamValModelPtr model );

};

DEFINE_PTR_TYPE(DefaultPluginParamValModel)

} //model
} //bv
