#pragma once

#include "CoreDEF.h"
#include "Engine/Models/Gizmos/GizmoLogicBase.h"
#include "Engine/Types/Values/TypedValues.h"
#include "Engine/Events/Interfaces/IEvent.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"




namespace bv
{

namespace model
{

class BasicNode;
DEFINE_PTR_TYPE( BasicNode );
DEFINE_WEAK_PTR_TYPE( BasicNode );

} // model




namespace nodelogic
{

class ShowFPS;

DEFINE_PTR_TYPE( ShowFPS );
DEFINE_CONST_PTR_TYPE( ShowFPS );



class ShowFPS : public model::GizmoLogicBase, public std::enable_shared_from_this< ShowFPS >
{
private:

    static const std::string            m_type;

    struct PARAMETERS
    {
        static const std::string    REFRESH_FREQUENCY;
    };

private:
    
    model::BasicNodeWeakPtr                 m_fpsNode;

    model::ValueParamState< Int32 >         m_refreshFrequency;
    Int32                                   m_numFramesSinceRefresh;
    UInt64                                  m_lastRefreshTime;

public:

    explicit    ShowFPS			( model::BasicNodeWeakPtr gizmoRoot, model::BasicNodeWeakPtr gizmoOwner, model::ITimeEvaluatorPtr timeEvaluator );
    virtual     ~ShowFPS		();


    static model::IGizmoLogicPtr        Create          ( model::BasicNodeWeakPtr gizmoRoot, model::BasicNodeWeakPtr gizmoOwner, model::ITimeEvaluatorPtr timeEvaluator );

    virtual void                        Initialize		()				override;
    virtual void                        Update			( TimeType t )	override;
    virtual void                        Deinitialize	()				override;

    virtual void                        CreateGizmoSubtree  ( BVProjectEditor * editor ) override;

    virtual const std::string &         GetType         () const override;
    static const std::string &          Type            ();

private:

    void            SetTranslation          ( model::BasicNodePtr node, const glm::vec3 & transform, TimeType time = 0.0f );
    void            SetText                 ( model::BasicNodePtr node, const std::wstring & text, TimeType time = 0.0f );

    std::wstring    FormatFPS               ( float fps );

private:

    void            NodeRemovedHandler      ( IEventPtr evt );
    
};


}   // nodelogic
}	// bv


