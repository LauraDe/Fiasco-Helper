//
//  Player.h
//  Fiasco Helper
//
//  Created by Laura D. on 6/4/14.
//  Copyright (c) 2014 Laura Delaney. All rights reserved.
//

#ifndef __Fiasco_Helper__Player__
#define __Fiasco_Helper__Player__

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Player
{
private:
public:
    string name;
    vector<string> GameElementDescriptions; //the game elements themselves are not needed, only the line of description for how it applies to this player matters.
    
};

#endif /* defined(__Fiasco_Helper__Player__) */
