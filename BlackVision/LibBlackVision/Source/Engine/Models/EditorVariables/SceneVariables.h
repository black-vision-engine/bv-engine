#pragma once

#include <map>
#include <string>

#include "Serialization/ISerializer.h"
#include "Serialization/ISerializable.h"
#include "Serialization/SerializationHelper.h"

namespace bv
{


typedef std::map< std::string, std::string >      SceneVariablesMap;

class SceneVariables    : public ISerializable
{
private:

    SceneVariablesMap       m_varCollection;

public:
    SceneVariables();
    ~SceneVariables();

    virtual void                Serialize       ( ISerializer & ser ) const override;
    void                        Deserialize     ( const IDeserializer & deser );


    bool                        AddVariable     ( const std::string & varName, const std::string & varContent );
    bool                        DeleteVariable  ( const std::string & varName );
    
    Expected< std::string >     GetVariable     ( const std::string & varName ) const;
};


}	// bv
