//==============================================================================
// Window.cc
//	: program file for WINDOW class
//
//------------------------------------------------------------------------------
//
//				Date: Sat Sep 10 06:03:40 2016
//
//==============================================================================

//------------------------------------------------------------------------------
//	Including Header Files
//------------------------------------------------------------------------------

#include "Window.h"


//------------------------------------------------------------------------------
//	Defining Macros
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//	Private Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	File menu
void Window::loadData()
{
    QString fileName = QFileDialog::getOpenFileName( this,
						     tr("Choose a file name"), "./data/",
						     tr("Data files (*.dat)"));
    if ( fileName.isEmpty() )
        return;

    _widget->net()->load( fileName.toStdString().c_str() );
    _widget->annotate();

    cerr << "Load a graph from : " << fileName.toStdString() << endl;
    _widget->repaint();
}


void Window::saveData()
{
    QString fileName = QFileDialog::getSaveFileName( this,
						     tr("Choose a file name"), "./data/",
						     tr("Data files (*.dat)"));
    if ( fileName.isEmpty() )
        return;

    _widget->net()->save( fileName.toStdString().c_str() );

    cerr << "Save a graph to : " << fileName.toStdString() << endl;
    _widget->repaint();
}


void Window::capture()
{
    QString fileName = QFileDialog::getSaveFileName( this,
						     tr("Choose a file name"), ".",
						     tr("Image files (*.png *.jpg)"));
    if ( fileName.isEmpty() )
        return;

    _widget->flushGL();
    _widget->flushGL();
    _widget->flushGL();

    QImage screenshot;

    screenshot = _widget->grabFrameBuffer();
    screenshot.save( fileName );
    cerr << "Saving an image as : " << fileName.toStdString() << endl;
    _widget->repaint();
    
}

//------------------------------------------------------------------------------
//	Optimize menu
void Window::oprStart( void )
{
    _widget->start();
}

void Window::oprStop( void )
{
    _widget->stop();
    _widget->repaint();
}

void Window::oprRandom( void )
{
    _widget->net()->random();
    _widget->repaint();
}


//------------------------------------------------------------------------------
//	Flag menu
void Window::switchFlag( bool & flag )
{
    flag = !flag;
    if ( flag ) cerr << "Flag enabled" << endl;
    else cerr << "Flag disabled" << endl;
    _widget->repaint();
}
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//	Protected Slots
//------------------------------------------------------------------------------

void Window::keyPressEvent( QKeyEvent *e )
{
    cerr << " Modifiers = " << e->modifiers() << endl;
    if ( e->key() == Qt::Key_Alt ) cerr << "Alt key" << endl;

    switch( e->key() ){
    case Qt::Key_Escape:
        close();
        break;
    default:
        QWidget::keyPressEvent( e );
    }
    update();
}


void Window::mouseMoveEvent( QMouseEvent *e )
{
    const double ratioV = 2.0;
    const double ratioS = 4.0;
    const double ratioT = 2.0;

    switch ( e->buttons() ){
    case Qt::LeftButton:
	if ( e->type() == QEvent::MouseMove ) {
	    if ( _widget->mouse_mode() == DRAGGING_MODE ) {
		_widget->setVDCoord( e->x(), e->y() );
	    }
	    _widget->pointer_x() = e->x();
	    _widget->pointer_y() = e->y();
	}
        break;
    case Qt::RightButton:
	if ( e->type() == QEvent::MouseMove ) {
	    // cerr << "right button x = " << e->x() << " y = " << e->y() << endl;
	    _widget->pointer_x() = e->x();
	    _widget->pointer_y() = e->y();
	}
        break;
      case Qt::MiddleButton:
	if ( e->type() == QEvent::MouseMove ) {
#ifdef SKIP
	    _widget->maxSide() -= 
		ratioS * ( ( double ) e->y() - _last_pointer_y ) / ( double )( height() );
#endif	// SKIP
	    _widget->center_x() += 
		-ratioT * ( double )( e->x() -  _widget->pointer_x() ) / ( double )width();
	    _widget->center_y() += 
		ratioT * ( double )( e->y() -  _widget->pointer_y() ) / ( double )height();
#ifdef DEBUG
	    cerr << " centerx = " << _widget->center_x()
		 << " centery = " << _widget->center_y() << endl;
#endif	// DEBUG
	    _widget->pointer_x() = e->x();
	    _widget->pointer_y() = e->y();
	}
        break;
      default:
	  QWidget::mouseMoveEvent( e );
    }
    update();
}


