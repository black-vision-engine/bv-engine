#pragma once

#include "poly2tri/common/shapes.h"

#include <vector>
#include <deque>


typedef std::vector< p2t::Point * > Polyline;
typedef std::vector< Polyline > PolylinesVec;
typedef std::vector< p2t::Point * > IntersectionsVec;


// ***********************
//
enum EventType
{
    StartPoint,
    EndPoint,
    Intersection
};

// ***********************
//
struct Event
{
    EventType       Type;
    p2t::Point *    Point;
    p2t::Edge *     Edge;

    Event( EventType type, p2t::Point * point, p2t::Edge * edge )
        : Type( type )
        , Point( point )
        , Edge( edge )
    {}
};



// ***********************
//
struct IntersectionData
{
    int             PolylineIdx;        // Point under this index is directly before intersection point.
    bool            Processed;          // Segment after PolylineIdx already used.
    p2t::Point *    IntersectionPoint;
};





class PolylineValidator
{
private:
    Polyline        m_polyline;

    // Temporary helpers    
    std::vector< p2t::Edge* >   m_edgeList;

    // Results
    std::vector< p2t::Point * > m_intersections;

public:
    explicit        PolylineValidator   ( Polyline polyline );
    explicit        PolylineValidator   ( Polyline&& polyline );
                    ~PolylineValidator  ();


    const IntersectionsVec &        FindSelfIntersections   ();
    PolylinesVec                    DecomposeContour        ();


private:
    void        Init        ();
    void        InitEdges   ( Polyline & polyline );
    void        Sort        ();

    bool        CheckRepeatPoints   ();

    std::deque< Event >     InitEventQueue              ();
    void                    ProcessBeginPoint           ( Event & event, std::deque< Event > & eventQueue, std::vector< p2t::Edge * > & sweepLine );
    void                    ProcessEndPoint             ( Event & event, std::deque< Event > & eventQueue, std::vector< p2t::Edge * > & sweepLine );
    void                    ProcessIntersectionPoint    ( Event & event, std::deque< Event > & eventQueue, std::vector< p2t::Edge * > & sweepLine, std::vector< p2t::Point * > & intersections );

    int                     AddToSweepLine              ( p2t::Edge * addEdge, std::vector< p2t::Edge * > & sweepLine );
    int                     FindInSweepLine             ( p2t::Edge * edge, std::vector< p2t::Edge * > & sweepLine );
    int                     BruteFindInSweepLine        ( p2t::Edge * edge, std::vector< p2t::Edge * > & sweepLine );
    void                    DeleteFromSweepLine         ( p2t::Edge * edge, std::vector< p2t::Edge * > & sweepLine );
    void                    AddIntersectionEvent        ( const Event & event, std::deque< Event > & eventQueue );

    std::vector< IntersectionData > InitDividePoints    ();
public:

    static p2t::Point *     Intersect                   ( p2t::Edge * edge1, p2t::Edge * edge2 );
};


