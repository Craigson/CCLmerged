//
//  Trail.cpp
//  CCLfixed
//
//  Created by Craig Pickard on 8/31/15.
//
//

#include "Trail.h"

Trail::Trail(){
    
}

Trail::Trail(const glm::vec3& origin)

{
 
    trail = ci::gl::VertBatch( GL_LINE_STRIP );
    
    ci::ColorAf defaultColor( 0.9f, 0.9f, 0.9f,0.9f);
    
    trail.color( defaultColor );
    
    trailBatch = ci::gl::Batch::create ( trail , ci::gl::getStockShader( ci::gl::ShaderDef().color() ) );
    
    
}





void Trail::update(const glm::vec3& pos)

{
    
    trail.color( ci::ColorAf(0.9f, 0.9f, 0.9f,0.1f) );
    
    trail.vertex(pos);
    
}



void Trail::render()
{
    trail.draw();
}
