/*

In this version we added 'const' for unwanted changes in function prototypes.
In benchmarking this simple functions with Eigen, we realized that for small
sized matrices ....
*/


#ifndef  LINEARALGEBRA_H_
#define  LINEARALGEBRA_H_


// TODO(Sobhan): guarding headerfiles by #ifndef

/* + */ 
bool IsPointOnDomain(const double test_point[3], const double  v1[3],  const double  v2[3], const double v3[3], double);

void ImagePointOntoDomain(double img_point[3], double test_point[3], double v1[3], double v2[3], double v3[3]);
/* + */
double Angle(const double v1[3], const double v2[3]);
double FloatMod(double, double );
void ImgPointVector(double reslt_V[3], const double point[3], const double v[3]);
double EcldDistance(const double a[3], const double b[3]);
double DotProduct(const double a[3], const double b[3]);
void CrossProduct(double c[3], const double a[3], const double b[3]);
double Norm(const double a[3]);
void NormalVector(double c[3], const double a[3]);
double Determinant(const double a[3][3]);
void MatrixTrans(double c[3][3], const double a[3][3]);
void MatrixInverse( double c[3][3], const double a[3][3]);
void multiplyScalarMatrix( double c[3][3], const double a[3][3], const double scalar );
// void DotProductMatrix(double c[][100], double  a[][100],int n1, int n2, double b[][100], int m1, int m2);
void Assign(double c[3], const double a[3]);
void printing(const double c[3]);
void Minus(double c[3], const double a[3], const double b[3]);
void Minus(double d[3], const double a[3], const double b[3], const double c[3]);
void Plus(double c[3], const double a[3], const double b[3]);
void Plus(double d[3], const double a[3], const double b[3], const double c[3]);
void multiplyScalarVector(double c[3], const double a[3], const double scalar);
void MultiplyMatrixMatrix(double c[3][3], const double a[3][3], const double b[3][3]);
//changed in this version to delete normal vector 
double DistPointToLine(const double p[3], const double v1[3], const double v2[3]);
void CholeskyDecompse(double c[3][3], const double a[3][3]);
void Column(double c[3], const double a[3][3], const int col_index);
void Row(double c[3], const double a[3][3], const int row_index);
double  Min(const double a[], const int count);
double  Max(const double a[], const int count);


#endif  // LIEARALGEBRA_H_




