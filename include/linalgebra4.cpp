#include<cmath>
#include<iostream>

#include"linalgebra4.h"

//TODO : exception for DotProductMatrix function 
//       change the 'img' into 'proj'


using namespace std;


// check if a given point is on the domain defined by v1, v2 and v3 vectors 
bool IsPointOnDomain(const double test_point[3], const double  v1[3],  const double  v2[3], const double v3[3], double ignore_sourinding_scale){
    double A_mat[3][3], b_mat[3], inv_A[3][3],  beta[3];
    for (int i=0;i<3; i++)
        {A_mat[i][0] = v1[i]; A_mat[i][1] = v2[i];A_mat[i][2] = v3[i];}

    MatrixInverse(inv_A, A_mat);
    Assign(b_mat, test_point); 

    for (int i=0;i<3; i++){
            double su=0;
            for (int j=0;j<3; j++)
                su += inv_A[i][j]*b_mat[j];
            beta[i]=su;
        }// for beta

    if   (beta[0]+0.000001>=0-ignore_sourinding_scale && beta[0]<=1.0000001                    +ignore_sourinding_scale &&
          beta[1]+0.000001>=0-ignore_sourinding_scale && beta[1]<=1.0000001  +ignore_sourinding_scale && 
          beta[2]+0.000001>=0-ignore_sourinding_scale && beta[2]<=1.0000001+ignore_sourinding_scale)
            return true;

    return false;

};///

// the domain is defined by 3 vectors 
void ImagePointOntoDomain(double img_point[3], double test_point[3], double v1[3], double v2[3], double v3[3]){
    double A_mat[3][3], b_mat[3], inv_A[3][3], tol_img_g[3], beta[3];
    for (int i=0;i<3; i++)
        {A_mat[i][0] = v1[i]; A_mat[i][1] = v2[i];A_mat[i][2] = v3[i];}

    MatrixInverse(inv_A, A_mat);
    Assign(b_mat, test_point); 

    for (int i=0;i<3; i++){
            double su=0;
            for (int j=0;j<3; j++)
                su += inv_A[i][j]*b_mat[j];
            beta[i]=su;
        }// for beta

          for (int i=0;i<3; i++){
            tol_img_g[i] =  FloatMod(beta[i],1); 
            // cout<<tol_img_g[i]<<" ";
        }// for tol_img_g

        for (int i=0;i<3; i++){
           double su=0; //sumation
            for (int j=0;j<3; j++)
                su += A_mat[i][j]*tol_img_g[j];
            img_point[i]=su;
        }// for res_point

};///

double FloatMod(double n1, double n2){
    int div = floor(n1/n2 +0.00000001);
    return n1 - (n2 * div);
}///

double Angle(const double v1[3], const double v2[3]){

// The problem with arccos in cmath is that if arcos(1.000000001) then it returns nan.
// the following tris to solve that.

    double up_frac   = DotProduct(v1, v2);
    double down_frac = Norm(v1)*Norm(v2);
    if (floor (up_frac/down_frac)==1  )
      return acos((up_frac/down_frac)-0.000001);
    else if (floor (up_frac/down_frac)==-2  )
      return acos((up_frac/down_frac)+0.000001);
    else
      return acos((up_frac/down_frac));  

}///

// This operator projects the vector or point 'point' orthogonally onto the line spanned by vector init_v. If init_v = 0, we define proj 0 ⁡ ( v ) := 0 
// proj(point)=<point, v_init>/<v_init, v_init> * v_init 
void ImgPointVector(double reslt_V[3],  const double point[3], const double init_v[3]){


double A[3] ={0,0,0};
double B[3], AB[3], AP[3], temp[3];
Assign(B, init_v);

Minus(AP, point, A);
Minus(AB, B, A);



double multi = DotProduct(AP, AB)/DotProduct(AB, AB);
multiplyScalarVector(temp, AB, multi);
Plus(reslt_V, A, temp);



}///

double EcldDistance(const double a[3], const double b[3]){
    double c[3];
    Minus(c, a, b);
    return Norm(c);

}


double DotProduct(const double a[3], const double b[3]){
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2] ;
}

void CrossProduct(double c[3], const double a[3], const double b[3]) {
   c[0] = a[1] * b[2] - a[2] * b[1];
   c[1] = a[2] * b[0] - a[0] * b[2];
   c[2] = a[0] * b[1] - a[1] * b[0];
}

double Norm(const double a[3]){    
    return sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
}


void NormalVector(double c[3], const double a[3]){

    double n = Norm(a);
    c[0]= a[0]/n;
    c[1]= a[1]/n;
    c[2]= a[2]/n;
     
}

double Determinant(const double a[3][3]){
    
    double det = 0;
    
    for(int i=0;i<3;i++)
        det = det + a[0][i] * (a[1][(i+1)%3] * a[2][(i+2)%3] - a[1][(i+2)%3] * a[2][(i+1)%3]) ;

    return det;
}


// void DotProductMatrix(double c[][100], double  a[][100],int n1, int n2, double b[][100], int m1, int m2){

// // if (n2 != m1)  return 0;// thowing an error
// // double sum  = 0.0;


// for(int i=0;i<n1;i++)
//     for(int k=0;k<m2;k++)
//         c[i][k] = 0;

// for(int i=0;i<n1;i++)
//     for(int k=0;k<m2;k++)
//         { 
//             // sum = 0.0;
//             for(int j=0;j<n2;j++)
//                 c[i][k] = c[i][k] + a[i][j]*b[j][k];
//         }//for

// }

void MatrixTrans(double c[3][3], const double a[3][3]){

    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            c[i][j] = a[j][i];

}

