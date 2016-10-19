#ifndef _Graph_H
#define _Graph_H

#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include <ctime>
#include <cstdlib>

using namespace std;

#include <boost/config.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/iteration_macros.hpp>
#include <boost/graph/betweenness_centrality.hpp>

using namespace boost;

#include "Coord2.h"

//------------------------------------------------------------------------------
//  Boost customization
//------------------------------------------------------------------------------
// Vertex attributes
// Label
enum vertex_mylabel_t		{ vertex_mylabel };
// Switch
enum vertex_myswitch_t		{ vertex_myswitch };
// Positions
enum vertex_mycoord_t		{ vertex_mycoord };
// Vertex name
enum vertex_myname_t		{ vertex_myname };
// Weight values
enum vertex_myweight_t		{ vertex_myweight };
// Cell width
enum vertex_mywidth_t		{ vertex_mywidth };
// Cell height
enum vertex_myheight_t		{ vertex_myheight };
// Total displacements
enum vertex_myshift_t		{ vertex_myshift };
// Displacements by the force-directed model
enum vertex_myforce_t		{ vertex_myforce };
// ID
enum vertex_myid_t		{ vertex_myid };
//centrality



// Label
enum edge_mylabel_t		{ edge_mylabel };
// ID
enum edge_myid_t		{ edge_myid };

namespace boost {
    // vertex properties
    BOOST_INSTALL_PROPERTY( vertex, mylabel );
    BOOST_INSTALL_PROPERTY( vertex, myswitch );
    BOOST_INSTALL_PROPERTY( vertex, mycoord );
    BOOST_INSTALL_PROPERTY( vertex, myname );
    BOOST_INSTALL_PROPERTY( vertex, myweight );
    BOOST_INSTALL_PROPERTY( vertex, mywidth );
    BOOST_INSTALL_PROPERTY( vertex, myheight );
    BOOST_INSTALL_PROPERTY( vertex, myshift );
    BOOST_INSTALL_PROPERTY( vertex, myforce );
    BOOST_INSTALL_PROPERTY( vertex, myid );
    // edge properties
    BOOST_INSTALL_PROPERTY( edge, mylabel );
    BOOST_INSTALL_PROPERTY( edge, myid );
}


//------------------------------------------------------------------------------
//	Customizing vertex properties
//------------------------------------------------------------------------------
// Vertex named ID 
typedef property< vertex_mylabel_t, unsigned int >		MyVLabel;
// Vertex annotation switch
typedef property< vertex_myswitch_t, bool, MyVLabel >		MyVSwitch;
// Vertex position
typedef property< vertex_mycoord_t, Coord2, MyVSwitch >		MyVCoord;
// Vertex name
typedef property< vertex_myname_t, string, MyVCoord >		MyVName;
// Vertex weight
typedef property< vertex_myweight_t, double, MyVName >		MyVWeight;
// Vertex width
typedef property< vertex_mywidth_t, int, MyVWeight >		MyVWidth;
// Vertex height
typedef property< vertex_myheight_t, int, MyVWidth >		MyVHeight;
// Total vertex displacement
typedef property< vertex_myshift_t, Coord2, MyVHeight >		MyVShift;
// Vertex displacement by the force-directed model
typedef property< vertex_myforce_t, Coord2, MyVShift >		MyVForce;
// Vertex ID
typedef property< vertex_myid_t, unsigned int, MyVForce >	MyVertexProperty;

//------------------------------------------------------------------------------
//	Customizing edge properties
//------------------------------------------------------------------------------
// Edge ID
typedef property< edge_mylabel_t, unsigned int >		MyELabel;
// Edge label
typedef property< edge_myid_t, unsigned int, MyELabel >		MyEdgeProperty;

//------------------------------------------------------------------------------
//	Composing graph structure
//------------------------------------------------------------------------------
typedef adjacency_list< vecS, listS, undirectedS,
			MyVertexProperty, MyEdgeProperty >	Graph;

typedef graph_traits< Graph >					GraphTraits;
typedef graph_traits< Graph >::vertex_descriptor		VertexDescriptor;
typedef graph_traits< Graph >::edge_descriptor			EdgeDescriptor;

typedef graph_traits< Graph >::vertex_iterator			VertexIterator;
typedef graph_traits< Graph >::edge_iterator			EdgeIterator;

// Vertex property map
typedef property_map< Graph, vertex_mylabel_t >::type		VertexLabelMap;
typedef property_map< Graph, vertex_myswitch_t >::type		VertexSwitchMap;
typedef property_map< Graph, vertex_mycoord_t >::type		VertexCoordMap;
typedef property_map< Graph, vertex_myname_t >::type		VertexNameMap;
typedef property_map< Graph, vertex_myweight_t >::type		VertexWeightMap;
typedef property_map< Graph, vertex_mywidth_t >::type		VertexWidthMap;
typedef property_map< Graph, vertex_myheight_t >::type		VertexHeightMap;
typedef property_map< Graph, vertex_myshift_t >::type		VertexShiftMap;
typedef property_map< Graph, vertex_myforce_t >::type		VertexForceMap;
typedef property_map< Graph, vertex_myid_t >::type		VertexIDMap;

// Edge property map
typedef property_map< Graph, edge_mylabel_t >::type		EdgeLabelMap;
typedef property_map< Graph, edge_myid_t >::type		EdgeIDMap;

#endif  // _Graph_H

