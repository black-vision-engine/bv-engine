#pragma once

#include <memory>

#include "Engine/Models/Plugins/Interfaces/IParamValModel.h"


namespace bv { namespace model {

class IPluginParamValModel;
DEFINE_PTR_TYPE(IPluginParamValModel)



/**@brief 
@ingroup Model*/
class IPluginParamValModel
{
public:

    virtual     void                Update                          () = 0;

    virtual     IParamValModelPtr   GetPluginModel                  () = 0;

    virtual     IParamValModelPtr   GetTransformChannelModel        () = 0;
    virtual     IParamValModelPtr   GetVertexAttributesChannelModel () = 0;
    virtual     IParamValModelPtr   GetPixelShaderChannelModel      () = 0;
    virtual     IParamValModelPtr   GetVertexShaderChannelModel     () = 0;
    virtual     IParamValModelPtr   GetGeometryShaderChannelModel   () = 0;

	virtual		ITimeEvaluatorPtr	GetTimeEvaluator				() const = 0;

    virtual     ~IPluginParamValModel   () {} 

};

} //model
} //bv
