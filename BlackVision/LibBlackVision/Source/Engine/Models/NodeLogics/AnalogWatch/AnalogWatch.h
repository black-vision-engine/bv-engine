#pragma once

#include "Engine/Models/NodeLogics/NodeLogicBase.h"
#include "Engine/Types/Values/TypedValues.h"


namespace bv { 
    

namespace model {

    class BasicNode;
    DEFINE_WEAK_PTR_TYPE( BasicNode )

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

    struct PARAMETERS
    {
        static const std::string    SMOOTH_HOURS;
        static const std::string    SMOOTH_MINUTES;
        static const std::string    SMOOTH_SECONDS;
    };

private:

    model::BasicNodeWeakPtr	        m_parentNode;

    model::BasicNode *	            m_hourNode;
    model::BasicNode *	            m_minuteNode;
    model::BasicNode *	            m_secondsNode;

    bool                                m_started;

    ValueBoolPtr                        m_smoothHours;
    ValueBoolPtr                        m_smoothMinutes;
    ValueBoolPtr                        m_smoothSeconds;

public:

    explicit                            AnalogWatch     ( model::BasicNodeWeakPtr parent, model::ITimeEvaluatorPtr timeEvaluator );
                                        ~AnalogWatch    ();

	virtual void                        Initialize		()				override;
    virtual void                        Update			( TimeType t )	override;
    virtual void                        Deinitialize	()				override;


    virtual const std::string &         GetType         () const override;
    static const std::string &          Type            ();

    virtual void                        Serialize       ( ISerializer & ser ) const override;
    static AnalogWatchPtr               Create          ( const IDeserializer & deser, model::BasicNodeWeakPtr parentNode );

    virtual bool                        HandleEvent     ( IDeserializer & eventDeser, ISerializer & response, BVProjectEditor * editor ) override;

private:

    bool                                StartWatch      ( IDeserializer & eventDeser, ISerializer & response, BVProjectEditor * editor );
    bool                                ClearWatch      ( IDeserializer & eventDeser, ISerializer & response, BVProjectEditor * editor );

private:

    void            SetInitialPosition      ( model::BasicNode * node );
    void            ClearPosition           ( model::BasicNode * node );
    void            UpdateTime              ( model::BasicNode * node, float ratio );

    void            NodeRemovedHandler      ( IEventPtr evt );
    void            NodeMovedHandler        ( IEventPtr evt );
    void            NodeRemoved             ( model::BasicNode * parent, model::BasicNode * node );
};

}   // nodelogic
}	// bv

