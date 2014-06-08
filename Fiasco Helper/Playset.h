//
//  Playset.h
//  Fiasco Helper
//
//  Created by Laura D. on 6/2/14.
//  Copyright (c) 2014 Laura Delaney. All rights reserved.
//

#ifndef __Fiasco_Helper__Playset__
#define __Fiasco_Helper__Playset__

#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>
#include "GameElement.h"

using namespace std;


//TODOs:
// Move playsetItems to private, and add a get method


class Playset
{
private:
    string filename;
    
    void displaySet(); //break into smaller parts
    void displayElement(int type); // 0 = relationship, 1 = need, 2 = object, 3 = location
    
    //a playset consists of 144 game elements, which are divided into 4 types. each of the four types has 6 sections. Each section has 1 description, and 6 elements. (1+6 = 7). This is how the [4][6][7] array is determined
    // now four([1])[6][7] arrays, for readability
    Relationship playsetItemsRelationships[6][7]; //relationships
    GameElement playsetItemsNeeds[6][7]; //non-relationships
    GameElement playsetItemsLocations[6][7]; //non-relationships
    GameElement playsetItemsObjects[6][7]; //non-relationships
    
    void load(); // loads the playset from a .txt into the arrays of game elements
    void loadBasicSetInfo(int choice); //loads the author and description
    
public:

    GameElement& getItem (int a, int b, int c); // a is the type identifier,
    Relationship& getRelationship (int b, int c);
    Playset();
    Playset(string filename);
};

#endif /* defined(__Fiasco_Helper__Playset__) */
