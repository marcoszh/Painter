// draw_test.cpp : 定义控制台应用程序的入口点。
//

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

typedef vector<struct point> List;
typedef vector<List> OutLine;

//采用大津算法决定阈值
int Otsu(IplImage *src)
{
	int width = src ->width;
	int height = src->height;
	int widthStep = src->widthStep;

	int hist[256] ={0};
	int MatrixHist[256] = {0};

	//统计直方图
	for(int i = 0 ;i< width ;i++)
	{
		for(int j = 0; j< height ;j++)
		{
			hist[(uchar)(src->imageData[j*widthStep + i])]++;
		}
	}
	//计算质量矩
	int count=0;
	for(int i=0;i<255 ;i++)
	{
		MatrixHist[i]=i*hist[i];
		count += MatrixHist[i];
	}

	float u1;
	float u2;
	int theld = 1;
	float sb=0.0;
	float max_sb=0.0;
	float sum1=hist[0];
	float sum2;
	float sum = width*height;
	float Mah1=MatrixHist[0]; //计算质量矩
	float Mah2=0;
	//
	for(int i = 1 ;i<256 ;i++)
	{
		//计算数目以及质量矩
	    sum1 += hist[i];
		Mah1 += MatrixHist[i];

		if(sum1 > 0)
		{
			//计算总的比例
			sum2 = sum - sum1;
			Mah2 = count - Mah1;
			if(sum2==0)
				break;
			//计算期望
			u1 = Mah1 /sum1;
			u2 = Mah2 /sum2;

			sb = sum1/sum * sum2/sum *(u1-u2)*(u1-u2);//方差
			if(sb > max_sb)
			{
				max_sb = sb;
				theld = i;
			}
		}
	}
		
	return theld;
}
//采用k均值聚类算法确定阈值
int getTheld(IplImage *src)
{
	int width = src ->width;
	int height = src->height;
	int widthStep = src->widthStep;
	int sum = width * height;

	int hist[256] ={0};
	int MatrixHist[256] = {0};

	//统计直方图
	for(int i = 0 ;i< width ;i++)
	{
		for(int j = 0; j< height ;j++)
		{
			hist[(uchar)(src->imageData[j*widthStep + i])]++;
		}
	}
	//计算质量矩
	int theld =-1;
	MatrixHist[0] = hist[0];
	for(int i=1; i<=255 ;i++)
	{
		MatrixHist[i] = hist[i] + MatrixHist[i-1];
		hist[i]  = i* hist[i];
		if(MatrixHist[i] > sum/2 && theld == -1)
			theld = i;
	}
	//初始中心1 和 2
	double cent_one;
	double cent_two;
	int sum1;
	int sum2;
	int temp;
	//尽心迭代寻找聚类中心
	while(1)
	{
		sum1 = 0;
		sum2 = 0;
		//第一类中心
		for(int i = 0;i <= theld;i++)
			sum1 +=hist[i];
		cent_one = sum1/(float)MatrixHist[theld];

		//第二类中心
		for(int i = theld+1;i <= 255;i++)
			sum2 +=hist[i];
		cent_two = sum2/(float)(sum - MatrixHist[theld]);
		//计算新的阈值
	    temp = int(cent_two+cent_one)/2;
		//和上一次阈值比较
		if(temp == theld)
			break;
		else
			theld = temp;
	}
	return theld;
}
//二值化 阈值有上面两种阈值算法选一种
void OtsuTheld(IplImage *src,IplImage *new_src)
{
	int width = src->width;
	int height = src ->height;
	int widthStep = src->widthStep;

	//选用大津
	int theld = Otsu(src) - 120;//120的常量视情况可以改变
	//用聚类
	//int theld = getTheld(src) ;


	for(int i = 0;i<width ;i++)
	{
		for(int j = 0;j<height ;j++)
		{
			if( uchar(src->imageData[j*widthStep + i]) >= theld)
				new_src->imageData[j*widthStep + i] = 255;
			else 
				new_src->imageData[j*widthStep + i] = 0;
		}
	}
}

