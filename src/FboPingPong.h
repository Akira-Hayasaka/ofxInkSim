//
//  FboPingPong.h
//  emptyExample
//
//  Created by Akira on 5/11/15.
//
//

#ifndef emptyExample_FboPingPong_h
#define emptyExample_FboPingPong_h

#include "ofMain.h"

class FboPingPong
{
public:
    
    void allocate(ofFbo::Settings settings);
    void swap();
    void clear(int alpha = 255);
    ofPtr<ofFbo> getOldTex();
    ofPtr<ofFbo> getNewTex();
    ofPtr<ofFbo> getReadTex();
    ofPtr<ofFbo> getWriteTex();
    
private:
    
    int readTex;
    int writeTex;
    ofPtr<ofFbo> tex0;
    ofPtr<ofFbo> tex1;
    vector<ofPtr<ofFbo> > fbos;
};

#endif
