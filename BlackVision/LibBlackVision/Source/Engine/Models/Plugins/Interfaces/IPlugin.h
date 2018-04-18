#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <memory>

#include "Engine/Models/IDGenerator.h"

#include "Engine/Interfaces/IValue.h"
#include "Engine/Models/Plugins/Interfaces/IParameter.h"
#include "Engine/Models/Plugins/Interfaces/ICachedParameter.h"
#include "Engine/Interfaces/IUpdatable.h"
#include "Engine/Models/Plugins/Channels/RendererContext/RendererContext.h"
#include "Engine/Models/Plugins/Interfaces/IPluginParamValModel.h"
#include "Assets/AssetDescriptor.h"
#include "Assets/LAsset.h"
#include "Engine/Models/Plugins/Interfaces/IVertexAttributesChannel.h"
#include "Engine/Models/Plugins/Interfaces/IPixelShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/IVertexShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/IGeometryShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/ITransformChannel.h"
#include "Engine/Models/Plugins/Interfaces/IAudioChannel.h"
#include "Engine/Models/Plugins/Parameters/CompositeTypedParameters.h"
#include "Mathematics/Rect.h"




/**@defgroup PluginsList
@ingroup Model*/


/**@page Plugins Plugins


*/



namespace bv { namespace model {

class IDefaultTransformChannel;
class ITransformChannel;
class IVertexShaderChannel;
class IGeometryShaderChannel;
class ResourceHandle;

class IPlugin;

DEFINE_PTR_TYPE(IPlugin)
DEFINE_CONST_PTR_TYPE(IPlugin)




/**@brief Plugin interface.
@ingroup Model*/
class IPlugin : public IUpdatable
{
public:
    virtual IPluginPtr							Clone						() const = 0;

    virtual UniqueID			                GetID		                () const = 0;
    
    virtual const std::string &                 GetName                     () const = 0;
    virtual const std::string &                 GetTypeUid                  () const = 0;

    virtual bool								IsValid						() const = 0;

    //TODO: rething the whole query API - and const correctness as well for that matter
    virtual IPluginParamValModelPtr             GetPluginParamValModel      () const = 0; //FIXME: czy tu nie powinno byc tez od razu const przy zwracanym wskazniku?
    virtual IParameterPtr                       GetParameter                ( const std::string & name ) const = 0;
    virtual bv::IValueConstPtr                  GetValue                    ( const std::string & name ) const = 0;
    virtual ICachedParameterPtr                 GetCachedParameter          ( const std::string & name ) const = 0;

    virtual IParamValModelPtr					GetResourceStateModel		( const std::string & name ) const = 0;
    virtual std::vector< IParameterPtr >        GetResourceStateModelParameters () const = 0;

    virtual std::vector< IParameterPtr >        GetParameters               () const = 0;

    virtual IVertexAttributesChannelConstPtr    GetVertexAttributesChannel  () const = 0;
    virtual ITransformChannelConstPtr           GetTransformChannel         () const = 0;
    virtual IPixelShaderChannelPtr              GetPixelShaderChannel       () const = 0;
    virtual IVertexShaderChannelConstPtr        GetVertexShaderChannel      () const = 0;
    virtual IGeometryShaderChannelConstPtr      GetGeometryShaderChannel    () const = 0;
    virtual IAudioChannelPtr                    GetAudioChannel             () const = 0;

    virtual RendererContextConstPtr             GetRendererContext          () const = 0;
    virtual void                                SetRendererContext          ( RendererContextPtr context ) = 0;

    virtual IPluginConstPtr                     GetPrevPlugin               () const = 0;
    virtual IPluginPtr							GetPrevPlugin				() = 0;
    virtual bool								SetPrevPlugin               ( IPluginPtr plugin ) = 0;

    // axis-aligned bounding box
    virtual mathematics::RectConstPtr			GetAABB						( const glm::mat4 & ) const = 0;

    //FIXME: implement a more generic interface for resource loading
    virtual bool                                LoadResource                ( AssetDescConstPtr assetDescr ) = 0;
    virtual std::vector< LAsset >               GetLAssets                  () const = 0;


    virtual ParamTransformPtr				    GetParamTransform			() const = 0; //FIXME: BArdzo brzydki hack zeby zadzialal Scroller

    virtual std::vector< ITimeEvaluatorPtr >	GetTimelines				() const = 0;

    virtual                                     ~IPlugin                    (){}

};

} // model
} // bv
