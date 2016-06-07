#pragma once

#include "CoreDEF.h"
#include "../NodeLogicBase.h"           // Widgets/NodeLogicBase.h doesn't work


namespace bv
{



namespace model {

    class BasicNode;
    DEFINE_PTR_TYPE( BasicNode )

} // model




namespace nodelogic
{

class AnalogWatch;

DEFINE_PTR_TYPE( AnalogWatch )
DEFINE_CONST_PTR_TYPE( AnalogWatch )



class AnalogWatch : public model::NodeLogicBase, public std::enable_shared_from_this< AnalogWatch >
{
private:
    static const std::string            m_type;

    struct ACTION 
    {
        static const std::string    START_WATCH;
        static const std::string    CLEAR_WATCH;
    };

private:
    bv::model::BasicNodePtr	            m_parentNode;

    bv::model::BasicNodePtr	            m_hourNode;
    bv::model::BasicNodePtr	            m_minuteNode;
    bv::model::BasicNodePtr	            m_secondsNode;

    bool                                m_started;

public:
    explicit    AnalogWatch             ( bv::model::BasicNodePtr parent, bv::model::ITimeEvaluatorPtr timeEvaluator );
                ~AnalogWatch            ();

	virtual void                        Initialize		()				override {}
    virtual void                        Update			( TimeType t )	override;
	virtual void                        Deinitialize	()				override {}


    virtual const std::string &         GetType         () const override;
    static const std::string &          Type            ();

    virtual void                        Serialize       ( ISerializer & ser ) const override;
    static AnalogWatchPtr               Create          ( const IDeserializer & deser, bv::model::BasicNodePtr parentNode );

    virtual bool                        HandleEvent     ( IDeserializer & eventDeser, ISerializer & response, BVProjectEditor * editor ) override;

private:

    bool                                StartWatch      ( IDeserializer & eventDeser, ISerializer & response, BVProjectEditor * editor );
    bool                                ClearWatch      ( IDeserializer & eventDeser, ISerializer & response, BVProjectEditor * editor );

private:

    void            SetInitialPosition      ( bv::model::BasicNodePtr& node );
    void            ClearPosition           ( bv::model::BasicNodePtr& node );
    void            UpdateTime              ( bv::model::BasicNodePtr& node, float ratio );
};


}   // nodelogic

}	// bv