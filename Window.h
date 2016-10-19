#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <map>
#include <list>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <cstdlib>

using namespace std;

#include <QtGui/QMainWindow>
#include <QtGui/QApplication>
#include <QtGui/QFileDialog>
#include <QtGui/QKeyEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QAction>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>

#include "Widget.h"

class Window : public QMainWindow
{
    Q_OBJECT

private slots:

    // file
    void loadData( void );
    void saveData( void );
    void capture( void );

    // optimize
    void oprStart( void );
    void oprStop( void );
    void oprRandom( void );

    // flag
    void switchFlag( bool & flag );
    void switchLabel( void )		{ switchFlag( _widget->labelFlag() ); }

private:

    // static QGLContext * openglContext( void );
    Widget *_widget;


protected slots:

    void keyPressEvent		( QKeyEvent *event );
    void mouseMoveEvent		( QMouseEvent *event );
    void mousePressEvent	( QMouseEvent *event );
    void mouseReleaseEvent	( QMouseEvent *event );
    void wheelEvent		( QWheelEvent *event );
    // void contentMenuEvent	( QContentMenuEvent *event );

protected:

    // mouse button statuses
    bool	_left_button, _middle_button, _right_button;

    // file
    QMenu	*fileMenu;
    QAction	*selTestAct;
    QAction	*actLoad;
    QAction	*actSave;
    QAction	*actCapture;

    // optimize
    QMenu	*optimizeMenu;
    QAction	*actStart;
    QAction	*actStop;
    QAction	*actRandom;

    // flag
    QMenu	*flagMenu;
    QAction	*actLabel;
    
    void initParams( void );

    void createActions( void );
    void createMenus( void );

public:
    explicit Window( QGLWidget *parent = 0 );
    ~Window();

    void setNet( Net * __net ) {
	_widget->setNet( __net );
    }

    // void initializeGL( void );
    // void paintGL( void );
};


#endif // WINDOW_H
