#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include<stdio.h>
#include<math.h>
#include <stdlib.h>
#include <string.h>
#include "classificationPerformances.h"


#define FILE_TEST "testSet.csv"
#define FILE_FIMODEL "fiModel.csv"
#define LINE 10000
#define NB_VACCS 600
#define NB_MOVEMENTS 6
#define NB_LINES_MATRIX_BYMVT 3
#define NB_LINES_MATRIX_FIMODEL 2
#define NB_LINES_FISET 30

typedef struct file File;
struct file {
    int movement;
    double vAccs[NB_VACCS];
};

typedef struct model Model;
struct model
{
    int numMotion;
    double averages[NB_VACCS];
    double stds[NB_VACCS];
    double globalAvg;
};

void initArray(double array[]);
int indMinValue(double array[]);
int compareDistances(double distances[], Model model[]);
int compareGlobalAvg(double sumGlobalAvg, int totVAccs, Model model[]);
int estimatedClass(int indicator1, int indicator2, int indicator3);
void fillModel(FILE* fpFiModel, Model model[]);
File fillFile(char line[]);


void main(void) {
    FILE* fpTest;
    FILE* fpFiModel;
    char line[LINE];
    File file;
    int realClasses[NB_LINES_FISET];
    int estimatedClasses[NB_LINES_FISET];
    int confusionMatrix[NB_MOVEMENTS][NB_MOVEMENTS] = { 0 };
    Model model[NB_MOVEMENTS];
    double sumGlobalAvg;
    double distancesAvgs[NB_MOVEMENTS];
    double distancesStds[NB_MOVEMENTS];
    int totVaccs;
    double lineStd;
    int iClass;
    int indicateur1;
    int indicateur2;
    int indicateur3;


    fopen_s(&fpTest, FILE_TEST, "r");
    fopen_s(&fpFiModel, FILE_FIMODEL, "r");

    if (fpTest == NULL || fpFiModel == NULL) {
        puts("error opening trainSet or fiModel");
    }
    else {
        fillModel(fpFiModel, model);
        iClass = 0;
        fgets(line, LINE, fpTest); // headers
        while (!feof(fpTest) && iClass < NB_LINES_FISET) {
            fgets(line, LINE, fpTest);
            file = fillFile(line);
            initArray(distancesAvgs);
            initArray(distancesStds);
            sumGlobalAvg = 0;
            totVaccs = 0;

            for (int iMovement = 0; iMovement < NB_MOVEMENTS; iMovement++)
            {
                for (int iVacc = 0; iVacc < NB_VACCS; iVacc++) {
                    sumGlobalAvg += file.vAccs[iVacc];
                    distancesAvgs[iMovement] += pow(file.vAccs[iVacc] - model[iMovement].averages[iVacc], 2);
                    lineStd = sqrt(pow(file.vAccs[iVacc] - model[iMovement].averages[iVacc], 2)); //(xi - moyenne)²( / 1)
                    distancesStds[iMovement] += pow(lineStd - model[iMovement].stds[iVacc], 2);
                    totVaccs++;
                }
            }

            for (int iDistance = 0; iDistance < NB_MOVEMENTS; iDistance++) {
                distancesAvgs[iDistance] = sqrt(distancesAvgs[iDistance]);
                distancesStds[iDistance] = sqrt(distancesStds[iDistance]);
            }

            indicateur1 = compareDistances(distancesAvgs, model);
            indicateur2 = compareDistances(distancesStds, model);
            indicateur3 = compareGlobalAvg(sumGlobalAvg, totVaccs, model);
            estimatedClasses[iClass] = estimatedClass(indicateur1, indicateur2, indicateur3);
            realClasses[iClass] = file.movement;
            iClass++;

        }

        fclose(fpFiModel);
        fclose(fpTest);

        createConfusionMatrix(realClasses, estimatedClasses, confusionMatrix, NB_LINES_FISET);
        printf("Legende \n %d : downstairs, %d : jogging, %d : sitting, %d : standing, %d : upstairs, %d : walking \n", 1, 2, 3, 4, 5, 6);
        displayResultsByClass(confusionMatrix);
        displayAccuracy(confusionMatrix);
        displayConfusionMatrix(confusionMatrix);
    }

}

void fillModel(FILE* fpFiModel, Model model[])
{
    char line[LINE];
    char* token;

    fgets(line, LINE, fpFiModel); // headers
    int iModel = 0;
    while (iModel < NB_MOVEMENTS) {
        fgets(line, LINE, fpFiModel);
        token = strtok(line, ",");
        model[iModel].numMotion = atoi(token);
        int iAvg = 0;
        while (token != NULL && iAvg < NB_VACCS) {
            model[iModel].averages[iAvg] = atof(token);
            token = strtok(NULL, ",");
            iAvg++;
        }

        fgets(line, LINE, fpFiModel);
        token = strtok(line, ",");
        token = strtok(NULL, ",");
        int iStd = 0;
        while (token != NULL && iStd < NB_VACCS) {
            model[iModel].stds[iStd] = atof(token);
            token = strtok(NULL, ",");
            iStd++;
        }

        fgets(line, LINE, fpFiModel);
        token = strtok(line, ",");
        token = strtok(NULL, ",");
        model[iModel].globalAvg = atof(token);
        iModel++;
    }
}


File fillFile(char line[]) {
    File file;
    char* token;
    token = strtok(line, ",");
    file.movement = atoi(token);
    token = strtok(NULL, ",");
    token = strtok(NULL, ",");
    token = strtok(NULL, ",");
    int i = 0;
    while (token != NULL && i < NB_VACCS) {
        file.vAccs[i] = atof(token);
        token = strtok(NULL, ",");
        i++;
    }

    return file;
}


void initArray(double array[]) {
    for (int i = 0; i < NB_MOVEMENTS; i++) {
        array[i] = 0;
    }
}

int indMinValue(double array[]) {
    int iMin = 0;
    for (int i = 1; i < NB_MOVEMENTS; i++)
    {
        if (array[i] < array[iMin])
            iMin = i;
    }
    return iMin;
}

int compareDistances(double distances[], Model model[]) {
    int iMin = indMinValue(distances);
    return model[iMin].numMotion;
}

int compareGlobalAvg(double sumGlobalAvg, int totVAccs, Model model[]) {
    double globalAverage = sumGlobalAvg / totVAccs;
    double distances[NB_MOVEMENTS];
    for (int i = 0; i < NB_MOVEMENTS; i++)
    {
        distances[i] = sqrt(pow(globalAverage - model[i].globalAvg, 2));
    }
    int iMin = indMinValue(distances);
    return model[iMin].numMotion;
}

int estimatedClass(int indicator1, int indicator2, int indicator3) {
    if (indicator1 == indicator2)
        return indicator1;
    else if (indicator2 == indicator3)
        return indicator2;
    else if (indicator3 == indicator1)
        return indicator3;
    else
        return indicator1;
}

