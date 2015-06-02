#pragma once

#include "Engine/Models/Plugins/Interfaces/IPluginDescriptor.h"
#include "Engine/Models/Plugins/Interfaces/IPlugin.h"
#include "Engine/Models/Plugins/Interfaces/IPluginParamValModel.h"
#include "Engine/Models/Plugins/Interfaces/IPixelShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/IVertexShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/IGeometryShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/ISequenceAnimationSource.h"

#include "Engine/Models/Plugins/Manager/PluginsManager.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannel.h"
#include "Engine/Models/Plugins/Channels/Transform/TransformChannel.h"
#include "TextureInfo.h"

#include "Engine/Interfaces/ISerializable.h"
//#include "Engine/Models/Plugins/PluginsFactory.h"

namespace bv { namespace model {

template< class Iface = IPlugin >
class BasePlugin : public Iface, public ISerializable
{
protected:

    ///////////////// Previous plugin ///////////
    IPluginPtr                                  m_prevPlugin;

    std::string                                 m_name;
    std::string                                 m_uid;

    IPluginParamValModelPtr                     m_pluginParamValModel;

protected:

    explicit                                    BasePlugin                  ( const std::string & name, const std::string & uid, IPluginPtr prevPlugin, IPluginParamValModelPtr model );

public:
    virtual void                                Serialize                   ( SerializeObject &/*doc*/ ) const override {}
    static ISerializablePtr                     Create                      ( DeserializeObject&/*doc*/ );

    virtual                                     ~BasePlugin                 ();

    virtual IPluginParamValModelPtr             GetPluginParamValModel      () const override;
    virtual IParameterPtr                       GetParameter                ( const std::string & name ) const override;
    virtual bv::IValueConstPtr                  GetValue                    ( const std::string & name ) const override;
    virtual ICachedParameterPtr                 GetCachedParameter          ( const std::string & name ) const override;
    virtual IStatedValuePtr                     GetState                    ( const std::string & name ) const;


    virtual void                                Update                      ( TimeType t );

    const std::string &                         GetName                     () const override   { return m_name; } 
    const std::string &                         GetTypeUid                  () const override   { return m_uid; } 

    virtual void                                SetGeometryChannel          ( VertexAttributesChannelPtr vaChannel ) { assert(!"Implement in derived class"); }
    virtual void                                SetTransformChannel         ( TransformChannelPtr transformChannel ) { assert(!"Implement in derived class"); }
    virtual void                                SetPixelShaderChannel       ( IPixelShaderChannelPtr pShCh )         { assert(!"Implement in derived class"); }
    virtual void                                SetVertexShaderChannel      ( IVertexShaderChannelPtr vShCh )        { assert(!"Implement in derived class"); }
    virtual void                                SetGeometryShaderChannel    ( IGeometryShaderChannelPtr gShCh )      { assert(!"Implement in derived class"); }

    virtual IVertexAttributesChannelConstPtr    GetVertexAttributesChannel  () const override;
    virtual ITransformChannelConstPtr           GetTransformChannel         () const override;
    virtual IPixelShaderChannelConstPtr         GetPixelShaderChannel       () const override;
    virtual IVertexShaderChannelConstPtr        GetVertexShaderChannel      () const override;
    virtual IGeometryShaderChannelConstPtr      GetGeometryShaderChannel    () const override;

    virtual RendererContextConstPtr             GetRendererContext          () const override;

    virtual IPluginConstPtr                     GetPrevPlugin               () const override;
    virtual IPluginPtr							GetPrevPlugin               () override;
    virtual void								SetPrevPlugin               ( IPluginPtr plugin ) override;

	virtual mathematics::RectConstPtr			GetAABB						( const glm::mat4 & ) const override;

    virtual bool                                LoadResource                ( AssetDescConstPtr assetDescr );

	virtual ParamTransformVecPtr				GetParamTransform			() const override { return nullptr; }


protected:

