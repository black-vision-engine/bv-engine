#pragma once

#include "CoreDEF.h"
#include "Engine/Models/NodeLogics/NodeLogicBase.h"
#include "Engine/Types/Values/TypedValues.h"
#include "Engine/Events/Interfaces/IEvent.h"

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



class Cloner : public model::NodeLogicBase, public std::enable_shared_from_this< Cloner >
{
private:
    static const std::string            m_type;

    struct ACTION
    {
        //static const std::string    ACTION_NAME;
    };

    struct PARAMETERS
    {
        //static const std::string    PARAMETER_NAME;
    };

private:
    bv::model::BasicNodeWeakPtr			m_parentNode;


public:
    explicit    Cloner			( bv::model::BasicNodeWeakPtr parent, bv::model::ITimeEvaluatorPtr timeEvaluator );
    ~Cloner		();

    virtual void                        Initialize		()				override;
    virtual void                        Update			( TimeType t )	override;
    virtual void                        Deinitialize	()				override;


    virtual const std::string &         GetType         () const override;
    static const std::string &          Type            ();

    virtual void                        Serialize       ( ISerializer & ser ) const override;
    static ClonerPtr			Create          ( const IDeserializer & deser, bv::model::BasicNodeWeakPtr parentNode );

    virtual bool                        HandleEvent     ( IDeserializer & eventDeser, ISerializer & response, BVProjectEditor * editor ) override;

private:
    void            NodeRemovedHandler      ( IEventPtr evt );

};


}   // nodelogic
}	// bv

