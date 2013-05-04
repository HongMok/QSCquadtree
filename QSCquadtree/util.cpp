#include "functions.h"

/************************************************************************/
/* 
	输出树节点内容，深度遍历
*/
/************************************************************************/
void display(Node *root)
{
	
	if(root->isLeaf())
	{
		cout<<"1 ";
	}
	else
	{
		cout<<"0 ";
		display(root->nw);
		display(root->ne);
		display(root->sw);
		display(root->se);
	}
	
}


/************************************************************************/
/* 
	计算PSNR(peak signal to noise ratio)，峰值信噪比
*/
/************************************************************************/
double PSNR ( IplImage *&img1 , IplImage *&imggest ) //计算PSNR

{
	int M = img1->height;
	int N = img1->width;

	double temp=0;
	double temp1=0;

	for(int y = 0;y<M;y++)
	{
		uchar* ptr1 = (uchar*) (img1->imageData+y*img1->widthStep);
		uchar* ptrg = (uchar*) (imggest->imageData+y*imggest->widthStep);

		for (int x = 0;x<N;x++)
		{
			temp+=pow((double)(ptr1[x]-ptrg[x]),2);
		}
	}


	temp1=10*log10 (255.0*255*M*N/temp);
	return (temp1);


}


/************************************************************************/
/* 
	计算BPP(bits per pixel),每像素色彩位
*/
/************************************************************************/
double BPP ( vector<grayLevels> &P ,int M , int N , vector<char> &Q )  //计算BPP

{

	double b = (double)(Q.size()+40.0*P.size())/(M*N);
	return b;
}


/************************************************************************/
/* 
	显示分割示意图
*/
/************************************************************************/
void showSegmentation(IplImage *img1, vector<couplePoints> &C)
{
	//画示意图
	cvNamedWindow("分割示意图",CV_WINDOW_AUTOSIZE);

		//创建素描图像
		IplImage* sketch = cvCreateImage(cvGetSize(img1), IPL_DEPTH_8U,1); 
		for(int y = 0;y<img1->height;y++)             
		{
			uchar* ptrsketch = (uchar*) (sketch->imageData+y*sketch->widthStep);

			for (int x = 0;x<img1->width;x++)
			{
				ptrsketch[x]=255;
			}
		}
		//素描图像初始化完成
		for (int i=0 ; i<C.size();i++)
		{
			if (C[i].x1==0&&C[i].y1==0)
				cvRectangle(sketch,cvPoint(C[i].x1,C[i].y1),cvPoint(C[i].x2,C[i].y2),cvScalar(0x00,0x00,0x00));
			else   if (C[i].x1==0&&C[i].y1!=0)
				cvRectangle(sketch,cvPoint(C[i].x1,C[i].y1-1),cvPoint(C[i].x2,C[i].y2),cvScalar(0x00,0x00,0x00));
			else   if (C[i].x1!=0&&C[i].y1==0)
				cvRectangle(sketch,cvPoint(C[i].x1-1,C[i].y1),cvPoint(C[i].x2,C[i].y2),cvScalar(0x00,0x00,0x00));
			else   if (C[i].x1!=0&&C[i].y1!=0)
				cvRectangle(sketch,cvPoint(C[i].x1-1,C[i].y1-1),cvPoint(C[i].x2,C[i].y2),cvScalar(0x00,0x00,0x00));

		}
		cvShowImage("分割示意图",sketch);//载入转化后的灰度图像 
}