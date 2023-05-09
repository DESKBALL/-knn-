
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>

using namespace std;

#include "detectface.h"


int readtraindata(string path)
{
	size_t type = 0;

	for (const auto& entry : filesystem::directory_iterator(path))
	{
		//��Ѱ·����.dat��׺�ļ�
		if (entry.path().extension() == ".dat")
		{
			//���ļ�
			ifstream in_file(entry.path(), ios::binary);
			if (!in_file.is_open()) {
				printf("Error: failed to opened traindata.\n");
				return -1;
			}

			//��ȡ�ļ����е���Ϣ
			string this_face=entry.path().filename().string();
			this_face.erase(this_face.size() - 4, 4);
			_file_name_.push_back(this_face);

			//��ȡ����
			int sizeNums = 0;			// �����ֽ���
			float* feature = NULL;		// ����

			while (in_file.read((char*)&sizeNums, sizeof(MInt32)))
			{
				//����Ѵ�������
				feature = (float*)malloc(sizeNums);
				
				//��ȡ����
				in_file.read((char*)feature, sizeNums);

				//��������������
				FACEPOINT buffer;
				buffer.feature = feature;
				buffer.type = type;
				_train_data_.push_back(buffer);
			}
			
			type++;
			in_file.close();
		}
	}

	return type;
}