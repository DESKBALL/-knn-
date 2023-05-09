
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <windows.h>

#include<opencv2/opencv.hpp>
#include<arcsoft_face_sdk.h>

using namespace std;
using namespace cv;

#include "detectface.h"


int main()
{
	//��ʼ������ͷ
	int index = 0;
	VideoCapture cap = VideoCapture(index);

	//��ȡѵ����
	readtraindata();

	//�����߳�
	mutex mtx;								// ����������
	bool runThread = true;					// �����߳�����
	ASF_MultiFaceInfo faceRect = { 0 };		// �洢��������
	thread detectFace(DetectFace,
		ref(cap), ref(runThread), ref(faceRect), ref(mtx)); //�����߳�

	//��ʾ����
	Mat os;
    while (true)
    {
		//������ͷ��������Mat���� 
		cap >> os;
        {
            lock_guard<mutex> mlock(mtx);
			
			Rect cvRect; 

			if (faceRect.faceRect && Mpos >= 0 && Mpos < _file_name_.size())
			{
				//MRECTתRECT
				cvRect.x = faceRect.faceRect->left;
				cvRect.y = faceRect.faceRect->top;
				cvRect.width = faceRect.faceRect->right - faceRect.faceRect->left;
				cvRect.height = faceRect.faceRect->bottom - faceRect.faceRect->top;

				//�������
				rectangle(os, cvRect, Scalar(0x02, 0x03, 0xFE), 2);

				//��ӡ������Ϣ
				string faceInfo=_file_name_[Mpos];
				putText(os, faceInfo, Point(cvRect.x, cvRect.y + cvRect.height + 25),
					FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0x02, 0x03, 0xFE), 2);
			}
        }

        //��ʾ����
        imshow("detectfaces",os);

        if (waitKey(1) == '\r') {
            break;
        }

        //����
        this_thread::sleep_for(chrono::milliseconds(40));
    }

	//�ͷ�ѵ����ռ�ÿռ�
	for (auto& freedata : _train_data_)
	{
		free(freedata.feature);
	}

    //�˳��߳�
    runThread = false;
    detectFace.join();
    return 0;
}