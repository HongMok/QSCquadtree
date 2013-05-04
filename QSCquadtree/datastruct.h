#include<stdlib.h>
#include "highgui.h"
#include "cxcore.h"
#include "vector"
#include <queue>
#include "cv.h"
#include "ctime"
#include "iostream"
#include <math.h>

using namespace cv;
using namespace std;

struct grayLevels 
{
	uchar u;
	uchar b;
	uchar l;
	uchar r;
	uchar c;
};

struct Node 
{
	Node *nw;
	Node *ne;
	Node *sw;
	Node *se;

	bool visited;

	grayLevels gl;

	bool isLeaf()
	{
		if(nw == NULL && ne == NULL && sw == NULL && se == NULL)
			return true;
		else
			return false;
	}

	void setLeaf()
	{
		nw = ne = sw = se = NULL;
	}
};

struct couplePoints
{
	int x1;
	int y1;

	int x2;
	int y2;
};


class CouplePointsHelper
{
public:
	couplePoints static getNW(couplePoints ps)
	{
		couplePoints psNW;
		psNW.x1 = ps.x1;
		psNW.y1 = ps.y1;
		psNW.x2 = (ps.x1 + ps.x2)/2;
		psNW.y2 = (ps.y1 + ps.y2)/2;

		return psNW;
	}
	couplePoints static getNE(couplePoints ps)
	{
		couplePoints psNE;
		psNE.x1 = __min((ps.x1 + ps.x2)/2+1, ps.x2);
		psNE.y1 = ps.y1;
		psNE.x2 = ps.x2;
		psNE.y2 = (ps.y1 + ps.y2)/2;

		return psNE;
	}
	couplePoints static getSW(couplePoints ps)
	{
		couplePoints psSW;
		psSW.x1 = ps.x1;
		psSW.y1 = __min((ps.y1 + ps.y2)/2 + 1, ps.y2);
		psSW.x2 = (ps.x1 + ps.x2)/2;
		psSW.y2 = ps.y2;

		return psSW;
	}
	couplePoints static getSE(couplePoints ps)
	{
		couplePoints psSE;
		psSE.x1 = __min((ps.x1 + ps.x2) / 2 + 1, ps.x2);
		psSE.y1 = __min((ps.y1 + ps.y2) / 2 + 1, ps.y2);
		psSE.x2 = ps.x2;
		psSE.y2 = ps.y2;

		return psSE;
	}


	couplePoints static imgPS(IplImage *img)
	{
		couplePoints ps;
		ps.x1 = 0;
		ps.y1 = 0;
		ps.x2 = img->width - 1;
		ps.y2 = img->height - 1;

		return ps;
	}
protected:
private:
};






