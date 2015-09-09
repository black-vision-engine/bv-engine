#include "AnimationAssetDescriptor.h"

#include "IO/DirIO.h"


namespace bv {

const std::string AnimationAssetDesc::uid = "ANIMATION_ASSET_DESC";

// ***********************
//
void                AnimationAssetDesc::Serialize       ( SerializeObject & sob ) const
{
    assert( false );  // TODO: Needs rewriting. Serializa only m_path
    { sob; }
//sob.SetName( "asset" );
//    sob.SetValue( "type", "anim" );
//    
//    for( auto frame : GetFrames() )
//        frame->Serialize( sob );
//                
//sob.Pop();
}

// ***********************
//
ISerializableConstPtr     AnimationAssetDesc::Create          ( DeserializeObject & dob )
{
    assert( false );  // TODO: Needs rewriting. Serializa only m_path
    { dob; }
    //auto frames = dob.LoadProperties< const TextureAssetDesc >( "asset" );

    //return AnimationAssetDescConstPtr( new AnimationAssetDesc ( frames ) );
    return nullptr;
}

// *******************************
//
AnimationAssetDesc::AnimationAssetDesc							( const std::string & path, const std::string & filter )
    : m_path( path )
    , m_filter( filter )
{
    auto files = Dir::ListFiles( m_path, m_filter );

    if ( files.size() > 0 )
    {
	    for( auto f : files )
	    {
		    m_frames.push_back( TextureAssetDesc::Create( f, true ) );
        }
    }
}

//// *******************************
////
//AnimationAssetDesc::AnimationAssetDesc	( const std::vector< TextureAssetDescConstPtr > & frames )
//    : m_frames( frames )
//    , m_path( "" )
//{
//}


// *******************************
//
AnimationAssetDesc::~AnimationAssetDesc							()
{
}

// *******************************
//
const std::vector< TextureAssetDescConstPtr > & AnimationAssetDesc::GetFrames		() const
{
    return m_frames;
}

//// *******************************
////
//AnimationAssetDescConstPtr			AnimationAssetDesc::Create ( const std::vector< TextureAssetDescConstPtr > & frames )
//{
//    return AnimationAssetDescConstPtr( new AnimationAssetDesc ( frames ) );
//}

// *******************************
//
std::string                     AnimationAssetDesc::GetKey      () const
{
    return m_path;
}

// *******************************
//
AnimationAssetDescConstPtr		AnimationAssetDesc::CreateFromDirFrames( const std::string & path, const std::string & filter )
{
    return AnimationAssetDescConstPtr( new AnimationAssetDesc ( path, filter ) );
}

// *******************************
//
VoidConstPtr AnimationAssetDesc::QueryThis() const
{
	return shared_from_this();
}

// *******************************
//
const std::string &	AnimationAssetDesc::GetUID() const
{
	return AnimationAssetDesc::UID();
}

// *******************************
//
bool AnimationAssetDesc::IsCacheable() const
{
	return false;
}

// *******************************
//
const std::string &	AnimationAssetDesc::UID()
{
	return AnimationAssetDesc::uid;
}

} //bv
