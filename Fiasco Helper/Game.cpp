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


GameElement Game::getPlaysetItem(int i, int j, int k)
{
    return playset1.getItem(i, j, k);
}

Relationship Game::getPlaysetRelationship(int j, int k)
{
    return playset1.getRelationship(j, k);
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

int Game::totalNumbersAvailable()
{
    return (numbersAvailable[0] + numbersAvailable[1] + numbersAvailable[2] + numbersAvailable[3] + numbersAvailable[4] + numbersAvailable[5]);
}

void GameThreePlayer::Setup()
{
    rollAvailableDice();
    //runs the rest of the setup process, selelcting all of the elements, and tracking the dice
    SelectElements();
    
    cout << 3 << endl;
}

void GameFourPlayer::Setup()
{
    rollAvailableDice();
    SelectElements();
    cout << 4 << endl;
}

void GameFivePlayer::Setup()
{
    
    rollAvailableDice();
    SelectElements();
    cout << 5 << endl;
}

void Game::SelectElements()
{
    int elementType;
    int subcategory;
    int elementNumber;
    int choice;
    int player1;
    int player2;
    
    do{
        Game::SetupMenuOfAvailableGameElements();
        cout << "What type of element would you like to add?" << endl;
        cout << "0) Relationship\n" << "1) Need\n" << "2) Location\n" << "3) Object" << endl;
        while ( (!(cin >> elementType)) || elementType < 0 || elementType > 3)
        {
            cin.clear();
            cin.ignore(INT_MAX,'\n');
            cout << "Wrong format \n";
            continue;
        }
        
        cout << "What is the number of the subcategory? " << "ex: " << getPlaysetItem(elementType, 2, 0).description << " is subcategory number 3" << endl;
        
        while ( (!(cin >> subcategory)) || subcategory < 1 || subcategory > 6)
        {
            cin.clear();
            cin.ignore(INT_MAX,'\n');
            cout << "Invalid choice. If you dislike your element choice from the choice before this, you can clear your selection after thhis menu set finishes \n";
            continue;
        }
        subcategory -= 1; //adjust for the fact that people count from 1 and computers count from 0
        
        cout << "Which element within this subcategory would you like to choose?" << endl;
        for (int i = 0; i < 6; ++i)
        {
            if (numbersAvailable[i] > 0 || totalNumbersAvailable() == 1) //if the element is available, display it with its number
            {
                cout << i+1 << ") " << getPlaysetItem(elementType, subcategory, i).description << endl;
            }
        }
        
        while ( (!(cin >> elementNumber)) || elementNumber < 1 || elementNumber > 6)
        {
            cin.clear();
            cin.ignore(INT_MAX,'\n');
            cout << "Invalid Choice. If you dislike your previous selection, you can clear it at the end of this menu set. \n";
            continue;
        }
        elementNumber -= 1;
    
    cout << "Do you wish choose the element " << getPlaysetItem(elementType, subcategory, elementNumber).description << endl;
    cout << "1) yes\n2) no" << endl;
    while ( (!(cin >> choice)) || choice < 1 || choice > 2)
    {
        cin.clear();
        cin.ignore(INT_MAX,'\n');
        cout << "Invalid Choice.\n";
        continue;
    }
        
    
        
    
    if (choice == 1)
    {
        // if they say "choose this element", use the die
        numbersAvailable[elementNumber] -= 1;
        
        if (elementType != 0)
        {
            cout << "Pick a player number to assign this element to. Enter 0 for a general element" << endl;
            //list the player names
            for (int i = 1; i <= numberOfPlayers; ++i)
            {
                cout << Players[i].nameCharacter << " is player number " << i << endl;
            }
            
            while ( (!(cin >> player1)) || player1 < 0 || player1 > numberOfPlayers)
            {
                cin.clear();
                cin.ignore(INT_MAX,'\n');
                cout << "Invalid Choice.\n";
                continue;
            }
            
            
            //assing the element
            assignNonRelationshipElement(getPlaysetItem(elementType, subcategory, elementNumber), player1);
            cout << Players[player1].GameElementDescriptions[Players[player1].GameElementDescriptions.size() -1] << endl;
        }
        
        else
        {
            cout << "Pick the first player in this relationship. If the relationship has direction, this will be the player with the first half of the relationship" << endl;
            //list the player names
            for (int i = 1; i <= numberOfPlayers; ++i)
            {
                cout << Players[i].nameCharacter << " is player number " << i << endl;
            }
            while ( (!(cin >> player1)) || player1 < 0 || player1 > numberOfPlayers)
            {
                cin.clear();
                cin.ignore(INT_MAX,'\n');
                cout << "Invalid Choice.\n";
                continue;
            }
            cout << "Now pick the second player in the relationship." << endl;
            while ( (!(cin >> player2)) || player2 < 0 || player2 > numberOfPlayers || player1 == player2)
            {
                cin.clear();
                cin.ignore(INT_MAX,'\n');
                cout << "Invalid Choice.\n";
                if (player1 == player2)
                {
                    cout << "must choose a different player than player1" << endl;
                }
                continue;
            }
            assignRelationship(getPlaysetRelationship(subcategory, elementNumber), player1, player2);
        }
    }
    
}while(totalNumbersAvailable() > 0);
    
}


//displays the elements of the set that can be selected based on the dice that are left.
void Game::SetupMenuOfAvailableGameElements()
{
    cout << "called menu setup list" << endl;
    
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
    
    for (int i = 0; i < 6; ++i)
    {
        cout << "There are " << numbersAvailable[i] << " dice available that rolled a " << i+1 << endl;
    }
}

//method to assign relationships (with or without direction
void Game::assignRelationship(Relationship r1, int player1, int player2)
{
    assert(player1 > 0 && player1 <= Game::numberOfPlayers); //make sure that the number of players is valid
    assert(player2 > 0 && player2 <= Game::numberOfPlayers); //make sure that the number of players is valid
    string temp;
    
    if (r1.hasDirection == true)
    {
        //SAMPLE: Eldonar is the admirer in a romantic relationship of admirer and admired with Joseph
        temp = Players[player1].nameCharacter + " is the " + r1.description1 + " in a " + r1.category.substr(2, r1.category.size() -2)+ " relationship  of "+ r1.description + " with " + Players[player2].nameCharacter;
        Players[player1].GameElementDescriptions.push_back(temp);
        
        
        //SAMPLE: Joseph is the admired in a romantic relationship of admirer and admired with Eldonar.
        temp = Players[player2].nameCharacter + " is the " + r1.description2 + " in a " + r1.category.substr(2, r1.category.size() -2)+ " relationship  of "+ r1.description + " with " + Players[player1].nameCharacter;
        Players[player1].GameElementDescriptions.push_back(temp);
        
    }
    else
    {
        temp = Players[player1].nameCharacter + " has a " + r1.category.substr(2, r1.category.size() -2)+ " relationship  of "+ r1.description + " with " + Players[player2].nameCharacter;
        Players[player1].GameElementDescriptions.push_back(temp);
        
        temp = Players[player2].nameCharacter + " has a " + r1.category.substr(2, r1.category.size() -2)+ " relationship  of "+ r1.description + " with " + Players[player1].nameCharacter;
        Players[player2].GameElementDescriptions.push_back(temp);
    }
    
}


//method to assign needs, locations, and objects
void Game::assignNonRelationshipElement(GameElement element, int playerNumber)
{
    cout << "assinging non relationship element" << endl;
    assert(playerNumber >= 0 && playerNumber <= Game::numberOfPlayers); //make sure that the number of players is valid, player number 0 is the 'general' or 'unassigned' elements that apply to all players. they are stored in a player object for general game elements.
    assert(element.type >= 1 && element.type <=3); //make sure that type is valid, type 0 is a relationship

    cout << "Got past assertations" << endl;
    cout << element.category.size() << endl;
    cout << element.category.substr(2, element.category.size() -2) << endl;
    string temp;
   
    if (playerNumber != 0)
    {
        switch (element.type)
        {
            case 1:
                temp = Players[playerNumber].nameCharacter + " has a " + "Need " + element.category.substr(2, element.category.size() -2) + " " + element.description;
                break;
            case 2:
                temp = Players[playerNumber].nameCharacter + " has a " + "Location " + element.category.substr(2, element.category.size() -2) + " " + element.description;
                break;
            case 3:
                temp = Players[playerNumber].nameCharacter + " has a " + element.category.substr(2, element.category.size() -2) + "  Object: " + element.description;
                break;
        }//close switch
    }//close if
    else
    {
        switch (element.type)
        {
            case 1:
                temp = " There is a general Need " + element.category.substr(2, element.category.size() -2) + " " + element.description;
                break;
            case 2:
                temp = " There is a general Location " + element.category.substr(2, element.category.size() -2) + " " + element.description;
                break;
            case 3:
                temp = "There is a general " + element.category.substr(2, element.category.size() -2) + " Object: " + element.description;
                break;
        }//close switch
    }//close else
    
    //add the generated line describing the game element to the player's array of game elements that affect them.
    Players[playerNumber].GameElementDescriptions.push_back(temp);
    
    
}

