#pragma once

#include "Engine/Models/Plugins/Interfaces/IPlugin.h"
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

    std::vector< IParameter * >                 m_modelParameters;

public:

    explicit                                    BasePlugin                  ( const IPlugin * prevPlugin );

    virtual const std::vector< IParameter * > & QueryModelParameters        ();
    virtual void                                Update                      ( TimeType t );

    const char*                                 GetName                     () const                                        { return UIDType::GetName(); } 

    virtual void                                SetGeometryChannel          ( VertexAttributesChannel * vaChannel )         { assert(!"Implement in derived class"); }
    virtual void                                SetTransformChannel         ( TransformChannel * transformChannel )         { assert(!"Implement in derived class"); }
    virtual void                                SetPixelShaderChannel       ( IPixelShaderChannel * pShCh )                 { assert(!"Implement in derived class"); }
    virtual void                                SetVertexShaderChannel      ( IVertexShaderChannel * vShCh )                { assert(!"Implement in derived class"); }
    virtual void                                SetGeometryShaderChannel    ( IGeometryShaderChannel * gShCh )              { assert(!"Implement in derived class"); }

    virtual const IVertexAttributesChannel *    GetVertexAttributesChannel  () const;                                       
    virtual const ITransformChannel *           GetTransformChannel         () const;                                       
    virtual const IPixelShaderChannel *         GetPixelShaderChannel       () const;                                       
    virtual const IVertexShaderChannel *        GetVertexShaderChannel      () const;                                       
    virtual const IGeometryShaderChannel *      GetGeometryShaderChannel    () const;                                       

    virtual Textures                            GetTextures                 () const                                        { return Textures(); }

    virtual bool                                HasAnimatingTexture         () const                                        { return false; }

    virtual ISequenceAnimationSource *          QuerySequenceAnimationSource()                                              { return nullptr; }
};

// Implementation

// *******************************
//
template<class Iface, class UIDType >
const std::vector< IParameter * > & BasePlugin< Iface, UIDType >::QueryModelParameters        ()
{
    return m_modelParameters;
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
BasePlugin< Iface, UIDType >::BasePlugin( const IPlugin* prevPlugin )
    : m_prevPlugin( prevPlugin )
{
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


} // model
} // bv
