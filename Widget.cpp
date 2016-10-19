//==============================================================================
// Widget.cc
//	: program file for the class WIDGET
//
//------------------------------------------------------------------------------
//
//				Date: Sat Sep 10 06:11:52 2016
//
//==============================================================================

//------------------------------------------------------------------------------
//	Including Header Files
//------------------------------------------------------------------------------

#include <vector>
#include <algorithm>
#include <strstream>
#include <GLUT/glut.h>
#include "Common.h"
#include "Widget.h"
#include "Window.h"
#include "Net.h"

//------------------------------------------------------------------------------
//	Defining Macros
//------------------------------------------------------------------------------

//#define COMMON_RADIUS	(2.4)
//#define COMMON_RADIUS	(3.6)
#define COMMON_RADIUS	(10.0)
//#define COMMON_RADIUS	(100.0)
#define COMMON_HEIGHT 	(-1.0)

int timecount = 0;       //add 20151230
int fixratio  = 0;       //add 20160108


//------------------------------------------------------------------------------
//	Private Functions
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//	Protected Functions
//------------------------------------------------------------------------------
//
//  Widget::_cone --	draw a cone
//
//  Inputs
//	coord:		coordinates of the center
//	weight:		assigned weight
//	aspect :	aspect ratio of the distance metric
//	up:		vertical translation
//
//  Outputs
//	none
//
void Widget::_cone( Coord2 & coord, double weight, double aspect, double up )
{
    glPushMatrix();
    glTranslated( coord.x(), coord.y(), COMMON_HEIGHT+up );
    glScaled( aspect, 1.0, 1.0 );
    glutSolidCone( COMMON_RADIUS*weight, 1.0, 64, 16 );		// radius, height,
								// number of circular ars,
								// number of vertical segments
    // glutSolidCone( COMMON_RADIUS, 1.0, 64, 16 );
    glPopMatrix();
}


//
//  Widget::_pyramid --	draw a pyramid
//
//  Inputs
//	coord :		coordinates of the center
//	weight :	assigned weight
//	aspect :	aspect ratio of the distance metric
//	up :		vertical translation
//
//  Outputs
//	none
//
void Widget::_pyramid( Coord2 & coord, double weight, double aspect, double up )
{
    glPushMatrix();
    glTranslated( coord.x(), coord.y(), COMMON_HEIGHT+up );
    glScaled( aspect, 1.0, 1.0 );
    glRotated( 45.0, 0.0, 0.0, 1.0 );
    glutSolidCone( COMMON_RADIUS*weight, 1.0, 4, 16 );
    //glutSolidCone( COMMON_RADIUS, 1.0, 4, 16 );
    glPopMatrix();
}


//
//  Widget::_prism --	draw a prism
//
//  Inputs
//	coord :		coordinates of the center
//	weight :	assigned weight
//	aspect :	aspect ratio of the distance metric
//	up :		vertical translation
//
//  Outputs
//	none
//
void Widget::_prism( Coord2 & coord, double weight, double aspect, double up )
{
    glPushMatrix();
    glTranslated( coord.x(), coord.y(), COMMON_HEIGHT+up );
    glScaled( aspect, 1.0, 1.0 );
    // No rotation here!!
    // glRotated( 45.0, 0.0, 0.0, 1.0 );
    glutSolidCone( COMMON_RADIUS*weight, 1.0, 4, 16 );    //半径，高さ，円の分割数(4->角錐)，高さの分割数
    //glutSolidCone( COMMON_RADIUS, 1.0, 4, 16 );
    glPopMatrix();
}


