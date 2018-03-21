#include "stdafx.h"
#include "NodeNameMangler.h"

#include <regex>



namespace bv {
namespace model
{

std::string nodeNameIdxRegex( "\\(([[:digit:]]*)\\)$" );
//std::regex nodeNameRegex( "^(.*)\(([[:digit:]]*)\)$" );






// ***********************
//
std::string             NodeNameMangler::GenerateNewName        ( const std::string & name, const TNodeVec & names )
{
    std::vector< UInt32 > usedIdx;
    std::regex nodeIdxRegex( "^" + name + nodeNameIdxRegex );


    for( auto & node : names )
    {
        std::smatch sm;
        if( std::regex_match( node->GetName(), sm, nodeIdxRegex ) )
        {
            auto idxStr = sm[ 1 ].str();
            auto idx = Convert::String2T< UInt32 >( idxStr );
            usedIdx.push_back( idx );
        }
    }

    auto index = ChooseIndex( usedIdx );
    return name + "(" + Convert::T2String( index ) + ")";
}

// ***********************
//
UInt32                  NodeNameMangler::ChooseIndex            ( const std::vector< UInt32 > & usedIdx )
{
    auto maxValueIter = std::max_element( usedIdx.begin(), usedIdx.end() );
    if( maxValueIter != usedIdx.end() )
    {
        return *maxValueIter + 1;
    }

    return 1;
}

}   // model
}	// bv

