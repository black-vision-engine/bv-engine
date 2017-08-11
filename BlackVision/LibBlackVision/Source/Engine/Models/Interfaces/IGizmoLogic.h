#pragma once


#include "INodeLogic.h"




namespace bv
{

class BVProjectEditor;


namespace model
{



// ***********************
//
enum class GizmoType
{
    Scene,
    Node,
    Logic,
    Plugin,
    Effect,

    Total
};





// ***********************
//
class IGizmoLogic : public INodeLogic
{
private:
public:

    virtual void                CreateGizmoSubtree      ( BVProjectEditor  * editor ) = 0;

};

DEFINE_PTR_TYPE( IGizmoLogic )
DEFINE_CONST_PTR_TYPE( IGizmoLogic )


}   // model
}	// bv