//
//  Widget::_draw --	draw a graph
//
//  Inputs
//	mode :	rendering mode
//
//  Outputs
//	none
//
void Widget::_draw( GLenum mode )           /*********************************************
                                                  memo   glColor3f RGB -> (0~255)/255
                                             *********************************************/
{
    ostringstream ostr;
    Coord2 curcoord;
    Graph &		g		= ( Graph & )(*_net);
    VertexIDMap		vertexID	= get( vertex_myid, g );
    VertexCoordMap	vertexCoord	= get( vertex_mycoord, g );
    VertexNameMap	vertexName	= get( vertex_myname, g );
    VertexSwitchMap	vertexSwitch	= get( vertex_myswitch, g );
    VertexWidthMap	vertexWidth	= get( vertex_mywidth, g );
    VertexHeightMap	vertexHeight	= get( vertex_myheight, g );
    
#ifdef DEBUG
    if ( _net->labelFlag() ) cerr << "==> flag label ON" << endl;
    else cerr << "==> flag label OFF" << endl;
#endif	// DEBUG

    // for Picking
    glInitNames();
    glPushName( NO_INDEX );

    glShadeModel( GL_SMOOTH );
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    
    // for enabling antialiasing
    glEnable( GL_LINE_SMOOTH );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    glDisable( GL_LIGHTING );
    
    glLineWidth( 1.0 );
    //glColor3f( 0.0, 0.0, 0.0 );
    glColor3f( 0.5, 0.5, 0.5 );        //---------------add20151007
 
    //draw graph edges
    glBegin( GL_LINES );
    BGL_FORALL_EDGES( ed, g, Graph ) {
        VertexDescriptor vdS = source( ed, g );
        VertexDescriptor vdT = target( ed, g );
        //cout << "vertexCoord[ vdS ]=";_Print( vertexCoord[ vdS ]);
        //cout << "vertexCoord[ vdT ]=";_Print( vertexCoord[ vdT ]);
        glVertex2dv( vertexCoord[ vdS ].element() );
        glVertex2dv( vertexCoord[ vdT ].element() );
    }
    glEnd();
    
    // for disabling antialiasing
    glDisable( GL_LINE_SMOOTH );
    glDisable( GL_BLEND );
    
    // draw graph nodes
    double radius = 9.0;
    glEnable( GL_POINT_SMOOTH );
    BGL_FORALL_VERTICES( vd, g, Graph ) {
        // cerr << HERE << "vertexCoord[" << vd << "]=" << vertexCoord[ vd ];
        glLoadName( vertexID[ vd ] );
        curcoord = vertexCoord[ vd ];
	//cout << "curcoord=" << curcoord << endl;
	
	glPointSize( radius );

	if ( vertexSwitch[ vd ] ) glColor3d( 1.0, 0.0, 0.0 );
	else glColor3d( 0.2, 0.2, 0.2 );
	
        glBegin( GL_POINTS );
        glVertex2dv( curcoord.element() );  //node position
        glEnd();

	glLoadName( NO_INDEX );
        
        if ( _net->labelFlag() ) {             //***************draw Graph label
	    // cerr << " Width = " << vertexWidth[ vd ] << " Height = " << vertexHeight[ vd ] << endl;
            string & name = vertexName[ vd ];
            const double unitL = LABEL_UNIT_RATIO * window_side();
            unsigned int len = name.length();
            double cx = curcoord[ 0 ];
            double cy = curcoord[ 1 ];
            double lw = unitL * vertexWidth[ vd ];
            double lh = unitL * vertexHeight[ vd ];

            glColor3f( 1.0, 1.0, 1.0 );
            glBegin( GL_QUADS );    //square entire color -> white
            glVertex2d( curcoord[0]-lw, curcoord[1]-lh );
            glVertex2d( curcoord[0]+lw, curcoord[1]-lh );
            glVertex2d( curcoord[0]+lw, curcoord[1]+lh );
            glVertex2d( curcoord[0]-lw, curcoord[1]+lh );
            glEnd();
	    
	    glColor3f( 0.0, 0.0, 0.0 );
                    
            glBegin( GL_LINE_LOOP );    //square edge color -> black or red
            glVertex2d( curcoord[0]-lw, curcoord[1]-lh );
            glVertex2d( curcoord[0]+lw, curcoord[1]-lh );
            glVertex2d( curcoord[0]+lw, curcoord[1]+lh );
            glVertex2d( curcoord[0]-lw, curcoord[1]+lh );
            glEnd();
                    
            // glDisable( GL_DEPTH_TEST );
            renderText( curcoord[ 0 ]-0.75*lw, curcoord[ 1 ]-0.65*lh, 0.0,
                       vertexName[ vd ].c_str(),
                       _font );
	    // glEnable( GL_DEPTH_TEST );
        }
    }
    
    if ( _mouse_mode == SELECTING_MODE ) _rectangle();
}


//
//  Widget::_annotate --	setup aspects
//
//  Inputs
//	none
//
//  Outputs
//	onoe
//
void Widget::_annotate( void )
{
    Graph &		g		= ( Graph & )(*_net);
    VertexNameMap	vertexName	= get( vertex_myname, g );
    VertexWidthMap	vertexWidth	= get( vertex_mywidth, g );
    VertexHeightMap	vertexHeight	= get( vertex_myheight, g );

    QFontMetrics qfm( _font );
    BGL_FORALL_VERTICES( vd, g, Graph ) {
	QString qstr( vertexName[ vd ].c_str() );
	int width  = qfm.width( qstr );
	int height = qfm.height();
	vertexWidth [ vd ]	= width;
	vertexHeight[ vd ]	= height;
    }
}


