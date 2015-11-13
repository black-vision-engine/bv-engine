#pragma once

#include <cassert>

#include "Engine/Models/Plugins/Interfaces/IParameter.h"
#include "Engine/Types/Enums.h"

#include "CoreDEF.h"
#include <Serialization/ISerializable.h>

#include "Mathematics/Interpolators/InterpolatorBasicTypes.h"

namespace bv { namespace model {

class AbstractModelParameter;
DEFINE_PTR_TYPE(AbstractModelParameter)

class AbstractModelParameter : public IParameter, public std::enable_shared_from_this< AbstractModelParameter >, public ISerializable
{
protected:

    ITimeEvaluatorPtr           m_timeEvaluator;

    std::string                 m_name;
    ModelParamType              m_type;

protected:

    explicit                    AbstractModelParameter  ( const std::string & name, ModelParamType type, ITimeEvaluatorPtr evaluator );
    virtual                     ~AbstractModelParameter ();

public:
    virtual void                Serialize               ( ISerializer& doc ) const override = 0;
    static ISerializablePtr     Create                  ( const IDeserializer& doc );

    virtual const std::string & GetName                 () const override;
    virtual ModelParamType      GetType                 () const override;

    virtual void                SetCurveType            ( CurveType type ) = 0;
    virtual CurveType           GetCurveType            () = 0;

    virtual void                SetWrapPostMethod       ( WrapMethod method ) = 0;
    virtual void                SetWrapPreMethod        ( WrapMethod method ) = 0;
    virtual WrapMethod          GetWrapPostMethod       () = 0;
    virtual WrapMethod          GetWrapPreMethod        () = 0;

    virtual int                 GetNumKeys              () = 0;

    virtual ITimeEvaluatorPtr   GetTimeEvaluator        () override;
    virtual void                SetTimeEvaluator        ( ITimeEvaluatorPtr timeEvaluator ) override;

    inline  TimeType            GetLocalEvaluationTime  () const;

};

} //model
} //bv

#include "AbstractModelParameter.inl"
