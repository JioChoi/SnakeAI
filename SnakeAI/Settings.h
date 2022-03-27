#pragma once
/*
0 - apple
1 - livedTime + closePoint * 5
2 - livedTime
3 - apple + closePoint
*/

extern std::string fileName;
extern int mutationRate;
extern int runTill;
extern int oneGeneration;
extern int mode;

#define BOARD_SIZE 20
#define CALCULATE_ONCE 250