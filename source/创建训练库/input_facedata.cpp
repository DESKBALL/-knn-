
#include <iostream>
#include <fstream>
#include <cstring>

#include <opencv2/opencv.hpp>
#include <arcsoft_face_sdk.h>

using namespace std;
using namespace cv;

#include "input_facedata.h"


input_facedata::input_facedata
	(int index){
	
	//��ʼ������ͷ
	cap = VideoCapture(index, CAP_ANY);
	if (!cap.isOpened()) {
		printf("Error: failed to VideoCaptrue.\n");
		exit(1);
	}


	//��֤Arcsoft֤��
	MRESULT res = ASFOnlineActivation(APPID, SDKKey);
	if (res != 0 /*���*/ && res != 0x16002 /*SDK�Ѽ���*/) {
		printf("Error: failed to activate license. stop(0x%08x)", res);
		exit(1);
	}

}


input_facedata::~input_facedata() {

	cap.release();          // �ͷ���ʹ�õ� VideoCapture ����
	destroyAllWindows();    // �ر����д򿪵� OpenCV ����
}



void input_facedata::showimage(bool& run) 
{
	Mat os;
	while (run)
	{
		cap >> os;
		imshow("����ͷ", os);		
		waitKey(1);
		this_thread::sleep_for(chrono::microseconds(40));
	}
}


bool input_facedata::dectectface()
{
	//��ʼ������
	//����������������
	MHandle handle = NULL;
	MRESULT res = ASFInitEngine(ASF_DETECT_MODE_IMAGE, ASF_OP_0_ONLY,
		32, MaxFaceNum, ASF_FACE_DETECT | ASF_FACERECOGNITION, &handle);
	if (res != 0) {
		printf("Error: failed to initialize Engine. stop(0x%08x)\n", res);
		return false;
	}

	//��ȡͼ��
	Mat os;
	cap >> os;
	if (os.empty()) {
		printf("Error: failed to cap. \n");
		return false;
	}

	//��������
	ASVLOFFSCREEN offscreen = { 0 };
	offscreen.i32Width = os.cols;
	offscreen.i32Height = os.rows;
	offscreen.u32PixelArrayFormat = ASVL_PAF_RGB24_B8G8R8;;
	offscreen.pi32Pitch[0] = os.step;
	offscreen.ppu8Plane[0] = os.data;

	//��׽����
	bool is_getfeatrue = false;
	ASF_MultiFaceInfo faceRect = { 0 };
	do{
		//ѭ��ֱ���ɹ���������
		res = ASFDetectFacesEx(handle, &offscreen, &faceRect);
		if (res == 0) {
			//��⵽��������ú�����ȡ����
			is_getfeatrue = facefeature(offscreen, faceRect);
		}
	} while (res != 0 || !is_getfeatrue);
	
	//�ͷ���Դ
	ASFUninitEngine(handle);
	handle = NULL;
	return true;
}


bool input_facedata::facefeature(
	ASVLOFFSCREEN offscreen, ASF_MultiFaceInfo face)
{
	//��ʼ��ͼ������
	//����ģʽΪ������ȡ
	MHandle handle = NULL;
	MRESULT res = ASFInitEngine(ASF_DETECT_MODE_IMAGE, ASF_OP_0_ONLY,
		32, MaxFaceNum, ASF_FACERECOGNITION | ASF_FACE_DETECT, &handle);
	if (res != 0) {
		printf("Error: failed to initialize Engine. stop(0x%08x)\n", res);
		return -1;
	}

	//��Rect���ݴ�������
	ASF_SingleFaceInfo signedfaceinfo;
	signedfaceinfo.faceRect = *face.faceRect;
	signedfaceinfo.faceOrient = *face.faceOrient;

	//��������
	//��ʼ��ȡ����
	ASF_FaceFeature face_features = { 0 };
	res = ASFFaceFeatureExtractEx(handle, &offscreen, &signedfaceinfo, &face_features);
	if (res != 0) {
		printf("Error: failed to detect. stop(0x%08x)\n", res);
		return false;
	}
	else
	{	//����ɹ�����ӡ�������ֵ
		float* Vfeature = (float*)face_features.feature;
		size_t lenght = face_features.featureSize / sizeof(float);

		system("cls");
		printf("����ɹ�!\n");
		for (size_t i = 0; i < lenght; i++)
		{
			printf("%f ",Vfeature[i]);
		}
		putchar('\n');

		//�洢����
		ASF_FaceFeature buffer = { 0 };
		buffer.feature = (MByte*)calloc(1,face_features.featureSize);
		memcpy(buffer.feature, face_features.feature, face_features.featureSize);
		buffer.featureSize = face_features.featureSize;
		
		//��ӵ����������
		Data.push_back(buffer);
	}

	//�ͷ�����ռ�õ��ڴ�ռ�
	ASFUninitEngine(handle);
	handle = NULL;
	return true;
}

void input_facedata::CreatFaceData(string filename)
{
	//����ѵ�����ļ�
	filename = "bin\\" + filename + ".dat";
	ofstream writeData(filename, ios::app | ios::binary);

	size_t filesize = 0;	// ��¼д���ֽ���

	//д��
	for (auto& face : Data)
	{
		//��������
		size_t feature_len = face.featureSize / sizeof(float);

		//������ͷ�������ռ�ô�С
		writeData.write((char*)&face.featureSize, sizeof(int));

		//д����������
		writeData.write((char*)face.feature, face.featureSize);

		//��¼д��������
		filesize = filesize + sizeof(int) + face.featureSize;

		//�ͷ��ڴ�
		free(face.feature);
	}

	writeData.close();
	Data.clear();
	system("cls");
	printf("���, �ܹ�д���ֽ���: %llu\n",filesize);
}



