//
//  CCL_MocapJoint.cpp
//  CCL_JSON
//
//  Created by Gene Han on 8/28/15.
//
//

#include "CCL_MocapJoint.h"
#define NULL_POINT -123456.f

CCL_MocapJoint::CCL_MocapJoint( string jName, int skip){
    jointName = jName;
    this->skip = skip;
    xUpdated = false;
    yUpdated = false;
    zUpdated = false;
    valid = false;

};

void CCL_MocapJoint::addUUID(string title,const string uuid){
    if( title.compare("X") == 0){
        string url = "http://api.piecemeta.com/streams/" + uuid+".json?skip=10";
     //   std::cout << "[Stream URL]"<< url<< std::endl;
        const JsonTree stream( loadUrl(url) );
        int frameCount = stream["frameCount"].getValue<int>();
        for (int i = 0 ; i < stream["frames"].getNumChildren() ; i++){
            string temp = stream["frames"][i].getValue();
            if( temp == "null" || temp.size() == 0){
   //             cout << "[FrameAt:"<<i<< ","<<temp<<endl;
                if ( i!=0 ){
                std::string prev = stream["frames"][i-1].getValue();
                xPositions.push_back(xPositions[i-1]);
                } else {
                    xPositions.push_back(stof(temp));
                }

            }else{
  //              cout << "[FrameAt:"<<i<< ","<<temp<<endl;
                xPositions.push_back(stof(temp));
            }
        }
    //    cout << jointName << "X FrameCount:" << frameCount <<" FramesNum:" << xPositions.size() << " Skip:10" << endl;
        xUpdated = true;
    }else if( title.compare("Y") == 0 ){
        string url = "http://api.piecemeta.com/streams/" + uuid+".json?skip=10";
     //   std::cout << "[Stream URL]"<< url<< std::endl;
        const JsonTree stream( loadUrl(url) );
        int frameCount = stream["frameCount"].getValue<int>();
        for (int i = 0 ; i < stream["frames"].getNumChildren() ; i++){
            string temp = stream["frames"][i].getValue();
            if( temp == "null" || temp.size() == 0){
                //             cout << "[FrameAt:"<<i<<
                if ( i!=0 ){
                std::string prev = stream["frames"][i-1].getValue();
                yPositions.push_back(yPositions[i-1]);
            } else {
                yPositions.push_back(stof(temp));
            }
            
            }else{
                //              cout << "[FrameAt:"<<i<< ","<<temp<<endl;
                yPositions.push_back(stof(temp));
            }
        }
      //  cout << jointName << "Y FrameCount:" << frameCount <<" FramesNum:" << yPositions.size() << " Skip:10" << endl;
        yUpdated = true;
    }else if( title.compare("Z") == 0){
        string url = "http://api.piecemeta.com/streams/" + uuid+".json?skip=10";
    //    std::cout << "[Stream URL]"<< url<< std::endl;
        const JsonTree stream( loadUrl(url) );
        int frameCount = stream["frameCount"].getValue<int>();
        for (int i = 0 ; i < stream["frames"].getNumChildren() ; i++){
            string temp = stream["frames"][i].getValue();
            if( temp == "null" || temp.size() == 0){
                //             cout << "[FrameAt:"<<i<< ","<<temp<<endl;
                if ( i!=0 ){
                    std::string prev = stream["frames"][i-1].getValue();
                    zPositions.push_back(zPositions[i-1]);
                } else {
                    zPositions.push_back(stof(temp));
                }
                
            }else{
                //              cout << "[FrameAt:"<<i<< ","<<temp<<endl;
                zPositions.push_back(stof(temp));
            }
        }
   //     cout << jointName <<  "Z FrameCount:" << frameCount <<" FramesNum:" << zPositions.size() << " Skip:10" << endl;
        zUpdated = true;
    }
    
    if( xUpdated && yUpdated && zUpdated){
        
        loadPositions();
    }
    

};




void CCL_MocapJoint::loadPositions(){
    
    jointPositions.clear();
    
    int i = 0;
    
    for( i = 0 ; i < xPositions.size() ; i++){
        
        float x = xPositions[i];
        
        if( x == -123456 ){
            
            if( i == 0 ){
                
                x=0;
                
                for( int j = i; j < xPositions.size(); j++){
                    
                    if( xPositions[j] != -123456){
                        
                        x = xPositions[j];
                        
                        if( j == xPositions.size()-1){
                            
                            x = 0;
                            
                        }
                        
                        break;
                        
                    }
                    
                }
                
            }else
                
                x = jointPositions[jointPositions.size()-1].x;
            
        }
        
        float y = yPositions[i];
        
        if( y == -123456){
            
            if( i == 0 ){
                
                y = 0;
                
                for( int j = i; j < yPositions.size(); j++){
                    
                    if( yPositions[j] != -123456){
                        
                        y = yPositions[j];
                        
                        if( j == yPositions.size()-1){
                            
                            y = 0;
                            
                        }
                        
                        break;
                        
                    }
                    
                }
                
            }else
                
                y = jointPositions[jointPositions.size()-1].y;
            
        }
        
        float z = zPositions[i];
        
        if( z == -123456){
            
            if( i == 0 ){
                
                z = 0;
                
                for( int j = i; j < zPositions.size(); j++){
                    
                    if( zPositions[j] != -123456){
                        
                        z = zPositions[j];
                        
                        if( j == zPositions.size()-1){
                            
                            z = 0;
                            
                        }
                        
                        break;
                        
                    }
                    
                    
                    
                }
                
                
                
            }else
                
                z = jointPositions[jointPositions.size()-1].z;
            
        }
        
        
        
        vec3 vecter = vec3(x,y,z);
        
        if( x !=0 || y != 0 || z != 0)
            
            valid = true;
        
        
        
        jointPositions.push_back(vecter);
        
        //      cout << "["<<jointName<< " at " << i<< "] " << vecter << endl;
        
    }
    
    if( !valid )
        
        
        std::cout << "added joint " << jointName << ", count:" << jointPositions.size() << std::endl;
    
    
    
    
    
};