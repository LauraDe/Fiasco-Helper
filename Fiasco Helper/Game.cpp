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


//setup is different based on the number of players
/*
 setup should:
 -roll all dice
 -count the number of dice in each number (kept in numbersAvailable[6])
 -display all game elements that can be chosen
 -prompt the user to choose and element, and to either assign it to a player or keep it 'general'
 -inform the user of required game elements, and check if an element would fulfill a still needed requirement.
 
 YAY! SETUP DOES ALL OF THESE THINGS NOW
 */

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

//methods for setup//


//used to guide the user to choose an element
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
        
        elementType = cinInt(3, 0, "Wrong format");
        
        cout << "What is the number of the subcategory? " << endl;
        for (int i = 0; i < 6; ++i)
        {
            cout << getPlaysetItem(elementType, i, 0).description << endl;
        }
        
        subcategory = cinInt(6, 1, "Invalid choice. If you dislike your element choice from the choice before this, you can clear your selection after this menu set finishes");
        
        subcategory -= 1; //adjust for the fact that people count from 1 and computers count from 0
        
        cout << "Which element within this subcategory would you like to choose?" << endl;
        for (int i = 0; i < 6; ++i)
        {
            if (numbersAvailable[i] > 0 || totalNumbersAvailable() == 1) //if the element is available, display it with its number
            {
                cout << i+1 << ") " << getPlaysetItem(elementType, subcategory, i).description << endl;
            }
        }
        elementNumber = cinInt(6, 1, "Invalid Choice. If you dislike your previous selection, you can clear it at the end of this menu set.");
        elementNumber -= 1; //adjust for the fact that people count from 1 and computers count from 0
        
        //if the element is not a relationship, ask to confirm their choice.
        if (getPlaysetItem(elementType, subcategory, elementNumber).type != 0)
        {
        cout << "Do you wish choose the element " << getPlaysetItem(elementType, subcategory, elementNumber).description << endl;
        cout << "1) yes\n2) no" << endl;
        choice = cinInt(2, 1, "Invalid Choice");
            
            //can skip this step if all of the needed elements have been chosen, or if they said no to the element
            if (AllNeededElementsChosen() == false && choice != 2)
            {
                //can skip this step if the element is an unchosen needed element
                if (IsNeededUnchosenElement(getPlaysetItem(elementType, subcategory, elementNumber)) == false)
                {
                    //if there are more dice left than elements needed, the user gets a warning and a second chance to rechoose their element
                    if (NumberOfNeededElementsLeftToChoose() < totalNumbersAvailable())
                    {
                        cout << "There are still unchosen elements that need to be chosen. Are you sure that you want to choose this element, knowing that it will not fulfill any of the needed elements?" << endl;
                        cout << "1) yes\n2) no" << endl;
                        choice = cinInt(2, 1, "Invalid Choice");
                    }
                    else // otherwise, they are forced to rechoose
                    {
                        choice = 2;
                        cout << "You cannot choose this element. You must choose a required element." << endl;
                    }//close else
                }//close (if is needed element)
            }//close if (not all elements are chosen)
        }//close if (not a relationship)
        else{
            cout << "Do you wish choose the element " << getPlaysetItem(elementType, subcategory, elementNumber).description << endl;
            cout << "1) yes\n2) no" << endl;
            choice = cinInt(2, 1, "Invalid Choice");
            
            if (choice == 2)
            {
                //selecting the players for the relationship has to happen before the element choice is confirmed, so it can be checked against available needed relationships
                cout << "Pick the first player in this relationship. If the relationship has direction, this will be the player with the first half of the relationship" << endl;
                //list the player names
                for (int i = 1; i <= numberOfPlayers; ++i)
                {
                    cout << Players[i].nameCharacter << " is player number " << i << endl;
                }
                
                player1 = cinInt(6, 0, "Invalid Choice");
                
                cout << "Now pick the second player in the relationship." << endl;
                //can't replace with cinInt because it also needs to not equal player 1.
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
                
                //can skip this step if all of the needed elements have been chosen
                if (AllNeededElementsChosen() == false)
                {
                    //can skip this step if the element is an unchosen needed element
                    if (IsNeededUnchosenElement(getPlaysetItem(elementType, subcategory, elementNumber), player1, player2) == false)
                    {
                        //if there are more dice left than elements needed, the user gets a warning and a second chance to rechoose their element
                        if (NumberOfNeededElementsLeftToChoose() < totalNumbersAvailable())
                        {
                            cout << "There are still unchosen elements that need to be chosen. Are you sure that you want to choose this element, knowing that it will not fulfill any of the needed elements?" << endl;
                            cout << "1) yes\n2) no" << endl;
                            choice = cinInt(2, 1, "Invalid Choice");
                        }
                        else // otherwise, they are forced to rechoose
                        {
                            choice = 2;
                            cout << "You cannot choose this element. You must choose a required element." << endl;
                        }//close else
                    }//close if is needed element
                }//close if not all elements are chosen
            }//close else
        }//close if they said no to the choice
        
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
                
                player1 = cinInt(numberOfPlayers, 0, "Invalid Choice");
                
                
                //assing the element
                assignNonRelationshipElement(getPlaysetItem(elementType, subcategory, elementNumber), player1);
                cout << Players[player1].GameElementDescriptions[Players[player1].GameElementDescriptions.size() -1] << endl;
            }
            else
            {
                assert(player1 > 0 && player1 <= numberOfPlayers);
                assert(player2 > 0 && player2 <= numberOfPlayers);
                
                //Xcode thinks that player1 and player2 could be uninitialized at this point. I do not agree with this, but have added assertations to confirm that player1 and player2 are both initialized and valid
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
    assert(playerNumber >= 0 && playerNumber <= Game::numberOfPlayers); //make sure that the number of players is valid, player number 0 is the 'general' or 'unassigned' elements that apply to all players. they are stored in a player object for general game elements.
    assert(element.type >= 1 && element.type <=3); //make sure that type is valid, type 0 is a relationship
    
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
    cout << temp << endl;
    cout << "enter anything to continue" << endl;
    cin.ignore(INT_MAX);
    cin.clear();
    getline (cin, temp);
}

