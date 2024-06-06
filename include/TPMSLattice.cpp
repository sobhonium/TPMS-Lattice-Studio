/*

The difference between this file testclass4__.cpp and testclass.cpp is that
fog=gof is made possible. It adds more two public functions into public segmet:
 UpdateAngle(...) and UpdateEdgeLength(...). It also has a high lelvel of information hiding

Updates on this version: Everything rewritten to be in consitency with checking anypoint on space
                     rather than unitcell bounds...
Date: July 23, 2022                     
*/

// FIXIT(SBN): Change the term 'image' into 'project' for imaging vectors and 
                // it seams like we don't need abs on DifferenceDistanceToGraphs
                // // check. If we return the true(without abs) difference dists we can also use it for min_dist or function and this function  no longer needs to be added. Do it as soon as possible.
               
                

#include<vector>
#include<cmath>

#include"TPMSLattice.h"
#include"linalgebra4.h"

#include<iostream>

using namespace std;


///////////////////////////////////////////////////////////////////////////
// Constructor of class LineSegment. 
// It gets two points and assgin them as start and end points of a linesemnt.
LineSegment::LineSegment(const double point1[3], const double point2[3]){
    Assign(s_point, point1);
    Assign(e_point, point2);
};///

// This function return back the start point of a linesement. In fact it fills 
// the sent input array s[3] by the start point of the linesegment.
void LineSegment::GetStartPoint(double s[3]){
    Assign(s, s_point);
}///

// This function return back the end point of a linesement. In fact it fills 
// the sent input array e[3] by the end point of the linesegment.
void LineSegment::GetEndPoint(double e[3]){
    Assign(e, e_point);
}///

// The shortest distance from a point test_point[3] to the linesegment is given
// by this function. This function calls DistPointToLine from linalgebra4.h.
// (Normal vector computation is deleted in this version of code). 
double LineSegment::ShortestDistanceToline(const double test_point[3]){
    return  DistPointToLine(test_point,
                            s_point, e_point
                            );
};///

// The linesegment's length is returned by running this function.
double LineSegment::GetLength(){
    return EcldDistance(s_point, e_point);
};///


///////////////////////////////////////////////////////////////////////////////
// Defualt constructor(programmer-defined defualt constructor)    
Graph::Graph(){
     //Graph constructor
     line_count = 0;  //empty graph   
}///;

// Receving two gaphs as input arguments, this member function creates a line 
// segemnt and adds that to graph
void Graph::AddLine(const double point1[3], const double point2[3]){
   
    line.push_back({point1, point2});
    line_count = line.size();
}///

// This function helps user retrieve a line by sending the index of that line
// in graph to the function.
LineSegment Graph::GetLineByIndex( int index){
    return line[index];
}///

// The number of graph's lines are given by this function.
int Graph::GetLineCount(){
    return line_count;
}///

// This Function calculates the shortest distance to graph, given a point in 
// Eclidean space. This function is updated (from v.1.0) in a way that it does 
// not compute the normal vector. 
double Graph::GetShortestDistanceToGraph(const double test_point[3]){


    double dis, min_dis; 
    min_dis = 9999.0;    //assigning a larg number for default min distance.

    for (int i = 0; i< GetLineCount(); i++)
    {   
        dis = GetLineByIndex(i).ShortestDistanceToline(test_point);

        if (dis < min_dis)
            min_dis = dis; 
                                          
    }

    return min_dis;
}///






///////////////////////////////////////////////////////////////////////////////
// General Constructor
// It runs several builtin (encapsulated/internaluse.) private functions to build up the inital
// Skeletal graphs. If fact this constructor (like any other standard constructor)
// initialize the data members' values. For GyroidSekeletal, the data members 
// are skeletal variables such as edges, vertices, and more importantly, gerator
// vectors that help expand the skeletal in different direction.
GyroidSkeletal::GyroidSkeletal( const double x[6], const double theta[5],  int d1, int d2, int d3){

    // filling data member X, Theta and dim1,2,3
    for (int i=0;i<5;i++)
        Theta[i] = theta[i]; //defualt degree is 60 

    for (int i=0;i<6;i++)
        X[i] = x[i];

    dim1=d1; dim2=d2; dim3=d3;
    
    // private functions that the client has no access to (Encapsulated to him).
    ComputeBuildingBlock();        // edges and vertices of building block graph are built
    ParameterizeBuildingBlock();   // Building block graph is changing based on customized parameters
    ComputGenerators();            // Generator vectors are computed.
    /* + */ 
    // filling up the vector variable wall_coordinates 
    SetMinMaxCoordinators();
    SetWallCoordinators();
    FindMinMax_ThroughtGenerators();
  
    /* + */

    CreateBuildBlockGraphs();
    CreateNetworkStructure();
    CreateEmbracingGraphs();   
    // cout<<"Parameterized Gyroid Skeletal is created Successfuly."<<endl;
}///

