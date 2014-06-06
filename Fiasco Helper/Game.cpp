//
//  Game.cpp
//  Fiasco Helper
//
//  Created by Laura D. on 6/2/14.
//  Copyright (c) 2014 Laura Delaney. All rights reserved.
//

#include "Game.h"
#include <assert.h>
/*
 TODO:
 void Round1();
 void Scene();
 void Tilt();
 void Round2();
 void Aftermath();
 
;
 void assignRelationship(Relationship, int player1, int player2);
 
 
 NOTES:
 The playset is loaded into the arrays of game elements BEFORE the Game constructor is called.
 
 */


//initializes the dice vectors, the vector of players, and the player names
//NOTE: the constructor is called AFTER a playset is chosen
Game::Game(int numberOfPlayers)
{
    this -> numberOfPlayers = numberOfPlayers;
    
    lightDice.resize(this -> numberOfPlayers +1);
    darkDice.resize(this -> numberOfPlayers +1);
    lightDice[0] = (this -> numberOfPlayers * 2);
    darkDice[0] = (this -> numberOfPlayers * 2);
    
    Players.resize(this -> numberOfPlayers +1);
    string temp;
    Players[0].nameReal = "Unassigned";
    Players[0].nameCharacter = "General";
    for (int i = 1; i <= numberOfPlayers; ++i)
    {
        cout << "Who is player " << i << "?" << endl;
        cin >> temp;
        Players[i].nameReal = temp;
        cout << "And what is their character name?" << endl;
        cin >> temp;
        Players[i].nameCharacter = temp;
    }
    
    cout << "This game has " << Game::Players.size() -1 << " (" << numberOfPlayers << ") players." << endl;
    
}

//constructors for each subclass call the parent class constructor with an integer referring to their number of players
GameThreePlayer::GameThreePlayer() : Game(3)
{
}
GameFourPlayer::GameFourPlayer() : Game(4)
{
}
GameFivePlayer::GameFivePlayer() : Game(5)
{
}


//rolls the available dice, and tallies the results in an array
void Game::rollAvailableDice()
{
    int temp; //stores the result of each roll so it can be counted
    
    //resets the array of numbers available
    for (int i = 0; i < 6; ++i)
    {
        Game::numbersAvailable[i] = 0;
    }
    
    //rolls all unassigned dice, and tallies the results (number of each # 1-6 rolled) in the numbersAvailable array
    for (int i = 0; i < (Game::lightDice[0] + Game::darkDice[0]); ++i)
    {
        temp = d6.roll();
        
        if ( temp >= 1 && temp <= 6)
        {
            Game::numbersAvailable[temp-1] += 1;
        }
        else
        {
            cout << "Rolled a d6 and got a number that was not 1-6" << endl;
        }
    }
}


//setup is different based on the number of players
/*
 setup should:
 -roll all dice
    -count the number of dice in each number (kept in numbersAvailable[6])
 -display all game elements that can be chosen
 -prompt the user to choose and element, and to either assign it to a player or keep it 'general'
 -
 */


void GameThreePlayer::Setup()
{
    Game::rollAvailableDice();
    Game::SetupMenuOfAvailableGameElements();
    cout << 3 << endl;
}

void GameFourPlayer::Setup()
{
    Game::rollAvailableDice();
    Game::SetupMenuOfAvailableGameElements();
    cout << 4 << endl;
}

void GameFivePlayer::Setup()
{
    Game::rollAvailableDice();
    Game::SetupMenuOfAvailableGameElements();
    cout << 5 << endl;
}

//displays the elements of the set that can be selected based on the dice that are left.
void Game::SetupMenuOfAvailableGameElements()
{
    cout << "called menu setup list" << endl;
    for (int i = 0; i < 6; ++i)
    {
        cout << "There are " << numbersAvailable[i] << " dice available that rolled a " << i+1 << endl;
    }
    
    
        for (int i = 0; i < 4; ++i)
        {
            switch (i)
            {
                case 0:
                    cout << endl << endl << "RELATIONSHIPS" << endl;
                    break;
                case 1:
                    cout << endl << endl << "NEEDS" << endl;
                    break;
                case 2:
                    cout << endl << endl << "LOCATIONS" << endl;
                    break;
                case 3:
                    cout << endl << endl << "OBJECTS" << endl;
                    break;
                default:
                    cout << "invalid for loop in setup menu" << endl;
                    exit(EXIT_FAILURE);
            }
            
            for (int j = 0; j < 6; ++j)
            {
                for (int k = 0; k < 7; ++ k)
                {
                    if (k != 0)
                    {
                        cout << k << " ";
                    }
                    
                    
                    if (k == 0)
                    {
                      cout << endl << playset1.getItem(i, j, k).description << endl;
                    }
                    else if (Game::numbersAvailable[k-1] > 0) //k-1 because the arrays are offset. element '1' in the getItem call is based off of the number of dice left in element '0' in the numbersAvailable array.
                    {
                      cout << playset1.getItem(i, j, k).description << endl;
                    }
                    //if it is the last die, all elements should be displayed
                    else if (numbersAvailable[0] + numbersAvailable[1] +numbersAvailable[2] +numbersAvailable[3] + numbersAvailable[4] +numbersAvailable[5] == 1)
                    {
                        cout << playset1.getItem(i, j, k).description << endl; //breakpoint here so I can make sure that it is being called correctly
                    }
                    else{
                        cout << "There are none of this element left" << endl;
                    }
                }//close for k
            }//close for j
        }//close for i
}


void Game::assignNonRelationshipElement(GameElement element, int playerNumber)
{
    assert(playerNumber > 0 && playerNumber <= Game::numberOfPlayers); //make sure that the number of players is valid
    assert(element.type >= 1 && element.type <=3); //make sure that type is valid, type 0 is a relationship

    string temp;
   
    if (playerNumber != 0)
    {
        switch (element.type)
        {
            case 1:
                temp = Players[playerNumber].nameCharacter + " has a " + "Need " + element.category.substr(2, element.category.size() -2) + element.description;
                break;
            case 2:
                temp = Players[playerNumber].nameCharacter + " has a " + "Location " + element.category.substr(2, element.category.size() -2) + element.description;
                break;
            case 3:
                temp = Players[playerNumber].nameCharacter + " has a " + element.category.substr(2, element.   category.size() -2) + " Object: " + element.description;
                break;
        }//close switch
    }//close if
    else
    {
        switch (element.type)
        {
            case 1:
                temp = " There is a general Need " + element.category.substr(2, element.category.size() -2) + element.description;
                break;
            case 2:
                temp = " There is a general Location " + element.category.substr(2, element.category.size() -2) + element.description;
                break;
            case 3:
                temp = "There is a general" + element.category.substr(2, element.   category.size() -2) + " Object: " + element.description;
                break;
        }//close switch
    }//close else
    
    //add the generated line describing the game element to the player's array of game elements that affect them.
    Players[playerNumber].GameElementDescriptions.push_back(temp);
    
    
}

