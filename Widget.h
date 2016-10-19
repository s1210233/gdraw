#ifndef WIDGET_H
#define WIDGET_H

#define WAITING_MODE		( 0)
#define SELECTING_MODE		( 1)
#define DRAGGING_MODE		( 2)

#include <QtOpenGL/QGLWidget>
#include <QtCore/QBasicTimer>

#include "Net.h"
#include "Timer.h"

extern int timecount;    //add 20151231
extern int fixratio;     //add 20160108

class Widget : public QGLWidget
{
    Q_OBJECT
    
private:
    QBasicTimer	*	_timer;

protected:
    // mouse pointer coordinates
    int			_last_pointer_x, _last_pointer_y;

    // corner coordinates
    double		_corner_x, _corner_y;
    
    // mouse mode
    int			_mouse_mode = 0;
			// WAITING_MODE -> SELECTING_MODE -> DRAGGING_MODE -> SELECTING_MODE
			//				  -> CONFIRMING_MODE -> WAITING_MODE
			// WAITING_MODE -> ROTATING_MODE -> WAITING_MODE
			// 
			// WAITING_MODE -> SWITCHING_MODE -> WAITING_MODE

    QFont		_font;

    Net			* _net;

    void		_cone		( Coord2 & coord, double weight, double aspect, double up );
    void		_pyramid	( Coord2 & coord, double weight, double aspect, double up );
    void		_prism		( Coord2 & coord, double weight, double aspect, double up );
    void		_draw		( GLenum mode );
    void		_cell		( GLenum mode );
    void		_annotate	( void );
    void		_rectangle	( void );

    void		initializeGL	( void );
    void		paintGL		( void );
    
    void		timerEvent	( QTimerEvent * e );

    bool		_handleVertex	( int nHits, unsigned int * buf, VertexDescriptor & vd );
    void		_retrieveCluster( int nHits, unsigned int * buf, vector< unsigned int > & ids );

    bool		_pickVertex	( int x, int y, VertexDescriptor & curVD );
    void		_selectVertices	( int x, int y, bool sw );



public:
    explicit Widget( QWidget *parent = 0 );
    ~Widget();
    
    const int & pointer_x( void ) const		{ return _last_pointer_x; }
    int & pointer_x( void )			{ return _last_pointer_x; }

    const int & pointer_y( void ) const		{ return _last_pointer_y; }
    int & pointer_y( void )			{ return _last_pointer_y; }

    const double & center_x( void ) const	{ return _net->center_x(); }
    double & center_x( void )			{ return _net->center_x(); }

    const double & center_y( void ) const	{ return _net->center_y(); }
    double & center_y( void )			{ return _net->center_y(); }

    const double & window_side( void ) const	{ return _net->window_side(); }
    double & window_side( void )		{ return _net->window_side(); }

    const int & mouse_mode( void ) const	{ return _mouse_mode; }
    int & mouse_mode( void )			{ return _mouse_mode; }

    const double & corner_x( void ) const	{ return _corner_x; }
    double & corner_x( void )			{ return _corner_x; }
    const double & corner_y( void ) const	{ return _corner_y; }
    double & corner_y( void )			{ return _corner_y; }

    void setNet( Net * __net ) {
	_net = __net;
	_annotate();
    }

    VertexDescriptor & curVD( void )		{ return _net->curVD(); }
    void setVDCoord( int x, int y )		{ _net->setVDCoord( x, y ); }

    const bool & labelFlag( void )	const	{ return _net->labelFlag(); }
    bool & labelFlag( void )			{ return _net->labelFlag(); }

    void annotate( void )			{ _annotate(); }

    bool pickVertex	( int x, int y, VertexDescriptor & vd ) {
	return _pickVertex( x, y, vd );
    }
    void selectVertices	( int x, int y, bool sw ) {
	_selectVertices( x, y, sw );
    }

    void start( void )				{
	checkInETime();
	cerr << "*********** Starting Execution Time = " << checkOutETime() << endl;
	checkInCPUTime();
	cerr << "*********** Starting CPU Time = " << checkOutCPUTime() << endl;
	_timer->start( 30, this );
    }
    void stop( void )				{
	_timer->stop();
    }

    Net *		net( void )		{ return _net; }

    void		flushGL( void )		{ paintGL(); }

signals:

public slots:

};

#endif // WIDGET_H
