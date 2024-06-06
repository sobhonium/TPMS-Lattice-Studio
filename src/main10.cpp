#include<iostream>
#include<fstream>

// to be deleted
#include"../include/TPMSLattice.h"
#include"../include/linalgebra4.h"
#include<cmath>


// TODO(Sobhan): 
//              



using namespace std;

// FIXIT(SBN): These functions coupled more than enough with main function or other functions


void SaveEdgePoints(GyroidSkeletal sklt){

cout<<"==> Edges Points are computed..."<<endl;
cout<<"==> Edge Lines are computed."<<endl;
    
ofstream outp1; outp1.open("../app/p1.txt");
ofstream outp2; outp2.open("../app/p2.txt");

for(int i=0;i<sklt.GetBuildBlockGraph1().GetLineCount(); i++) {

    //line start point;
    double s[3];  
    //line end point;
    double e[3];
    sklt.GetBuildBlockGraph1().GetLineByIndex(i).GetStartPoint(s);
    sklt.GetBuildBlockGraph1().GetLineByIndex(i).GetEndPoint(e);
    //  double* s={s_point};
    //  double* e={sklt.GetGraph1().GetLineByIndex(i).e_point};
        outp1<<s[0]<<" "<<s[1]<<" "<<s[2]<<endl;
        outp1<<e[0]<<" "<<e[1]<<" "<<e[2]<<endl;


    sklt.GetBuildBlockGraph2().GetLineByIndex(i).GetStartPoint(s);
    sklt.GetBuildBlockGraph2().GetLineByIndex(i).GetEndPoint(e);
        outp2<<s[0]<<" "<<s[1]<<" "<<s[2]<<endl;
        outp2<<e[0]<<" "<<e[1]<<" "<<e[2]<<endl;
        
    }

outp1.close();
outp2.close();


cout<<"==> Edge Points are added into ../app/p1.txt and ../app/p2.txt files."<<endl;
cout<<"==> Construction of skeleton is done."<<endl; 
}//function
void SaveMinMaxCordinates(GyroidSkeletal skelet){

double mincordin[3], maxcordin[3];
skelet.GetMinCoordinators(mincordin);
skelet.GetMaxCoordinators(maxcordin);

cout<<"========================================="<<endl;
cout<<"==> Original Min coordinates =["<<mincordin[0]<< ","<<mincordin[1] <<"," <<mincordin[2]<<"]"<<endl;
cout<<"==> Original Max coordinates =["<<maxcordin[0]<< ","<<maxcordin[1] << ","<<maxcordin[2]<<"]"<<endl;

// writing coordinates inside a file
ofstream coordinat("../src/MinMaxCoordinates.txt"); 
    coordinat<<mincordin[0]<< " "<<mincordin[1] <<" " <<mincordin[2]<<endl;
    coordinat<<maxcordin[0]<< " "<<maxcordin[1] << " "<<maxcordin[2]<<endl;
coordinat.close();
}///function 

void SaveGenerators(GyroidSkeletal skelet){
    double g1[3], g2[3],  g3[3];
    skelet.GetGenerators(g1, g2, g3); 
    
    ofstream generator("../src/generators.txt"); 
    generator<<g1[0]<< " "<<g1[1] <<" " <<g1[2]<<endl;
    generator<<g2[0]<< " "<<g2[1] <<" " <<g2[2]<<endl;
    generator<<g3[0]<< " "<<g3[1] <<" " <<g3[2]<<endl;
    generator.close();
}
void SaveBuildingBlockVertices(GyroidSkeletal skelet){

// ofstream building_block_verts_file_ch1("building_block_verts_file_ch1.txt");
// ofstream building_block_verts_file_ch2("building_block_verts_file_ch2.txt");
// for (int i =0; i<7; i++) {
//     for (int j=0 ; j<3; j++) {
//         building_block_verts_file_ch1<<gg.Parameterized_building_block_vert[i][j]<<" ";
//         building_block_verts_file_ch2<<gg.neg_Parameterized_building_block_vert[i][j]+gg.wall_coordinates[j]<<" ";
        
//     }
//     building_block_verts_file_ch1<<endl;
//     building_block_verts_file_ch2<<endl;
// }

// building_block_verts_file_ch1.close();
// building_block_verts_file_ch2.close();

}//function

