#include "PolylineValidator.h"

#include "glm/glm.hpp"

#include <algorithm>
#include <deque>


bool            CompareEvent    ( const Event & a, const Event & b );
bool            CompareEdge     ( p2t::Edge * leftEdge, p2t::Edge * rightEdge, double sweepY );
double          FindX           ( p2t::Edge * edge, double sweepY );

p2t::Edge *     GetAboveEdge    ( int pos, std::vector< p2t::Edge* >& sweepLine );
p2t::Edge *     GetBelowEdge    ( int pos, std::vector< p2t::Edge* >& sweepLine );

p2t::Point *    GetIntesection  ( p2t::Edge * edge1, p2t::Edge * edge2 );

bool            operator<       ( const p2t::Point & point1, const p2t::Point& point2 );


// ***********************
//
PolylineValidator::PolylineValidator   ( Polyline polyline )
    :   m_polyline( polyline )
{
    Init();
}

// ***********************
//
PolylineValidator::PolylineValidator   ( Polyline&& polyline )
    :   m_polyline( std::move( polyline ) )
{
    Init();
}

// ***********************
//
void    PolylineValidator::Init        ()
{
    InitEdges( m_polyline );
}


// ***********************
//
void    PolylineValidator::InitEdges    ( Polyline & polyline )
{
    int numPoints = (int)polyline.size();
    for( int i = 0; i < numPoints; i++ )
    {
        int j = i < numPoints - 1 ? i + 1 : 0;
        m_edgeList.push_back( new p2t::Edge( *polyline[ i ], *polyline[ j ] ) );
    }
}

// ***********************
//
void    PolylineValidator::Sort         ()
{
    std::sort( m_polyline.begin(), m_polyline.end(), p2t::cmp );
}

// ***********************
//
std::deque< Event >     PolylineValidator::InitEventQueue  ()
{
    std::deque< Event > eventQueue;

    // Fill event queue with all contours end points.
    for( int i = 0; i < m_polyline.size(); i++ )
    {
        // Contours are closed. Last point from m_polyline is connected to first point in polyline.
        int j = i < m_polyline.size() - 1 ? i + 1 : 0;

        int topIdx;     // Index of top point.
        int bottomIdx;  // Index of bottom point.

        // Comparing points not pointers !!!
        if( *m_polyline[ j ] < *m_polyline[ i ] )
        {
            // m_polyline[ i ] is top point
            topIdx = i;
            bottomIdx = j;
        }
        else
        {
            // m_polyline[ j ] is top point
            topIdx = j;
            bottomIdx = i;
        }

        // Note: Both points share the same edge from index i. (Check initialization in function PolylineValidator::InitEdges)
        eventQueue.push_back( Event( EventType::StartPoint, m_polyline[ topIdx ], m_edgeList[ i ] ) );
        eventQueue.push_back( Event( EventType::EndPoint, m_polyline[ bottomIdx ], m_edgeList[ i ] ) );
    }

    // Sort event queue - top left points first.
    std::sort( eventQueue.begin(), eventQueue.end(), CompareEvent );

    return eventQueue;
}


// ***********************
// Function assumes, that m_polyline list is sorted.
bool    PolylineValidator::CheckRepeatPoints   ()
{
    for( int i = 0; i < m_polyline.size() - 1; ++i )
    {
        if( m_polyline[ i ] == m_polyline[ i + 1 ] )
            return true;
    }
    return false;
}

// ***********************
// Implementation based on http://geomalgorithms.com/a09-_intersect-3.html
const IntersectionsVec &        PolylineValidator::FindSelfIntersections   ()
{
    // Two points with same coordinates can't exist.
    if( CheckRepeatPoints() )
        throw std::runtime_error( "[Error] Repeat points" );

    std::deque< Event > eventQueue = InitEventQueue();
    std::vector< p2t::Edge* > sweepLine;
    std::vector< p2t::Point* > intersection;


    while( !eventQueue.empty() )
    {
        auto& event = eventQueue[ 0 ];
        
        if( event.Type == EventType::StartPoint )
            ProcessBeginPoint( event, eventQueue, sweepLine );
        else if( event.Type == EventType::EndPoint )
            ProcessEndPoint( event, eventQueue, sweepLine );
        else
            ProcessIntersectionPoint( event, eventQueue, sweepLine, intersection );

        eventQueue.pop_front();
    }

    return m_intersections;
}


