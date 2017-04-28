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

#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.inl"

#include "Engine/Models/IDGenerator.h"

#include "Serialization/ISerializable.h"

namespace bv { namespace model {



class BasePlugin : public IPlugin, public ISerializable
{
private:

    IPluginPtr                                  m_prevPlugin;

protected:

    UInt32										m_id;

    std::string                                 m_name;
    std::string                                 m_uid;

    IPluginParamValModelPtr                     m_pluginParamValModel;
    
    std::vector< LAsset >                       m_assets;

protected:

    explicit                                    BasePlugin                  ( const std::string & name, const std::string & uid, IPluginPtr prevPlugin, IPluginParamValModelPtr model );

public:
    virtual void                                Serialize                   ( ISerializer& doc ) const override;
    static ISerializablePtr                     Create                      ( const IDeserializer& doc );

    virtual IPluginPtr							Clone						() const override;


    virtual                                     ~BasePlugin                 ();

    virtual IPluginParamValModelPtr             GetPluginParamValModel      () const override;
    virtual IParameterPtr                       GetParameter                ( const std::string & name ) const override;
    virtual bv::IValueConstPtr                  GetValue                    ( const std::string & name ) const override;
    virtual ICachedParameterPtr                 GetCachedParameter          ( const std::string & name ) const override;
    virtual IStatedValuePtr                     GetState                    ( const std::string & name ) const;
    
    virtual IParamValModelPtr					GetResourceStateModel		( const std::string & name ) const override;
    virtual std::vector< IParameterPtr >        GetResourceStateModelParameters () const override;
    
    virtual std::vector< IParameterPtr >        GetParameters               () const override;

    virtual std::vector< LAsset >               GetLAssets                  () const;
    virtual void                                SetAsset                    ( int i, LAsset lasset );

    virtual void                                Update                      ( TimeType t );

    UInt32										GetID		                () const override   { return m_id; }

    const std::string &                         GetName                     () const override   { return m_name; } 
    const std::string &                         GetTypeUid                  () const override   { return m_uid; } 

    virtual bool								IsValid						() const override;

    virtual void                                SetGeometryChannel          ( VertexAttributesChannelPtr vaChannel ) { assert(!"Implement in derived class"); }
    virtual void                                SetTransformChannel         ( TransformChannelPtr transformChannel ) { assert(!"Implement in derived class"); }
    virtual void                                SetPixelShaderChannel       ( IPixelShaderChannelPtr pShCh )         { assert(!"Implement in derived class"); }
    virtual void                                SetVertexShaderChannel      ( IVertexShaderChannelPtr vShCh )        { assert(!"Implement in derived class"); }
    virtual void                                SetGeometryShaderChannel    ( IGeometryShaderChannelPtr gShCh )      { assert(!"Implement in derived class"); }
    virtual void                                SetAudioChannel             ( IAudioChannelPtr audioChannel )        { assert(!"Implement in derived class"); }

    virtual IVertexAttributesChannelConstPtr    GetVertexAttributesChannel  () const override;
    virtual ITransformChannelConstPtr           GetTransformChannel         () const override;
    virtual IPixelShaderChannelPtr              GetPixelShaderChannel       () const override;
    virtual IVertexShaderChannelConstPtr        GetVertexShaderChannel      () const override;
    virtual IGeometryShaderChannelConstPtr      GetGeometryShaderChannel    () const override;
    virtual IAudioChannelPtr                    GetAudioChannel             () const override;

    virtual RendererContextConstPtr             GetRendererContext          () const override;
    virtual void                                SetRendererContext          ( RendererContextPtr context ) override;

    virtual IPluginConstPtr                     GetPrevPlugin               () const override;
    virtual IPluginPtr							GetPrevPlugin               () override;
    virtual bool								SetPrevPlugin               ( IPluginPtr plugin ) override;

    virtual mathematics::RectConstPtr			GetAABB						( const glm::mat4 & ) const override;

    virtual bool                                LoadResource                ( AssetDescConstPtr assetDescr );

    virtual ParamTransformPtr				    GetParamTransform			() const override { return nullptr; }

    virtual std::vector< ITimeEvaluatorPtr >	GetTimelines				() const override;

protected:

    IParamValModelPtr                           PluginModel                 () const;
    IParamValModelPtr                           TransformChannelModel       () const;
    IParamValModelPtr                           VertexAttributesChannelModel() const;
    IParamValModelPtr                           PixelShaderChannelModel     () const;
    IParamValModelPtr                           VertexShaderChannelModel    () const;
    IParamValModelPtr                           GeometryShaderChannelModel  () const;

    bool                                        ParameterChanged            ( const std::string & name );
};


DEFINE_PTR_TYPE( BasePlugin );


ParamTransformPtr						    GetCurrentParamTransform( const IPlugin * pl );

typedef std::shared_ptr< const BasePlugin > BasePluginConstPtr;

} // model

namespace CloneViaSerialization {

void					UpdateTimelines				( const model::IPlugin * obj, const std::string & prefix, const std::string & destScene );

} //CloneViaSerialization

} // bv
