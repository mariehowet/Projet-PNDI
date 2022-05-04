#include<stdio.h>

#define NB_VACCS 600
#define NB_MOVEMENTS 6
#define NB_LINES_MATRIX 3
#define FILE_TRAIN "trainSet.csv"
#define FILE_FIMODEL "fiModel.csv"
#define LINE 1000

typedef struct file File;
struct file {
	int movement;
	int gender;
	int index;
	double vAccs[NB_VACCS];
};

void main(void) {
	FILE* fpTrain;
	File* fpFiModel;
	char line[LINE];
	double matrixByMvmt[NB_LINES_MATRIX][NB_MOVEMENTS];
	File file;
	int iVacc;

	fopen(&fpTrain, FILE_TRAIN, "r");
	fopen(&fpFiModel, FILE_FIMODEL, "r");

	fprintf(fpFiModel, "%s", "Mouvement");
	
	for (int i = 0; i < NB_VACCS; i++) {
		fprintf(fpFiModel, ",%s", "Vacc" + i + 1);
	}

	fgets(line, LINE, fpTrain); // header line
	fgets(line, LINE, fpTrain);
	for (int iMvmt = 1; iMvmt <= NB_MOVEMENTS; iMvmt++) {
		initMatrix(matrixByMvmt, NB_LINES_MATRIX);
		sscanf_s(line, "%d,%d,%d") // problème de lecture du tableau vACCS de la structure file
		while (file.movement == iMvmt) {
			iVacc = 0;
			while (iVacc < NB_VACCS && file.vAccs[iVacc] != NULL) {

			}
		}
	}



}

void initMatrix(double matrix[][NB_VACCS],int  nbLines) {
	for (int i = 0; i < nbLines; i++) {
		for (int j = 0; j < NB_VACCS; i++) {
			matrix[i][j] = 0;
		}
	}
}