#pragma once
#include "Engine/Events/Events.h"
#include "Engine/Models/BVScene.h"
#include "BVAppLogic.h"


namespace bv
{

class BVAppLogic;

class RemoteControlInterface
{
private:

    BVAppLogic *            m_AppLogic;

	std::vector< float >    m_cyclistPos;
    std::vector< float >    m_concentrateDistMarks;

	// height map
	void            UpdateCyclistPosition( int cyclistPos, float km );
    void            HMSetCyclistPosition    ( int cyclistPos, float km );
    void            HMShow                  ();
    void            HMReset                 ();
    void            HMStart                 ( float km );
    void            HMZoomIn                ( float km, float leftKM, float rightKM, float yScale );
    void            HMZoomOutFromCurrent    ( float scale);
    void            HMZoomInFake            ( float km, float leftKM, float rightKM, float yScale );
    void            HMConcentratedDistanceAnimStart( int num );
    void            HMConcentratedDistanceAnimStart2( int num );

public:

    RemoteControlInterface          ( BVAppLogic * appLogic );
	~RemoteControlInterface         (void);

    void            OnUpdateParam   ( IEventPtr evt );
    void            OnSetParam      ( bv::IEventPtr evt );
	void            OnWidgetCmd     ( bv::IEventPtr evt );
    void            OnInformation   ( bv::IEventPtr evt );
	void            OnSceneStructure( bv::IEventPtr evt );
    void            OnTimelineCmd   ( bv::IEventPtr evt );
    void            OnTimerCmd      ( bv::IEventPtr evt );
	void            OnNodeAppearing ( IEventPtr evt );
	void            OnNodeLeaving   ( IEventPtr evt );
	void            OnNoMoreNodes   ( IEventPtr evt );

	// FIXME: hack
    void            UpdateHM        ();

};

}//bv