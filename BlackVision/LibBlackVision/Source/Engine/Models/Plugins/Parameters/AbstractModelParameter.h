#pragma once

#include <cassert>

#include "Engine/Models/Plugins/Interfaces/IParameter.h"
#include "Engine/Types/Enums.h"

#include "CoreDEF.h"
#include <Serialization/ISerializable.h>


namespace bv { namespace model {

class AbstractModelParameter;
DEFINE_PTR_TYPE(AbstractModelParameter)

class AbstractModelParameter : public IParameter, public std::enable_shared_from_this< AbstractModelParameter >, public ISerializable
{
protected:

    ITimeEvaluatorPtr       m_timeEvaluator;

    std::string             m_name;
    ModelParamType          m_type;
    InterpolationMethod     m_method;

protected:

    explicit                    AbstractModelParameter  ( const std::string & name, ModelParamType type, ITimeEvaluatorPtr evaluator );
    virtual                     ~AbstractModelParameter ();

public:
    virtual void                Serialize       ( ISerializer& doc ) const override = 0;
    static ISerializablePtr     Create          ( ISerializer& doc );

    virtual const std::string & GetName                 () const override;
    virtual ModelParamType      GetType                 () const override;

	virtual void                SetInterpolationMethod ( InterpolationMethod method ) override;
	virtual InterpolationMethod GetInterpolationMethod () const override;

    virtual ITimeEvaluatorPtr   GetTimeEvaluator        () override;
    virtual void                SetTimeEvaluator        ( ITimeEvaluatorPtr timeEvaluator ) override;

    inline  TimeType            GetLocalEvaluationTime  () const;

};

} //model
} //bv

#include "AbstractModelParameter.inl"