// Default constuctor
GyroidSkeletal::GyroidSkeletal( int d1, int d2, int d3){

    // filling data member X, Theta and dim1,2,3
    for (int i=0;i<5;i++)
        Theta[i] = 2* M_PI/3; // defualt angle degree is 60 

    for (int i=0;i<6;i++)
        X[i] = 1;             // default edge length is 1

    dim1=d1; dim2=d2; dim3=d3;
    
    // private functions that the client has no access to (Encapsulated to him).
    ComputeBuildingBlock();        // edges and vertices of building block graph are built
    ParameterizeBuildingBlock();   // Building block graph is changing based on customized parameters
    ComputGenerators();            // Generator vectors are computed.
    /* + */ 
    // filling up the vector variable wall_coordinates 
    SetMinMaxCoordinators();
    SetWallCoordinators();
    FindMinMax_ThroughtGenerators();
    /* + */

    CreateBuildBlockGraphs();
    CreateNetworkStructure();
    CreateEmbracingGraphs();   
};///



// This function builds the buildng block skeltons
void GyroidSkeletal::CreateBuildBlockGraphs(){
    // cout<<"CreateNetworkStructure function is calling..."<<endl;
    CreateGraphStructures(buildblock_graph1, buildblock_graph2, 1,1,1);};// unitcell graph

// Creates the entire network through the given values of dim1, dim1, dim3.
// the network will be created through the generator directions (not the standard direction).
void GyroidSkeletal::CreateNetworkStructure(){
    // cout<<"CreateNetworkStructure function is calling..."<<endl;
    CreateGraphStructures(network_graph1,network_graph2,  dim1, dim2, dim3);} ;


// This builtin function runs only within constructor and let's reduce the lines
// checking process by a considerable far minimal algorithm.
// It is known that in order to produce an exact unitcell (or any extended shape)
// larger skeleton should be chosen to embrace the unitcell domain to let it 
// be as accurate as possible. This function if simply developed to crate the embracing
// graph that surround the building block skeleton. The minimality it provides rooted
// from this fact that we consider a growth value to let the skeltons be larger
// than unitcell domain by the amount of this value. experimental it turns out 
// the 0.3 as a growth scale is suffuciently large enousht to presents a nice 
// and accurate embracing graphs. The function below looks at a larger skeleton
// lines (which has 162 lines) and picks the bests out of them in order to make 
// a satisfactory embracing graph.
// Further, the embracing graphs are used to retrieved the distance of a point 
// and that the sole purpose of using them.
// * The number of minimla lines to embrace the building block skelton might be 
// different in each of embracing graphs namely embracing_graph1, or embracing_graph2
void GyroidSkeletal::CreateEmbracingGraphs(){
    // cout<<"CreateEmbracingGraph function is calling..."<<endl;
    //-
    // CreateGraphStructures(embracing_graph1, embracing_graph2, 2,2,2);
  
    // +
    // in comparison with what we used to do and created the embracing graphs with
    // skelet(2,2,2), this method here reduces embracing graphs line numbers from
    //  162 into 38---> 4 times faster (as an example).

    double temp1[3], temp2[3];
    double s_line_point[3], e_line_point[3]; 
    double scaled_g1[3], scaled_g2[3],scaled_g3[3];

    double growth_scale = 0.6;
    bool cond1, cond2; 
    

    Graph init_graph1, init_graph2;
    CreateGraphStructures(init_graph1, init_graph2, 2,2,2);
    
    
    // which line of the initial graphs are a good candid to be in embracing graph.
    // i.e which line satisfies the condition of being close to the specified domain
    for (int i=0;i<init_graph1.GetLineCount();i++){
        init_graph1.GetLineByIndex(i).GetStartPoint(s_line_point);
        init_graph1.GetLineByIndex(i).GetEndPoint(e_line_point);

        Minus(temp1,s_line_point, start_point);
        Minus(temp2,e_line_point, start_point); 
        
        // start or end point of a line in the inital graph should be in the domain
        // defined by generators and growth_scale. if so, the line is said to be 
        // a minimal line for the embracing graph.
        cond1 = IsPointOnDomain(temp1, generator1, generator2, generator3, growth_scale);
        cond2 = IsPointOnDomain(temp2, generator1, generator2, generator3, growth_scale);
        
        // head or tail  of a line should be in domain. if so, that is a nice and minal line
        // so let's pick it.
        if(cond1 || cond2){
            embracing_graph1.AddLine(s_line_point, e_line_point);
        } //if
    }// for

    for (int i=0;i<init_graph2.GetLineCount();i++){
        init_graph2.GetLineByIndex(i).GetStartPoint(s_line_point);
        init_graph2.GetLineByIndex(i).GetEndPoint(e_line_point);

        Minus(temp1,s_line_point, start_point);
        Minus(temp2,e_line_point, start_point); 

        cond1 = IsPointOnDomain(temp1, generator1, generator2, generator3, growth_scale);
        cond2 = IsPointOnDomain(temp2, generator1, generator2, generator3, growth_scale);
        

        if(cond1 || cond2){
            embracing_graph2.AddLine(s_line_point, e_line_point);
        } //if
    }// for



// cout<<"Number of embracing lines1: "<<embracing_graph1.GetLineCount()<<endl;
// cout<<"Number of embracing lines2: "<<embracing_graph2.GetLineCount()<<endl;

}; /// sorounding graph for exact point check



