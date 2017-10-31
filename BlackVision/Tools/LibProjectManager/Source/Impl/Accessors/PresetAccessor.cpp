#include "PresetAccessor.h"

#include "AssetAccessor.h"

#include "SceneDescriptor.h"
#include "Serialization/BV/XML/BVXMLSerializer.h"
#include "Serialization/BV/XML/BVXMLDeserializer.h"
#include "Serialization/BV/BVSerializeContext.h"
#include "Serialization/BV/BVDeserializeContext.h"

#include "Serialization/SerializationHelper.h"
#include "Serialization/Json/JsonDeserializeObject.h"
#include "Assets/Thumbnail/Impl/PresetThumbnail.h"

#include "Engine/Models/Interfaces/ITimeEvaluator.h"
#include "Engine/Models/Timeline/TimeEvaluatorBase.h"

#include "IO/DirIO.h"

#include "Assets/AssetDescsWithUIDs.h"

namespace bv
{

std::string PresetAccessor::m_fileExt = ".*\\.bvpreset";

// ********************************
//
PresetAccessorConstPtr PresetAccessor::Create( const Path & path )
{
    return PresetAccessorConstPtr( new PresetAccessor( path ) );
}

// ********************************
//
PresetAccessor::PresetAccessor	( const Path & path )
    : m_path( path )
{}

// ********************************
//
void                        PresetAccessor::SavePreset ( const model::BasicNodePtr node, const Path & path ) const
{    
    auto context = BVSerializeContext::CreateContextFromEmptiness();
    BVXMLSerializer ser( context );

    auto bvSerCo = ser.GetBVSerializeContext();
    bvSerCo->sceneNameInTimeline = false;
    
    ser.EnterChild( "preset" );

    auto timelines = node->GetTimelines( true );

    ser.EnterArray( "timelines" );

    for( auto t : timelines )
    {
        t->Serialize( ser );
    }

    ser.ExitChild(); // timelines

    node->Serialize( ser );   

    ser.ExitChild(); // preset

    auto fileName = m_path / path;

    auto dirName = fileName.ParentPath();

    Dir::CreateDir( dirName.Str(), true );

    File::Touch( fileName.Str() );

    ser.Save( fileName.Str() );
}
    
// ********************************
//
model::BasicNodePtr         PresetAccessor::LoadPreset( const Path & path, const model::OffsetTimeEvaluatorPtr & timeline ) const
{
    if( Path::Exists( m_path / path ) )
    {
        auto context = BVDeserializeContext::CreateContextFromEmptiness( timeline );
        BVXMLDeserializer deser( ( m_path / path ).Str(), context );

        deser.EnterChild( "preset" );

        auto timelines = SerializationHelper::DeserializeArray< model::TimeEvaluatorBase< model::ITimeEvaluator > >( deser, "timelines" );

        for( auto t : timelines )
        {
            if( !timeline->GetChild( t->GetName() ) )
            {
                timeline->AddChild( t );
            }
        }

        deser.EnterChild( "node" );
        auto nodePreset = model::BasicNodePtr( model::BasicNode::Create( deser ) );
        deser.ExitChild(); // node

        deser.ExitChild(); // preset

        return nodePreset;
    }
    else
    {
        return nullptr;
    }
}

// ********************************
//
PathVec                     PresetAccessor::ListPresets( const Path & path, bool recursive ) const
{
    auto presetList = Path::List( ( m_path / path ).Str(), recursive, m_fileExt );

    for( auto & p : presetList )
    {
        p = Path::RelativePath( p, m_path );
    }

    return presetList;
}

// ********************************
//
PathVec                     PresetAccessor::ListPresets() const
{
    return ListPresets( m_path, true );
}

// ***********************
//
ThumbnailConstPtr           PresetAccessor::GetPresetThumbnail  ( const Path & path ) const
{
    auto absolutePath = m_path / path;
    Path thumbPath = AssetAccessor::GetThumbnailPath( absolutePath );

    if( !Path::Exists( thumbPath ) )
        return nullptr;

    auto context = BVDeserializeContext::CreateContextFromEmptiness();
    JsonDeserializeObject deser( context );
    deser.LoadFile( thumbPath.Str() );

    auto thumb = PresetThumbnail::Create( deser );
    return thumb;
}

} // bv