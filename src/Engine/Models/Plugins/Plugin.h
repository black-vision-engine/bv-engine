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

template< class Iface, class DescType >
class BasePlugin : public Iface
{
protected:
    ///////////////// Previous plugin ///////////
    const IPlugin *                             m_prevPlugin;
    IPluginParamValModelPtr                     m_pluginParamValModel;

protected:

    explicit                                    BasePlugin                  ( const IPlugin * prevPlugin, IPluginParamValModelPtr model = nullptr );

public:

    virtual                                     ~BasePlugin                 ();

    virtual IPluginParamValModel *              GetPluginParamValModel      ();
    virtual void                                Update                      ( TimeType t );

    const char*                                 GetName                     () const                                            { return DescType::GetName(); } 

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
template<class Iface, class DescType >
void BasePlugin< Iface, DescType >::Update  ( TimeType t )
{
    assert( !"Implement in derived class" );
}

// *******************************
//
template<class Iface, class DescType >
BasePlugin< Iface, DescType >::BasePlugin   ( const IPlugin * prevPlugin, IPluginParamValModelPtr model )
    : m_prevPlugin( prevPlugin )
    , m_pluginParamValModel( model )
{
}

// *******************************
//
template<class Iface, class DescType >
BasePlugin< Iface, DescType >::~BasePlugin()
{
}

// *******************************
//
template<class Iface, class DescType >
IPluginParamValModel *   BasePlugin< Iface, DescType >::GetPluginParamValModel  ()
{
    return m_pluginParamValModel.get();
}

// *******************************
//
template<class Iface, class DescType  >
const IVertexAttributesChannel *            BasePlugin< Iface, DescType >::GetVertexAttributesChannel   () const
{
    if( m_prevPlugin )
    {
        return m_prevPlugin->GetVertexAttributesChannel();
    }

    return nullptr;
}

// *******************************
//
template<class Iface, class DescType >
const ITransformChannel *           BasePlugin< Iface, DescType >::GetTransformChannel          () const
{
    if( m_prevPlugin )
    {
        return m_prevPlugin->GetTransformChannel();
    }

    return nullptr;
}

// *******************************
//
template<class Iface, class DescType >
const IPixelShaderChannel *         BasePlugin< Iface, DescType >::GetPixelShaderChannel        () const
{
    if( m_prevPlugin )
    {
        return m_prevPlugin->GetPixelShaderChannel();
    }

    return nullptr;
}

// *******************************
//
template<class Iface, class DescType >
const IVertexShaderChannel *        BasePlugin< Iface, DescType >::GetVertexShaderChannel       () const
{
    if( m_prevPlugin ) 
    {
        return m_prevPlugin->GetVertexShaderChannel();
    }

    return nullptr;
}

// *******************************
//
template<class Iface, class DescType >
const IGeometryShaderChannel *      BasePlugin< Iface, DescType >::GetGeometryShaderChannel     () const
{
    if( m_prevPlugin )
    {
        return m_prevPlugin->GetGeometryShaderChannel();
    }

    return nullptr;
}

// *******************************
//
template<class Iface, class DescType >
const IDefaultTransformChannel *    BasePlugin< Iface, DescType >::GetDefaultTransformChannel   () const
{
    if( m_prevPlugin )
    {
        return m_prevPlugin->GetDefaultTransformChannel();
    }

    return nullptr;
}

} // model
} // bv
