#include "stdafx.h"
#include "HelperVertexShaderChannel.h"

namespace bv { namespace model
{

// ***********************
//
void        HelperVertexShaderChannel::InverseTextureMatrix        ( IPluginParamValModelPtr pluginParamValModel, const char * paramName )
{
    auto param = QueryTypedParam< ParamTransformPtr >( pluginParamValModel->GetVertexShaderChannelModel()->GetParameter( paramName ) );
    auto transform = param->Evaluate();
    QueryTypedValue< ValueMat4Ptr >( pluginParamValModel->GetVertexShaderChannelModel()->GetValue( "txMat" ) )->SetValue( glm::inverse( transform ) );

}

} // model
} // bv

