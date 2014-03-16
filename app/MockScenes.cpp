#include "MockScenes.h"

#include "Engine/Models/BasicNode.h"
#include "Engine/Models/Plugins/PluginsFactory.h"
#include "Engine/Models/Plugins/Simple/SimpleTransformPlugin.h"
#include "Engine/Models/Plugins/GeometryMultiRectPlugin.h"
#include "Engine/Types/Enums.h"


#define _USE_MATH_DEFINES
#include <math.h>
//#include <cmath>


// *********************************
// FIXME: move it to a valid BV windowed version of engine and wrap with a macro
namespace bv
{

class TexturePixelShaderChannelPD
{
public:

    static const std::string            alphaParamName;
    static const std::string            txMatrix0ParamName;
    static const std::string            txMatrix1ParamName;

};

// ***************************** DESCRIPTOR ********************************** 

//PLUGIN PARAMETERS
const std::string TexturePixelShaderChannelPD::alphaParamName( "alpha" );
const std::string TexturePixelShaderChannelPD::txMatrix0ParamName( "txMat0" );
const std::string TexturePixelShaderChannelPD::txMatrix1ParamName( "txMat1" );

class MyPixelShaderChannel : public model::DefaultPixelShaderChannel
{
private:

    model::ParamFloat       m_alphaParam;
    ValueFloatPtr           m_alphaValue;

    model::ParamTransform   m_tex0TransformParam;
    ValueMat4Ptr            m_tex0TransformValue;

    model::ParamTransform   m_tex1TransformParam;
    ValueMat4Ptr            m_tex1TransformValue;

public:

    virtual void                    Update( float t )
    {
        //ShaderChannel::Update( t );

        //m_alphaValue->SetValue( m_alphaParam.Evaluate( t ) );
        //m_tex0TransformValue->SetValue( m_tex0TransformParam.Evaluate( t ) );
        //m_tex1TransformValue->SetValue( m_tex1TransformParam.Evaluate( t ) );
    }

    MyPixelShaderChannel( const std::string & shaderFile, const FloatInterpolator & alpha, const TransformF & tex0Transform, const TransformF & tex1Transform )
        : DefaultPixelShaderChannel( shaderFile, nullptr )
        , m_alphaParam( TexturePixelShaderChannelPD::alphaParamName, alpha )
        , m_tex0TransformParam( TexturePixelShaderChannelPD::txMatrix0ParamName, tex0Transform )
        , m_tex1TransformParam( TexturePixelShaderChannelPD::txMatrix1ParamName, tex1Transform )
    {
        //m_alphaValue            = model::ValueFloatPtr( new model::ValueFloat( ParamDesc::alphaParamName ) );
        //m_tex0TransformValue    = model::ValueMat4Ptr( new model::ValueMat4( ParamDesc::txMatrix0ParamName ) );
        //m_tex1TransformValue    = model::ValueMat4Ptr( new model::ValueMat4( ParamDesc::txMatrix1ParamName ) );

        //RegisterValue( m_alphaValue.get() );
        //RegisterValue( m_tex0TransformValue.get() );
        //RegisterValue( m_tex1TransformValue.get() );
    }
};


class MyVertexShaderChannel : public model::ShaderChannel< model::IVertexShaderChannel >
{
public:

    MyVertexShaderChannel( const std::string & shaderFile )
        : ShaderChannel( shaderFile, nullptr )
    {}

};

const std::string fontFile = "../dep/Media/fonts/ARIALUNI.TTF";

// ***************************************
//
FloatInterpolator    TestParamFactory::ConstantValue( float val )
{
    FloatInterpolator ret;
    ret.AddKey( 0.0f, val );

    return ret;
}

namespace
{
    FloatInterpolator ConstValue( float val )
    {
        return TestParamFactory::ConstantValue( val );
    }
}

// ***************************************
//
model::BasicNode *     TestScenesFactory::SimpeTextureTestScene()
{
    //FIXME: worst of hacks
    static int numcall = 0;

    model::BasicNode * root = new model::BasicNode( "SimpleTextureRoot" );

    TransformF     trns;

    FloatInterpolator angle; angle.SetWrapPostMethod( bv::WrapMethod::pingPong );
    FloatInterpolator x;
    FloatInterpolator y;
    FloatInterpolator z;

    x.AddKey( 0.f, 0.f );
    y.AddKey( 0.f, 0.f );
    z.AddKey( 0.f, 1.f );

    if( numcall == 0 ) 
    {
        angle.AddKey(0.f, 0.f);
        angle.AddKey(5.f, 180.f);
    }
    else
    {
        angle.AddKey(0.f, 300.f);
        angle.AddKey(6.5f, 0.f);
    }

    trns.AddRotation(angle, x, y ,z);


    TransformF tx0m;
    TransformF tx1m;

    FloatInterpolator alpha;

    if( numcall == 0 ) 
    {
        alpha.AddKey( 0.0f, 0.25f ); alpha.SetWrapPostMethod( WrapMethod::pingPong );
        alpha.AddKey( 1.0f, 0.05f );
        alpha.AddKey( 2.0f, 0.5f );
        alpha.AddKey( 5.0f, 1.0f );
        alpha.AddKey( 8.0f, 0.75f );
        alpha.AddKey( 10.0f, 0.0f );
    }
    else
    {
        alpha.AddKey( 0.0f, 0.25f ); alpha.SetWrapPostMethod( WrapMethod::pingPong );
        alpha.AddKey( 2.0f, 0.05f );
        alpha.AddKey( 3.0f, 0.5f );
        alpha.AddKey( 5.0f, 1.0f );
        alpha.AddKey( 7.0f, 0.75f );
        alpha.AddKey( 8.0f, 0.0f );
    }

    FloatInterpolator angTex0; angTex0.SetWrapPostMethod( WrapMethod::pingPong );
    FloatInterpolator angTex1; angTex1.SetWrapPostMethod( WrapMethod::pingPong );

    if( numcall == 0 ) 
    {
        angTex0.AddKey(0.f, 0.f);
        angTex0.AddKey(3.f, 180.f);
        angTex0.AddKey(4.f, 30.f);

        angTex1.AddKey(0.f, 180.f);
        angTex1.AddKey(4.f, 0.f);
        angTex1.AddKey(8.f, 120.f);
    }
    else
    {
        angTex0.AddKey(0.f, 0.f);
        angTex0.AddKey(2.f, 80.f);
        angTex0.AddKey(5.f, -230.f);

        angTex1.AddKey(0.f, 280.f);
        angTex1.AddKey(7.f, 0.f);
        angTex1.AddKey(12.f, 120.f);
    }

    tx0m.AddRotation( angTex0, x, y, z ); //FIXME: memory lik
    tx1m.AddRotation( angTex1, x, y, z ); //FIXME: memory lik

    FloatInterpolator xs; xs.SetWrapPostMethod( bv::WrapMethod::pingPong );
    FloatInterpolator ys; ys.SetWrapPostMethod( bv::WrapMethod::pingPong );
    FloatInterpolator zs;

    
    if( numcall == 0 )
    {
        xs.AddKey(0.f, 0.5f);
        xs.AddKey(13.f, 4.f);
        ys.AddKey(0.f, 0.5f);
        ys.AddKey(13.f, 4.f);
        zs.AddKey(0.f, 1.f);
    }
    else
    {
        xs.AddKey(0.f, 0.2f);
        xs.AddKey(1.5f, 0.7f);
        ys.AddKey(0.f, 0.2f);
        ys.AddKey(1.5f, 0.67f);
        zs.AddKey(0.f, 1.f);
    }

    trns.AddScale(xs, ys ,zs);

    if( numcall == 1 )
    {
        FloatInterpolator xt; xt.AddKey( 0.0f, 0.0f );
        FloatInterpolator yt; yt.AddKey( 0.0f, 0.0f );
        FloatInterpolator zt; zt.AddKey( 0.0f, 0.1f );

        trns.AddTranslation( xt, yt, zt );
    }

    /////////////////////////////// SimpleRect plugin //////////////////////////////////

    auto w = model::ParametersFactory::CreateParameter( "width", InterpolatorsHelper::CreateConstValue( 1.f ) );
    auto h = model::ParametersFactory::CreateParameter( "height", InterpolatorsHelper::CreateConstValue( 1.f )  );

    model::GeometryRectPlugin    * rectPlugin  = new model::GeometryRectPlugin( w, h );
    
    root->AddPlugin( rectPlugin );

    model::SimpleTransformPlugin      * stpl  = model::PluginsFactory::CreateSimpleTransformPlugin( rectPlugin, model::ParametersFactory::CreateParameter( "transformation", trns, nullptr, 0 ) );

    root->AddPlugin( stpl );

    /////////////////////////////// Texture plugin //////////////////////////////////

    model::SimpleTexturePlugin    * stpp  = nullptr;
    
    std::vector< const model::TextureDescriptor > textures;
        
    textures.push_back( model::TextureDescriptor( "asci_arial_atlas_red.png", model::ParametersFactory::CreateParameter( "texTransform", tx0m ) ) );
    textures.push_back( model::TextureDescriptor( "pliczek_z_kwiatkiem.jpg", model::ParametersFactory::CreateParameter( "texTransform", tx1m ) ) );
    textures[0].alpha = model::ParametersFactory::CreateParameter( "texAlpha", InterpolatorsHelper::CreateConstValue( 0.4f) );
    textures[1].wrappingModeX = TextureWrappingMode::TWM_CLAMP_BORDER;
    textures[1].wrappingModeY = TextureWrappingMode::TWM_CLAMP_EDGE;
    textures[1].filteringMode = TextureFilteringMode::TFM_LINEAR;
    textures[1].alpha = model::ParametersFactory::CreateParameter( "texAlpha", InterpolatorsHelper::CreateConstValue( 1.f) );
    textures[1].borderColor = model::ParametersFactory::CreateParameter( "texBorderColor", InterpolatorsHelper::CreateConstValue( glm::vec4( 0.f, 1.f, 0.f, 0.3f ) ) );

    stpp = model::PluginsFactory::CreateSimpleTexturePlugin( stpl, textures );

    root->AddPlugin                 ( stpp );

    numcall++;

    return root;
}

// *******************************
//
model::BasicNode *      TestScenesFactory::SimpleMultiCCScene      ()
{
    model::BasicNode * root = new model::BasicNode( "SimpleMultiCCSceneRoot" );

    /////////////////////////////// SimpleRect plugin //////////////////////////////////

    // ******************** Connected Components *******************************
    float w = 1.0f;
    float h = 1.0f;
    float dist = 0.75f;

    // ******************** Plugin intself *******************************
    model::GeometryMultiRectPlugin    *  rectPlugin  = model::PluginsFactory::CreateGeometryMultiRectPlugin();
    rectPlugin->AddRectConnectedComponnent( 1.f, 1.f );
    rectPlugin->AddRectConnectedComponnent( w, h, -dist, -dist );
    rectPlugin->AddRectConnectedComponnent( w, h, dist, -dist );
    rectPlugin->AddRectConnectedComponnent( w, h, dist, dist );
    rectPlugin->AddRectConnectedComponnent( w, h, -dist, dist );

    for( unsigned int i = 1; i < 6; ++i )
    {
        float w = 0.2f;
        float h = 0.2f;
        float dist = 0.3f;

        rectPlugin->AddRectConnectedComponnent( w, h, -dist * i, 0.f );
        rectPlugin->AddRectConnectedComponnent( w, h, 0.f, -dist * i );
        rectPlugin->AddRectConnectedComponnent( w, h, dist * i, 0.f );
        rectPlugin->AddRectConnectedComponnent( w, h, 0.f, dist * i );     
    }
    
    root->AddPlugin( rectPlugin );

    TransformF trns;

    FloatInterpolator angle; angle.SetWrapPostMethod(bv::WrapMethod::pingPong);

    angle.AddKey(0.f, 0.f);
    angle.AddKey(5.f, 360.f);

    trns.AddRotation(angle, ConstValue( 0.f ), ConstValue( 0.f ), ConstValue( 1.f ) );

    auto transformPlugin  =  model::PluginsFactory::CreateSimpleTransformPlugin( rectPlugin, model::ParametersFactory::CreateParameter( "transformation", trns, nullptr, 0 ) );
    
    root->AddPlugin( transformPlugin );

    /////////////////////////////// Texture plugin //////////////////////////////////

    model::SimpleTexturePlugin    * stpp  = nullptr;

    FloatInterpolator alpha;
    alpha.AddKey( 0.0f, 0.25f ); alpha.SetWrapPostMethod( WrapMethod::pingPong );
    alpha.AddKey( 1.0f, 0.09f );
    alpha.AddKey( 2.0f, 0.5f );
    alpha.AddKey( 5.0f, 1.0f );
    alpha.AddKey( 8.0f, 0.75f );
    alpha.AddKey( 10.0f, 0.2f );

    TransformF tx0m;
    tx0m.AddScale( ConstValue( 1.0f ), alpha, ConstValue( 1.0f ) );

    TransformF tx1m;    
    tx1m.AddScale( alpha, ConstValue( 1.0f ), ConstValue( 1.0f ) );

    std::vector< const model::TextureDescriptor > textures;

    textures.push_back( model::TextureDescriptor( "simless_00.jpg" ) );
    textures.push_back( model::TextureDescriptor( "simless_01.jpg" ) );

    stpp = model::PluginsFactory::CreateSimpleTexturePlugin( transformPlugin, textures );

    //stpp->SetPixelShaderChannel     ( new MyPixelShaderChannel( "../dep/media/shaders/simpletexture.frag", alpha, *tx0m, *tx1m ) ); // FIXME: add texture transformation
    //stpp->SetVertexShaderChannel    ( new MyVertexShaderChannel( "../dep/media/shaders/simpletexture.vert" ) );

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
            it->AddKey(t, glm::vec3(x, float(dev) / float(RAND_MAX), z));
            t += 0.1f;
        }

