#pragma once

#include "Engine/Models/Plugins/Interfaces/IPlugin.h"
#include "Engine/Models/Plugins/Interfaces/IParameter.h"
#include "Engine/Models/Plugins/Channels/Geometry/GeometryChannel.h"
#include "Engine/Models/Plugins/Channels/Transform/TransformChannel.h"
#include "Engine/Models/Plugins/Interfaces/IPixelShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/IVertexShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/IGeometryShaderChannel.h"

namespace bv { namespace model {

class GeometryChannel;
class TransformChannel;
class IPixelShaderChannel;
class IVertexShaderChannel;
class IGeometryShaderChannel;
class ResourceHandle;

struct TextureInfo
{
    ResourceHandle* m_resHandle;
    std::string     m_texName;
    glm::mat4       m_texTransform;

    TextureInfo( ResourceHandle* resHandle, const std::string& texName, const glm::mat4& texTransform = glm::mat4( 1.f ) )
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

    ///////////////// Channels //////////////////

    GeometryChannel*               m_geomChannel;
    TransformChannel*              m_transformChannel;
    IPixelShaderChannel*           m_pshaderChannel;
    IVertexShaderChannel*          m_vshaderChannel;
    IGeometryShaderChannel*        m_gshaderChannel;

    ///////////////// Textures //////////////////
    typename Iface::Textures       m_textures;

public:

    explicit                                    BasePlugin                  ();

    virtual void                                Update                      ( float t );

    const std::string &                         GetName                     () const                                        { return ParamDesc::pluginName; } 

    virtual const std::vector< IValue * > &     GetValuesList               () const                                        { return m_values; }
    virtual const std::vector< IParameter * > & GetParametersList           () const                                        { return m_params; };

    void                                        RegisterValue               ( IValue * v )                                  { m_values.push_back( v ); }


    void                                        SetGeometryChannel          ( GeometryChannel* geomChannel )                { m_geomChannel = geomChannel; }
    void                                        SetTransformChannel         ( TransformChannel* transformChannel )          { m_transformChannel = transformChannel; }
    void                                        SetPixelShaderChannel       ( IPixelShaderChannel * pShCh )                 { m_pshaderChannel = pShCh; }
    void                                        SetVertexShaderChannel      ( IVertexShaderChannel * vShCh )                { m_vshaderChannel = vShCh; }
    void                                        SetGeometryShaderChannel    ( IGeometryShaderChannel * gShCh )              { m_gshaderChannel = gShCh; }

    virtual const IGeometryChannel*             GetGeometryChannel          () const                                        { return m_geomChannel; }
    virtual const ITransformChannel*            GetTransformChannel         () const                                        { return m_transformChannel; }
    virtual const IPixelShaderChannel*          GetPixelShaderChannel       () const                                        { return m_pshaderChannel; }
    virtual const IVertexShaderChannel*         GetVertexShaderChannel      () const                                        { return m_vshaderChannel; }
    virtual const IGeometryShaderChannel*       GetGeometryShaderChannel    () const                                        { return m_gshaderChannel; }

    virtual const Textures&                     GetTextures                 () const                                        { return m_textures; }

protected:

    const ParamDesc &                           PluginParamDesc             () const                                        { return m_paramDesc; }

};

// Implementation

template<class Iface, class ParameterDescriptor >
void BasePlugin< Iface, ParameterDescriptor >::Update( float t )
{
    if(m_geomChannel) m_geomChannel->Update( t );
    if(m_transformChannel) m_transformChannel->Update( t );
    if(m_pshaderChannel) m_pshaderChannel->Update( t );
    if(m_vshaderChannel) m_vshaderChannel->Update( t );
    if(m_gshaderChannel) m_gshaderChannel->Update( t );
}

template<class Iface, class ParameterDescriptor >
BasePlugin< Iface, ParameterDescriptor >::BasePlugin()
    : m_paramDesc( ParameterDescriptor() )
    , m_geomChannel( nullptr )
    , m_transformChannel(nullptr)
    , m_pshaderChannel(nullptr)
    , m_vshaderChannel(nullptr)
    , m_gshaderChannel(nullptr)
{}


} // model
} // bv
