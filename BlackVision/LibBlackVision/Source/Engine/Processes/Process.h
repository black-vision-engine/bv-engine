#pragma once

#include <memory>
#include "CoreDEF.h"

namespace bv
{

class Process;

DEFINE_PTR_TYPE( Process )
DEFINE_WEAK_PTR_TYPE( Process )

enum ProcessState : int
{
    PS_UNINITIALIZED = 0, 
    PS_REMOVED,  
    PS_RUNNING,
    PS_PAUSED,
    PS_SUCCEEDED, 
    PS_FAILED,
    PS_ABORTED,

    PS_TOTAL
};

class Process
{
private:

    friend class ProcessManager;
	
private:

    ProcessState    m_state;
    
    ProcessPtr      m_childProcess;

public:

                    Process     ();
    virtual         ~Process    ();
	
protected:

    virtual void    OnUpdate    ( unsigned long millis ) = 0;
    
    virtual void    OnInit      ();
    virtual void    OnSuccess   ();
    virtual void    OnFail      ();
    virtual void    OnAbort     ();

public:

    ProcessState    GetState    () const;

    bool            IsAlive     () const;
    bool            IsDead      () const;
    bool            IsRemoved   () const;
    bool            IsPaused    () const;

    void            Succeed     ();
    void            Fail        ();
    
    void            Pause       ();
    void            UnPause     ();
    
    void            AttachChild ( ProcessPtr child );
    ProcessPtr      RemoveChild ();
    ProcessPtr      PeekChild   ();

private:

    void            SetState    ( ProcessState state );

};

} //bv
