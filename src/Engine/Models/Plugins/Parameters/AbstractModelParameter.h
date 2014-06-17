#pragma once

#include <cassert>

#include "Engine/Models/Plugins/Interfaces/IParameter.h"
#include "Engine/Types/Enums.h"

#include "System/BasicTypes.h"


namespace bv { namespace model {

class AbstractModelParameter;
typedef std::shared_ptr< AbstractModelParameter > AbstractModelParameterPtr;

class AbstractModelParameter : public IParameter
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
