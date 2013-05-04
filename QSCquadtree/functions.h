

#include "datastruct.h"




//encoding.cpp
void qsc_encoding(IplImage *srcimg, vector<char> &lineartree, vector<grayLevels> &transmission, vector<couplePoints> &psV, double epsilon);

//decoding.cpp
void qsc_decoding(IplImage *desimg, vector<char> &lineartree, vector<grayLevels> &transmission);


//util.cpp
void display(Node *root);
double PSNR ( IplImage *&img1 , IplImage *&imggest );
double BPP ( vector<grayLevels> &P ,int M , int N , vector<char> &Q );
void showSegmentation(IplImage *img1, vector<couplePoints> &C);