#pragma once

#include <iostream>
#include <fstream>

#include <opencv2/opencv.hpp>
#include <arcsoft_face_sdk.h>

#define APPID (char*)"4UVZzzZSKsWwEBMuKrBFyfqhYVmAUGFwFDmUik9kDJDN"
#define SDKKey (char*)"CVZRULqDSFRRLpd3nxW7TYH2Zjmxayd4WAzYS8Yb3vHP"
#define FeatureNums 512
#define MaxFaceNum 1


/*----------------------------------------------------------------------------*/

class input_facedata
{
private:
	cv::VideoCapture cap;						// ����ͷ
	

public:
	vector<ASF_FaceFeature> Data;				// ����д�������

	//��ʼ������
	input_facedata(
		int index = 0							// ����ͷID
	);

	//�ͷ�ռ��
	~input_facedata();							

	//�������ͷ����
	void showimage(
		bool& run								//��������ͷ����
	);

	//��������
	bool dectectface();

	//������ȡ
	bool facefeature(
		ASVLOFFSCREEN offscreen,				// ͼ����Ϣ
		ASF_MultiFaceInfo face					// ������Ϣ
	);

	//��������
	void CreatFaceData(
		string filename							// ѵ������ǩ
	);
};

