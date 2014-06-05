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
   
private:
    Dice d6; // the die that gets rolled all of the time
    
    
    // the benefits of
    int lightDice[6] = {0,0,0,0,0,0}; // will use 4, 5, or 6 spaces of the array
    int darkDice[6] = {0,0,0,0,0,0}; // same as light dice, but dark
    
    Playset playset1;
    int numbersAvailable[6]; //use in the setup and the tilt
    

    
    //used in the setup to ensure all needed game elements are chosen first
    bool AllRelationshipsChosen = false;
    bool NeedChosen = false;
    bool LocationChosen = false;
    bool ObjectChosen = false;
    
    //methods used by Setup();
    void assignNonRelationshipElement(GameElement element, int playerNumber);
    void assignRelationship(Relationship, int player1, int player2);
    void SetupMenuOfAvailableGameElements();
    
    
    
    void rollAvailableDice(); //rolled during the setup, tilt, and aftermath
    
    
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
