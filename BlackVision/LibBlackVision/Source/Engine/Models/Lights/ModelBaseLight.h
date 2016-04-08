#pragma once

#include "Mathematics/glm_inc.h"
#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"
#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"
#include "Engine/Models/Interfaces/IModelLight.h"
#include "Serialization/SerializationHelper.h"


namespace bv { namespace model {


class ModelBaseLight : public IModelLight
{
protected:

    DefaultParamValModelPtr     m_paramModel;

public:

                                ModelBaseLight      ();

    virtual void                Serialize           ( ISerializer & ser ) const override;
    static ISerializablePtr		Create              ( const IDeserializer & deser );

    virtual void				Update			    ( TimeType t )	override;
    
    virtual std::vector< IParameterPtr > &          GetParameters   () override;
    virtual IParameterPtr                           GetParameter    ( const std::string & name ) override;

    virtual const std::vector< IValueConstPtr > &   GetValues       () const override;
    virtual IValueConstPtr                          GetValue        ( const std::string & name ) const override;

};

DEFINE_PTR_TYPE(ModelBaseLight)
DEFINE_CONST_PTR_TYPE(ModelBaseLight)

} //model
} //bv
