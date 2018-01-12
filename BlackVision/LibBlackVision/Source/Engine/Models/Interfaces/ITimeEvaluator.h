#pragma once

#include <vector>
#include <string>
#include <memory>

#include "CoreDEF.h" //FIXME: store in PCH

#include "Serialization/ISerializable.h"
#include "Engine/Interfaces/ICloneable.h"

namespace bv { namespace model {

class ITimeEvaluator;
DEFINE_PTR_TYPE(ITimeEvaluator)

class ITimeEvaluator : public ISerializable, public ICloneable
{
public:
    virtual void				                        SetName         ( std::string )					= 0;
    virtual const std::string &                         GetName         () const						= 0;

    virtual void                                        SetGlobalTime   ( TimeType t )					= 0;
    virtual TimeType                                    GetLocalTime    () const						= 0;

    virtual void                                        AddChild        ( ITimeEvaluatorPtr child )		= 0;

    virtual ITimeEvaluatorPtr                           GetChild        ( const std::string & name )	= 0;
    virtual const std::vector< ITimeEvaluatorPtr > &    GetChildren     () = 0;

    virtual bool                                        RemoveChild     ( ITimeEvaluatorPtr child )		= 0;
    virtual bool                                        RemoveChild     ( const std::string & name )	= 0;

    virtual                                             ~ITimeEvaluator () {}


    virtual const std::string&                          GetType         () const = 0;
};

} //model
} //bv