//
//寻找边缘
void FindOther(int *src_xy,int *src_st,IplImage *edg,int i,int j,int low)
 {
	 int x_x[8]={-1,0,1,-1,1,-1,0,1};
	 int y_y[8]={-1,-1,-1,0,0,1,1,1};
     int ii,jj;
	 int width = edg->width;
	 int height = edg->height;
	 for(int i=0;i<8;i++)
	 {
		 jj=j+ x_x[i];   //水平方向
		 ii=i+ y_y[i];   //竖直方向
		 if(ii >=0 && ii< height && jj>=0 && jj< width) //判断访存是否越界
		 {
			if(src_st[ii*width+jj]==128  &&  src_xy[ii*width+jj] > low && ((uchar *)(edg->imageData+ii*edg->widthStep))[jj] !=255 )
			{
				((uchar *)(edg->imageData+ii*edg->widthStep))[jj] =255;
				FindOther(src_xy,src_st,edg,ii,jj,low);
			}
		 }
	 }
 }
//寻找边缘起点
 void FindST(int *src_xy,int *src_st,IplImage *edg,int high ,int low)
 {
	 int width= edg->width ;
	 int height = edg->height ;
	 for(int i=0;i< height;i++)
	 {
		 for(int j=0;j< width ;j++)
		 {
			 if(src_xy[i*width+j] >= high && src_st[i*width+j] ==128 && ((uchar *)(edg->imageData+i*edg->widthStep))[j] !=255 )
			 {
				 ((uchar *)(edg->imageData+i*edg->widthStep))[j] =255;
				 FindOther(src_xy,src_st,edg,i,j,low);
			 }
			 if(((uchar *)(edg->imageData+i*edg->widthStep))[j] !=255)
				 ((uchar *)(edg->imageData+i*edg->widthStep))[j] =0;  
		 }
	 }
 }
