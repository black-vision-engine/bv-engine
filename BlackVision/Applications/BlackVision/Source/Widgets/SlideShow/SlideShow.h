#pragma once

#include "Widgets/NodeLogicBase.h"
#include "Engine/Models/SceneModel.h"

#include "CoreDEF.h"


namespace bv { namespace nodelogic {


class SlideShow;

DEFINE_PTR_TYPE( SlideShow )
DEFINE_CONST_PTR_TYPE( SlideShow )


class SlideShow:  public model::NodeLogicBase, public std::enable_shared_from_this< SlideShow >
{
private:

    static const std::string        m_type;

    static const Float32            m_defaultFadeTime;

    struct ACTION 
    {
        static const std::string    START_SLIDE_SHOW;
        static const std::string    STOP_SLIDE_SHOW;
        static const std::string    PAUSE_SLIDE_SHOW;
    };

private:

	model::BasicNodePtr				m_parentNode;
    model::ITimeEvaluatorPtr        m_timeEval;

    Float32                         m_fadeInTime;       //in miliseconds
    Float32                         m_fadeOutTime;      //in miliseconds

public:

	explicit                        SlideShow           ( model::BasicNodePtr parent, model::ITimeEvaluatorPtr timeEval );
	                                ~SlideShow          ();

	static SlideShowPtr             Create              ( model::BasicNodePtr parent, model::ITimeEvaluatorPtr timeEval );

    virtual void                    Serialize           ( ISerializer & ser ) const override;
    static SlideShowPtr             Create              ( const IDeserializer & deser, model::BasicNodePtr parent );

	virtual void	                Initialize		    ()				override {}
	virtual void	                Update			    ( TimeType t )	override;
	virtual void	                Deinitialize	    ()				override {}

    virtual const std::string &     GetType             () const override;
    static const std::string &      Type                ();

    virtual bool                    HandleEvent         ( IDeserializer & eventSer, ISerializer & response, BVProjectEditor * editor ) override;

};

} //nodelogic
} //bv