    IParamValModelPtr                           PluginModel                 ();
    IParamValModelPtr                           TransformChannelModel       ();
    IParamValModelPtr                           VertexAttributesChannelModel();
    IParamValModelPtr                           PixelShaderChannelModel     ();
    IParamValModelPtr                           VertexShaderChannelModel    ();
    IParamValModelPtr                           GeometryShaderChannelModel  ();

    bool                                        ParameterChanged            ( const std::string & name );
};

// Implementation

// *******************************
//
template< class Iface >
void BasePlugin< Iface >::Update  ( TimeType t )
{
    { t; } // FIXME: suppress unused warning
    assert( !"Implement in derived class" );
}

// *******************************
//
template< class Iface >
BasePlugin< Iface >::BasePlugin   ( const std::string & name, const std::string & uid, IPluginPtr prevPlugin, IPluginParamValModelPtr model )
    : m_prevPlugin( prevPlugin )
    , m_name( name )
    , m_uid( uid )
    , m_pluginParamValModel( model )
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
IPluginParamValModelPtr     BasePlugin< Iface >::GetPluginParamValModel () const
{
    return m_pluginParamValModel;
}

// *******************************
//
template< class Iface >
IParameterPtr               BasePlugin< Iface >::GetParameter           ( const std::string & name ) const
{
    IPluginParamValModelPtr pvm =    GetPluginParamValModel(); //FIXME: this is pretty hackish to avoid const correctness related errors
    
    IParamValModelPtr models[] = {    pvm->GetPluginModel()
                                    , pvm->GetTransformChannelModel()
                                    , pvm->GetVertexAttributesChannelModel()
                                    , pvm->GetPixelShaderChannelModel()
                                    , pvm->GetVertexShaderChannelModel()
                                    , pvm->GetGeometryShaderChannelModel() 
                                };

    IParameterPtr retParam = nullptr;

    for( auto model : models )
    {
        if( model && ( retParam = model->GetParameter( name ) ) )
        {
            return retParam;
        }
    }

    return nullptr;
}


// *******************************
//
template< class Iface >
IStatedValuePtr             BasePlugin< Iface >::GetState               ( const std::string & name ) const
{
    IPluginParamValModelPtr pvm =    GetPluginParamValModel(); //FIXME: this is pretty hackish to avoid const correctness related errors
    
    IParamValModelPtr models[] = {    pvm->GetPluginModel()
                                    , pvm->GetTransformChannelModel()
                                    , pvm->GetVertexAttributesChannelModel()
                                    , pvm->GetPixelShaderChannelModel()
                                    , pvm->GetVertexShaderChannelModel()
                                    , pvm->GetGeometryShaderChannelModel() 
                                };

    IStatedValuePtr retParam = nullptr;

    for( auto model : models )
    {
        if( model && ( retParam = model->GetState( name ) ) )
        {
            return retParam;
        }
    }

    return nullptr;
}

// *******************************
//
struct NullDeleter {template<typename T> void operator()(T*) {} };

template< class Iface >
ICachedParameterPtr             BasePlugin< Iface >::GetCachedParameter          ( const std::string & name ) const // FIXME mader fakier
{
	assert( false ); // DEPRECATED
    IParameterPtr param = GetParameter( name );

    //ParamBoolPtr qParam = std::static_pointer_cast< IParameterPtr, ParamBoolPtr >( param );
    //ICachedParameterPtr cParam = std::dynamic_pointer_cast< ParamBoolPtr, ICachedParameterPtr >( qParam );
    //auto ret = cParam;

    IParameter* hParam = param.get();
    ParamBool* hqParam = static_cast< ParamBool* >( hParam ); // FIXME: although we may assume implementation here, we really shouldn't
    ICachedParameter* hcParam = dynamic_cast< ICachedParameter* >( hqParam );
    auto ret = std::shared_ptr< ICachedParameter >( hcParam, NullDeleter() ); // FIXME: removing a need for NullDeleter would be very good idea

    return ret;
}

// *******************************
//
template< class Iface >
bv::IValueConstPtr         BasePlugin< Iface >::GetValue           ( const std::string & name ) const
{
    IPluginParamValModelPtr pvm =    GetPluginParamValModel(); //FIXME: this is pretty hackish to avoid const correctness related errors
    
    IParamValModelPtr models[] = {    pvm->GetPluginModel()
                                    , pvm->GetTransformChannelModel()
                                    , pvm->GetVertexAttributesChannelModel()
                                    , pvm->GetPixelShaderChannelModel()
                                    , pvm->GetVertexShaderChannelModel()
                                    , pvm->GetGeometryShaderChannelModel() 
                                };

    bv::IValueConstPtr retVal = nullptr;

    for( auto model : models )
    {
        if( model && ( retVal = model->GetValue( name ) ) )
        {
            return retVal;
        }
    }

    return nullptr;
}

// *******************************
//
template< class Iface >
IVertexAttributesChannelConstPtr            BasePlugin< Iface >::GetVertexAttributesChannel   () const
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
ITransformChannelConstPtr           BasePlugin< Iface >::GetTransformChannel            () const
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
IPixelShaderChannelConstPtr         BasePlugin< Iface >::GetPixelShaderChannel          () const
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
IVertexShaderChannelConstPtr        BasePlugin< Iface >::GetVertexShaderChannel         () const
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
IGeometryShaderChannelConstPtr      BasePlugin< Iface >::GetGeometryShaderChannel       () const
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
RendererContextConstPtr             BasePlugin< Iface >::GetRendererContext             () const
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
template< class Iface >
IPluginConstPtr                     BasePlugin< Iface >::GetPrevPlugin                  () const
{
    return m_prevPlugin;
}

// *******************************
//
template< class Iface >
IPluginPtr							BasePlugin< Iface >::GetPrevPlugin                  ()
{
    return m_prevPlugin;
}

// *******************************
//
template< class Iface >
void								BasePlugin< Iface >::SetPrevPlugin                  ( IPluginPtr plugin )
{
    m_prevPlugin = plugin;
}


// *******************************
//
template< class Iface >
mathematics::RectConstPtr			BasePlugin< Iface >::GetAABB						( const glm::mat4 & ) const
{
	return nullptr;
}

// *******************************
//
template< class Iface >
bool                                BasePlugin< Iface >::LoadResource                   ( AssetDescConstPtr assetDescr )
{
    return false;
}

// *******************************
//
template< class Iface >
IParamValModelPtr                           BasePlugin< Iface >::PluginModel            ()
{
    return m_pluginParamValModel->GetPluginModel();
}

// *******************************
//
template< class Iface >
IParamValModelPtr                           BasePlugin< Iface >::TransformChannelModel        ()
{
    return m_pluginParamValModel->GetTransformChannelModel();
}

// *******************************
//
template< class Iface >
IParamValModelPtr                           BasePlugin< Iface >::VertexAttributesChannelModel ()
{
    return m_pluginParamValModel->GetVertexAttributesChannelModel();
}

// *******************************
//
template< class Iface >
IParamValModelPtr                           BasePlugin< Iface >::PixelShaderChannelModel      ()
{
    return m_pluginParamValModel->GetPixelShaderChannelModel();
}

// *******************************
//
template< class Iface >
IParamValModelPtr                           BasePlugin< Iface >::VertexShaderChannelModel     ()
{
    return m_pluginParamValModel->GetVertexShaderChannelModel();
}

// *******************************
//
template< class Iface >
IParamValModelPtr                           BasePlugin< Iface >::GeometryShaderChannelModel   ()
{
    return m_pluginParamValModel->GetGeometryShaderChannelModel();
}


// *******************************
//
template< class Iface >
bool                                        BasePlugin< Iface >::ParameterChanged            ( const std::string & name )
{
    auto state = GetState( name );
    assert( state );
    return state->StateChanged();
}

ParamTransformVecPtr						GetCurrentParamTransform( const IPlugin * pl );

} // model
} // bv