void SetBound(double & x_low, double &x_high, double &  y_low, double &y_high, double & z_low, double &z_high, GyroidSkeletal skelet, int dim){


// as in frontend the regualr unitcell is starts with dim=1 and the first exapsion is dim=2 
// here we use dim-1 to have a cositency with the purpose. In c++ backend we use dim=0 for 
// regular unitcell size...
// dim = dim-1;


ifstream manual; manual.open("../src/manual.txt");
    int is_manual_corin;
    manual>>is_manual_corin;//0 or 1
manual.close();

double mincordin[3], maxcordin[3];
skelet.GetMinCoordinators(mincordin);
skelet.GetMaxCoordinators(maxcordin);

 x_low=mincordin[0];
 x_high=maxcordin[0];

 y_low=mincordin[1];
 y_high=maxcordin[1];

 z_low=mincordin[2];
 z_high=maxcordin[2];


if (is_manual_corin == 1){
    ifstream manual_cordin; manual_cordin.open("../src/manualcordin.txt");
        manual_cordin>>x_low;
        manual_cordin>>x_high;
        manual_cordin>>y_low;
        manual_cordin>>y_high;
        manual_cordin>>z_low;
        manual_cordin>>z_high;   
    manual_cordin.close();
}

  

if (dim>0){
x_low = x_low - 1 * abs(x_low)/2;
y_low = y_low - 1 * abs(y_low)/2;
z_low = z_low - 1 * abs(z_low)/2;

x_high = x_high + 1 * abs(x_high)/2;
y_high = y_high + 1 * abs(y_high)/2; 
z_high = z_high + 1 * abs(z_high)/2;
}
cout<<"========================================="<<endl;
// x_low = 0;
// y_low = 0;
// z_low = 0;

// x_high = 1;
// y_high = 1; 
// z_high = 1;

cout<<"==> Cordinates:: x=["<< x_low<<","<<x_high<<"],  y=["<<y_low<<","<<y_high<<"],   z=["<<z_low<<","<<z_high<<"]"<<endl;    

}//function;

void  FindPoints(double x_low, double x_high,  double y_low, double y_high, double z_low, double z_high, const double stepsize, GyroidSkeletal skelet, int dim){
    


ifstream surf_file; surf_file.open("../app/surf_file.txt");
    int is_surf;
    surf_file>>is_surf;
surf_file.close();


cout<<"========================================="<<endl;
cout<<"==> Saving points in demofile.txt....";

// ofstream expansion("expandsion.txt"); 
    //expansion<<"";    
    ofstream outdata; outdata.open("../src/demofile.txt");

double point[3], po[3];
double temp1[3], temp2[3], temp3[3];
double su[3];

double g1[3], g2[3], g3[3];
skelet.GetGenerators(g1, g2, g3);


 

if (is_surf == 1)
    for(double x=x_low; x<x_high; x+=stepsize)
    for(double y=y_low; y<y_high; y+=stepsize)
    for(double z=z_low; z<z_high; z+=stepsize) {           

        point[0] = x;
        point[1] = y;
        point[2] = z;
        // cout<<"*";
        //  cout<<endl<<point[0]<<" "<<point[1]<<" "<<point[2]<<" ";
        bool s = skelet.Surface(point);        
        // if point is on the surface:
        if(s) { 
                
                // cout<<point[0]<<" "<<point[1]<<" "<<point[2]<<endl;
                 
       for(int a =0; a <= dim-1; a++)
        for(int b =0; b <=dim-1; b++)
            for(int c =0;c <= dim-1; c++) {
                 
                    multiplyScalarVector(temp1, g1, a);
                    multiplyScalarVector(temp2, g2, b);
                    multiplyScalarVector(temp3, g3, c);
                       
                    Plus(su, temp1,temp2, temp3);
                    Plus(po, su, point);
                    // outdata<<point[0]<<" "<<point[1]<<" "<<point[2]<<" "<<s.normal_vector[0]<<" "<<s.normal_vector[1]<<" "<<s.normal_vector[2]<<endl;
                   
                    outdata<<po[0]<<" "<<po[1]<<" "<<po[2]<<endl;
                    //outdata<<x<<" " <<y<<"  "<<z<<" "<< s.normal_vector[0]<<" "<<s.normal_vector[1]<<" "<<s.normal_vector[2]<<endl;
            }//for c
    }
 
}//for
else // is_surf != 1
    for(double x=x_low; x<x_high; x+=stepsize*3)
    for(double y=y_low; y<y_high; y+=stepsize*3)
    for(double z=z_low; z<z_high; z+=stepsize*3) {           
    
    point[0] = x;
    point[1] = y;
    point[2] = z;
    //cout<<"*";
 
    bool vol = skelet.Volume(point);        
    if(vol)
  { 
    
       for(int a =0; a <= dim-1; a++)
        for(int b =0; b <=dim-1; b++)
            for(int c =0;c <= dim-1; c++) {
                 
    //              point[0] = x;
    // point[1] = y;
    // point[2] = z;
                    multiplyScalarVector(temp1, g1, a);
                    multiplyScalarVector(temp2, g2, b);
                    multiplyScalarVector(temp3, g3, c);
                    
                  
                    Plus(su, temp1,temp2, temp3);
                    Plus(po, su, point);
                    // outdata<<point[0]<<" "<<point[1]<<" "<<point[2]<<" "<<s.normal_vector[0]<<" "<<s.normal_vector[1]<<" "<<s.normal_vector[2]<<endl;
                      outdata<<po[0]<<" "<<po[1]<<" "<<po[2]<<endl;
        //outdata<<x<<" " <<y<<"  "<<z<<" "<< s.normal_vector[0]<<" "<<s.normal_vector[1]<<" "<<s.normal_vector[2]<<endl;
            }//for c
    }
 

 
 
}//for
cout<<"(done)."<<endl<<"==> See ../src/demofile.txt"<<endl;
};//fucntion


