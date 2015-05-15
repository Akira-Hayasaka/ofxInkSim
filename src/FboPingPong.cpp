//
//  FboPingPong.cpp
//  emptyExample
//
//  Created by Akira on 5/11/15.
//
//

#include "FboPingPong.h"

void FboPingPong::allocate(ofFbo::Settings settings)
{
    readTex = 0;
    writeTex = 1;
    
    tex0 = ofPtr<ofFbo>(new ofFbo);
    tex0->allocate(settings);
    tex1 = ofPtr<ofFbo>(new ofFbo);
    tex1->allocate(settings);
    
    fbos.push_back(tex0);
    fbos.push_back(tex1);
    
    clear(0);
}

void FboPingPong::swap()
{
    int t = readTex;
    readTex = writeTex;
    writeTex = t;
}

void FboPingPong::clear(int alpha)
{
    for (auto &f : fbos)
    {
        f->begin();
        ofClear(0, alpha);
        f->end();
    }
}

ofPtr<ofFbo> FboPingPong::getOldTex()
{
    return getReadTex();
}

ofPtr<ofFbo> FboPingPong::getNewTex()
{
    return getWriteTex();
}

ofPtr<ofFbo> FboPingPong::getReadTex()
{
    return fbos.at(readTex);
}

ofPtr<ofFbo> FboPingPong::getWriteTex()
{
    return fbos.at(writeTex);
}