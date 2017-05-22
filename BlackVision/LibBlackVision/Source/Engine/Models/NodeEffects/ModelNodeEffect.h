#pragma once

#include "Engine/Models/Interfaces/IModelNodeEffect.h"
#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"

#include "Assets/Assets.h"


namespace bv { namespace model {

class ModelNodeEffect;
DEFINE_PTR_TYPE( ModelNodeEffect )
DEFINE_CONST_PTR_TYPE( ModelNodeEffect )

class ModelNodeEffect : public IModelNodeEffect
{
protected:

    NodeEffectType                  m_type;

    //FIXME: ParamValModel should be replaced with separate models for FSE & Pre/PostFSELogic
    DefaultParamValModelPtr         m_paramValModel;

    AssetDescVec                    m_assetsDescs;

public:

    static std::string                  EFFECT_ENABLED_PARAM_NAME;

                                        ModelNodeEffect     ( NodeEffectType type );
                                        ModelNodeEffect     ( NodeEffectType type, DefaultParamValModelPtr model );
                                        ~ModelNodeEffect    ();

    virtual void                        Serialize           ( ISerializer & ser ) const override;

    virtual void                        Update              ( TimeType t ) override;
    
    virtual NodeEffectType              GetType		        () const override;

    virtual UInt32                                  NumRequiredAssets   () const override;
    virtual bool                                    AddAsset            ( const AssetDescConstPtr & assetDesc, SizeType idx ) override;
    virtual bool                                    RemoveAsset         ( SizeType idx ) override;
    virtual AssetDescVec                            GetAssets           () const;

    virtual void                                    RegisterEvaluator   ( IParamValEvaluatorPtr paramEval ) override;
    
    virtual IParameterPtr                           GetParameter        ( const std::string & name ) const override;
    virtual const std::vector< IParameterPtr > &    GetParameters       () const override;
    virtual const std::vector< IValueConstPtr > &   GetValues           () const override;

    virtual bool                                    IsEnabled           () const override;

    static ModelNodeEffectPtr						Create              ( NodeEffectType type );
    static ModelNodeEffectPtr						Create              ( NodeEffectType type, DefaultParamValModelPtr model );
    static ISerializablePtr							Create              ( const IDeserializer & doc );
    static ModelNodeEffectPtr						CreateTyped 		( const IDeserializer & deser );
};

} // model
} // bv
