#pragma once

#include "EndUserParamDescriptor.h"
#include "Engine/Events/ParamAddress.h"

#include "Engine/Events/EventHandlerHelpers.h"

#include "Engine/Models/BasicNode.h"
#include "Engine/Models/Cameras/CameraModel.h"
#include "Engine/Models/Interfaces/IModelLight.h"

#include "Engine/Models/ModelSceneEditor.h"

#include <unordered_map>
#include <map>



namespace bv
{




template< typename ParamContainerTypePtr >
class ParameterMapping
{
public:

    struct PtrParamAddress
    {
        model::IParameterPtr        Parameter;
        model::BasicNodePtr         Node;
        ParamContainerTypePtr       Container;      // Depends on ParameterAddress::TargetType


        PtrParamAddress()   {}

        // ***********************
        //
        bool        operator<   ( const PtrParamAddress & other ) const
        {
            if( Node.get() < other.Node.get() )
                return true;
            if( Node.get() == other.Node.get() )
            {
                if( Container.get() < other.Container.get() )
                    return true;
                if( Container.get() == other.Container.get() )
                {
                    if( Parameter.get() < other.Parameter.get() )
                        return true;
                }
            }
            return false;
        }
    };

    //typedef std::unordered_map< model::IParameterPtr, EndUserParamDescriptor >      ParamDescriptorMap;
    //typedef std::map< PtrParamAddress, ParameterAddress * >                         PtrParamStrParamMap;
    typedef std::map< ParameterAddress, EndUserParamDescriptor >                    ParamDescriptorMap;
    typedef std::multimap< PtrParamAddress, ParamDescriptorMap::iterator >          ParamPtr2AddressMap;

private:

    model::SceneModel *     m_ownerScene;

    ParamDescriptorMap      m_paramsDescsMap;
    ParamPtr2AddressMap     m_ptr2StrAddressMap;

public:
    explicit ParameterMapping( model::SceneModel * owner )
        : m_ownerScene( owner )
    {}


    bool                    AddDescriptor   ( ParameterAddress && param, EndUserParamDescriptor && descriptor );
    bool                    RemoveDescriptor( const ParameterAddress & param );

    void                    Serialize       ( ISerializer & ser ) const;
    void                    Deserialize     ( const IDeserializer & deser );

