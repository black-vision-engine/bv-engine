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

class BoundingBoxLogic;

DEFINE_PTR_TYPE( BoundingBoxLogic );
DEFINE_CONST_PTR_TYPE( BoundingBoxLogic );



class BoundingBoxLogic : public model::GizmoLogicBase, public std::enable_shared_from_this< BoundingBoxLogic >
{
private:

    struct BoxInfo
    {
        glm::vec3       Size;
        glm::vec3       Center;

        // ***********************
        //
        BoxInfo()
            :   Size( glm::vec3( 0.0f ) )
            ,   Center( glm::vec3( 0.0f ) )
        {}

    };

private:

    static const std::string            m_type;

    struct PARAMETERS
    {
        static const std::string        BOX_COLOR;
        static const std::string        CENTER_COLOR;
        static const std::string        CENTER_SIZE;
    };

private:

    model::BasicNodeWeakPtr             m_centerNode;
    model::BasicNodeWeakPtr             m_bbNode;

    model::ValueParamState< glm::vec4 >     m_boxColor;
    model::ValueParamState< glm::vec4 >     m_centerColor;
    model::ValueParamState< float >         m_centerSize;

public:

    explicit    BoundingBoxLogic        ( model::BasicNodeWeakPtr gizmoRoot, model::BasicNodeWeakPtr gizmoOwner, model::ITimeEvaluatorPtr timeEvaluator );
    virtual    ~BoundingBoxLogic        ();

    virtual void                        Initialize			()				override;
    virtual void                        Update				( TimeType t )	override;
    virtual void                        Deinitialize		()				override;

    virtual void                        CreateGizmoSubtree  ( BVProjectEditor * editor ) override;

    virtual const std::string &         GetType         	() const override;
    static const std::string &          Type            	();

private:

    static void            SetTranslation          ( model::BasicNodePtr node, const glm::vec3 & transform, TimeType time = 0.0f );
    static void            SetColor                ( model::BasicNodePtr node, const glm::vec4 & transform, TimeType time = 0.0f );
    static void            SetCenterSize           ( model::BasicNodePtr node, float size, TimeType time = 0.0f );
    static void            SetBoxSize              ( model::BasicNodePtr node, const glm::vec3 & size, TimeType time = 0.0f );

    static BoxInfo         ComputeBox              ( model::BasicNodePtr node );

private:

    void            NodeRemovedHandler      ( IEventPtr evt );

};


}   // nodelogic
}	// bv

