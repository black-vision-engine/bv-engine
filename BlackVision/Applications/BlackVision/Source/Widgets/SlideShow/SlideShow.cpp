#include "stdafx.h"

#include "SlideShow.h"

#include "Engine/Events/EventHandlerHelpers.h"
#include "Widgets/NodeLogicHelper.h"

#include "Serialization/BV/BVDeserializeContext.h"
#include "Serialization/SerializationHelper.h"
#include "Tools/StringHeplers.h"

#include "Engine/Models/BVProjectEditor.h"


namespace bv { namespace nodelogic {


const std::string           SlideShow::m_type                       = "slide_show";
const Float32               SlideShow::m_defaultFadeTime            = 1000;

// *******************************

const std::string           SlideShow::ACTION::START_SLIDE_SHOW     = "StartSlideShow";
const std::string           SlideShow::ACTION::STOP_SLIDE_SHOW      = "StopSlideShow";
const std::string           SlideShow::ACTION::PAUSE_SLIDE_SHOW     = "PauseSlideShow";


// *******************************
//
SlideShowPtr	            SlideShow::Create				( model::BasicNodePtr parent, model::ITimeEvaluatorPtr timeEval )
{
	return std::make_shared< SlideShow >( parent, timeEval );
}

// *******************************
//
SlideShow::SlideShow        ( model::BasicNodePtr parent, model::ITimeEvaluatorPtr timeEval )
    : m_parentNode( parent )
    , m_timeEval( timeEval )
    , m_fadeInTime( m_defaultFadeTime )
    , m_fadeOutTime( m_defaultFadeTime )
{
}

// *******************************
//
SlideShow::~SlideShow     ()
{
}

// *******************************
//
void		        SlideShow::Update			( TimeType )
{
}

// ***********************
//
void                SlideShow::Serialize       ( ISerializer & ser ) const
{
    auto context = static_cast< BVSerializeContext * >( ser.GetSerializeContext() );
    assert( context != nullptr );

    ser.EnterChild( "logic" );

    ser.SetAttribute( "type", m_type );

    if( context->detailedInfo )     // Without detailed info, we need to serialize only logic type.
    {
        ser.SetAttribute( "fadeInTime", SerializationHelper::T2String( m_fadeInTime ) );
        ser.SetAttribute( "fadeOutTime", SerializationHelper::T2String( m_fadeOutTime ) );

        auto timeline = model::TimelineManager::GetInstance()->GetTimelinePath( m_timeEval );
        ser.SetAttribute( "timelinePath", timeline );
    }

    ser.ExitChild();
}

// ***********************
//
SlideShowPtr            SlideShow::Create       ( const IDeserializer & deser, bv::model::BasicNodePtr parent )
{
    auto timelinePath = deser.GetAttribute( "timelinePath" );

    if( !timelinePath.empty() )
    {
        auto deserContext = static_cast< BVDeserializeContext * >( deser.GetDeserializeContext() );
        if( deserContext )
        {
            model::ITimeEvaluatorPtr sceneTimeline = deserContext->GetSceneTimeline();
            if( !sceneTimeline )
            {
                sceneTimeline = model::TimelineManager::GetInstance()->GetRootTimeline();
            }
            auto timeEval = bv::model::TimelineHelper::GetTimeEvaluator( timelinePath, sceneTimeline );
        
            auto logic = SlideShow::Create( parent, timeEval );
            
            logic->m_fadeInTime = SerializationHelper::String2T< Float32 >( deser.GetAttribute( "fadeInTime" ), m_defaultFadeTime );
            logic->m_fadeOutTime = SerializationHelper::String2T< Float32 >( deser.GetAttribute( "fadeOutTime" ), m_defaultFadeTime );

            return logic;
        }
    }
    return nullptr;
}

// ***********************
//
bool                    SlideShow::HandleEvent      ( IDeserializer & /*eventSer*/, ISerializer & /*response*/, BVProjectEditor * /*editor*/ )
{
    //std::string action = eventSer.GetAttribute( "Action" );

    //auto context = static_cast< BVDeserializeContext * >( eventSer.GetDeserializeContext() );
    //auto scene = editor->GetModelScene( context->GetSceneName() );

    //if( action == ACTION::UPDATE_PIECHART )
    //{
    //    UpdateChart();
    //    return true;
    //}
    //else if( action == ACTION::UPDATE_PIESLICE )
    //{
    //    auto sliceDescIdx = SerializationHelper::String2T< UInt32 >( eventSer.GetAttribute( "pieSliceIdx" ), 0 );
    //    if( RemoveSlice( scene, sliceDescIdx, editor ) )
    //    {
    //        auto sliceDesc = SerializationHelper::DeserializeObject< PieSliceDesc >( eventSer, "pieSlice" );
    //        if( sliceDesc )
    //        {
    //            AddSlice( scene, sliceDesc, sliceDescIdx, editor );
    //            UpdateChart();
    //            return true;
    //        }
    //    }
    //    return false;
    //}
    //else if( action == ACTION::ADD_PIESLICE )
    //{
    //    auto sliceDesc = SerializationHelper::DeserializeObject< PieSliceDesc >( eventSer, "pieSlice" );
    //    if( sliceDesc )
    //    {
    //        AddSlice( scene, sliceDesc, ( UInt32 )m_slicesDesc.size(), editor );
    //        UpdateChart();
    //        return true;
    //    }
    //    return false;
    //}
    //else if( action == ACTION::REMOVE_PIESLICE )
    //{
    //    auto sliceDescIdx = SerializationHelper::String2T< UInt32 >( eventSer.GetAttribute( "pieSliceIdx" ), 0 );
    //    if( RemoveSlice( scene, sliceDescIdx, editor ) )
    //    {
    //        UpdateChart();
    //        return true;
    //    }
    //    return false;
    //}
    //else 
    //{
    //    response.SetAttribute( ERROR_INFO_STRING, "Unknown command. This logic supports only 'Replicate' command." );
    //}

    return false;
}

// ***********************
//
const std::string &         SlideShow::Type                 ()
{
    return m_type;
}

// ***********************
//
const std::string &         SlideShow::GetType              () const
{
    return Type();
}

} //nodelogic
} //bv