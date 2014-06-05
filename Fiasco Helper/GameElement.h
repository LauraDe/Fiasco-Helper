//
//  GameElement.h
//  Fiasco Helper
//
//  Created by Laura D. on 6/2/14.
//  Copyright (c) 2014 Laura Delaney. All rights reserved.
//

#ifndef __Fiasco_Helper__GameElement__
#define __Fiasco_Helper__GameElement__

#include <iostream>
#include <string>
using namespace std;

//playsets are made up of game elements
class GameElement
{
public:
    string description;
    int type; // 0 is relationship, 1 is need, 2 is object, 3 is location
    bool usedInSetup = false; //to check if it has been used during setup
};

class Relationship : public GameElement
{
private:
public:
    bool hasDirection;
    string description1;  //first role in a relationship that has direction
    string description2; //second role in a relationship that has direction
};

#endif /* defined(__Fiasco_Helper__GameElement__) */
