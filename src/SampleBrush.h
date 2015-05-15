//
//  SampleBrush.h
//  emptyExample
//
//  Created by Akira on 5/14/15.
//
//

#ifndef __emptyExample__SampleBrush__
#define __emptyExample__SampleBrush__

#include "ofMain.h"
#include "BrashIF.h"

class SampleBrush : public BrashIF
{
public:
    
    void draw(int brushSize = 10);
    void dragged(int x, int y);
    void released();
    
private:
    
    deque<ofVec3f> points;
};

#endif /* defined(__emptyExample__SampleBrush__) */
