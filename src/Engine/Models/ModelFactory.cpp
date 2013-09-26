#include "ModelFactory.h"

#include "Engine\Models\Plugins\Transform\PluginTransformSimple.h"
#include "Engine\Models\Plugins\Geometry\PluginGeometryRect.h"
#include "Engine\Models\Plugins\Geometry\PluginGeometryRing.h"
#include "Engine\Models\BasicNode.h"

namespace bv { namespace model {

// ********************************
//
//PluginGeometryRect *      ModelFactory::CreatePluginGeometryRect( float w, float h )
//{
//    return new PluginGeometryRect( w, h );
//}
//
//// ********************************
////
//PluginGeometryRing *     ModelFactory::CreatePluginGeometryRing( const FloatInterpolator & innerRadius, const FloatInterpolator & outerRadius,
//                                                                 const FloatInterpolator & startAngle, const FloatInterpolator & endAngle, int numSegments )
//{
//    return new PluginGeometryRing( innerRadius, outerRadius,	startAngle, endAngle, numSegments );
//}
//
//// ********************************
////
//PluginTransformSimple *  ModelFactory::CreatePluginTransformSimple( const TransformF & transform )
//{
//    return new PluginTransformSimple( transform );
//}

// ********************************
//
BasicNode *              ModelFactory::CreateNode( ITransformPlugin * tPlugin, IGeometryGenPlugin * gPlugin, IShaderPlugin * pShaderPlugin, 
                                                   IShaderPlugin * vShaderPlugin, IShaderPlugin * gShaderPlugin, BasicNode * parent)
{
    BasicNode * ret = new BasicNode();

    //ret->addTransformPlugin( tPlugin );
    //ret->setGeometryShaderPlugin( gShaderPlugin );
    //ret->setPixelShaderPlugin( pShaderPlugin );
    //ret->setVertexShaderPlugin( vShaderPlugin );

    if( parent != nullptr )
    {
        parent->AddChild( ret );
    }

    return ret;
}

} // model
} // bv