// In this function of class, the vertices of building block gyroid are
// initialized. 
// * Client has no access to run this function from his client code. Only 
// member functions have access to this function (client does not need to know the
// implementation).
void GyroidSkeletal::ComputeBuildingBlock(){

    // The initial vertices locations of building block. They are computed by 
    // embeding a gyroid standard structure into 3D space. 
    // It is believed that having these vertices is enough to built up the 
    // whole gyoid network structure (Generator vectors are also needed).
    double init_vert[7][3] =  {{0.62500, 0.62500, 0.62500},
                                {0.87500, 0.62500, 0.37500},
                                {0.37500, 0.87500, 0.62500}, 
                                {0.62500, 0.37500, 0.87500}, 
                                {0.87500, 0.37500, 0.12500}, 
                                {0.12500, 0.87500, 0.37500},     
                                {0.37500, 0.12500, 0.87500}};
    
    for (int i = 0;i < 7; i++)
        Assign(building_block_vert[i], init_vert[i]);// w in maple

    /* still not necessay to create edges of building block. If needed, uncomment
    this snippet.
    int edg_pattern[6][2] = {{0,1}, {0,2}, {0,3}, {1,4}, {2,5}, {3,6}}


    Assign(building_block_edg[0][0], building_block_vert[0]);
    Assign(building_block_edg[0][1], building_block_vert[1]);
    
    Assign(building_block_edg[1][0], building_block_vert[0]);
    Assign(building_block_edg[1][1], building_block_vert[2]);
    
    Assign(building_block_edg[2][0], building_block_vert[0]);
    Assign(building_block_edg[2][1], building_block_vert[3]);
    
    Assign(building_block_edg[3][0], building_block_vert[1]);
    Assign(building_block_edg[3][1], building_block_vert[4]);
    
    Assign(building_block_edg[4][0], building_block_vert[2]);
    Assign(building_block_edg[4][1], building_block_vert[5]);
    
    Assign(building_block_edg[5][0], building_block_vert[3]);
    Assign(building_block_edg[5][1], building_block_vert[6]);
   */
};


// The whole paramterization procedure is done through running this function.
// It needs two steps to paramterize the whole structure: 
    // 1) degree parameterziation; 
    // 2) length parameterization.
// After running this function, the building block of gyroid structure is 
// parameterized and it will look different than what it used to be like.
// * Client has no access to run this function from the client code. 

// reciveing an array of points (skeletal points) change the angle between lines (created with those points)
// and return an anglized skeleton
void GyroidSkeletal::UpdateAngle(double angled_vert[7][3], const double target_vert[7][3], double theta[5]){
   // shown with p11 in maple codes
    // ---> angled_vert[7][3];
    
    // # line1;
    // p_rotated[0] := <p[0]>;
    // p_rotated[1] := <p[1]>;
    Assign(angled_vert[0], target_vert[0]); 
    Assign(angled_vert[1], target_vert[1]);
 
    double av[3];

    // # line2
    // v1 := p[1] - p[0];
    // v2 := p[2] - p[0];
    // p_rotated[2] := Rot(v1, v2, Theta[1]) + p_rotated[0];
    double v1[3], v2[3];
    Minus(v1, target_vert[1], target_vert[0]);
    Minus(v2, target_vert[2], target_vert[0]);
    RotateVector(v1, v2, theta[0], av); 
    Plus(angled_vert[2], av, angled_vert[0]);
    

   

    // line3
    // v1 := p[1] - p[0];
    // v2 := p[2] - p[0];
    // p_rotated[3] := Rot(v1, v2, 2*Pi - Theta[2]) + p_rotated[0];
    Minus(v1, target_vert[1], target_vert[0]);
    Minus(v2, target_vert[2], target_vert[0]);
    RotateVector(v1, v2, 2*M_PI - theta[1], av);
    Plus(angled_vert[3], av, angled_vert[0]);
    
  


    // line4
    // v1 := p[0] - p[1];
    // v2 := p[4] - p[1];
    // p_rotated[4] := Rot(v1, v2, Theta[3]) + p_rotated[1];
    Minus(v1, target_vert[0], target_vert[1]);
    Minus(v2, target_vert[4], target_vert[1]);
    RotateVector(v1, v2, theta[2], av);
    

    Plus(angled_vert[4], av, angled_vert[1]);
  


    // line5
    // v1 := p[0] - p[2];
    // v2 := p[5] - p[2];
    // p_rotated[5] := Rot(v1, v2, Theta[4]) + p_rotated[2];
    Minus(v1, target_vert[0], target_vert[2]);
    Minus(v2, target_vert[5], target_vert[2]);
    RotateVector(v1, v2, theta[3], av);
       
    Plus(angled_vert[5], av, angled_vert[2]);
    

    // line6;
    // v1 := p[0] - p[3];
    // v2 := p[6] - p[3];
    // p_rotated[6] := Rot(v1, v2, Theta[5]) + p_rotated[3];
    Minus(v1, target_vert[0], target_vert[3]);
    Minus(v2, target_vert[6], target_vert[3]);
    RotateVector(v1, v2, theta[4], av);
    
    Plus(angled_vert[6], av, angled_vert[3]);
    
    // for (int i=0;i<7;i++)
    //     cout<<angled_vert[i][0]<<" "<<angled_vert[i][1]<<" "<<angled_vert[i][2]<<endl;

}///


