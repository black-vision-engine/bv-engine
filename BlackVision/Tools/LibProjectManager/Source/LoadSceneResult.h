#pragma once

#include "Engine/Models/SceneModel.h"
#include "Exception.h"
#include "Expected.h"

namespace bv
{

class SceneWithWarnings
{
    model::SceneModelPtr                m_scene;
    Exceptions                          m_warnings;

public:
                                        SceneWithWarnings( model::SceneModelPtr scene )
                                            : m_scene( scene )
                                        {}

                                        SceneWithWarnings( model::SceneModelPtr scene, Exceptions warnings )
                                            : m_scene( scene )
                                            , m_warnings( warnings )
                                        {}

    model::SceneModelPtr                GetScene() const { return m_scene; }
    Exceptions                          GetWarnings() const { return m_warnings; }
};

class LoadSceneResult : public Expected< SceneWithWarnings >
{
public:
                                        LoadSceneResult( model::SceneModelPtr scene )
                                            : Expected( scene )
                                        {}
                                        LoadSceneResult( model::SceneModelPtr scene, Exceptions warnings )
                                            : Expected( SceneWithWarnings( scene, warnings ) )
                                        {}
                                        LoadSceneResult( const std::string & reason )
                                            : Expected( reason )
                                        {}
                                        LoadSceneResult( const Expected< SceneWithWarnings > & base )
                                            : Expected< SceneWithWarnings >( base )
                                        {}

    operator const model::SceneModelPtr ()
    {
        return GetVal().GetScene();
    }
};

}	// bv
