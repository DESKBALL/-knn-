//��ȡ��������
#include <iostream>
#include <opencv2/opencv.hpp>
#include <arcsoft_face_sdk.h>

using namespace std;
using namespace cv;

#include "detectface.h"


//���ⲿ����ͼ��Rect����
int ArcFace(ASVLOFFSCREEN offscreen, ASF_MultiFaceInfo face)
{
	//����ASFFaceFeatureExtractEx();
	// ��Ҫ 1.������  2.ͼ����Ϣ(ASVLOFFSCREEN�ṹ��) 3.����λ��(ASF_SingleFaceInfo)
	//һ��ASF_FaceFeatrue�����ڴ洢���
	 

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
	ASF_FaceFeature face_features = {0};
	res = ASFFaceFeatureExtractEx(handle, &offscreen, &signedfaceinfo, &face_features);
	if (res != 0) {
		printf("Error: failed to detect. stop(0x%08x)\n",res);
		return -1;
	}
	else
	{	//����ɹ����������ֵ
		size_t feature_len = face_features.featureSize / sizeof(float);
		float* Vfeature = (float*)face_features.feature;
		
		system("cls");
		printf("�ɼ�������:%llu, ����ֵ��\n",face_features.featureSize/sizeof(float));
		for (size_t i = 0; i < feature_len; i++)
		{	
			printf("%f, ",Vfeature[i]);
		}
		putchar('\n');

		knn(Vfeature);
		
	}


	//�ͷ�����ռ�õ��ڴ�ռ�
	ASFUninitEngine(handle);
	handle = NULL;
	

    return 0;
}