//
//  SampleBrush.cpp
//  emptyExample
//
//  Created by Akira on 5/14/15.
//
//

#include "SampleBrush.h"

void SampleBrush::draw(int brushSize)
{
    for (int i = 1; i < points.size(); i++)
    {
        ofVec3f thisPoint = points.at(i - 1);
        ofVec3f nextPoint = points.at(i);
        
        ofVec3f direction = (nextPoint - thisPoint);
        
        float len = direction.length();
        
        float thickness = ofMap(len, 0, 60, brushSize, 2, true);
        
        if (i + 1 < points.size())
        {
            if (thisPoint.distance(nextPoint) > thickness)
            {
                int steps = 20;
                
                // get next thickness
                ofVec3f nnPoint = points.at(i + 1);
                direction = (nnPoint - nextPoint);
                len = direction.length();
                float nextThickness = ofMap(len, 0, 60, brushSize, 2, true);
                
                for (int j = 0; j < steps; j++)
                {
                    ofVec3f interp = thisPoint.interpolated(nextPoint, ofMap(j, 0, steps-1, 0.0, 1.0, true));
                    float thisThickness = ofMap(j, 0, steps-1, thickness, nextThickness, true);
                    ofCircle(interp, thisThickness);
                }
            }
        }
        
        ofCircle(thisPoint, thickness);
    }
    
//    if (!points.empty())
//        ofCircle(points.back(), brushSize);
}

void SampleBrush::dragged(int x, int y)
{
    ofVec3f mousePoint(x,y,0);
    points.push_back(mousePoint);
    if (points.size() > 5)
        points.pop_front();
}

void SampleBrush::released()
{
    points.clear();
}