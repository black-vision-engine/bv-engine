#pragma once

#include <cassert>

#include "Engine/Models/Plugins/Interfaces/IParameter.h"
#include "Engine/Types/Enums.h"

#include "CoreDEF.h"


namespace bv { namespace model {

class AbstractModelParameter;
DEFINE_PTR_TYPE(AbstractModelParameter)

class AbstractModelParameter : public IParameter, public std::enable_shared_from_this< AbstractModelParameter >
{
protected:

    ITimeEvaluatorPtr       m_timeEvaluator;

    std::string             m_name;
    ModelParamType          m_type;

protected:

    explicit                    AbstractModelParameter  ( const std::string & name, ModelParamType type, ITimeEvaluatorPtr evaluator );
    virtual                     ~AbstractModelParameter ();

public:

    virtual const std::string & GetName                 () const override;
    virtual ModelParamType      GetType                 () const override;

    virtual ITimeEvaluatorPtr   GetTimeEvaluator        () override;
    virtual void                SetTimeEvaluator        ( ITimeEvaluatorPtr timeEvaluator ) override;

    inline  TimeType            GetLocalEvaluationTime  () const;

};

} //model
} //bv

#include "AbstractModelParameter.inl"
