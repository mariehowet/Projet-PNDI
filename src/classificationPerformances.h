#ifndef CLASSIFICATION_PERFORMACES_H
#define CLASSIFICATION_PERFORMACES_H
#include <stdio.h> 
#include <stdbool.h>
#include < stdlib.h>
#define NB_TESTS 8
#define NB_CLASSES 5

void displayResultsByClass(int realClasses[], int estimatedClasses[], int confusionMatrix[][NB_CLASSES]);
void displayAccuracy(int realClasses[], int estimatedClasses[], int confusionMatrix[][NB_CLASSES]);
void displayConfusionMatrix(int confusionMatrix[][NB_CLASSES]);
void createConfusionMatrix(int realClasses[], int estimatedClasses[], int confusionMatrix[][NB_CLASSES]);
#endif