void Widget::_rectangle( void )
{
    glMatrixMode( GL_PROJECTION );
    glPushMatrix();		// <====
    glLoadIdentity();
    glOrtho( 0, width(), height(), 0, -1.0, 1.0 );
    
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();		// <====
    glLoadIdentity();

    glEnable( GL_LINE_SMOOTH );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );

    glLineWidth( 7.0 );
    // glColor3d( 1.00, 0.36, 0.36 );
    glColor3d( 0.50, 0.50, 0.50 );
    glBegin( GL_LINE_LOOP );
    glVertex2i(	_corner_x,  _corner_y  );
    glVertex2i( _last_pointer_x, _corner_y  );
    glVertex2i( _last_pointer_x, _last_pointer_y );
    glVertex2i( _corner_x,  _last_pointer_y );
    glEnd();
    glLineWidth( 3.0 );
    //glColor3d( 1.00, 0.647, 0.00 );
    glColor3d( 1.00, 1.00, 1.00 );
    glBegin( GL_LINE_LOOP );
    glVertex2i(	_corner_x,  _corner_y  );
    glVertex2i( _last_pointer_x, _corner_y  );
    glVertex2i( _last_pointer_x, _last_pointer_y );
    glVertex2i( _corner_x,  _last_pointer_y );
    glEnd();

    glDisable( GL_LINE_SMOOTH );
    glDisable( GL_BLEND );

    glMatrixMode( GL_PROJECTION );
    glPopMatrix();

    glMatrixMode( GL_MODELVIEW );
    glPopMatrix();
}



void Widget::initializeGL( void )
{
    // glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glClearColor( 1.0, 1.0, 1.0, 1.0 );

    _mouse_mode = WAITING_MODE;
    
#ifdef SKIP
    _timer->start( 30, this );
    // _timer->start( 20, this );
    // _timer->start( 16, this );
#endif	// SKIP
}

void Widget::paintGL()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    //glOrtho( -SCREEN_SIDE, SCREEN_SIDE, -SCREEN_SIDE, SCREEN_SIDE, -2.0, 2.0 ); //common.h->SCREEN_SIDE=2.0
    glOrtho( 
	    center_x() - window_side()*SCREEN_SIDE,
	    center_x() + window_side()*SCREEN_SIDE,
	    center_y() - window_side()*SCREEN_SIDE,
	    center_y() + window_side()*SCREEN_SIDE,
	    -2.0, 2.0 );
    // glOrtho( -SCREEN_SIDE, SCREEN_SIDE, -SCREEN_SIDE, SCREEN_SIDE, -2.0, 2.0 ); //common.h->SCREEN_SIDE=2.0
    
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    
    glEnable( GL_DEPTH_TEST );
     // Depth buffering enabled
    glDepthFunc( GL_LESS );
    glDisable( GL_DEPTH_TEST );
    
    _draw( GL_RENDER );
    
    // Return to normal rendering
    glDisable( GL_LINE_SMOOTH );
    glDisable( GL_BLEND );
    
    // _paintName();

    // cerr << "*" << ends;
    glFlush();
    // glutSwapBuffers();

#ifdef ACTIVATE_SNAPS
    QImage screenshot;
    static unsigned int counter = 0;
    screenshot = grabFrameBuffer();
    ostringstream ostr;
    ostr << "snap/snap" << setw( 4 ) << setfill('0') << counter++ << ".png" << ends;
    screenshot.save( ostr.str().c_str() );
    cerr << "Saving an image as : " << ostr.str().c_str() << endl;
#endif	// ACTIVATE_SNAPS
    
}



void Widget::timerEvent( QTimerEvent *e )
{
    Graph & g = ( Graph & )(*_net);
    VertexIDMap         vertexID      = get( vertex_myid, g );
    
    Q_UNUSED( e );

    double err = 0.0;
    
    _net->force();
    err = _net->gap();
    if ( err < FINAL_EPSILON ) {
	_timer->stop();
	cerr << "[Force-Directed] Finished Execution Time = " << checkOutETime() << endl;
	cerr << "[Force-Directed] Finished CPU Time = " << checkOutCPUTime() << endl;
    }
    
    updateGL();
}


