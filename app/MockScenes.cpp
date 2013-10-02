#include "MockScenes.h"

#include "Engine/Models/BasicNode.h"
#include "Mathematics/Transform/MatTransform.h"
#include "Engine/Models/Plugins/Parameter.h"
#include "Engine/Models/ModelFactory.h"
#include "Engine/Models/Plugins/SimpleTexturePlugin.h"
#include "Engine/Models/Plugins/SimpleTextPlugin.h"
#include "Engine/Models/Plugins/Channels/Geometry/GeometryChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/Simple/RectComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/GeometryChannelDescriptor.h"
#include "Engine/Models/Plugins/Channels/Transform/SimpleTransformChannel.h"
#include "Engine/Models/Plugins/Channels/PixelShader/PixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/VertexShader/VertexShaderChannel.h"
#include "Engine/Models/Plugins/Channels/GeometryShader/GeometryShaderChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributeChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributeChannelDescriptor.h"
#include "Engine/Models/Plugins/GeometryPluginRect.h"

#define _USE_MATH_DEFINES
#include <math.h>
//#include <cmath>

// *********************************
// FIXME: move it to a valid BV windowed version of engine and wrap with a macro
namespace bv
{



class SimpleTexturePluginPD : public BaseParametersDescriptor
{
public:

    static const std::string            pluginName;

    static const std::string            alphaParamName;
    static const std::string            txMatrix0ParamName;
    static const std::string            txMatrix1ParamName;

    explicit SimpleTexturePluginPD();
};

// ***************************** DESCRIPTOR ********************************** 
//PLUGIN NAME
const std::string SimpleTexturePluginPD::pluginName( "SimpleTexturePlugin" );

//PLUGIN PARAMETERS
const std::string SimpleTexturePluginPD::alphaParamName( "alpha" );
const std::string SimpleTexturePluginPD::txMatrix0ParamName( "txMat0" );
const std::string SimpleTexturePluginPD::txMatrix1ParamName( "txMat1" );

class MyPixelShaderChannel : public model::ShaderChannel< model::IPixelShaderChannel, SimpleTexturePluginPD >
{
    model::ParamFloat *        m_alphaParam;
    model::ValueFloat *        m_alphaValue;

    model::ParamTransform *    m_tex0TransformParam;
    model::ValueMat4 *         m_tex0TransformValue;

    model::ParamTransform *    m_tex1TransformParam;
    model::ValueMat4 *         m_tex1TransformValue;

public:
    virtual void                    Update( float t )
    {
        ShaderChannel::Update( t );

        m_alphaValue->SetValue( m_alphaParam->Evaluate( t ) );
        m_tex0TransformValue->SetValue( m_tex0TransformParam->Evaluate( t ) );
        m_tex1TransformValue->SetValue( m_tex1TransformParam->Evaluate( t ) );
    }

    MyPixelShaderChannel( const std::string& shaderFile, const FloatInterpolator & alpha, const TransformF & tex0Transform, const TransformF & tex1Transform )
        : ShaderChannel( shaderFile )
    {
        m_alphaParam = new model::ParamFloat( ParamDesc::alphaParamName, alpha );
        m_alphaValue = new model::ValueFloat( ParamDesc::alphaParamName );

        m_tex0TransformParam = new model::ParamTransform( ParamDesc::txMatrix0ParamName, tex0Transform );
        m_tex0TransformValue = new model::ValueMat4( ParamDesc::txMatrix0ParamName );

        m_tex1TransformParam = new model::ParamTransform( ParamDesc::txMatrix1ParamName, tex1Transform );
        m_tex1TransformValue = new model::ValueMat4( ParamDesc::txMatrix1ParamName);

        RegisterValue( m_alphaValue );
        RegisterValue( m_tex0TransformValue );
        RegisterValue( m_tex1TransformValue );
    }
};




class TextPluginPD : public BaseParametersDescriptor
{
public:

    static const std::string            pluginName;

    explicit TextPluginPD()
        : BaseParametersDescriptor( pluginName )
    {}
};

const std::string TextPluginPD::pluginName = "SimpleTextPlugin";

class TextShaderChannel : public model::ShaderChannel< model::IPixelShaderChannel, TextPluginPD >
{
public:
    virtual void                    Update( float t )
    {
        ShaderChannel::Update( t );
    }

