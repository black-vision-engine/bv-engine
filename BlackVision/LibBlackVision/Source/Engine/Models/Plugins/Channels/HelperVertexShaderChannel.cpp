#include "stdafx.h"
#include "HelperVertexShaderChannel.h"



#include "Memory/MemoryLeaks.h"



namespace bv { namespace model
{

// ***********************
//
void        HelperVertexShaderChannel::InverseTextureMatrix        ( IPluginParamValModelPtr pluginParamValModel, const std::string & paramName )
{
    auto param = QueryTypedParam< ParamTransformPtr >( pluginParamValModel->GetVertexShaderChannelModel()->GetParameter( paramName ) );
    auto transform = param->Evaluate();
    QueryTypedValue< ValueMat4Ptr >( pluginParamValModel->GetVertexShaderChannelModel()->GetValue( paramName ) )->SetValue( glm::inverse( transform ) );

}

} // model
} // bv

