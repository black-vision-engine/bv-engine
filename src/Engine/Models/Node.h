#pragma once
#include <iostream>

namespace bv
{

class SceneNode;

class Node
{
public:
    virtual SceneNode*      BuildScene  ()                                     = 0;
    virtual void            Print       (std::ostream& out, int tabs = 0)   const   = 0;

    virtual void            Update      (float t)                                   = 0;


    virtual ~Node() {};

};

} //bv