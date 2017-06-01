#pragma once

#include "Engine/Types/Enums.h"
#include "CoreDEF.h"

#include "Engine/Models/Timeline/Dynamic/DefaultTimeline.h"
#include "Engine/Models/Plugins/Interfaces/IPlugin.h"

#include "Engine/Models/Interfaces/IModelNode.h"
#include "testai/IAICommand.h"


namespace bv {

class BVAppLogic;

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

    virtual void        SetTimeline     ( model::DefaultTimelinePtr timeline ) = 0;

protected:

    virtual bool        TriggerImpl     ( TimeType t ) = 0;

};

// ************************************
class AICommandPlay : public AICommandBase
{
private:

    model::DefaultTimelinePtr m_timeline;

public:

                        AICommandPlay   ( model::DefaultTimelinePtr timeline, TimeType triggerTime );
                        ~AICommandPlay  ();

    virtual void        SetTimeline     ( model::DefaultTimelinePtr timeline ) override;

protected:

    virtual bool        TriggerImpl     ( TimeType t ) override;

};

// ************************************
class AICommandStop : public AICommandBase
{
private:

    model::DefaultTimelinePtr m_timeline;

public:

                        AICommandStop   ( model::DefaultTimelinePtr timeline, TimeType triggerTime );
                        ~AICommandStop  ();

    virtual void        SetTimeline     ( model::DefaultTimelinePtr timeline ) override;

protected:

    virtual bool        TriggerImpl     ( TimeType t ) override;

};

// ************************************
class AICommandReverse : public AICommandBase
{
private:

    model::DefaultTimelinePtr m_timeline;

public:

                        AICommandReverse    ( model::DefaultTimelinePtr timeline, TimeType triggerTime );
                        ~AICommandReverse   ();

    virtual void        SetTimeline     ( model::DefaultTimelinePtr timeline ) override;

protected:

    virtual bool        TriggerImpl         ( TimeType t ) override;

};

// ************************************
class AICommandSetPlayDirection : public AICommandBase
{
private:

    model::DefaultTimelinePtr   m_timeline;
    TimelinePlayDirection       m_direction;

public:

                        AICommandSetPlayDirection    ( model::DefaultTimelinePtr timeline, TimeType triggerTime, TimelinePlayDirection direction );
                        ~AICommandSetPlayDirection   ();

    virtual void        SetTimeline     ( model::DefaultTimelinePtr timeline ) override;

protected:

    virtual bool        TriggerImpl         ( TimeType t ) override;

};

// ************************************
class AICommandSetTimeAndStop : public AICommandBase
{
private:

    model::DefaultTimelinePtr   m_timeline;
    TimeType                    m_eventTime;

public:

                        AICommandSetTimeAndStop     ( model::DefaultTimelinePtr timeline, TimeType triggerTime, TimeType eventTime );
                        ~AICommandSetTimeAndStop    ();

    virtual void        SetTimeline     ( model::DefaultTimelinePtr timeline ) override;

protected:

    virtual bool        TriggerImpl         ( TimeType t ) override;

};

// ************************************
class AICommandSetTimeAndPlay : public AICommandBase
{
private:

    model::DefaultTimelinePtr   m_timeline;
    TimeType                    m_eventTime;

public:

                        AICommandSetTimeAndPlay     ( model::DefaultTimelinePtr timeline, TimeType triggerTime, TimeType eventTime );
                        ~AICommandSetTimeAndPlay    ();

    virtual void        SetTimeline     ( model::DefaultTimelinePtr timeline ) override;

protected:

    virtual bool        TriggerImpl         ( TimeType t ) override;

};

// ************************************
class AICommandEnableOverridenAlpha : public AICommandBase
{
private:

    TimeType                m_eventTime;
    model::IModelNodePtr    m_node;

public:

                        AICommandEnableOverridenAlpha   ( model::IModelNodePtr root, const std::string & node, TimeType triggerTime );
                        ~AICommandEnableOverridenAlpha  ();

    virtual void        SetTimeline                     ( model::DefaultTimelinePtr timeline ) override;

protected:

    virtual bool        TriggerImpl                     ( TimeType t ) override;

};

// ************************************
class AICommandDisableAlpha : public AICommandBase
{
private:

    TimeType                m_eventTime;
    model::IModelNodePtr    m_node;

public:

                        AICommandDisableAlpha       ( model::IModelNodePtr root, const std::string & node, TimeType triggerTime );
                        ~AICommandDisableAlpha      ();

    virtual void        SetTimeline                 ( model::DefaultTimelinePtr timeline ) override;

protected:

    virtual bool        TriggerImpl                 ( TimeType t ) override;

};


// ************************************
class AICommandEnableOverridenAlphaNM : public AICommandBase
{
private:

    TimeType                m_eventTime;
    model::IModelNodePtr    m_node;

public:

                        AICommandEnableOverridenAlphaNM ( model::IModelNodePtr root, const std::string & node, TimeType triggerTime );
                        ~AICommandEnableOverridenAlphaNM();

    virtual void        SetTimeline                     ( model::DefaultTimelinePtr timeline ) override;

protected:

    virtual bool        TriggerImpl                     ( TimeType t ) override;

};

// ************************************
class AICommandDisableAlphaNM : public AICommandBase
{
private:

    TimeType                m_eventTime;
    model::IModelNodePtr    m_node;

public:

                        AICommandDisableAlphaNM     ( model::IModelNodePtr root, const std::string & node, TimeType triggerTime );
                        ~AICommandDisableAlphaNM    ();

    virtual void        SetTimeline                 ( model::DefaultTimelinePtr timeline ) override;

protected:

    virtual bool        TriggerImpl                 ( TimeType t ) override;

};

// ************************************
class AICommandReloadScene : public AICommandBase
{
private:

    TimeType                m_eventTime;
    BVAppLogic *            m_logic;

public:

                        AICommandReloadScene        ( BVAppLogic * logic, TimeType triggerTime );
                        ~AICommandReloadScene       ();

    virtual void        SetTimeline                 ( model::DefaultTimelinePtr timeline ) override;

protected:

    virtual bool        TriggerImpl                 ( TimeType t ) override;

};

// ************************************
class AICommandReloadTexture : public AICommandBase
{
private:

    TimeType                m_eventTime;

    std::string             m_textureName;

    model::IPluginPtr       m_plugin;

public:

                        AICommandReloadTexture      ( BVAppLogic * logic, TimeType triggerTime, const std::string & nodeName, const std::string & pluginName, const std::string & textureName );
                        ~AICommandReloadTexture     ();

    virtual void        SetTimeline                 ( model::DefaultTimelinePtr timeline ) override;

protected:

    virtual bool        TriggerImpl                 ( TimeType t ) override;

};

} //bv
