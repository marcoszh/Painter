#include "stdafx.h"
#include "opencv2/opencv.hpp"

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <limits>

using namespace cv;
using namespace std;

struct point
{
	int x;
	int y;
};
//
typedef vector<struct point> List;
typedef vector<List> OutLine;

//
void FindOther(IplImage *src,int yy,int xx,List *list)
{

	 int x_x[]={-1,0,1,-1,1,-1,0,1};
	 int y_y[]={-1,-1,-1,0,0,1,1,1};

     int x,y;

	 int width = src->width;
	 int height = src->height;
	 int widthStep = src->widthStep;
	 struct point p;
	 //�������������
	 for(int i=0;i<8;i++)
	 {
		 y=yy+ y_y[i];   //��ֱ����
		 x=xx+ x_x[i];   //ˮƽ����

		 //�ж��Ƿ�Խ��
		 if(y < 0 || y >= height ||x < 0 || x >= width)
			 continue;
		 //�ݹ����
		if((unsigned char)src->imageData[y*widthStep+x] > 160)
		{
			//��Ǹõ� ����ݹ�
			src->imageData[y*widthStep+x] = 64;

			p.x = x;
			p.y = y;
			list->push_back(p);
			//�ݹ����
			FindOther(src,y,x,list);
		}
	 }
}
//

//
void FindST(IplImage *src,OutLine *line)
 {
	 int width= src->width ;
	 int height = src->height;
	 int widthStep = src->widthStep;

	 int count = 0;

	 struct point p;
	 List list;
	 //�Ը��н��еݹ����
	 for(int j=0;j< height ;j++)
	 {
		 for(int i=0;i< width ;i++)
		 {
			 if((unsigned char)src->imageData[j*widthStep + i] > 125&&(unsigned char)src->imageData[j*widthStep + i] < 130)
			 {
				 //��ʼ������Χ��¼�ĵ�
				 p.x = i;
			     p.y = j;
				 list.push_back(p);
				 //��Ǹĵ�
				 src->imageData[j*widthStep + i] = 64;
				 //����ݹ麯��
				 FindOther(src,j,i,&list);
				 line->push_back(list); 
				 list.clear();
			 }
		 }
	 }
}
//�������д���ļ� ���Ժ��������ʹ��
void write_file(char filename[],OutLine line,int num)
{
	FILE *fp;
	fp = fopen(filename,"w");
	if(!fp)
		printf("err");
	for(int j = 0; j< line.at(num).size();j++)
	{
		//д�ļ�
		fprintf(fp,"%d %d\n",line.at(num).at(j).x,line.at(num).at(j).y);
	}
	fclose(fp);
}
//
void main()
{
	//����ͼ��
	IplImage *src = cvLoadImage("F://0111.bmp",0);

	IplImage *panel = cvCreateImage(cvSize(src->width,src->height),8,1);
	//��ʼ����ͼ��
	cvSet(panel,cvScalarAll(255));

	//����������������
	OutLine line;
	FindST(src,&line);

	for(int i = 0;i<line.size();i++)
	{
		//Ѱ����Ҫ�������� ����5�����ֶ�����
		if(line.at(i).size()< 80)
			continue;
		for(int j = 0; j< line.at(i).size();j++)
			panel->imageData[line.at(i).at(j).y * panel->widthStep + line.at(i).at(j).x] = 0;
		//չʾ������
		cvNamedWindow("win");
		cvShowImage("win",panel);
		cvWaitKey(0);
		cvDestroyWindow("win");
		//����д���ļ�
		char name[5];
		itoa(i,name,10);
		write_file(name,line,i);
	}

	//cvSaveImage("F://0444.bmp",newSrc);
	cvReleaseImage(&src);
	cvReleaseImage(&panel);
}