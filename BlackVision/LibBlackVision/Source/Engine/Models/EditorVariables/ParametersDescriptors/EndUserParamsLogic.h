#pragma once

#include "EndUserParamDescriptor.h"
#include "Engine/Events/ParamAddress.h"

#include <map>



namespace bv
{


class ISerializer;
class IDeserializer;



class EndUserParamsLogic
{
public:

    //union ParamContainer
    //{
    //    model::BasePluginPtr        Plugin;
    //    model::IModelLightPtr       Light;
    //    model::CameraModelPtr       Camera;
    //    model::INodeLogicPtr        Logic;
    //    model::IModelNodeEffectPtr  Effect;
    //};

    //struct PointerParamAddres
    //{
    //    model::BasicNodePtr         Node;
    //    ParamContainer              Container;      // Depends on ParameterAddress::TargetType
    //    model::IParameterPtr        Parameter;
    //};

private:

    std::map< ParameterAddress, EndUserParamDescriptor >    m_paramsDescsMap;

public:
    explicit                EndUserParamsLogic();
                            ~EndUserParamsLogic();

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
