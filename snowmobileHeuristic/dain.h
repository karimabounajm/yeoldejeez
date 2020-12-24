#ifndef santa
#define santa



struct gameValues;


struct pathWay;


struct gameValues* initializeGameValues(char* filename);


void getTransformations(struct gameValues* inIT);


void addGameboard(struct gameValues* inIT);


void checkBestCoord(struct gameValues* inIT);


void gameTime(struct gameValues* inIT, struct pathWay* curPath, struct pathWay* extPath)



#endif santa