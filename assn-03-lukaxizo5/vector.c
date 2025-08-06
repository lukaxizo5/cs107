#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <search.h>

void VectorNew(vector *v, int elemSize, VectorFreeFunction freeFn, int initialAllocation){
    assert(elemSize > 0);
    if (initialAllocation == 0) initialAllocation = 4;
    assert(initialAllocation > 0);
    v->allocLength = initialAllocation;
    v->logicalLength = 0;
    v->elemSize = elemSize;
    v->freeFn = freeFn;
    v->base = malloc(elemSize * v->allocLength);
    assert(v->base != NULL);
}

void VectorDispose(vector *v){
    if (v->freeFn != NULL) {
        for (int i = 0; i < v->logicalLength; i++) {
            void* elemToFree = (char*)(v->base) + i * v->elemSize;
            v->freeFn(elemToFree);
        }
    }
    free(v->base);
}

int VectorLength(const vector *v){ 
    return v->logicalLength; 
}

void *VectorNth(const vector *v, int position){ 
    assert(position >= 0 && position < v->logicalLength);
    return (char*)(v->base) + position * v->elemSize; 
}

void VectorReplace(vector *v, const void *elemAddr, int position){
    assert(position >= 0 && position < v->logicalLength);
    if (v->freeFn != NULL) {
        v->freeFn((char*)(v->base) + position * v->elemSize);
    }
    memcpy((char*)(v->base) + position * v->elemSize, elemAddr, v->elemSize);
}

void VectorInsert(vector *v, const void *elemAddr, int position){
    assert(position >= 0 && position <= v->logicalLength);
    if (v->logicalLength == v->allocLength) {
        v->allocLength *= 2;
        v->base = realloc(v->base, v->allocLength * v->elemSize);
        assert(v->base != NULL);
    }
    if (position != v->logicalLength) {
        memcpy((char*)(v->base) + (position + 1) * v->elemSize, (char*)(v->base) + position * v->elemSize, (v->logicalLength - position) * v->elemSize);
    }
    memcpy((char*)(v->base) + position * v->elemSize, elemAddr, v->elemSize);
    v->logicalLength++;
}

void VectorAppend(vector *v, const void *elemAddr){
    VectorInsert(v, elemAddr, v->logicalLength);
}

void VectorDelete(vector *v, int position){
    assert(position >= 0 && position < v->logicalLength);
    if (v->freeFn != NULL) {
        v->freeFn((char*)(v->base) + position * v->elemSize);
    }
    if (position != v->logicalLength - 1) {
        memcpy((char*)(v->base) + position * v->elemSize, (char*)(v->base) + (position + 1) * v->elemSize, (v->logicalLength - position - 1) * v->elemSize);
    }
    v->logicalLength--;
}

void VectorSort(vector *v, VectorCompareFunction compare){
    assert(compare != NULL);
    qsort(v->base, v->logicalLength, v->elemSize, compare);
}

void VectorMap(vector *v, VectorMapFunction mapFn, void *auxData){
    assert(mapFn != NULL);
    for (int i = 0; i < v->logicalLength; i++) {
        mapFn((char*)(v->base) + i * v->elemSize, auxData);
    }
}

static const int kNotFound = -1;
int VectorSearch(const vector *v, const void *key, VectorCompareFunction searchFn, int startIndex, bool isSorted){ 
    assert(startIndex >= 0 && startIndex <= v->logicalLength);
    assert(key != NULL);
    assert(searchFn != NULL);
    assert(v->base != NULL);
    void* elem = NULL;
    if (isSorted) {
        elem = bsearch(key, (char*)(v->base) + startIndex * v->elemSize, v->logicalLength - startIndex, v->elemSize, searchFn);
    }
    else {
        size_t count = v->logicalLength - startIndex;
        elem = lfind(key, (char*)(v->base) + startIndex * v->elemSize, &count, v->elemSize, searchFn);
    }
    if (elem == NULL) return kNotFound;
    return ((char*)(elem) - (char*)(v->base)) / (v->elemSize); 
}