#pragma once

#include "Engine/Models/Plugins/Interfaces/IPlugin.h"
#include "Engine/Models/Plugins/Interfaces/IParameter.h"
#include "Engine/Models/Plugins/Interfaces/IPixelShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/IVertexShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/IGeometryShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/ISequenceAnimationSource.h"

#include "Engine/Models/Plugins/Channels/Geometry/GeometryChannel.h"
#include "Engine/Models/Plugins/Channels/Transform/TransformChannel.h"


namespace bv { namespace model {

class ResourceHandle;

struct TextureInfo
{
    ResourceHandle *    m_resHandle;
    std::string         m_texName;
    glm::mat4           m_texTransform;

    TextureInfo( ResourceHandle * resHandle, const std::string & texName, const glm::mat4 & texTransform = glm::mat4( 1.f ) )
        : m_resHandle( resHandle )
        , m_texName( texName )
        , m_texTransform( texTransform )
    {}
};

template< class Iface, class ParameterDescriptor >
class BasePlugin : public Iface
{
public:

    typedef typename Iface::Textures Textures;

protected:

    typedef ParameterDescriptor ParamDesc;

protected:

    ParamDesc                       m_paramDesc;

    std::vector< IParameter * >     m_params;
    std::vector< IValue * >         m_values;

    ///////////////// Previous plugin ///////////
    const IPlugin*                  m_prevPlugin;


    ///////////////// Textures //////////////////
    typename Iface::Textures        m_textures;

public:

    explicit                                    BasePlugin                  ( const IPlugin* prevPlugin );

    virtual void                                Update                      ( TimeType t );

    const std::string &                         GetName                     () const                                        { return ParamDesc::pluginName; } 

    virtual const std::vector< IValue * > &     GetValuesList               () const                                        { return m_values; }
    virtual const std::vector< IParameter * > & GetParametersList           () const                                        { return m_params; };

    void                                        RegisterValue               ( IValue * v )                                  { m_values.push_back( v ); }


    virtual void                                SetGeometryChannel          ( GeometryChannel* geomChannel )                { assert(!"Implement in derived class"); }
    virtual void                                SetTransformChannel         ( TransformChannel* transformChannel )          { assert(!"Implement in derived class"); }
    virtual void                                SetPixelShaderChannel       ( IPixelShaderChannel * pShCh )                 { assert(!"Implement in derived class"); }
    virtual void                                SetVertexShaderChannel      ( IVertexShaderChannel * vShCh )                { assert(!"Implement in derived class"); }
    virtual void                                SetGeometryShaderChannel    ( IGeometryShaderChannel * gShCh )              { assert(!"Implement in derived class"); }

    virtual const IGeometryChannel *            GetGeometryChannel          () const;                                       
    virtual const ITransformChannel *           GetTransformChannel         () const;                                       
    virtual const IPixelShaderChannel *         GetPixelShaderChannel       () const;                                       
    virtual const IVertexShaderChannel *        GetVertexShaderChannel      () const;                                       
    virtual const IGeometryShaderChannel *      GetGeometryShaderChannel    () const;                                       

    virtual const Textures &                    GetTextures                 () const                                        { return m_textures; }

    virtual bool                                HasAnimatingTexture         () const                                        { return false; }

    virtual ISequenceAnimationSource *          QuerySequenceAnimationSource()                                              { return nullptr; }

protected:

    const ParamDesc &                           PluginParamDesc             () const                                        { return m_paramDesc; }

};

// Implementation

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

template<class Iface, class ParameterDescriptor >
BasePlugin< Iface, ParameterDescriptor >::BasePlugin( const IPlugin* prevPlugin )
    : m_paramDesc( ParameterDescriptor() )
    , m_prevPlugin( prevPlugin )
{}

template<class Iface, class ParameterDescriptor >
const IGeometryChannel *            BasePlugin< Iface, ParameterDescriptor >::GetGeometryChannel          () const
{
    if( m_prevPlugin ) 
        return m_prevPlugin->GetGeometryChannel();
    else
        return nullptr;
}

template<class Iface, class ParameterDescriptor >
const ITransformChannel *           BasePlugin< Iface, ParameterDescriptor >::GetTransformChannel         () const
{
    if( m_prevPlugin ) 
        return m_prevPlugin->GetTransformChannel();
    else
        return nullptr;
}

template<class Iface, class ParameterDescriptor >
const IPixelShaderChannel *         BasePlugin< Iface, ParameterDescriptor >::GetPixelShaderChannel       () const
{
    if( m_prevPlugin ) 
        return m_prevPlugin->GetPixelShaderChannel();
    else
        return nullptr;
}

template<class Iface, class ParameterDescriptor >
const IVertexShaderChannel *        BasePlugin< Iface, ParameterDescriptor >::GetVertexShaderChannel      () const
{
    if( m_prevPlugin ) 
        return m_prevPlugin->GetVertexShaderChannel();
    else
        return nullptr;
}

template<class Iface, class ParameterDescriptor >
const IGeometryShaderChannel *      BasePlugin< Iface, ParameterDescriptor >::GetGeometryShaderChannel    () const
{
    if( m_prevPlugin ) 
        return m_prevPlugin->GetGeometryShaderChannel();
    else
        return nullptr;
}


} // model
} // bv