void canny(IplImage *src,IplImage *edg)
{
	//获取图片长宽度
	int width = src->width;
	int height = src->height;
	//储存平滑结果
	int *src_sm;
	//储存水平 垂直 二阶泛数梯度
	int *src_x;
	int *src_y;
	int *src_xy;
	//极大值点
	int *src_st;
	//直方图
	int *src_hist;
	//申请内存
	src_sm = (int*)malloc(sizeof(int) * width * height);
	src_x = (int*)malloc(sizeof(int) * width * height);
	src_y = (int*)malloc(sizeof(int) * width * height);
	src_xy = (int*)malloc(sizeof(int) * width * height);
	src_st = (int*)malloc(sizeof(int) * width * height);
	src_hist = (int*)malloc(sizeof(int) * 256);
	//图像高斯平滑
	//cvSmooth(src,sm);
	for(int i=0;i < height;i++)
	{
		for(int j=0;j < width ;j++)
		{
			if(i==0||i==(height-1)||j==0||j==(width-1))
			{
				src_sm[i*width+j] =((uchar *)(src->imageData+i*src->widthStep))[j]; 
			}
			else
				src_sm[i*width+j] = (((uchar *)(src->imageData+i*src->widthStep))[j]*4
				                  + ((uchar *)(src->imageData+(i-1)*src->widthStep))[j-1]
			                      + ((uchar *)(src->imageData+(i-1)*src->widthStep))[j]*2
								  + ((uchar *)(src->imageData+(i-1)*src->widthStep))[j+1]
								  + ((uchar *)(src->imageData+(i)*src->widthStep))[j-1]*2
								  + ((uchar *)(src->imageData+(i)*src->widthStep))[j+1]*2
								  + ((uchar *)(src->imageData+(i+1)*src->widthStep))[j-1]
								  + ((uchar *)(src->imageData+(i+1)*src->widthStep))[j]*2
								  + ((uchar *)(src->imageData+(i+1)*src->widthStep))[j+1])/16;
		}
	}
	//cvCanny(sm,edg,100,60);
	//计算x放向导数
	for(int i=0;i < height;i++)
	{
		for(int j=0;j < width ;j++)
		{
			if(j==0||j==(width-1))
				src_x[i*width+j]=0;
			else
				src_x[i*width+j]=src_sm[i*width+j+1] - src_sm[i*width+j-1];
		}
	}
	//计算y方向梯度
	for(int i=0;i < height;i++)
	{
		for(int j=0;j < width ;j++)
		{
			if(i ==0||i==(height-1))
				src_y[i*width+j]=0;
			else
				src_y[i*width+j]=src_sm[(i+1)*width+j] - src_sm[(i-1)*width+j];
		}
	}
	//计算二阶范数梯度
	for(int i=0;i < height;i++)
	{
		for(int j=0;j < width ;j++)
		{
			src_xy[i*width+j] = (int)sqrt((float)(src_x[i*width+j]*src_x[i*width+j]+src_y[i*width+j]*src_y[i*width+j])+0.5);
		}
	}
	//非最大抑制
	int gx,gy;
	int g1,g2,g3,g4;
	float weight;   //中间变量
	float dT,dT1,dT2;

	for(int i=0,k=0;i < height;i++)
	{
		for(int j=0;j < width ;j++)
		{
		   k=i*width +j;
		   if(i==0||i==(height-1)||j==0||j==(width-1))
			   src_st[k]=0;
		   else
		   {
			   if(src_xy[k]==0)
				   src_st[k]=0;
			   else
			   {
				   gx=src_x[k];
				   gy=src_y[k];
				   if(abs(gy)>abs(gx))
				   {
					   weight =fabs(float(gx)/float(gy));
					   g2 = src_xy[k-width];
					   g4 = src_xy[k+width];
					   if(gx*gy>0)
					   {
						   g1 = src_xy[k-width-1];
						   g3 = src_xy[k+width+1];
					   }
					   else
					   {
						   g1 = src_xy[k-width+1];
						   g3 = src_xy[k+width-1];
					   }
				   }
				   else
				   {
					   weight =fabs(float(gy)/float(gx));
					   g2 = src_xy[k+1];
					   g4 = src_xy[k-1];
					   if(gx*gy>0)
					   {
						   g1 = src_xy[k+width+1];
						   g3 = src_xy[k-width-1];
					   }
					   else
					   {
						   g1 = src_xy[k-width+1];
						   g3 = src_xy[k+width-1];
					   }
				   }
				   dT = src_xy[k];
				   dT1 = weight*g1 + (1-weight)*g2; 
				   dT2 = weight*g3 + (1-weight)*g4; 
				   if(dT >= dT1 && dT >= dT2)
					   src_st[i*width+j] = 128; 
				   else 
					   src_st[i*width+j] = 0; 
			   }
		   }
		}
	}
	//统计直方图
	for(int i=0 ;i<256 ;i++)
	{
		src_hist[i]=0;
	}
    //计算极值点
	for(int i=0;i < height;i++)
	{
		for(int j=0;j < width ;j++)
		{
			if(src_st[i*width+j]==128)
				src_hist[src_xy[i*width+j]]++;
		}
	}

	//计算直方图总数
	int count=0;
	int max = 0;
	for(int i=0 ;i<256 ;i++)
	{
		if(src_hist[i]!=0)
		{
			count+=src_hist[i];
			max=i;
		}
	}
	//最大阈值控制点数
	int highcount=0;
	highcount=(int)(count*0.5);
	//最大阈值
	int high=0;
	count = 0;//count置零 准备计数
	for(int i=0 ;i<max ;i++)
	{
		count+=src_hist[i];
		if(count>=highcount)
		{
			high = i-1;
			break;
		}
	}
	//最小阈值
	int low=0;
	low = (int)(high*0.5);
	//寻找边缘起点 以及边缘
	FindST(src_xy,src_st,edg,high,low);
	
	//结束计时
	//输出总时间

	free(src_x);
	free(src_y);
	free(src_xy);
	free(src_st);
}

void main()
{
	IplImage *src = cvLoadImage("F://022.bmp",0);

	IplImage *newSrc = cvCreateImage(cvSize(src->width,src->height),8,1);
	IplImage *edge = cvCreateImage(cvSize(src->width,src->height),8,1);
	//
	OtsuTheld(src,newSrc);
	//使用自适应canny算子对图像进行处理
	canny(newSrc,edge);
	//显示图像
	cvNamedWindow("win");
	cvShowImage("win",edge);
	cvWaitKey(0);
	cvDestroyWindow("win");

	//储存二值化图像
	//cvSaveImage("F://0222.bmp",newSrc);
	cvReleaseImage(&src);
	cvReleaseImage(&edge);
	cvReleaseImage(&newSrc);
}
