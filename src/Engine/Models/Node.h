#pragma once

#include <iostream>


namespace bv {

class SceneNode;

class IModelNode
{
public:

    virtual SceneNode *     BuildScene  ()                                          = 0;
    virtual void            Print       ( std::ostream& out, int tabs = 0 ) const   = 0;

    virtual void            Update      ( TimeType t )                                 = 0;
    virtual bool            IsVisible   ( TimeType t ) const                           = 0;

    virtual                 ~IModelNode () {};

};

} //bv