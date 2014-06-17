#pragma once

#include <vector>
#include <string>
#include <memory>

#include "System/BasicTypes.h" //FIXME: store in PCH


namespace bv { namespace model {

class ITimeEvaluator;
DEFINE_PTR_TYPE(ITimeEvaluator)

class ITimeEvaluator
{
public:

    virtual const std::string &                         GetName         () const        = 0;

    virtual void                                        SetGlobalTime   ( TimeType t )  = 0;
    virtual TimeType                                    GetLocalTime    () const        = 0;

    virtual void                                        AddChild        ( ITimeEvaluatorPtr child ) = 0;

    virtual ITimeEvaluatorPtr                           GetChild        ( const std::string & name ) = 0;
    virtual const std::vector< ITimeEvaluatorPtr > &    GetChildren     () = 0;

    virtual bool                                        RemoveChild     ( ITimeEvaluatorPtr child ) = 0;
    virtual bool                                        RemoveChild     ( const std::string & name ) = 0;

    virtual                                             ~ITimeEvaluator () {}

};

} //model
} //bv
