//==============================================================================
// Net.cc
//	: program file for networks
//
//------------------------------------------------------------------------------
//
//				Date: Sat Sep 10 06:08:25 2016
//
//==============================================================================
//どうやらここらしいぜ
//------------------------------------------------------------------------------
//	Including Header Files
//------------------------------------------------------------------------------

#include <ctime>

using namespace std;

#include <boost/format.hpp>
#include "Common.h"
#include "Net.h"

//------------------------------------------------------------------------------
//	Defining Macros
//------------------------------------------------------------------------------

//#define RESPONSIVE_CONTROL

#define CHEBYSHEV_DISTANCE

//------------------------------------------------------------------------------
//	Private Functions
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
//	Protected Functions
//------------------------------------------------------------------------------

//
//  Net::_init --	initialize the network (called once when necessary)
//
//  Inputs
//	none
//
//  Outputs
//	none
//
void Net::_init( void )
{
    // const char theName[] = "void Net::_init : ";

    // srand48( time( NULL ) );
#if defined( B143_NETWORK )
    srand48( 7 ); // for b143 
#else	// defined( B143_NETWORK )
    srand48( 3 );
#endif	// defined( B143_NETWORK )
    
    _center_x = 0.0;
    _center_y = 0.0;
    _window_side = 1.0;     //initial window coordinates -> (-2,-2),(2,-2),(2,2),(-2,2)
    _labelFlag = false;
    _clutterBit = false;
    _conflictArea = 0.0;
    _spaceBit	= false;
}


//
//  Net::_random --	randomize the node coordinates
//
//  Inputs
//	none
//
//  Outputs
//	none
//
void Net::_random( void )
{
    // const char theName[] = "Net::_random : ";

    VertexCoordMap      vertexCoord     = get( vertex_mycoord, *this );

    BGL_FORALL_VERTICES( vd, *this, Graph ) {
	vertexCoord[ vd ] = Coord2( CONTENT_RATIO*SCREEN_SIDE*(2.0*drand48()-1.0), 
				    CONTENT_RATIO*SCREEN_SIDE*(2.0*drand48()-1.0) );
    }
}


//
//  Net::_load --	load the data from the input file
//
//  Inputs
//	_filename	: file name
//
//  Outputs
//	none
//
void Net::_load( const char * filename )
{
    Graph &		graph = ( Graph & )*this;
    VertexIDMap		vertexID	= get( vertex_myid, graph );
    VertexSwitchMap	vertexSwitch    = get( vertex_myswitch, graph );
    VertexCoordMap      vertexCoord     = get( vertex_mycoord, graph );
    VertexNameMap       vertexName      = get( vertex_myname, graph );
    VertexWeightMap     vertexWeight    = get( vertex_myweight, graph );
    ifstream		ifs( filename );
    char		buf[ BUFFER_SIZE ];

    graph.clear();

    if ( !ifs ) {
    	cerr << "Cannot open the target file : " << filename << endl;
        return;
    }

    //------------------------------------------------------------------------------
    //  Read vertices
    //------------------------------------------------------------------------------
    istringstream istr;

    // number of vertices
    unsigned int numV;
    ifs.getline( buf, sizeof( buf ) );
    istr.clear();
    istr.str( buf );
    istr >> numV;
    cerr << HERE << "Number of vertices = " << numV << endl;

    // information about each vertex
    for ( unsigned int i = 0; i < numV; ++i ) {
	string name;
	double weight;
	double x, y;
	ifs.getline( buf, sizeof( buf ) );
	istr.clear();
	istr.str( buf );
	istr >> name >> weight >> x >> y;
	// cerr << HERE << "Vertex name = " << name.c_str() << " weight = " << weight << endl;
	VertexDescriptor vd = add_vertex( graph );
	vertexID[ vd ] = i;
	for ( unsigned int k = 0; k < name.length(); ++k ) 
	    if ( name[ k ] == '_' ) name[ k ] = ' ';
	vertexName[ vd ] = name;
	vertexSwitch[ vd ] = false;
	vertexWeight[ vd ] = weight;
	double const small = 0.05;
	vertexCoord[ vd ] = Coord2( x+small*(2.0*drand48()-1.0),
				    y+small*(2.0*drand48()-1.0) );
#ifdef SKIP
	vertexCoord[ vd ] = Coord2( CONTENT_RATIO*SCREEN_SIDE*(2.0*drand48()-1.0), 
				    CONTENT_RATIO*SCREEN_SIDE*(2.0*drand48()-1.0) );
#endif	// SKIP
    }

    // number of edges
    unsigned int numE;
    ifs.getline( buf, sizeof( buf ) );
    istr.clear();
    istr.str( buf );
    istr >> numE;
    cerr << HERE << "Number of edges = " << numE << endl;

    // link the pair of endpoints with each edge
    for ( unsigned int i = 0; i < numE; ++i ) {
	unsigned int idO, idD;
	ifs.getline( buf, sizeof( buf ) );
	istr.clear();
	istr.str( buf );
	istr >> idO >> idD;
	// cerr << HERE << "Edge = " << idO << " -- " << idD << endl;
	VertexDescriptor vdO = vertex( idO, graph ); 
	VertexDescriptor vdD = vertex( idD, graph ); 
	bool is;
	EdgeDescriptor ed;
	tie( ed, is ) = add_edge( vdO, vdD, graph );
    }

    ifs.close();

    _random();

    cerr << " No of vertices = " << num_vertices( graph ) << endl;
    cerr << " No of edges = " << num_edges( graph ) << endl;

    return;
}