//
//  Widget::_handleVertex --	
//
//  Inputs
//	variable : explanation of a variable
//
//  Outputs
//
//
bool Widget::_handleVertex( int nHits, unsigned int * buf, VertexDescriptor & vd )
{
    unsigned int *	ptr		= NULL; //, names;
    float		minDepth	= 1000.0;
    int			hitID		= NO_INDEX;
    Graph &		g		= *_net;
    
    ptr = buf;
    
    for ( int i = 0; i < nHits; ++i ) { // for each bit
	if ( ptr[ 0 ] != 1 ) {
	    cerr << " Number of names for hit = " << ( int )ptr[ 0 ] << endl;
	    assert( ptr[ 0 ] == 1 );
	}
	float curDepth = (float)ptr[ 1 ]/0xffffffff;
	int curID = ( int )ptr[ 3 ];
	//#ifdef DEBUG
	cerr << " i = " << i 
	     << " [0]: " << ptr[ 0 ]
	     << " [1]: " << ptr[ 1 ]
	     << " [2]: " << ptr[ 2 ]
	     << " [3]: " << ptr[ 3 ] << endl;
	// #endif	// DEBUG
	if ( ( curDepth < minDepth ) && ( curID != NO_INDEX ) ) {
	    minDepth = curDepth;
	    hitID = ptr[ 3 ];
	}
	ptr += 4;
    }
    
    cerr << " hitID = " << hitID << " depth = " << minDepth << endl;
    
    if ( hitID != NO_INDEX ) {
	vd = vertex( ( unsigned int )hitID, g );
	// change_flag = true;
	return true;
    }

    return false;
}


//
//  Widget::_retrieveCluster --	
//
//  Inputs
//	variable : explanation of a variable
//	variable : explanation of a variable
//
//  Outputs
//
//
void Widget::_retrieveCluster( int nHits, unsigned int * buffer, vector< unsigned int > & ids )
{
    unsigned int * ptr = NULL; //, names;
    //float minDepth = 1000.0;
    //int hitID = NO_INDEX;

    // cerr << "**** retrieveCluster ****" << endl;
    ids.clear();

    ptr = buffer;

    for ( int i = 0; i < nHits; ++i ) { // for each bit
	if ( ptr[ 0 ] != 1 ) {
	    cerr << " Number of names for hit = " << ( int )ptr[ 0 ] << endl;
	    assert( ptr[ 0 ] == 1 );
	}
	//float curDepth = (float)ptr[ 1 ]/0xffffffff;
	int curID = ( int )ptr[ 3 ];
#ifdef DEBUG
	cerr << " i = " << i 
	     << " [0]: " << ptr[ 0 ]
	     << " [1]: " << ptr[ 1 ]
	     << " [2]: " << ptr[ 2 ]
	     << " [3]: " << ptr[ 3 ] << endl;
#endif	// DEBUG
	if ( curID != NO_INDEX ) {
	    vector< unsigned int >::iterator it = std::find( ids.begin(), ids.end(), curID );
	    if ( it == ids.end() ) ids.push_back( curID );
	}
	ptr += 4;
    }

    for ( unsigned int k = 0; k < ids.size(); ++k ) {
	cerr << " set[ " << setw( 2 ) << k << " ] = " << setw( 3 ) << ids[ k ];
	if ( k % 2 == 1 ) cerr << endl;
    }
    if ( ids.size() % 2 == 1 ) cerr << endl;

    update();
}


