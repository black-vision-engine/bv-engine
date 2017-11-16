#include "gtest/gtest.h"

#include "Serialization/ISerializer.h"
#include "Serialization/IDeserializer.h"

#include "Serialization/Json/JsonSerializeObject.h"
#include "Serialization/Json/JsonDeserializeObject.h"
#include "Serialization/XML/XMLSerializer.h"
#include "Serialization/XML/XMLDeserializer.h"

using namespace bv;



// ***********************
// This test replicates deserializer bug https://www.pivotaltracker.com/story/show/152918006
// Deserializer crashed in destructor when it tried to load file and failed loading it.
TEST( Serialization_Serializers_XML, CreateAndLoadNonExistingFile )
{
    XMLDeserializer deser;
    ASSERT_FALSE( deser.LoadFile( "NonExistingFile.xml" ) );
}