//
//  Net::_save --	save the data from the input file
//
//  Inputs
//	_filename	: file name
//
//  Outputs
//	none
//
void Net::_save( const char * filename )
{
    Graph &		graph = ( Graph & )*this;
    VertexIDMap		vertexID	= get( vertex_myid, graph );
    VertexSwitchMap	vertexSwitch    = get( vertex_myswitch, graph );
    VertexCoordMap      vertexCoord     = get( vertex_mycoord, graph );
    VertexNameMap       vertexName      = get( vertex_myname, graph );
    VertexWeightMap     vertexWeight    = get( vertex_myweight, graph );
    const double	size = 512;
    ofstream		ofs( filename );

    if ( !ofs ) {
    	cerr << "Cannot open the target file : " << filename << endl;
        return;
    }

    //------------------------------------------------------------------------------
    //  Read vertices
    //------------------------------------------------------------------------------
    // number of vertices
    unsigned int numV = num_vertices( graph );
    ofs << numV << endl;

    // information about each vertex
    BGL_FORALL_VERTICES( vd, graph, Graph ) {
	ofs << vertexName[ vd ].c_str() << "\t";
	// double wx = size * 0.25 * ( vertexCoord[ vd ].x() + 2.0 );
	// double wy = size * 0.25 * ( vertexCoord[ vd ].y() + 2.0 );
	double wx = vertexCoord[ vd ].x() / 1.6;
	double wy = vertexCoord[ vd ].y() / 1.6;
	ofs << boost::format( "%6.3f\t%6.3f\t%6.3f" ) % vertexWeight[ vd ] % wx % wy;
	ofs << endl;
    }

    // number of edges
    unsigned int numE = num_edges( graph );
    
    ofs << numE << endl;

    // link the pair of endpoints with each edge
    BGL_FORALL_EDGES( ed, graph, Graph ) {
        VertexDescriptor vdS = source( ed, graph );
        VertexDescriptor vdT = target( ed, graph );
	unsigned int idS = vertexID[ vdS ];
	unsigned int idT = vertexID[ vdT ];
	ofs << idS << "\t" << idT << endl;
    }

    ofs.close();

    return;
}


//
//  Net::force --	compute the displacements exerted by the force-directed model
//
//  Inputs
//	none
//
//  Outputs
//	none
//
void Net::_force( void )
{
    // const char theName[] = "Net::force : ";
    // const double ka = 0.001;
    Graph & g	= ( Graph & )*this;
#if defined( B143_NETWORK ) || defined( B106_NETWORK )
    const double ka = 0.5;	// b143
    const double kr = 0.005;	// b143
#else	// defined( B143_NETWORK ) || defined( B106_NETWORK )
    const double ka = 1.0;
    const double kr = 0.01;
#endif	// defined( B143_NETWORK ) || defined( B106_NETWORK )
    //const double ka = 5.0;
    //const double kr = 0.05;
    // const double L = 0.4;
    // const double L = 1.0;
    double side = _window_side * SCREEN_SIDE;
    double L = sqrt( SQUARE( side ) / ( double )max( 1.0, ( double )num_vertices( g ) ) );
    
    VertexCoordMap	vertexCoord	= get( vertex_mycoord, g );
    VertexForceMap	vertexForce	= get( vertex_myforce, g );

    BGL_FORALL_VERTICES( vdi, g, Graph ) {
        Coord2 shift( 0.0, 0.0 );
        // put( vertex_myforce, g, vdi, shift );
        vertexForce[ vdi ] = shift;
        BGL_FORALL_VERTICES( vdj, g, Graph ) {
            if ( vdi == vdj ) {
                ; // do nothing
            }
            else {
                Coord2 diff, unit;
                double dist;
                
                diff = vertexCoord[ vdj ] - vertexCoord[ vdi ];
                dist = diff.norm();
                unit = diff.unit();
                
                // pair< EdgeDescriptor, bool > isConnected = edge( vdi, vdj );
                EdgeDescriptor	ed;
                bool		is;
                tie( ed, is ) = edge( vdi, vdj, g );
                if ( is ) {
                    // Drawing force by the spring
                    vertexForce[ vdi ] += ka * ( dist - L ) * unit;
                }
                // Replusive force by Couloum's power
                vertexForce[ vdi ] -= ( kr/SQUARE( dist ) ) * unit;
            }
        }
    }
}


