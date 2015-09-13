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
    
    //CREATE THE SPHERE BATCH USING THE GLOBAL SHADER
    mSphereBatch = gl::Batch::create( geom::Sphere(), shader);
    
    //CREATE VBOMESH CONTAINING THE GEOMETRY FOR THE MARKER POSITIONS
    body = gl::VboMesh::create( geom::Sphere().subdivisions( 16 ).radius(2) );
    
    // CREATE THE SPHERES AT THE INITIAL JOINT LOCATIONS
    for ( int i = 0; i < jointList.size(); ++i ) {
        float instanceX = jointList[i].jointPositions[0].x;
        float instanceY = jointList[i].jointPositions[0].y;
        float instanceZ = jointList[i].jointPositions[0].z;
        
    initialJointPositions.push_back( vec3( instanceX, instanceY, instanceZ));
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
    
}

int Dancer::getSize(){
    return jointList[0].jointPositions.size();
}

void Dancer::loadJointData(const std::string& jsonData )
{
    //"CCL_JOINT_CCL3_00_skip10.json"
    jointList = ccl::loadMotionCaptureFromJson(ci::app::getAssetPath(jsonData));
}


void Dancer::update(const int& FRAME_COUNT)
{
    updatedJointPositions.clear();
    
    glm::vec3 *newPositions = (glm::vec3*)mInstanceDataVbo->mapReplace();
    
    for( int i = 0; i < jointList.size(); ++i ) {
        
        float instanceX = jointList[i].jointPositions[FRAME_COUNT].x;
        float instanceY = jointList[i].jointPositions[FRAME_COUNT].y;
        float instanceZ = jointList[i].jointPositions[FRAME_COUNT].z;
        
        vec3 newPos(vec3(instanceX,instanceY, instanceZ)); //CREATE A NEW VEC3 FOR UPDATING THE VBO
        
        updatedJointPositions.push_back(newPos);
        
    //    std::cout<< updatedJointPositions.size() << std::endl;
       //
        
    }
    
    //REPLACE VEC3s IN VBO BY INCREMENTING THE POINTER
    for (int i = 0; i < updatedJointPositions.size(); i++){
        *newPositions++ = updatedJointPositions[i];
    }

    
    mInstanceDataVbo->unmap();
    // std::cout << "position: " << positions[0] << std::endl;

}

void Dancer::render(){
    mSphereBatch->drawInstanced( jointList.size() );
}