        it->AddKey(t, glm::vec3(1.f, 1.f, 1.f));

        return alpha;
    }
}

// ***************************************
//    
void TestParametersFactory::CameraAnimation_ver1( Vec3Interpolator * direction, Vec3Interpolator * position, Vec3Interpolator * up )
{
    position->SetWrapMethod(WrapMethod::repeat, WrapMethod::repeat);

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
    //    position->AddKey(t, glm::vec3(x, float(dev) / float(RAND_MAX), z));
    //    t += 0.1f;
    //}

    direction->AddKey(0.f, glm::vec3(0.f, 0.f, 0.f));
    up->AddKey(0.f, glm::vec3(0.f, 1.f, 0.f));

    //m_modelScene->AddCameraInterpolators(direction, position, up);
}

// ***************************************
//    
void TestParametersFactory::CameraAnimation_ver2( Vec3Interpolator * direction, Vec3Interpolator * position, Vec3Interpolator * up )
{
    position->SetWrapMethod(WrapMethod::repeat, WrapMethod::pingPong);

    float t = 0.f;
    double endTime = 2. * M_PI;
    double dt = endTime / 10.f;
    position->AddKey(0.f, glm::vec3(-2.f, 0.1f, 3.f));
    position->AddKey(20.f, glm::vec3(2.f, 0.1f, 3.f));
    

    //float alpha = 0.f;
    //for(float alpha = 0.f; alpha <= 2 * M_PI ; alpha += 0.05f)
    //{
    //    float z = 2 * cosf(alpha);
    //    float x = 2 * sinf(alpha);
    //    int dev = rand();
    //    position->AddKey(t, glm::vec3(x, float(dev) / float(RAND_MAX), z));
    //    t += 0.1f;
    //}

    direction->AddKey(0.f, glm::vec3(0.f, 0.f, 0.f));

    up->AddKey(0.f, glm::vec3(0.f, 1.f, 0.f));
    up->AddKey(3.f, glm::vec3(0.f, 1.f, 0.f));

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
    bv::model::BasicNode* mockTree = new bv::model::BasicNode( "MockRoot" );

    // SOLID
    bv::Vec4Interpolator color;

    float rs = 10.f;

    color.AddKey(rs * .25f, glm::vec4(0,0,1,0.7f));
    //color.AddKey(rs *.5f, glm::vec4(0.3,0.4,0.5,1));
    //color.AddKey(rs *.75f, glm::vec4(0.1,1.0,0.8,0));
    //color.AddKey(rs *.95f, glm::vec4(0.7,0.2,0.4,1));
    //color.AddKey(rs *1.15f, glm::vec4(0.4,0.1,1.,0));
    //color.AddKey(rs *1.33f, glm::vec4(1,1.0,0.1,1));
    //color.AddKey(rs *1.6f, glm::vec4(1,0.4,0.2,0));
    //color.AddKey(rs *1.9f, glm::vec4(0,0.4,1.0,1));

    color.SetWrapMethod(bv::WrapMethod::clamp, bv::WrapMethod::pingPong);

//    bv::SolidColorPlugin* solid1 = new bv::SolidColorPlugin(color);

  //  mockTree->setPixelShaderPlugin(solid1);

    //bv::FloatInterpolator si;

    //si.SetWrapMethod(bv::WrapMethod::repeat, bv::WrapMethod::pingPong);
    //float s = 5.0f;
    ////si.AddKey(0.f, 0.5f);si.AddKey(0.4f, 0.5f);si.AddKey(0.5f, s * 0.6f);si.AddKey(0.65f, 0.4f);si.AddKey(0.8f, s * 0.9f);si.AddKey(0.95f, 0.35f);si.AddKey(1.05f, 0.5f);
    //si.AddKey(0.f, 0.1f);
    //si.AddKey(2.4f, 2.5f);//si.AddKey(0.5f, s * 0.6f);si.AddKey(0.65f, 0.4f);si.AddKey(0.8f, s * 0.9f);si.AddKey(0.95f, 0.35f);si.AddKey(1.05f, 0.5f);
    
    //mockTree->setGeometryShaderPlugin(new bv::ExtrudePlugin(si));

//    mockTree->addGeometryPlugin(new bv::PluginGeometryRect(4.f, 4.f));

    // TRANSFORMATION
    bv::FloatInterpolator angle; angle.SetWrapPostMethod (bv::WrapMethod::pingPong );
    bv::FloatInterpolator x;
    bv::FloatInterpolator y;
    bv::FloatInterpolator z;

    x.AddKey( 0.f, 0.f );
    y.AddKey( 0.f, 0.f );
    z.AddKey( 0.f, 1.f );
    angle.AddKey( 0.f, 0.f );
    angle.AddKey( 5.f, 180.f );

    bv::TransformF trans;
    trans.AddTransform( new bv::RotationF( angle, x,y,z ) );
//    bv::PluginTransformSimple* transPlugin = bv::ModelFactory::CreatePluginTransformSimple(*trans);

    //FIXME:
    //mockTree->addTransformPlugin(transPlugin);

    if( parent != nullptr )
    {
        parent->AddChild( mockTree );
    }

    return mockTree;
}

