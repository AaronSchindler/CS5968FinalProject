#include "MinHashOnePermutation.h"
#include <random>
#include <iostream>
#include <math.h>
#include <vector>
#include <climits>
#include <algorithm>
#include <queue>
using namespace std;

typedef pair<int, float> PAIR;

struct cmp {
    bool operator()(const PAIR &a, const PAIR &b) {
        return a.second > b.second; //lower is better
    };
};

MinHashOnePermutation::MinHashOnePermutation(int numPartitions, int noOfBitsToHash)
{

    _numpartitions = numPartitions;
    _rangePow = noOfBitsToHash;
    _lognumhash = log2(numPartitions);


    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, INT_MAX);

    _randa = dis(gen);
    if (_randa % 2 == 0)
        _randa++;
    _randHash = new int[2];
    _randHash[0] = dis(gen);
    if (_randHash[0] % 2 == 0)
        _randHash[0]++;
    _randHash[1] = dis(gen);
    if (_randHash[1] % 2 == 0)
        _randHash[1]++;

}


void MinHashOnePermutation::getMap(int n, int* binids)
{
	// 1 => <1 0 0 0 0 >
	// Creates bit vector of size rangePow
    int range = 1 << _rangePow;
    // binsize is the number of times the range is larger than the total number of hashes we need.
    int binsize = ceil(1.0*range / _numpartitions);

	uint32_t curhash = MurmurHash ((char *)&n, (uint32_t) sizeof(n), (uint32_t)_randa);
	//        unsigned int curhash = (unsigned int)(((unsigned int)h*i) << 5);
	curhash = curhash & ((1<<_rangePow)-1);
    for (int i = 0; i < n; i++)
    {
        binids[i] = (int)floor(curhash / binsize);;
    }

}


int * DensifiedMinhash::getHashEasy(int* binids, float* data, int dataLen, int topK)
{

    // binsize is the number of times the range is larger than the total number of hashes we need.
// read the data and add it to priority queue O(dlogk approx 7d) with index as key and values as priority value, get topk index O(1) and apply minhash on retuned index.

    priority_queue<PAIR, vector<PAIR>, cmp> pq;

    for (int i = 0; i < topK; i++)
    {
        pq.push(std::make_pair(i,data[i]));
    }

    for (int i = topK; i < dataLen; i++)
    {
        pq.push(std::make_pair(i,data[i]));
        pq.pop();
    }



    int *hashes = new int[_numhashes];
    //float *values = new float[_numhashes];
    int *hashArray = new int[_numhashes];

    for (int i = 0; i < _numhashes; i++)
    {
        hashes[i] = INT_MIN;
    }


    for (int i = 0; i < topK; i++)
    {
        PAIR pair = pq.top();
        pq.pop();
        int index = pair.first;
        int binid = binids[index];
        if (hashes[binid] < index) {
            hashes[binid] = index;
        }
    }


    for (int i = 0; i < _numhashes; i++)
    {
        int next = hashes[i];
        if (next != INT_MIN)
        {
            hashArray[i] = hashes[i];
            continue;
        }
        int count = 0;
        while (next == INT_MIN)
        {
            count++;
            int index = std::min(
                    getRandDoubleHash(i, count),
                    _numhashes);

            next = hashes[index]; // Kills GPU.
            if (count > 100) // Densification failure.
                break;
        }
        hashArray[i] = next;
    }
    delete[] hashes;
    return hashArray;
}


int * DensifiedMinhash::getHash(int* indices, float* data, int* binids, int dataLen)
{
    int *hashes = new int[_numhashes];
    int *hashArray = new int[_numhashes];

    for (int i = 0; i < _numhashes; i++)
    {
        hashes[i] = INT_MIN;
    }

    if (dataLen<0){

    }

    for (int i = 0; i < dataLen; i++)
    {
        int binid = binids[indices[i]];

        if (hashes[binid] < indices[i]){
            hashes[binid] = indices[i];
        }
    }

    for (int i = 0; i < _numhashes; i++)
    {
        int next = hashes[i];
        if (next != INT_MIN)
        {
            hashArray[i] = hashes[i];
            continue;
        }
        int count = 0;
        while (next == INT_MIN)
        {
            count++;
            int index = std::min(
                    getRandDoubleHash(i, count),
                    _numhashes);

            next = hashes[index]; // Kills GPU.
            if (count > 100) // Densification failure.
                break;
        }
        hashArray[i] = next;
    }
    delete[] hashes;
    //   delete[] values;
    return hashArray;
}


int DensifiedMinhash::getRandDoubleHash(int binid, int count) {
    unsigned int tohash = ((binid + 1) << 6) + count;
    return (_randHash[0] * tohash << 3) >> (32 - _lognumhash); // _lognumhash needs to be ceiled.
}


DensifiedMinhash::~DensifiedMinhash()
{
    delete[] _randHash;
}