void MatrixInverse( double c[3][3], const double a[3][3]){

double det =  Determinant(a);

for(int i=0;i<3;i++)
    for(int j=0;j<3;j++)
        c[i][j] = (a[(j+1)%3][(i+1)%3] * a[(j+2)%3][(i+2)%3] - a[(j+1)%3][(i+2)%3] * a[(j+2)%3][(i+1)%3])/det;
}


void multiplyScalarMatrix( double c[3][3], const double a[3][3], const double scalar ){
     for(int i=0;i<3; i++)
       for(int j=0;j<3; j++)
            c[i][j] = a[i][j]*scalar;
};

 void Assign(double c[3], const double a[3]){
   // Assign a into c
    c[0] = a[0];
    c[1] = a[1];
    c[2] = a[2];
 }

void printing(const double c[3]){
    // calculates a-b
    cout<<c[0]<<" " <<c[1]<<" " << c[2]<<endl;

}
void Minus(double c[3], const double a[3], const double b[3]){
    // calculates a-b
    c[0] = a[0] - b[0];
    c[1] = a[1] - b[1];
    c[2] = a[2] - b[2];
}

void Minus(double d[3], const double a[3], const double b[3], const double c[3]){
    d[0] = a[0] - b[0] - c[0];
    d[1] = a[1] - b[1] - c[1];;
    d[2] = a[2] - b[2] - c[2];;
}

void Plus(double c[3], const double a[3], const double b[3]){
    // calculates c=a+b
    c[0] = a[0] + b[0];
    c[1] = a[1] + b[1];
    c[2] = a[2] + b[2];
}

void Plus(double d[3], const double a[3], const double b[3], const double c[3]){
// calculates d= a+b+c
    d[0] = c[0] + a[0] + b[0];
    d[1] = c[1] + a[1] + b[1];
    d[2] = c[2] + a[2] + b[2];
}

void multiplyScalarVector(double c[3], const double a[3], const double scalar){
    
    c[0] = a[0]*scalar;
    c[1] = a[1]*scalar;
    c[2] = a[2]*scalar;
}

void MultiplyMatrixMatrix(double c[3][3], const double a[3][3], const double b[3][3]){
    
    for(int i=0;i<3;i++)
        for(int k=0;k<3;k++)
           c[i][k] = 0;

    for(int i=0;i<3;i++)
        for(int k=0;k<3;k++)
        { 
            // sum = 0.0;
            for(int j=0;j<3;j++)
                c[i][k] = c[i][k] + a[i][j]*b[j][k];
        }//for

}



double DistPointToLine(const double P[3],
                       const double A[3], 
                       const double B[3] 
                       ){
    // Calculates the distance of the given cordinates with every line from both
    // channels. If distance from both are equal and the are the same, the point
    // is right on the surface

    // double A[3], B[3], P[3];
    // Assign(A, v1);
    // Assign(B, v2);
    // Assign(P, p);

    // double* A = v1;
    // double* B = v2;
    // double* P = p; 
    
    double AB[3];
    Minus(AB, B, A) ;
      
    double AP[3]; 
    Minus(AP, P, A) ; 
   
   
    double AV[3]; 
    multiplyScalarVector(AV, AB,DotProduct(AP, AB)/DotProduct(AB, AB)); 
   
    double V[3];
    Plus(V, A, AV);
   
    double PV[3];
    Minus(PV, V, P);
    double n_PV = Norm(PV);

    double BV[3];
    Minus(BV, V, B);
    
    // double n_AV =  Norm(AV);
    // double n_BV =  Norm(BV);
   
     
    double residual = Norm(AV) + Norm(BV) - Norm(AB);
    
    return sqrt(n_PV*n_PV+residual*residual);
    // +
    // if (residual < 0.001)
    //     return Norm(PV);
    // else{
    //     double BP[3]; 
    //     Minus(BP, P, B) ; 
    //     double n_BP = Norm(BP);
    //     double n_AP = Norm(AP);
    //     if (n_AP < n_BP)
    //         return n_AP;
    //     else 
    //         return n_BP;       
    // }
    // This line is deleted because normal vecrot is not needed for this version
    // Assign(projected_point_on_the_line, V);
    // if (residual < 0.01) return Norm(PV);
    // return Norm(PV)*99999 ;

    // return Norm(PV) + residual;
}

void CholeskyDecompse(double c[3][3], const double a[3][3]){
    
    double sum;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j <= i; j++) {
             sum = 0;
 
            if (j == i) // summation for diagonals
            {
                for (int k = 0; k < j; k++)
                    sum += pow(c[j][k], 2);
                    c[j][j] = sqrt(a[j][j] - sum);
            } 
            else {
 
                // Evaluating L(i, j) using L(j, j)
                for (int k = 0; k < j; k++)
                    sum += (c[i][k] * c[j][k]);
                    c[i][j] = (a[i][j] - sum) / c[j][j];
            }
        }
    }
}

void Column(double c[3], const double a[3][3], const int col_index){
    for (int i = 0; i < 3; i++)
        c[i] = a[i][col_index];  
}


void Row(double c[3], const double a[3][3], const int row_index){
    for (int i = 0; i < 3; i++)
        c[i] = a[row_index][i];  
}

double  Min(const double a[], const int count){
    int i = 0;
    double minum = 999999999;
    for (int i=0; i<count; i++)
        if (a[i]<minum)
            minum = a[i];
    
    return minum;
}

double  Max(const double a[], const int count){
    int i = 0;
    double maxim = -999999999;
    for (int i=0; i<count; i++)
        if (a[i] > maxim)
            maxim = a[i];
    
    return maxim;
}