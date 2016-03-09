#pragma once

#include <map>
#include <string>

#include "Serialization/ISerializer.h"
#include "Serialization/ISerializable.h"

namespace bv
{


typedef std::map< std::string, ISerializer * >      SceneVariablesMap;

class SceneVariables    : public ISerializable
{
private:

    SceneVariablesMap       m_varCollection;

public:
    SceneVariables();
    ~SceneVariables();

    virtual void                Serialize       ( ISerializer & ser ) const override;
    static ISerializablePtr     Create          ( const IDeserializer & deser );


    bool                        AddVariable     ( const std::string & varName, IDeserializer & deser );
    bool                        DeleteVariable  ( const std::string & varName );
    
    const ISerializer *         GetVariable     ( const std::string & varName ) const;
};


}	// bv
