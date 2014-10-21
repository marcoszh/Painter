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
	 //八邻域进行搜索
	 for(int i=0;i<8;i++)
	 {
		 y=yy+ y_y[i];   //垂直方向
		 x=xx+ x_x[i];   //水平方向

		 //判断是否越界
		 if(y < 0 || y >= height ||x < 0 || x >= width)
			 continue;
		 //递归查找
		if((unsigned char)src->imageData[y*widthStep+x] > 160)
		{
			//标记该点 进入递归
			src->imageData[y*widthStep+x] = 64;

			p.x = x;
			p.y = y;
			list->push_back(p);
			//递归调用
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
	 //对该行进行递归查找
	 for(int j=0;j< height ;j++)
	 {
		 for(int i=0;i< width ;i++)
		 {
			 if((unsigned char)src->imageData[j*widthStep + i] > 125&&(unsigned char)src->imageData[j*widthStep + i] < 130)
			 {
				 //初始轮廓外围记录的点
				 p.x = i;
			     p.y = j;
				 list.push_back(p);
				 //标记改点
				 src->imageData[j*widthStep + i] = 64;
				 //进入递归函数
				 FindOther(src,j,i,&list);
				 line->push_back(list); 
				 list.clear();
			 }
		 }
	 }
}
//点的数据写入文件 待以后拟合曲线使用
void write_file(char filename[],OutLine line,int num)
{
	FILE *fp;
	fp = fopen(filename,"w");
	if(!fp)
		printf("err");
	for(int j = 0; j< line.at(num).size();j++)
	{
		//写文件
		fprintf(fp,"%d %d\n",line.at(num).at(j).x,line.at(num).at(j).y);
	}
	fclose(fp);
}
//
void main()
{
	//加载图像
	IplImage *src = cvLoadImage("F://0111.bmp",0);

	IplImage *panel = cvCreateImage(cvSize(src->width,src->height),8,1);
	//初始化画图板
	cvSet(panel,cvScalarAll(255));

	//定义轮廓储存容器
	OutLine line;
	FindST(src,&line);

	for(int i = 0;i<line.size();i++)
	{
		//寻找需要的轮廓线 参数5可以手动设置
		if(line.at(i).size()< 80)
			continue;
		for(int j = 0; j< line.at(i).size();j++)
			panel->imageData[line.at(i).at(j).y * panel->widthStep + line.at(i).at(j).x] = 0;
		//展示轮廓线
		cvNamedWindow("win");
		cvShowImage("win",panel);
		cvWaitKey(0);
		cvDestroyWindow("win");
		//创建写入文件
		char name[5];
		itoa(i,name,10);
		write_file(name,line,i);
	}

	//cvSaveImage("F://0444.bmp",newSrc);
	cvReleaseImage(&src);
	cvReleaseImage(&panel);
}