// parameterze the lengths of edges in skeletal.
// this function receive aan array of points (skeletal points) change the edge len and redefines 
// the vertices positions.
//  maybe the name should be changed !
void GyroidSkeletal::UpdateEdgeLength(double edged_vert[7][3], double parameterized_edg[6][3], const double target_vert[7][3], double x[6]){
    


    // we have e0...e1 that are parmetized by X 
    Minus(parameterized_edg[0], target_vert[1], target_vert[0]); // e0= v1-v0
    //param is: e0 = e0*X[0]
    multiplyScalarVector(parameterized_edg[0], parameterized_edg[0], x[0]); 

    Minus(parameterized_edg[1], target_vert[2], target_vert[0]); // e1= v2-v0
    //param is: e1 = e1*X[1]
    multiplyScalarVector(parameterized_edg[1], parameterized_edg[1], x[1]); 

    Minus(parameterized_edg[2], target_vert[3], target_vert[0]); // e2= v3-v0
    //param is: e2 = e2*X[2]
    multiplyScalarVector(parameterized_edg[2], parameterized_edg[2], x[2]); 
    

    
    
    // Doing v[1] := v0 + e[1]
    //       v[2] := v0 + e[2]
    //       v[3] := v0 + e[3]

    //Parameterized_building_block_vert[7][3] is here used equal to v in maple 

    Assign(edged_vert[0], target_vert[0]);
    Plus(edged_vert[1],
         edged_vert[0], 
         parameterized_edg[0]);
    Plus(edged_vert[2], 
         edged_vert[0], 
         parameterized_edg[1]);
    Plus(edged_vert[3], 
         edged_vert[0], 
         parameterized_edg[2]);   


    //v[4] := v[1] + (p1[4] - p1[1])*x[4]
    double temp[3], tm[3];
    Minus(tm, target_vert[4], target_vert[1]);
    multiplyScalarVector(temp, tm, x[3]);
    Plus(edged_vert[4],  
         edged_vert[1],
         temp);

    // v[5] := v[2] + (p1[5] - p1[2])*x[5]
    Minus(tm, target_vert[5], target_vert[2]);
    multiplyScalarVector(temp, tm, x[4]);
    Plus(edged_vert[5],  
         edged_vert[2],
         temp);
    
    //v[6]= v[3] + (p1[6] - p1[3])*x[6]
    Minus(tm, target_vert[6], target_vert[3]);
    multiplyScalarVector(temp, tm, x[5]);
    Plus(edged_vert[6],  
         edged_vert[3],
         temp);     


    //e[4] := v[4] - v[1] - v0;
    //e[5] := v[5] - v[2] - v0;
    //e[6] := v[6] - v[3] - v0; 
    Minus(parameterized_edg[3], 
          edged_vert[4], 
          edged_vert[1], 
          target_vert[0]);

    Minus(parameterized_edg[4], 
          edged_vert[5], 
          edged_vert[2], 
          target_vert[0]); 

    Minus(parameterized_edg[5], 
          edged_vert[6], 
          edged_vert[3], 
          target_vert[0]);      

}///

void GyroidSkeletal::ParameterizeBuildingBlock(){
    

    // 1) first paramterize edges with angles (not lengths):
        double angled_vert[7][3];
        UpdateAngle(angled_vert, building_block_vert, Theta);


    // 2) Now parameterize edges(obviously some vertices are also changed):

    // parameterized_edg[6][3];// e in maple code
        UpdateEdgeLength(Parameterized_building_block_vert, parameterized_edg, angled_vert, X);
    
};///


