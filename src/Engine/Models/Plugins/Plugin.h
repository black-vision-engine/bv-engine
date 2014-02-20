#pragma once

#include "Engine/Models/Plugins/Interfaces/IPluginDescriptor.h"
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

template< class Iface = IPlugin >
class BasePlugin : public Iface
{
protected:
    ///////////////// Previous plugin ///////////
    const IPlugin *                             m_prevPlugin;

    std::string                                 m_name;
    std::string                                 m_uid;

    IPluginParamValModelPtr                     m_pluginParamValModel;

protected:

    explicit                                    BasePlugin                  ( const std::string & name, const std::string & uid, const IPlugin * prevPlugin, IPluginParamValModelPtr model );

public:

    virtual                                     ~BasePlugin                 ();

    virtual IPluginParamValModel *              GetPluginParamValModel      ();
    virtual void                                Update                      ( TimeType t );

    const std::string &                         GetName                     () const    { return m_name; } 
    const std::string &                         GetTypeUid                  () const    { return m_uid; } 

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

    virtual Textures                            GetTextures                 () const                                            { return Textures(); }

    virtual bool                                HasAnimatingTexture         () const                                            { return false; }

    virtual ISequenceAnimationSource *          QuerySequenceAnimationSource()                                                  { return nullptr; }

protected:

    IParamValModel *                            PluginModel                 ();
    IParamValModel *                            TransformChannelModel       ();
    IParamValModel *                            VertexAttributesChannelModel();
    IParamValModel *                            PixelShaderChannelModel     ();
    IParamValModel *                            VertexShaderChannelModel    ();
    IParamValModel *                            GeometryShaderChannelModel  ();

};

// Implementation

// *******************************
//
template< class Iface >
void BasePlugin< Iface >::Update  ( TimeType t )
{
    assert( !"Implement in derived class" );
}

// *******************************
//
template< class Iface >
BasePlugin< Iface >::BasePlugin   ( const std::string & name, const std::string & uid, const IPlugin * prevPlugin, IPluginParamValModelPtr model )
    : m_prevPlugin( prevPlugin )
    , m_pluginParamValModel( model )
    , m_name( name )
    , m_uid( uid )
{
}

// *******************************
//
template< class Iface >
BasePlugin< Iface >::~BasePlugin()
{
}

// *******************************
//
template< class Iface >
IPluginParamValModel *   BasePlugin< Iface >::GetPluginParamValModel  ()
{
    return m_pluginParamValModel.get();
}

// *******************************
//
template< class Iface >
const IVertexAttributesChannel *            BasePlugin< Iface >::GetVertexAttributesChannel   () const
{
    if( m_prevPlugin )
    {
        return m_prevPlugin->GetVertexAttributesChannel();
    }

    return nullptr;
}

// *******************************
//
template< class Iface >
const ITransformChannel *           BasePlugin< Iface >::GetTransformChannel          () const
{
    if( m_prevPlugin )
    {
        return m_prevPlugin->GetTransformChannel();
    }

    return nullptr;
}

// *******************************
//
template< class Iface >
const IPixelShaderChannel *         BasePlugin< Iface >::GetPixelShaderChannel        () const
{
    if( m_prevPlugin )
    {
        return m_prevPlugin->GetPixelShaderChannel();
    }

    return nullptr;
}

// *******************************
//
template< class Iface >
const IVertexShaderChannel *        BasePlugin< Iface >::GetVertexShaderChannel       () const
{
    if( m_prevPlugin ) 
    {
        return m_prevPlugin->GetVertexShaderChannel();
    }

    return nullptr;
}

// *******************************
//
template< class Iface >
const IGeometryShaderChannel *      BasePlugin< Iface >::GetGeometryShaderChannel     () const
{
    if( m_prevPlugin )
    {
        return m_prevPlugin->GetGeometryShaderChannel();
    }

    return nullptr;
}

// *******************************
//
template< class Iface >
IParamValModel *                            BasePlugin< Iface >::PluginModel                  ()
{
    return m_pluginParamValModel->GetPluginModel();
}

// *******************************
//
template< class Iface >
IParamValModel *                            BasePlugin< Iface >::TransformChannelModel        ()
{
    return m_pluginParamValModel->GetTransformChannelModel();
}

// *******************************
//
template< class Iface >
IParamValModel *                            BasePlugin< Iface >::VertexAttributesChannelModel ()
{
    return m_pluginParamValModel->GetVertexAttributesChannelModel();
}

// *******************************
//
template< class Iface >
IParamValModel *                            BasePlugin< Iface >::PixelShaderChannelModel      ()
{
    return m_pluginParamValModel->GetPixelShaderChannelModel();
}

// *******************************
//
template< class Iface >
IParamValModel *                            BasePlugin< Iface >::VertexShaderChannelModel     ()
{
    return m_pluginParamValModel->GetVertexShaderChannelModel();
}

// *******************************
//
template< class Iface >
IParamValModel *                            BasePlugin< Iface >::GeometryShaderChannelModel   ()
{
    return m_pluginParamValModel->GetGeometryShaderChannelModel();
}

} // model
} // bv