model::BasicNode* SceneExamples::BuildMockScene2( bv::model::BasicNode * parent )
{
    bv::model::BasicNode* mockTree = new bv::model::BasicNode( "Mock2Root" );

    // SOLID
    bv::Vec4Interpolator color;

    color.AddKey(.0f, glm::vec4(1,0,0,0.5f));
    //color.AddKey(3.f, glm::vec4(0.3,0.4,0.5,1.f));
    //color.AddKey(1.f, glm::vec4(0.1,0.2,0.8,1.f));

    //color.AddKey(2.f, glm::vec4(1,0.4,0.5,1.f));

    color.SetWrapMethod(bv::WrapMethod::clamp, bv::WrapMethod::pingPong);

//    bv::SolidColorPlugin* solid1 = new bv::SolidColorPlugin(color);

//    mockTree->setPixelShaderPlugin(solid1);

    //bv::FloatInterpolator scaleInt;

    //scaleInt.SetWrapMethod(bv::WrapMethod::pingPong, bv::WrapMethod::pingPong);

    //scaleInt.AddKey(0.f, -3.5f);
    //scaleInt.AddKey(-1.30f, 3.5f);

    //mockTree->setGeometryShaderPlugin(new bv::ExtrudePlugin(scaleInt));

//    mockTree->addGeometryPlugin(new bv::PluginGeometryRect());

    // TRANSFORMATION
    bv::FloatInterpolator x;
    bv::FloatInterpolator y;
    bv::FloatInterpolator z;

    float s = 2.0f;
    float l = 1.4f;
    x.AddKey(0.f, 0.5f);x.AddKey(0.4f, 0.5f);x.AddKey(0.5f, s * 0.6f);x.AddKey(0.65f, 0.4f);x.AddKey(0.8f, s * 0.9f);x.AddKey(0.95f, 0.35f);x.AddKey(1.05f, 0.5f);
    y.AddKey(0.f, 0.5f);y.AddKey(0.4f, 0.5f);y.AddKey(0.5f, l * 0.6f);y.AddKey(0.65f, 0.4f);y.AddKey(0.8f, l * 0.9f);y.AddKey(0.95f, 0.35f);y.AddKey(1.05f, 0.5f);
    z.AddKey(0.f, 1.f);

    x.SetWrapPostMethod(bv::WrapMethod::repeat);
    y.SetWrapPostMethod(bv::WrapMethod::repeat);
    
    bv::TransformF * trans = new bv::TransformF();

    bv::FloatInterpolator mx;
    bv::FloatInterpolator my;
    bv::FloatInterpolator mz;

    mx.AddKey(0.f, 0.0f);
    my.AddKey(0.f, 0.0f);
    mz.AddKey(0.f, 0.0f);

    mx.AddKey(1.f, 1.0f);
    my.AddKey(2.f, -2.0f);
    mz.AddKey(3.f, 0.f);

    trans->AddTranslation(mx, my, mz);
    trans->AddScale(x,y,z);

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
//    transX.AddKey(0.f,x);
//    bv::FloatInterpolator transY;
//    transY.AddKey(0.f,y);
//    bv::FloatInterpolator transZ;
//    transZ.AddKey(0.f,z);
//    trans->addTranslation(transX,transY,transZ);
//    bv::PluginTransformSimple* transPlugin = bv::ModelFactory::CreatePluginTransformSimple(*trans);
//    return transPlugin;
//}



//void AnimLines(bv::BasicNode* parent)
//{
//    bv::BasicNode* lineUp = new bv::BasicNode();
//    bv::Vec4Interpolator color;
//    
//    color.AddKey(0.01f, glm::vec4(0,0,1,1));
//    color.SetWrapMethod(bv::WrapMethod::clamp, bv::WrapMethod::pingPong);
//    bv::SolidColorPlugin* solidRect = new bv::SolidColorPlugin(color);
//    lineUp->setPixelShaderPlugin(solidRect);
////	lineUp->addGeometryPlugin(new bv::PluginGeometryRect(0.1f,0.1f));
//    
//    //bez sensu
//    bv::FloatInterpolator si;
//    si.SetWrapMethod(bv::WrapMethod::repeat, bv::WrapMethod::pingPong);
//    float s = 5.0f;
//    si.AddKey(0.f, 0.1f);
//    lineUp->setGeometryShaderPlugin(new bv::ExtrudePlugin(si));
//
//
//    bv::FloatInterpolator identity; 
//    identity.AddKey(0.f, 1.f);
//
//
//    bv::FloatInterpolator scaleX; 
//    scaleX.SetWrapPostMethod(bv::WrapMethod::clamp);
//    scaleX.AddKey(0.f, 1.f);
//    scaleX.AddKey(2.f, 10.0f);
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
//    color.AddKey(0, glm::vec4(1,0,0,1));
//    color.SetWrapMethod(bv::WrapMethod::clamp, bv::WrapMethod::pingPong);
//    bv::SolidColorPlugin* solidRect = new bv::SolidColorPlugin(color);
//    rectNode->setPixelShaderPlugin(solidRect);
//
//    //extrude
//    bv::FloatInterpolator si;
//    si.SetWrapMethod(bv::WrapMethod::repeat, bv::WrapMethod::pingPong);
//    float s = 5.0f;
//    si.AddKey(0.f, 0.1f);
//    si.AddKey(1.f, -0.1f);
//    //si.AddKey(2.4f, 2.5f);//si.AddKey(0.5f, s * 0.6f);si.AddKey(0.65f, 0.4f);si.AddKey(0.8f, s * 0.9f);si.AddKey(0.95f, 0.35f);si.AddKey(1.05f, 0.5f);
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
//    identity.AddKey(0.f, 1.f);
//
//    bv::FloatInterpolator scaleX; scaleX.SetWrapPostMethod(bv::WrapMethod::pingPong);
//    scaleX.AddKey(0.f, 1.f);
//    scaleX.AddKey(5.f, 1.2f);
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
//    color.AddKey(2, glm::vec4(0.1,0,0,1));
//    color.AddKey(5, glm::vec4(1,0,0,1));
// 
//    color.SetWrapMethod(bv::WrapMethod::clamp, bv::WrapMethod::pingPong);
//
//    bv::SolidColorPlugin* solid1 = new bv::SolidColorPlugin(color);
//
//    mockTree->setPixelShaderPlugin(solid1);
//
//    bv::FloatInterpolator si;
//
//    si.SetWrapMethod(bv::WrapMethod::repeat, bv::WrapMethod::pingPong);
//    float s = 5.0f;
//    si.AddKey(0.f, 0.1f);
//    si.AddKey(0.2f, 1.2f);
//    //si.AddKey(2.4f, 2.5f);//si.AddKey(0.5f, s * 0.6f);si.AddKey(0.65f, 0.4f);si.AddKey(0.8f, s * 0.9f);si.AddKey(0.95f, 0.35f);si.AddKey(1.05f, 0.5f);
//
//    mockTree->setGeometryShaderPlugin(new bv::ExtrudePlugin(si));
//
//    FloatInterpolator radIn;
//    FloatInterpolator radOut;
//    FloatInterpolator start;
//    FloatInterpolator end;
//
//    radIn.AddKey(0.f, 1.7f);
//    radOut.AddKey(0.f, 2.f);
//    start.AddKey(0.f, startAngle);
//    end.AddKey(0.f, endAngle);
////	   mockTree->addGeometryPlugin(new bv::PluginGeometryRing(radIn, radOut, start, end, 3));
//    //mockTree->addGeometryPlugin(new bv::PluginGeometryRect());
//
//    // TRANSFORMATION
//    bv::FloatInterpolator angle; angle.SetWrapPostMethod(bv::WrapMethod::pingPong);
//    bv::FloatInterpolator x;
//    bv::FloatInterpolator y;
//    bv::FloatInterpolator z;
//
//    x.AddKey(0.f, 0.f);
//    y.AddKey(0.f, 0.f);
//    z.AddKey(0.f, -1.1f);
//    angle.AddKey(0.f, 0.f);
//    //angle.AddKey(5.f, 180.f);
//
//    bv::TransformF* trans = new bv::TransformF();
//    
//    bv::FloatInterpolator transX;
//    transX.AddKey(0.f,offsetX);
//
//    bv::FloatInterpolator transY;
//    transY.AddKey(0.f,offsetY);
//    
//    bv::FloatInterpolator transZ;
//    transZ.AddKey(0.f,offsetZ);
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
//    color.AddKey(number*atom, glm::vec4(0,0,0,0));
//    color.AddKey(number*atom+2, glm::vec4(0,0,0,0));
//    color.AddKey(number*atom+3, glm::vec4(1,0,0,1));
//    color.AddKey(number*atom+4, glm::vec4(0,0,0,0));
//    color.AddKey(number*atom+6, glm::vec4(0,0,0,0));
//    color.SetWrapMethod(bv::WrapMethod::clamp, bv::WrapMethod::repeat);
//    bv::SolidColorPlugin* solidRect = new bv::SolidColorPlugin(color);
//    rectNode->setPixelShaderPlugin(solidRect);
//
//    //extrude
//    bv::FloatInterpolator si;
//    si.SetWrapMethod(bv::WrapMethod::repeat, bv::WrapMethod::pingPong);
//    float s = 5.0f;
//    si.AddKey(0.f, 0.1f);
//    //si.AddKey(1.f, 0.3f);
//    //si.AddKey(2.4f, 2.5f);//si.AddKey(0.5f, s * 0.6f);si.AddKey(0.65f, 0.4f);si.AddKey(0.8f, s * 0.9f);si.AddKey(0.95f, 0.35f);si.AddKey(1.05f, 0.5f);
//    rectNode->setGeometryShaderPlugin(new bv::ExtrudePlugin(si));
//    
//    //rectNode->addGeometryPlugin(new bv::PluginGeometryRect(RectWidth,0.1f));
//
//    // b edzie sie rozszerzal
//
//    bv::FloatInterpolator identity; 
//    identity.AddKey(0.f, 1.f);
//
//    bv::FloatInterpolator scaleX; scaleX.SetWrapPostMethod(bv::WrapMethod::pingPong);
//    scaleX.AddKey(0.f, 1.f);
//    scaleX.AddKey(5.f, 1.2f);
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

