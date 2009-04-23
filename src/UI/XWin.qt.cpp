#include "XWin.h"

XWin::XWin(
	int		default_dnd,
	const char*	inTitle,
	int		inAttributes,
	int		inX,
	int		inY,
	int		inWidth,
	int		inHeight,
	QWidget *parent) : QMainWindow(parent)
{
	memset(mDragging,0,sizeof(int)*BUTTON_DIM);
	mMouse.x = 0;
	mMouse.y = 0;
	SetTitle(inTitle);

	int x = (inAttributes & xwin_style_fullscreen) ? 0 : inX;
	int y = (inAttributes & xwin_style_fullscreen) ? 0 : inY;
	int w = (inAttributes & xwin_style_fullscreen) ? 1024 : inWidth;
	int h = (inAttributes & xwin_style_fullscreen) ? 768 : inHeight;
	MoveTo(x, y);
	Resize(w, h);
	setMouseTracking(true);
	if (default_dnd)
		setAcceptDrops(true);
}

XWin::XWin(int default_dnd, QWidget *parent) : QMainWindow(parent)
{
	memset(mDragging,0,sizeof(int)*BUTTON_DIM);
	mMouse.x = 0;
	mMouse.y = 0;
	setMouseTracking(true);
	if (default_dnd)
		setAcceptDrops(true);
}

XWin::~XWin()
{
	close();
}

void XWin::resizeEvent(QResizeEvent* e)
{
	Resized(e->size().width(), e->size().height());
}

void XWin::mousePressEvent(QMouseEvent* e)
{
	int btn = 0;
	mMouse.x = e->globalX();
	mMouse.y = e->globalY();
        if (e->button() == Qt::LeftButton)
		btn = 0;
        if (e->button() == Qt::MidButton)
		btn = 2;
        if (e->button() == Qt::RightButton)
		btn = 1;
	mDragging[btn]=1;
	ClickDown(mMouse.x, mMouse.y, btn);
}

void XWin::mouseReleaseEvent(QMouseEvent* e)
{
        int btn = 0;
	mMouse.x = e->globalX();
	mMouse.y = e->globalY();
        if (e->button() == Qt::LeftButton)
		btn = 0;
        if (e->button() == Qt::MidButton)
		btn = 2;
        if (e->button() == Qt::RightButton)
		btn = 1;
	mDragging[btn]=0;
	ClickUp(mMouse.x, mMouse.y, btn);
}

void XWin::mouseMoveEvent(QMouseEvent* e)
{
	mMouse.x = e->globalX();
	mMouse.y = e->globalY();
	int bc=0;
	for(int b=0;b<BUTTON_DIM;++b) {
		if(mDragging[b]) {
			++bc;
			ClickDrag(mMouse.x, mMouse.y, b);
		}
	}
	if(bc==0)
		ClickMove(mMouse.x, mMouse.y);
}

void XWin::wheelEvent(QWheelEvent* e)
{
	mMouse.x = e->globalX();
	mMouse.y = e->globalY();
	MouseWheel(mMouse.x, mMouse.y, (e->delta() < 0) ? -1 : 1, 0);
}

void XWin::keyPressEvent(QKeyEvent* e)
{
	if (e->text().size() != 0)
	{
		unsigned int utf32char = e->text().toUcs4().at(0);
		KeyPressed(utf32char, 0, 0, 0);
	}
}

void XWin::keyReleaseEvent(QKeyEvent* e)
{}

void XWin::dragEnterEvent(QDragEnterEvent* e)
{
	if (e->mimeData()->hasFormat("text/uri-list"))
		e->acceptProposedAction();
}

void XWin::dragLeaveEvent(QDragLeaveEvent* e)
{}

void XWin::dragMoveEvent(QDragMoveEvent* e)
{}

void XWin::dropEvent(QDropEvent* e)
{
	vector<string> inFiles;
	QList<QUrl> urls = e->mimeData()->urls();
	for (int i = 0; i < urls.size(); ++i) {
		if (urls.at(i).scheme() == "file")
			inFiles.push_back(urls.at(i).toLocalFile().toStdString());
	}
	ReceiveFilesFromDrag(inFiles);
}

/* prevent pure virtual function calls. ben, we need to restructure this,
** it hinders us using deep inheritance schemes, typically needed by Qt
*/

void XWin::Resized(int inWidth, int inHeight)
{}

void XWin::Update(XContext ctx)
{}

void XWin::SetTitle(const char * inTitle)
{
	setWindowTitle(inTitle);
}

void XWin::MoveTo(int inX, int inY)
{
	move(inX, inY);
}

void XWin::Resize(int inWidth, int inHeight)
{
	resize(inWidth, inHeight);
}

void XWin::ForceRefresh(void)
{
	Update(0);
}

void XWin::UpdateNow(void)
{
	ForceRefresh();
}

void XWin::SetVisible(bool visible)
{
	setVisible(visible);
}

bool XWin::GetVisible(void) const
{
	return isVisible();
}

bool XWin::GetActive(void) const
{
	return isActiveWindow();
}

void XWin::SetTimerInterval(double seconds)
{
    return;
}

void XWin::GetBounds(int * outX, int * outY)
{
	if (outX) *outX = size().width();
	if (outY) *outY = size().height();
}

void XWin::GetMouseLoc(int * outX, int * outY)
{
	if (outX) *outX = mMouse.x;
	if (outY) *outY = mMouse.y;
}

void XWin::ReceiveFilesFromDrag(const vector<string>& inFiles)
{
	ReceiveFiles(inFiles, 0, 0);
}

xmenu XWin::CreateMenu(xmenu parent, int item, const char * inTitle)
{
    return 0;
}

int XWin::AppendMenuItem(xmenu menu, const char * inTitle)
{}

int XWin::AppendSeparator(xmenu menu)
{}

void XWin::CheckMenuItem(xmenu menu, int item, bool inCheck)
{}
void XWin::EnableMenuItem(xmenu menu, int item, bool inEnable)
{}

void XWin::DrawMenuBar(void)
{}

int XWin::TrackPopupCommands(xmenu in_menu, int mouse_x, int mouse_y, int current)
{
    return -1;
}