// ***********************
//
void                    PolylineValidator::AddIntersectionEvent        ( const Event & event, std::deque< Event > & eventQueue )
{
    auto iter = std::upper_bound( eventQueue.begin(), eventQueue.end(), event, CompareEvent );

    auto eventBefore = *iter;

    // We must prevent adding duplicate intersection events.
    if( eventBefore.Type == EventType::Intersection )
    {
        if( event.Point->x == eventBefore.Point->x &&
            event.Point->y == eventBefore.Point->y )
            return;
    }
    
    eventQueue.insert( iter, event );
}

// ***********************
//
void                    PolylineValidator::ProcessBeginPoint           ( Event & event, std::deque< Event > & eventQueue, std::vector< p2t::Edge* >& sweepLine )
{
    p2t::Edge * newEdge = event.Edge;

    auto pos = AddToSweepLine( newEdge, sweepLine );
    p2t::Edge * aboveEdge = GetAboveEdge( pos, sweepLine );
    p2t::Edge * belowEdge = GetBelowEdge( pos, sweepLine );

    p2t::Point * aboveIntersect = GetIntesection( newEdge, aboveEdge );
    p2t::Point * belowIntersect = GetIntesection( newEdge, belowEdge );

    if( aboveIntersect )
    {
        AddIntersectionEvent( Event( EventType::Intersection, aboveIntersect, nullptr ), eventQueue );
    }
    if( belowIntersect )
    {
        AddIntersectionEvent( Event( EventType::Intersection, belowIntersect, nullptr ), eventQueue );
    }
}

// ***********************
//
void                    PolylineValidator::ProcessEndPoint             ( Event & event, std::deque< Event > & eventQueue, std::vector< p2t::Edge* >& sweepLine )
{
    p2t::Edge * curEdge = event.Edge;

    auto pos = BruteFindInSweepLine( curEdge, sweepLine );
    assert( pos >= 0 );

    p2t::Edge * aboveEdge = GetAboveEdge( pos, sweepLine );
    p2t::Edge * belowEdge = GetBelowEdge( pos, sweepLine );

    DeleteFromSweepLine( curEdge, sweepLine );

    auto intersection = GetIntesection( aboveEdge, belowEdge );
    if( intersection )
    {
        // If intersection exist, it won't be added.
        AddIntersectionEvent( Event( EventType::Intersection, intersection, nullptr ), eventQueue );
    }
}

// ***********************
//
void                    PolylineValidator::ProcessIntersectionPoint    ( Event & event, std::deque< Event > & eventQueue, std::vector< p2t::Edge* > & sweepLine, std::vector< p2t::Point* > & intersections )
{
    m_intersections.push_back( event.Point );

    assert( event.Point->edge_list.size() >= 2 );

    auto edgeIdx = FindInSweepLine( event.Point->edge_list[ 0 ], sweepLine );
    assert( edgeIdx >= 1 );

    // Intersection point stores two edges which intersects.
    // We must determine which one is above edge and which one is below.
    p2t::Edge * tmpMidlle = sweepLine[ edgeIdx ];
    p2t::Edge * tmpAbove = GetAboveEdge( edgeIdx, sweepLine );
    p2t::Edge * tmpBelow = GetBelowEdge( edgeIdx, sweepLine );

    p2t::Edge * aboveEdge = nullptr;
    p2t::Edge * belowEdge = nullptr;
    int aboveIdx;
    int belowIdx;

    if( tmpBelow && tmpBelow == event.Point->edge_list[ 1 ] )
    {
        aboveEdge = tmpMidlle;      aboveIdx = edgeIdx;
        belowEdge = tmpBelow;       belowIdx = edgeIdx - 1;
    }
    else if( tmpAbove && tmpAbove == event.Point->edge_list[ 1 ] )
    {
        aboveEdge = tmpAbove;       aboveIdx = edgeIdx + 1;
        belowEdge = tmpMidlle;      belowIdx = edgeIdx;
    }
    else
    {
        assert( false );
    }

    // Swap after reaching intersection point in event queue.
    std::iter_swap( sweepLine.begin() + aboveIdx, sweepLine.begin() + belowIdx );

    p2t::Edge * aboveAboveEdge = GetAboveEdge( aboveIdx, sweepLine );
    p2t::Edge * belowBelowEdge = GetBelowEdge( belowIdx, sweepLine );

    auto aboveIntersect = GetIntesection( aboveEdge, aboveAboveEdge );
    auto belowIntersect = GetIntesection( belowEdge, belowBelowEdge );

    if( aboveIntersect )
        AddIntersectionEvent( Event( EventType::Intersection, aboveIntersect, nullptr ), eventQueue );
    if( belowIntersect )
        AddIntersectionEvent( Event( EventType::Intersection, belowIntersect, nullptr ), eventQueue );
}

