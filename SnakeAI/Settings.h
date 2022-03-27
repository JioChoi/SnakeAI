#pragma once
/*
0 - apple
1 - livedTime + closePoint * 5
2 - livedTime
3 - apple + closePoint
*/
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define MODE 0
#define MUTATION_RATE 0
#define RUN_TILL 200
#define ONE_GENERATION_NUM 500 //999

#define BOARD_SIZE 20
#define CALCULATE_ONCE 250

#define FILE_NAME "m" STR(MODE) "_mu" STR(MUTATION_RATE) "_indi" STR(ONE_GENERATION_NUM) "_gen" STR(RUN_TILL)