void Window::mousePressEvent( QMouseEvent *e )
{
    // cerr << "**** Press" << endl;
#ifdef DEBUG
    cerr << " ebuttons = " << e->buttons() << endl;
    cerr << " left = " << Qt::LeftButton << endl;
    cerr << " right = " << Qt::RightButton << endl;
#endif	// DEBUG

    if ( e->buttons() & Qt::LeftButton ) {
	// cerr << "**** left Press" << endl;
	if ( e->type() == QEvent::MouseButtonPress ) {
	    _left_button = true;
	    if ( _widget->pickVertex( e->x(), e->y(), _widget->curVD() ) )
		_widget->mouse_mode() = DRAGGING_MODE;
	    else
		_widget->mouse_mode() = WAITING_MODE;
	    _widget->pointer_x() = e->x();
	    _widget->pointer_y() = e->y();
	}
    }
    else if ( e->buttons() & Qt::RightButton ) {
	// cerr << "**** right Press: type = " << e->type() << endl;
	if ( e->type() == QEvent::MouseButtonPress ) {
	    cerr << "Right mouse button pressed" << endl;
	    _right_button = true;
	    _widget->mouse_mode() = SELECTING_MODE;
	    _widget->corner_x() = e->x();
	    _widget->corner_y() = e->y();
	    _widget->pointer_x() = e->x();
	    _widget->pointer_y() = e->y();
	}
    }
    else if ( e->buttons() & Qt::MiddleButton ) {
	// cerr << "**** middle Press" << endl;
	if ( e->type() == QEvent::MouseButtonPress ) {
	    _widget->pointer_x() = e->x();
	    _widget->pointer_y() = e->y();
	}
    }
    else {
	  QWidget::mousePressEvent( e );
    }

    update();
}


void Window::mouseReleaseEvent( QMouseEvent *e )
{
    const int limit = 4;
    // cerr << "**** Release" << endl;

#ifdef DEBUG
    cerr << " ebuttons = " << e->buttons() << endl;
    cerr << " left = " << Qt::LeftButton << endl;
    cerr << " right = " << Qt::RightButton << endl;
#endif	// DEBUG

    if ( e->type() == QEvent::MouseButtonRelease ) {
	if ( _left_button ) {
	    // cerr << "**** left Release" << endl;
	    _left_button = false;
	    // _last_pointer_x = e->x();
	    // _last_pointer_y = e->y();
	}
	else if ( _right_button ) {
	    _widget->selectVertices( e->x(), e->y(), _widget->net()->labelFlag() );
	    _widget->mouse_mode() = WAITING_MODE;
	    _right_button = false;
	}
	else if ( _middle_button ) {
	    // _last_pointer_x = e->x();
	    // _last_pointer_y = e->y();
	    _middle_button = false;
	}
    }
    else {
	QWidget::mouseReleaseEvent( e );
    }

    _widget->repaint();
    update();
}


void Window::wheelEvent( QWheelEvent *e )
{
#ifdef DEBUG
    cerr << " delta = " << e->delta() << ends;
#endif	// DEBUG
    // _widget->window_side() += e->delta()/12000.0;
    _widget->window_side() += e->delta()/1200.0;
    _widget->window_side() = max( 0.1, min( 10.0, _widget->window_side() ) );
    cerr << " side = " << _widget->window_side() << endl;
    _widget->repaint();
    update();
}


//------------------------------------------------------------------------------
//	Protected Functions
//------------------------------------------------------------------------------
void Window::initParams( void )
{
    _left_button	= false;
    _middle_button	= false;
    _right_button	= false;
}

