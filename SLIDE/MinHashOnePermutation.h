#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <climits>
#include <iostream>
#include <random>
#include <vector>
#include <string.h>
#include "MurmurHash.h"


using namespace std;
class MinHashOnePermutation
{
private:
    int *_randHash, _randa, _numpartitions, _rangePow,_lognumhash;
public:
    MinHashOnePermutation(int numPartitions, int noOfBitsToHash);
    int * getHash(int* indices, float* data, int* binids, int dataLen);
    int getRandDoubleHash(int binid, int count);
    int * getHashEasy(int* binids, float* data, int dataLen, int topK);
    void getMap(int n, int* binid);
    ~MinHashOnePermutation();
};