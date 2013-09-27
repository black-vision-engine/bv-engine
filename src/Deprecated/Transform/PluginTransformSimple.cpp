#include "PluginTransformSimple.h"

#include "Engine/Models/Plugins/Parameter.h"

namespace bv
{
// ***************************** DESCRIPTOR ********************************** 
//PLUGIN NAME
//const std::string SimpleTransformPluginParametersDescriptor::pluginName( "transform.simple" );
//
//// *************************************
////
//SimpleTransformPluginParametersDescriptor::SimpleTransformPluginParametersDescriptor()
//    : BaseParametersDescriptor( pluginName )
//{
//}
//
//// ***************************** PLUGIN ********************************** 
//
//// *********************************
////
//PluginTransformSimple::PluginTransformSimple( const TransformF & transInterpolator )
//{
//    m_transParam    = new ParamTransform( "localTransform", transInterpolator );
//    m_value         = new ValueMat4     ( "MVP" );
//    
//    RegisterValue( m_value );
//}
//
//// *********************************
////
//void PluginTransformSimple::Update( float t )
//{
//    if( m_transParam->IsEvaluationNeeded( t ) )
//    {
//        m_value->SetValue( m_transParam->Evaluate( t ) );
//    }
//}
//
//// *********************************
////
//PluginTransformSimple::~PluginTransformSimple  ()
//{
//    delete m_transParam;
//}

} // bv