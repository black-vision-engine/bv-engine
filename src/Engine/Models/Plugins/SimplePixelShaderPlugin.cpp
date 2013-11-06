
#include "SimplePixelShaderPlugin.h"
#include "Engine/Models/Plugins/Channels/ShaderChannel.h"

namespace bv { namespace model {

class SimplePixelShaderChannel : public ShaderChannel< model::IPixelShaderChannel, TexturePixelShaderChannelPD >
{

};

SimplePixelShaderPlugin::SimplePixelShaderPlugin          ( const IPlugin * prev, const std::string& shaderPath )
    : m_prev( prev )
    , m_shaderPath( shaderPath )
{
}

SimplePixelShaderPlugin::~SimplePixelShaderPlugin         ()
{
}

const IGeometryChannel*         SimplePixelShaderPlugin::GetGeometryChannel          () const
{
    return m_prev->GetGeometryChannel();
}

const ITransformChannel*        SimplePixelShaderPlugin::GetTransformChannel         () const
{
    return m_prev->GetTransformChannel();
}

const IPixelShaderChannel*      SimplePixelShaderPlugin::GetPixelShaderChannel       () const
{

}

const IVertexShaderChannel*     SimplePixelShaderPlugin::GetVertexShaderChannel      () const
{
    return m_prev->GetVertexShaderChannel();
}

const IGeometryShaderChannel*   SimplePixelShaderPlugin::GetGeometryShaderChannel    () const
{
    return m_prev->GetGeometryShaderChannel();
}

void                            SimplePixelShaderPlugin::Update                      ( float t )
{
    BasePlugin::Update( t );
}

void                            SimplePixelShaderPlugin::Print                       ( std::ostream & out, int tabs = 0 ) const
{
    out << GetName() << std::endl;
}

} // model
} // bv