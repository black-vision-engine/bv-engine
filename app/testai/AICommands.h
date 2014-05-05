#include "Engine/Types/Enums.h"

#include "testai/IAICommand.h"


namespace bv {

namespace model {

class DefaultTimeline;

} //model

// ************************************
class AICommandBase : public IAICommand
{
private:

    TimeType    m_triggerTime;
    TimeType    m_lastTriggerTime;

    std::string m_repr;

public:

    AICommandBase( TimeType triggerTime, const std::string & repr );

    virtual TimeType    LastTriggerTime () const override;
    virtual TimeType    GetTriggerTime  () const override;
    virtual bool        TriggerCommand  ( TimeType t ) override;

    virtual std::string Repr            () const override;
    void                SetRepr         ( const std::string & repr );

protected:

    virtual bool        TriggerImpl     ( TimeType t ) = 0;

};

// ************************************
class AICommandPlay : public AICommandBase
{
private:

    model::DefaultTimeline  * m_timeline;

public:

                        AICommandPlay   ( model::DefaultTimeline * timeline, TimeType triggerTime );
                        ~AICommandPlay  ();


protected:

    virtual bool        TriggerImpl     ( TimeType t ) override;

};

// ************************************
class AICommandStop : public AICommandBase
{
private:

    model::DefaultTimeline  * m_timeline;

public:

                        AICommandStop   ( model::DefaultTimeline * timeline, TimeType triggerTime );
                        ~AICommandStop  ();

protected:

    virtual bool        TriggerImpl     ( TimeType t ) override;

};

// ************************************
class AICommandReverse : public AICommandBase
{
private:

    model::DefaultTimeline  * m_timeline;

public:

                        AICommandReverse    ( model::DefaultTimeline * timeline, TimeType triggerTime );
                        ~AICommandReverse   ();

protected:

    virtual bool        TriggerImpl         ( TimeType t ) override;

};

// ************************************
class AICommandSetPlayDirection : public AICommandBase
{
private:

    model::DefaultTimeline  *   m_timeline;
    TimelinePlayDirection       m_direction;

public:

                        AICommandSetPlayDirection    ( model::DefaultTimeline * timeline, TimeType triggerTime, TimelinePlayDirection direction );
                        ~AICommandSetPlayDirection   ();

protected:

    virtual bool        TriggerImpl         ( TimeType t ) override;

};

// ************************************
class AICommandSetTimeAndStop : public AICommandBase
{
private:

    model::DefaultTimeline  *   m_timeline;
    TimeType                    m_eventTime;

public:

                        AICommandSetTimeAndStop     ( model::DefaultTimeline * timeline, TimeType triggerTime, TimeType eventTime );
                        ~AICommandSetTimeAndStop    ();

protected:

    virtual bool        TriggerImpl         ( TimeType t ) override;

};

// ************************************
class AICommandSetTimeAndPlay : public AICommandBase
{
private:

    model::DefaultTimeline  *   m_timeline;
    TimeType                    m_eventTime;

public:

                        AICommandSetTimeAndPlay     ( model::DefaultTimeline * timeline, TimeType triggerTime, TimeType eventTime );
                        ~AICommandSetTimeAndPlay    ();

protected:

    virtual bool        TriggerImpl         ( TimeType t ) override;

};

} //bv