// Given a vector (to rotate) and another vector from the same plane, 
// this function rotates the first vector for about certain angle degree in that
// plane.
void GyroidSkeletal::RotateVector(const double target_vec_to_rotate[3], const double help_planmake_vec[3], const double rotate_degree, double result_rotated_vec[3]){

    double k[3], vec_n[3];
    CrossProduct(vec_n, target_vec_to_rotate, help_planmake_vec);
    NormalVector(k, vec_n);
    
    // rodridugs formula:
    //v1_rot := v1*cos(Theta) + CrossProduct(k, v1)*sin(Theta) + k*(1 - cos(Theta))*DotProduct(k, v1)
    double temp1[3], temp2[3], temp3[3], tm[3];
    multiplyScalarVector(temp1, target_vec_to_rotate, cos(rotate_degree));

    CrossProduct(tm, k, target_vec_to_rotate);
    multiplyScalarVector(temp2, tm, sin(rotate_degree));

    // Important: k*(1 - cos(Theta))*DotProduct(k, v1) is always 0 for our application. beacuse k is prependicular to v1. BTW, for keeping the generality of code we compute this:
 
    multiplyScalarVector(temp3, k, (1-cos(rotate_degree)) * DotProduct(k, target_vec_to_rotate));

    // you can testify that this is 0
    // cout<<temp3[0]<<" "<<temp3[1]<<" "<<temp3[2]<<endl;
  
    // v1_rot*Norm(v2, 2)/Norm(v1, 2) // useful for fog(x)=gof(x)
    Plus(tm, temp1, temp2, temp3);
    multiplyScalarVector(result_rotated_vec, tm, Norm(help_planmake_vec)/Norm(target_vec_to_rotate));

// cout<<"------------------"<<endl;
//     cout<<"v1=   ["<<target_vec_to_rotate[0]<<" "<<target_vec_to_rotate[1]<<" "<<target_vec_to_rotate[2]<<"]"<<endl;
//     cout<<"v2=   ["<<help_planmake_vec[0]<<" "<<help_planmake_vec[1]<<" "<<help_planmake_vec[2]<<"]"<<endl;
//     cout<<"v_rot=["<<result_rotated_vec[0]<<" "<<result_rotated_vec[1]<<" "<<result_rotated_vec[2]<<"] "<<endl;

};///


// This function computes the three generators of gyroid buildinb block.
// It builds up neccesary matrices that help find the generators.   
void GyroidSkeletal::ComputGenerators(){
    
    
    double temp[3];
    // the formula is this:
    // Upsilon[1] := <e[1] + e[4] - e[2] + v0>;
    // Upsilon[2] := <e[2] + e[5] - e[3] + v0>;
    // Upsilon[3] := <e[3] + e[6] - e[1] + v0>;
    double upsil[3][3];
    Plus(temp, parameterized_edg[0], parameterized_edg[3], building_block_vert[0]);
    Minus(upsil[0], temp, parameterized_edg[1]);
    
    Plus(temp, parameterized_edg[1], parameterized_edg[4], building_block_vert[0]);
    Minus(upsil[1], temp, parameterized_edg[2]);  

    Plus(temp, parameterized_edg[2], parameterized_edg[5], building_block_vert[0]);
    Minus(upsil[2], temp, parameterized_edg[0]); 

    // Upsilon^T
    double upsil_trans[3][3];
    MatrixTrans(upsil_trans, upsil);
    

    //A := Upsilon * Upsilon^T
    double A[3][3];

     for(int i = 0; i < 3; i++)
        for(int k = 0; k < 3; k++)
             A[i][k] =  DotProduct(upsil[i], upsil[k]);
    
    // MultiplyMatrixMatrix(A, upsil, upsil_trans );// is also correct for A
   

    //b_hat := Upsilon * e
    double b_hat[3][6];
    for(int i = 0 ; i < 3; i++)
        for(int k = 0; k < 6;k++)
            b_hat[i][k] = DotProduct(upsil[i], parameterized_edg[k]);

    //A inverse
    double A_inv[3][3];
    MatrixInverse(A_inv, A);

    // a_hat= A_inv*b_hat
    double a_hat[3][6];
    for(int i = 0;i < 3; i++)
        for(int k = 0; k < 6; k++)
            a_hat[i][k] = 0;


    for(int i = 0; i < 3; i++)
        for(int k = 0; k < 6; k++)
                for(int j = 0; j < 3; j++)
                    a_hat[i][k] = a_hat[i][k] + A_inv[i][j]*b_hat[j][k];


    double C[3][3];
    double C_inv[3][3];
    
    for (int i = 0; i < 3; i++)
        for (int j =  0; j < 3; j++)
            C[i][j] = a_hat[i][j+3]; // a[1..3][4..6] in maple
    
    
    MatrixInverse(C_inv, C);  
    
    // ED in maple code
    // here we show that with E:= Matrix([e[4], e[5], e[6]])
    double  E[3][3];
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            E[i][j] = parameterized_edg[i+3][j]; 
    
    // C_inv^T
    double C_inv_trans[3][3];
    MatrixTrans(C_inv_trans, C_inv);
    

    // alpha := C_inv^T * E 
    double alpha[3][3];
    MultiplyMatrixMatrix(alpha, C_inv_trans, E );



    // double g1[3], g2[3], g3[3]; // generators
    Row(generator1, alpha, 0);
    Row(generator2, alpha, 1);
    Row(generator3, alpha, 2);
}///


