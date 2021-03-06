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
 BUGS:
 assigning dice to other is handled incorrectly
 1
 */

//initializes the dice vectors, the vector of players, and the player names
//NOTE: the constructor is called AFTER a playset is chosen
Game::Game(int numberOfPlayers)
{
    this -> numberOfPlayers = numberOfPlayers;
    //set the vectors of dice to be the of a size on more than the number of players (players plus unassigned dice), and initialize all of the counts of dice to 0
    lightDice.resize(this ->numberOfPlayers +1, 0);
    darkDice.resize(this -> numberOfPlayers +1 , 0);
    
    //set the number of unassigned light dice and unassigned dark dice to be equal to two times the number of players. The total number of dice will be four times the number of players
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
        //this section prompts the user to input which element they would like to add
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
                cout << i+1 << ") " << getPlaysetItem(elementType, subcategory, i+1).description << endl;
            }
        }
        
        do{
        elementNumber = cinInt(6, 1, "Invalid Choice. If you dislike your previous selection, you can clear it at the end of this menu set.");
            if (numbersAvailable[elementNumber-1] < 1 && totalNumbersAvailable() != 1)
            {
                cout << "That number is unavailable" << endl;
            }
        }while (numbersAvailable[elementNumber-1] < 1 && totalNumbersAvailable() != 1);
        
        //element does not get decremented, because there is a category title in element 0.
        
        //if the element is not a relationship, ask to confirm their choice.
        if (getPlaysetItem(elementType, subcategory, elementNumber).type != 0)
        {
            cout << "Do you wish choose the element " << getPlaysetItem(elementType, subcategory, elementNumber).description << endl;
            cout << "1) yes\n2) no" << endl;
            choice = cinInt(2, 1, "Invalid Choice");
        
            if (choice != 2) // if they choose the element, have them pick the player number, so it can be checked to see if it is a needed element
            {
                if (elementType != 0)
                {
                    cout << "Pick a player number to assign this element to. Enter 0 for a general element" << endl;
                    //list the player names
                    for (int i = 1; i <= numberOfPlayers; ++i)
                    {
                        cout << Players[i].nameCharacter << " is player number " << i << endl;
                    }
                    
                    player1 = cinInt(numberOfPlayers, 0, "Invalid Choice");
            }
            }
            
            //can skip this step if all of the needed elements have been chosen, or if they said no to the element
            if (AllNeededElementsChosen() == false && choice != 2)
            {
                //can skip this step if the element is an unchosen needed element
                assert(player1 >= 0 && player1 <= numberOfPlayers); //program thinks player1 could be unititialized, I don't. Therefore using an assertation to confirm this.
                
                if (IsNeededUnchosenElement(getPlaysetItem(elementType, subcategory, elementNumber), player1) == false)
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
            
            if (choice == 1) //if they
            {
                //selecting the players for the relationship has to happen before the element choice is confirmed, so it can be checked against available needed relationships
                cout << "Pick the first player in this relationship. If the relationship has direction, this will be the player with the first half of the relationship" << endl;
                //list the player names
                for (int i = 1; i <= numberOfPlayers; ++i)
                {
                    cout << Players[i].nameCharacter << " is player number " << i << endl;
                }
                
                player1 = cinInt(6, 1, "Invalid Choice");
                
                cout << "Now pick the second player in the relationship." << endl;
                //can't replace with cinInt because it also needs to not equal player 1.
                while ( (!(cin >> player2)) || player2 < 1 || player2 > numberOfPlayers || player1 == player2)
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
            numbersAvailable[elementNumber-1] -= 1;
            
            if (elementType != 0)
            {
                //player number is set earlier
                
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
                if (k == 0)
                {
                    cout << endl << playset1.getItem(i, j, k).description << endl;
                }
                else if (Game::numbersAvailable[k-1] > 0) //k-1 because the arrays are offset. element '1' in the getItem call is based off of the number of dice left in element '0' in the numbersAvailable array.
                {
                    cout << k << " ";
                    cout << playset1.getItem(i, j, k).description << endl;
                }
                //if it is the last die, all elements should be displayed
                else if (numbersAvailable[0] + numbersAvailable[1] +numbersAvailable[2] +numbersAvailable[3] + numbersAvailable[4] +numbersAvailable[5] == 1)
                {
                    cout << k << " ";
                    cout << playset1.getItem(i, j, k).description << endl; //breakpoint here so I can make sure that it is being called correctly
                }
                else{
                    //cout << "There are none of this element left" << endl;
                    //
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
    
    //mark that the element has been chosen, if it is a needed element
    MarkNeededElementChosen(r1, player1, player2);
    
    if (r1.hasDirection == true)
    {
        //SAMPLE: Eldonar is the admirer in a romantic relationship of admirer and admired with Joseph
        temp = Players[player1].nameCharacter + " is the " + r1.description1 + " in a " + r1.category.substr(2, r1.category.size() -2)+ " relationship of "+ r1.description + " with " + Players[player2].nameCharacter;
        Players[player1].GameElementDescriptions.push_back(temp);
        cout << temp << endl;
        
        //SAMPLE: Joseph is the admired in a romantic relationship of admirer and admired with Eldonar.
        temp = Players[player2].nameCharacter + " is the " + r1.description2 + " in a " + r1.category.substr(2, r1.category.size() -2)+ " relationship of "+ r1.description + " with " + Players[player1].nameCharacter;
        Players[player1].GameElementDescriptions.push_back(temp);
        cout << temp << endl;
        
    }
    else
    {
        temp = Players[player1].nameCharacter + " has a " + r1.category.substr(2, r1.category.size() -2)+ " relationship of "+ r1.description + " with " + Players[player2].nameCharacter;
        Players[player1].GameElementDescriptions.push_back(temp);
        cout << temp << endl;
        
        temp = Players[player2].nameCharacter + " has a " + r1.category.substr(2, r1.category.size() -2)+ " relationship of "+ r1.description + " with " + Players[player1].nameCharacter;
        Players[player2].GameElementDescriptions.push_back(temp);
        cout << temp << endl;
    }
    
    
    cout << "enter anything to continue" << endl;
    cin.ignore(INT_MAX,'\n');
    cin.get();
    
}
//method to assign needs, locations, and objects
void Game::assignNonRelationshipElement(GameElement element, int playerNumber)
{
    assert(playerNumber >= 0 && playerNumber <= Game::numberOfPlayers); //make sure that the number of players is valid, player number 0 is the 'general' or 'unassigned' elements that apply to all players. they are stored in a player object for general game elements.
    assert(element.type >= 1 && element.type <=3); //make sure that type is valid, type 0 is a relationship
    
    string temp;
    //mark that the element has been chosen
    MarkNeededElementChosen(element, playerNumber);
    
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
    cin.ignore(INT_MAX,'\n');
    cin.get();
    
}

//returns true if all of the needed elements have been chosen
bool GameThreePlayer::AllNeededElementsChosen()
{
    bool temp;
    temp = true;
    if (NeedChosen == false)
    {
        cout << "Still need to choose a general need" << endl;
        temp = false;
    }
    if (LocationChosen == false)
    {
        cout << "Still need to choose a general location" << endl;
        temp = false;
    }
    if (ObjectChosen == false)
    {
        cout << "Still need to choose a general object" << endl;
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
        cout << "Still need to choose a general need" << endl;
        temp = false;
    }
    if (LocationChosen == false)
    {
        cout << "Still need to choose a general location" << endl;
        temp = false;
    }
    if (ObjectChosen == false)
    {
        cout << "Still need to choose a general object" << endl;
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
        cout << "Still need to choose a general need" << endl;
        temp = false;
    }
    
    if (LocationChosen == false)
    {
        cout << "Still need to choose a general location" << endl;
        temp = false;
    }
    
    if (ObjectChosen == false)
    {
        cout << "Still need to choose a general object" << endl;
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
        //cout << "Still need to choose a general need" << endl;
        ++temp;
    }
    if (LocationChosen == false)
    {
       // cout << "Still need to choose a general location" << endl;
        temp++;
    }
    
    if (ObjectChosen == false)
    {
        //cout << "Still need to choose a general object" << endl;
        ++temp;
    }
    if (Relationship12Chosen == false)
    {
        //cout << "Still need a relationship between " << Players[1].nameCharacter << " and " << Players[2].nameCharacter << endl;
        ++temp;
    }
    if (Relationship23Chosen == false)
    {
        //cout << "Still need a relationship between " << Players[2].nameCharacter << " and " << Players[3].nameCharacter << endl;
        ++temp;
    }
    //here starts where this method is different for the game because of the number of players
    if (Relationship13Chosen == false)
    {
        //cout << "Still need a relationship between " << Players[1].nameCharacter << " and " << Players[3].nameCharacter << endl;
        ++temp;
    }
    
    return temp;
}
int GameFourPlayer::NumberOfNeededElementsLeftToChoose()
{
    int temp;
    temp = 0;
    if (NeedChosen == false)
    {
       // cout << "Still need to choose a general need" << endl;
        temp++;
    }
    if (LocationChosen == false)
    {
       // cout << "Still need to choose a general location" << endl;
        temp++;
    }
    
    if (ObjectChosen == false)
    {
       // cout << "Still need to choose a general object" << endl;
        temp++;
    }
    if (Relationship12Chosen == false)
    {
        //cout << "Still need a relationship between " << Players[1].nameCharacter << " and " << Players[2].nameCharacter << endl;
        temp++;
    }
    if (Relationship23Chosen == false)
    {
       // cout << "Still need a relationship between " << Players[1].nameCharacter << " and " << Players[3].nameCharacter << endl;
        temp++;
    }
    //here starts where this method is different for the game because of the number of players
    if (Relationship34Chosen == false)
    {
        //cout << "Still need a relationship between " << Players[3].nameCharacter << " and " << Players[4].nameCharacter << endl;
        temp++;
    }
    if (Relationship14Chosen == false)
    {
       // cout << "Still need a relationship between " << Players[1].nameCharacter << " and " << Players[4].nameCharacter << endl;
        temp++;
    }
    
    return temp;
}
int GameFivePlayer::NumberOfNeededElementsLeftToChoose()
{
    int temp;
    temp = 0;
    if (NeedChosen == false)
    {
        //cout << "Still need to choose a need" << endl;
        temp++;
    }
    if (LocationChosen == false)
    {
        //cout << "Still need to choose a location" << endl;
        temp++;
    }
    if (ObjectChosen == false)
    {
       // cout << "Still need to choose an object" << endl;
        temp++;
    }
    if (Relationship12Chosen == false)
    {
        //cout << "Still need a relationship between " << Players[1].nameCharacter << " and " << Players[2].nameCharacter << endl;
        temp++;
    }
    if (Relationship23Chosen == false)
    {
        //cout << "Still need a relationship between " << Players[1].nameCharacter << " and " << Players[3].nameCharacter << endl;
        temp++;
    }
    //here starts where this method is different for the game because of the number of players
    if (Relationship34Chosen == false)
    {
        //cout << "Still need a relationship between " << Players[3].nameCharacter << " and " << Players[4].nameCharacter << endl;
        temp++;
    }
    if (Relationship45Chosen == false)
    {
        //cout << "Still need a relationship between " << Players[4].nameCharacter << " and " << Players[5].nameCharacter << endl;
        temp++;
    }
    if (Relationship15Chosen == false)
    {
        //cout << "Still need a relationship between " << Players[1].nameCharacter << " and " << Players[5].nameCharacter << endl;
        temp++;
    }
    
    return temp;
}

//checks if an element is a needed element that has not been chosen yet
bool GameThreePlayer::IsNeededUnchosenElement(GameElement element, int player1, int player2)
{
    bool temp = false;
    
    if (element.type == 1 && NeedChosen == false && player1 == 0)
    {
        temp = true;
    }
    if (element.type == 2 && LocationChosen == false && player1 == 0)
    {
        temp = true;
    }
    if (element.type == 3 && ObjectChosen == false && player1 == 0)
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
    
    if (element.type == 1 && NeedChosen == false && player1 == 0)
    {
        temp = true;
    }
    if (element.type == 2 && LocationChosen == false && player1 == 0)
    {
        temp = true;
    }
    if (element.type == 3 && ObjectChosen == false && player1 == 0)
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
    
    if (element.type == 1 && NeedChosen == false && player1 == 0)
    {
        temp = true;
    }
    if (element.type == 2 && LocationChosen == false && player1 == 0)
    {
        temp = true;
    }
    if (element.type == 3 && ObjectChosen == false && player1 == 0)
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

void GameThreePlayer::MarkNeededElementChosen(GameElement element, int player1, int player2)
{
    if (element.type == 1 && NeedChosen == false && player1 == 0)
    {
        NeedChosen = true;
    }
    if (element.type == 2 && LocationChosen == false && player1 == 0)
    {
        LocationChosen = true;
    }
    if (element.type == 3 && ObjectChosen == false && player1 == 0)
    {
        ObjectChosen = true;
    }
    //here starts where this method is different for the game because of the number of players
    
    if (element.type == 0)
    {
        if ( ( (player1 == 1 && player2 == 2) || (player2 == 1 && player1 == 2) ) && Relationship12Chosen == false)
        {
            Relationship12Chosen = true;
        }
        if ( ( (player1 == 2 && player2 == 3) || (player2 == 2 && player1 == 3) ) && Relationship23Chosen == false)
        {
            Relationship23Chosen = true;
        }
        if ( ( (player1 == 1 && player2 == 3) || (player2 == 1 && player1 == 3) ) && Relationship13Chosen == false)
        {
            Relationship13Chosen = true;
        }
    }
    
    return;
}
void GameFourPlayer::MarkNeededElementChosen(GameElement element, int player1, int player2)
{
    
    if (element.type == 1 && NeedChosen == false)
    {
        NeedChosen = true;
    }
    if (element.type == 2 && LocationChosen == false)
    {
        LocationChosen = true;
    }
    if (element.type == 3 && ObjectChosen == false)
    {
        ObjectChosen = true;
    }
    //here starts where this method is different for the game because of the number of players
    if (element.type == 0)
    {
        if ( ( (player1 == 1 && player2 == 2) || (player2 == 1 && player1 == 2) ) && Relationship12Chosen == false)
        {
            Relationship12Chosen = true;
        }
        if ( ( (player1 == 2 && player2 == 3) || (player2 == 2 && player1 == 3) ) && Relationship23Chosen == false)
        {
            Relationship23Chosen = true;
        }
        if ( ( (player1 == 3 && player2 == 4) || (player2 == 3 && player1 == 4) ) && Relationship34Chosen == false)
        {
            Relationship34Chosen = true;
        }
        if ( ( (player1 == 1 && player2 == 4) || (player2 == 1 && player1 == 4) ) && Relationship14Chosen == false)
        {
            Relationship14Chosen = true;
        }
    }
    
    return;
}
void GameFivePlayer::MarkNeededElementChosen(GameElement element, int player1, int player2)
{
    
    if (element.type == 1 && NeedChosen == false)
    {
        NeedChosen = true;
    }
    if (element.type == 2 && LocationChosen == false)
    {
        LocationChosen = true;
    }
    if (element.type == 3 && ObjectChosen == false)
    {
        ObjectChosen = true;
    }
    //here starts where this method is different for the game because of the number of players
    if (element.type == 0)
    {
        if ( ((player1 == 1 && player2 == 2) || (player2 == 1 && player1 == 2)) && Relationship12Chosen == false)
        {
            Relationship12Chosen = true;
        }
        if ( ((player1 == 2 && player2 == 3) || (player2 == 2 && player1 == 3)) && Relationship23Chosen == false)
        {
            Relationship23Chosen = true;
        }
        if ( ((player1 == 3 && player2 == 4) || (player2 == 3 && player1 == 4)) && Relationship34Chosen == false)
        {
            Relationship34Chosen = true;
        }
        if ( ((player1 == 4 && player2 == 5) || (player2 == 4 && player1 == 5)) && Relationship45Chosen == false)
        {
            Relationship45Chosen = true;
        }
        if ( ((player1 == 1 && player2 == 5) || (player2 == 1 && player1 == 5)) && Relationship15Chosen == false)
        {
            Relationship15Chosen = true;
        }
    }
    return;

}
/*END OF SETUP METHODS*/

/*
 Round 1, and Round 2 need:
 to have scenes (2 per player)
 in a set order
 to assign light and dark dice to players
 to say how many light and how many dark dice are left
 
 Round 1 needs:
 to assign dice to other players
 
 Round 2 needs:
 the same player to keep the dice
 
 scenes need:
 to establish or resolve
 to remind the player whose scene it is of their list of game elements, as well as the general game elements
 tell the round whether a dark die or light die was used
 */

void Game::Round1()
{
    int choice; // used to assign the die to another player
    bool LightOrDark; // true is light, false is dark
    
    //set the order of who will do scenes first, second, third, etc. This will remain constant for the whole game
    SetTurnRotationOrder();
    
    //run a scene for each player in order, then another scene for each player in order
    for (int i = 0; i < (2 * numberOfPlayers); ++i)
    {
        LightOrDark = Scene((playerTurnOrder[i % numberOfPlayers]));
        cout << "Because this is Round 1, you give your die to another player. Which player would you like to give your die to?" << endl;
        //display the current dice distribution
        for (int j = 1; j <= numberOfPlayers; ++j) // should loop through 1, 2, 3, (4), (5),
        {
            //used for debugging
            if (j == playerTurnOrder[i % numberOfPlayers])
            {
                cout << "j = " << j << " which is equal to playerTurnOrder " << i << " % " << numberOfPlayers << endl;
            }
            
            //print the player number, name, and number of each dice that they have
            if (j != playerTurnOrder[i % numberOfPlayers])
            {
                cout << playerTurnOrder[j % numberOfPlayers] << " " << Players[playerTurnOrder[j -1]].nameCharacter << " currently has " << lightDice[j] << " light dice and " << darkDice[j] << " dark dice " << endl;
            }
        }
        //read in user input until a valid choice is made
        do{
            choice = cinInt(6, 1, "Invalid choice");
            if (choice == playerTurnOrder[i % numberOfPlayers])
            {
                cout << "You cannot give yourself the die" << endl;
            }
        }while (choice == playerTurnOrder[i % numberOfPlayers]);
        
        
        //assign the die accordingly
        //NOTE: do not need to worry about the last die wild card in round1. Do need to worry about it in round two.
        if (LightOrDark == true)
        {
            lightDice[0]--;
            lightDice[choice]++;
        }
        else
        {
            darkDice[0]--;
            darkDice[choice]++;
        }
    }
    
}

void Game::Round2()
{
    bool LightOrDark; // true is light, false is dark
    for (int i = 0; i < (2 * numberOfPlayers); ++i)
    {
        LightOrDark = Scene((playerTurnOrder[i % numberOfPlayers]));
        cout << "Because this is Round 2, you keep your die?" << endl;
        //assign the die accordingly
        //NOTE: do not need to worry about the last die wild card in round1. Do need to worry about it in round two.
        
        if (LightOrDark == true)
        {
            if (lightDice[0] > 0)
            {
                lightDice[0]--;
            }
            lightDice[i % numberOfPlayers]++;
        }
        else
        {
            if (darkDice[0] > 0)
            {
                darkDice[0]--;
            }
            darkDice[i % numberOfPlayers]++;
        }
        
        
        //display the current dice distribution
        for (int j = 1; j <= numberOfPlayers; ++j) // should loop through 1, 2, 3, (4), (5),
        {
                cout << playerTurnOrder[j % numberOfPlayers] << " " << Players[playerTurnOrder[j -1]].nameCharacter << " currently has " << lightDice[j] << " light dice and " << darkDice[j] << " dark dice " << endl;
        }
    }
    
}


/* Methods Called by Round1 and Round 2*/
//sets the order of who will do scenes first, second, third, etc. This will remain constant for the whole game
void Game::SetTurnRotationOrder()
{
    
    //prompt the users to set the turn rotation
    cout << "The player in position 1 (different than player number 1) will go first. The player in position two will go second, and so on and so forth." << endl;
    //looping through as many times as there are players to assign, assign the players
    for (int i = 0; i < numberOfPlayers; ++i)
    {
        cout << "Choose a player for postiion "<< i+1 << endl;
        
        for (int j = 0; j < numberOfPlayers; ++j)
        {
            //if the player does not have a spot in the turn rotation, display their name and number
            if (Players[j+1].turnRotationSet == false)
            {
                cout << j+1 << ") " << Players[j+1].nameCharacter << endl;
            }
        }
        
        //input the choice, making sure that the player has not been chosen yet
        int temp;
        bool isSameCheck;
        do{
            isSameCheck = false;
            //directly below is where their choice is read in, via the cinInt function that sanitizes the input.
            temp = cinInt(numberOfPlayers, 1, "Invalid Choice");
            for (int l = 0; l < playerTurnOrder.size(); ++l)
            {
                if (playerTurnOrder[l] == temp)
                {
                    isSameCheck = true;
                    cout << "That player has been chosen. Please choose another player." << endl;
                }
            }
        }while (isSameCheck == true);
        
        //once a valid choice has been made, add the player's number to the turn order vector, and set the flag for the player that says that they are in the turn rotation. This will keep them from being in the rotation twice.
        playerTurnOrder.push_back(temp);
        Players[temp].turnRotationSet = true;
    }
}

//displays the game elements relevant to the scene, prompts the user to choose a light or dark die, forces their choices if there is only one color of dice left, but honors the last dice wildcard. Passes back the color chosen.
bool Game::Scene(int playerNumber)
{
    bool Establishing; //true is establish, false is resolve, //may not end up needing this variable
    bool LightorDark; //true is light, false is dark
    
    //assert that a valid player number was passed through
    assert (playerNumber > 0 && playerNumber <= numberOfPlayers);
    
    //display the general need(s), location(s) and object(s)
    for (int i = 0; i < Players[0].GameElementDescriptions.size(); ++i)
    {
        cout << Players[0].GameElementDescriptions[i] << endl;
    }
    //display descriptions of the game elements that affect the player in question
    for (int i = 0; i < Players[playerNumber].GameElementDescriptions.size(); ++i)
    {
        cout << Players[playerNumber].GameElementDescriptions[i] << endl;
    }
    
    //tell the players how many of each type of dice are left
    cout << "There are " << totalLightLeft() << " light dice left and " << totalDarkLeft() << " dark dice left" << endl;
    
   
    cout << Players[playerNumber].nameReal << " Choose one of the following for " << Players[playerNumber].nameCharacter << ":\n" << "1) Establish\n" <<"2) Resolve" << endl; //string broken up into multiple strings for readability
    if (cinInt(2, 1, "Invalid Choice") == 1)
    {
        //if establishing
        Establishing = true;
        if ( (darkDice[0] > 0 && lightDice[0] > 0) || (darkDice[0] + lightDice[0] == 1))
        {
            cout << "How did the scene go? " << endl;
            cout << "Did this end well or poorly?\n" << "1) Well (light die)\n" <<"2)Poorly (dark die)" << endl;
        
            if (cinInt(2, 1, "Invalid Choice") == 1)
            {
                LightorDark = true;
            }
            else
            {
                LightorDark = false;
            }
        }
        else if (darkDice[0] == 0)
        {
            LightorDark = true;
            cout << "This must end well, because there are only light dice left" << endl;
        }
        else
        {
            LightorDark = false;
            cout << "This must end poorly, because there are only dark dice left" << endl;
        }
    }
    else
    {
        //if not establishing, are resolving

        Establishing = false;
        
        
        if ( (darkDice[0] > 0 && lightDice[0] > 0) || (darkDice[0] + lightDice[0] == 1))
        {
            cout << "Does this end well or poorly?\n" << "1) Well (light die)\n" <<"2)Poorly (dark die)" << endl;

            if (cinInt(2, 1, "Invalid Choice") == 1)
            {
                LightorDark = true;
            }
            else
            {
                LightorDark = false;
            }
        }
        else if (darkDice[0] == 0)
        {
            LightorDark = true;
            cout << "This must end well, because there are only light dice left" << endl;
        }
        else
        {
            LightorDark = false;
            cout << "This must end poorly, because there are only dark dice left" << endl;
        }
        
    }
    
    return LightorDark;
}


void Game::Tilt()
{
    cout << "This has not been implemented yet. Please do the tilt yourself" << endl;
    rollAvailableDice();
    for (int i = 0; i < 6; ++i)
    {
        cout << "Dice available that rolled a " << i+1 << ": " << Game::numbersAvailable[i] << endl;
    }
    string temp;
    cout << "Please enter the first tilt in the form TILT ELEMENT must affect CHARACTER NAME" << endl << "For now, these will be stored as general information, and displayed at the start of each scene" << endl;
    getline(cin, temp);
    
    Players[0].GameElementDescriptions.push_back(temp);
    cout << "Please enter the second tilt in the form TILT ELEMENT must affect CHARACTER NAME" << endl;
    getline(cin, temp);
    
}

void Game::Aftermath()
{
    cout << "The aftermath has not been fully implemented yet." << endl;
    cout << "This function will roll the players' dice, and return the score of each player, as well as whether it is light or dark. You will need to look up the results in an aftermath table." << endl;
    
    vector<int> playerScore;
    int temp;
    for (int i = 1; i <= numberOfPlayers; ++i)
    {
        temp = 0;
        for (int j = 0; j < lightDice[i]; ++j)
        {
            temp += d6.roll();
        }
        for (int k = 0; k < darkDice[i]; ++k)
        {
            temp -= d6.roll();
        }
        playerScore.push_back(temp);
        if (playerScore[i-1] > 0)
        {
            cout << Players[i].nameCharacter << ", played by " << Players[i].nameReal << "ended with a light score of " << abs(temp) << endl;
        }
        if (playerScore[i-1] < 0)
        {
            cout << Players[i].nameCharacter << ", played by " << Players[i].nameReal << "ended with a dark score of " << abs(temp) << endl;
        }
                                                                                                                     
    }
}


/*Utility functions*/

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

//reads in an integer, and checks to be sure it is an integer, and that it is between the lower limit and the upper limit, INCLUSIVE
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
//gets a game element based on the input (type, subcategory, element number)
GameElement Game::getPlaysetItem(int i, int j, int k)
{
    return playset1.getItem(i, j, k);
}
//gets a relationship game element based on the input (subcategory, element number)
Relationship Game::getPlaysetRelationship(int j, int k)
{
    return playset1.getRelationship(j, k);
}

//returns the total number of available dice rolled. Used during setup. Possibly useful during the tilt and aftermath as well.
int Game::totalNumbersAvailable()
{
    for (int i = 0; i < 6; ++i)
    {
        assert (numbersAvailable[i] >= -1); //can end up as -1 because of the last wildcard dice
    }
    return (numbersAvailable[0] + numbersAvailable[1] + numbersAvailable[2] + numbersAvailable[3] + numbersAvailable[4] + numbersAvailable[5]);
}

int Game::totalLightLeft()
{
    return lightDice[0];
}

int Game::totalDarkLeft()
{
    return darkDice[0];
}