void FindMinMax_ThroughtGenerators(GyroidSkeletal skelet, double max_g[3], double min_g[3], double gmax_vec[3][3], double gmin_vec[3][3])
{

min_g[0] = 9999; max_g[0] = -9999;//g1
min_g[1] = 9999; max_g[1] = -9999;//g2
min_g[2] = 9999; max_g[2] = -9999;//g3

double v[7][3],q[3], g[3][3], temp[3];//v ==> parameterized buidling blobkc,  g ==> generators
skelet.GetParameterizedBuildingBlockVerts(v);
skelet.GetGenerators(g[0],g[1], g[2]);


double andaz;
for (int j=0; j<=2; j++)
   { for (int i=0; i<=6; i++){
           
            // cout<<"---------------------"<<endl;
            // cout<<"i="<<i<<endl;
            ImgPointVector(q, v[i],g[j]);
            andaz = Norm(q); 
            // cout<<"q:"<<q[0]<<" "<<q[1]<<" "<<q[2]<<" "<<endl;
            andaz *= -1;
            double angle = Angle(g[j], q);
            // cout<<"angle="<<angle<<endl;
            if (floor(abs(angle))<0.01)
                andaz *= -1; 

            // if (i==0) { 
            //             min_g[j]=andaz;
            //             max_g[j]=andaz;}
            // // the following implements: if (angle == 0)
              
            // else{
                // cout<<"andaz="<<andaz<<endl; 
            if (andaz<min_g[j]) {min_g[j]=andaz; Assign(gmin_vec[j],q);cout<<" upate min: "<<angle<<"  "<<andaz<<endl;}
            else if (andaz>max_g[j]) {max_g[j]=andaz; Assign(gmax_vec[j],q);cout<<" upate max: "<<angle<<" " <<andaz<<endl;}
            // }
        //    if(Angle(g[j], q) < 0.01){cout<<"0 happened!"<<endl;}
        //    cout<<j<<","<<i <<", q: ["<<q[0]<<", "<<q[1]<<", "<<q[2]<<"] "<<endl;
        //    cout<<"------------"<<endl;
        //   q := img(v[i], gvec[j])

    }//for i
cout<<"minmax nahaie:"<<min_g[j]<<" "<<max_g[j]<<endl;
   }   //for j
// cout<<"max:"<<endl;        
// for (int j=0;j<=2;j++)
//     // cout<<gmax_vec[j][0]<<" "<<gmax_vec[j][1]<<" "<<gmax_vec[j][2]<<" "<<endl;
//     cout<<max_g[j]<<" ";
// for (int j=0;j<=6;j++)
// cout<<v[j][0]<<" "<<v[j][1]<<" "<<v[j][2]<<" "<<endl;
}///


