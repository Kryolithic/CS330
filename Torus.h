#ifndef TORUS_H
#define TORUS_H

#include"mesh.h"

class Torus
{
    //torus data
    int rSeg, cSeg;
	double r, c;
	

public:
    Torus(double r, double c, int rSeg, int cSeg)
    {
        this->rSeg = rSeg;
        this->cSeg = cSeg;
        this->r = r;
        this->c = c;
    }
       
    double getTorusRadius()
    {
        return r;
    }

    double getTorusTubeRad()
    {
        return c;
    }
    int getTorusRSegment()
    {
        return rSeg;
    }
    int getTorusCSegment()
    {
        return cSeg;
    }

};


#endif // !TORUS_H
