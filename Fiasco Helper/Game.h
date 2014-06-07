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
#include <assert.h>
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

    //the below functions are used to run the game
    virtual void Setup() = 0; // =0 means its a pure virtual function, will only be called from the subclasses, can only create subclasses
    void Round1();
    void Scene();
    void Tilt();
    void Round2();
    void Aftermath();
    
protected:
    /* PROPERTIES */
    //all games have at least three players, these players are added to the vector during construction
    vector<Player> Players;
    
    //this array is used in the setup and the tilt, keeps track of how many 1s, 2s, 3s, 4s, 5s, and 6s have been rolled and are available to be used. The total function returns the number of dice left to use
    int numbersAvailable[6];
    int totalNumbersAvailable();
    
    
    //used in the setup to ensure all needed game elements are chosen first
    //still need to be implemented
    bool NeedChosen = false;
    bool LocationChosen = false;
    bool ObjectChosen = false;
    bool Relationship12Chosen = false; //needed by all games
    bool Relationship23Chosen = false; // needed by all games
    
    
    /* METHODS */
    //default constructor is only called by its child classes. cannot create a plain 'game' object, must be a two, three, or four person game.
    Game(int numberOfPlayers);
    
    //methods used by Setup();
    void SelectElements(); //used to choose and assign elements, calls the two assign methods.
     void SetupMenuOfAvailableGameElements(); //displays all 168 elements, and displays a filler message in place of the elements which are unavailable.
    void assignRelationship(Relationship r1, int player1, int player2); //assigns relationships
    void assignNonRelationshipElement(GameElement element, int playerNumber); //assigns other game elements
    //used in the setup to ensure all needed game elements are chosen first
    //still need to be implemented
    virtual bool AllNeededElementsChosen () = 0; // =0 because it is a purely virtual function
    virtual bool IsNeededUnchosenElement(GameElement element, int player1 = 0, int player2 = 0) = 0;
    
    //UTILITY FUNCTIONS:
    void rollAvailableDice(); //rolled during the setup, tilt, and aftermath
    int cinInt(int upperLimit, int lowerLimit, string errorMessage); //used for reading in integers, sanitizing the input to prevent strings or characters from being read into an integer variable
    GameElement getPlaysetItem(int i, int j, int k); //gets a game element from the playset arrays
    Relationship getPlaysetRelationship(int j, int k); // gets a relationship from the playset array
    
private:
    Dice d6; // the die that gets rolled all of the time
    
    // the benefits of
    vector<int> lightDice; // will use 4, 5, or 6 spaces of the array
    vector<int> darkDice; // same as light dice, but dark
    
    Playset playset1; // accessed through get methods
   
};


class GameThreePlayer : public Game
{
public:
    void Setup();
    GameThreePlayer();
    bool AllNeededElementsChosen ();
    bool IsNeededUnchosenElement(GameElement element, int player1 = 0, int player2 = 0);
private:
    bool Relationship13Chosen = false;
};

class GameFourPlayer : public Game
{
public:
    void Setup();
    GameFourPlayer();
    bool AllNeededElementsChosen ();
    bool IsNeededUnchosenElement(GameElement element, int player1 = 0, int player2 = 0);
private:
    Player player4;
    bool Relationship34Chosen = false;
    bool Relationship14Chosen = false;
    
};

class GameFivePlayer : public Game
{
public:
    GameFivePlayer();
    void Setup();
    bool AllNeededElementsChosen ();
    bool IsNeededUnchosenElement(GameElement element, int player1 = 0, int player2 = 0);
private:
    Player player4;
    Player player5;
    bool Relationship34Chosen = false;
    bool Relationship45Chosen = false;
    bool Relationship15Chosen = false;
    

};



#endif /* defined(__Fiasco_Helper__Game__) */
