#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "game.h"


// organizing the relevant pieces of data by struct

struct gameValues
{
    int height, width;
    char left, right, up, down, quit;

    int playXcoo, playYcoo;
    int powerDuration;
    char pIcon, powIcon;

    int numItems, scoreValue;
};


// generalized input testing, to see if we got the information correctly

void structTest(struct gameValues* gV)
{
    printf("the height is %d, and the width is %d\n", gV->height, gV->width);
    printf("Left is %c, right is %c, up is %c, down is %c, and quit is %c\n", gV->left, gV->right, gV->up, gV->down, gV->quit);
    printf("Player icon is %c, powered up is %c, and starting position is %d %d\n\n", gV->pIcon, gV->powIcon, gV->playXcoo, gV->playYcoo);
}


// finding the map boundaries - height and width

void addDimensions(struct gameValues* gV, FILE *file)
{
    fscanf(file, " %d", &gV->height);
    fscanf(file, " %d\n", &gV->width);
}

// determining the icons of the player and the starting position

void addPlayer(struct gameValues* gV, char*buffer)
{
    gV->pIcon = strtok(buffer, " ")[0];
    gV->powIcon = strtok(NULL, " ")[0];
    gV->playYcoo = atoi(strtok(NULL, " "));
    gV->playXcoo = atoi(strtok(NULL, " "));
}


// finding the control characters - left right up down and quit
// note, include default of controls, and adds the player
// for simplification, also add player characters in the same block:
// makes the if/else condition easier.

void addControls(struct gameValues* gV, FILE *file)
{
    char controls [5], bufferNaughty[100];
    char* bufferNice;

    fgets(bufferNaughty, 100, file);
    if (strlen(bufferNaughty) == 10)
    {
        controls[0] = strtok(bufferNaughty, " ")[0];
        int i = 0;
        while (i <= 3)
        {
            i++;
            bufferNice = strtok(NULL, " ");
            if (bufferNice != NULL) controls[i] = bufferNice[0];
            else break;
        }

        gV->left = controls[0]; gV->right = controls[1]; gV->up = controls[2];
        gV->down = controls[3]; gV->quit = controls[4];

        fgets(bufferNaughty, 100, file);
        addPlayer(gV, bufferNaughty);
    }

    else
    {
        gV->left = 'a'; gV->right = 'd'; gV->up = 'w';
        gV->down = 's'; gV->quit = 'q';

        addPlayer(gV, bufferNaughty);
    }
}


// this will create the 2d array board, which does not include the border; pushing to remote
// takes meant to take a 4th+ line string, meaning a character identifying the type followed by coordinates
// note that this is meant to replace values within the 2d array to represent these items, building is big

void createBoard(struct gameValues* gV, char* buffer, char gameBoard[gV->height][gV->width])
{
    int x_cord, y_cord;
    char type = strtok(buffer, " ")[0];

    y_cord = atoi(strtok(NULL, " "));
    x_cord = atoi(strtok(NULL, " "));

//    this is to create a building, which a 4x6. if/else reflects composition
    if (type == 'B')
    {
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 6; k++)
            {
                if ((j >= 2) && (k >= 2) && (k <= 3))
                    gameBoard[y_cord + j][x_cord + k] = '&';
                else gameBoard[y_cord + j][x_cord + k] = '-';
            }
        }
    }

//    for the items
    if (type == 'I')
    {
        gameBoard[y_cord][x_cord] = '$';
        gV->numItems++;
    }

//    for all enemies
    if (type == 'E') gameBoard[y_cord][x_cord] = 'X';

//    for the powerups
    if (type == 'P') gameBoard[y_cord][x_cord] = '*';
}


// this helper function essentially runs the create array file until the file is fully read, taking the string to the board creator function
// the board creator function will add in all the special

void loopArray(struct gameValues* gV, FILE *file, char gameBoard[gV->height][gV->width])
{
    char bufferLoop[100];
    gV->numItems = 0;
    gV->scoreValue = 0;
    gV->powerDuration = 0;

    while(fgets(bufferLoop, 100, file)) createBoard(gV, bufferLoop, gameBoard);
}


// prints the board by creating the shell and putting in the character in the proper position
// note: also checks for powerup and it's duration, changing the character used for the player accordingly

void brintBoard(struct gameValues* gV, char gameBoard[gV->height][gV->width])
{
    printf("Score: %d\n", gV->scoreValue);
    printf("Items remaining: %d\n", gV->numItems);
    int height = gV->height; int width = gV->width;

    if (gV->powerDuration > 0)
        gameBoard[gV->playYcoo][gV->playXcoo] = gV->powIcon;
    else
        gameBoard[gV->playYcoo][gV->playXcoo] = gV->pIcon;

    for (int i = -2; i < width; i++) printf("*");
    for (int i = 0; i < height; i++)
    {
        printf("\n*");
        for (int j = 0; j < width; j++) printf("%c", gameBoard[i][j]);
        printf("*");
    }
    printf("\n");
    for (int i = -2; i < width; i++) printf("*");
    printf("\n");
}


