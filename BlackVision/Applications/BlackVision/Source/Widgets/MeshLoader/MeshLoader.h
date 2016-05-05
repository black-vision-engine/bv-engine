#pragma once
#include "CoreDEF.h"
#include "Engine/Models/Interfaces/INodeLogic.h"
#include "Engine/Types/Values/TypedValues.h"
#include "Engine/Models/Plugins/Parameters/CachedSimpleTypedParameters.h"
#include "Engine/Models/Interfaces/ITimeEvaluator.h"
#include "Engine/Models/BasicNode.h"

#include "Assets/Assets.h"


namespace bv { namespace nodelogic {

class MeshLoader;

DEFINE_PTR_TYPE( MeshLoader )
DEFINE_CONST_PTR_TYPE( MeshLoader )


class MeshLoader:  public model::INodeLogic, public std::enable_shared_from_this< MeshLoader >
{
private:

    static const std::string        m_type;

private:

    MeshAssetDescConstPtr           m_assetDesc;
    MeshAssetConstPtr               m_asset;

    bool                            m_textureEnabled;
    bool                            m_materialEnabled;

	model::BasicNodePtr				m_parentNode;
	bool							m_isFinalized;
	model::ParamFloatPtr			m_param;
	ValueFloatPtr					m_value;

public:

	explicit                        MeshLoader    ( model::BasicNodePtr parent, const std::string & assetPath );
	                                ~MeshLoader   ();


	virtual void	                Initialize		    ()				override {}
	virtual void	                Update			    ( TimeType t )	override;
	virtual void	                Deinitialize	    ()				override {}

    virtual const std::string &     GetType             () const override;
    static const std::string &      Type                ();

	static MeshLoaderPtr            Create              ( model::BasicNodePtr parent, const std::string & assetPath );
	bv::model::IParameterPtr        GetValueParam       ();

    virtual model::IParameterPtr                        GetParameter        ( const std::string & name ) const override;
    virtual const std::vector< model::IParameterPtr > & GetParameters       () const override;


    virtual void                    Serialize           ( ISerializer & ser ) const override;
    static MeshLoaderPtr            Create              ( const IDeserializer & deser, model::BasicNodePtr parent );

    virtual bool                    HandleEvent         ( IDeserializer & eventSer, ISerializer & response, BVProjectEditor * editor  ) override;

    void                            Load                ( MeshAssetConstPtr asset, model::BasicNodePtr parent, model::ITimeEvaluatorPtr timeEval );
    void                            Load                ();

};

} //nodelogic
} //bv
