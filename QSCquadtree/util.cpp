#include "functions.h"

/************************************************************************/
/* 
	������ڵ����ݣ���ȱ���
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
	����PSNR(peak signal to noise ratio)����ֵ�����
*/
/************************************************************************/
double PSNR ( IplImage *&img1 , IplImage *&imggest ) //����PSNR

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
	����BPP(bits per pixel),ÿ����ɫ��λ
*/
/************************************************************************/
double BPP ( vector<grayLevels> &P ,int M , int N , vector<char> &Q )  //����BPP

{

	double b = (double)(Q.size()+40.0*P.size())/(M*N);
	return b;
}


/************************************************************************/
/* 
	��ʾ�ָ�ʾ��ͼ
*/
/************************************************************************/
void showSegmentation(IplImage *img1, vector<couplePoints> &C)
{
	//��ʾ��ͼ
	cvNamedWindow("�ָ�ʾ��ͼ",CV_WINDOW_AUTOSIZE);

		//��������ͼ��
		IplImage* sketch = cvCreateImage(cvGetSize(img1), IPL_DEPTH_8U,1); 
		for(int y = 0;y<img1->height;y++)             
		{
			uchar* ptrsketch = (uchar*) (sketch->imageData+y*sketch->widthStep);

			for (int x = 0;x<img1->width;x++)
			{
				ptrsketch[x]=255;
			}
		}
		//����ͼ���ʼ�����
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
		cvShowImage("�ָ�ʾ��ͼ",sketch);//����ת����ĻҶ�ͼ�� 
}