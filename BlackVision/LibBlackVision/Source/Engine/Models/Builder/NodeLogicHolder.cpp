#include "stdafx.h"

#include "Engine/Models/Builder/NodeLogicHolder.h"

namespace bv
{

namespace
{


class INodeLogicHolder
{
private:
    static INodeLogicFactory *          factory;
public:
    INodeLogicHolder();
    virtual ~INodeLogicHolder();

    static void                         SetFactory      ( INodeLogicFactory * newFactory );
    static INodeLogicFactory *          GetFactory      ();
};

static INodeLogicHolder     holder;
INodeLogicFactory *         INodeLogicHolder::factory = nullptr;

// ***********************
//
INodeLogicHolder::INodeLogicHolder()
{
    factory = nullptr;
}

// ***********************
//
INodeLogicHolder::~INodeLogicHolder()
{
    delete factory;
}

// ***********************
//
void            INodeLogicHolder::SetFactory      ( INodeLogicFactory * newFactory )
{
    if( factory )
        delete factory;
    factory = newFactory;
}

// ***********************
// Creates INodeLogicFactory object in case no one has set instance of derived class.
INodeLogicFactory *          INodeLogicHolder::GetFactory      ()
{
    if( !factory )
        factory = new INodeLogicFactory();
    return factory;
}

} // annonymous


// ***********************
//
INodeLogicFactory *         GetNodeLogicFactory         ()
{
    return INodeLogicHolder::GetFactory();
}


// ***********************
//
void    SetNodeLogicFactory         ( INodeLogicFactory * factory )
{
    INodeLogicHolder::SetFactory( factory );
}



} // bv