#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include <stdlib.h>
#include <math.h>

#define FILE_DATA "data_subjects_info.csv"
#define FILE_TRAIN "trainSet.csv"
#define FILE_TEST "testSet.csv"
#define LGPATH 100
#define LINE 1000
#define NB_VACCS 600
#define NB_PARTCIPANTS 24
#define NB_TESTS 15

typedef struct data Data;
struct data {
	int code;
	int weight;
	int height;
	int age;
	int gender;
};

typedef struct file File;
struct file {
	int code;
	double attituderoll;
	double	attitudepitch;
	double	attitudeyaw;
	double	gravityx;
	double	gravityy;
	double	gravityz;
	double	rotationRatex;
	double rotationRatey;
	double	rotationRatez;
	double	userAccelerationx;
	double	userAccelerationy;
	double	userAccelerationz;
};

void arrayGenders(int genders[]);
void arrayMvtNames(char paths[][LGPATH], char mvtNames[][LGPATH]);
void writingDataSet(FILE* fpSet, char actualPath[], char mvtName[], int index, int gender);

void main(void) {
	char paths[][LGPATH] = { "dws_1/", "dws_2/", "dws_11/", "jog_9/", "jog_16/", "sit_5/", "sit_13/", "std_6/",
		"std_14/", "ups_3/", "ups_4/", "ups_12/", "wlk_7/", "wlk_8/", "wlk_15/"
	};

	char subs[][LGPATH] = { "sub_1.csv", "sub_2.csv", "sub_3.csv", "sub_4.csv", "sub_5.csv", "sub_6.csv", "sub_7.csv",
		"sub_8.csv", "sub_9.csv", "sub_10.csv", "sub_11.csv", "sub_12.csv", "sub_13.csv", "sub_14.csv", "sub_15.csv", "sub_16.csv",
		"sub_17.csv", "sub_18.csv","sub_19.csv", "sub_20.csv", "sub_21.csv", "sub_22.csv", "sub_23.csv", "sub_24.csv"
	};

	char actualPath[LGPATH];
	int genders[NB_PARTCIPANTS];
	char mvtNames[NB_TESTS][LGPATH];
	FILE* fpTrain;
	FILE* fpTest;
	arrayGenders(genders);
	arrayMvtNames(paths, mvtNames);

	fopen_s(&fpTrain, FILE_TRAIN, "w");
	fopen_s(&fpTest, FILE_TEST, "w");
	if (fpTrain == NULL || fpTest == NULL)
		puts("error opening trainSet or testSet");
	else {
		fprintf(fpTrain, "%s, %s, %s", "mouvement", "genre","index");
		for(int i = 0;i < NB_VACCS; i++)
			fprintf(fpTrain, ", %s %d", "vAcc", i + 1);
		fprintf(fpTrain, "\n");

		fprintf(fpTest, "%s, %s, %s", "mouvement", "genre", "index");
		for (int i = 0; i < NB_VACCS; i++)
			fprintf(fpTest, ", %s %d", "vAcc", i + 1);
		fprintf(fpTest, "\n");

		int iTest1 = 1;
		int iTest2 = 2;
		int indexTest = 1;
		int indexTrain = 1;
		for (int i = 0; i < NB_TESTS; i++) {
			if (iTest2 == NB_PARTCIPANTS) {
				iTest1 = 1;
				iTest2 = 2;
			}
			for (int j = 0; j < NB_PARTCIPANTS; j++) {
				strcpy_s(actualPath, LGPATH, paths[i]);
				strcat_s(actualPath, LGPATH, subs[j]);
				if (iTest1 == j || iTest2 == j) {
					writingDataSet(fpTest, actualPath, mvtNames[i], indexTest, genders[j]);
					indexTest++;
				}
				else {
					writingDataSet(fpTrain, actualPath, mvtNames[i], indexTrain, genders[j]);
					indexTrain++;
				}
			}
			iTest1 += 2;
			iTest2 += 2;
		}
		fclose(fpTrain);
		fclose(fpTest);
	}
}

void arrayGenders(int genders[]) {
	FILE* fpInfos;
	char line[LINE];
	Data data;
	int i = 0;

	fopen_s(&fpInfos, FILE_DATA, "r");

	if (fpInfos == NULL) {
		puts("error opening data_subjects_info.csv");
	}
	else {
		fgets(line, LINE, fpInfos);
		while (fgets(line, LINE, fpInfos) != NULL) {
			sscanf_s(line, "%d,%d,%d,%d,%d", &data.code, &data.weight, &data.height, &data.age, &data.gender);
			genders[i] = data.gender;
			i++;
		}
		fclose(fpInfos);
	}
}

void arrayMvtNames(char paths[][LGPATH], char mvtNames[][LGPATH]) {
	for (int i = 0; i < NB_TESTS; i++) {
		strncpy_s(mvtNames[i], LGPATH, paths[i], 3);
	}
}

void writingDataSet(FILE* fpSet, char actualPath[], char mvtName[], int index, int gender) {
	FILE* fp;
	int mvtNum;
	File file;
	char line[LINE];
	
	fopen_s(&fp, actualPath, "r"); 
	if (fp == NULL) {
		printf("%d error opening actualpath \n", index);
	}
	else {
		if (strcmp(mvtName, "dws") == 0)
			mvtNum = 1;
		else if (strcmp(mvtName, "jog") == 0)
			mvtNum = 2;
		else if (strcmp(mvtName, "sit") == 0)
			mvtNum = 3;
		else if (strcmp(mvtName, "std") == 0)
			mvtNum = 4;
		else if (strcmp(mvtName, "ups") == 0)
			mvtNum = 5;
		else
			mvtNum = 6;
		fprintf(fpSet, "%d, %d,%d", mvtNum, gender,index);
		fgets(line, LINE, fp);
		int i = 0;
		while (fgets(line, LINE, fp) != NULL && i < NB_VACCS) { 
			sscanf_s(line, "%d, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf,%lf, %lf", 
				&file.code, &file.attituderoll, &file.attitudepitch, &file.attitudeyaw, 
				&file.gravityx, &file.gravityy, &file.gravityz,
				&file.rotationRatex, &file.rotationRatey, &file.rotationRatez,
				&file.userAccelerationx, &file.userAccelerationy, &file.userAccelerationz
			);
			fprintf(fpSet, ",%lf", sqrt(pow(file.userAccelerationx, 2) + pow(file.userAccelerationy, 2) + pow(file.userAccelerationz, 2)));
			i++;
		}
		fprintf(fpSet, "\n");
		fclose(fp);
	}
}


