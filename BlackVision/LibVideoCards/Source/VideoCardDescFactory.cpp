#include "VideoCardDescFactory.h"

#include "Models/BlackMagic/BlackMagicVideoCard.h"
#include "Models/BlueFish/BlueFishVideoCard.h"


namespace bv {
namespace videocards
{

// ***********************
//
IVideoCardDescPtr       VideoCardDescFactory::CreateEmptyDescriptor        ( const std::string & cardName ) const
{
    auto iter = m_descCreateFunMap.find( cardName );
    if( iter != m_descCreateFunMap.end() )
    {
        return iter->second();
    }

    return nullptr;
}

// ***********************
//
void                    VideoCardDescFactory::RegisterDescriptor           ( const std::string & uid, VideoCardDescCreateFun createFun )
{
    if( m_descCreateFunMap.find( uid ) == m_descCreateFunMap.end() )
    {
        m_descCreateFunMap[ uid ] = createFun;
    }
}

// ***********************
//
void                    VideoCardDescFactory::RegisterDefaultCreators      ()
{
    RegisterDescriptor( bluefish::VideoCardDesc::UID(), &bluefish::VideoCardDesc::CreateDescriptor );
    RegisterDescriptor( blackmagic::VideoCardDesc::UID(), &blackmagic::VideoCardDesc::CreateDescriptor );
}

// ***********************
//
DescriptorsVec          VideoCardDescFactory::ReadDescriptorsFromConfig     ( const IDeserializer & deser ) const
{
    DescriptorsVec descs;

    if( deser.EnterChild( "videocards" ) )
    {
        if( deser.EnterChild( "videocard" ) )
        {
            do
            {
                auto name = deser.GetAttribute( "name" );
                auto videoCardDesc = CreateEmptyDescriptor( name );

                if( videoCardDesc )
                {
                    videoCardDesc->Deserialize( deser );
                    descs.push_back( videoCardDesc );
                }

            } while( deser.NextChild() );

            deser.ExitChild(); // videocard
        }

        deser.ExitChild(); // videocards
    }

    return descs;
}



}   // videocards
}	// bv

