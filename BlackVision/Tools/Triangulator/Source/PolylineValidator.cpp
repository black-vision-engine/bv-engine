#include "PolylineValidator.h"


#include <algorithm>
#include <deque>


bool            CompareEvent    ( const Event a, const Event b );



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
    //Sort();
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

        // Add Edge to first point. Note that polyline[ j ] edge is st in Edge contructor.
        //polyline[ i ]->edge_list.push_back( m_edgeList[ m_edgeList.size() - 1 ] );
    }
}

// ***********************
//
void    PolylineValidator::Sort         ()
{
    std::sort( m_polyline.begin(), m_polyline.end(), p2t::cmp );
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
//
void    PolylineValidator::MakeSimplePolygon   ()
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

}

// ***********************
//
std::deque< Event >     PolylineValidator::InitEventQueue  ()
{
    std::deque< Event > eventQueue;

    // Fill event queue with all contours end points.
    for( int i = 0; i < m_polyline.size() - 1; i++ )
    {
        // Note: we take first edge since there're no repeat points
        eventQueue.push_back( Event( EventType::StartPoint, m_polyline[ i ], m_polyline[ i ]->edge_list[ 0 ] ) );
        eventQueue.push_back( Event( EventType::EndPoint, m_polyline[ i + 1 ], m_polyline[ i ]->edge_list[ 0 ] ) );
    }
    // Add last contour point
    eventQueue.push_back( Event( EventType::StartPoint, m_polyline[ m_polyline.size() ], m_polyline[ m_polyline.size() ]->edge_list[ 0 ] ) );
    eventQueue.push_back( Event( EventType::EndPoint, m_polyline[ 0 ], m_polyline[ m_polyline.size() ]->edge_list[ 0 ] ) );

    std::sort( eventQueue.begin(), eventQueue.end(), CompareEvent );

    return eventQueue;
}

// ***********************
//
void                    PolylineValidator::ProcessBeginPoint           ( Event & event, std::deque< Event > & eventQueue, std::vector< p2t::Edge* >& sweepLine )
{


}

// ***********************
//
void                    PolylineValidator::ProcessEndPoint             ( Event & event, std::deque< Event > & eventQueue, std::vector< p2t::Edge* >& sweepLine )
{



}

// ***********************
//
void                    PolylineValidator::ProcessIntersectionPoint    ( Event & event, std::deque< Event > & eventQueue, std::vector< p2t::Edge* >& sweepLine, std::vector< p2t::Point* > & intersections )
{


}


bool            CompareEvent    ( const Event a, const Event b )
{
    if( a.Point->y < b.Point->y )
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
    }
    return false;
}

