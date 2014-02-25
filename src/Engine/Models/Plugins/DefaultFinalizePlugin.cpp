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
const std::string &                 DefaultFinalizePlugin::GetName                      () const
{
    return m_name;
}

// *******************************
//
const std::string &                 DefaultFinalizePlugin::GetTypeUid                   () const
{
    return m_uid;
}

// *******************************
//
IPluginParamValModel *              DefaultFinalizePlugin::GetPluginParamValModel       () const
{
    return nullptr;
}

// *******************************
//
IParameter *                        DefaultFinalizePlugin::GetParameter                 ( const std::string & name ) const
{
    return nullptr;
}

// *******************************
//
const bv::IValue *                  DefaultFinalizePlugin::GetValue                    ( const std::string & name ) const
{
    return nullptr;
}

// *******************************
//
const IVertexAttributesChannel *    DefaultFinalizePlugin::GetVertexAttributesChannel   () const
{
    assert( m_prevPlugin );

    return m_prevPlugin->GetVertexAttributesChannel();
}

// *******************************
//
const ITransformChannel *           DefaultFinalizePlugin::GetTransformChannel          () const
{
    assert( m_prevPlugin );

    return m_prevPlugin->GetTransformChannel();
}

// *******************************
//
const IPixelShaderChannel *         DefaultFinalizePlugin::GetPixelShaderChannel        () const
{
    assert( m_prevPlugin );

    return m_prevPlugin->GetPixelShaderChannel();
}

// *******************************
//
const IVertexShaderChannel *        DefaultFinalizePlugin::GetVertexShaderChannel       () const
{
    assert( m_prevPlugin );

    return m_prevPlugin->GetVertexShaderChannel();
}

// *******************************
//
const IGeometryShaderChannel *      DefaultFinalizePlugin::GetGeometryShaderChannel     () const
{
    assert( m_prevPlugin );

    return m_prevPlugin->GetGeometryShaderChannel();
}

// *******************************
//
TextureInfoVec                            DefaultFinalizePlugin::GetTextures            () const // FIXME: use resources
{
    assert( m_prevPlugin );

    return m_prevPlugin->GetTextures();
}

// *******************************
//
bool                                DefaultFinalizePlugin::HasAnimatingTexture          () const
{
    assert( m_prevPlugin );

    return m_prevPlugin->HasAnimatingTexture();
}

// *******************************
//
const ISequenceAnimationSource *    DefaultFinalizePlugin::QuerySequenceAnimationSource () const// FIXME: use generic interface for querying resources that can be
{
    assert( m_prevPlugin );

    return m_prevPlugin->QuerySequenceAnimationSource();
}

// *******************************
//
void                                DefaultFinalizePlugin::Update                       ( TimeType t )
{
    //TODO: implement if there is logic that should be run
}

// *******************************
//
void                                DefaultFinalizePlugin::SetPrevPlugin                ( const IPlugin * plugin )
{
    assert( plugin != nullptr );

    m_prevPlugin = plugin;
}

// *******************************
//
void                                DefaultFinalizePlugin::SetName                      ( const std::string & name )
{
    m_name = name;
}

} //model
}  //bv
