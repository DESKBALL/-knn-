//ʶ����������
#include<iostream>
#include<thread>
#include<opencv2/opencv.hpp>
#include "arcsoft_face_sdk.h"  

using namespace std;
using namespace cv;

#include"detectface.h"


//ʹ��Arcface SDK
void DetectFace(VideoCapture& cap, bool& run,
	ASF_MultiFaceInfo& faceRect, mutex& mtx)
{
	//���߳̽�������ת
	//ʹ��ASFDetectFacesEx��ȡRect����
	//���(�������ģʽ)��ͼ������ASF_MultiFaceInfo���ڴ洢out

	

	//��ʼ������
	//����������������
	MHandle handle = NULL;
	MRESULT res = ASFInitEngine(ASF_DETECT_MODE_IMAGE, ASF_OP_0_ONLY,
		32, MaxFaceNum, ASF_FACE_DETECT | ASF_FACERECOGNITION, &handle);
	if (res != 0) {
		printf("Error: failed to initialize Engine. stop(0x%08x)\n", res);
		return;
	}
	

	//�������
	Mat os;
	while (run)
	{
		//��ȡͼ��
		cap >> os;
		if (os.empty()) {
			printf("Error: failed to cap. \n");
			return;
		}

		ASVLOFFSCREEN offscreen = { 0 };
		offscreen.i32Width = os.cols;
		offscreen.i32Height = os.rows;
		offscreen.u32PixelArrayFormat = ASVL_PAF_RGB24_B8G8R8;;
		offscreen.pi32Pitch[0] = os.step;
		offscreen.ppu8Plane[0] = os.data;


		//��׽����
		{
			lock_guard<mutex> mlock(mtx);

			faceRect = { 0 };
			res = ASFDetectFacesEx(handle, &offscreen, &faceRect);
			if (res != 0 && res != 0x15005) {
				printf("Not face!\n");
			}
			else
			{
				ArcFace(offscreen, faceRect);
			}
		}
		

		this_thread::sleep_for(chrono::milliseconds(500));
	}

	ASFUninitEngine(handle);
}