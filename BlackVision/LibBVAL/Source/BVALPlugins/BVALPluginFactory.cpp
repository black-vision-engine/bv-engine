#include "BVALPluginFactory.h"

#include "BVALPlugins/BVALPlugin.h"


namespace bv { 
    
namespace {

BVALPlugin					defaultPlugin;

}

// *****************************
//
BVALPlugin &		BVALPluginFactory::BVAL	()
{
	return BVAL( BVALPluginType::BVAL_DEFAULT );
}

// *****************************
//
BVALPlugin &		BVALPluginFactory::BVAL	( BVALPluginType bvalType )
{
    { bvalType; }
	return defaultPlugin;
}

} //bv
