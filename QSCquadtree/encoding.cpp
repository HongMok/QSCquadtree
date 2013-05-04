#include "functions.h"


Node * BuildTree(IplImage *img, couplePoints ps, double epslison, vector<couplePoints> &psV);
void _enCoding(Node *root, vector<char> &lineartree, vector<grayLevels> &transmission);
bool IsHomogeneousBlock(IplImage *img, couplePoints ps, double epslison);
grayLevels FourTuple(IplImage *img, couplePoints ps);
grayLevels FiveTuple(IplImage *img, couplePoints ps);


/************************************************************************/
/* 
	qsc_encoding	--  对图像压缩编码

	srcimg：IplImage *						--  输入原始图像
	lineartree：vector<char> &				--  编码树表格
	transmission：vector<grayLevels> &		--	编码块轮廓点灰度值表格
*/
/************************************************************************/
void qsc_encoding(IplImage *srcimg, vector<char> &lineartree, vector<grayLevels> &transmission, vector<couplePoints> &psV, double epsilon)
{
	couplePoints ps;
	ps.x1 = 0;
	ps.y1 = 0;
	ps.x2 = srcimg->width - 1;
	ps.y2 = srcimg->height - 1;
	grayLevels gl = FourTuple(srcimg, ps);
	transmission.push_back(gl);		//初始化transmission第一个元素


	//vector<couplePoints> psV;

	Node *root;
	time_t start = clock();
	root = BuildTree(srcimg, ps, epsilon, psV);
	cout<<"encoding buildtree: "<<clock() - start<<" ms"<<endl;

	showSegmentation(srcimg, psV);

	start = clock();
	_enCoding(root, lineartree, transmission);
	cout<<"encoding bits: "<<clock() - start<<" ms"<<endl;
}

/************************************************************************/
/* 
	BuildTree	--	对原始图像建树保存
*/
/************************************************************************/
Node * BuildTree(IplImage *img, couplePoints ps, double epslison, vector<couplePoints> &psV)
{
	Node *root;
	root = new Node();

	psV.push_back(ps);

	if(IsHomogeneousBlock(img, ps, epslison) == true)
	{
		root->ne = NULL;
		root->nw = NULL;
		root->se = NULL;
		root->sw = NULL;
		root->visited = false;
		return root;
	}

	couplePoints psNW, psNE, psSW, psSE;
	Node *nNW, *nNE, *nSW, *nSE;

	
	psNW = CouplePointsHelper::getNW(ps);
	psNE = CouplePointsHelper::getNE(ps);
	psSW = CouplePointsHelper::getSW(ps);
	psSE = CouplePointsHelper::getSE(ps);

	nNW = new Node();
	nNE = new Node();
	nSW = new Node();
	nSE = new Node();
	nNW = BuildTree(img, psNW, epslison, psV);
	nNE = BuildTree(img, psNE, epslison, psV);
	nSW = BuildTree(img, psSW, epslison, psV);
	nSE = BuildTree(img, psSE, epslison, psV);

	root->nw = nNW;
	root->ne = nNE;
	root->sw = nSW;
	root->se = nSE;

	root->gl = FiveTuple(img, ps);

	return root;
}

/************************************************************************/
/* 
	IsHomogeneousBlock	--	判断是否为同类块
	epslison:double		--	判断筏值
*/
/************************************************************************/
bool IsHomogeneousBlock(IplImage *img, couplePoints ps, double epslison)
{
	uchar *ptrtemp = (uchar *)img->imageData + img->widthStep * ps.y1;
	uchar g1 = ptrtemp[ps.x1];
	uchar g2 = ptrtemp[ps.x2];
	ptrtemp = (uchar *)img->imageData + img->widthStep * ps.y2;
	uchar g3 = ptrtemp[ps.x1];
	uchar g4 = ptrtemp[ps.x2];

	for(int y = ps.y1; y <= ps.y2; y++)
	{
		uchar *ptr = (uchar *)img->imageData + img->widthStep * y;
		for(int x = ps.x1; x <= ps.x2; x++)
		{
			double g = ptr[x];
			double r1, r2;
			//处理单行单列情况
			if(ps.y1 == ps.y2)
			{
				r1 = 1;
			}
			else
			{
				r1 = (y - ps.y1)/(ps.y2 - ps.y1);
			}
			if(ps.x1 == ps.x2)
			{
				r2 = 1;
			}
			else
			{
				r2 = (x - ps.x1)/(ps.x2 - ps.x1);
			}
			double g5 = g1 + (g2 - g1) * r2;
			double g6 = g3 + (g4 - g3) * r2;
			double gest = g5 + (g6 - g5) * r1;

			if(abs(g - gest) > epslison)
			{
				return false;
			}
		}
	}

	return true;
}

/************************************************************************/
/* 
	_encoding	--	编码，广度遍历
*/
/************************************************************************/
void _enCoding(Node *root, vector<char> &lineartree, vector<grayLevels> &transmission)
{
	//cout<<"encoding..."<<endl;
	//display(root);
	//cout<<endl;
	//return;

	queue<Node *> nodeQ;
	nodeQ.push(root);
	while(!nodeQ.empty())
	{
		Node *nx;
		nx = nodeQ.front();
		nodeQ.pop();
		if(nx->visited == false)
		{
			nx->visited = true;
			//todo
			if(nx->isLeaf())
			{
				lineartree.push_back('1');
			}
			else
			{
				lineartree.push_back('0');
				transmission.push_back(nx->gl);
			}
			if(nx->nw != NULL)
				nodeQ.push(nx->nw);
			if(nx->ne != NULL)
				nodeQ.push(nx->ne);
			if(nx->sw != NULL)
				nodeQ.push(nx->sw);
			if(nx->se != NULL)
				nodeQ.push(nx->se);
		}
	}
}

/************************************************************************/
/* 
	FourTuple	--	返回块的四角灰度值，u = ul, b = ur, l = bl, r = br, c = NULL
*/
/************************************************************************/
grayLevels FourTuple(IplImage *img, couplePoints ps)
{
	grayLevels gl;
	uchar *ptr = (uchar *)img->imageData + ps.y1*img->widthStep;
	gl.u = ptr[ps.x1];
	gl.b = ptr[ps.x2];
	ptr = (uchar *)img->imageData + ps.y2*img->widthStep;
	gl.l = ptr[ps.x1];
	gl.r = ptr[ps.x2];

	gl.c = NULL;
	return gl;
}

/************************************************************************/
/* 
	FiveTuple	--	返回块五角灰度值
*/
/************************************************************************/
grayLevels FiveTuple(IplImage *img, couplePoints ps)
{
	grayLevels gl;
	uchar *ptr = (uchar *)img->imageData + ps.y1*img->widthStep;
	gl.u = ptr[(ps.x1+ps.x2)/2];
	ptr = (uchar *)img->imageData + ((ps.y1+ps.y2)/2) *img->widthStep;
	gl.l = ptr[ps.x1];
	gl.r = ptr[ps.x2];
	gl.c = ptr[(ps.x1+ps.x2)/2];
	ptr = (uchar *)img->imageData + ps.y2 * img->widthStep;
	gl.b = ptr[(ps.x1+ps.x2)/2];

	return gl;
}






