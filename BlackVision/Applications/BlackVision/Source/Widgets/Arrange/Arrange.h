#pragma once

#include "CoreDEF.h"
#include "../NodeLogicBase.h"           // Widgets/NodeLogicBase.h doesn't work
#include "Engine/Types/Values/TypedValues.h"


namespace bv
{

namespace model
{

class BasicNode;
DEFINE_PTR_TYPE( BasicNode );

} // model




namespace nodelogic
{

class Arrange;

DEFINE_PTR_TYPE( Arrange );
DEFINE_CONST_PTR_TYPE( Arrange );



class Arrange : public model::NodeLogicBase, public std::enable_shared_from_this< Arrange >
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
    bv::model::BasicNodePtr	            m_parentNode;


public:
    explicit    Arrange			( bv::model::BasicNodePtr parent, bv::model::ITimeEvaluatorPtr timeEvaluator );
    ~Arrange		();

    virtual void                        Initialize		()				override {}
    virtual void                        Update			( TimeType t )	override;
    virtual void                        Deinitialize	()				override {}


    virtual const std::string &         GetType         () const override;
    static const std::string &          Type            ();

    virtual void                        Serialize       ( ISerializer & ser ) const override;
    static ArrangePtr			        Create          ( const IDeserializer & deser, bv::model::BasicNodePtr parentNode );

    virtual bool                        HandleEvent     ( IDeserializer & eventDeser, ISerializer & response, BVProjectEditor * editor ) override;


};


}   // nodelogic
}	// bv
