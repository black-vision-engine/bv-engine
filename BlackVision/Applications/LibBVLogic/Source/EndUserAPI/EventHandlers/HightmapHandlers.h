#pragma once

#include "Engine/Events/Events.h"

namespace bv
{
class BVAppLogic;

class HightmapHandlers
{
private:
    BVAppLogic*         m_appLogic;

    std::vector< float >    m_cyclistPos;
    std::vector< float >    m_concentrateDistMarks;
public:
    HightmapHandlers( BVAppLogic* appLogic );
    ~HightmapHandlers();

	// height map
	void            UpdateCyclistPosition   ( int cyclistPos, float km );
    void            HMSetCyclistPosition    ( int cyclistPos, float km );
    void            HMShow                  ();
    void            HMReset                 ();
    void            HMStart                 ( float km );
    void            HMZoomIn                ( float km, float leftKM, float rightKM, float yScale );
    void            HMZoomOutFromCurrent    ( float scale);
    void            HMZoomInFake            ( float km, float leftKM, float rightKM, float yScale );
    void            HMConcentratedDistanceAnimStart     ( int num );
    void            HMConcentratedDistanceAnimStart2    ( int num );

	// FIXME: hack
    void            UpdateHM        ();
    void            HightmapHandler         ( bv::IEventPtr eventPtr );
};

} //bv