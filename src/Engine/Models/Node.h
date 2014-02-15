#pragma once

#include <iostream>
#include <memory>

#include "Engine/Interfaces/IUpdatable.h"

namespace bv {

class SceneNode;

class IModelNode : public IUpdatable
{
public:

    virtual SceneNode *         BuildScene  ()                                          = 0;
    virtual void                Print       ( std::ostream& out, int tabs = 0 ) const   = 0;

    virtual bool                IsVisible   ( TimeType t ) const                        = 0;

    virtual const std::string & GetName     () const                                    = 0;

    virtual                     ~IModelNode () {};

};

typedef std::shared_ptr< IModelNode >       IModelNodePtr;

} //bv