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

    typedef std::unordered_map< model::IParameterPtr, EndUserParamDescriptor >      ParamDescriptorMap;
    typedef std::map< PtrParamAddress, ParameterAddress * >                         PtrParamStrParamMap;
    typedef std::map< ParameterAddress, PtrParamAddress * >                         StrParamPtrParamMap;

private:

    ParamDescriptorMap      m_paramsDescsMap;
    PtrParamStrParamMap     m_ptr2StrAddressMap;
    StrParamPtrParamMap     m_str2PtrAddressMap;

public:

    bool                    AddDescriptor   ( model::SceneModel * owner, ParameterAddress && param, EndUserParamDescriptor && descriptor );
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
inline bool             ParameterMapping< ParamContainerTypePtr >::AddDescriptor    ( model::SceneModel * owner, ParameterAddress && param, EndUserParamDescriptor && descriptor )
{
    if( !IsValidParamTargetType< ParamContainerTypePtr >( param.ParamTargetType ) )
        return false;

    PtrParamAddress paramPtr = FindParameter< ParamContainerTypePtr >( owner, param );

    auto leftIter = m_ptr2StrAddressMap.insert( std::make_pair< PtrParamAddress, ParameterAddress * >( std::move( paramPtr ), nullptr ) );
    auto rightIter = m_str2PtrAddressMap.insert( std::make_pair< ParameterAddress, PtrParamAddress * >( std::move( param ), nullptr ) );

    //assert( leftIter.second );
    //assert( rightIter.second );

    leftIter.first->second = const_cast< ParameterAddress * >( &rightIter.first->first );
    rightIter.first->second = const_cast< PtrParamAddress * >( &leftIter.first->first );

    m_paramsDescsMap[ leftIter.first->first.Parameter ] = std::move( descriptor );

    return true;
}

// ***********************
//
template< typename ParamContainerTypePtr >
inline bool             ParameterMapping< ParamContainerTypePtr >::RemoveDescriptor ( const ParameterAddress & param )
{
    if( !IsValidParamTargetType< ParamContainerTypePtr >( param.ParamTargetType ) )
        return false;

    auto iter = m_str2PtrAddressMap.find( param );
    if( iter == m_str2PtrAddressMap.end() ) return false;

    auto & parameter = iter->second->Parameter;

    auto numErased1 = m_paramsDescsMap.erase( parameter );
    auto numErased2 = m_ptr2StrAddressMap.erase( *( iter->second ) );
    m_str2PtrAddressMap.erase( iter );

    assert( numErased1 == 1 );
    assert( numErased2 == 1 );

    return true;
}

// ***********************
//
template< typename ParamContainerTypePtr >
inline void             ParameterMapping< ParamContainerTypePtr >::Serialize        ( ISerializer & /*ser*/ ) const
{}

// ***********************
//
template< typename ParamContainerTypePtr >
inline void             ParameterMapping< ParamContainerTypePtr >::Deserialize      ( const IDeserializer & /*deser*/ )
{}

// ***********************
//
template< typename ParamContainerTypePtr >
inline EndUserParamDescriptor *     ParameterMapping< ParamContainerTypePtr >::GetDescriptor    ( const ParameterAddress & param )
{
    if( !IsValidParamTargetType< ParamContainerTypePtr >( param.ParamTargetType ) )
        return false;

    auto iter = m_str2PtrAddressMap.find( param );
    if( iter == m_str2PtrAddressMap.end() ) return nullptr;

    return &m_paramsDescsMap[ iter->second->Parameter ];
}


}	// bv


