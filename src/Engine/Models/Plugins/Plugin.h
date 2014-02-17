#pragma once

#include "Engine/Models/Plugins/Interfaces/IPlugin.h"
#include "Engine/Models/Plugins/Interfaces/IPluginParamValModel.h"
#include "Engine/Models/Plugins/Interfaces/IPixelShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/IVertexShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/IGeometryShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/ISequenceAnimationSource.h"

#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannel.h"
#include "Engine/Models/Plugins/Channels/Transform/TransformChannel.h"
#include "PluginEnums.h"
#include "TextureInfo.h"


namespace bv { namespace model {

template< class Iface, class UIDType >
class BasePlugin : public Iface
{
protected:
    ///////////////// Previous plugin ///////////
    const IPlugin *                             m_prevPlugin;
    IPluginParamValModel *                      m_pluginParamValModel;

protected:

    explicit                                    BasePlugin                  ( const IPlugin * prevPlugin, IPluginParamValModel * model = nullptr );

public:

    virtual                                     ~BasePlugin                 ();

    virtual IPluginParamValModel *              GetPluginModelParameters    ();
    virtual void                                Update                      ( TimeType t );

    const char*                                 GetName                     () const                                            { return UIDType::GetName(); } 

    virtual void                                SetGeometryChannel          ( VertexAttributesChannel * vaChannel ) { assert(!"Implement in derived class"); }
    virtual void                                SetTransformChannel         ( TransformChannel * transformChannel ) { assert(!"Implement in derived class"); }
    virtual void                                SetPixelShaderChannel       ( IPixelShaderChannel * pShCh )         { assert(!"Implement in derived class"); }
    virtual void                                SetVertexShaderChannel      ( IVertexShaderChannel * vShCh )        { assert(!"Implement in derived class"); }
    virtual void                                SetGeometryShaderChannel    ( IGeometryShaderChannel * gShCh )      { assert(!"Implement in derived class"); }

    virtual const IVertexAttributesChannel *    GetVertexAttributesChannel  () const override;
    virtual const ITransformChannel *           GetTransformChannel         () const override;
    virtual const IPixelShaderChannel *         GetPixelShaderChannel       () const override;
    virtual const IVertexShaderChannel *        GetVertexShaderChannel      () const override;
    virtual const IGeometryShaderChannel *      GetGeometryShaderChannel    () const override;
    virtual const IDefaultTransformChannel *    GetDefaultTransformChannel  () const override;

    virtual Textures                            GetTextures                 () const                                            { return Textures(); }

    virtual bool                                HasAnimatingTexture         () const                                            { return false; }

    virtual ISequenceAnimationSource *          QuerySequenceAnimationSource()                                                  { return nullptr; }

};

// Implementation

// *******************************
//
template<class Iface, class UIDType >
IPluginParamValModel *              BasePlugin< Iface, UIDType >::GetPluginModelParameters    ()
{
    assert( false );

    return nullptr;
}

// *******************************
//
template<class Iface, class UIDType >
void BasePlugin< Iface, UIDType >::Update( TimeType t )
{
    assert( !"Implement in derived class" );
}

// *******************************
//
template<class Iface, class UIDType >
BasePlugin< Iface, UIDType >::BasePlugin( const IPlugin * prevPlugin, IPluginParamValModel * model )
    : m_prevPlugin( prevPlugin )
    , m_pluginParamValModel( model )
{
}

// *******************************
//
template<class Iface, class UIDType >
BasePlugin< Iface, UIDType >::~BasePlugin()
{
    delete m_pluginParamValModel;
}

// *******************************
//
template<class Iface, class UIDType  >
const IVertexAttributesChannel *            BasePlugin< Iface, UIDType >::GetVertexAttributesChannel          () const
{
    if( m_prevPlugin )
    {
        return m_prevPlugin->GetVertexAttributesChannel();
    }

    return nullptr;
}

// *******************************
//
template<class Iface, class UIDType >
const ITransformChannel *           BasePlugin< Iface, UIDType >::GetTransformChannel         () const
{
    if( m_prevPlugin )
    {
        return m_prevPlugin->GetTransformChannel();
    }

    return nullptr;
}

// *******************************
//
template<class Iface, class UIDType >
const IPixelShaderChannel *         BasePlugin< Iface, UIDType >::GetPixelShaderChannel       () const
{
    if( m_prevPlugin )
    {
        return m_prevPlugin->GetPixelShaderChannel();
    }

    return nullptr;
}

// *******************************
//
template<class Iface, class UIDType >
const IVertexShaderChannel *        BasePlugin< Iface, UIDType >::GetVertexShaderChannel      () const
{
    if( m_prevPlugin ) 
    {
        return m_prevPlugin->GetVertexShaderChannel();
    }

    return nullptr;
}

// *******************************
//
template<class Iface, class UIDType >
const IGeometryShaderChannel *      BasePlugin< Iface, UIDType >::GetGeometryShaderChannel    () const
{
    if( m_prevPlugin )
    {
        return m_prevPlugin->GetGeometryShaderChannel();
    }

    return nullptr;
}

// *******************************
//
template<class Iface, class UIDType >
const IDefaultTransformChannel *    BasePlugin< Iface, UIDType >::GetDefaultTransformChannel  () const
{
    if( m_prevPlugin )
    {
        return m_prevPlugin->GetDefaultTransformChannel();
    }

    return nullptr;
}

} // model
} // bv
