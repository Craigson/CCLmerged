#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/CameraUi.h"
#include "Dancer.h"

using namespace ci;
using namespace ci::app;
using namespace std;

int FRAME_COUNT;
int TOTAL_FRAMES;

class CCLmergedApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
    void mouseDrag (MouseEvent event) override;
	void update() override;
	void draw() override;
    
    void setupEnviron( int xSize, int zSize, int spacing );     //METHOD FOR SETTING UP THE 3D ENVIRONMENT
    void renderScene();                                         //METHOD FOR RENDERING THE 3D ENVIRONMENT
    void setupShader();
    
    //GLOBAL SHADER
    gl::GlslProgRef		mGlsl;
    gl::GlslProgRef     solidShader;
    
    //CREATE A VERTEX BATCH FOR THE FLOOR MESH
    gl::VertBatchRef	mGridMesh;
    
    CameraPersp			mCamera;
    CameraUi			mCamUi;
    
    int                 mCurrentFrame = 0;
    
    //CREATE A SINGLE DANCER OBJECT (TEST)
    Dancer dancer1;
};

void CCLmergedApp::setup()
{
    setFrameRate(12);
    
    //SETUP THE 3D ENVIRONMENT
    setupEnviron( 5000, 5000, 100 );
    
    setupShader();
    
    
    //mCamera.setEyePoint(vec3(0,200,650));
    mCamUi = CameraUi( &mCamera, getWindow() );
    
    dancer1 = Dancer("CCL_JOINT_CCL3_00_skip10.json", mGlsl);
    
    gl::enableDepthWrite();
    gl::enableDepthRead();

}

void CCLmergedApp::mouseDown( MouseEvent event )
{
}




/*------------------- MOUSE DRAGGED ------------------------------*/

void CCLmergedApp::mouseDrag( MouseEvent event )
{
    mCamUi.mouseDrag( event );
}




/*----------------------------- UPDATE ------------------------------*/

void CCLmergedApp::update()
{
    TOTAL_FRAMES = dancer1.getSize();
    
    std::cout<< dancer1.getSize() << std::endl;
    
    dancer1.update(FRAME_COUNT);
    
//    //MANUALLY INCREMENT THE FRAME, IF THE FRAME_COUNT EXCEEDS TOTAL FRAMES, RESET THE COUNTER
    if (FRAME_COUNT < TOTAL_FRAMES)
    {
        FRAME_COUNT += 1;
    } else {
        FRAME_COUNT = 0;
    }
}




/* ---------------------------- DRAW --------------------------------*/

void CCLmergedApp::draw()
{
    
    gl::clear(Color(0.05f,0.05f,0.05f) );

    gl::setMatrices( mCamera );
    
    renderScene();
    
    dancer1.render();
}

//------------------- SETUP THE ENVIRONMENT / GRID -----------------------

void CCLmergedApp::setupEnviron( int xSize, int zSize, int spacing )
{
    CI_ASSERT( ( spacing <= xSize ) && ( spacing <= zSize ) );
    
    // Cut in half and adjust for spacing.
    xSize = ( ( xSize / 2 ) / spacing ) * spacing;
    zSize = ( ( zSize / 2 ) / spacing ) * spacing;
    
    const int xMax = xSize + spacing;
    const int zMax = zSize + spacing;
    const ColorA defaultColor( 0.9f, 0.9f, 0.9f,0.1f);
    const ColorA black( 0, 0, 0, 1 );
    
    mGridMesh = gl::VertBatch::create( GL_LINES );
    
    // Add x lines.
    for( int xVal = -xSize; xVal < xMax; xVal += spacing ) {
        mGridMesh->color( defaultColor );
        mGridMesh->vertex( (float)xVal, 0, (float)-zSize );
        mGridMesh->vertex( (float)xVal, 0, (float)zSize );
    }// end for each x dir line
    
    // Add z lines.
    for( int zVal = -zSize; zVal < zMax; zVal += spacing ) {
        mGridMesh->color( defaultColor );
        mGridMesh->vertex( (float)xSize, 0, (float)zVal );
        mGridMesh->vertex( (float)-xSize, 0, (float)zVal );
    }// end for each z dir line
    
    //SETUP THE CAMERA
    mCamera.lookAt( vec3( 500, 500, 0 ), vec3( 0 ) );
    mCamera.setEyePoint(vec3(500,1000,0));
    mCamera.setFarClip(20000);
}




/*------------------ RENDER THE SCENE ------------------------*/

void CCLmergedApp::renderScene()
{
    
   // gl::pushMatrices();
    mGridMesh->draw();
   // gl::popMatrices();
}




/*--------------------- SETUP SHADERS -----------------------*/

void CCLmergedApp::setupShader(){
    
    //CHOOSE BETWEEN solidShader AND mGlsl AS SHADERS FOR THE SPHERES
    //    gl::ScopedColor color( Color( 0, 1, 0 ) );
    //    solidShader = gl::getStockShader( color );
    mGlsl = gl::GlslProg::create( loadAsset( "shader.vert" ), loadAsset( "shader.frag" ) );
}

CINDER_APP( CCLmergedApp, RendererGl(RendererGl::Options().msaa( 16 ) ), [&]( App::Settings *settings ) {
    settings->setWindowSize( 1280, 720 );
    //settings->setFullScreen();
    
} )