// ***********************
//
p2t::Point *            PolylineValidator::Intersect                   ( p2t::Edge * edge1, p2t::Edge * edge2 )
{
    // http://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect

    glm::vec2 edge1Begin = glm::vec2( edge1->p->x, edge1->p->y );
    glm::vec2 edge1End = glm::vec2( edge1->q->x, edge1->q->y );

    glm::vec2 edge2Begin = glm::vec2( edge2->p->x, edge2->p->y );
    glm::vec2 edge2End = glm::vec2( edge2->q->x, edge2->q->y );

    // Edge vectors
    glm::vec2 edge1Vec = edge1End - edge1Begin;
    glm::vec2 edge2Vec = edge2End - edge2Begin;

    // Vector from begin points of edge1 and edge2
    glm::vec2 edge12Vec = edge2Begin - edge1Begin;

    auto edge12Cross = edge1Vec.x * edge2Vec.y - edge1Vec.y * edge2Vec.x;           // r x s (check in link)
    auto edge1Point2Cross = edge12Vec.x * edge1Vec.y - edge12Vec.y * edge1Vec.x;    // (p - q) x r
    auto edge2Point1Cross = edge12Vec.x * edge2Vec.y - edge12Vec.y * edge2Vec.x;    // (p - q) x s

    if( edge12Cross == 0 )
        return nullptr;

    float edge1Factor = edge2Point1Cross / edge12Cross;
    float edge2Factor = edge1Point2Cross / edge12Cross;

    // Note: We don't need intersection on begin and end point. Add some margin.
    const float epsilon = 0.00001f;
    if( edge1Factor > 0.0f + epsilon && edge1Factor < 1.0f - epsilon &&
        edge2Factor > 0.0f + epsilon && edge2Factor < 1.0f - epsilon )
    {
        glm::vec2 intersectionPoint = edge1Begin + edge1Factor * edge1Vec;
        p2t::Point * intersectPoint = new p2t::Point( intersectionPoint.x, intersectionPoint.y );
        return intersectPoint;
    }


    return nullptr;
}

// ***********************
// @return Returns -1 if edge is not present in sweepLine
int                     PolylineValidator::FindInSweepLine             ( p2t::Edge * searchedEdge, std::vector< p2t::Edge * > & sweepLine )
{
    int resultIdx = 0;
    auto sweepY = searchedEdge->p->y;

    if( sweepLine.empty() )
        return -1;

    if( sweepLine.size() == 1 )
    {
        // !Compare( a, b ) && !Compare( b, a ) means that a, b are equal.
        if( !CompareEdge( sweepLine[ 0 ], searchedEdge, sweepY ) && !CompareEdge( searchedEdge, sweepLine[ 0 ], sweepY ) )
            return 0;
        else
            return -1;
    }

    // Binary search 
    size_t left = 0;
    auto right = sweepLine.size();
    auto span = right - left;

    while( span > 1 )
    {
        span = span / 2;
        auto pos = left + span;

        auto edge = sweepLine[ pos ];
        if( CompareEdge( edge, searchedEdge, sweepY ) )
        {
            right = pos;
        }
        else if( CompareEdge( searchedEdge, edge, sweepY ) )
        {
            left = pos;
        }
        else
        {
            return (int)pos;
        }
    }

    auto edge = sweepLine[ left ];
    if( !CompareEdge( edge, searchedEdge, sweepY ) && !CompareEdge( searchedEdge, edge, sweepY ) )
        return (int)left;
    else
        return -1;
}

// ***********************
// This method iterates through edges and compares pointers. It doesn't use binary search, because
// sweepLine vector is ordered with function CompareEdge.
// Hopefully this function is more cache efficient and brute force won't be performance problem.
int                     PolylineValidator::BruteFindInSweepLine        ( p2t::Edge * edge, std::vector< p2t::Edge * > & sweepLine )
{
    for( int i = 0; i < sweepLine.size(); ++i )
    {
        if( edge == sweepLine[ i ] )
            return i;
    }
    return -1;
}

