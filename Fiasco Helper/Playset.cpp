//
//  Playset.cpp
//  Fiasco Helper
//
//  Created by Laura D. on 6/2/14.
//  Copyright (c) 2014 Laura Delaney. All rights reserved.
//

#include "Playset.h"

Playset::Playset()
{
    int choice = 0; //initialized so it can be checked the first time
    int UseThisSetResponse = 0; //used to ask if the user wants to use the set they selected
    bool setChosen = false; // user has not chosen a set yet
    
    //for this to work on other machines, has to be re-hardcoded?
    filename = "/Users/Kittywizard/Documents/CS 171/Other/Fiasco Helper/Resources/";
    
    
    while (setChosen == false)
    {
        cout << "This menu is hardcoded. To add new playsets, ask me to hardcode a menu option for them. Enjoy!" << endl;
        cout << "Select a playset" << endl;
        cout << "1) Action 6 News" << endl;
        cout << "2) Pipe-Weed, a hobbit playset" << endl;
        cout << "3) No Strings Attached, a muppet playset" << endl;

        while (!(cin >> choice) || choice < 1 || choice > 3)
        {
            cin.clear();
            cin.ignore(INT_MAX,'\n');
            cout << "Wrong format \n";
            continue;
        }
        loadBasicSetInfo(choice);
        
        cout << "Would you like to choose this set? enter 1 for yes, and 2 for no" << endl;
        while (!(cin >> UseThisSetResponse ) || UseThisSetResponse  < 1 || UseThisSetResponse > 2)
        {
            cin.clear();
            cin.ignore(INT_MAX,'\n');
            cout << "Wrong format \n";
            continue;
        }
        
        //if they choose to use the set set, let them exit the loop
        if (UseThisSetResponse  == 1)
        {
            setChosen = true;
        }
    }
    
    switch (choice)
    {
        case 1:
            filename += "Action_6.txt";
            break;
        case 2:
            filename += "Pipe_Weed.txt";
            break;
        case 3:
            filename += "No_Strings_Attached.txt";
            break;
        default:
            cout << "Default case reached selecting a playset. Will now exit" << endl;
            exit(EXIT_FAILURE);
    }
    
      load(); //loads the playset selected

}

Playset::Playset(string filename)
{
    this -> filename = filename;
    load();
}

void Playset::loadBasicSetInfo(int choice)
{
    string NameOfFile = filename;
    string temp; // used to carry strings between fin and cout
    switch (choice)
    {
        case 1:
            NameOfFile += "Action_6.txt";
            break;
        case 2:
            NameOfFile += "Pipe_Weed.txt";
            break;
        case 3:
            NameOfFile += "No_Strings_Attached.txt";
            break;
        default:
            cout << "Default case reached selecting a playset. Will now exit" << endl;
            exit(EXIT_FAILURE);
    }

    ifstream fin;
    fin.open(NameOfFile);
    
    //if the file fails to open, tell the user what happened and exit the program
    if (fin.fail())
    {
        cout << "Failed to load playset. Please exit the program" << endl;
        exit(EXIT_FAILURE);
    }
    //otherwise go to location 168 (after all of the set elements, and print out the author name, credits, and description of the playset
    else
    {
        int count = 0;
        while (!fin.eof())
        {
            count++;
            getline(fin, temp);
            if (count > 168)
            {
            cout << temp << endl;
            }
            
        }

        cout << endl;
    }
    
    fin.close(); //remember to close the file
    return;
}

//is called by the constructor, after the filename is set
void Playset::load()
{
    ifstream fin;
    fin.open(filename);
    string temp; //used to pull a string from the file, line by line
    vector<string> temporary;
    
    
    //if the file fails to open, tell the user what happened and exit the program
    if (fin.fail())
    {
        cout << "Failed to load playset. Please exit the program" << endl;
        exit(EXIT_FAILURE);
    }
    
    // otherwise, read the file into a vector, line by line.
    else
    {
        while (!fin.eof())
        {
            getline(fin, temp);
            temporary.push_back(temp);
            //cout << temp << endl;
        }
    }
    
    //if there are not enough lines to be a full playset, tell the user what happened, and exit the program.
    if (temporary.size() < 168)
    {
        cout << "File was read in, but is invalid. The file should have 168 lines. It has " << temporary.size() << " lines instead" << endl;
        fin.close(); // is this needed?
        exit(EXIT_FAILURE);
    }
    else
    {
        int count = 0;
        int position; //used to track where to split a relationship description into two parts
        
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 6; ++j)
            {
                for (int k = 0; k < 7; ++ k)
                {
                    
                    //loads the playset game element items into the appropriate arrays
                    switch(i)
                    {
                        case 0:
                            playsetItemsRelationships[j][k].description = temporary[count];
                            
                            position = playsetItemsRelationships[j][k].description.find(" and ");
                            
                            //if the relationship has a direction (contains " and " as part of the string), then set description 1 as the first role, and description 2 as the second role
                            if ( position != string::npos)
                            {
                                playsetItemsRelationships[j][k].hasDirection = true;
                                playsetItemsRelationships[j][k].description1 = playsetItemsRelationships[j][k].description.substr(0,position);
                                playsetItemsRelationships[j][k].description2 = playsetItemsRelationships[j][k].description.substr(position+5, playsetItemsRelationships[j][k].description.length());
                            }
                            break;
                        case 1:
                            playsetItemsNeeds[j][k].description = temporary[count];
                            break;
                        case 2:
                            playsetItemsLocations[j][k].description = temporary[count];
                            break;
                        case 3:
                            playsetItemsObjects[j][k].description = temporary[count];
                            break;
                        default:
                            cout << "got to default case loading playset. error" << endl;
                            return;
                    }
                    
                    ++count; //increment to the next element
                }
            }
        }
    }
    
    
    fin.close();
    return;
}

GameElement& Playset::getItem(int a, int b, int c)
{
    
    switch(a)
    {
        case 0:
            return playsetItemsRelationships[b][c];
            break;
        case 1:
            return playsetItemsNeeds[b][c];
            break;
        case 2:
            return playsetItemsLocations[b][c];
            break;
        case 3:
            return playsetItemsObjects[b][c];
            break;
        default:
            cout << "got to default case getting item. error" << endl;
            return playsetItemsRelationships[0][0];
    }
    
}