//
//  Net::_gap --	return the convergence error in laying out the graph
//
//  Inputs
//	none
//
//  Outputs
//	error value
//
double Net::_gap( void )
{
    const char theName[] = "Net::_gap : ";
    	
    Graph &		g	= ( Graph & )*this;
    vector< Coord2 >	displace;
    double		err		= 0.0;
    // const double	scale		= TRANSFORMATION_STEP;
    const double	scale		= TRANSFORMATION_STEP*min(1.0,100.0/( double )num_vertices( *this ));
    VertexIDMap		vertexID	= get( vertex_myid, g );
    VertexCoordMap	vertexCoord	= get( vertex_mycoord, g );
    VertexShiftMap	vertexShift	= get( vertex_myshift, g );
    VertexForceMap	vertexForce	= get( vertex_myforce, g );
    VertexSwitchMap	vertexSwitch	= get( vertex_myswitch, g );
    
    //cerr << "Net::_net->gp() = " << gp() << endl;
    //cerr << "Net::_net->clut() = " << clut() << endl;
    
    displace.clear();
    BGL_FORALL_VERTICES( vd, g, Graph ) {
        Coord2 val;
	val = vertexForce[ vd ];
	assert( vertexID[ vd ] == displace.size() );
        displace.push_back( val );
    }

    // Scale the displacement of a node at each scale
    const double limit = DISPLACEMENT_LIMIT;
    BGL_FORALL_VERTICES( vd, g, Graph ) {
	unsigned int idV = vertexID[ vd ];
	vertexShift[ vd ] = scale * displace[ idV ];
	double norm = vertexShift[ vd ].norm();
	if ( norm > limit ) {
	    vertexShift[ vd ] *= ( limit/norm );
	}
    }
    
    // Move the entire layout while keeping its barycenter at the origin of the
    // screen coordinates
    Coord2 average;
    average.zero();
    BGL_FORALL_VERTICES( vd, g, Graph ) {
        Coord2 newcoord = vertexCoord[ vd ] + vertexShift[ vd ];
        average += newcoord;
    }
    average /= ( double )num_vertices( g );
    
    BGL_FORALL_VERTICES( vd, g, Graph ) {
        vertexShift[ vd ] -= average;
    }
    
    // Force the vertices stay within the screen
    const double central = CONTENT_RATIO;
    // double central = 0.90;
    // double minX = -central * SCREEN_SIDE;
    // double minY = -central * SCREEN_SIDE;
    // double maxX =  central * SCREEN_SIDE;
    // double maxY =  central * SCREEN_SIDE;
    double minX = _center_x - central * _window_side * SCREEN_SIDE;
    double minY = _center_y - central * _window_side * SCREEN_SIDE;
    double maxX = _center_x + central * _window_side * SCREEN_SIDE;
    double maxY = _center_y + central * _window_side * SCREEN_SIDE;
    // cerr << "[ " << minX << " , " << maxX << " ]  x [ " << minY << " , " << maxY << " ] " << endl;
    BGL_FORALL_VERTICES( vd, g, Graph ) {
	if ( vertexSwitch[ vd ] ) {
	    Coord2 newcoord = vertexCoord[ vd ] + vertexShift[ vd ];
	    if ( newcoord.x() < minX )
		vertexShift[ vd ].x() += minX - newcoord.x();
	    if ( newcoord.x() > maxX )
		vertexShift[ vd ].x() += maxX - newcoord.x();
	    if ( newcoord.y() < minY )
		vertexShift[ vd ].y() += minY - newcoord.y();
	    if ( newcoord.y() > maxX )
		vertexShift[ vd ].y() += maxY - newcoord.y();
	}
    }

    BGL_FORALL_VERTICES( vd, g, Graph ) {
        vertexCoord[ vd ] += vertexShift[ vd ]; //vertexCoord[ vd ] renew;
        err += vertexShift[ vd ].squaredNorm();
    }

    return err/( double )num_vertices( g );
}


