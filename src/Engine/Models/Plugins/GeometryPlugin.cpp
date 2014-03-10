#include "GeometryPlugin.h"


namespace bv { namespace model { 

// ***************************** PLUGIN ********************************** 
// *******************************
//
GeometryPlugin::GeometryPlugin  ( model::VertexAttributesChannel * vaChannel )
    : BasePlugin( "dupa", "dupa", nullptr, nullptr )
    , m_vaChannel( vaChannel )
{
}

// *******************************
//
GeometryPlugin::~GeometryPlugin ()
{
    delete m_vaChannel;
}

// *******************************
//
const IVertexAttributesChannel *   GeometryPlugin::GetVertexAttributesChannel          () const
{
    return m_vaChannel;
}

// *******************************
//
void  GeometryPlugin::Update          ( TimeType t )
{
    m_vaChannel->Update( t );
}

// *******************************
//
void  GeometryPlugin::Print           ( std::ostream & out, int tabs ) const
{
}

}
}
