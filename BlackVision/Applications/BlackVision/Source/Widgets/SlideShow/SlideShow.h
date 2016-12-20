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

class SlideShow;

DEFINE_PTR_TYPE( SlideShow );
DEFINE_CONST_PTR_TYPE( SlideShow );



class SlideShow : public model::NodeLogicBase, public std::enable_shared_from_this< SlideShow >
{
public:

    enum FadeType
    {
        NoFade,
        FadeAlpha
    };

    typedef std::shared_ptr< bv::model::ParamEnum< FadeType > > FadeTypeParamPtr;

private:
    static const std::string            m_type;

    struct ACTION
    {
        static const std::string    START_SLIDE_SHOW;
        static const std::string    STOP_SLIDE_SHOW;
        static const std::string    PAUSE_SLIDE_SHOW;
    };

    struct PARAMETERS
    {
        static const std::string    FADE_TYPE;
        static const std::string    FADE_TIME;
        static const std::string    PRESENCE_TIME;
    };

private:
    bv::model::BasicNodeWeakPtr			m_parentNode;

    FadeTypeParamPtr                    m_fadeType;
    ValueFloatPtr                       m_fadeTime;
    ValueFloatPtr                       m_presenceTime;

    UInt64                              m_startTime;
    UInt32                              m_nodeIdx;

    bool                                m_started;

public:
    explicit                            SlideShow       ( bv::model::BasicNodeWeakPtr parent, bv::model::ITimeEvaluatorPtr timeEvaluator );
                                        ~SlideShow		();

    virtual void                        Initialize		()				override {};
    virtual void                        Update			( TimeType t )	override;
    virtual void                        Deinitialize	()				override {};


    virtual const std::string &         GetType         () const override;
    static const std::string &          Type            ();

    virtual void                        Serialize       ( ISerializer & ser ) const override;
    static SlideShowPtr			        Create          ( const IDeserializer & deser, bv::model::BasicNodeWeakPtr parentNode );

    virtual bool                        HandleEvent     ( IDeserializer & eventDeser, ISerializer & response, BVProjectEditor * editor ) override;


private:
    void            HideAllNodes            ();
    void            ShowAllNodes            ( bool value = true );
    void            ShowNode                ( int idx );
};


}   // nodelogic
}	// bv
