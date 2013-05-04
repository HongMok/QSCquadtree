#include "functions.h"

Node *rebuildTree(vector<char> &lineartree, vector<grayLevels> &transmission);

void generateSonNode(vector<char> &lineartree, int &li, vector<grayLevels> &transmission, int &ti, queue<Node *> &nodeQ, Node *root, int sonType);

void redraw(IplImage *img, couplePoints ps, Node *root, grayLevels curr4);

void _redraw(IplImage *img, couplePoints ps, grayLevels curr4);

grayLevels calFour(grayLevels curr4, grayLevels curr5, int type);




/************************************************************************/
/* 
	qsc_decoding	--	解码

	lineartree	--	树表
	transmission	--	内部节点边缘灰度值
*/
/************************************************************************/
void qsc_decoding(IplImage *desimg, vector<char> &lineartree, vector<grayLevels> &transmission)
{
	Node *root;

	time_t start = clock();
	root = rebuildTree(lineartree, transmission);
	cout<<"decoding buildtree: "<<clock() - start<<" ms"<<endl;

	//cout<<"decoding..."<<endl;
	//display(root);

	couplePoints ps = CouplePointsHelper::imgPS(desimg);

	start = clock();
	redraw(desimg, ps, root, transmission[0]);
	cout<<"decoding redraw: "<<clock() - start<<" ms"<<endl;
}


/************************************************************************/
/* 
	重建图像表示树
*/
/************************************************************************/
Node *rebuildTree(vector<char> &lineartree, vector<grayLevels> &transmission)
{
	Node *root;
	root = new Node();
	root->gl = transmission[1];

	int li = 1;
	int ti = 2;

	Node *temp;

	queue<Node *> nodeQ;
	nodeQ.push(root);
	while(!nodeQ.empty())
	{
		temp = nodeQ.front();
		nodeQ.pop();

		generateSonNode(lineartree, li, transmission, ti, nodeQ, temp, 1);
		generateSonNode(lineartree, li, transmission, ti, nodeQ, temp, 2);
		generateSonNode(lineartree, li, transmission, ti, nodeQ, temp, 3);
		generateSonNode(lineartree, li, transmission, ti, nodeQ, temp, 4);
	}
	return root;
}


/************************************************************************/
/* 
	生成新的节点
*/
/************************************************************************/
void generateSonNode(vector<char> &lineartree, int &li, vector<grayLevels> &transmission, int &ti, queue<Node *> &nodeQ, Node *root, int sonType)
{
	Node *temp;
	temp = new Node();
	//temp = NodePool::createNode();
	if(lineartree[li++] == '0')
	{
		temp->gl = transmission[ti++];
		nodeQ.push(temp);
	}
	else
	{
		temp->setLeaf();
	}

	switch(sonType)
	{
	case 1:
		root->nw = temp;
		break;
	case 2:
		root->ne = temp;
		break;
	case 3:
		root->sw = temp;
		break;
	case 4:
		root->se = temp;
		break;
	}
}


/************************************************************************/
/* 
	重绘图像
*/
/************************************************************************/
void redraw(IplImage *img, couplePoints ps, Node *root, grayLevels curr4)
{
	if(root->isLeaf())
	{
		_redraw(img, ps, curr4);
		return;
	}

	redraw(img, CouplePointsHelper::getNW(ps), root->nw, calFour(curr4, root->gl, 1));
	redraw(img, CouplePointsHelper::getNE(ps), root->ne, calFour(curr4, root->gl, 2));
	redraw(img, CouplePointsHelper::getSW(ps), root->sw, calFour(curr4, root->gl, 3));
	redraw(img, CouplePointsHelper::getSE(ps), root->se, calFour(curr4, root->gl, 4));
}


void _redraw(IplImage *img, couplePoints ps, grayLevels curr4)
{
	uchar g1 = curr4.u;
	uchar g2 = curr4.b;
	uchar g3 = curr4.l;
	uchar g4 = curr4.r;

	for(int y = ps.y1; y <= ps.y2; y++)
	{
		uchar *ptr = (uchar *)img->imageData + img->widthStep * y;
		for(int x = ps.x1; x <= ps.x2; x++)
		{
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
			uchar g5 = g1 + (g2 - g1) * r2;
			uchar g6 = g3 + (g4 - g3) * r2;
			uchar gest = g5 + (g6 - g5) * r1;

			ptr[x] = gest;
		}
	}
}


/************************************************************************/
/* 
	计算子区域的 四点灰度值
*/
/************************************************************************/
grayLevels calFour(grayLevels curr4, grayLevels curr5, int type)
{
	grayLevels newgl;
	newgl.c = NULL;
	switch(type)
	{
	case 1:
		newgl.u = curr4.u;
		newgl.b = curr5.u;
		newgl.l = curr5.l;
		newgl.r = curr5.c;
		break;
	case 2:
		newgl.u = curr5.u;
		newgl.b = curr4.b;
		newgl.l = curr5.c;
		newgl.r = curr5.r;
		break;
	case 3:
		newgl.u = curr5.l;
		newgl.b = curr5.c;
		newgl.l = curr4.l;
		newgl.r = curr5.b;
		break;
	case 4:
		newgl.u = curr5.c;
		newgl.b = curr5.r;
		newgl.l = curr5.b;
		newgl.r = curr4.r;
		break;
	}

	return newgl;
}






