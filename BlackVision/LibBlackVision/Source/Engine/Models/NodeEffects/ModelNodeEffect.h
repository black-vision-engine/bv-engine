#pragma once

#include "Engine/Models/Interfaces/IModelNodeEffect.h"
#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"


namespace bv { namespace model {

class ModelNodeEffect : public IModelNodeEffect
{
protected:

    NodeEffectType                  m_type;

    //FIXME: ParamValModel should be replaced with separate models for FSE & Pre/PostFSELogic
    DefaultParamValModelPtr         m_paramValModel;

public:

                                        ModelNodeEffect     ( NodeEffectType type );
                                        ~ModelNodeEffect    ();

    virtual void                        Serialize           ( ISerializer & ser ) const override;

    virtual void                        Update              ( TimeType t ) override;
    
    virtual NodeEffectType			    GetType		        () const override;


    virtual void                                    RegisterEvaluator   ( IParamValEvaluatorPtr paramEval ) override;
    virtual IParameterPtr                           GetParameter        ( const std::string & name ) const override;
    virtual const std::vector< IParameterPtr > &    GetParameters       () const override;
    virtual const std::vector< IValueConstPtr > &   GetValues           () const override;


    static IModelNodeEffectPtr          Create              ( NodeEffectType type );

};

} // model
} // bv