    EndUserParamDescriptor *      GetDescriptor   ( const ParameterAddress & param );


private:
    

};

     

/////////////////////////////////////////////////////////////////////////////
// Helpers
/////////////////////////////////////////////////////////////////////////////


// ***********************
//
template< typename ParamContainerTypePtr >
inline ParameterAddress::TargetType                    GetParamTargetType ()
{    return ParameterAddress::TargetType::FailTarget;   }

template<>
inline ParameterAddress::TargetType                    GetParamTargetType< model::CameraModelPtr > ()
{    return ParameterAddress::TargetType::CameraParam;  }

template<>
inline ParameterAddress::TargetType                    GetParamTargetType< model::IModelLightPtr > ()
{    return ParameterAddress::TargetType::LightParam;   }

template<>
inline ParameterAddress::TargetType                    GetParamTargetType< model::BasePluginPtr > ()
{    return ParameterAddress::TargetType::PluginParam;  }

template<>
inline ParameterAddress::TargetType                    GetParamTargetType< model::INodeLogicPtr > ()
{    return ParameterAddress::TargetType::NodeLogicParam;   }

template<>
inline ParameterAddress::TargetType                    GetParamTargetType< model::IModelNodeEffectPtr > ()
{    return ParameterAddress::TargetType::GlobalEffectParam;    }

// ***********************
//
template< typename ParamContainerTypePtr >
inline bool        IsValidParamTargetType  ( ParameterAddress::TargetType target )  {  return target == GetParamTargetType< ParamContainerTypePtr >(); }

template<>
inline bool        IsValidParamTargetType< model::BasePluginPtr >  ( ParameterAddress::TargetType target )
{    return target == ParameterAddress::TargetType::PluginParam || target == ParameterAddress::TargetType::ResourceParam;    }


// ***********************
//
template< typename ParamContainerTypePtr >
inline ParamContainerTypePtr           GetParamContainer                           ( model::SceneModel * /*owner*/, model::BasicNodePtr & /*node*/, const ParameterAddress & /*param*/ )
{
    static_assert( false, "Specialize" );
    return nullptr;
}

// ***********************
//
template<>
inline model::CameraModelPtr           GetParamContainer< model::CameraModelPtr >  ( model::SceneModel * owner, model::BasicNodePtr & /*node*/, const ParameterAddress & param )
{   return owner->GetCamerasLogic().GetCamera( param.Index );    }

// ***********************
//
template<>
inline model::IModelLightPtr           GetParamContainer< model::IModelLightPtr >  ( model::SceneModel * owner, model::BasicNodePtr & /*node*/, const ParameterAddress & param )
{    return owner->GetLight( param.Index );  }

// ***********************
//
template<>
inline model::BasePluginPtr            GetParamContainer< model::BasePluginPtr >   ( model::SceneModel * /*owner*/, model::BasicNodePtr & node, const ParameterAddress & param )
{    return std::static_pointer_cast< model::BasePlugin >( node->GetPlugin( param.PluginName ) );    }

// ***********************
//
template<>
inline model::INodeLogicPtr            GetParamContainer< model::INodeLogicPtr >   ( model::SceneModel * /*owner*/, model::BasicNodePtr & node, const ParameterAddress & /*param*/ )
{    return node->GetLogic();   }

// ***********************
//
template<>
inline model::IModelNodeEffectPtr      GetParamContainer< model::IModelNodeEffectPtr > ( model::SceneModel * /*owner*/, model::BasicNodePtr & node, const ParameterAddress & /*param*/ )
{    return node->GetNodeEffect();  }



// ***********************
//
template< typename ParamContainerTypePtr >
inline std::string                      GetParameterMappingSerializationName   ()
{
    static_assert( false, "Specialize" );
    return "";
}

template<> inline std::string                      GetParameterMappingSerializationName< model::IModelLightPtr >        ()   { return "LightParamsDescriptors"; }
template<> inline std::string                      GetParameterMappingSerializationName< model::BasePluginPtr >         ()   { return "PluginParamsDescriptors"; }
template<> inline std::string                      GetParameterMappingSerializationName< model::INodeLogicPtr >         ()   { return "LogicParamsDescriptors"; }
template<> inline std::string                      GetParameterMappingSerializationName< model::IModelNodeEffectPtr >   ()   { return "EffectParamsDescriptors"; }
template<> inline std::string                      GetParameterMappingSerializationName< model::CameraModelPtr >        ()   { return "CameraParamsDescriptors"; }



// ***********************
//
template< typename ParamContainerTypePtr >
inline typename ParameterMapping< ParamContainerTypePtr >::PtrParamAddress         FindParameter   ( model::SceneModel * owner, const ParameterAddress & param )
{
    ParameterMapping< ParamContainerTypePtr >::PtrParamAddress ptrAddress;

    ptrAddress.Node = std::static_pointer_cast< model::BasicNode >( owner->GetModelSceneEditor()->GetNode( param.NodeName ) );       // Nullptr in case of Light or Camera.
    ptrAddress.Parameter = GetParameter( owner, param );
    ptrAddress.Container = GetParamContainer< ParamContainerTypePtr >( owner, ptrAddress.Node, param );

    return ptrAddress;
}


/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////


// ***********************
//
template< typename ParamContainerTypePtr >
inline bool             ParameterMapping< ParamContainerTypePtr >::AddDescriptor    ( ParameterAddress && param, EndUserParamDescriptor && descriptor )
{
    if( !IsValidParamTargetType< ParamContainerTypePtr >( param.ParamTargetType ) )
        return false;

    PtrParamAddress paramPtr = FindParameter< ParamContainerTypePtr >( m_ownerScene, param );
    if( paramPtr.Parameter != nullptr )
    {
        auto iter = m_paramsDescsMap.insert( std::make_pair( std::move( param ), std::move( descriptor ) ) );

        auto insertValue = std::make_pair( paramPtr, iter.first );
        m_ptr2StrAddressMap.insert( insertValue );
        return true;
    }

    return true;
}

// ***********************
//
template< typename ParamContainerTypePtr >
inline bool             ParameterMapping< ParamContainerTypePtr >::RemoveDescriptor ( const ParameterAddress & param )
{
    if( !IsValidParamTargetType< ParamContainerTypePtr >( param.ParamTargetType ) )
        return false;

    PtrParamAddress paramPtr = FindParameter< ParamContainerTypePtr >( m_ownerScene, param );
    
    auto descIter = m_paramsDescsMap.find( param );
    auto range = m_ptr2StrAddressMap.equal_range( paramPtr );

    if( descIter == m_paramsDescsMap.end() ) return false;

    for( auto ptrIter = range.first; ptrIter != range.second; ptrIter++ )
    {
        if( ptrIter->second == descIter )
            ptrIter = m_ptr2StrAddressMap.erase( ptrIter );
    }

    m_paramsDescsMap.erase( descIter );
    return true;
}

// ***********************
//
template< typename ParamContainerTypePtr >
inline void             ParameterMapping< ParamContainerTypePtr >::Serialize        ( ISerializer & ser ) const
{
    ser.EnterArray( GetParameterMappingSerializationName< ParamContainerTypePtr >() );

        for( auto & paramMapping : m_paramsDescsMap )
        {
            ser.EnterChild( "mapping" );
                ser.EnterChild( "param" );
                paramMapping.first.Serialize( ser );
                ser.ExitChild();    // param

                paramMapping.second.Serialize( ser );
            ser.ExitChild();    // mapping
        }

    ser.ExitChild();    // endUserParams
}

// ***********************
//
template< typename ParamContainerTypePtr >
inline void             ParameterMapping< ParamContainerTypePtr >::Deserialize      ( const IDeserializer & deser )
{
    if( deser.EnterChild( GetParameterMappingSerializationName< ParamContainerTypePtr >() ) )
    {
        if( deser.EnterChild( "mapping" ) )
        {
            do
            {
                bool paramValid = false;
                bool descriptorValid = false;

                ParameterAddress param;
                if( deser.EnterChild( "param" ) )
                {
                    param = ParameterAddress::Create( deser );

                    deser.ExitChild();  // param
                    paramValid = true;
                }

                EndUserParamDescriptor descriptor;
                if( deser.EnterChild( "paramDescriptor" ) )
                {
                    descriptor = EndUserParamDescriptor::Create( deser );

                    deser.ExitChild();  // paramDescriptor
                    descriptorValid = true;
                }

                if( paramValid && descriptorValid )
                    AddDescriptor( std::move( param  ), std::move( descriptor ) );

            } while( deser.NextChild() );

            deser.ExitChild();  // mapping
        }
        deser.ExitChild();  // endUserParams
    }
}

// ***********************
//
template< typename ParamContainerTypePtr >
inline EndUserParamDescriptor *     ParameterMapping< ParamContainerTypePtr >::GetDescriptor    ( const ParameterAddress & param )
{
    if( !IsValidParamTargetType< ParamContainerTypePtr >( param.ParamTargetType ) )
        return nullptr;

    auto result = m_paramsDescsMap.find( param );
    if( result != m_paramsDescsMap.end() )
        return &m_paramsDescsMap[ param ];
    return nullptr;
}


}	// bv


