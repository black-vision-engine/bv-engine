#pragma once

namespace bv
{

//FIXME: possibly add an interface such as IAppLogic (if necessary)
class BVAppLogic
{
private:

    unsigned long   m_startTime;

public:

                    BVAppLogic      ();
                    ~BVAppLogic     ();

    void            SetStartTime    ( unsigned long millis );
    virtual void    OnUpdate        ( unsigned long millis );

    //FIXME: add if necessary
    //virtual void VChangeState(state)=0;

};

} //bv