// This function is run (in private) after the generators are found.
// It looks at dim1, dim2, dim3 and also generators to see how much the skeletal
// should grow. Another task of this function is to build up the second skeletal.
// this function stores the skeletals in graph1, and graph2. These graphs are
// two objects of class 'Graph'. 
// In fact creation of the second graph from the frist graph is tricky and done
// by wall coordiantes of graph1-- this is a built-in symmetry feature of gyroid.
void GyroidSkeletal::CreateGraphStructures(Graph &graph1, Graph &graph2, int d1, int d2, int d3){



    double temp1[3]; 
    double temp2[3];
    double temp3[3]; 

    double su[3];             // su = (a*g1 + b*g2 + c*g3)/2 
     
    double pillar[7][3];      // pp1 in maple code (for line1)
    double neg_pillar[7][3];  // pp1 in maple code (for line2)
    double neg_vert[7][3];    // -p11 (we use this for creating graph2 line2)


    // creating other points in the nework in both channel + and -.
    for(int a = -(d1-1); a <= d1-1; a++)
        for(int b = -(d2-1); b <= d2-1; b++)
            for(int c = -(d3-1); c <= d3-1; c++) {    
                    // cout<<"Within "<<endl;
                    // Other points in the nework are obtained with generators
                    // su is a linear combination of generators,
                    // su = a * g1+ a * g1 + a * g1
                    multiplyScalarVector(temp1, generator1, a);
                    multiplyScalarVector(temp2, generator2, b);
                    multiplyScalarVector(temp3, generator3, c);
                    
                  
                    Plus(su, temp1,temp2, temp3);
                    for (int i=0;i<=6;i++)
                        Plus(pillar[i], Parameterized_building_block_vert[i], su);

                    // adding the calculated edges to graphs
                    graph1.AddLine(pillar[0], pillar[1]);
                    graph1.AddLine(pillar[0], pillar[2]);
                    graph1.AddLine(pillar[0], pillar[3]);
                    graph1.AddLine(pillar[1], pillar[4]);
                    graph1.AddLine(pillar[2], pillar[5]);
                    graph1.AddLine(pillar[3], pillar[6]);


                    // the idea is that the graph2 points are:
                    // pp2 = wall_cordinate - pp1 + su  
                    // -pp1 calculated here:
                    neg_Parameterized_building_block_vert[7][3];
                    for (int i = 0;i <= 6; i++)
                        multiplyScalarVector(neg_Parameterized_building_block_vert[i],
                                             Parameterized_building_block_vert[i],
                                              -1);
    
                    // pp2 is calculated by: pp2 = wall_cordinate - pp1 + su      
                    for (int i = 0;i <= 6; i++)
                        Plus(neg_pillar[i], neg_Parameterized_building_block_vert[i],
                                            wall_coordinates, 
                                            su);
                    
                    graph2.AddLine(neg_pillar[0], neg_pillar[1]);
                    graph2.AddLine(neg_pillar[0], neg_pillar[2]);
                    graph2.AddLine(neg_pillar[0], neg_pillar[3]);
                    graph2.AddLine(neg_pillar[1], neg_pillar[4]);
                    graph2.AddLine(neg_pillar[2], neg_pillar[5]);
                    graph2.AddLine(neg_pillar[3], neg_pillar[6]);

               
           }

}///


// Min an max coordinates are found with the maximum/min x, y, z of building 
// blocks. This functions goes through the building block and find the max/min 
// in regards with x,y,z values. 
void GyroidSkeletal::SetMinMaxCoordinators() {

    double cordx[7];
    double cordy[7];
    double cordz[7];
    
    for (int i = 0; i < 7 ; i++) {
            cordx[i] = Parameterized_building_block_vert[i][0];
            
            cordy[i] = Parameterized_building_block_vert[i][1];
            cordz[i] = Parameterized_building_block_vert[i][2];
        } 
    max_coordinates[0] = Max(cordx, 7);
    max_coordinates[1] = Max(cordy, 7);
    max_coordinates[2] = Max(cordz, 7);

    min_coordinates[0] = Min(cordx, 7);
    min_coordinates[1] = Min(cordy, 7);
    min_coordinates[2] = Min(cordz, 7);


}///



// The second graph (graph2) in grroid strucure is obtained from graph1 and 
// Wall coordinates. This function find s the wall coordinates with the help
// of data members mincoorindates and maxcoordinates.
void GyroidSkeletal::SetWallCoordinators() {

    double wall[3] = {min_coordinates[0] + max_coordinates[0], 
                      min_coordinates[1] + max_coordinates[1],
                      min_coordinates[2] + max_coordinates[2]};
    Assign(wall_coordinates,  wall);
}///


// Mincoordinates help find and intertwine graph2 with graph1
void GyroidSkeletal::GetMinCoordinators(double mincordin[3]){
    Assign(mincordin,min_coordinates);
}///



// Maxcoordinates help find and intertwine  graph2 with graph1
void GyroidSkeletal::GetMaxCoordinators(double maxcordin[3]){
    Assign(maxcordin,max_coordinates);
}///


