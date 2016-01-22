#include "PresetAccessor.h"
#include "SceneDescriptor.h"
#include "Serialization/BV/XML/BVXMLSerializer.h"
#include "Serialization/BV/XML/BVXMLDeserializer.h"

#include "Serialization/SerializationHelper.h"

#include "Engine/Models/Interfaces/ITimeEvaluator.h"
#include "Engine/Models/Timeline/TimeEvaluatorBase.h"

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
    auto ser = BVXMLSerializer();
    
    ser.EnterChild( "preset" );

    auto timelines = node->GetTimelines( true );

    ser.EnterArray( "timelines" );

    for( auto t : timelines )
    {
        t->Serialize( ser );
    }

    ser.ExitChild(); // timelines

    ser.EnterChild( "node" );
    node->Serialize( ser );   
    ser.ExitChild(); // node

    ser.ExitChild(); // preset

    ser.Save( ( m_path / path ).Str() );
}
    
// ********************************
//
model::BasicNodePtr         PresetAccessor::LoadPreset( const Path & path, const model::OffsetTimeEvaluatorPtr & timeline ) const
{
    BVXMLDeserializer deser( ( m_path / path ).Str(), timeline, nullptr );

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

// ********************************
//
PathVec                     PresetAccessor::ListPresets( const Path & path ) const
{
    return Path::List( ( m_path / path ).Str(), true, m_fileExt );
}

// ********************************
//
PathVec                     PresetAccessor::ListPresets() const
{
    return Path::List( m_path, true, m_fileExt );
}

} // bv