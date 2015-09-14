#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/CameraUi.h"
#include "Dancer.h"
#include "CinderImGui.h"

using namespace ci;
using namespace ci::app;
using namespace std;

int FRAME_COUNT;
int TOTAL_FRAMES = 6400;
BOOL paused;

//ADD BACKGROUND COLOR CHANGE
//ADD COLOR FOR EACH DANCER
//SHOW FLOOR

/********** DATA ____ GUI ***********************/
int CURRENT_DATA_SET = 0;
int LOADED_DATA_SET = 0;


bool showDancers[] = {true, false, false, false,false,false};
int dancerFPS[] = {24,60,60,60,30,30};

int CinderFrameReate = 60;

/********** DATA ____ GUI ***********************/

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
    
    void createDancers();
    
    void drawGUI();
    
    //GLOBAL SHADER
    gl::GlslProgRef		mGlsl;
    //gl::GlslProgRef     solidShader;
    
    //CREATE A VERTEX BATCH FOR THE FLOOR MESH
    gl::VertBatchRef	mGridMesh;
    
    CameraPersp			mCamera;
    CameraUi			mCamUi;
    
    int                 mCurrentFrame = 0;
    
    //CREATE A SINGLE DANCER OBJECT (TEST)
   // Dancer dancer1;
    
    std::vector<Dancer> dancers;
    
};

void CCLmergedApp::setup()
{
    setFrameRate(60);
    
    //SETUP THE 3D ENVIRONMENT
    setupEnviron( 5000, 5000, 100 );
    
    setupShader();
    
    for (int i = 0; i < dancers.size(); i++){
        showDancers[i] = false;
    }
    
    showDancers[0] = true;
    
    
    //bool isDancer1 = true;
    dancerFPS[0] = 24;
    
    //bool isDancer2 = true;
    dancerFPS[1] = 60;
    
    //bool isDancer3 = true;
    dancerFPS[2] = 60;
    
    //bool isDancer4 = true;
    dancerFPS[3] = 60;
    
    //bool isDancer5 = true;
    dancerFPS[4] = 30;
    
    //bool isDancer6 = true;
    dancerFPS[5] = 30;
    
    //mCamera.setEyePoint(vec3(0,200,650));
    
    createDancers();
    
   gl::enableDepthWrite();
    gl::enableDepthRead();
    gl::enableAlphaBlending();

    /************* UI *************/
    
    // THIS HAS BE DONE BEFORE "CameraUI" //
   ui::initialize();
    
    /************* UI *************/
    
    
    mCamUi = CameraUi( &mCamera, getWindow() );
    
    //    mCamUi.disconnect();
    
    std::cout << "end of setup" << std::endl;
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
 
    /************* UI *************/
    if( paused)
        return;
    /************* UI *************/
    
    for (int i = 0; i < dancers.size(); i++){
        if (showDancers[i])dancers[i].update(FRAME_COUNT/(CinderFrameReate/dancerFPS[i]));
    }

//    //MANUALLY INCREMENT THE FRAME, IF THE FRAME_COUNT EXCEEDS TOTAL FRAMES, RESET THE COUNTER
    if (FRAME_COUNT < TOTAL_FRAMES)
    {
        FRAME_COUNT += 1;
    } else {
        FRAME_COUNT = 0;
    }
    std::cout << getAverageFps() << std::endl;
}




/* ---------------------------- DRAW --------------------------------*/

void CCLmergedApp::draw()
{
    gl::clear(Color(0.05f,0.05f,0.05f) );
    gl::setMatrices( mCamera );
    renderScene();
    drawGUI();

    for (int i =0; i < dancers.size(); i++){
        if( showDancers[i])
            dancers[i].render();
    }
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

//---------------------- CREATE DANCERS ----------------------

void CCLmergedApp::createDancers()
{
    dancers.push_back(Dancer("CCL_JOINT_CCL3_00_skip10.json", mGlsl));
    dancers.push_back(Dancer("CCL_JOINT_CCL4_00_skip4.json", mGlsl));
    dancers.push_back(Dancer("CCL_JOINT_CCL4_01_skip4.json", mGlsl));
    dancers.push_back(Dancer("CCL_JOINT_CCL4_02_skip4.json", mGlsl));
    dancers.push_back(Dancer("CCL_JOINT_CCL4_03_skip8.json", mGlsl));
    dancers.push_back(Dancer("CCL_JOINT_CCL4_04_skip8.json", mGlsl));
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

void CCLmergedApp::drawGUI()
{
    
    /************* UI *************/
    
    ui::InputInt("FPS", &CinderFrameReate);
    setFrameRate(CinderFrameReate);
    
    if( ui::Button("PLAY") ){
        paused = false;
    }
    if( ui::Button("PAUSE") ){
        paused = true;
    }
    ui::SliderInt("PROGRESS", &FRAME_COUNT, 0, TOTAL_FRAMES);
    
    /********** DATA ____ GUI ***********************/
    
    vector<std::string> dataVector = {"CCL_JOINT_CCL3_00_skip10.json",
        "CCL_JOINT_CCL4_00_skip4.json",
        "CCL_JOINT_CCL4_01_skip4.json",
        "CCL_JOINT_CCL4_02_skip4.json",
        "CCL_JOINT_CCL4_03_skip8.json",
        "CCL_JOINT_CCL4_04_skip8.json"};
    
    ui::Checkbox("Dancer1", &showDancers[0]);
    ui::InputInt("D1 FPS", &dancerFPS[0]);
    
    ui::Checkbox("Dancer2", &showDancers[1]);
    ui::InputInt("D2 FPS", &dancerFPS[1]);
    
    ui::Checkbox("Dancer3", &showDancers[2]);
    ui::InputInt("D3 FPS", &dancerFPS[2]);
    
    ui::Checkbox("Dancer4", &showDancers[3]);
    ui::InputInt("D4 FPS", &dancerFPS[3]);
    
    ui::Checkbox("Dancer5", &showDancers[4]);
    ui::InputInt("D5 FPS", &dancerFPS[4]);
    
    ui::Checkbox("Dancer6", &showDancers[5]);
    ui::InputInt("D6 FPS", &dancerFPS[5]);
    
}

