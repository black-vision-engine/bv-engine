#pragma once

#include "poly2tri/common/shapes.h"

#include <vector>
#include <deque>


typedef std::vector< p2t::Point * > Polyline;
typedef std::vector< Polyline > PolilinesVec;


enum EventType
{
    StartPoint,
    EndPoint,
    Intersection
};

struct Event
{
    EventType       Type;
    p2t::Point *    Point;
    p2t::Edge *     Edge;

    Event( EventType type, p2t::Point * point, p2t::Edge * edge )
        : Type( Type )
        , Point( point )
        , Edge( edge )
    {}
};




class PolylineValidator
{
private:
    Polyline        m_polyline;

    // Temporary helpers    
    std::vector< p2t::Edge* >   m_edgeList;

public:
    explicit        PolylineValidator   ( Polyline polyline );
    explicit        PolylineValidator   ( Polyline&& polyline );

    void            MakeSimplePolygon   ();

private:
    void        Init        ();
    void        InitEdges   ( Polyline & polyline );
    void        Sort        ();

    bool        CheckRepeatPoints   ();

    std::deque< Event >     InitEventQueue              ();
    void                    ProcessBeginPoint           ( Event & event, std::deque< Event > & eventQueue, std::vector< p2t::Edge* > & sweepLine );
    void                    ProcessEndPoint             ( Event & event, std::deque< Event > & eventQueue, std::vector< p2t::Edge* > & sweepLine );
    void                    ProcessIntersectionPoint    ( Event & event, std::deque< Event > & eventQueue, std::vector< p2t::Edge* > & sweepLine, std::vector< p2t::Point* > & intersections );
};