// ***********************
//
int                     PolylineValidator::AddToSweepLine              ( p2t::Edge* addEdge, std::vector< p2t::Edge* > & sweepLine )
{
    auto sweepY = addEdge->p->y;

    if( sweepLine.size() == 0 )
    {
        sweepLine.push_back( addEdge );
        return 0;
    }
    else if( sweepLine.size() == 1 )
    {
        if( CompareEdge( sweepLine[ 0 ], addEdge, sweepY ) )
        {
            sweepLine.insert( sweepLine.begin(), addEdge );
            return 0;
        }
        else
        {
            sweepLine.push_back( addEdge );
            return 1;
        }
    }
    else
    {
        // Binary search 
        size_t left = 0;
        auto right = sweepLine.size();
        auto span = right - left;

        while( span > 1 )
        {
            span = span / 2;
            auto pos = left + span;

            auto edge = sweepLine[ pos ];
            if( CompareEdge( edge, addEdge, sweepY ) )
            {
                right = pos;
            }
            else
            {
                left = pos;
            }
        }

        auto edge = sweepLine[ left ];
        if( CompareEdge( edge, addEdge, sweepY ) )
        {
            sweepLine.insert( sweepLine.begin() + left, addEdge );
            return (int)left;
        }
        else
        {
            if( left + 1 < sweepLine.size() )
            {
                sweepLine.insert( sweepLine.begin() + left + 1, addEdge );
                return int( left + 1 );
            }
            else
            {
                sweepLine.push_back( addEdge );
                return (int)sweepLine.size() - 1;
            }
        }
    }
}

// ***********************
//
void                    PolylineValidator::DeleteFromSweepLine         ( p2t::Edge * edge, std::vector< p2t::Edge * > & sweepLine )
{
    auto pos = BruteFindInSweepLine( edge, sweepLine );
    assert( pos >= 0 );

    sweepLine.erase( sweepLine.begin() + pos );
}

// ***********************
//
bool            CompareEvent    ( const Event & a, const Event & b )
{
    if( a.Point->y > b.Point->y )
    {
        return true;
    }
    else if( a.Point->y == b.Point->y )
    {
        // Make sure q is point with greater x value
        if( a.Point->x < b.Point->x )
        {
            return true;
        }
        else if( a.Point->x == b.Point->x )
        {
            // Start point goes before end point.
            if( a.Type == EventType::StartPoint && b.Type != EventType::StartPoint )
                return true;
        }
    }
    return false;
}

// ***********************
//
bool            CompareEdge     ( p2t::Edge * leftEdge, p2t::Edge * rightEdge, double sweepY )
{
    auto leftX = FindX( leftEdge, sweepY );
    auto rightX = FindX( rightEdge, sweepY );

    if( leftX < rightX )
        return true;

    return false;
}

// ***********************
// Finds X coordinate of edge in point Y.
double          FindX           ( p2t::Edge * edge, double sweepY )
{
    p2t::Point* point1 = edge->p;
    p2t::Point* point2 = edge->q;

    // Dangerous situation. We can't order edges.
    if( point1->y == point2->y )
        return point1->x;

    auto deltaX = point2->x - point1->x;
    auto deltaY = point2->y - point1->y;
    auto sweepPointDelta = sweepY - point1->y;

    return point1->x + deltaX * ( sweepPointDelta / deltaY );
}

// ***********************
//
p2t::Edge *     GetAboveEdge    ( int pos, std::vector< p2t::Edge * > & sweepLine )
{
    // SweepLine is sorted so we can take element after and before pos if they exist.
    if( pos >= 0 && pos < sweepLine.size() - 1 )
        return sweepLine[ pos + 1 ];
    return nullptr;
}

// ***********************
//
p2t::Edge *     GetBelowEdge    ( int pos, std::vector< p2t::Edge * > & sweepLine )
{
    // SweepLine is sorted so we can take element after and before pos if they exist.
    if( pos > 0 && pos < sweepLine.size() )
        return sweepLine[ pos - 1 ];
    return nullptr;
}

// ***********************
// Edges can be nullptr.
// Function adds two edges to point edge list.
p2t::Point *    GetIntesection  ( p2t::Edge * edge1, p2t::Edge * edge2 )
{
    if( edge1 && edge2 )
    {
        auto point = PolylineValidator::Intersect( edge1, edge2 );
        if( point )
        {
            point->edge_list.push_back( edge1 );
            point->edge_list.push_back( edge2 );

            return point;
        }
    }
    return nullptr;
}

// ***********************
//
bool            operator<       ( const p2t::Point & a, const p2t::Point & b )
{
    if( a.y < b.y )
    {
        return true;
    }
    else if( a.y == b.y )
    {
        // Make sure q is point with greater x value
        if( a.x < b.x )
        {
            return true;
        }
    }
    return false;
}
