#pragma once

#include "CoreDEF.h"
#include "Engine/Models/NodeLogics/NodeLogicBase.h"
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

class Cloner;

DEFINE_PTR_TYPE( Cloner );
DEFINE_CONST_PTR_TYPE( Cloner );

enum class ClonerPlaneType
{
    CPT_XY = 0,
    CPT_XZ,
    CPT_YZ
};

class Cloner : public model::NodeLogicBase, public std::enable_shared_from_this< Cloner >
{
private:
    static const std::string            m_type;

    struct ACTION
    {
        static const std::string    REGENERATE;
        static const std::string    REMOVE_ONLY_EXCESS;
        static const std::string    REMOVE_ALL_CLONES;
    };

    struct PARAMETERS
    {
        static const std::string    N_ROWS;
        static const std::string    N_COLS;
        static const std::string    DELTA;
        static const std::string    RENAME_SUBTREE;
        static const std::string    REMOVE_EXCESS;
        static const std::string    PLANE_TYPE;
    };

private:
    bv::model::BasicNodeWeakPtr			        m_parentNode;

    model::ValueParamState< Int32 >             m_numRows;
    model::ValueParamState< Int32 >             m_numCols;
    model::ValueParamState< glm::vec3 >         m_delta;
    model::ValueParamState< bool >              m_renameSubtree;
    model::ValueParamState< bool >              m_removeExcees;
    model::ValueParamState< ClonerPlaneType >   m_planeType;

    bool                                        m_updateClonesNeeded; 
    bool                                        m_updatePositionsNeeded;    

public:
    explicit    Cloner			( bv::model::BasicNodeWeakPtr parent, bv::model::ITimeEvaluatorPtr timeEvaluator );
    ~Cloner		();

    virtual void                        Initialize		()				override;
    virtual void                        Update			( TimeType t )	override;
    virtual void                        Deinitialize	()				override;


    virtual const std::string &         GetType         () const override;
    static const std::string &          Type            ();

    virtual void                        Serialize       ( ISerializer & ser ) const override;
    static ClonerPtr			        Create          ( const IDeserializer & deser, bv::model::BasicNodeWeakPtr parentNode );

    virtual bool                        HandleEvent     ( IDeserializer & eventDeser, ISerializer & response, BVProjectEditor * editor ) override;

private:
    void                    NodeRemovedHandler      ( IEventPtr evt );

    void                    UpdateClones            ();
    void                    UpdatePositions         ();

    void                    CloneNode               ( UInt32 clonesNum ) const;

    glm::vec3               Transform2Plane         ( const glm::vec3 & v, ClonerPlaneType plane ) const;
    void                    RemoveClones            ();
    void                    RemoveExcessNodes       ();
    void                    Regenerate              ();
};


}   // nodelogic
}	// bv

