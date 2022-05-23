#ifndef CLASSIFICATION_PERFORMACES_H
#define CLASSIFICATION_PERFORMACES_H
#include <stdio.h> 
#include <stdbool.h>
#include <stdlib.h>
#define NB_TESTS 30
#define NB_CLASSES 6

void displayResultsByClass(int realClasses[], int estimatedClasses[], int confusionMatrix[][NB_CLASSES]) {
	printf("Classes\tBien classes\tTotal\tPourcentage\n");
	int sum;
	double percent;
	for (int iColumn = 0; iColumn < NB_CLASSES; iColumn++) {
		sum = 0;
		for (int iLine = 0; iLine < NB_CLASSES; iLine++)
		     sum += confusionMatrix[iLine][iColumn];
		if (sum == 0)
		   percent = 0;
	        else 
		    percent = (double)confusionMatrix[iColumn][iColumn] / sum * 100;
		printf("%d\t\t%d\t%d\t%.2f%%\n", iColumn + 1, confusionMatrix[iColumn][iColumn], sum, percent);
	}
}

void displayAccuracy(int confusionMatrix[][NB_CLASSES]) {
	int sumWP = 0;
	int total = 0;
	for (int i = 0; i < NB_CLASSES; i++) {
		for (int j = 0; j < NB_CLASSES; j++) {
			if (i == j)
				sumWP += confusionMatrix[i][j];
			total += confusionMatrix[i][j];
		}
	}
	printf("The accuracy is %.2f %%\n", (double)sumWP / total * 100);
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

int indexSearch(int array[], int iTest) {
	int i = 1;
	while (i <= NB_CLASSES && i != array[iTest])
		i++;
	return i;
}

void createConfusionMatrix(int realClasses[], int estimatedClasses[], int confusionMatrix[][NB_CLASSES]) {
	for (int iTest = 0; iTest < NB_TESTS; iTest++) {
		int i = indexResearch(estimatedClasses,iTest);
		int j = indexResearch(realClasses, iTest);
		if(i <= NB_CLASSES)
		   confusionMatrix[i - 1][j - 1]++;
	}
}

#endif

