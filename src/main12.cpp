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


void ChecksurfvolPoints(GyroidSkeletal skelet, int boundx, int boundy, int boundz, double stepsize){

cout<<"step size:"<<stepsize<<endl;
double test_point[3];

ifstream surf_file; surf_file.open("../app/surf_file.txt");
    int is_surf;
    surf_file>>is_surf;
surf_file.close();

ofstream outdata; outdata.open("../src/demofile.txt");



cout<<"Bound: "<<boundx<<" "<<boundy<<" "<<boundz<<endl;
 if (is_surf==1){
    for (double i=0;i<=boundx; i+= stepsize)
      for (double j=0;j<=boundy; j+=stepsize)
        for (double k=0;k<boundz; k+=stepsize){
            //  if (i*i+j*j>1) continue;
            // cout<< "i,j,k:"<<i<<j<<k<<endl; 
            test_point[0]=i; test_point[1]=j; test_point[2]=k;
            if (skelet.IsSurfacePoint(test_point))
                outdata<<test_point[0]<<" "<<test_point[1]<<" "<<test_point[2]<<endl;
        }//for k
    }//if 
else{
    for (double i=0;i<=boundx; i+= stepsize)
      for (double j=0;j<=boundy; j+=stepsize)
        for (double k=0;k<boundz; k+=stepsize){
            
            // if (i*i+j*j>1) continue;
            // cout<< "i,j,k:"<<i<<j<<k<<endl; 
            test_point[0]=i; test_point[1]=j; test_point[2]=k;
            if (skelet.IsVolumePoint(test_point))
                outdata<<test_point[0]<<" "<<test_point[1]<<" "<<test_point[2]<<endl;
        }//for k
    }//else  



}///



int main(int argc, char** argv) {




double X[6] = {1, 1,1,1,1,1}; // parameters' inital value
double Theta[5] = {2* M_PI/3, 2* M_PI/3, 2* M_PI/3, 2* M_PI/3, 2* M_PI/3};

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
    cout<<endl<<"dimension = "<<dimesion[i-12]<<" ";}

// percsion in python code
double stepsize = 0.01;
for (int i = 15; i < argc && i <= 15; i++)
    {stepsize = atof( argv[i]);
    cout<<endl<<"stepsize = "<<stepsize<<" ";}

cout<<endl<<"==============================="<<endl;
GyroidSkeletal skelet(X, Theta, 1,1,1);  //  Parameterized : non-standard gyroid  

SaveEdgePoints(skelet);
SaveMinMaxCordinates(skelet);
SaveBuildingBlockVertices(skelet);
SaveGenerators(skelet);



ChecksurfvolPoints(skelet,  dimesion[0],dimesion[1],dimesion[2], stepsize);


return 0;
}



