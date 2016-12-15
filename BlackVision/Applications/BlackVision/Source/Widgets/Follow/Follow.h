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

class Follow;

DEFINE_PTR_TYPE( Follow );
DEFINE_CONST_PTR_TYPE( Follow );



class Follow : public model::NodeLogicBase, public std::enable_shared_from_this< Follow >
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
    bv::model::BasicNodeWeakPtr         m_parentNode;


public:
    explicit    Follow			        ( bv::model::BasicNodeWeakPtr parent, bv::model::ITimeEvaluatorPtr timeEvaluator );
    ~Follow		();

    virtual void                        Initialize		()				override {}
    virtual void                        Update			( TimeType t )	override;
    virtual void                        Deinitialize	()				override {}


    virtual const std::string &         GetType         () const override;
    static const std::string &          Type            ();

    virtual void                        Serialize       ( ISerializer & ser ) const override;
    static FollowPtr			        Create          ( const IDeserializer & deser, bv::model::BasicNodeWeakPtr parentNode );

    virtual bool                        HandleEvent     ( IDeserializer & eventDeser, ISerializer & response, BVProjectEditor * editor ) override;


};


}   // nodelogic
}	// bv
