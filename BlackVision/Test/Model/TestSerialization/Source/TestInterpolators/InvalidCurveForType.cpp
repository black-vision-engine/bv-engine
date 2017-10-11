#include "gtest/gtest.h"



#include "Mathematics/Transform/MatTransform.h"

#include "Utils/Comparators/ParamComparator.h"
#include "Utils/Serialization/Serialize.h"

#include "UseLoggerLibBlackVision.h"


#include "Mathematics/Interpolators/CompositeInterpolator.h"
#include "Mathematics/Interpolators/CompositeInterpolator.inl"


using namespace bv;




// ***********************
// Creating interpolator linear interpolator for string returns nullptr.
TEST( Serialization_ParamValModel, Interpolators_LinearCurveForString )
{
    auto interpolator = CreateDummyInterpolator( CurveType::CT_LINEAR, Key< TimeType, std::string >( 0.0, std::string() ), Key< TimeType, std::string >( 2.0, std::string() ), std::numeric_limits< float >::epsilon() );
    EXPECT_EQ( interpolator, nullptr );
}

// ***********************
// Creating interpolator linear interpolator for Wstring returns nullptr.
TEST( Serialization_ParamValModel, Interpolators_LinearCurveForwString )
{
    auto interpolator = CreateDummyInterpolator( CurveType::CT_LINEAR, Key< TimeType, std::wstring >( 0.0, std::wstring() ), Key< TimeType, std::wstring >( 2.0, std::wstring() ), std::numeric_limits< float >::epsilon() );
    EXPECT_EQ( interpolator, nullptr );
}

// ***********************
//
TEST( Serialization_ParamValModel, Interpolators_LinearCurveForStringXML )
{
    auto actual = Deserialize< CompositeInterpolator< TimeType, std::string > >( "TestAssets/Serialization/Interpolators/InvalidCurveForString.xml", "interpolator" );

}