// Given a point, the diff distance to the graph1 and graph2 are returend.
// An upper level program might call this function to see if the distance 
// from a point to each of graph1 or graph2 are the same or not. If so, that might
// decide that the point is on the surface (Schoen's viewpoint about medial graphs). 
// Since last update, changed entire to delete struct
double GyroidSkeletal::DifferenceDistanceToGraphs(double point[3]){
   
    double dis_to_graph1, dis_to_graph2;
    dis_to_graph1 = embracing_graph1.GetShortestDistanceToGraph(point);
    dis_to_graph2 = embracing_graph2.GetShortestDistanceToGraph(point);
   
    // to avoid 9999 in both graphs that return 0
    // if ( (abs(dis_to_graph1 - 9999.0) < 0.0001) || (abs(dis_to_graph2 - 9999.0) < 0.0001))
    //     {dis_to_graph1 = 999999;return 10000;}

    return abs(dis_to_graph1 - dis_to_graph2); 

}///




// Gives the graph1 as an object.
// You can see and work on this graph's lines and some other features of the 
// class Graph.
Graph GyroidSkeletal::GetBuildBlockGraph1 (){
    return buildblock_graph1;
};///

Graph GyroidSkeletal::GetBuildBlockGraph2() {
    return buildblock_graph2;
};///

Graph GyroidSkeletal::GetNetworkGraph1 () {
    return network_graph1;
};///

Graph GyroidSkeletal::GetNetworkGraph2 (){
    return network_graph2;
};///

Graph GyroidSkeletal::GetEmbracingGraph1 (){
    return embracing_graph1;
};///

Graph GyroidSkeletal::GetEmbracingGraph2 (){
    return embracing_graph2;
};///


// We used to set up generators in public segment of this class. Here, now
// we have given the handle of generators (now in private 
// segment) to a seperate function.
void GyroidSkeletal::GetGenerators(double g1[3], double g2[3], double g3[3]){
    Assign(g1, generator1);
    Assign(g2, generator2);
    Assign(g3, generator3);
};///


//  a public method for to retieve the private building block.
void GyroidSkeletal::GetParameterizedBuildingBlockVerts(double v[7][3]){

 for (int i=0;i<7;i++)
    Assign(v[i], Parameterized_building_block_vert[i]);
};///

//  a public method for to retieve the private parameters X and Theta.
void GyroidSkeletal::GetParameters(double x[6], double theta[5]){
    for (int i=0;i<6;i++)
        x[i] = X[i];
    for (int i=0;i<5;i++)    
        theta[i] = Theta[i];

};///


// min max point in unitcell(though each generatos) 
// and start_point on unitcell are the purpose of this function.
void GyroidSkeletal::FindMinMax_ThroughtGenerators()
{
    min_g[0] = 9999; max_g[0] = -9999;//for generator1
    min_g[1] = 9999; max_g[1] = -9999;//
    min_g[2] = 9999; max_g[2] = -9999;//

    double v[7][3],q[3], g[3][3], temp[3],  temp1[3];;//v ==> parameterized buidling blobkc,  g ==> generators
    GetParameterizedBuildingBlockVerts(v);

    Assign(g[0], generator1);
    Assign(g[1], generator2);
    Assign(g[2], generator3);


    // find the image of every vetiex on building block on g1, g2 and g3 and
    // say if the point is on negetive or positive direction. 
    double length_;
    for (int j=0; j<=2; j++)
    { for (int i=0; i<=6; i++){
            
                ImgPointVector(q, v[i],g[j]);
                length_ = Norm(q); 
                
                // if the angle is 0 the point is in the same direction as g1
                // if not (angl=pi) the poiint is on opp. direction as g1.
                length_ *= -1;
                double angle = Angle(g[j], q);
                
                if (floor(abs(angle))<0.01)
                    length_ *= -1; 

            
                if (length_<min_g[j]) {min_g[j]=length_; Assign(gmin_vec[j],q);}
                else if (length_>max_g[j]) {max_g[j]=length_; Assign(gmax_vec[j],q);}
    

        }//for i
    }   //for j


    // sfinding the start_point was a question that tunes the start point of genrators
    // for making unitcell domain and other further calculation. 
    // In fact, the calculation for generators so far are very standard and start from
    // point {0,0,0} which is the standard point for vectors. btw, the gyroid needed to 
    // have calculation from its own center, i.e. v0. Transforming the attention form
    // {0,0,0} into v0 is the purpose off this lines of code below.   
    double v0[3] = {0.625, 0.625, 0.625}; // v0 equla to ParameterizeBuildingBlock[0];
    Plus(temp, gmin_vec[0],gmin_vec[1],gmin_vec[2]);
    multiplyScalarVector(temp1, temp, 0.5);
    Plus(start_point, v0, temp1);
}///


// This function checks if the given point is on the surface of not.
// It does not now work with normal vector---> only a bool value will be returned.
bool GyroidSkeletal::Surface(double point[3]){
  
    // all the variables are changed 
    bool s;
    double n_direc[3];
    double  dis_to_graph1, dis_to_graph2;

    dis_to_graph1 = embracing_graph1.GetShortestDistanceToGraph(point);
    dis_to_graph2 = embracing_graph2.GetShortestDistanceToGraph(point);
    
    // to avoid 9999 in both graphs that return 0
    if ( abs(dis_to_graph1 - 9999.0) < 0.001 || abs(dis_to_graph2 - 9999.0) < 0.001 ) 
             dis_to_graph1 = 999999;

    if (abs(dis_to_graph1 - dis_to_graph2)<0.001)
            {s = true;
            //+
            // cout<< "equal Dist="<<abs(dis_to_graph1)<<endl;
            }
        else 
            s = false;

        // not needed as a normal vector  
        // Minus(n_direc, dis_to_graph2.projected_point_on_line, point);
        // NormalVector(s.normal_vector, n_direc);

        return s;
}///