// takes in input from the inputChange function, and checks the coordinates that would be moved into for validity
// also takes into account special interactions, and cases such as powered up contact with enemies (diffuse kit)

int checkMovement(struct gameValues* gV, int x_cord, int y_cord, char gameBoard[gV->height][gV->width])
{
    if (gameBoard[y_cord][x_cord] == 'X')
    {
        if (gV->powerDuration >= 0)
        {
            printf("players coords are %d %d\n boom boom's coords are %d %d", gV->playXcoo, gV->playYcoo, x_cord, y_cord);
            gameBoard[gV->playYcoo][gV->playXcoo] = ' ';
            gV->playYcoo = y_cord; gV->playXcoo = x_cord;
            gameBoard[y_cord][x_cord] = gV->pIcon;
            gV->scoreValue++;
            return 1;
        }
        else
        {
            gV->pIcon = ' '; // because of way brintboard is coded, with it adding the player icon itself, changing player icon is easiest
            gameBoard[y_cord][x_cord] = '@';
            return 0;
        }
    }

    else if (gameBoard[y_cord][x_cord] == '$')
    {
        gameBoard[gV->playYcoo][gV->playXcoo] = ' ';
        gameBoard[y_cord][x_cord] = gV->pIcon;
        gV->numItems--; gV->scoreValue++;
        gV->playYcoo = y_cord; gV->playXcoo = x_cord;
        return 1;
    }

    if (x_cord >= 0 && x_cord < gV->width && y_cord >= 0 && y_cord <= gV->height)
    {
        if (gameBoard[y_cord][x_cord] == ' ')
        {
            gameBoard[gV->playYcoo][gV->playXcoo] = ' ';
            gV->playYcoo = y_cord; gV->playXcoo = x_cord;
            gameBoard[y_cord][x_cord] = gV->pIcon;
            return 1;
        }

        else if (gameBoard[y_cord][x_cord] == '*')
        {
            gameBoard[gV->playYcoo][gV->playXcoo] = ' ';
            gameBoard[y_cord][x_cord] = gV->pIcon;
            gV->playYcoo = y_cord; gV->playXcoo = x_cord;
            gV->powerDuration = 7;
            return 1;
        }
        else return 2;
    }

    else return 2;
}


// asks for input from the user, and sends the input to the proper function. also checks if big boom (enemy contact) has been made

int inputChange(struct gameValues* gV, char gameBoard[gV->height][gV->width])
{
    char bufferInput[100];
    int bigBoom = 1;

    printf("Enter input: ");
    scanf("%s", bufferInput);
    fflush(stdin);


    if (bufferInput[0] == gV->left)
    {
        bigBoom = checkMovement(gV, gV->playXcoo - 1, gV->playYcoo, gameBoard);
        if (bigBoom != 2) brintBoard(gV, gameBoard);
    }

    else if (bufferInput[0] == gV->right)
    {
        bigBoom = checkMovement(gV, gV->playXcoo + 1, gV->playYcoo, gameBoard);
        brintBoard(gV, gameBoard);
    }
    else if (bufferInput[0] == gV->up)
    {
        bigBoom = checkMovement(gV, gV->playXcoo, gV->playYcoo - 1, gameBoard);
        brintBoard(gV, gameBoard);
    }
    else if (bufferInput[0] == gV->down)
    {
        bigBoom = checkMovement(gV, gV->playXcoo, gV->playYcoo + 1, gameBoard);
        brintBoard(gV, gameBoard);
    }
    else printf("Invalid input.\n");
    
    if (bufferInput[0] == gV->quit)
    {
        printf("You have quit\n");
        return 0;
    }
    if (bigBoom == 0)
    {
        printf("You have died\n");
        return 0;
    }

    return 1;
}


// the actual playGame file, which puts together all the helper functions

int playGame(const char* filename)
{
    FILE *file = fopen(filename, "r");
    if (!file) return 0;

    struct gameValues* gV = malloc(sizeof(struct gameValues));

    addDimensions(gV, file);
    addControls(gV, file);

    char gameBoard[gV->height][gV->width];
    for (int i = 0; i < gV->height; i++) memset(gameBoard[i], ' ', gV->width);

    loopArray(gV, file, gameBoard);
    brintBoard(gV, gameBoard);

    while (inputChange(gV, gameBoard))
    {
        gV->powerDuration--;
        if (gV->numItems == 0)
        {
            printf("Congratulations! You have won.\n");
            break;
        }
    }
    printf("Final score: %d\n", gV->scoreValue);
    free(gV);
    fclose(file);
    return 1;
}


 int main()
 {
     const char* filename = "/Users/karim/GitHub/ecs-36A/7-assignment/7-assignment/Example Files/game3.txt";
     playGame(filename);     
     const char* fiename = "/Users/karim/GitHub/ecs-36A/7-assignment/7-assignment/Example Files/game2.txt";
     playGame(fiename);
     return 0;

 }

