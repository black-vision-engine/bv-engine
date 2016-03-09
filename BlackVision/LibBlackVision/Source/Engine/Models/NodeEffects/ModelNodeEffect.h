#pragma once

#include "Engine/Models/Interfaces/IModelNodeEffect.h"
#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"

#include "Assets/Assets.h"

namespace bv { namespace model {

class ModelNodeEffect : public IModelNodeEffect
{
protected:

    NodeEffectType                  m_type;

    //FIXME: ParamValModel should be replaced with separate models for FSE & Pre/PostFSELogic
    DefaultParamValModelPtr         m_paramValModel;

    AssetDescVec                    m_assetsDescs;

public:

                                        ModelNodeEffect     ( NodeEffectType type );
                                        ~ModelNodeEffect    ();

    virtual void                        Serialize           ( ISerializer & ser ) const override;

    virtual void                        Update              ( TimeType t ) override;
    
    virtual NodeEffectType			    GetType		        () const override;

    virtual bool                                    AddAsset            ( const AssetDescConstPtr & assetDesc ) override;
    virtual AssetDescVec                            GetAssets           () const;
    virtual void                                    RegisterEvaluator   ( IParamValEvaluatorPtr paramEval ) override;
    virtual IParameterPtr                           GetParameter        ( const std::string & name ) const override;
    virtual const std::vector< IParameterPtr > &    GetParameters       () const override;
    virtual const std::vector< IValueConstPtr > &   GetValues           () const override;


    static IModelNodeEffectPtr          Create              ( NodeEffectType type );

};

} // model
} // bv
