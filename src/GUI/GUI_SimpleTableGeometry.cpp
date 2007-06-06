#include "GUI_SimpleTableGeometry.h"
#include "GUI_Messages.h"
#include <algorithm>

GUI_SimpleTableGeometry::GUI_SimpleTableGeometry(
			int		num_cols,
			int *	default_col_widths,
			int		row_height)
{
	mRowHeight = row_height;
	mCols.resize(num_cols);
	int p = 0;
	for (int n = 0; n < num_cols; ++n)
	{
		p += default_col_widths[n];
		mCols[n] = p;
	}
}
			
GUI_SimpleTableGeometry::~GUI_SimpleTableGeometry()
{
}

//int			GUI_SimpleTableGeometry::GetColCount(void)
//{
//	return mCols.size();
//}

int			GUI_SimpleTableGeometry::GetCellLeft (int n)
{
	if(n > 0) ExtendTo(n-1);
	return (n == 0) ? 0 : mCols[n-1];
}

int			GUI_SimpleTableGeometry::GetCellRight(int n)
{
	ExtendTo(n);
	return mCols[n];
}

int			GUI_SimpleTableGeometry::GetCellWidth(int n)
{
	ExtendTo(n);
	return (n == 0) ? mCols[0] : (mCols[n] - mCols[n-1]);
}

int			GUI_SimpleTableGeometry::GetCellBottom(int n)
{
	return n * mRowHeight;
}

int			GUI_SimpleTableGeometry::GetCellTop	 (int n)
{
	return (n+1)*mRowHeight;
}

int			GUI_SimpleTableGeometry::GetCellHeight(int n)
{
	return mRowHeight;
}
	
int			GUI_SimpleTableGeometry::ColForX(int n)
{
	vector<int>::iterator i = lower_bound(mCols.begin(),mCols.end(),n);
	if (i == mCols.end()) return (int) mCols.size()-1;
	int p = distance(mCols.begin(),i);
	if (n == *i) ++p;
	if (p == mCols.size()) return (int)mCols.size()-1;
	return p;
}

int			GUI_SimpleTableGeometry::RowForY(int n)
{
	return n / mRowHeight;
}

bool		GUI_SimpleTableGeometry::CanSetCellWidth (void) const { return true; }
	
void		GUI_SimpleTableGeometry::SetCellWidth (int n, int w)
{
	if (w < 5) w = 5;
	ExtendTo(n);
	int delta = w - GetCellWidth(n);
	for (int i = n; i < mCols.size(); ++i)	
		mCols[i] += delta;
//	BroadcastMessage(GUI_TABLE_SHAPE_RESIZED, 0);
}

bool		GUI_SimpleTableGeometry::CanSetCellHeight(void) const { return false; }

void		GUI_SimpleTableGeometry::SetCellHeight(int n, int h)
{
}

void		GUI_SimpleTableGeometry::ExtendTo(int n)
{
	if (n < mCols.size()) return;
	int last_width = mCols[mCols.size()-1] - mCols[mCols.size()-2];
	int pos = mCols[mCols.size()-1];
	
	int num_to_add = n + 1 - mCols.size();
	
	while (num_to_add > 0)
	{
		pos += last_width;
		mCols.push_back(pos);
		--num_to_add;
	}	
}
