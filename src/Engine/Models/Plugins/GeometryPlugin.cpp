#include "GeometryPlugin.h"


namespace bv { namespace model { 

// ***************************** PLUGIN ********************************** 
// *******************************
//
GeometryPlugin::GeometryPlugin  ( model::VertexAttributesChannel * geomChannel )
    : BasePlugin( nullptr )
    , m_geomChannel( geomChannel )
{
}

// *******************************
//
GeometryPlugin::~GeometryPlugin ()
{
    delete m_geomChannel;
}

// *******************************
//
const IVertexAttributesChannel *   GeometryPlugin::GetGeometryChannel          () const
{
    return m_geomChannel;
}

// *******************************
//
void  GeometryPlugin::Update          ( TimeType t )
{
    m_geomChannel->Update( t );
}

// *******************************
//
void  GeometryPlugin::Print           ( std::ostream & out, int tabs ) const
{
}

}
}
