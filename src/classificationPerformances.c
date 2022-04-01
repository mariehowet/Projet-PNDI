#include "classificationPerformances.h"
#include <stdio.h> 
#include <stdbool.h>
#include < stdlib.h>
#define NB_TESTS 8
#define NB_CLASSES 5

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
	displayAccuracy(realClasses, estimatedClasses, confusionMatrix);
	displayConfusionMatrix(confusionMatrix);
}

void displayResultsByClass(int realClasses[], int estimatedClasses[], int confusionMatrix[][NB_CLASSES]) {
	printf("Classes\tBien classes\tTotal\tPourcentage\n");
	int sum;
	for (int i = 0; i < NB_CLASSES; i++) {
		sum = 0;
		for (int j = 0; j < NB_CLASSES; j++)
			sum += confusionMatrix[i][j];
		printf("%d\t\t%d\t%d\t%.2f%%\n", i + 1, confusionMatrix[i][i], sum, ((double)confusionMatrix[i][i] / sum) * 100);
	}
}

void displayAccuracy(int realClasses[], int estimatedClasses[], int confusionMatrix[][NB_CLASSES]) {
	int sumWP = 0;
	int sumRP = 0;
	for (int i = 0; i < NB_CLASSES; i++) {
		for (int j = 0; j < NB_CLASSES; j++) {
			if (i == j && confusionMatrix[i][j] >= 1)
				sumWP += confusionMatrix[i][j];
			sumRP += confusionMatrix[i][j];
		}
	}
	printf("The accuracy is %.2f %%\n", (double)sumWP / sumRP * 100);
}

void displayConfusionMatrix(int confusionMatrix[][NB_CLASSES]) {
	for (int i = 0; i < NB_CLASSES; i++)
		printf("\t%d", (i + 1));
	printf("\n__________________________________________________\n");
	for (int i = 0; i < NB_CLASSES; i++) {
		printf("%d |", (i + 1));
		for (int j = 0; j < NB_CLASSES; j++) {
			printf("\t%d", confusionMatrix[i][j]);
		}
		printf("\n");
	}
}

void createConfusionMatrix(int realClasses[], int estimatedClasses[], int confusionMatrix[][NB_CLASSES]) {
	for (int iTest = 0; iTest < NB_TESTS; iTest++) {
		int i = 1;
		while (i <= NB_CLASSES && i != estimatedClasses[iTest])
			i++;
		int j = 1;
		while (j <= NB_CLASSES && j != realClasses[iTest])
			j++;
		if (i <= NB_CLASSES)
			confusionMatrix[i - 1][j - 1]++;
	}
}
