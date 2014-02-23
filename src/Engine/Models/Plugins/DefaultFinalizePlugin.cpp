#include "DefaultFinalizePlugin.h"

#include <cassert>


namespace bv { namespace model {

std::string DefaultFinalizePlugin::m_uid = "DEFAULT_FINALIZE";

// *******************************
//
DefaultFinalizePlugin::DefaultFinalizePlugin       ()
    : m_prevPlugin( nullptr )
    , m_name( "finalizer" )
{
}

// *******************************
//
DefaultFinalizePlugin::~DefaultFinalizePlugin      ()
{
}
                                    
// *******************************
//
const std::string &                 DefaultFinalizePlugin::GetName                     () const
{
    return m_name;
}

// *******************************
//
const std::string &                 DefaultFinalizePlugin::GetTypeUid                  () const
{
    return m_uid;
}

// *******************************
//
IPluginParamValModel *              DefaultFinalizePlugin::GetPluginParamValModel      ()
{
    return nullptr;
}

// *******************************
//
const IVertexAttributesChannel *    DefaultFinalizePlugin::GetVertexAttributesChannel  () const
{
    assert( m_prevPlugin );
    return nullptr;
}

// *******************************
//
const ITransformChannel *           DefaultFinalizePlugin::GetTransformChannel         () const
{
    assert( m_prevPlugin );
    return nullptr;
}

// *******************************
//
const IPixelShaderChannel *         DefaultFinalizePlugin::GetPixelShaderChannel       () const
{
    assert( m_prevPlugin );
    return nullptr;
}

// *******************************
//
const IVertexShaderChannel *        DefaultFinalizePlugin::GetVertexShaderChannel      () const
{
    assert( m_prevPlugin );
    return nullptr;
}

// *******************************
//
const IGeometryShaderChannel *      DefaultFinalizePlugin::GetGeometryShaderChannel    () const
{
    assert( m_prevPlugin );
    return nullptr;
}

// *******************************
//
Textures                            DefaultFinalizePlugin::GetTextures                 () const // FIXME: use resources
{
    return Textures();
}

// *******************************
//
bool                                DefaultFinalizePlugin::HasAnimatingTexture         () const
{
    return false;   //FIXME: fix it somehow
}

// *******************************
//
ISequenceAnimationSource *          DefaultFinalizePlugin::QuerySequenceAnimationSource() // FIXME: use generic interface for querying resources that can be
{
    return nullptr; //FIXME: fix int somehow
}

// *******************************
//
void                                DefaultFinalizePlugin::Update                      ( TimeType t )
{
    //TODO: implement if there is logic that should be run
}

// *******************************
//
void                                DefaultFinalizePlugin::RegisterPrevPlugin          ( const IPlugin * plugin )
{
    assert( plugin != nullptr );

    m_prevPlugin = plugin;
}

// *******************************
//
void                                DefaultFinalizePlugin::SetName                     ( const std::string & name )
{
    m_name = name;
}

} //model
}  //bv
