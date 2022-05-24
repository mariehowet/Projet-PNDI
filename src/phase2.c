#include "classificationPerformances.h"
#define NB_TESTS 8
void main(void) {
	int realClasses[] = { 5, 2, 5, 3, 1, 3, 2, 4};
	int estimatedClasses[] = { 5, 5, 1, 2, 1, 3, 2, 4};
	int confusionMatrix[NB_CLASSES][NB_CLASSES] = { 0 };
	createConfusionMatrix(realClasses, estimatedClasses, confusionMatrix, NB_TESTS);
	displayResultsByClass(confusionMatrix);
	displayAccuracy(confusionMatrix);
	displayConfusionMatrix(confusionMatrix);
}
