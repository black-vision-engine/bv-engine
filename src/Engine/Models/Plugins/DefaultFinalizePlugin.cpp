#include "DefaultFinalizePlugin.h"

#include <cassert>

#include "Engine/Models/Plugins/Interfaces/IPixelShaderChannel.h"


namespace bv { namespace model {

std::string DefaultFinalizePlugin::m_uid = "DEFAULT_FINALIZE";

// *******************************
//
DefaultFinalizePlugin::DefaultFinalizePlugin       ()
    : m_prevPlugin( nullptr )
    , m_name( "finalizer" )
    , m_finalizePSC( nullptr )
    , m_finalizeVSC( nullptr )
    , m_finalizeGSC( nullptr )

{
    m_defaultVSChannel = DefaultVertexShaderChannel::Create();
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
IPluginParamValModelPtr             DefaultFinalizePlugin::GetPluginParamValModel       () const
{
    return nullptr;
}

// *******************************
//
IParameterPtr                       DefaultFinalizePlugin::GetParameter                 ( const std::string & name ) const
{
    return nullptr;
}

// *******************************
//
bv::IValueConstPtr                  DefaultFinalizePlugin::GetValue                    ( const std::string & name ) const
{
    return nullptr;
}

// *******************************
//
IVertexAttributesChannelConstPtr    DefaultFinalizePlugin::GetVertexAttributesChannel   () const
{
    assert( m_prevPlugin );

    return m_prevPlugin->GetVertexAttributesChannel();
}

// *******************************
//
ITransformChannelConstPtr           DefaultFinalizePlugin::GetTransformChannel          () const
{
    assert( m_prevPlugin );

    return m_prevPlugin->GetTransformChannel();
}

// *******************************
//
IPixelShaderChannelConstPtr         DefaultFinalizePlugin::GetPixelShaderChannel        () const
{
    assert( m_prevPlugin );
    assert( m_prevPlugin->GetPixelShaderChannel() );

    if( m_finalizePSC == nullptr )
    {
//        m_finalizePSC = std::make_shared< DefaultFinalizePixelShaderChannel >( m_prevPlugin->GetPixelShaderChannel() );
    }

    return m_prevPlugin->GetPixelShaderChannel();
}

// *******************************
//
IVertexShaderChannelConstPtr        DefaultFinalizePlugin::GetVertexShaderChannel       () const
{
    assert( m_prevPlugin );

    auto vsc = m_prevPlugin->GetVertexShaderChannel();

    if( vsc == nullptr )
    {
        vsc = m_defaultVSChannel;
    }

    return vsc;
}

// *******************************
//
IGeometryShaderChannelConstPtr      DefaultFinalizePlugin::GetGeometryShaderChannel     () const
{
    assert( m_prevPlugin );

    return m_prevPlugin->GetGeometryShaderChannel();
}

// *******************************
//
IPixelShaderChannelPtr              DefaultFinalizePlugin::GetPixelShaderChannel        ()
{
    assert( m_prevPlugin );

    return m_prevPlugin->GetPixelShaderChannel();
}

// *******************************
//
IVertexShaderChannelPtr             DefaultFinalizePlugin::GetVertexShaderChannel       ()
{
    assert( m_prevPlugin );

    return m_prevPlugin->GetVertexShaderChannel();
}

// *******************************
//
IGeometryShaderChannelPtr           DefaultFinalizePlugin::GetGeometryShaderChannel     ()
{
    assert( m_prevPlugin );

    return m_prevPlugin->GetGeometryShaderChannel();
}

// *******************************
//
RendererContextConstPtr             DefaultFinalizePlugin::GetRendererContext           () const
{
    auto psc = GetPixelShaderChannel();

    if( psc )
    {
        return psc->GetRendererContext();
    }

    return nullptr;
}

// *******************************
//
bool                                DefaultFinalizePlugin::LoadResource                 ( IPluginResourceDescrConstPtr resDescr )
{
    return false;
}

// *******************************
//
void                                DefaultFinalizePlugin::Update                       ( TimeType t )
{
    //TODO: implement if there is logic that should be run
}

// *******************************
//
void                                DefaultFinalizePlugin::SetPrevPlugin                ( IPluginPtr plugin )
{
    assert( plugin != nullptr );

    m_finalizePSC = nullptr;
    m_finalizeVSC = nullptr;
    m_finalizeGSC = nullptr;

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
