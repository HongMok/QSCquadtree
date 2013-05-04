#include "functions.h"


/************************************************************************/
/* 
参数配置：

	1.运行程序名称
	2.原始图像地址
	3.epsilon
	4.保存原始灰度图像地址
	5.保存解码后图像地址
*/
/************************************************************************/
int main(int argc, char **argv)
{
	if(argc != 5)
		return -1;

	char *filename = argv[1];
	double epsilon = atof(argv[2]);
	char *save_srcimg = argv[3];
	char *save_desimg = argv[4];

	IplImage *srcImg = cvLoadImage(filename, 0);

	if(srcImg == NULL)
	{
		cout<<"输入图像为空"<<endl;
		return 0;
	}

	cvNamedWindow("source image", CV_WINDOW_AUTOSIZE);
	cvShowImage("source image", srcImg);
	cvSaveImage(save_srcimg, srcImg);
	vector<char> lineartree;
	vector<grayLevels> transmission;
	vector<couplePoints> psV;

	//encoding
	time_t start_t = clock();
	qsc_encoding(srcImg, lineartree, transmission, psV, epsilon);
	time_t end_t = clock();
	cout<<"encoding time: "<<end_t - start_t<<" ms"<<endl;
	/*
	for(int i=0; i < lineartree.size(); i++)
	{
		cout<<lineartree[i]<<" ";
	}
	cout<<endl;
	for(int i=0; i < transmission.size(); i++)
	{
		cout<<"u:"<<(int)transmission[i].u<<" ";
		cout<<"b:"<<(int)transmission[i].b<<" ";
		cout<<"l:"<<(int)transmission[i].l<<" ";
		cout<<"r:"<<(int)transmission[i].r<<" ";
		cout<<endl;
	}
	cout<<endl;
	*/

	cout<<endl<<"------------------------------------------------------------------------"<<endl;
	

	//decoding
	IplImage *desImg = cvCreateImage(cvGetSize(srcImg), IPL_DEPTH_8U, 0);
	start_t = clock();
	qsc_decoding(desImg, lineartree, transmission);
	end_t = clock();
	cout<<"decoding time: "<<end_t - start_t<<" ms"<<endl;
	cvNamedWindow("destination image", CV_WINDOW_AUTOSIZE);
	cvShowImage("destination image", desImg);
	cvSaveImage(save_desimg, desImg);

	cout<<endl<<"========================================================================"<<endl;
	cout<<"epsilon: "<<epsilon<<endl;
	cout<<"lineartable size: "<<lineartree.size()<<endl;
	cout<<"transimission size: "<<transmission.size()<<endl;
	cout<<endl;
	
	cout<<"块数："<<psV.size()<<endl;
	cout<<"PSNR: "<<PSNR(srcImg, desImg)<<endl;
	cout<<"BPP: "<<BPP(transmission, srcImg->height, srcImg->width, lineartree)<<endl;
	cout << "CR:" << 8.0/BPP(transmission, srcImg->height, srcImg->width, lineartree)<<endl;


	cvWaitKey(0);
	cvReleaseImage(&srcImg);
	cvReleaseImage(&desImg);
	cvDestroyAllWindows();


	return 0;
}