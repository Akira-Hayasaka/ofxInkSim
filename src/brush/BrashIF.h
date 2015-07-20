//
//  BrashIF.h
//  emptyExample
//
//  Created by Akira on 5/14/15.
//
//

#ifndef __emptyExample__BrashIF__
#define __emptyExample__BrashIF__

#include "ofMain.h"

class BrashIF
{
public:
    
    virtual void draw(int thickness = 10) = 0;
    virtual void dragged(int x, int y) = 0;
    virtual void released() = 0;
};

#endif /* defined(__emptyExample__BrashIF__) */