//returns true if all of the needed elements have been chosen
bool GameThreePlayer::AllNeededElementsChosen()
{
    bool temp;
    temp = true;
    if (NeedChosen == false)
    {
        cout << "Still need to choose a need" << endl;
        temp = false;
    }
    if (LocationChosen == false)
    {
        cout << "Still need to choose a location" << endl;
        temp = false;
    }
    if (ObjectChosen == false)
    {
        cout << "Still need to choose an object" << endl;
        temp = false;
    }
    if (Relationship12Chosen == false)
    {
        cout << "Still need a relationship between " << Players[1].nameCharacter << " and " << Players[2].nameCharacter << endl;
        temp = false;
    }
    if (Relationship23Chosen == false)
    {
        cout << "Still need a relationship between " << Players[2].nameCharacter << " and " << Players[3].nameCharacter << endl;
        temp = false;
    }
    //here starts where this method is different for the game because of the number of players
    if (Relationship13Chosen == false)
    {
        cout << "Still need a relationship between " << Players[1].nameCharacter << " and " << Players[3].nameCharacter << endl;
        temp = false;
    }
    
    return temp;
}
bool GameFourPlayer::AllNeededElementsChosen()
{
    bool temp;
    temp = true;
    if (NeedChosen == false)
    {
        cout << "Still need to choose a need" << endl;
        temp = false;
    }
    if (LocationChosen == false)
    {
        cout << "Still need to choose a location" << endl;
        temp = false;
    }
    if (ObjectChosen == false)
    {
        cout << "Still need to choose an object" << endl;
        temp = false;
    }
    if (Relationship12Chosen == false)
    {
        cout << "Still need a relationship between " << Players[1].nameCharacter << " and " << Players[2].nameCharacter << endl;
        temp = false;
    }
    if (Relationship23Chosen == false)
    {
        cout << "Still need a relationship between " << Players[1].nameCharacter << " and " << Players[3].nameCharacter << endl;
        temp = false;
    }
    //here starts where this method is different for the game because of the number of players
    if (Relationship34Chosen == false)
    {
        cout << "Still need a relationship between " << Players[3].nameCharacter << " and " << Players[4].nameCharacter << endl;
        temp = false;
    }
    if (Relationship14Chosen == false)
    {
        cout << "Still need a relationship between " << Players[1].nameCharacter << " and " << Players[4].nameCharacter << endl;
        temp = false;
    }
    
    return temp;
}
bool GameFivePlayer::AllNeededElementsChosen()
{
    bool temp;
    temp = true;
    if (NeedChosen == false)
    {
        cout << "Still need to choose a need" << endl;
        temp = false;
    }
    
    if (LocationChosen == false)
    {
        cout << "Still need to choose a location" << endl;
        temp = false;
    }
    
    if (ObjectChosen == false)
    {
        cout << "Still need to choose an object" << endl;
        temp = false;
    }
    if (Relationship12Chosen == false)
    {
        cout << "Still need a relationship between " << Players[1].nameCharacter << " and " << Players[2].nameCharacter << endl;
        temp = false;
    }
    if (Relationship23Chosen == false)
    {
        cout << "Still need a relationship between " << Players[1].nameCharacter << " and " << Players[3].nameCharacter << endl;
        temp = false;
    }
    //here starts where this method is different for the game because of the number of players
    if (Relationship34Chosen == false)
    {
        cout << "Still need a relationship between " << Players[3].nameCharacter << " and " << Players[4].nameCharacter << endl;
        temp = false;
    }
    if (Relationship45Chosen == false)
    {
        cout << "Still need a relationship between " << Players[4].nameCharacter << " and " << Players[5].nameCharacter << endl;
        temp = false;
    }
    if (Relationship15Chosen == false)
    {
        cout << "Still need a relationship between " << Players[1].nameCharacter << " and " << Players[5].nameCharacter << endl;
        temp = false;
    }
    
    return temp;
}

