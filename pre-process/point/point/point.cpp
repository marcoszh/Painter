// point.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <vector>
#include <limits>
#include <fstream>
#include <sstream>
#include <io.h>

using namespace cv;
using namespace std;

struct point
{
	int x;
	int y;
};

typedef vector<struct point> List;
typedef vector<List> OutLine;
//读取文件
void readFile(char fileName[],List *list)
{
    string str;
	char ch[10];
	int  temp;
	//计数
	struct point p;
	//定义文件指针
	ifstream fp(fileName,ios::in);

	if(!fp)
	{
		printf("file open fail!");
		exit(0);
	}
	while(!fp.eof())
	{
		//读入一行
        getline(fp,str);
		//分隔字符
        istringstream istr(str);
		//读入x y的值
        istr>>ch;
        temp=atoi(ch);
		if(temp==0)
			continue;
		p.x = temp;

		istr>>ch;
        temp=atoi(ch);
		p.y = temp;
		//放入容器
		list->push_back(p);
    }
	fp.close();
}
//搜寻指定文件的文件
int findFile(char FilePath[],char FileName[][40])
{
	long handle;
	int FileCount=0;
	struct _finddata_t fileinfo;
	//找到第一个文件
	handle=_findfirst(FilePath,&fileinfo);
	if (handle == -1)
	{
		printf("%d\n",handle);
		printf("搜索失败!");
		exit(0);
	}
	strcpy(FileName[FileCount],(const char*)fileinfo.name);
	//printf("%s\n",FileName[FileCount]);
	FileCount ++;
	//找到接下来的文件
	while(!_findnext(handle,&fileinfo))
	{
		strcpy(FileName[FileCount],(const char*)fileinfo.name);
		//printf("%s\n",FileName[FileCount]);
		FileCount++;
	}

	_findclose(handle);

	return FileCount;
}
//写入文件
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
int main()
{
	//指定文件名数组大小
	char FileName[30][40];
	int fileNum = findFile("E:\\project_su\\point\\point\\橙\\*.*",FileName);

    OutLine line;
	List list;
	for(int i= 0 ; i < fileNum ;i++)
	{
		char filename[60] = "E:\\project_su\\point\\point\\橙\\";

		if(FileName[i][0] == '.')
			continue;

		strcat(filename,FileName[i]);
		//printf("%s",filename);

		readFile(filename,&list);
		//
		line.push_back(list);
		list.clear();
	}
	//
	int left = 500;//最左面的位置
	int up = 500;  //最上面的位置
	for(int i=0 ;i< line.size();i++)
	{
		for(int j= 0;j < line.at(i).size() ;j++)
		{
			if(line.at(i).at(j).x < left)
				left = line.at(i).at(j).x;
			if(line.at(i).at(j).y < up)
				up = line.at(i).at(j).y;
		}
	}
	printf("%d\t%d\n",up,left);

	for(int i=0 ;i< line.size();i++)
	{
		for(int j= 0;j < line.at(i).size() ;j++)
		{
			line.at(i).at(j).x =line.at(i).at(j).x - left+1;
			line.at(i).at(j).y =line.at(i).at(j).y - up+1;
		}
	}

	//
	IplImage *panel = cvCreateImage(cvSize(600,600),8,1);

	cvSet(panel,cvScalarAll(255));

	int x_x[]={-1,0,1,-1,0,1,-1,0,1};
	int y_y[]={-1,-1,-1,0,0,0,1,1,1};

	cvNamedWindow("src");
	cvShowImage("src",panel);
	int count = 0;
	for(int i=0 ;i< line.size();i++)
	{
		for(int j= 0;j < line.at(i).size() ;j++)
		{	
			for(int k = 0;k<9;k++)
			panel->imageData[(line.at(i).at(j).y+y_y[k]) *panel->widthStep + line.at(i).at(j).x + x_x[k]] = 0;
			count++;
			if(count == 10)
			{
				cvShowImage("src",panel);
				count = 0;
				cvWaitKey(50);
			}
		}
		cvShowImage("src",panel);
		count = 0;
		cvWaitKey(20);
	}
	//
	for(int i=0 ;i< line.size();i++)
	{
		char name[5];
		itoa(i,name,10);
		write_file(name,line,i);
	}
	cvShowImage("src",panel);
	cvWaitKey(0);

	cvDestroyWindow("src");
	cvReleaseImage(&panel);

	return 0;
}