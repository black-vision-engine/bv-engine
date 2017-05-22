#pragma once

#include "Engine/Models/NodeLogics/NodeLogicBase.h"
#include "Engine/Models/SceneModel.h"
#include "Assets/Assets.h"


namespace bv { namespace nodelogic {

class MeshLoader;

DEFINE_PTR_TYPE( MeshLoader )
DEFINE_CONST_PTR_TYPE( MeshLoader )


class MeshLoader:  public model::NodeLogicBase, public std::enable_shared_from_this< MeshLoader >
{
private:

    static const std::string        m_type;

    struct ACTION 
    {
        static const std::string    LOAD;
        static const std::string    MESH_INFO;
        static const std::string    GET_ASSET_PATH;
        static const std::string    SET_ASSET_PATH;
    };

private:

    MeshAssetDescConstPtr           m_assetDesc;
    MeshAssetConstPtr               m_asset;

    bool                            m_textureEnabled;
    bool                            m_materialEnabled;

    model::BasicNodeWeakPtr         m_parentNode;
    model::ITimeEvaluatorPtr        m_timeEval;

public:

    explicit                        MeshLoader          ( model::BasicNodeWeakPtr parent, model::ITimeEvaluatorPtr timeEval, const std::string & assetPath );
                                    ~MeshLoader         ();

    static MeshLoaderPtr            Create              ( model::BasicNodeWeakPtr parent, model::ITimeEvaluatorPtr timeEval, const std::string & assetPath );

    virtual void                    Serialize           ( ISerializer & ser ) const override;
    static MeshLoaderPtr            Create              ( const IDeserializer & deser, model::BasicNodeWeakPtr parent );

    virtual void                    Initialize          ()              override {}
    virtual void                    Update              ( TimeType t )  override;
    virtual void                    Deinitialize        ()              override {}

    virtual const std::string &     GetType             () const override;
    static const std::string &      Type                ();

    virtual bool                    HandleEvent         ( IDeserializer & eventSer, ISerializer & response, BVProjectEditor * editor ) override;

    void                            Load                ( IDeserializer & eventSer, BVProjectEditor * editor );
    void                            Load                ( model::SceneModelPtr scene, BVProjectEditor * editor );

    bool                            MeshInfo            ( ISerializer & response );

    MeshAssetConstPtr               GetMeshAsset        () const;

    void                            EnableTextures      ( bool enabled );
    void                            EnableMaterials     ( bool enabled );

private:
    
    model::BasicNodePtr             Load                ( MeshAssetConstPtr asset, model::ITimeEvaluatorPtr timeEval );

    void                            GetMeshes           ( MeshAssetConstPtr asset, std::vector< MeshAssetConstPtr > & meshes ) const;

    void                            LoadTexture         ( const std::string & txPath, const std::string & pluginUID, const std::string & pluginName, model::BasicNodePtr node, model::ITimeEvaluatorPtr timeEval );
    std::string                     TranslatePath       ( const std::string & txPath );
};

} //nodelogic
} //bv
