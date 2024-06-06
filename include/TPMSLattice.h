/*
    Copyright:   ---,
    License:     --,     
    Author:      SBN,
    Version:     1.0,     
    Date:        October, 2021,
    Description: 
        This file represnt all classes specfication and functions prototypes for 
        construction of 2 intertwined gyroid skeletals. These skeletals(implmented
        class) consist of two Graphs(implmented class) and Line (implemented class). 
        
      
    Caution: These classes and functions have nothing to do surfaces; they are
             computing skeletal graphs. Despite this, they are valuable that 
             might be useful for do reconstructiong expanding the strucrue. So, 
             in order to use these classes for surface reconstruction, use these
             class functions to obtain graph skeletals and generatos vectors.
             They you can then be freely used in an upper-level program to use
             the coordinates of graphs and generatos vectors to reconstruct and
             expand the though structure.

*/

#ifndef TPMSLATICE_H
#define TPMSLATICE_H
 
#include<vector>




////////////////////////////////////////////////////////////////////////////////

// This class is used for line concepts in our codes.
// Given two ending points('s_point' and 'e_point') of a line, the 
// Linesegment class lets you have a sort of data type for
// working with lines.  
// Example: LineSegment line1(p1, p2); // initalization of an object
class LineSegment{
    private:    
        double s_point[3], e_point[3];                         // Two ending points of the line
    public:
        void GetStartPoint(double s[3]);                       // Start point of the line
        void GetEndPoint(double e[3]);                         // End point of the line
        LineSegment(const double a[3], const double b[3]);     // Constructor
        double ShortestDistanceToline(const double [3]);       // Finds the shorest distance from a point
        double GetLength();                                    // Gives the line length
};



////////////////////////////////////////////////////////////////////////////////
// This class represents a graph consisting of a group of lines. 
// It is used for graph concepts in our codes.
// Given a bunch of lines, the resultant graph is produced. 
class Graph{
    private:
        int  line_count;                                         // number of lines in graph   
        std::vector<LineSegment> line;                           // a composition of class 
        
    public:       
        //Graph(std::<vector> input_lines);                      // to be implemented 
        Graph();                                                 // Defualt constructor    
        void AddLine(const double [3], const double [3]);        // adds line to graph   
        LineSegment GetLineByIndex( int index);                  // geting a line segment
        int GetLineCount();                                      // geting line counts in graph
        double GetShortestDistanceToGraph(const double [3]);     // the shorest distance to graph 
};


// This class produces a gyroid like skelton (2 graphs).
// It recieves 2 parameter arrays (of 6 edge lengths and 5 angle degrees) and 
// parameterizes the gyroid skeleton into a new one. 
// For example, X = [1,1,1,1,1] with all degrees 60,  gives an 'srs' 
// network (standard gyroid),  and X = [1,1,1,3,3,3] gives back 'ths' network.
//
// Examples: 
//  degrees := [2*pi/3, 2*pi/3, 2*pi/3, 2*pi/3, 2*pi/3]
//  X := [1, 1, 1, 1, 1, 1]
// then, GyroidSkeletal(X,  degrees, 0,0,0) produces standard gyroid and
// GyroidSkeletal(X, degrees, 2, 2, 2)  produces a structure similar to gyroid 
// with -2,-1, 0, 1, 2 as its generator coefficeients.
class GyroidSkeletal{

    private:
        
        int dim1, dim2, dim3;                                    // dimensions of skeleton
        
        double X[6], Theta[5];                                   // Parameters
        double parameterized_edg[6][3]; //+ from a local param into private data member 

        /* - */                                   
            // Graph graph1;                                            // first graph of the skeleton
            // Graph graph2;
        /* + */                                           // second graph of the skeleton
        
        // double building_block_edg[6][2][3];                      // gyroid regular buildingblock
        double building_block_vert[7][3];                        // gyroid regular buildingblock
        double Parameterized_building_block_vert[7][3];          // gyroid-like  buildingblock
        double neg_Parameterized_building_block_vert[7][3];
        double generator1[3], generator2[3], generator3[3];      // generators of network
        double wall_coordinates[3];                              // for making 2nd channel's network graph (graph2) we need the coordinates of the wall which is made by max and min coordintes
        double min_coordinates[3]; double max_coordinates[3];  
        
        /* + */
        double max_g[3], min_g[3];
        double gmax_vec[3][3], gmin_vec[3][3];
        double start_point[3];  // unitcell's start_point ---> computed with gmax[] and gmin[]

        Graph buildblock_graph1;
        Graph buildblock_graph2;

        Graph network_graph1;
        Graph network_graph2;

        Graph embracing_graph1;
        Graph embracing_graph2; // surroudning skeletal to help find the most accurate surf/vol points


        /* + */        
        void ComputeBuildingBlock();                             // build edges and vertices of building block -- original ones without parameterization
        void ParameterizeBuildingBlock();                        // Parmaterize the building block

        void ComputGenerators();                                 // compute the three generators of network   
        void CreateGraphStructures( Graph & graph1, Graph & graph2, int d1, int d2, int d3);// create and expand the skeletal's graphs   
        void CreateBuildBlockGraphs();// unitcell graph
        void CreateNetworkStructure();
        void CreateEmbracingGraphs();

        
        void SetMinMaxCoordinators();                            // setting max & min coordinates    
        void SetWallCoordinators();                              // setting wall coordinates.

        void RotateVector(const double [3], 
                          const double [3], 
                          const double rotate_degree, 
                          double[3]);                            // Rotate a vector in a plane.

        /* + */
        void FindMinMax_ThroughtGenerators();
        void BuildEmbracingGraphs();
        
                           
    public:
        
        // constructors----
        GyroidSkeletal( const double x[6], 
                        const double theta[5], 
                        int d1=0, int d2=0, int d3=0);           // general constructor

        GyroidSkeletal(int d1=0, int d2=0, int d3=0);            // defualt constructor 
        // ----constructor.


        double DifferenceDistanceToGraphs(double point[3]);      // diff distance from point to graphs
        
        
  
        Graph GetBuildBlockGraph1();
        Graph GetBuildBlockGraph2();

        Graph  GetNetworkGraph1();
        Graph  GetNetworkGraph2();

        Graph GetEmbracingGraph1();
        Graph GetEmbracingGraph2();
        
        void GetMinCoordinators(double mincordin[3]);
        void GetMaxCoordinators(double maxcordin[3]);
        void GetGenerators(double[3], double[3], double[3]);

        // ToBe implemented for fog=f(g(x))
        void UpdateAngle(double angled_vert[7][3], const double target_vert[7][3], double theta[5]);
        void UpdateEdgeLength(double edged_vert[7][3], double parameterized_edg[6][3],  const double target_vert[7][3], double x[6]);
        void GetParameterizedBuildingBlockVerts(double v[7][3]);
        void GetParameters(double x[6], double theta[5]);

        //  it checks the point be surface point on the domain defined in constructor
        bool Surface(double point[3]);                           // specifies if a point is on surface
        bool Volume(double point[3]);                            //   "     if a point in pos/neg volum


        // it checkes if the given point is on surface or not. It does not matter
        //  if it is on the defined domain or not... It checks the possiblity of
        //  a point to be surface no matter where on space. 
        bool IsSurfacePoint(double test_point[3]);
        bool IsVolumePoint(double test_point[3]);

        // newly added 
        double min_dist(double test_point[3]);
        
};




#endif //TPMSLATICE_H
