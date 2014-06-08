//
//  main.cpp
//  Fiasco Helper
//
//  Created by Laura D. on 6/2/14.
//  Copyright (c) 2014 Laura Delaney. All rights reserved.
//



/*
 BUGS:
 can choose elements without the dice for them
 */

#include <iostream>
#include "Game.h"
#include "Dice.h"
#include <string>
#include <vector>
#include <fstream>
using namespace std;

int Welcome();

const string whereIAm = "/Users/Kittywizard/Documents/CS 171/Other/Fiasco Helper/Resources"; //fix in playset line 19

int main()
{
    switch (Welcome()) //launches a game with the number of players set in Welcome()

    {
        case 3:
        {
            GameThreePlayer newGame;
            newGame.Setup();
            break;
        }
        case 4:
        {
            GameFourPlayer newGame;
            newGame.Setup();
            break;
        }
        case 5:
        {
            GameFivePlayer newGame;
            newGame.Setup();
            break;
        }
        //having a default exit_failure case works better than an assert here. To assert I would need the result from welcome to be set
        default:
            cout << "invalid number of players received from the welcome function. Exiting now." << endl;
            exit(EXIT_FAILURE);
    }
    
    return 0;
}


// welcomes the user and returns the number of players
int Welcome()
{
    int temp = 0;
    cout << "Welcome to Fiasco Helper"  << endl << "This is a modified version of Fiasco based on how I learned to play. It has more relationships for even crazier Fiascos." << endl << endl;
    //add options to play a game or write a new playset
    
    //this is the 'I didn't make this game' disclaimer
    cout << "This program is an accessory for the Fiasco role-playing game by Bully Pulpit Games." << endl << "The playsets are copyrighted by various authors, and credit is given/shown when you view the playset information. All rights are reserved." << endl << "For more information about Fiasco or to download other playsets and materials, visit www.bullypulpitgames.com." << endl << "If you’d like to create your own playset or other Fiasco-related content, we’d like to help. Write us at info@bullypulpitgames.com." << endl << endl;
    
    cout << "Fiasco is a game for 3-5 adults." << endl << "Not all content is appropriate for children." << endl;
    
    cout << "Please enter the number of players for this game." << endl;
    
    
    while (temp < 3 || temp > 5)
    {
        if (!(cin >> temp))
        {
            cin.clear();
            cin.ignore(INT_MAX,'\n');
            cout << "Wrong format \n";
            continue;
        }
        if (temp < 3 || temp > 5)
        {
            cout << "invalid input \n";
        }
    }
    
    return temp;
}