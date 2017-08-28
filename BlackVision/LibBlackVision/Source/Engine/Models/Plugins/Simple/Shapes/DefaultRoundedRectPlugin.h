#pragma once

#include "Engine/Models/Plugins/Simple/DefaultGeometryPluginBase.h"
#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"





namespace bv { namespace model {


// ***********************
//
class DefaultRoundedRectPluginDesc : public DefaultGeometryPluginDescBase
{
public:
    DefaultRoundedRectPluginDesc();

    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;

    static  std::string                     UID                 ();
};



// ***********************
//
class DefaultRoundedRectPlugin : public DefaultGeometryPluginBase
{
public:
    static const std::string PN_SIZE;
    static const std::string PN_BEVELS;
    static const std::string PN_TESSELATION;

    struct PARAMS
    {
        static const std::string        SIZE;
        static const std::string        BEVELS;
        static const std::string        TESSELATION;
        static const std::string        STRETCH;
        static const std::string        USE_OUTLINE;
        static const std::string        OUTLINE_WIDTH;
        static const std::string        CROP;
        static const std::string        CROP_FACES;
    };

private:

	ValueParamState< glm::vec2 >		m_size;
	ValueParamState< glm::vec4 >		m_bevels;
	ValueParamState< Int32 >			m_tesselations;
    ValueParamState< glm::vec4 >        m_cropPlanes;
    ValueParamState< float >            m_stretch;
    ValueParamState< float >            m_outline;
    ValueParamState< bool >             m_useOutline;
    ValueParamState< bool >             m_crop;

    virtual std::vector<IGeometryGeneratorPtr>    GetGenerators() override;

    virtual bool                                NeedsTopologyUpdate();

    glm::vec2                                   GetSize();
    glm::vec4                                   GetBevels();
    int                                         GetTesselation();
public:
    DefaultRoundedRectPlugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model );
};

} }