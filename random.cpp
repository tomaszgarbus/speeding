#include "random.h"

int nextRandom(int min, int max)
{
    return min + rand()%(max-min+1);
}
