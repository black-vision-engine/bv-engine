#include "ProcessManager.h"

#include <cassert>


namespace bv
{

// ******************************
//
ProcessManager::ProcessManager                  ()
{
}

// ******************************
//
ProcessManager::~ProcessManager                 ()
{
    ClearAll();
}

// ******************************
//
ProcessWeakPtr  ProcessManager::Attach          ( ProcessPtr process )
{
    m_processList.push_front( process );

    return ProcessWeakPtr( process );
}

// ******************************
//
void            ProcessManager::Update          ( unsigned long millis )
{
    for( auto it = m_processList.begin(); it != m_processList.end(); )
    {
        ProcessPtr process = (*it);
        
        auto curIt = it;
        ++it;
    
        if( process->GetState() == ProcessState::PS_UNINITIALIZED )
        {
            process->OnInit();
        }

        if( process->GetState() == ProcessState::PS_RUNNING )
        {
            process->OnUpdate( millis ); 
        }

        if( process->IsDead() )
        {
            switch( process->GetState() )
            {
                case ProcessState::PS_SUCCEEDED:
                {
                    process->OnSuccess();
                
                    ProcessPtr child = process->RemoveChild(); 

                    if( child )
                    {
                        Attach( child );
                    }

                    break;
                }
                case ProcessState::PS_FAILED:
                    process->OnFail();
                    break;
                case ProcessState::PS_ABORTED:
                    process->OnAbort();
                    break;
                default:
                    assert( false );
            }
        
            m_processList.erase( curIt );
        }
    }
}

// ******************************
//FIXME: make sure that no processes are attached ahile AbortAll takes place
void            ProcessManager::AbortAll        ( bool immediateFlag )
{
    for( auto process : m_processList )
    {
        if( process->IsAlive() )
        {
            process->SetState( ProcessState::PS_ABORTED );

            if( immediateFlag )
            {
                process->OnAbort();
            }
        }
    }

    ClearAll();
}

// ******************************
//
SizeType    ProcessManager::GetProcessCount () const
{
    return m_processList.size();
}

// ******************************
//
void            ProcessManager::ClearAll        ()
{
    m_processList.clear();
}

} //bv