//returns the number of needed elements that still need to be chosen
int GameThreePlayer::NumberOfNeededElementsLeftToChoose()
{
    int temp;
    temp = 0;
    if (NeedChosen == false)
    {
        cout << "Still need to choose a need" << endl;
        ++temp;
    }
    if (LocationChosen == false)
    {
        cout << "Still need to choose a location" << endl;
        temp++;
    }
    
    if (ObjectChosen == false)
    {
        cout << "Still need to choose an object" << endl;
        ++temp;
    }
    if (Relationship12Chosen == false)
    {
        cout << "Still need a relationship between " << Players[1].nameCharacter << " and " << Players[2].nameCharacter << endl;
        ++temp;
    }
    if (Relationship23Chosen == false)
    {
        cout << "Still need a relationship between " << Players[2].nameCharacter << " and " << Players[3].nameCharacter << endl;
        ++temp;
    }
    //here starts where this method is different for the game because of the number of players
    if (Relationship13Chosen == false)
    {
        cout << "Still need a relationship between " << Players[1].nameCharacter << " and " << Players[3].nameCharacter << endl;
        ++temp;
    }
    
    return temp;
}
int GameFourPlayer::NumberOfNeededElementsLeftToChoose()
{
    int temp;
    temp = true;
    if (NeedChosen == false)
    {
        cout << "Still need to choose a need" << endl;
        temp++;
    }
    if (LocationChosen == false)
    {
        cout << "Still need to choose a location" << endl;
        temp++;
    }
    
    if (ObjectChosen == false)
    {
        cout << "Still need to choose an object" << endl;
        temp++;
    }
    if (Relationship12Chosen == false)
    {
        cout << "Still need a relationship between " << Players[1].nameCharacter << " and " << Players[2].nameCharacter << endl;
        temp++;
    }
    if (Relationship23Chosen == false)
    {
        cout << "Still need a relationship between " << Players[1].nameCharacter << " and " << Players[3].nameCharacter << endl;
        temp++;
    }
    //here starts where this method is different for the game because of the number of players
    if (Relationship34Chosen == false)
    {
        cout << "Still need a relationship between " << Players[3].nameCharacter << " and " << Players[4].nameCharacter << endl;
        temp++;
    }
    if (Relationship14Chosen == false)
    {
        cout << "Still need a relationship between " << Players[1].nameCharacter << " and " << Players[4].nameCharacter << endl;
        temp++;
    }
    
    return temp;
}
int GameFivePlayer::NumberOfNeededElementsLeftToChoose()
{
    int temp;
    temp = true;
    if (NeedChosen == false)
    {
        cout << "Still need to choose a need" << endl;
        temp++;
    }
    if (LocationChosen == false)
    {
        cout << "Still need to choose a location" << endl;
        temp++;
    }
    if (ObjectChosen == false)
    {
        cout << "Still need to choose an object" << endl;
        temp++;
    }
    if (Relationship12Chosen == false)
    {
        cout << "Still need a relationship between " << Players[1].nameCharacter << " and " << Players[2].nameCharacter << endl;
        temp++;
    }
    if (Relationship23Chosen == false)
    {
        cout << "Still need a relationship between " << Players[1].nameCharacter << " and " << Players[3].nameCharacter << endl;
        temp++;
    }
    //here starts where this method is different for the game because of the number of players
    if (Relationship34Chosen == false)
    {
        cout << "Still need a relationship between " << Players[3].nameCharacter << " and " << Players[4].nameCharacter << endl;
        temp++;
    }
    if (Relationship45Chosen == false)
    {
        cout << "Still need a relationship between " << Players[4].nameCharacter << " and " << Players[5].nameCharacter << endl;
        temp++;
    }
    if (Relationship15Chosen == false)
    {
        cout << "Still need a relationship between " << Players[1].nameCharacter << " and " << Players[5].nameCharacter << endl;
        temp++;
    }
    
    return temp;
}

