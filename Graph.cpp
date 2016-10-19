#include "common.h"
#include "Graph.h"


//------------------------------------------------------------------------------
//  Specific function
//------------------------------------------------------------------------------
void resetGraph( Graph & graph )
{
    VertexCoordMap      vertexCoord     = get( vertex_mycoord, graph );

    BGL_FORALL_VERTICES( vd, graph, Graph ) {
	vertexCoord[ vd ] = Coord2( SCREEN_SIDE*(2.0*drand48()-1.0), SCREEN_SIDE*(2.0*drand48()-1.0) );
    }
}


void loadGraph( Graph & graph, const char * filename )
{
    VertexIDMap		vertexID	= get( vertex_myid, graph );
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
    cerr << HERE << "Number of vertices = " << numV;

    // information about each vertex
    for ( unsigned int i = 0; i < numV; ++i ) {
	string name;
	double weight;
	ifs.getline( buf, sizeof( buf ) );
	istr.clear();
	istr.str( buf );
	istr >> name >> weight;
	cerr << HERE << "Vertex name = " << name.c_str() << " weight = " << weight << endl;
	VertexDescriptor vd = add_vertex( graph );
	vertexID[ vd ] = i;
	for ( unsigned int k = 0; k < name.length(); ++k ) 
	    if ( name[ k ] == '_' ) name[ k ] = ' ';
	vertexName[ vd ] = name;
	vertexWeight[ vd ] = weight;
	vertexCoord[ vd ] = Coord2( SCREEN_SIDE*(2.0*drand48()-1.0), SCREEN_SIDE*(2.0*drand48()-1.0) );
    }

    // number of edges
    unsigned int numE;
    ifs.getline( buf, sizeof( buf ) );
    istr.clear();
    istr.str( buf );
    istr >> numE;
    cerr << HERE << "Number of edges = " << numE;

    // link the pair of endpoints with each edge
    for ( unsigned int i = 0; i < numE; ++i ) {
	unsigned int idO, idD;
	ifs.getline( buf, sizeof( buf ) );
	istr.clear();
	istr.str( buf );
	istr >> idO >> idD;
	cerr << HERE << "Edge = " << idO << " -- " << idD << endl;
	VertexDescriptor vdO = vertex( idO, graph ); 
	VertexDescriptor vdD = vertex( idD, graph ); 
	bool is;
	EdgeDescriptor ed;
	tie( ed, is ) = add_edge( vdO, vdD, graph );
    }

    return;
}


