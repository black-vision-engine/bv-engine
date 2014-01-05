#pragma once

#include "Engine/Models/Plugins/Interfaces/IPlugin.h"
#include "Engine/Models/Plugins/Interfaces/IValue.h"
#include "Engine/Models/Plugins/Interfaces/IPixelShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/IVertexShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/IGeometryShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/ISequenceAnimationSource.h"

#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannel.h"
#include "Engine/Models/Plugins/Channels/Transform/TransformChannel.h"
#include "Engine/Models/Plugins/Parameters/Parameter.h"
#include "PluginEnums.h"
#include "TextureInfo.h"


namespace bv { namespace model {

template< class Iface, class UIDType >
class BasePlugin : public Iface
{
protected:
    std::vector< IValue * >         m_values;

    ///////////////// Previous plugin ///////////
    const IPlugin *                 m_prevPlugin;


public:

    explicit                                    BasePlugin                  ( const IPlugin* prevPlugin );

    virtual void                                Update                      ( TimeType t );

    const char*                                 GetName                     () const                                        { return UIDType::GetName(); } 

    virtual void                                SetGeometryChannel          ( VertexAttributesChannel* geomChannel )        { assert(!"Implement in derived class"); }
    virtual void                                SetTransformChannel         ( TransformChannel* transformChannel )          { assert(!"Implement in derived class"); }
    virtual void                                SetPixelShaderChannel       ( IPixelShaderChannel * pShCh )                 { assert(!"Implement in derived class"); }
    virtual void                                SetVertexShaderChannel      ( IVertexShaderChannel * vShCh )                { assert(!"Implement in derived class"); }
    virtual void                                SetGeometryShaderChannel    ( IGeometryShaderChannel * gShCh )              { assert(!"Implement in derived class"); }

    virtual const IVertexAttributesChannel *    GetGeometryChannel          () const;                                       
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
const IVertexAttributesChannel *            BasePlugin< Iface, UIDType >::GetGeometryChannel          () const
{
    if( m_prevPlugin )
    {
        return m_prevPlugin->GetGeometryChannel();
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
