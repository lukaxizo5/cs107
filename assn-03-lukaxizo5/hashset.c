#include "hashset.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

const static int startingSize = 4;

void HashSetNew(hashset *h, int elemSize, int numBuckets, HashSetHashFunction hashfn, HashSetCompareFunction comparefn, HashSetFreeFunction freefn){
	assert(elemSize > 0);
	assert(numBuckets > 0);
	assert(hashfn != NULL);
	assert(comparefn != NULL);
	h->elemSize = elemSize;
	h->numBuckets = numBuckets;
	h->count = 0;
	h->hashfn = hashfn;
	h->comparefn = comparefn;
	h->freefn = freefn;
	h->buckets = (vector*)(malloc(sizeof(vector) * numBuckets));
	assert(h->buckets != NULL);
	for (int i = 0; i < numBuckets; i++) {
		VectorNew(&h->buckets[i], h->elemSize, h->freefn, startingSize);
	}
}

void HashSetDispose(hashset *h){
	for (int i = 0; i < h->numBuckets; i++) {
		VectorDispose(&h->buckets[i]);
	}
	free(h->buckets);
}

int HashSetCount(const hashset *h){ 
	return h->count; 
}

void HashSetMap(hashset *h, HashSetMapFunction mapfn, void *auxData){
	assert(mapfn != NULL);
	for (int i = 0; i < h->numBuckets; i++) {
		VectorMap(&h->buckets[i], mapfn, auxData);
	}
}

void HashSetEnter(hashset *h, const void *elemAddr){
	assert(elemAddr != NULL);
	int hash = h->hashfn(elemAddr, h->numBuckets);
	assert(hash >= 0 && hash < h->numBuckets);
	int idx = VectorSearch(&h->buckets[hash], elemAddr, h->comparefn, 0, 0);
	if (idx == -1) {
		VectorAppend(&h->buckets[hash], elemAddr);
		h->count++;
	}
	else VectorReplace(&h->buckets[hash], elemAddr, idx);
}

void *HashSetLookup(const hashset *h, const void *elemAddr){ 
	assert(elemAddr != NULL);
	int hash = h->hashfn(elemAddr, h->numBuckets);
	assert(hash >= 0 && hash < h->numBuckets);
	int idx = VectorSearch(&h->buckets[hash], elemAddr, h->comparefn, 0, 0);
	if (idx == -1) return NULL;
	return VectorNth(&h->buckets[hash], idx); 
}