void FindBestPoints(GyroidSkeletal skelet,  double points[][3], int &point_count, const double gmax_vec[3][3], const double gmin_vec[3][3], double stepsize){

point_count = 0;
double v0[3] = {0.625, 0.625,0.625}, temp[3], temp1[3], temp2[3], temp3[3];; 
Plus(temp, gmin_vec[0],gmin_vec[1],gmin_vec[2]);
multiplyScalarVector(temp1, temp, 0.5);

double max_cord[3];
double min_cord[3];
skelet.GetMaxCoordinators(max_cord);
skelet.GetMinCoordinators(min_cord);

// cout<<"mincord:"<<min_cord[0]<<" "<<min_cord[1]<<" "<<min_cord[2]<<" "<<endl;
// cout<<"maxcord:"<<max_cord[0]<<" "<<max_cord[1]<<" "<<max_cord[2]<<" "<<endl;
double g[3][3], s[3];
skelet.GetGenerators(g[0],g[1], g[2]);
Plus(s, g[0], g[1], g[2]);

double chiral_maxmin[3];
for (int i=0;i<3;i++)
    chiral_maxmin[i] = max_cord[i]+min_cord[i];

// cout<<"chiral_maxmin:"<<chiral_maxmin[0]<<" "<<chiral_maxmin[1]<<" "<<chiral_maxmin[2]<<" "<<endl;
double start_point[3];
Plus(start_point, v0, temp1);

int tah = int(floor(1.0/stepsize)); int aval = 0;




for(int i=aval;i<=tah;i++)
for(int j=aval;j<=tah;j++)
for(int k=aval;k<=tah;k++)
{
    Minus(temp, gmax_vec[0], gmin_vec[0]);
    multiplyScalarVector(temp1, temp, i*stepsize);

    Minus(temp, gmax_vec[1], gmin_vec[1]);
    multiplyScalarVector(temp2, temp, j*stepsize);


    Minus(temp, gmax_vec[2], gmin_vec[2]);
    multiplyScalarVector(temp3, temp, k*stepsize);


    Plus(temp,  temp1, temp2, temp3);
    Plus(points[point_count],  start_point, temp);


    Minus(temp, chiral_maxmin , points[point_count]);
    point_count++;
    // Plus(points[point_count], temp, s);
    Assign(points[point_count], temp);
    point_count++;
} //for k

};///

void FindSurfPoint(double points[][3], int point_count,  GyroidSkeletal skelet2 ){

    ofstream outdata; outdata.open("../src/demofile.txt");
    for (int i=0;i<point_count;i++) 
       if (skelet2.Surface(points[i]))
           outdata<<points[i][0]<<" "<<points[i][1]<<" "<<points[i][2]<<endl;
}///

// double points[10000000][3];// using fixed memory

void FitPoints(GyroidSkeletal skelet, const double gmax_vec[3][3], const double gmin_vec[3][3], double stepsize, int dim1, int dim2, int dim3){

//  dim1=3; dim2=3; dim3=3;
ofstream outdata; outdata.open("../src/demofile.txt");

double Xx[6], Ttheta[5];
skelet.GetParameters(Xx,Ttheta);
GyroidSkeletal skelet2(Xx,Ttheta, 2,2,2);

int point_count = 0;
double v0[3] = {0.625, 0.625,0.625}, temp[3], temp1[3], temp2[3], temp3[3];; 
Plus(temp, gmin_vec[0],gmin_vec[1],gmin_vec[2]);
multiplyScalarVector(temp1, temp, 0.5);

double max_cord[3];
double min_cord[3];
skelet.GetMaxCoordinators(max_cord);
skelet.GetMinCoordinators(min_cord);

// cout<<"mincord:"<<min_cord[0]<<" "<<min_cord[1]<<" "<<min_cord[2]<<" "<<endl;
// cout<<"maxcord:"<<max_cord[0]<<" "<<max_cord[1]<<" "<<max_cord[2]<<" "<<endl;
double g[3][3], su[3], point[3], neg_point[3];
skelet.GetGenerators(g[0],g[1], g[2]);
Plus(su, g[0], g[1], g[2]);

double chiral_maxmin[3];
for (int i=0;i<3;i++)
    chiral_maxmin[i] = max_cord[i] + min_cord[i];

// cout<<"chiral_maxmin:"<<chiral_maxmin[0]<<" "<<chiral_maxmin[1]<<" "<<chiral_maxmin[2]<<" "<<endl;
double start_point[3];
Plus(start_point, v0, temp1);

int tah = int(floor(1.0/stepsize))-1; int aval = 0;


ifstream surf_file; surf_file.open("../app/surf_file.txt");
int is_surf;
surf_file>>is_surf;
cout<<"The status of surf_file.txt : "<<is_surf<<endl;
surf_file.close();


double po[3];

for(int i=aval;i<=tah;i++)
for(int j=aval;j<=tah;j++)
for(int k=aval;k<=tah;k++){

    multiplyScalarVector(temp1, g[0], i*stepsize);
    multiplyScalarVector(temp2, g[1], j*stepsize);
    multiplyScalarVector(temp3, g[2], k*stepsize);

    Plus(temp,  temp1, temp2, temp3);
    Plus(point,  start_point, temp);

    if (is_surf==1){
       if (skelet2.Surface(point)){
                for (int a=0;a<=dim1;a++)
                for (int b=0;b<=dim2;b++)
                for (int c=0;c<=dim3;c++){
                    //  if (a==0 && b==0 && c==0 ) continue;
                    multiplyScalarVector(temp1, g[0], a);
                    multiplyScalarVector(temp2, g[1], b);
                    multiplyScalarVector(temp3, g[2], c);

                    Plus(su, temp1,temp2, temp3);
                    Plus(po, point, su);
                    
                    outdata<<po[0]<<" "<<po[1]<<" "<<po[2]<<endl;
                }//for c
        point_count += 1;
        }//if
    }//if
    else if (is_surf==0)
            {
                // cout<<"******* ";
                if (skelet2.Volume(point)){
                for (int a=0;a<=dim1;a++)
                for (int b=0;b<=dim2;b++)
                for (int c=0;c<=dim3;c++){
                    //  if (a==0 && b==0 && c==0 ) continue;
                    multiplyScalarVector(temp1, g[0], a);
                    multiplyScalarVector(temp2, g[1], b);
                    multiplyScalarVector(temp3, g[2], c);

                    Plus(su, temp1,temp2, temp3);
                    Plus(po, point, su);
                    
                    outdata<<po[0]<<" "<<po[1]<<" "<<po[2]<<endl;
                }//for c
            point_count += 1;
            }//if
        }//else if

        
    } //for k



}///



