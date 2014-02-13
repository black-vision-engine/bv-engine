#pragma once

#include <cassert>

#include "Engine/Models/Plugins/Interfaces/IParameter.h"
#include "Engine/Interfaces/ITimeEvaluator.h"
#include "Engine/Types/Enums.h"

#include "System/BasicTypes.h"


namespace bv { namespace model {

class ITimeEvaluator;

class AbstractModelParameter : public IParameter
{
protected:

    const ITimeEvaluator *  m_timeEvaluator;

    std::string             m_name;
    ModelParamType          m_type;

protected:

    explicit                    AbstractModelParameter  ( const std::string & name, ModelParamType type, const ITimeEvaluator * evaluator = nullptr );
    virtual                     ~AbstractModelParameter ();

    template< typename DerivedClass >
    DerivedClass *              QueryImpl               ()
    {
        return static_cast< DerivedClass * >( QueryParamTyped() );
    }

public:

    virtual const std::string & GetName                 () const;
    virtual ModelParamType      GetType                 () const;

    inline void                 SetTimeEvaluator        ( const ITimeEvaluator * timeEvaluator );

};

} //model
} //bv

#include "AbstractModelParameter.inl"