    TextShaderChannel( const std::string& shaderFile )
        : ShaderChannel( shaderFile )
    {
    }
};

class MyVertexShaderPD : public BaseParametersDescriptor
{
};

class MyVertexShaderChannel : public model::ShaderChannel< model::IVertexShaderChannel, MyVertexShaderPD >
{
public:
    MyVertexShaderChannel( const std::string& shaderFile )
        : ShaderChannel( shaderFile )
    {}
};

const std::string fontFile = "../dep/Media/fonts/arial.ttf";

// ***************************************
//
FloatInterpolator    TestParamFactory::ConstantValue( float val )
{
    FloatInterpolator ret;
    ret.addKey( 0.0f, val );

    return ret;
}

namespace {
    FloatInterpolator ConstValue( float val )
    {
        return TestParamFactory::ConstantValue( val );
    }
}

// ***************************************
//
model::BasicNode *     TestScenesFactory::SimpeTextTestScene()
{
    model::BasicNode * root = new model::BasicNode();

    auto texPlugin = new model::SimpleTextPlugin( L"Litwo! Ojczyzno moja! ty jesteœ jak zdrowie.\nIle ci trzeba ceniæ, ten tylko siê dowie,\nKto ciê straci³. Dziœ piêknoœæ tw¹ w ca³ej ozdobie\nWidzê i opisujê, bo têskniê po tobie."
                                                    , fontFile );

    //auto texPlugin = new model::SimpleTextPlugin( L"L....,,,,\nL...,,,", fontFile );
        

    FloatInterpolator xs; xs.setWrapPostMethod( bv::WrapMethod::pingPong );
    FloatInterpolator ys; ys.setWrapPostMethod( bv::WrapMethod::pingPong );
    FloatInterpolator zs;

    xs.addKey(0.f, 1.f);
    ys.addKey(0.f, 1.f);
    zs.addKey(0.f, 1.f);

    TransformF *    trns  = new TransformF                ();

    trns->addScale( xs, ys, zs );

    FloatInterpolator xt; xt.setWrapPostMethod( bv::WrapMethod::pingPong );
    FloatInterpolator yt; yt.setWrapPostMethod( bv::WrapMethod::repeat );
    FloatInterpolator zt;

    xt.addKey(0.f, -9.f);
    //yt.addKey(0.f, 0.f);
    yt.addKey(0.f, -10.f);
    zt.addKey(0.f, -5.f);

    yt.addKey(30.f, 75.f);

    trns->addTranslation( xt, yt, zt );

    model::SimpleTransformChannel      * stch  = new model::SimpleTransformChannel();
    stch->AddTransform( trns );

    texPlugin->SetTransformChannel( stch );
    texPlugin->SetPixelShaderChannel     ( new TextShaderChannel( "../dep/media/shaders/text.frag" ) );
    texPlugin->SetVertexShaderChannel    ( new MyVertexShaderChannel( "../dep/media/shaders/simpletexture.vert" ) );

    root->AddPlugin                 ( texPlugin );


    return root;


}

// ***************************************
//
model::BasicNode *     TestScenesFactory::SimpeTextureTestScene()
{
    //FIXME: worst of hacks
    static int numcall = 0;

    model::BasicNode * root = new model::BasicNode();

    TransformF *    trns  = new TransformF                ();

    FloatInterpolator angle; angle.setWrapPostMethod(bv::WrapMethod::pingPong);
    FloatInterpolator x;
    FloatInterpolator y;
    FloatInterpolator z;

    x.addKey(0.f, 0.f);
    y.addKey(0.f, 0.f);
    z.addKey(0.f, 1.f);

    if( numcall == 0 ) 
    {
        angle.addKey(0.f, 0.f);
        angle.addKey(5.f, 180.f);
    }
    else
    {
        angle.addKey(0.f, 300.f);
        angle.addKey(6.5f, 0.f);
    }

    trns->addRotation(angle, x, y ,z);


    TransformF * tx0m = new TransformF();
    TransformF * tx1m = new TransformF();

    FloatInterpolator alpha;

    if( numcall == 0 ) 
    {
        alpha.addKey( 0.0f, 0.25f ); alpha.setWrapPostMethod( WrapMethod::pingPong );
        alpha.addKey( 1.0f, 0.05f );
        alpha.addKey( 2.0f, 0.5f );
        alpha.addKey( 5.0f, 1.0f );
        alpha.addKey( 8.0f, 0.75f );
        alpha.addKey( 10.0f, 0.0f );
    }
    else
    {
        alpha.addKey( 0.0f, 0.25f ); alpha.setWrapPostMethod( WrapMethod::pingPong );
        alpha.addKey( 2.0f, 0.05f );
        alpha.addKey( 3.0f, 0.5f );
        alpha.addKey( 5.0f, 1.0f );
        alpha.addKey( 7.0f, 0.75f );
        alpha.addKey( 8.0f, 0.0f );
    }

    FloatInterpolator angTex0; angTex0.setWrapPostMethod( WrapMethod::pingPong );
    FloatInterpolator angTex1; angTex1.setWrapPostMethod( WrapMethod::pingPong );

    if( numcall == 0 ) 
    {
        angTex0.addKey(0.f, 0.f);
        angTex0.addKey(3.f, 180.f);
        angTex0.addKey(4.f, 30.f);

        angTex1.addKey(0.f, 180.f);
        angTex1.addKey(4.f, 0.f);
        angTex1.addKey(8.f, 120.f);
    }
    else
    {
        angTex0.addKey(0.f, 0.f);
        angTex0.addKey(2.f, 80.f);
        angTex0.addKey(5.f, -230.f);

        angTex1.addKey(0.f, 280.f);
        angTex1.addKey(7.f, 0.f);
        angTex1.addKey(12.f, 120.f);
    }

    tx0m->addRotation( angTex0, x, y, z ); //FIXME: memory lik
    tx1m->addRotation( angTex1, x, y, z ); //FIXME: memory lik

    FloatInterpolator xs; xs.setWrapPostMethod( bv::WrapMethod::pingPong );
    FloatInterpolator ys; ys.setWrapPostMethod( bv::WrapMethod::pingPong );
    FloatInterpolator zs;

    
    if( numcall == 0 )
    {
        xs.addKey(0.f, 0.5f);
        xs.addKey(13.f, 4.f);
        ys.addKey(0.f, 0.5f);
        ys.addKey(13.f, 4.f);
        zs.addKey(0.f, 1.f);
    }
    else
    {
        xs.addKey(0.f, 0.2f);
        xs.addKey(1.5f, 0.7f);
        ys.addKey(0.f, 0.2f);
        ys.addKey(1.5f, 0.67f);
        zs.addKey(0.f, 1.f);
    }

    trns->addScale(xs, ys ,zs);

    if( numcall == 1 )
    {
        FloatInterpolator xt; xt.addKey( 0.0f, 0.0f );
        FloatInterpolator yt; yt.addKey( 0.0f, 0.0f );
        FloatInterpolator zt; zt.addKey( 0.0f, 0.1f );

        trns->addTranslation( xt, yt, zt );
    }

    /////////////////////////////// SimpleRect plugin //////////////////////////////////

    FloatInterpolator w;
    FloatInterpolator h;
    w.addKey( 0.f, 1.f );
    h.addKey( 0.f, 1.f );

    model::GeometryRectPlugin    * rectPlugin  = new model::GeometryRectPlugin( w, h );
    
    model::RectComponent *      rect        = model::RectComponent::Create();

    model::GeometryChannelDescriptor desc;

    for( auto compDesc : rect->GetVertexAttributeChannels() )
    {
        desc.AddVertexAttrChannelDesc( static_cast< const model::VertexAttributeChannelDescriptor * >( compDesc->GetDescriptor() ) );
    }

    model::GeometryChannel *    geomCh      = new model::GeometryChannel( PrimitiveType::PT_TRIANGLE_STRIP, desc );
    geomCh->AddConnectedComponent( rect );
    rectPlugin->SetGeometryChannel( geomCh );


    model::SimpleTransformChannel      * stch  = new model::SimpleTransformChannel();
    stch->AddTransform( trns );
    rectPlugin->SetTransformChannel       ( stch );

    root->AddPlugin( rectPlugin );

    /////////////////////////////// Texture plugin //////////////////////////////////

    model::SimpleTexturePlugin    * stpp  = nullptr;
    
    if( numcall == 0 )
    {
        stpp = new model::SimpleTexturePlugin  (rectPlugin, "asci_arial_atlas_red.png", "simless_00.jpg" );
    }
    else
    {
        stpp = new model::SimpleTexturePlugin  (rectPlugin, "asci_arial_atlas_red.png", "simless_01.jpg" );
    }


    stpp->SetPixelShaderChannel     ( new MyPixelShaderChannel( "../dep/media/shaders/simpletexture.frag", alpha, *tx0m, *tx1m ) );
    stpp->SetVertexShaderChannel    ( new MyVertexShaderChannel( "../dep/media/shaders/simpletexture.vert" ) );

    root->AddPlugin                 ( stpp );

    numcall++;

    return root;
}

// *******************************
//
model::BasicNode *      TestScenesFactory::SimpleMultiCCScene      ()
{
    model::BasicNode * root = new model::BasicNode();

    /////////////////////////////// SimpleRect plugin //////////////////////////////////

    // ******************** Connected Components *******************************
    float w = 1.0f;
    float h = 1.0f;
    float dist = 0.75f;

    model::RectComponent *          rect0 = model::RectComponent::Create( w, h, -dist, -dist );
    model::RectComponent *          rect1 = model::RectComponent::Create( w, h, dist, -dist );
    model::RectComponent *          rect2 = model::RectComponent::Create( w, h, dist, dist );
    model::RectComponent *          rect3 = model::RectComponent::Create( w, h, -dist, dist );

    model::GeometryChannelDescriptor desc;

    for( auto compDesc : rect0->GetVertexAttributeChannels() )
    {
        desc.AddVertexAttrChannelDesc( static_cast< const model::VertexAttributeChannelDescriptor * >( compDesc->GetDescriptor() ) );
    }

    // ******************** Geometry Channel *******************************
    model::GeometryChannel *    geomCh      = new model::GeometryChannel( PrimitiveType::PT_TRIANGLE_STRIP, desc );
    geomCh->AddConnectedComponent( rect0 );
    geomCh->AddConnectedComponent( rect1 );
    geomCh->AddConnectedComponent( rect2 );
    geomCh->AddConnectedComponent( rect3 );

    for( unsigned int i = 1; i < 6; ++i )
    {
        float w = 0.2f;
        float h = 0.2f;
        float dist = 0.3f;

        model::RectComponent *  rect0 = model::RectComponent::Create( w, h, -dist * i, 0.f );
        model::RectComponent *  rect1 = model::RectComponent::Create( w, h, 0.f, -dist * i );
        model::RectComponent *  rect2 = model::RectComponent::Create( w, h, dist * i, 0.f );
        model::RectComponent *  rect3 = model::RectComponent::Create( w, h, 0.f, dist * i );

        geomCh->AddConnectedComponent( rect0 );
        geomCh->AddConnectedComponent( rect1 );
        geomCh->AddConnectedComponent( rect2 );
        geomCh->AddConnectedComponent( rect3 );        
    }

    // ******************** Plugin intself *******************************
    model::GeometryRectPlugin    *  rectPlugin  = new model::GeometryRectPlugin( ConstValue( 1.0f ), ConstValue( 1.0f ) );
    model::SimpleTransformChannel  * simpleTransform0  = new model::SimpleTransformChannel();

    TransformF * trns = new TransformF();

    FloatInterpolator angle; angle.setWrapPostMethod(bv::WrapMethod::pingPong);

    angle.addKey(0.f, 0.f);
    angle.addKey(5.f, 360.f);

    trns->addRotation(angle, ConstValue( 0.f ), ConstValue( 0.f ), ConstValue( 1.f ) );

    simpleTransform0->AddTransform  ( trns );

    rectPlugin->SetTransformChannel ( simpleTransform0 );
    rectPlugin->SetGeometryChannel( geomCh );

    root->AddPlugin( rectPlugin );

    /////////////////////////////// Texture plugin //////////////////////////////////

    model::SimpleTexturePlugin    * stpp  = nullptr;

    FloatInterpolator alpha;
    alpha.addKey( 0.0f, 0.25f ); alpha.setWrapPostMethod( WrapMethod::pingPong );
    alpha.addKey( 1.0f, 0.09f );
    alpha.addKey( 2.0f, 0.5f );
    alpha.addKey( 5.0f, 1.0f );
    alpha.addKey( 8.0f, 0.75f );
    alpha.addKey( 10.0f, 0.2f );

    TransformF * tx0m = new TransformF();
    tx0m->addScale( ConstValue( 1.0f ), alpha, ConstValue( 1.0f ) );

    TransformF * tx1m = new TransformF();    
    tx1m->addScale( alpha, ConstValue( 1.0f ), ConstValue( 1.0f ) );

    stpp = new model::SimpleTexturePlugin  ( rectPlugin, "simless_00.jpg", "simless_01.jpg" );

    stpp->SetPixelShaderChannel     ( new MyPixelShaderChannel( "../dep/media/shaders/simpletexture.frag", alpha, *tx0m, *tx1m ) );
    stpp->SetVertexShaderChannel    ( new MyVertexShaderChannel( "../dep/media/shaders/simpletexture.vert" ) );

    root->AddPlugin                 ( stpp );

    return root;
}

namespace {
    float AddRotKeys( bv::Vec3Interpolator * it, float startAngle, double endAngle, float speed, float & t )
    {
        float alpha = startAngle;
        for(; alpha <= (float) endAngle ; alpha += speed )
        {
            float z = 2.5f * cosf(alpha);
            float x = 2.5f * sinf(alpha);
            int dev = rand();
            it->addKey(t, glm::vec3(x, float(dev) / float(RAND_MAX), z));
            t += 0.1f;
        }

        it->addKey(t, glm::vec3(1.f, 1.f, 1.f));

        return alpha;
    }
}

// ***************************************
//    
void TestParametersFactory::CameraAnimation_ver1( Vec3Interpolator * direction, Vec3Interpolator * position, Vec3Interpolator * up )
{
    position->setWrapMethod(WrapMethod::repeat, WrapMethod::repeat);

    float t = 0.f;
    double endTime = 2. * M_PI;
    double dt = endTime / 10.f;

    float alpha = AddRotKeys( position, 0.f, dt, 0.05f, t );
    alpha = AddRotKeys( position, alpha, 3. * dt, 0.5f, t );
    alpha = AddRotKeys( position, alpha, 5. * dt, 0.05f, t );
    alpha = AddRotKeys( position, alpha, 7. * dt, 0.5f, t );
    alpha = AddRotKeys( position, alpha, 8. * dt, 0.01f, t );
    alpha = AddRotKeys( position, alpha, endTime, 0.2f, t );
    alpha = AddRotKeys( position, 0.f, 2. * M_PI, 0.05f, t );
 
    //float alpha = 0.f;
    //for(float alpha = 0.f; alpha <= 2 * M_PI ; alpha += 0.05f)
    //{
    //    float z = 2 * cosf(alpha);
    //    float x = 2 * sinf(alpha);
    //    int dev = rand();
    //    position->addKey(t, glm::vec3(x, float(dev) / float(RAND_MAX), z));
    //    t += 0.1f;
    //}

    direction->addKey(0.f, glm::vec3(0.f, 0.f, 0.f));
    up->addKey(0.f, glm::vec3(0.f, 1.f, 0.f));

    //m_modelScene->AddCameraInterpolators(direction, position, up);
}

// ***************************************
//    
void TestParametersFactory::CameraAnimation_ver2( Vec3Interpolator * direction, Vec3Interpolator * position, Vec3Interpolator * up )
{
    position->setWrapMethod(WrapMethod::repeat, WrapMethod::pingPong);

    float t = 0.f;
    double endTime = 2. * M_PI;
    double dt = endTime / 10.f;
    position->addKey(0.f, glm::vec3(-2.f, 0.1f, 3.f));
    position->addKey(20.f, glm::vec3(2.f, 0.1f, 3.f));
    

    //float alpha = 0.f;
    //for(float alpha = 0.f; alpha <= 2 * M_PI ; alpha += 0.05f)
    //{
    //    float z = 2 * cosf(alpha);
    //    float x = 2 * sinf(alpha);
    //    int dev = rand();
    //    position->addKey(t, glm::vec3(x, float(dev) / float(RAND_MAX), z));
    //    t += 0.1f;
    //}

    direction->addKey(0.f, glm::vec3(0.f, 0.f, 0.f));

    up->addKey(0.f, glm::vec3(0.f, 1.f, 0.f));
    up->addKey(3.f, glm::vec3(0.f, 1.f, 0.f));

    //m_modelScene->AddCameraInterpolators(direction, position, up);
}

void RenderMockScene(bv::model::BasicNode* tree, std::ostream& out)
{
    //static float t = 0.f;

    //tree->Update(t);

    //out << "---------------------------------TIME : " << t << " ---------------------------";

    //out << std::endl;

    //tree->Print(out);

    //out << "----------END TREE------------" << std::endl << std::endl;

    //t += 0.1f;
}

model::BasicNode* SceneExamples::BuildMockScene(bv::model::BasicNode * parent )
{
    bv::model::BasicNode* mockTree = new bv::model::BasicNode();

    // SOLID
    bv::Vec4Interpolator color;

    float rs = 10.f;

    color.addKey(rs * .25f, glm::vec4(0,0,1,0.7f));
    //color.addKey(rs *.5f, glm::vec4(0.3,0.4,0.5,1));
    //color.addKey(rs *.75f, glm::vec4(0.1,1.0,0.8,0));
    //color.addKey(rs *.95f, glm::vec4(0.7,0.2,0.4,1));
    //color.addKey(rs *1.15f, glm::vec4(0.4,0.1,1.,0));
    //color.addKey(rs *1.33f, glm::vec4(1,1.0,0.1,1));
    //color.addKey(rs *1.6f, glm::vec4(1,0.4,0.2,0));
    //color.addKey(rs *1.9f, glm::vec4(0,0.4,1.0,1));

    color.setWrapMethod(bv::WrapMethod::clamp, bv::WrapMethod::pingPong);

//    bv::SolidColorPlugin* solid1 = new bv::SolidColorPlugin(color);

  //  mockTree->setPixelShaderPlugin(solid1);

    //bv::FloatInterpolator si;

    //si.setWrapMethod(bv::WrapMethod::repeat, bv::WrapMethod::pingPong);
    //float s = 5.0f;
    ////si.addKey(0.f, 0.5f);si.addKey(0.4f, 0.5f);si.addKey(0.5f, s * 0.6f);si.addKey(0.65f, 0.4f);si.addKey(0.8f, s * 0.9f);si.addKey(0.95f, 0.35f);si.addKey(1.05f, 0.5f);
    //si.addKey(0.f, 0.1f);
    //si.addKey(2.4f, 2.5f);//si.addKey(0.5f, s * 0.6f);si.addKey(0.65f, 0.4f);si.addKey(0.8f, s * 0.9f);si.addKey(0.95f, 0.35f);si.addKey(1.05f, 0.5f);
    
    //mockTree->setGeometryShaderPlugin(new bv::ExtrudePlugin(si));

//    mockTree->addGeometryPlugin(new bv::PluginGeometryRect(4.f, 4.f));

    // TRANSFORMATION
    bv::FloatInterpolator angle; angle.setWrapPostMethod(bv::WrapMethod::pingPong);
    bv::FloatInterpolator x;
    bv::FloatInterpolator y;
    bv::FloatInterpolator z;

    x.addKey(0.f, 0.f);
    y.addKey(0.f, 0.f);
    z.addKey(0.f, 1.f);
    angle.addKey(0.f, 0.f);
    angle.addKey(5.f, 180.f);

    bv::TransformF* trans = new bv::TransformF();
    trans->addTransform(new bv::RotationF(angle, x,y,z));
//    bv::PluginTransformSimple* transPlugin = bv::ModelFactory::CreatePluginTransformSimple(*trans);

    //FIXME:
    //mockTree->addTransformPlugin(transPlugin);

    if(parent != nullptr)
    {
        parent->AddChild(mockTree);
    }

    return mockTree;
}

model::BasicNode* SceneExamples::BuildMockScene2(bv::model::BasicNode* parent)
{
    bv::model::BasicNode* mockTree = new bv::model::BasicNode();

    // SOLID
    bv::Vec4Interpolator color;

    color.addKey(.0f, glm::vec4(1,0,0,0.5f));
    //color.addKey(3.f, glm::vec4(0.3,0.4,0.5,1.f));
    //color.addKey(1.f, glm::vec4(0.1,0.2,0.8,1.f));

    //color.addKey(2.f, glm::vec4(1,0.4,0.5,1.f));

    color.setWrapMethod(bv::WrapMethod::clamp, bv::WrapMethod::pingPong);

//    bv::SolidColorPlugin* solid1 = new bv::SolidColorPlugin(color);

//    mockTree->setPixelShaderPlugin(solid1);

    //bv::FloatInterpolator scaleInt;

    //scaleInt.setWrapMethod(bv::WrapMethod::pingPong, bv::WrapMethod::pingPong);

    //scaleInt.addKey(0.f, -3.5f);
    //scaleInt.addKey(-1.30f, 3.5f);

    //mockTree->setGeometryShaderPlugin(new bv::ExtrudePlugin(scaleInt));

//    mockTree->addGeometryPlugin(new bv::PluginGeometryRect());

    // TRANSFORMATION
    bv::FloatInterpolator x;
    bv::FloatInterpolator y;
    bv::FloatInterpolator z;

    float s = 2.0f;
    float l = 1.4f;
    x.addKey(0.f, 0.5f);x.addKey(0.4f, 0.5f);x.addKey(0.5f, s * 0.6f);x.addKey(0.65f, 0.4f);x.addKey(0.8f, s * 0.9f);x.addKey(0.95f, 0.35f);x.addKey(1.05f, 0.5f);
    y.addKey(0.f, 0.5f);y.addKey(0.4f, 0.5f);y.addKey(0.5f, l * 0.6f);y.addKey(0.65f, 0.4f);y.addKey(0.8f, l * 0.9f);y.addKey(0.95f, 0.35f);y.addKey(1.05f, 0.5f);
    z.addKey(0.f, 1.f);

    x.setWrapPostMethod(bv::WrapMethod::repeat);
    y.setWrapPostMethod(bv::WrapMethod::repeat);
    
    bv::TransformF* trans = new bv::TransformF();

    bv::FloatInterpolator mx;
    bv::FloatInterpolator my;
    bv::FloatInterpolator mz;

    mx.addKey(0.f, 0.0f);
    my.addKey(0.f, 0.0f);
    mz.addKey(0.f, 0.0f);

    mx.addKey(1.f, 1.0f);
    my.addKey(2.f, -2.0f);
    mz.addKey(3.f, 0.f);

    trans->addTranslation(mx, my, mz);
    trans->addScale(x,y,z);

//    bv::PluginTransformSimple* transPlugin = bv::ModelFactory::CreatePluginTransformSimple(*trans);

    //FIXME:
    //mockTree->addTransformPlugin(transPlugin);

    if(parent != nullptr)
    {
        parent->AddChild(mockTree);
    }

    return mockTree;
}


//bv::PluginTransformSimple* FuncTranslate(float x,float y,float z)
//{
//    bv::TransformF* trans = new bv::TransformF();
//    bv::FloatInterpolator transX;
//    transX.addKey(0.f,x);
//    bv::FloatInterpolator transY;
//    transY.addKey(0.f,y);
//    bv::FloatInterpolator transZ;
//    transZ.addKey(0.f,z);
//    trans->addTranslation(transX,transY,transZ);
//    bv::PluginTransformSimple* transPlugin = bv::ModelFactory::CreatePluginTransformSimple(*trans);
//    return transPlugin;
//}



//void AnimLines(bv::BasicNode* parent)
//{
//    bv::BasicNode* lineUp = new bv::BasicNode();
//    bv::Vec4Interpolator color;
//    
//    color.addKey(0.01f, glm::vec4(0,0,1,1));
//    color.setWrapMethod(bv::WrapMethod::clamp, bv::WrapMethod::pingPong);
//    bv::SolidColorPlugin* solidRect = new bv::SolidColorPlugin(color);
//    lineUp->setPixelShaderPlugin(solidRect);
////	lineUp->addGeometryPlugin(new bv::PluginGeometryRect(0.1f,0.1f));
//    
//    //bez sensu
//    bv::FloatInterpolator si;
//    si.setWrapMethod(bv::WrapMethod::repeat, bv::WrapMethod::pingPong);
//    float s = 5.0f;
//    si.addKey(0.f, 0.1f);
//    lineUp->setGeometryShaderPlugin(new bv::ExtrudePlugin(si));
//
//
//    bv::FloatInterpolator identity; 
//    identity.addKey(0.f, 1.f);
//
//
//    bv::FloatInterpolator scaleX; 
//    scaleX.setWrapPostMethod(bv::WrapMethod::clamp);
//    scaleX.addKey(0.f, 1.f);
//    scaleX.addKey(2.f, 10.0f);
//    
//    bv::TransformF* trans = new bv::TransformF();
//    trans->addScale(scaleX,identity,identity);
//    bv::PluginTransformSimple* transPlugin = bv::ModelFactory::CreatePluginTransformSimple(*trans);
//
//    
//
//    //FIXME:
//    //lineUp->addTransformPlugin(transPlugin);
//
//    //FIXME:
//    //lineUp->addTransformPlugin(FuncTranslate(0,-1,0));
//
//
//
//
//    if(parent != nullptr)
//    {
//        parent->AddChild(lineUp);
//    }
//
//}


//bv::BasicNode* CreateRect(bv::BasicNode* parent)
//{
//    bv::BasicNode* rectNode = new bv::BasicNode();
//    bv::Vec4Interpolator color;
//    color.addKey(0, glm::vec4(1,0,0,1));
//    color.setWrapMethod(bv::WrapMethod::clamp, bv::WrapMethod::pingPong);
//    bv::SolidColorPlugin* solidRect = new bv::SolidColorPlugin(color);
//    rectNode->setPixelShaderPlugin(solidRect);
//
//    //extrude
//    bv::FloatInterpolator si;
//    si.setWrapMethod(bv::WrapMethod::repeat, bv::WrapMethod::pingPong);
//    float s = 5.0f;
//    si.addKey(0.f, 0.1f);
//    si.addKey(1.f, -0.1f);
//    //si.addKey(2.4f, 2.5f);//si.addKey(0.5f, s * 0.6f);si.addKey(0.65f, 0.4f);si.addKey(0.8f, s * 0.9f);si.addKey(0.95f, 0.35f);si.addKey(1.05f, 0.5f);
//    rectNode->setGeometryShaderPlugin(new bv::ExtrudePlugin(si));
//   
//    //FIXME: not enaf paramiters
//    assert( false ); //FIXME: tu jest assert false jak by co !!!!!
//    //rectNode->AddPlugin( new SimpleTexturePixelPlugin( "pliczek_z_kwiatkiem.jpg" ) );
////	rectNode->addGeometryPlugin(new bv::PluginGeometryRect(0.2f,0.1f));
//
//    // b edzie sie rozszerzal
//
//    bv::FloatInterpolator identity; 
//    identity.addKey(0.f, 1.f);
//
//    bv::FloatInterpolator scaleX; scaleX.setWrapPostMethod(bv::WrapMethod::pingPong);
//    scaleX.addKey(0.f, 1.f);
//    scaleX.addKey(5.f, 1.2f);
//    
//    bv::TransformF* trans = new bv::TransformF();
//    trans->addScale(scaleX,identity,identity);
//    bv::PluginTransformSimple* transPlugin = bv::ModelFactory::CreatePluginTransformSimple(*trans);
//
//    //FIXME:
//    //rectNode->addTransformPlugin(transPlugin);
//    if(parent != nullptr)
//    {
//        parent->AddChild(rectNode);
//    }
//    return rectNode;
//}

//void createMyRing(bv::BasicNode* parent,float offsetZ,float offsetX,float offsetY,float startAngle,float endAngle)
//{
//    bv::BasicNode* mockTree = new bv::BasicNode();
//
//    
//
//
//    // SOLID
//    bv::Vec4Interpolator color;
//
//    float rs = 10.f;
//
//    color.addKey(2, glm::vec4(0.1,0,0,1));
//    color.addKey(5, glm::vec4(1,0,0,1));
// 
//    color.setWrapMethod(bv::WrapMethod::clamp, bv::WrapMethod::pingPong);
//
//    bv::SolidColorPlugin* solid1 = new bv::SolidColorPlugin(color);
//
//    mockTree->setPixelShaderPlugin(solid1);
//
//    bv::FloatInterpolator si;
//
//    si.setWrapMethod(bv::WrapMethod::repeat, bv::WrapMethod::pingPong);
//    float s = 5.0f;
//    si.addKey(0.f, 0.1f);
//    si.addKey(0.2f, 1.2f);
//    //si.addKey(2.4f, 2.5f);//si.addKey(0.5f, s * 0.6f);si.addKey(0.65f, 0.4f);si.addKey(0.8f, s * 0.9f);si.addKey(0.95f, 0.35f);si.addKey(1.05f, 0.5f);
//
//    mockTree->setGeometryShaderPlugin(new bv::ExtrudePlugin(si));
//
//    FloatInterpolator radIn;
//    FloatInterpolator radOut;
//    FloatInterpolator start;
//    FloatInterpolator end;
//
//    radIn.addKey(0.f, 1.7f);
//    radOut.addKey(0.f, 2.f);
//    start.addKey(0.f, startAngle);
//    end.addKey(0.f, endAngle);
////	   mockTree->addGeometryPlugin(new bv::PluginGeometryRing(radIn, radOut, start, end, 3));
//    //mockTree->addGeometryPlugin(new bv::PluginGeometryRect());
//
//    // TRANSFORMATION
//    bv::FloatInterpolator angle; angle.setWrapPostMethod(bv::WrapMethod::pingPong);
//    bv::FloatInterpolator x;
//    bv::FloatInterpolator y;
//    bv::FloatInterpolator z;
//
//    x.addKey(0.f, 0.f);
//    y.addKey(0.f, 0.f);
//    z.addKey(0.f, -1.1f);
//    angle.addKey(0.f, 0.f);
//    //angle.addKey(5.f, 180.f);
//
//    bv::TransformF* trans = new bv::TransformF();
//    
//    bv::FloatInterpolator transX;
//    transX.addKey(0.f,offsetX);
//
//    bv::FloatInterpolator transY;
//    transY.addKey(0.f,offsetY);
//    
//    bv::FloatInterpolator transZ;
//    transZ.addKey(0.f,offsetZ);
//
//    trans->addTranslation(transX,transY,transZ);
//
//    
//
//    trans->addTransform(new bv::RotationF(angle, x,y,z));
//    bv::PluginTransformSimple* transPlugin = bv::ModelFactory::CreatePluginTransformSimple(*trans);
//
//    //FIXME:
//    //mockTree->addTransformPlugin(transPlugin);
//
//
//    
//
//    if(parent != nullptr)
//    {
//        parent->AddChild(mockTree);
//    }
//}
//void NightRect(bv::BasicNode* parent, int number,float offsetX,float offsetY)
//{
//    float RectWidth=0.04f;
//    bv::BasicNode* rectNode = new bv::BasicNode();
//    bv::Vec4Interpolator color;
//    float atom=0.1f;
//    color.addKey(number*atom, glm::vec4(0,0,0,0));
//    color.addKey(number*atom+2, glm::vec4(0,0,0,0));
//    color.addKey(number*atom+3, glm::vec4(1,0,0,1));
//    color.addKey(number*atom+4, glm::vec4(0,0,0,0));
//    color.addKey(number*atom+6, glm::vec4(0,0,0,0));
//    color.setWrapMethod(bv::WrapMethod::clamp, bv::WrapMethod::repeat);
//    bv::SolidColorPlugin* solidRect = new bv::SolidColorPlugin(color);
//    rectNode->setPixelShaderPlugin(solidRect);
//
//    //extrude
//    bv::FloatInterpolator si;
//    si.setWrapMethod(bv::WrapMethod::repeat, bv::WrapMethod::pingPong);
//    float s = 5.0f;
//    si.addKey(0.f, 0.1f);
//    //si.addKey(1.f, 0.3f);
//    //si.addKey(2.4f, 2.5f);//si.addKey(0.5f, s * 0.6f);si.addKey(0.65f, 0.4f);si.addKey(0.8f, s * 0.9f);si.addKey(0.95f, 0.35f);si.addKey(1.05f, 0.5f);
//    rectNode->setGeometryShaderPlugin(new bv::ExtrudePlugin(si));
//    
//    //rectNode->addGeometryPlugin(new bv::PluginGeometryRect(RectWidth,0.1f));
//
//    // b edzie sie rozszerzal
//
//    bv::FloatInterpolator identity; 
//    identity.addKey(0.f, 1.f);
//
//    bv::FloatInterpolator scaleX; scaleX.setWrapPostMethod(bv::WrapMethod::pingPong);
//    scaleX.addKey(0.f, 1.f);
//    scaleX.addKey(5.f, 1.2f);
//    
//    bv::TransformF* trans = new bv::TransformF();
//    trans->addScale(scaleX,identity,identity);
//    bv::PluginTransformSimple* transPlugin = bv::ModelFactory::CreatePluginTransformSimple(*trans);
//
//    //FIXME:
//    //rectNode->addTransformPlugin(transPlugin);
//    //rectNode->addTransformPlugin(FuncTranslate(offsetX,offsetY,-4+number*RectWidth*6));
//    if(parent != nullptr)
//    {
//        parent->AddChild(rectNode);
//    }
//    
//}
//void NightRider(bv::BasicNode* parent)
//{
//    bv::BasicNode* NightRects= new bv::BasicNode();
//
//    for(int i=0;i<50;i++)
//    {
//        NightRect(parent,i,-2,0);
//        NightRect(parent,i,2,0);
//        NightRect(parent,i,0,-2);
//        NightRect(parent,i,0,2);
//        
//
//    }
//    /*if(parent != nullptr)
//    {
//        parent->addChild(NightRects);
//    }*/
//}
//
//void CreateRings(bv::BasicNode* parent,float z)
//{
//    float Angle1=3.1415f;
//    float Angle2=3.1415f/2.0;
//    float Angle3=0;
//    float Angle4=-3.1415f/2;
//    float offset=-3.1415f/2;
//    createMyRing(parent,z,-1,0.5,Angle1,Angle1+offset);	
//    createMyRing(parent,z,1,0.5,Angle2,Angle2+offset);
//    
//    createMyRing(parent,z,1,-0.5,Angle3, Angle3+offset);	
//    createMyRing(parent,z,-1,-0.5,Angle4,Angle4+offset);
//    
//}
//
//bv::BasicNode* SceneExamples::CreateSceneMock3(bv::BasicNode* parent)
//{
//    bv::BasicNode* rect = CreateRect(parent);
//
//    
//
//    CreateRings(rect,0);
//    CreateRings(rect,-3);
//    CreateRings(rect,-6);
//    CreateRings(rect,-9);
//    CreateRings(rect,-12);
//    CreateRings(rect,-15);
//    CreateRings(rect,-20);
//    CreateRings(rect,-30);
//    CreateRings(rect,-50);
//    CreateRings(rect,-70);
//
//    AnimLines(rect);
//    NightRider(rect);
//
//    return rect;
//
//
//}

}

