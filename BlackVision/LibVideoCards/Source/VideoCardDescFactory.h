#pragma once

#include "Interfaces/IVideoCardDescriptor.h"
#include "Serialization/IDeserializer.h"


#include <map>
#include <vector>



namespace bv {
namespace videocards
{

typedef IVideoCardDescPtr ( *VideoCardDescCreateFun )( );
typedef std::vector< IVideoCardDescPtr > DescriptorsVec;

// ***********************
/// @ingroup VideoCards
class VideoCardDescFactory
{
private:

    std::map< std::string, VideoCardDescCreateFun >     m_descCreateFunMap;

public:

    IVideoCardDescPtr       CreateEmptyDescriptor       ( const std::string & cardName ) const;

    void                    RegisterDescriptor          ( const std::string & uid, VideoCardDescCreateFun createFun );
    void                    RegisterDefaultCreators     ();

    DescriptorsVec          ReadDescriptorsFromConfig   ( const IDeserializer & deser ) const;
};



}   // videocards
}   // bv



