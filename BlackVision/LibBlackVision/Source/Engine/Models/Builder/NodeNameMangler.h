#pragma once

#include "Engine/Models/BasicNode.h"



namespace bv {
namespace model
{


// ***********************
//
class NodeNameMangler
{
public:

    static std::string              GenerateNewName     ( const std::string & name, const TNodeVec & names );

private:

    static UInt32                   ChooseIndex         ( const std::vector< UInt32 > & usedIdx );
};




}   // model
}	// bv

