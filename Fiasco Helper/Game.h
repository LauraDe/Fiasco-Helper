//
//  Game.h
//  Fiasco Helper
//
//  Created by Laura D. on 6/2/14.
//  Copyright (c) 2014 Laura Delaney. All rights reserved.
//

#ifndef __Fiasco_Helper__Game__
#define __Fiasco_Helper__Game__

#include <iostream>
#include <fstream>
#include "Dice.h"
#include "Playset.h"
#include "Player.h"
#include "GameElement.h"

using namespace std;

//consider making subclasses of game for 3, 4, and 5 players


class Game
{

public:
    int numberOfPlayers; // 3, 4, or 5
    
    
    //these arrays track the dice by the player number they belong to. player 0 is for unassigned/general dice.

    virtual void Setup() = 0; // =0 means its a pure virtual function, will only be called from the subclasses, can only create subclasses
    void Round1();
    void Scene();
    void Tilt();
    void Round2();
    void Aftermath();
    
protected:
    Game(int numberOfPlayers);
    
    //all games have at least three players, the games with more players will add them during construction
    vector<Player> Players;
    
    int numbersAvailable[6]; //use in the setup and the tilt, keeps track of how many 1s, 2s, 3s, 4s, 5s, and 6s have been rolled and are available to be used.
    
    int totalNumbersAvailable();
    
    //methods used by Setup();
    void assignNonRelationshipElement(GameElement element, int playerNumber);
    void assignRelationship(Relationship r1, int player1, int player2);
    void SetupMenuOfAvailableGameElements();
    void SelectElements();
    int cinInt(int upperLimit, int lowerLimit, string errorMessage); //used for reading in integers, sanitizing the input to prevent strings or characters from being read into an integer variable
    
    void rollAvailableDice(); //rolled during the setup, tilt, and aftermath
    
    GameElement getPlaysetItem(int i, int j, int k);
    Relationship getPlaysetRelationship(int j, int k);
private:
    Dice d6; // the die that gets rolled all of the time
    
    // the benefits of
    vector<int> lightDice; // will use 4, 5, or 6 spaces of the array
    vector<int> darkDice; // same as light dice, but dark
    
    Playset playset1;


    
    //used in the setup to ensure all needed game elements are chosen first
    bool AllRelationshipsChosen = false;
    bool NeedChosen = false;
    bool LocationChosen = false;
    bool ObjectChosen = false;
    
};


class GameThreePlayer : public Game
{
public:
    void Setup();
    GameThreePlayer();
    
private:

};

class GameFourPlayer : public Game
{
public:
    void Setup();
    GameFourPlayer();
private:
    Player player4;
    
};

class GameFivePlayer : public Game
{
private:
    Player player4;
    Player player5;
    
public:
    
    GameFivePlayer();
    void Setup();
};



#endif /* defined(__Fiasco_Helper__Game__) */
