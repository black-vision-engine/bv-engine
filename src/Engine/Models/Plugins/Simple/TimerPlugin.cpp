#include "TimerPlugin.h"

namespace bv { namespace model {

////////////////////////////
//
TimerPlugin::TimerPlugin( const ParamFloat& timeParam )
    : BasePlugin( nullptr )
    , m_timeParam( timeParam )
{
    
}

////////////////////////////
//
TimerPlugin*                        TimerPlugin::Create     ( const ParamFloat& timeParam )
{
    return new TimerPlugin( timeParam );
}

////////////////////////////
//
const IVertexAttributesChannel *    TimerPlugin::GetGeometryChannel          () const
{
    return m_vertexAttributeChannel.get();
}

////////////////////////////
//
Textures                            TimerPlugin::GetTextures                 () const
{
    return m_textures;
}

////////////////////////////
//
void                                TimerPlugin::Update                      ( TimeType t )
{
    m_currentTime = m_timeParam.Evaluate( t );
}

////////////////////////////
//
void                                TimerPlugin::Print                       ( std::ostream & out, int tabs ) const
{
}



} // model
} // bv