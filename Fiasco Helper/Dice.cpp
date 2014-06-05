//
//  Dice.cpp
//  Fiasco Helper
//
//  Created by Laura D. on 6/2/14.
//  Copyright (c) 2014 Laura Delaney. All rights reserved.
//

#include "Dice.h"

Dice::Dice()
{
    srand(time(NULL));
    return;
}

int Dice::roll()
{
    int temp;
    temp = rand() % 6;
    temp += 1;
    return temp;
}