void Window::createActions( void )
{
    // File:	load
    actLoad		= new QAction( tr("L&oad"), this );
    actLoad->setShortcut( tr("Ctrl+L") );
    actLoad->setStatusTip( tr("Load a graph file") );
    connect( actLoad, SIGNAL( triggered() ), this, SLOT( loadData() ) );
    // File:	save
    actSave		= new QAction( tr("S&ave"), this );
    actSave->setShortcut( tr("Ctrl+S") );
    actSave->setStatusTip( tr("Save a graph file") );
    connect( actSave, SIGNAL( triggered() ), this, SLOT( saveData() ) );
    // File:	capture
    actCapture		= new QAction( tr("Cap&ture"), this );
    actCapture->setShortcut( tr("Ctrl+P") );
    actCapture->setStatusTip( tr("Capture the main window as an image") );
    connect( actCapture, SIGNAL( triggered() ), this, SLOT( capture() ) );

    // Optimize:start
    actStart		= new QAction( tr("S&tart"), this );
    actStart->setShortcut( tr("Ctrl+S") );
    actStart->setStatusTip( tr("Start the graph layout") );
    connect( actStart, SIGNAL( triggered() ), this, SLOT( oprStart() ) );
    // Optimize:start
    actStop		= new QAction( tr("St&op"), this );
    actStop->setShortcut( tr("Ctrl+T") );
    actStop->setStatusTip( tr("Stop the graph layout") );
    connect( actStop, SIGNAL( triggered() ), this, SLOT( oprStop() ) );
    // Optimize:reset
    actRandom		= new QAction( tr("R&andom"), this );
    actRandom->setShortcut( tr("Ctrl+D") );
    actRandom->setStatusTip( tr("Randominze the graph layout") );
    connect( actRandom, SIGNAL( triggered() ), this, SLOT( oprRandom() ) );

    // Flag:	Label
    actLabel		= new QAction( tr("La&bel"), this );
    actLabel->setShortcut( tr("Ctrl+A") );
    actLabel->setStatusTip( tr("active/deactivate label cells") );
    connect( actLabel, SIGNAL( triggered() ), this, SLOT( switchLabel() ) );
}

void Window::createMenus( void )
{
    // File
    fileMenu = menuBar()->addMenu( tr("&File") );
    // Separator
    fileMenu->addSeparator();
    fileMenu->addAction( actLoad );
    fileMenu->addAction( actSave );
    fileMenu->addAction( actCapture );
    // Optimize
    optimizeMenu = menuBar()->addMenu( tr("Optim&ize") );
    optimizeMenu->addAction( actStart );
    optimizeMenu->addAction( actStop );
    optimizeMenu->addAction( actRandom );
    // Flag
    flagMenu = menuBar()->addMenu( tr("&Flag") );
    flagMenu->addAction( actLabel );
}

//------------------------------------------------------------------------------
//	Public Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	コンストラクタ
//	Constructors
//------------------------------------------------------------------------------

//
//  Window::Window --	default constructor
//			コンストラクタ(デフォルト)
//
//  Inputs
//	none
//
//  Outputs
//	none
//
Window::Window( QGLWidget *parent )
    : QMainWindow( parent )
{
    _widget = new Widget( this );
    setCentralWidget( _widget );

    initParams();
    createActions();
    createMenus();
}


//
//  Window::Window --	copy constructor
//			コピー・コンストラクタ
//
//  Inputs
//	obj	: object of this class
//
//  Outputs
//	none
//
#ifdef SKIP
Window::Window( const Window & obj )
      : %%%( ( const %%% & ) obj )
{

}
#endif	// SKIP


//------------------------------------------------------------------------------
//	デストラクタ
//	Destructor
//------------------------------------------------------------------------------

//
//  Window::~Window --	destructor
//			デストラクタ
//
//  Inputs
//	none
//
//  Outputs
//	none
//
Window::~Window()
{

}


//------------------------------------------------------------------------------
//	メンバ参照
//	Referrring to members
//------------------------------------------------------------------------------




// end of program file
// Do not add any stuff under this line.



