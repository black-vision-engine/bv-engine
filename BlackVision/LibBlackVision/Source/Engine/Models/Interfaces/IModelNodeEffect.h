#pragma once

#include "CoreDEF.h"

#include "Engine/Interfaces/IUpdatable.h"
#include "Engine/Models/Plugins/Interfaces/IParameter.h"
#include "Engine/Interfaces/IValue.h"
#include "Engine/Models/Plugins/Interfaces/IParamValEvaluator.h"

#include "Assets/AssetDescriptor.h"

#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/NodeEffect.h"




/**@page NodeEffects Effects

@section Description
You may think of effect as a way to change the normal rendering procedure.

@section Examples
Examples of usage:
- change order of rendering (see eg. @ref ZSortEffect)
- use additonal Render Targets

@section NodeEffectsAPIManipulation API Manipulation
@copydoc API_EffectsManipulation

@see bv::model::IModelNodeEffect
*/



namespace bv { namespace model {



/**@brief Node effect interface.
@copydoc NodeEffects
@ingroup Model*/
class IModelNodeEffect : public IUpdatable, public ISerializable
{
public:

    virtual                                         ~IModelNodeEffect       () {};

    virtual void                                    Serialize               ( ISerializer & ser ) const = 0;

    virtual void                                    Update                  ( TimeType t ) = 0;
    virtual NodeEffectType			                GetType			        () const = 0;

    virtual UInt32                                  NumRequiredAssets       () const = 0;
    virtual bool                                    RemoveAsset             ( SizeType idx ) = 0;
    virtual bool                                    AddAsset                ( const AssetDescConstPtr & assetDesc, SizeType idx ) = 0;
    virtual AssetDescVec                            GetAssets               () const = 0;

    virtual void                                    RegisterEvaluator       ( IParamValEvaluatorPtr paramEval ) = 0;
    
    virtual IParameterPtr                           GetParameter            ( const std::string & name ) const = 0;
    virtual const std::vector< IParameterPtr > &    GetParameters           () const = 0;
    
    virtual const std::vector< IValueConstPtr > &   GetValues               () const = 0;

    virtual bool                                    IsEnabled               () const = 0;

};

DEFINE_PTR_TYPE(IModelNodeEffect)
DEFINE_CONST_PTR_TYPE(IModelNodeEffect)

} // model
} // bv
