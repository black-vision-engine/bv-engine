#pragma once

#include "CoreDEF.h"
#include "Engine/Models/Gizmos/Logics/GizmoLogicBase.h"
#include "Engine/Types/Values/TypedValues.h"
#include "Engine/Events/Interfaces/IEvent.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"


namespace bv
{

namespace model
{

class GridLinesLogic;
class BasicNode;
DEFINE_PTR_TYPE( BasicNode );
DEFINE_WEAK_PTR_TYPE( BasicNode );

} // model




namespace nodelogic
{

class GridLines;

DEFINE_PTR_TYPE( GridLines );
DEFINE_CONST_PTR_TYPE( GridLines );



class GridLines : public model::GizmoLogicBase, public std::enable_shared_from_this< GridLines >
{
private:

    static const std::string            m_type;

    struct PARAMETERS
    {
        static const std::string    LINES_COLOR;
        static const std::string    LINES_LENGTH;
    };

private:

    model::ValueParamState< glm::vec4 >     m_linesColor;
    model::ValueParamState< float >         m_linesLength;

    model::GridLinesLogic *                 m_gridLinesLogic;
    UInt64                                  m_updateID;

public:

    explicit    GridLines         ( model::BasicNodeWeakPtr gizmoRoot, model::BasicNodeWeakPtr gizmoOwner, model::ITimeEvaluatorPtr timeEvaluator );
    virtual    ~GridLines		    ();

    static model::IGizmoLogicPtr        Create          	( model::BasicNodeWeakPtr gizmoRoot, model::BasicNodeWeakPtr gizmoOwner, model::ITimeEvaluatorPtr timeEvaluator );

    virtual void                        Initialize			()				override;
    virtual void                        Update				( TimeType t )	override;
    virtual void                        Deinitialize		()				override;

    virtual void                        CreateGizmoSubtree  ( BVProjectEditor * editor ) override;

    virtual const std::string &         GetType         	() const override;
    static const std::string &          Type            	();

private:

    static void             SetColor                ( model::BasicNodePtr node, const glm::vec4 & transform, TimeType time = 0.0f );

    void                    UpdateLines             ();
    bool                    PropagateUpdate         ();

private:

    void            NodeRemovedHandler      ( IEventPtr evt );

};


}   // nodelogic
}	// bv

