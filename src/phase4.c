#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include<stdio.h>
#include<math.h>
#include <stdlib.h>
#include <string.h>
#include "classificationPerformances.h"


#define FILE_TEST "testset.csv"
#define FILE_FIMODEL "fiModel.csv"
#define LINE 10000
#define NB_VACCS 600
#define NB_MOVEMENTS 6
#define NB_LINES_MATRIX_BYMVT 3
#define NB_LINES_MATRIX_FIMODEL 2
#define NB_LINES_FISET 30

typedef struct testLine TestLine;
struct testLine {
    int movement;
    int gender;
    int index;
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
TestLine fillTestLine(FILE* fpTest);


void main(void) {
    FILE* fpTest;
    FILE* fpFiModel;
    char line[LINE];
    TestLine testLine;
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
      } else {
            fillModel(fpFiModel, model);
            iClass = 0;
            fgets(line, LINE, fpTest); // headers
            while (!feof(fpTest) && iClass < NB_LINES_FISET) {
                testLine = fillTestLine(fpTest);
                initArray(distancesAvgs);
                initArray(distancesStds);
                sumGlobalAvg = 0;
                totVaccs = 0;

                for (int iMovement = 0; iMovement < NB_MOVEMENTS; iMovement++)
                {
                    for (int iVacc = 0; iVacc < NB_VACCS; iVacc++) {
                        sumGlobalAvg += testLine.vAccs[iVacc];
                        distancesAvgs[iMovement] += pow(testLine.vAccs[iVacc] - model[iMovement].averages[iVacc], 2);
                        lineStd = sqrt(pow(testLine.vAccs[iVacc] - model[iMovement].averages[iVacc], 2)); //(xi - moyenne)�( / 1)
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
                realClasses[iClass] = testLine.movement;
               printf("%d) real = %d, i1 =%d, i2=%d, i3=%d,estimated = %d\n",iClass + 1, realClasses[iClass], indicateur1, indicateur2, indicateur3, estimatedClasses[iClass]);
                iClass++;
               
            }
            
            fclose(fpFiModel);
            fclose(fpTest);

            createConfusionMatrix(realClasses, estimatedClasses, confusionMatrix);
            displayResultsByClass(realClasses, estimatedClasses, confusionMatrix);
            displayAccuracy(confusionMatrix);
            displayConfusionMatrix(confusionMatrix);
    }
      
}

void fillModel(FILE* fpFiModel,Model model[])
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


TestLine fillTestLine(FILE* fpTest) {
    TestLine testLine;
    char line[LINE];
    char* token;
    fgets(line, LINE, fpTest);
    token = strtok(line, ",");
    testLine.movement = atoi(token);
    token = strtok(NULL, ",");
    token = strtok(NULL, ",");

    token = strtok(NULL, ",");
    int i = 0;
    while (token != NULL && i < NB_VACCS) {
        testLine.vAccs[i] = atof(token);
        token = strtok(NULL, ",");
        i++;
    }
    return testLine;
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
        distances[i] =  sqrt(pow(globalAverage - model[i].globalAvg, 2));
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
