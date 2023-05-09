
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <fstream>
#include <windows.h>

#include <opencv2/opencv.hpp>
#include <arcsoft_face_sdk.h>

using namespace std;
using namespace cv;

#include "input_facedata.h"

input_facedata facecheck;	//��������ʶ�����

int main()
{
	//��ʾ����ͷ����
	bool run;
	thread camera(&input_facedata::showimage, &facecheck, ref(run));

	//������
	this_thread::sleep_for(chrono::seconds(1));
	system("cls");
	int Nums;
	printf("������Ҫд������������:");
	cin >> Nums;


	//��ȡ������Ϣ
	string filename,name, sex, age;
	int countSex;
	printf("�����������Ϣ\n");
	printf("����:"); cin >> name;
	do {
		printf("�Ա� (1. �� / 0. Ů) :");
		cin >> countSex;
		if		(countSex == 1)	sex = "mam";
		else if (countSex == 0) sex = "womem";
		else	cout << "�벻Ҫ����Ц! �㲻��������װֱ����!\n";
	} while (countSex != 1 && countSex != 0);
	printf("����:"); cin >> age;
	filename = name + ' ' + sex + ' ' + age;
	system("pause");


	while (Nums--)
	{
		facecheck.dectectface();
		system("pause");

		
	}

	facecheck.CreatFaceData(filename);
	run = false;
	camera.join();


    return 0;
}