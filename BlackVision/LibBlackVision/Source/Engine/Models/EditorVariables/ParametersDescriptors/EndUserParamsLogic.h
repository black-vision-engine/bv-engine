#pragma once

#include "EndUserParamDescriptor.h"
#include "Engine/Events/ParamAddress.h"

#include "ParameterMapping.h"

#include <map>



namespace bv
{


class ISerializer;
class IDeserializer;

namespace model
{
    class CameraModel;
    DEFINE_PTR_TYPE( CameraModel );

    class IModelLight;
    DEFINE_PTR_TYPE( IModelLight );

    class BasePlugin;
    DEFINE_PTR_TYPE( BasePlugin );

    class INodeLogic;
    DEFINE_PTR_TYPE( INodeLogic );

    class IModelNodeEffect;
    DEFINE_PTR_TYPE( IModelNodeEffect );
}



class EndUserParamsLogic
{
private:

    model::SceneModel *         m_ownerScene;

    ParameterMapping< model::CameraModelPtr >       m_cameraDescs;
    ParameterMapping< model::IModelLightPtr >       m_lightDescs;
    ParameterMapping< model::BasePluginPtr >        m_pluginDescs;
    ParameterMapping< model::INodeLogicPtr >        m_logicDescs;
    ParameterMapping< model::IModelNodeEffectPtr >  m_effectDescs;
    ParameterMapping< model::IModelNodePtr >        m_nodeDescs;

public:
    explicit                EndUserParamsLogic  ( model::SceneModel * owner );
                            ~EndUserParamsLogic ();

    bool                    AddDescriptor   ( ParameterAddress && param, EndUserParamDescriptor && descriptor );
    bool                    RemoveDescriptor( const ParameterAddress & param );

    void                    Serialize       ( ISerializer & ser ) const;
    void                    Deserialize     ( const IDeserializer & deser );

    EndUserParamDescriptor *      GetDescriptor   ( const ParameterAddress & param );

protected:

    void            NodeAdded           ( bv::IEventPtr evt );
    void            NodeRemoved         ( bv::IEventPtr evt );
    void            NodeMoved           ( bv::IEventPtr evt );
    void            PluginAdded         ( bv::IEventPtr evt );
    void            PluginMoved         ( bv::IEventPtr evt );
    void            PluginRemoved       ( bv::IEventPtr evt );
    void            LogicAdded          ( bv::IEventPtr evt );
    void            LogicRemoved        ( bv::IEventPtr evt );
    void            EffectAdded         ( bv::IEventPtr evt );
    void            EffectRemoved       ( bv::IEventPtr evt );

    void            LightAdded          ( bv::IEventPtr evt );
    void            LightRemoved        ( bv::IEventPtr evt );
    void            CameraAdded         ( bv::IEventPtr evt );
    void            CameraRemoved       ( bv::IEventPtr evt );
};


}	// bv