// This function checks if the given point is in the positive volume of not.
// Comparing with last verion: change entirely; all the structure changed
// into bool type rather than returning a struct datatype contianing normal vector
// and a bool value
bool GyroidSkeletal::Volume(double point[3]){
    bool vul;

    
    double dis_to_graph1, dis_to_graph2;

    dis_to_graph1 = embracing_graph1.GetShortestDistanceToGraph(point);
    dis_to_graph2 = embracing_graph2.GetShortestDistanceToGraph(point);
    
    // to avoid 9999 in both graphs that return 0
    if ( abs(dis_to_graph1 - 9999.0) < 0.001) 
            dis_to_graph1 = 999999;

    // -
    // for paper writing and results Used dis_to_graph1 > dis_to_graph2
    if (dis_to_graph1 < dis_to_graph2)
    // +
    // cout<<"*";
    // if (dis_to_graph1 < dis_to_graph2 && dis_to_graph1 < 0.1)
        return true;
    return vul; //else
}///


// There is always be a question on how one can say for sure if a line is on surface/volume
// or not--without expanding the hellish time-consuming skeleton building for network and then
// fit the point in that and then say if the point is at the same distance from those graph lines. To make it consice and short, it is rather easy to say if the given point has an image 
// inside the unitcell domain. The unitcell domain is defined by g1, g2 and g3 as generators, 
// and one atomic selection of them are making unitcell domain: 1*g1, 1*g2 and 1*g3. This domain
// forming a box (almost tilted). Now considering a box like this, given a point, one can    
// say with which coefficent of generator this point is positioned through the generator directions. when specfied,  it is easy to find the image of that point in unitcell domain. 
// now one can measure if the imaged point is on surf/vol or not. the answer is returend
// by checking if the imaged point is a surf/vol point in unitcell domain.
bool GyroidSkeletal::IsSurfacePoint(double test_point[3]){
    
    double mapped_point_on_unitcell[3], temp[3], temp1[3]; 
    Minus(temp, test_point, start_point); 
    ImagePointOntoDomain(temp1, temp, 
                            generator1, generator2, generator3);

    Plus(mapped_point_on_unitcell, temp1, start_point);


    if (Surface(mapped_point_on_unitcell))
        return true;
    return false;
}///

// There is always be a question on how one can say for sure if a line is on surface/volume
// or not--without expanding the hellish time-consuming skeleton building for network and then
// fit the point in that and then say if the point is at the same distance from those graph lines. To make it consice and short, it is rather easy to say if the given point has an image 
// inside the unitcell domain. The unitcell domain is defined by g1, g2 and g3 as generators, 
// and one atomic selection of them are making unitcell domain: 1*g1, 1*g2 and 1*g3. This domain
// forming a box (almost tilted). Now considering a box like this, given a point, one can    
// say with which coefficent of generator this point is positioned through the generator directions. when specfied,  it is easy to find the image of that point in unitcell domain. 
// now one can measure if the imaged point is on surf/vol or not. the answer is returend
// by checking if the imaged point is a surf/vol point in unitcell domain.
bool GyroidSkeletal::IsVolumePoint(double test_point[3]){
    
    double mapped_point_on_unitcell[3], temp[3], temp1[3]; 
    Minus(temp, test_point, start_point); 

    // find the mapped point on unitcell domain. unitcellodmain is defined by generators.
    ImagePointOntoDomain(temp1, temp, 
                            generator1, generator2, generator3);
     
    Plus(mapped_point_on_unitcell, temp1, start_point);

    // is the mapped (image) is a volume point?
    if (Volume(mapped_point_on_unitcell))
        return true;
    return false;
}///



// diff_dist() or min_dist() is returning the difference distance 
// from a point to graphs(2 medial graphs)--> it can be -, + or, a 
// really close value to zero. if returns 0 it means that the point is 
// on the surface. If it is -, it is close to the 1st graph,...
// you might notice that this one differs from DifferenceDistanceToGraphs() 
// because that function sometimes  
// this function first added from 23-LTS version for storing traing-data 

double GyroidSkeletal::min_dist(double point[3]){
    

    
    double dis_to_graph1, dis_to_graph2;

    dis_to_graph1 = embracing_graph1.GetShortestDistanceToGraph(point);
    dis_to_graph2 = embracing_graph2.GetShortestDistanceToGraph(point);
    
    // to avoid 9999 in both graphs that return 0
    if ( abs(dis_to_graph1 - 9999.0) < 0.001) 
            dis_to_graph1 = 999999;

    
    return dis_to_graph1 - dis_to_graph2;
    
}///
