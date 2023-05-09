#pragma once

#include<iostream>
#include<vector>
#include<mutex>
#include<opencv2/opencv.hpp>
#include<arcsoft_face_sdk.h>

#define APPID (char*)"4UVZzzZSKsWwEBMuKrBFyfqhYVmAUGFwFDmUik9kDJDN"
#define SDKKey (char*)"CVZRULqDSFRRLpd3nxW7TYH2Zjmxayd4WAzYS8Yb3vHP"
#define FeatureNums 512
#define MaxFaceNum 1


//�洢������ά����
struct FACEPOINT {
	float*      feature;		// ����ֵ
	unsigned    type;			// ���
};

extern std::vector<FACEPOINT> _train_data_;
extern std::vector<std::string> _file_name_;
extern size_t Mpos;

void DetectFace(
	cv::VideoCapture& cap,
	bool& run,
	ASF_MultiFaceInfo& faceRect,
	std::mutex& mtx
);

int ArcFace(
	ASVLOFFSCREEN offscreen, 
	ASF_MultiFaceInfo face
);

void knn(
	float* face
);

int readtraindata(
	std::string path = "bin\\"
);