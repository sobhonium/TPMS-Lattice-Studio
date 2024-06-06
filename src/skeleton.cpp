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


void SaveGenerators(GyroidSkeletal skelet){
    double g1[3], g2[3],  g3[3];
    skelet.GetGenerators(g1, g2, g3); 
    
    ofstream generator("../src/generators.txt"); 
    generator<<g1[0]<< " "<<g1[1] <<" " <<g1[2]<<endl;
    generator<<g2[0]<< " "<<g2[1] <<" " <<g2[2]<<endl;
    generator<<g3[0]<< " "<<g3[1] <<" " <<g3[2]<<endl;
    generator.close();
}///

void SaveEdgePoints(GyroidSkeletal sklt){

cout<<"==> Edges Points are computed..."<<endl;
cout<<"==> Edge Lines are computed."<<endl;
    
ofstream outp1; outp1.open("../app/p1.txt");
ofstream outp2; outp2.open("../app/p2.txt");

for(int i=0;i<sklt.GetNetworkGraph1().GetLineCount(); i++) {

    //line start point;
    double s[3];  
    //line end point;
    double e[3];
    sklt.GetNetworkGraph1().GetLineByIndex(i).GetStartPoint(s);
    sklt.GetNetworkGraph1().GetLineByIndex(i).GetEndPoint(e);
    //  double* s={s_point};
    //  double* e={sklt.GetGraph1().GetLineByIndex(i).e_point};
        outp1<<s[0]<<" "<<s[1]<<" "<<s[2]<<endl;
        outp1<<e[0]<<" "<<e[1]<<" "<<e[2]<<endl;


    sklt.GetNetworkGraph2().GetLineByIndex(i).GetStartPoint(s);
    sklt.GetNetworkGraph2().GetLineByIndex(i).GetEndPoint(e);
        outp2<<s[0]<<" "<<s[1]<<" "<<s[2]<<endl;
        outp2<<e[0]<<" "<<e[1]<<" "<<e[2]<<endl;
        
    }

outp1.close();
outp2.close();


cout<<"==> Edge Points are added into p1.txt and p2.txt files."<<endl;
cout<<"==> Construction of skeleton is done."<<endl; 
}///function

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

void SetBound(double & x_low, double &x_high, double &  y_low, double &y_high, double & z_low, double &z_high, GyroidSkeletal skelet){



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

  


x_low = x_low - 1 * abs(x_low)/2;
y_low = y_low - 1 * abs(y_low)/2;
z_low = z_low - 1 * abs(z_low)/2;

x_high = x_high + 1 * abs(x_high)/2;
y_high = y_high + 1 * abs(y_high)/2; 
z_high = z_high + 1 * abs(z_high)/2;

cout<<"========================================="<<endl;
// x_low = 0;
// y_low = 0;
// z_low = 0;

// x_high = 1;
// y_high = 1; 
// z_high = 1;

cout<<"==> Cordinates:: x=["<< x_low<<","<<x_high<<"],  y=["<<y_low<<","<<y_high<<"],   z=["<<z_low<<","<<z_high<<"]"<<endl;    

}//function;

void  FindPoints(double x_low, double x_high,  double y_low, double y_high, double z_low, double z_high, const double stepsize, GyroidSkeletal skelet){
    


ifstream surf_file; surf_file.open("../app/surf_file.txt");
    int is_surf;
    surf_file>>is_surf;
surf_file.close();


cout<<"========================================="<<endl;
cout<<"==> Saving points in ../src/demofile.txt...";

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
                 
                for(int a =0; a <= 0; a++)
                for(int b =0; b <=0; b++)
                for(int c =0; c <=0; c++) {
                 
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
    
       for(int a =0; a <= 0; a++)
        for(int b =0; b <=0; b++)
            for(int c =0;c <= 0; c++) {
                 
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



int main(int argc, char** argv) {




double X[6] = {1,1,1,1,1,1}; // parameters' value
double Theta[5] = {2* M_PI/3, 2* M_PI/3, 2* M_PI/3, 2* M_PI/3, 2* M_PI/3};
int dimesion=1;

cout<<endl<<"X = ";
for (int i = 1; i < argc && i <= 6; i++)
    {X[i-1] = atof( argv[i]);
    cout<<X[i-1]<<", ";}
     
cout<<endl<<"Theta=";
for (int i = 7; i < argc && i <= 11; i++)
    {Theta[i-7] = atof( argv[i]);
    cout<<Theta[i-7]<<", ";}


for (int i = 12; i < argc && i <= 12; i++)
    {dimesion = atof( argv[i]);
    cout<<endl<<"dimension = "<<dimesion<<" ";}

cout<<endl<<"==============================="<<endl;
GyroidSkeletal skelet(X, Theta, dimesion,dimesion, dimesion);  //  Parameterized : non-standard gyroid  
// GyroidSkeletal skelet( 1,1,1);
SaveEdgePoints(skelet);
SaveGenerators(skelet);
// SaveMinMaxCordinates(skelet);
// SaveBuildingBlockVertices(skelet);

// double x_low, x_high, y_low, y_high, z_low, z_high; 

// SetBound(x_low, x_high,  y_low, y_high,  z_low, z_high, skelet);



// to find volume change the content of surf_file.txt to 0
double stepsize = 0.01;
// FindPoints(x_low, x_high,  y_low, y_high,  z_low, z_high, stepsize, skelet);


 


 return 0;
}



