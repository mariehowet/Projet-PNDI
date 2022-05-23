#include "classificationPerformances.h"
#include <stdio.h> 
#include <stdbool.h>
#include < stdlib.h>

void displayResultsByClass(int realClasses[], int estimatedClasses[], int confusionMatrix[][NB_CLASSES]);
void displayAccuracy(int realClasses[], int estimatedClasses[], int confusionMatrix[][NB_CLASSES]);
void displayConfusionMatrix(int confusionMatrix[][NB_CLASSES]);
void createConfusionMatrix(int realClasses[], int estimatedClasses[], int confusionMatrix[][NB_CLASSES]);

void main(void) {
	int realClasses[] = { 5, 2, 5, 3, 1, 3, 2, 4};
	int estimatedClasses[] = { 5, 5, 1, 2, 1, 3, 2, 4};
	int confusionMatrix[NB_CLASSES][NB_CLASSES] = { 0 };
	createConfusionMatrix(realClasses, estimatedClasses, confusionMatrix);
	displayResultsByClass(realClasses, estimatedClasses, confusionMatrix);
	displayAccuracy(confusionMatrix);
	displayConfusionMatrix(confusionMatrix);
}
