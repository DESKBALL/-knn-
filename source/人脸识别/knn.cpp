
#include <io.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <filesystem>
#include <arcsoft_face_sdk.h>

using namespace std;
using namespace cv;

#include "detectface.h"

size_t typeNums;
size_t Nums;

//����ŷʽ����
double CountDistance(float* target, float* point, size_t len = 258)
{
	double D = 0.0f;
	for (size_t i = 0; i < len; i++)
	{
		//����
		D = D + pow(target[i] - point[i], 2);
	}

	return sqrt(D);
}


void knn(float* face)
{
	typeNums = _file_name_.size();
	Nums = _train_data_.size();

	vector<pair<double,int>> distance;	//����������
	for (size_t i = 0; i < Nums; i++)
	{
		float* facepoint = _train_data_[i].feature;				//ȡ������
		double D = CountDistance(face, facepoint);				//����ŷʽ����
		distance.push_back(make_pair(D,_train_data_[i].type));	//��ӵ�������
	}	


	//k����
	const int K = 3;	//�����㷨KֵΪ3
	int Maxpos = 0;		//�������
	//count��¼ǰK�������и��������ִ�������������������
	vector<pair<size_t,double>> count(typeNums);
	//��ʼ��count
	for (size_t i = 0; i < typeNums; i++)
	{
		count[i].first = 0;
		count[i].second = SIZE_MAX;
	}

	//����
	sort(distance.begin(), distance.end());
	//�������K�����������ִ���
	for (size_t i = 0; i < K && i < Nums; i++)
	{
		count[distance[i].second].first+=1;
		if (count[distance[i].second].second > distance[i].first) {
			count[distance[i].second].second = distance[i].first;
		}
	}
	//�ҵ�������
	for (size_t i = 0; i < typeNums; i++)
	{
		if (count[i].first > count[Maxpos].first) {
			Maxpos = i;
		}
	}

	double dis = count[Maxpos].second;
	printf("%.2lf\n", dis);
	if (dis < 1.0f)
		Mpos = Maxpos;
	else
		Mpos = -1;
	
	count.clear();
}