//
//  Dancer.cpp
//  CCLmerged
//
//  Created by Craig Pickard on 9/8/15.
//
//

#include "Dancer.h"
#include <vector>
#include "cinder/Utilities.h"
#include "cinder/Easing.h"

Dancer::Dancer()
{
}

Dancer::Dancer(const std::string& fileName, const gl::GlslProgRef& shader)
{
    //IMPORT ALL JOINT POSITION DATE FROM EXTERNAL JSON FILE
    loadJointData(fileName);
    
    //INITIALIZE BOOLEANS FOR DANCERS DISPLAY PROPERTIES
    this->showMarkers = true;
    this->showRibbons = false;
    this->showSkeleton = false;
    this->showTrails = true;
    
    //CREATE THE SPHERE BATCH USING THE GLOBAL SHADER
    mSphereBatch = gl::Batch::create( geom::Sphere(), shader);
    
    //CREATE VBOMESH CONTAINING THE GEOMETRY FOR THE MARKER POSITIONS
    body = gl::VboMesh::create( geom::Sphere().subdivisions( 16 ).radius(2) );
    
    // CREATE THE SPHERES AT THE INITIAL JOINT LOCATIONS
    for ( int i = 0; i < jointList.size(); ++i ) {
        if (jointList[i].valid){
            float instanceX = jointList[i].jointPositions[0].x;
            float instanceY = jointList[i].jointPositions[0].y;
            float instanceZ = jointList[i].jointPositions[0].z;
            
            initialJointPositions.push_back( vec3( instanceX, instanceY, instanceZ));
        }
    }
    
    //CREATE THE VBO THAT WILL CONTAIN PER-INSTANCE (RATHER THAN PER VERTEX) DATA
    mInstanceDataVbo = gl::Vbo::create( GL_ARRAY_BUFFER, initialJointPositions.size() * sizeof(vec3), initialJointPositions.data(), GL_DYNAMIC_DRAW );
    
    // we need a geom::BufferLayout to describe this data as mapping to the CUSTOM_0 semantic, and the 1 (rather than 0) as the last param indicates per-instance (rather than per-vertex)
    geom::BufferLayout instanceDataLayout;
    
    instanceDataLayout.append( geom::Attrib::CUSTOM_0, 3, 0, 0, 1 /* per instance */ );
    
    //NOW ADD IT TO THE VBO MESH THAT WE INITIAL CREATED FOR THE BODY / SKELETON
    body->appendVbo( instanceDataLayout, mInstanceDataVbo );
    
    //FINALLY, BUILD THE BATCH, AND MAP THE CUSTOM_0 ATTRIBUTE TO THE "vInstancePosition" GLSL VERTEX ATTRIBUTE
    mSphereBatch = gl::Batch::create( body, shader, { { geom::Attrib::CUSTOM_0, "vInstancePosition" } } );
    
    handTrail = Trail(initialJointPositions[17]);
    
    for (int i = 0; i < initialJointPositions.size(); i++){
        allTrails.push_back(Trail(initialJointPositions[i]));
    }
    
}

//ACCESSOR FUNCTION RETURNS TO THE TOTAL NUMBER OF FRAMES IN THE JOINTLIST
int Dancer::getSize(){
    return jointList[0].jointPositions.size();
}


/* --------- LOAD JOINT DATA -----------------*/

void Dancer::loadJointData(const std::string& jsonData )
{
    //LOAD THE MOTION CAPTURE DATA FROM A JSON FILE BY PASSING IN THE FILE NAME (STORED IN ASSETS) AS A STRING
    jointList = ccl::loadMotionCaptureFromJson(ci::app::getAssetPath(jsonData));
}


/*-------------------- UPDATE-------------------*/


void Dancer::update(const int& FRAME_COUNT)
{
    updatedJointPositions.clear();
    
    int frameRounded = FRAME_COUNT % getSize();
    
    glm::vec3 *newPositions = (glm::vec3*)mInstanceDataVbo->mapReplace();
    
    for( int i = 0; i < jointList.size(); ++i ) {

        if (jointList[i].valid){
            float instanceX = jointList[i].jointPositions[frameRounded].x;
            float instanceY = jointList[i].jointPositions[frameRounded].y;
            float instanceZ = jointList[i].jointPositions[frameRounded].z;
            
            vec3 newPos(vec3(instanceX,instanceY, instanceZ)); //CREATE A NEW VEC3 FOR UPDATING THE VBO
            
            updatedJointPositions.push_back(newPos);
        }
        
    }
    
    //REPLACE VEC3s IN VBO BY INCREMENTING THE POINTER AND SETTING EACH NEW POSITION
    for (int i = 0; i < updatedJointPositions.size(); i++){
        *newPositions++ = updatedJointPositions[i];
    }

    mInstanceDataVbo->unmap();

    //UPDATE THE TRAIL POSITION
    updateTrails(updatedJointPositions);

}

void Dancer::render(){
    if(showMarkers)mSphereBatch->drawInstanced( initialJointPositions.size() );
    if(showTrails)renderTrails();
}



//------------------- UPDATE TRAILS --------------------

void Dancer::updateTrails(const std::vector<glm::vec3>& updatedPositions)
{
    std::list<Trail>::iterator iter = allTrails.begin();
    while (iter != allTrails.end()){
        for (int i = 0; i < updatedPositions.size(); i++){
            iter->update(updatedPositions[i]);
            iter++;
        }
    }
}

//---------------------- RENDER TRAILS ------------------

void Dancer::renderTrails(){
    for (std::list<Trail>::iterator it = allTrails.begin(); it != allTrails.end(); ++it){
        it->render();
    }
}