int main(int argc, char** argv) {




double X[6] = {1, 4, 1, 3, 2, 2}; // parameters' value
double Theta[5] = {2* M_PI/4, 2* M_PI/4, 2* M_PI/2, 2* M_PI/4, 2* M_PI/4};

cout<<endl<<"X = ";
for (int i = 1; i < argc && i <= 6; i++)
    {X[i-1] = atof( argv[i]);
    cout<<X[i-1]<<", ";}
     
cout<<endl<<"Theta=";
for (int i = 7; i < argc && i <= 11; i++)
    {Theta[i-7] = atof( argv[i]);
    cout<<Theta[i-7]<<", ";}

int dimesion[3] = {1,1,1};
for (int i = 12; i < argc && i <= 14; i++)
    {dimesion[i-12] = atof( argv[i]);
    cout<<endl<<"dimension = "<<dimesion[i]<<" ";}

// percsion in python code
double stepsize = 0.01;
for (int i = 15; i < argc && i <= 15; i++)
    {stepsize = atof( argv[i]);
    cout<<endl<<"stepsize = "<<stepsize<<" ";}

cout<<endl<<"==============================="<<endl;
GyroidSkeletal skelet(X, Theta, 1,1,1);  //  Parameterized : non-standard gyroid  
// GyroidSkeletal skelet( 1,1,1);
SaveEdgePoints(skelet);
SaveMinMaxCordinates(skelet);
SaveBuildingBlockVertices(skelet);
SaveGenerators(skelet);



double max_g[3], min_g[3];
double gmax_vec[3][3], gmin_vec[3][3];
FindMinMax_ThroughtGenerators(skelet, max_g, min_g, gmax_vec, gmin_vec);



int point_count;
int dim1=dimesion[0], dim2=dimesion[1] , dim3=dimesion[2];
FitPoints(skelet, gmax_vec, gmin_vec, stepsize, dim1-1, dim2-1, dim3-1);
//  FindBestPoints(skelet, points, point_count, gmax_vec, gmin_vec, stepsize);

// cout<<point_count;

// GyroidSkeletal skelet2(X,Theta, 2,2,2);
// FindSurfPoint(points, point_count, skelet2);

// for (int i=0;i<point_count;i++)
// cout<<points[i][0]<<" "<<points[i][1]<<" "<<points[i][2]<<" "<<endl;



// double a1[3]= {1,2,3}, b1[3]={10,2,5}, ve[3];
// ImgPointVector(ve, a1, b1);
// cout<<endl<<"----->"<<ve[0]<<" "<<ve[1]<<" "<<ve[2]<<endl;

// cout<<"acos="<<Angle(a1, b1)<<endl;
// double x_low, x_high, y_low, y_high, z_low, z_high; 

// double g1[3], g2[3], g3[3];
// skelet.GetGenerators(g1, g2, g3);




// SetBound(x_low, x_high,  y_low, y_high,  z_low, z_high, skelet, dimesion);



// FindPoints(x_low, x_high,  y_low, y_high,  z_low, z_high, stepsize, skelet, dimesion);


 


return 0;
}