//------------------------------------------------------------------------------
//	Public Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	コンストラクタ
//	Constructors
//------------------------------------------------------------------------------

//
//  Net::Net --	default constructor
//			コンストラクタ(デフォルト)
//
//  Inputs
//	none
//
//  Outputs
//	none
//
Net::Net() 
      : Graph()  	
{
    _init();
    _random();
}


//
//  Net::Net --	copy constructor
//			コピー・コンストラクタ
//
//  Inputs
//	obj	: object of this class
//
//  Outputs
//	none
//
Net::Net( const Net & obj )
      : Graph( ( const Graph & ) obj )
{
    _init();
    _random();
    if ( this != &obj ) {
	( Graph & )*this = obj;
    }
}


//------------------------------------------------------------------------------
//	デストラクタ
//	Destructor
//------------------------------------------------------------------------------

//
//  Net::~Net --	destructor
//			デストラクタ
//
//  Inputs
//	none
//
//  Outputs
//	none
//
Net::~Net()
{

}


//------------------------------------------------------------------------------
//	メンバ参照
//	Referrring to members
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//	Misc. functions
//------------------------------------------------------------------------------



void Net::setVDCoord( int x, int y )
{
    double rx = 2.0 * ( double )x /( double )_width - 1.0;
    double ry = -2.0 * ( double )y /( double )_height + 1.0;
    double sx = rx * _window_side * SCREEN_SIDE + _center_x;
    double sy = ry * _window_side * SCREEN_SIDE + _center_y;
    Coord2 coord = Coord2( sx, sy );
    put( vertex_mycoord, *this, _curVD, coord );
}


//------------------------------------------------------------------------------
//	ソートのための関数
//	Functions for sorting
//------------------------------------------------------------------------------

#ifdef SKIP
//
//  compare --	comparison function for sorting
//		ソートのための比較関数
//
//  Inputs
//	a, b	: pointers to objects of this class
//		  このクラスへのオブジェクトへのポインタ
//
//  Outputs
//	a <  b	-1
//	a == b	 0
//	a >  b	 1
//
int compare ( const Net * a, const Net * b )
{
    int (*cf)( const Graph *, const Graph * );
    cf = compare;
    return ( (*cf)( &(a->Graph), &(b->Graph) ) );

    return compare( ( const Graph * )a, ( const Graph * )b );
}
#endif	// SKIP


//------------------------------------------------------------------------------
//	等号
//	equality
//------------------------------------------------------------------------------

#ifdef SKIP
//
//  operator == --	equality
//			等号
//
//  Inputs
//	a, b	: objects of this class
//
//  Outputs
//	boolean value
//
int operator == ( const Net & a, const Net & b )
{

    return ( ( const Graph & )a == ( const Graph & )b );
}
#endif	// SKIP


//------------------------------------------------------------------------------
//	代入演算子
//	Assignment opereators
//------------------------------------------------------------------------------

//
//  Net::operator = --	assignement
//				代入
//
//  Inputs
//	obj	: objects of this class
//
//  Outputs
//	this object
//
Net & Net::operator = ( const Net & obj )
{
    if ( this != &obj ) {
	( Graph & )*this = obj;
    }
    return *this;
}


//------------------------------------------------------------------------------
//	入出力
//	I/O functions
//------------------------------------------------------------------------------

//
//  operator << --	output
//
//  Argument
//	stream	: output stream
//	obj	: object of this class
//
//  Outputs
//	output stream
//
ostream & operator << ( ostream & stream, const Net & obj )
{
    // stream << ( const Graph & )obj;	
    return stream;
}


//
//  operator >> --	input
//
//  Inputs
//	stream	: input stream
//	obj	: object of this class
//
//  Outputs
//	input stream
//
istream & operator >> ( istream & stream, Net & obj )
{
    // stream >> ( Graph & )obj;
    return stream;
}


// end of header file
// Do not add any stuff under this line.
