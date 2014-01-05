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

template< class Iface, class ParameterDescriptor >
class BasePlugin : public Iface
{
protected:

    typedef ParameterDescriptor ParamDesc;

protected:

    ParamDesc                       m_paramDesc;

    std::vector< IValue * >         m_values;

    ///////////////// Previous plugin ///////////
    const IPlugin *                 m_prevPlugin;


public:

    explicit                                    BasePlugin                  ( const IPlugin* prevPlugin );

    virtual void                                Update                      ( TimeType t );

    const std::string &                         GetName                     () const                                        { return ParamDesc::pluginName; } 

    virtual const std::vector< IValue * > &     GetValuesList               () const                                        { return m_values; }

    void                                        RegisterValue               ( IValue * v )                                  { m_values.push_back( v ); }

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

protected:

    const ParamDesc &                           PluginParamDesc             () const                                        { return m_paramDesc; }

};

// Implementation

// *******************************
//
template<class Iface, class ParameterDescriptor >
void BasePlugin< Iface, ParameterDescriptor >::Update( TimeType t )
{
    assert( !"Implement in derived class" );
    //if( GetGeometryChannel() )          GetGeometryChannel()->Update( t );
    //if( GetTransformChannel() )         GetTransformChannel->Update( t );
    //if( GetPixelShaderChannel() )       GetPixelShaderChannel->Update( t );
    //if( GetVertexShaderChannel() )      GetVertexShaderChannel->Update( t );
    //if( GetGeometryShaderChannel() )    GetGeometryShaderChannel->Update( t );
}

// *******************************
//
template<class Iface, class ParameterDescriptor >
BasePlugin< Iface, ParameterDescriptor >::BasePlugin( const IPlugin* prevPlugin )
    : m_paramDesc( ParameterDescriptor() )
    , m_prevPlugin( prevPlugin )
{
}

// *******************************
//
template<class Iface, class ParameterDescriptor >
const IVertexAttributesChannel *            BasePlugin< Iface, ParameterDescriptor >::GetGeometryChannel          () const
{
    if( m_prevPlugin )
    {
        return m_prevPlugin->GetGeometryChannel();
    }

    return nullptr;
}

// *******************************
//
template<class Iface, class ParameterDescriptor >
const ITransformChannel *           BasePlugin< Iface, ParameterDescriptor >::GetTransformChannel         () const
{
    if( m_prevPlugin )
    {
        return m_prevPlugin->GetTransformChannel();
    }

    return nullptr;
}

// *******************************
//
template<class Iface, class ParameterDescriptor >
const IPixelShaderChannel *         BasePlugin< Iface, ParameterDescriptor >::GetPixelShaderChannel       () const
{
    if( m_prevPlugin )
    {
        return m_prevPlugin->GetPixelShaderChannel();
    }

    return nullptr;
}

// *******************************
//
template<class Iface, class ParameterDescriptor >
const IVertexShaderChannel *        BasePlugin< Iface, ParameterDescriptor >::GetVertexShaderChannel      () const
{
    if( m_prevPlugin ) 
    {
        return m_prevPlugin->GetVertexShaderChannel();
    }

    return nullptr;
}

// *******************************
//
template<class Iface, class ParameterDescriptor >
const IGeometryShaderChannel *      BasePlugin< Iface, ParameterDescriptor >::GetGeometryShaderChannel    () const
{
    if( m_prevPlugin )
    {
        return m_prevPlugin->GetGeometryShaderChannel();
    }

    return nullptr;
}


} // model
} // bv