//checks if an element is a needed element that has not been chosen yet
bool GameThreePlayer::IsNeededUnchosenElement(GameElement element, int player1, int player2)
{
    bool temp = false;
    
    if (element.type == 1 && NeedChosen == false)
    {
        temp = true;
    }
    if (element.type == 2 && LocationChosen == false)
    {
        temp = true;
    }
    if (element.type == 3 && ObjectChosen == false)
    {
        temp = true;
    }
    //here starts where this method is different for the game because of the number of players
    
    if (element.type == 0)
    {
        if ( ( (player1 == 1 && player2 == 2) || (player2 == 1 && player1 == 2) ) && Relationship12Chosen == false)
        {
            temp = true;
        }
        if ( ( (player1 == 2 && player2 == 3) || (player2 == 2 && player1 == 3) ) && Relationship23Chosen == false)
        {
            temp = true;
        }
        if ( ( (player1 == 1 && player2 == 3) || (player2 == 1 && player1 == 3) ) && Relationship13Chosen == false)
        {
            temp = true;
        }
        
    }
    
    return temp;
}
bool GameFourPlayer::IsNeededUnchosenElement(GameElement element, int player1, int player2)
{
    bool temp = false;
    
    if (element.type == 1 && NeedChosen == false)
    {
        temp = true;
    }
    if (element.type == 2 && LocationChosen == false)
    {
        temp = true;
    }
    if (element.type == 3 && ObjectChosen == false)
    {
        temp = true;
    }
    //here starts where this method is different for the game because of the number of players
    if (element.type == 0)
    {
        if ( ( (player1 == 1 && player2 == 2) || (player2 == 1 && player1 == 2) ) && Relationship12Chosen == false)
        {
            temp = true;
        }
        if ( ( (player1 == 2 && player2 == 3) || (player2 == 2 && player1 == 3) ) && Relationship23Chosen == false)
        {
            temp = true;
        }
        if ( ( (player1 == 3 && player2 == 4) || (player2 == 3 && player1 == 4) ) && Relationship34Chosen == false)
        {
            temp = true;
        }
        if ( ( (player1 == 1 && player2 == 4) || (player2 == 1 && player1 == 4) ) && Relationship14Chosen == false)
        {
            temp = true;
        }
        
    }
    
    return temp;
}
bool GameFivePlayer::IsNeededUnchosenElement(GameElement element, int player1, int player2)
{
    bool temp = false;
    
    if (element.type == 1 && NeedChosen == false)
    {
        temp = true;
    }
    if (element.type == 2 && LocationChosen == false)
    {
        temp = true;
    }
    if (element.type == 3 && ObjectChosen == false)
    {
        temp = true;
    }
    //here starts where this method is different for the game because of the number of players
    if (element.type == 0)
    {
        if ( ((player1 == 1 && player2 == 2) || (player2 == 1 && player1 == 2)) && Relationship12Chosen == false)
        {
            temp = true;
        }
        if ( ((player1 == 2 && player2 == 3) || (player2 == 2 && player1 == 3)) && Relationship23Chosen == false)
        {
            temp = true;
        }
        if ( ((player1 == 3 && player2 == 4) || (player2 == 3 && player1 == 4)) && Relationship34Chosen == false)
        {
            temp = true;
        }
        if ( ((player1 == 4 && player2 == 5) || (player2 == 4 && player1 == 5)) && Relationship45Chosen == false)
        {
            temp = true;
        }
        if ( ((player1 == 1 && player2 == 5) || (player2 == 1 && player1 == 5)) && Relationship15Chosen == false)
        {
            temp = true;
        }
        
    }
    
    return temp;
}


//Utility functions
//rolls the available dice, and tallies the results in an array, used in the setup, tilt, and aftermath
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

int Game::cinInt(int upperLimit, int lowerLimit, string errorMessage)
{
    int temp;
    while ( (!(cin >> temp)) || temp < lowerLimit || temp > upperLimit)
    {
        cin.clear();
        cin.ignore(INT_MAX,'\n');
        cout << errorMessage << endl;
        continue;
    }
    return temp;
}

GameElement Game::getPlaysetItem(int i, int j, int k)
{
    return playset1.getItem(i, j, k);
}
Relationship Game::getPlaysetRelationship(int j, int k)
{
    return playset1.getRelationship(j, k);
}

int Game::totalNumbersAvailable()
{
    return (numbersAvailable[0] + numbersAvailable[1] + numbersAvailable[2] + numbersAvailable[3] + numbersAvailable[4] + numbersAvailable[5]);
}


