//
//  Ribbon.h
//  CCLmerged
//
//  Created by Craig Pickard on 9/8/15.
//
//

#pragma once

#include <stdio.h>
#include "CCL_MocapJoint.h"

class Ribbon
{
public:
    Ribbon();
    Ribbon(std::vector<CCL_MocapJoint> jointList, const std::vector<vec3>& initalLocs);
    
    glm::vec3               _target;
    std::vector<glm::vec3>  _spine;
    std::vector<glm::vec3>  _triangles;
    size_t                  _joint_index = 0;
    
    void drawRibbons();
    void updateRibbons();
    
};