//
//  Widget::_pickVertex --	
//
//  Inputs
//	x, y : mouse coordinates
//
//  Outputs
//	none
//
bool Widget::_pickVertex( int x, int y, VertexDescriptor & curVD )
{
    Graph &		g		= *_net;
    VertexIDMap		vertexID	= get( vertex_myid, g );
    unsigned int	selectBuf[ SELECTION_SIZE ];
    int			nHits;
    int			viewport[ 4 ];

    cerr << " pickVertex " << endl;

    glGetIntegerv	( GL_VIEWPORT, viewport );
    
    // Picking begins here
    glSelectBuffer	( SELECTION_SIZE, selectBuf );
    glRenderMode	( GL_SELECT );
    
    glInitNames();
    
    glMatrixMode	( GL_PROJECTION );
    glPushMatrix(); // <====
    glLoadIdentity();
    // create small picking region near cursor location
    //double const size tolerance = 1.0;
    //double const size tolerance = 5.0;
    //const double tolerance = 10.0;
    const double tolerance = 20.0;
#ifdef DEBUG
    cerr << "viewport = " << viewport[0] << "," << viewport[1] << ","
         << viewport[2] << "," << viewport[3] << endl;
    cerr << " x = " << x << " y = " << y << endl;
#endif	// DEBUG
    gluPickMatrix( (double)x, (double)(viewport[3]-y), tolerance, tolerance, viewport );
    
    glOrtho( 
	    center_x() - window_side()*SCREEN_SIDE,
	    center_x() + window_side()*SCREEN_SIDE,
	    center_y() - window_side()*SCREEN_SIDE,
	    center_y() + window_side()*SCREEN_SIDE,
	    -2.0, 2.0 );
    
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();	// <====
    glLoadIdentity();
    
    _draw( GL_SELECT );
    
    glMatrixMode( GL_PROJECTION );
    glPopMatrix();	// <====
    glMatrixMode( GL_MODELVIEW );
    glPopMatrix();	// <====
    
    glFlush();

    nHits = glRenderMode( GL_RENDER );
    VertexDescriptor vd = NULL;

    update();

    if ( _handleVertex( nHits, selectBuf, vd ) ) {
	curVD = vd;
	return true;
    }

    return false;
}


//
//  Widget::_selectVertices --	
//
//  Inputs
//	x, y : screen coordinates
//
//  Outputs
//	none
//
void Widget::_selectVertices( int x, int y, bool sw )
{
    Graph &		g		= *_net;
    VertexIDMap		vertexID	= get( vertex_myid, g );
    unsigned int	selectBuf[ SELECTION_SIZE ];
    int			nHits;
    int			viewport[ 4 ];

    cerr << " selectVertices " << endl;

    double origin_x =	( ( double )_corner_x + x ) / 2.0;
    double origin_y =	( ( double )_corner_y + y ) / 2.0;
    double full_x =	MAX2( 1.0, fabs( _corner_x - x ) );
    double full_y =	MAX2( 1.0, fabs( _corner_y - y ) );

    glGetIntegerv( GL_VIEWPORT, viewport );

    // Selection begins here
    glSelectBuffer( SELECTION_SIZE, selectBuf );
    glRenderMode( GL_SELECT );

    glInitNames();
    
    glMatrixMode( GL_PROJECTION );
    glPushMatrix(); // <====
    glLoadIdentity();

    gluPickMatrix( origin_x, ( double )viewport[ 3 ] - origin_y,
		   full_x, full_y,
		   viewport );

    glOrtho( 
	    center_x() - window_side()*SCREEN_SIDE,
	    center_x() + window_side()*SCREEN_SIDE,
	    center_y() - window_side()*SCREEN_SIDE,
	    center_y() + window_side()*SCREEN_SIDE,
	    -2.0, 2.0 );

    glMatrixMode( GL_MODELVIEW );
    glPushMatrix(); // <====
    glLoadIdentity();

    _draw( GL_SELECT );

    glMatrixMode( GL_PROJECTION );
    glPopMatrix(); // <====
    glMatrixMode( GL_MODELVIEW );
    glPopMatrix(); // <====

    glFlush();

    vector< unsigned int > ids;
    nHits = glRenderMode( GL_RENDER );
    _retrieveCluster( nHits, selectBuf, ids );

    for ( unsigned int k = 0; k < ids.size(); ++k ) {
	VertexDescriptor vd = vertex( ids[ k ], g );
	put( vertex_myswitch, g, vd, sw );
    }

    update();
}


//------------------------------------------------------------------------------
//	Public Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	コンストラクタ
//	Constructors
//------------------------------------------------------------------------------
//
//  Widget::~Widget --	destructor
//			デストラクタ
//
//  Inputs
//	none
//
//  Outputs
//	none
//
Widget::Widget( QWidget *parent )
    : QGLWidget( parent )
{
    _timer = new QBasicTimer();
    // _font = QFont( "Arial", FONT_SIZE, QFont::Bold, false );
    _font = QFont( "Tahoma", FONT_SIZE, QFont::Normal, false );
}




//------------------------------------------------------------------------------
//	デストラクタ
//	Destructor
//------------------------------------------------------------------------------
Widget::~Widget()
{
    //#ifdef ENABLE_TIMER
    delete _timer;
    _timer = NULL;
    //#endif	// ENABLE_TIMER
}


//------------------------------------------------------------------------------
//	メンバ参照
//	Referrring to members
//------------------------------------------------------------------------------




// end of header file
// Do not add any stuff under this line.



