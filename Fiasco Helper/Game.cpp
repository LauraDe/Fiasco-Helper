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

Game::Game(int numberOfPlayers)
{
    this -> numberOfPlayers = numberOfPlayers;
    lightDice[0] = (this -> numberOfPlayers * 2);
    darkDice[0] = (this -> numberOfPlayers * 2);
    
    Players.resize(numberOfPlayers);
    string temp;
    Players[0].name = "General";
    
    for (int i = 1; i < numberOfPlayers; ++i)
    {
        cout << "What is the name of player " << i << "?" << endl;
        cin >> temp;
        Players[i].name = temp;
    }
    
    cout << "This game has " << Game::Players.size() << " (" << numberOfPlayers << ") players." << endl;
    
}

//constructors to properly set the number of players
GameThreePlayer::GameThreePlayer() : Game(3)
{
}
GameFourPlayer::GameFourPlayer() : Game(4)
{
}
GameFivePlayer::GameFivePlayer() : Game(5)
{
}


void Game::rollAvailableDice()
{
    int temp; //stores the result of each roll so it can be counted
    
    //resets the array of numbers available
    for (int i = 0; i < 6; ++i)
    {
        Game::numbersAvailable[i] = 0;
    }
    
    //rolls all unassigned dice, and tallies the results in the numbersAvailable array
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
    void SetupMenuOfAvailableGameElements();
    cout << 3 << endl;
}

void GameFourPlayer::Setup()
{
    void SetupMenuOfAvailableGameElements();
    cout << 4 << endl;
}

void GameFivePlayer::Setup()
{
    void SetupMenuOfAvailableGameElements();
    cout << 5 << endl;
}


void Game::SetupMenuOfAvailableGameElements()
{
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
                    if (k == 0)
                    {
                      cout << playset1.getItem(i, j, k).description << endl;
                    }
                    else if (Game::numbersAvailable[k] > 0)
                    {
                      cout << playset1.getItem(i, j, k).description << endl;
                    }
                    
                }//close for k
            }//close for j
        }//close for i
}


void Game::assignNonRelationshipElement(GameElement element, int playerNumber)
{
    assert(playerNumber > 0 && playerNumber <= Game::numberOfPlayers); //make sure that the number of players is valid
    
    
    
}

