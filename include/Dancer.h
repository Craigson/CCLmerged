//
//  Dancer.h
//  CCLmerged
//
//  Created by Craig Pickard on 9/8/15.
//
//

#pragma once

#include <stdio.h>
//#include "Skeleton.h"
//#include "Trail.h"
#include "CCL_MocapJoint.h"
#include "CCL_MocapData.h"
#include "Ribbon.h"
#include "Trail.h"

class Dancer{
public:
    
    Dancer();
    Dancer(const std::string& fileName, const gl::GlslProgRef& shader);
    
    void loadJointData(const std::string& jsonData);
    void render();
    void update(const int& FRAME_COUNT);
    int getSize();
    
  //  void cleanData(std::vector<CCL_MocapJoint>& jointList);
    void updateTrail(const glm::vec3& updatedPos);
    void renderTrail();
    
private:
    
    std::vector<CCL_MocapJoint>     jointList;  //GLOBAL CONTAINER TO HOLD THE CCL_JOINT OBJECTS
    std::vector<glm::vec3>          initialJointPositions;  //POSITION OF EACH JOINT AT FRAME 0
    std::vector<glm::vec3>          updatedJointPositions;  //CONTAINER FOR EACH UPDATED JOINT POSITION
    
    //MARKERS
    gl::VboMeshRef      body;
    gl::BatchRef        mSphereBatch;
    gl::VboRef			mInstanceDataVbo;
    
    //DISPLAY PROPERTIES
   bool renderMarkers, renderSkeleton, renderTrails, renderRibbons;
    
    //  Skeleton skeleton;
    Trail handTrail;
    //  std::vector<Ribbon> ribbons;
    
    // std::vector<ci::vec3> createRibbons(float width, const std::function<float (float)> &shape_fn, const ci::vec3 &eye_vector, const std::vector<ci::vec3> &spine);

};