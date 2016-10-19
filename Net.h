//==============================================================================
// Net.h
//	: program file for networks
//
//------------------------------------------------------------------------------
//
//				Date: Sat Sep 10 23:10:30 2016
//
//==============================================================================

#ifndef _Net_H		// begining of header file
#define _Net_H		// notifying that this file is included

//------------------------------------------------------------------------------
//	Including Header Files
//------------------------------------------------------------------------------

#include "Graph.h"

//------------------------------------------------------------------------------
//	Defining Macros
//------------------------------------------------------------------------------
// for b143 network
// #define B143_NETWORK
// for b106 network
// #define B106_NETWORK

#ifndef	DIVISOR
#define DIVISOR		(29)
#endif	// DIVISOR

#ifdef B143_NETWORK
#define	MIN_RATIO	(0.01)	// b143
#else	// B143_NETWORK
#define	MIN_RATIO	(0.0)
#endif	// B143_NETWORK
#define MAX_RATIO	(1.0)
#define STEP_RATIO	(0.1)

//------------------------------------------------------------------------------
//	Defining Classes
//------------------------------------------------------------------------------

class Net : public Graph {

  private:

  protected:

    int			_width;
    int			_height;

    // center coordinates
    double		_center_x, _center_y;
    
    // scale of the window (length of the side)
    double		_window_side;

    VertexDescriptor	_curVD;

    unsigned int	_prevID;
    unsigned int	_nextID;

    bool		_labelFlag;	

    bool		_clutterBit;
    double		_conflictArea;

    bool		_spaceBit;

    void		_init	( void );
    void		_random	( void );

    void		_load	( const char * filename );
    void		_save	( const char * filename );

    void		_force		( void );
    double		_gap		( void );

  public:

//------------------------------------------------------------------------------
//	コンストラクタ
//	Constructors
//------------------------------------------------------------------------------
    Net();			// default constructor
				// コンストラクタ(デフォルト)	
    Net( const Net & obj );
				// copy constructor
				// コピー・コンストラクタ

//------------------------------------------------------------------------------
//	デストラクタ
//	Destructor
//------------------------------------------------------------------------------
    ~Net();			// destructor
				// デストラクタ

//------------------------------------------------------------------------------
//	メンバ参照
//	Reffwering to members
//------------------------------------------------------------------------------
    const int &			width ( void )	const	{ return _width; }
    int &			width ( void )		{ return _width; }
    const int &			height( void )	const	{ return _height; }
    int &			height( void )		{ return _height; }

    const double &		center_x( void ) const	{ return _center_x; }
    double &			center_x( void ) 	{ return _center_x; }
    const double &		center_y( void ) const	{ return _center_y; }
    double &			center_y( void ) 	{ return _center_y; }

    const double &		window_side( void ) const
							{ return _window_side; }
    double &			window_side( void )	{ return _window_side; }

    VertexDescriptor &		curVD( void )		{ return _curVD; }
    void setVDCoord( int x, int y );

    const unsigned int &	prevID( void )	const	{ return _prevID; }
    unsigned int &		prevID( void )		{ return _prevID; }
    const unsigned int &	nextID( void )	const	{ return _nextID; }
    unsigned int &		nextID( void )		{ return _nextID; }

    const bool & labelFlag( void ) const	{ return _labelFlag; }
    bool & labelFlag( void )			{ return _labelFlag; }

    void setClutterBit( void )			{ _clutterBit = true; }
    void clearClutterBit( void )		{ _clutterBit = false; }
    const bool & clutterBit( void ) const	{ return _clutterBit; }

    double & conflictArea( void )		{ return _conflictArea; }
    const double & conflictArea( void ) const	{ return _conflictArea; }
    
    void setSpaceBit( void )			{ _spaceBit = true; }
    void clearSpaceBit( void )			{ _spaceBit = false; }
    const bool & spaceBit( void )	const	{ return _spaceBit; }


//------------------------------------------------------------------------------
//	Fundamental functions
//------------------------------------------------------------------------------
    void	random( void )			{ _random(); }

//------------------------------------------------------------------------------
//	Misc. functions
//------------------------------------------------------------------------------
    void	force	( void )		{ _force(); }
    double	gap	( void )		{ return _gap(); }

//------------------------------------------------------------------------------
//	file I/O
//------------------------------------------------------------------------------
    void load	( const char * filename )	{ _load( filename ); }
    void save	( const char * filename )	{ _save( filename ); }


//------------------------------------------------------------------------------
//	ソートのための関数
//	Functions for sorting
//------------------------------------------------------------------------------
#ifdef SKIP
    friend int compare ( const Net * a, const Net * b );
    friend int compare ( const Net ** a, const Net ** b );
				// comparison
				//  比較関数
#endif	// SKIP

//------------------------------------------------------------------------------
//	等号
//	equality
//------------------------------------------------------------------------------
#ifdef SKIP
    friend int operator == ( const Net & a, const Net & b );
    friend int operator != ( const Net & a, const Net & b ) {
	return ( ! ( a == b ) );
    }				// equality
				// 等号
#endif	// SKIP

//------------------------------------------------------------------------------
//	代入演算子
//	Assignment opereators
//------------------------------------------------------------------------------
    Net & operator = ( const Net & obj );
				// assignment
				// 代入

//------------------------------------------------------------------------------
//	入出力
//	I/O functions
//------------------------------------------------------------------------------
    friend ostream & operator << ( ostream & stream, const Net & obj );
				// output
				// 出力
    friend istream & operator >> ( istream & stream, Net & obj );
				// input
				// 入力

//------------------------------------------------------------------------------
//	クラス名
//	Class name
//------------------------------------------------------------------------------
    virtual const char * className( void ) const { return "Net"; }
				// class name
				// クラス名
};

#ifdef SKIP
inline int arrange( const Net * a, const Net * b ) {
    return compare( a, b );
}				// ソートのためのふたつのベクトルの比較
#endif	// SKIP



#endif // _Net_H

// end of header file
// Do not add any stuff under this line.
