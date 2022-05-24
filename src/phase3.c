#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>

#define FILE_TRAIN "trainSet.csv"
#define FILE_FIMODEL "fiModel.csv"
#define LINE 10000
#define NB_VACCS 600
#define NB_MOVEMENTS 6
#define NB_LINES_MATRIX_BYMVT 3
#define NB_LINES_MATRIX_FIMODEL 2

typedef struct file File;
struct file {
	int movement;
	double vAccs[NB_VACCS];
};

File fillFile(char line[]);
void initMatrix(double matrix[][NB_VACCS], int  nbLines);
void matrixForFiModel(double matrixByMvmt[][NB_VACCS], double matrixFiModel[][NB_VACCS]);
double generalAverage(double matrixByMvmt[][NB_VACCS]);
void writing(FILE* fpFiModel, int iLine, double matrix[][NB_VACCS]);
void writingInFiModel(FILE* fpFiModel, int iMovement, double matrixFiModel[][NB_VACCS], double generalAverage);

void main(void) {
	FILE* fpTrain;
	FILE* fpFiModel;
	char line[LINE];
	File file;
	int iVacc;
	double matrixByMvmt[NB_LINES_MATRIX_BYMVT][NB_VACCS];
	double matrixFiModel[NB_LINES_MATRIX_FIMODEL][NB_VACCS];
	double generalAvg;

	fopen_s(&fpTrain, FILE_TRAIN, "r");
	fopen_s(&fpFiModel, FILE_FIMODEL, "w");

	if (fpTrain == NULL || fpFiModel == NULL)
		puts("error opening trainSet or fiModel");
	else {

		fprintf(fpFiModel, "%s", "mouvement");
		for (int i = 0; i < NB_VACCS; i++)
			fprintf(fpFiModel, ", %s %d", "vAcc", i + 1);
		fprintf(fpFiModel, "\n");

		fgets(line, LINE, fpTrain);
		int iMvt = 1;
		while (!feof(fpTrain) && iMvt <= NB_MOVEMENTS) {
			initMatrix(matrixByMvmt, NB_LINES_MATRIX_BYMVT);

			fgets(line, LINE, fpTrain);
			do {
				file = fillFile(line);
				iVacc = 0;
				while (iVacc < NB_VACCS && file.vAccs[iVacc] > 0) {
					matrixByMvmt[0][iVacc] += file.vAccs[iVacc];
					matrixByMvmt[1][iVacc] += pow(file.vAccs[iVacc], 2);
					matrixByMvmt[2][iVacc]++;
					iVacc++;
				}
			} while (fgets(line, LINE, fpTrain) != NULL && file.movement == iMvt);
			matrixForFiModel(matrixByMvmt, matrixFiModel);
			generalAvg = generalAverage(matrixByMvmt);
			writingInFiModel(fpFiModel, iMvt, matrixFiModel, generalAvg);
			iMvt++;
		}
		fclose(fpFiModel);
		fclose(fpTrain);
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

void initMatrix(double matrix[][NB_VACCS], int  nbLines) {
	for (int i = 0; i < nbLines; i++) {
		for (int j = 0; j < NB_VACCS; j++) {
			matrix[i][j] = 0;
		}
	}
}

void matrixForFiModel(double matrixByMvmt[][NB_VACCS], double matrixFiModel[][NB_VACCS]) {
	int iVacc = 0;
	while (iVacc < NB_VACCS && matrixByMvmt[0][iVacc] != 0) {
		matrixFiModel[0][iVacc] = matrixByMvmt[0][iVacc] / matrixByMvmt[2][iVacc];
		matrixFiModel[1][iVacc] = (matrixByMvmt[1][iVacc] / matrixByMvmt[2][iVacc]) - pow((matrixFiModel[0][iVacc]), 2);
		iVacc++;
	}
}

double generalAverage(double matrixByMvmt[][NB_VACCS]) {
	int i = 0;
	double numerator = 0;
	double denominator = 0;

	while (i < NB_VACCS && matrixByMvmt[0][i] != 0) {
		numerator += matrixByMvmt[0][i];
		denominator += matrixByMvmt[2][i];
		i++;
	}

	return numerator / denominator;
}

void writing(FILE* fpFiModel, int iLine, double matrix[][NB_VACCS]) {
	int i = 0;
	while (i < NB_VACCS && matrix[iLine][i] != 0) {
		fprintf(fpFiModel, ",%lf", matrix[iLine][i]);
		i++;
	}
}

void writingInFiModel(FILE* fpFiModel, int iMovement, double matrixFiModel[][NB_VACCS], double generalAverage) {
	fprintf(fpFiModel, "%d", iMovement);
	writing(fpFiModel, 0, matrixFiModel);
	fprintf(fpFiModel, "\n%d", iMovement);
	writing(fpFiModel, 1, matrixFiModel);
	fprintf(fpFiModel, "\n%d, %lf\n", iMovement, generalAverage);
}


