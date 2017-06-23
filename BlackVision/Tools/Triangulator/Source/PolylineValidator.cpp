#include "PolylineValidator.h"

#include "glm.hpp"

#include <algorithm>


namespace triangulator {

const float epsilon = 0.00001f;



bool            CompareEvent    ( const Event & a, const Event & b );
bool            CompareEdge     ( p2t::Edge * leftEdge, p2t::Edge * rightEdge, double sweepY );
double          FindX           ( p2t::Edge * edge, double sweepY );
double          EdgeDelta       ( p2t::Edge * edge );

p2t::Edge *     GetAboveEdge    ( int pos, std::vector< p2t::Edge* >& sweepLine );
p2t::Edge *     GetBelowEdge    ( int pos, std::vector< p2t::Edge* >& sweepLine );

p2t::Point *    GetIntesection  ( p2t::Edge * edge1, p2t::Edge * edge2 );

bool            operator<       ( const p2t::Point & point1, const p2t::Point & point2 );
bool			Equal			( const p2t::Point & point1, const p2t::Point & point2 );

bool            IsIntersectionStart ( const p2t::Point * polylinePoint, const p2t::Point * nextPoint, const p2t::Point * intersectionPoint );
int             FindFreeDividePoint ( std::vector< IntersectionData >& data );
int             FindNextContourPart ( std::vector< IntersectionData >& data, Polyline & polyline, const p2t::Point * partEnd, int intersectionIdx );

bool            IsSharedPointIntersection   ( const Event& intersectEvent );


// ***********************
//
PolylineValidator::PolylineValidator   ( const Polyline & polyline )
{
    m_polyline.reserve( polyline.size() );
    
    for( int i = 0; i < polyline.size(); ++i )
    {
        m_polyline.push_back( new p2t::Point( *polyline[ i ] ) );
    }

    Init();
}

// ***********************
//
PolylineValidator::PolylineValidator   ( Polyline && polyline )
    :   m_polyline( std::move( polyline ) )
{
    Init();
}

// ***********************
//
PolylineValidator::~PolylineValidator  ()
{
    ClearEdges();


    for( auto & point : m_intersections )
    {
        delete point;
    }

    if( !m_polyline.empty() && m_resultPolylines.empty() )
    {
        for( auto & point : m_polyline )
        {
            delete point;
        }
    }
    else
    {
        for( auto & polyline : m_resultPolylines )
        {
            for( auto point : polyline )
            {
                delete point;
            }
        }
    }
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
    for( size_t i = 0; i < polyline.size(); i++ )
    {
        size_t j = i < polyline.size() - 1 ? i + 1 : 0;
        //m_edgeList.push_back( new p2t::Edge( *polyline[ i ], *polyline[ j ] ) );

        if( !Equal( *polyline[ i ], *polyline[ j ] ) )
        {
            m_edgeList.push_back( new p2t::Edge( *polyline[ i ], *polyline[ j ] ) );
        }
        else
        {
            if( j == 0 )
            {
                delete m_polyline[ i ];
                delete m_edgeList[ i - 1 ];
                m_polyline.erase( m_polyline.begin() + i );
                m_edgeList.erase( m_edgeList.begin() + i - 1 );

                m_edgeList.push_back( new p2t::Edge( *polyline[ i - 1 ], *polyline[ 0 ] ) );
            }
            else
            {
                delete m_polyline[ j ];
                m_polyline.erase( m_polyline.begin() + j );

                --i;    // Next point ca be equal too.
            }
        }
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
PolylinesVec &&         PolylineValidator::StealDecomposedPolylines()
{
    ClearEdges();

    // Return source polylines, if no decomposition was needed.
    if( m_resultPolylines.empty() && !m_polyline.empty() )
    {
        m_resultPolylines.push_back( std::move( m_polyline ) );
    }
    else
    {
        m_polyline.clear();
    }

    return std::move( m_resultPolylines );
}

// ***********************
//
IntersectionsVec &&     PolylineValidator::StealIntersections     ()
{
    ClearEdges();
    return std::move( m_intersections );
}

// ***********************
//
void    PolylineValidator::ClearEdges  ()
{
    for( auto & point : m_intersections )
        point->edge_list.clear();

    for( auto & point : m_polyline )
        point->edge_list.clear();

    for( auto & edge : m_edgeList )
        delete edge;

    m_edgeList.clear();
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
// @deprecated
void    PolylineValidator::RepairRepeatPoints   ()
{
    for( int i = 0; i < m_polyline.size(); ++i )
    {
        int j = i < m_polyline.size() - 1 ? i + 1 : 0;
        if( Equal( *m_polyline[ i ], *m_polyline[ j ] ) )
        {
            delete m_polyline[ j ];
            m_polyline.erase( m_polyline.begin() + j );

            --i;    // Next point ca be equal too.
        }
    }

    for( int i = 0; i < m_edgeList.size(); ++i )
    {
        if( Equal( *m_edgeList[ i ]->p, *m_edgeList[ i ]->q ) )
        {
            delete m_edgeList[ i ];
            m_edgeList.erase( m_edgeList.begin() + i );

            --i;    // Next point ca be equal too.
        }
    }
}

// ***********************
// Checks if intersections are valid for further decomposition.
void    PolylineValidator::ValidateIntersections    ( const IntersectionsVec & intersect )
{
    // FIXME: Two intersections shouldn't lie on the same edge.
    for( auto inter1 = intersect.begin(); inter1 != intersect.end(); inter1++ )
    {
        for( auto inter2 = inter1 + 1; inter2 != intersect.end(); inter2++ )
        {
            if( inter1 == inter2 )
                continue;

            auto edge11 = ( *inter1 )->edge_list[ 0 ];
            auto edge12 = ( *inter1 )->edge_list[ 1 ];

            auto edge21 = ( *inter2 )->edge_list[ 0 ];
            auto edge22 = ( *inter2 )->edge_list[ 1 ];

            if( edge11 == edge21 ||
                edge11 == edge22 ||
                edge12 == edge21 ||
                edge12 == edge22 )
                throw std::runtime_error( "[PolylineValidator] One edge is intersected multpile times. Fix this in future versions." );
        }
    }
}

// ***********************
//
std::vector< IntersectionData > PolylineValidator::InitDividePoints ()
{
    std::vector< IntersectionData > dividePoints;
    dividePoints.reserve( 2 * m_intersections.size() );  // Each intersection gives 2 divide points.
                                                         // Divide polyline into ranges between intersection points.
    for( int i = 0; i < m_polyline.size(); ++i )
    {
        int nextPoint = i + 1 < m_polyline.size() ? i + 1 : 0;
        for( int j = 0; j < m_intersections.size(); j++ )
        {
            if( IsIntersectionStart( m_polyline[ i ], m_polyline[ nextPoint ], m_intersections[ j ] ) )
            {
                IntersectionData data;
                data.IntersectionPoint = m_intersections[ j ];
                data.Processed = false;
                data.PolylineIdx = i;

                dividePoints.push_back( data );
                break;
            }
        }
    }

    return dividePoints;
}

// ***********************
// Implementation based on http://geomalgorithms.com/a09-_intersect-3.html
const IntersectionsVec &        PolylineValidator::FindSelfIntersections   ()
{
    // Two points with same coordinates can't exist.
    //RepairRepeatPoints();

    std::deque< Event > eventQueue = InitEventQueue();
    std::vector< p2t::Edge* > sweepLine;
    std::vector< p2t::Point* > intersection;


    while( !eventQueue.empty() )
    {
        auto event = eventQueue[ 0 ];
        eventQueue.pop_front();
        
        if( event.Type == EventType::StartPoint )
            ProcessBeginPoint( event, eventQueue, sweepLine );
        else if( event.Type == EventType::EndPoint )
            ProcessEndPoint( event, eventQueue, sweepLine );
        else
            ProcessIntersectionPoint( event, eventQueue, sweepLine, intersection );
    }

    return m_intersections;
}

// ***********************
//
const PolylinesVec &         PolylineValidator::DecomposeContour        ()
{
    PolylinesVec& polylines = m_resultPolylines;
    if( m_intersections.empty() )
    {
        polylines.push_back( m_polyline );
        return polylines;
    }

    ValidateIntersections( m_intersections );

    // Number of intersection is maximal number of separate segments.
    polylines.reserve( m_intersections.size() + 1 );
    
    // Collect intersection points indicies.
    std::vector< IntersectionData > dividePoints = InitDividePoints();



    // Connect end of m_polyline with begining.
    polylines.push_back( Polyline() );
    for( int idx = dividePoints.back().PolylineIdx + 1; idx < m_polyline.size(); idx++ )
    {
        polylines[ 0 ].push_back( m_polyline[ idx ] );
    }

    for( int idx = 0; idx <= dividePoints.front().PolylineIdx; idx++ )
    {
        polylines[ 0 ].push_back( m_polyline[ idx ] );
    }

    dividePoints.back().Processed = true;


    // Add intersection point
    auto dividePoint = dividePoints[ 0 ].IntersectionPoint;
    polylines[ 0 ].push_back( new p2t::Point( dividePoint->x, dividePoint->y ) );

    // Connect ranges into contours.
    int intersectIdx = FindNextContourPart( dividePoints, m_polyline, m_polyline[ dividePoints.front().PolylineIdx ], 0 );


    int curPolyline = 0;
    if( intersectIdx < 0 )
    {
        intersectIdx = FindFreeDividePoint( dividePoints );
        curPolyline = 1;
        polylines.push_back( Polyline() );
    }

    do
    {
        // Make single closed contour.
        curPolyline = (int)polylines.size() - 1;

        do
        {
            int beginIdx = dividePoints[ intersectIdx ].PolylineIdx + 1;
            int endIdx = dividePoints[ intersectIdx + 1 ].PolylineIdx;

            // Rewrite contour points to polyline.
            for( int i = beginIdx; i <= endIdx; ++i )
            {
                polylines[ curPolyline ].push_back( m_polyline[ i ] );
            }

            // Add intersection point
            auto dividePoint = dividePoints[ intersectIdx + 1 ].IntersectionPoint;
            polylines[ curPolyline ].push_back( new p2t::Point( dividePoint->x, dividePoint->y ) );

            // Set current points range as used.
            dividePoints[ intersectIdx ].Processed = true;

            // Find contour continuation.
            intersectIdx = FindNextContourPart( dividePoints, m_polyline, m_polyline[ endIdx ], intersectIdx + 1 );
        } while( intersectIdx != -1 );

        //// We added have duplicate point on contour closing. Delete it.
        //delete polylines[ curPolyline ].back();
        //polylines[ curPolyline ].pop_back();

        // Add place for new contour.
        polylines.push_back( Polyline() );

        // Find new segment. Returns -1 if all segments were used.
        intersectIdx = FindFreeDividePoint( dividePoints );
    } while( intersectIdx != -1 );

    // We added to many polylines (check loop above).
    polylines.pop_back();

    return polylines;
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
    // Segment start and end point are intersection, which we don't count in.
    //if( !IsSharedPointIntersection( event ) )
        m_intersections.push_back( event.Point );

    assert( event.Point->edge_list.size() >= 2 );

    auto edge1Idx = BruteFindInSweepLine( event.Point->edge_list[ 0 ], sweepLine );
    auto edge2Idx = BruteFindInSweepLine( event.Point->edge_list[ 1 ], sweepLine );
    assert( edge1Idx >= 0 );
    assert( edge2Idx >= 0 );

    p2t::Edge * aboveEdge = nullptr;
    p2t::Edge * belowEdge = nullptr;
    int aboveIdx;
    int belowIdx;

    if( edge1Idx > edge2Idx )
    {
        aboveEdge = sweepLine[ edge1Idx ];      aboveIdx = edge1Idx;
        belowEdge = sweepLine[ edge2Idx ];      belowIdx = edge2Idx;
    }
    else
    {
        belowEdge = sweepLine[ edge1Idx ];      belowIdx = edge1Idx;
        aboveEdge = sweepLine[ edge2Idx ];      aboveIdx = edge2Idx;
    }


    // Swap after reaching intersection point in event queue.
    std::iter_swap( sweepLine.begin() + aboveIdx, sweepLine.begin() + belowIdx );

    p2t::Edge * aboveAboveEdge = GetAboveEdge( aboveIdx, sweepLine );
    p2t::Edge * belowBelowEdge = GetBelowEdge( belowIdx, sweepLine );

    // Note: we swapped below with above. Now we compare new above edge with above above edge.
    auto aboveIntersect = GetIntesection( belowEdge, aboveAboveEdge );
    auto belowIntersect = GetIntesection( aboveEdge, belowBelowEdge );

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
    const float epsilon = 0.00005f;
    if( edge1Factor >= 0.0f + epsilon && edge1Factor <= 1.0f - epsilon &&
        edge2Factor >= 0.0f + epsilon && edge2Factor <= 1.0f - epsilon )
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
    auto sweepY = addEdge->q->y;    // Take upper point y value.

    if( sweepLine.size() == 0 )
    {
        sweepLine.push_back( addEdge );
        return 0;
    }
    else if( sweepLine.size() == 1 )
    {
        if( CompareEdge( addEdge, sweepLine[ 0 ], sweepY ) )
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
        for( int i = 0 ; i < sweepLine.size(); ++i )
        {
            auto edge = sweepLine[ i ];
            if( CompareEdge( addEdge, edge, sweepY ) )
            {
                sweepLine.insert( sweepLine.begin() + i, addEdge );
                return i;
            }
        }

        sweepLine.push_back( addEdge );
        return (int)sweepLine.size() - 1;

        //// Binary search 
        //size_t left = 0;
        //auto right = sweepLine.size();
        //auto span = right - left;

        //while( span > 1 )
        //{
        //    span = span / 2;
        //    auto pos = left + span;

        //    auto edge = sweepLine[ pos ];
        //    if( CompareEdge( addEdge, edge, sweepY ) )
        //    {
        //        right = pos;
        //    }
        //    else
        //    {
        //        left = pos;
        //    }
        //}

        //auto edge = sweepLine[ left ];
        //if( CompareEdge( addEdge, edge, sweepY ) )
        //{
        //    sweepLine.insert( sweepLine.begin() + left, addEdge );
        //    return (int)left;
        //}
        //else
        //{
        //    if( left + 1 < sweepLine.size() )
        //    {
        //        sweepLine.insert( sweepLine.begin() + left + 1, addEdge );
        //        return int( left + 1 );
        //    }
        //    else
        //    {
        //        sweepLine.push_back( addEdge );
        //        return (int)sweepLine.size() - 1;
        //    }
        //}
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

    if( leftX == rightX )
    {
        // Probably we have two edges with beginning in the same point.
        // Check which segment makes greater progress in x axis.
        auto leftDelta = EdgeDelta( leftEdge );
        auto rightDelta = EdgeDelta( rightEdge );
        if( leftDelta < rightDelta )
            return true;
    }

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

// ================================ //
//
double          EdgeDelta       ( p2t::Edge * edge )
{
    p2t::Point* point1 = edge->p;
    p2t::Point* point2 = edge->q;

    auto deltaX = point1->x - point2->x;
    auto deltaY = point2->y - point1->y;

    return deltaX / deltaY;
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
        if( a.x > b.x )
        {
            return true;
        }
    }
    return false;
}

// ================================ //
//
bool			Equal			( const p2t::Point & point1, const p2t::Point & point2 )
{
    if( abs( point1.x - point2.x ) < epsilon && abs( point1.y - point2.y ) < epsilon )
        return true;
    return false;
}


// ***********************
//
bool            IsIntersectionStart ( const p2t::Point * polylinePoint, const p2t::Point * nextPoint, const p2t::Point * intersectionPoint )
{
    assert( intersectionPoint->edge_list.size() == 2 );

    // Note: Make pointers comparision.
    if( polylinePoint == intersectionPoint->edge_list[ 0 ]->p &&
        nextPoint == intersectionPoint->edge_list[ 0 ]->q )
        return true;

    if( polylinePoint == intersectionPoint->edge_list[ 0 ]->q &&
        nextPoint == intersectionPoint->edge_list[ 0 ]->p )
        return true;

    if( polylinePoint == intersectionPoint->edge_list[ 1 ]->p &&
        nextPoint == intersectionPoint->edge_list[ 1 ]->q )
        return true;

    if( polylinePoint == intersectionPoint->edge_list[ 1 ]->q &&
        nextPoint == intersectionPoint->edge_list[ 1 ]->p )
        return true;

    return false;
}


// ***********************
//
int             FindFreeDividePoint ( std::vector< IntersectionData > & data )
{
    for( int i = 0; i < data.size(); ++i )
    {
        if( !data[ i ].Processed )
            return i;
    }

    return -1;
}

// ***********************
// http://geomalgorithms.com/a09-_intersect-3.html - decompose into Simple Pieces
int             FindNextContourPart ( std::vector< IntersectionData > & data, Polyline & polyline, const p2t::Point * partEnd, int intersectionIdx )
{
    auto & intersect = data[ intersectionIdx ];
    auto point = intersect.IntersectionPoint;

    // There are two occurances of one intersection point in data array. One we already met (intersectionIdx).
    // We must find other.
    for( int i = 0; i < data.size(); ++i )
    {
        if( !data[ i ].Processed && data[ i ].IntersectionPoint == point && intersectionIdx != i )
            return i;
    }

    return -1;

    //// q is upper point, p is lower point.
    //// If edge goes from top to bottom, we choose top point from second edge.
    //// Check link in comment to this function.
    //if( point->edge_list[ 0 ]->p == partEnd )
    //{
    //    nextPoint = point->edge_list[ 1 ]->p;
    //}
    //else if( point->edge_list[ 0 ]->q == partEnd )
    //{
    //    nextPoint = point->edge_list[ 1 ]->q;
    //}
    //else if( point->edge_list[ 1 ]->p == partEnd )
    //{
    //    nextPoint = point->edge_list[ 0 ]->p;
    //}
    //else if( point->edge_list[ 1 ]->q == partEnd )
    //{
    //    nextPoint = point->edge_list[ 0 ]->p;
    //}
    //else
    //    assert( false );

    //// Iterate all segments of contours to find proper continuation point.
    //for( int i = 0; i < data.size(); ++i )
    //{
    //    auto polylineIdx = data[ i ].PolylineIdx;
    //    if( !data[ i ].Processed && polyline[ polylineIdx ] == nextPoint )
    //        return i;
    //}

    //return -1;
}

// ***********************
//
bool            IsSharedPointIntersection   ( const Event& intersectEvent )
{
    assert( intersectEvent.Type == EventType::Intersection );

    if( intersectEvent.Point->edge_list[ 0 ]->p == intersectEvent.Point->edge_list[ 1 ]->p )
        return true;
    if( intersectEvent.Point->edge_list[ 0 ]->q == intersectEvent.Point->edge_list[ 1 ]->q )
        return true;

    if( intersectEvent.Point->edge_list[ 0 ]->p == intersectEvent.Point->edge_list[ 1 ]->q )
        return true;
    if( intersectEvent.Point->edge_list[ 1 ]->p == intersectEvent.Point->edge_list[ 0 ]->q )
        return true;

    return false;
}

} //triangulator
