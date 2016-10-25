#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct Atom 
{
  float x; float y; float z;
} atom;


#include "cylinderPolakRib.c"
#define PI 3.1415927

/*3.1415926535897932384626433832 */



/* Function that calculates the euclidean norm of a vector, represented by atom A. */
float norm2(atom A)
{
  return sqrt(pow(A.x,2)+pow(A.y,2)+pow(A.z,2));
} 
/* Function that calculates the Cross Product between the vectors represented 
by the atoms A1 and A2 and stores the result in A3.*/
void crossproduct(atom A1,atom A2,atom *A3)
{
  (*A3).x = A1.y*A2.z - A1.z*A2.y;
  (*A3).y = A1.z*A2.x - A1.x*A2.z;
  (*A3).z = A1.x*A2.y - A1.y*A2.x;
}

/* Function that sums two Atoms */
void atom_sum(atom A1,atom A2, atom *A3)
{
  (*A3).x = A1.x + A2.x;
  (*A3).y = A1.y + A2.y;
  (*A3).z = A1.z + A2.z; 
}
/* Function that subtracts two Atoms */
void atom_subtract(atom A1,atom A2, atom *A3)
{
  (*A3).x = A1.x - A2.x;
  (*A3).y = A1.y - A2.y;
  (*A3).z = A1.z - A2.z; 
}
/* Function that computes and returns the scalar product between two vectors, represented   */
/* by the atoms A1 and A2                                                                  */
float atom_dotproduct(atom A1,atom A2)
{
   return A1.x*A2.x+A1.y*A2.y+A1.z*A2.z;
}
/* Function that does a scalar multiplication of an Atoms and a float */
void atom_scalarproduct(atom A1,float multiplier, atom *A3)
{
  (*A3).x = A1.x * multiplier ;
  (*A3).y = A1.y * multiplier ;
  (*A3).z = A1.z * multiplier ;
}
/* function to calculate the vectors from the axis, for use with rmsd and 
gradients, and also the  */
void CalculateVectorsFromAxis(atom atm_pointR, atom atm_vectorA, 
      float *dbl_AxisDistances, atom *atm_VectorsFromAxis, atom *atm_Helix, 
      float *dbl_meanD, int int_NumberOfAtoms){
    // take in pointers to an array of points, axis distances, vectors to axis 
    int i;
    float dbl_XdotA;      
    atom atm_XminusR, atm_XdotAstarA;  
    
    // calculate the perpendicular vectors to each atom
    for (i=0; i<int_NumberOfAtoms;i++){
        // X.A 
        dbl_XdotA = atom_dotproduct(atm_Helix[i],atm_vectorA);
        // (X.A)*A
        atom_scalarproduct(atm_vectorA,dbl_XdotA,&atm_XdotAstarA);
        // X-R
        atom_subtract(atm_Helix[i],atm_pointR, &atm_XminusR);
        // X-R-(X.A)*A, which is the distance from the axis ?
        atom_subtract(atm_XminusR,atm_XdotAstarA, &atm_VectorsFromAxis[i]);
        
       // printf(" %f %f %f\n", atm_VectorsFromAxis[i].x, atm_VectorsFromAxis[i].y, atm_VectorsFromAxis[i].z); 
    }   
    
    //work out the vector distances, and calculate the mean
    *dbl_meanD = 0.0;
    for (i=0; i<int_NumberOfAtoms; i++){
        dbl_AxisDistances[i] = norm2(atm_VectorsFromAxis[i]);
        *dbl_meanD = *dbl_meanD + dbl_AxisDistances[i] / int_NumberOfAtoms;
        //printf("%f\n ", dbl_AxisDistances[i]); 
    }
}
void NormalizeZetas(float *zetas)
    {
    //make atm_pointR and atm_vectorA
    atom atm_pointR, atm_AstarLdotA,  atm_vectorA;      
    float dbl_lenA;
    // transfer from zetas to point R and vector A
    atm_pointR.x  = zetas[1]; 
    atm_pointR.y  = zetas[2]; 
    atm_pointR.z  = zetas[3]; 
    atm_vectorA.x = zetas[4]; 
    atm_vectorA.y = zetas[5]; 
    atm_vectorA.z = zetas[6];
    
    //printf("\n     zetas = %f %f %f %f %f %f", zetas[1], zetas[2], zetas[3], zetas[4], zetas[5], zetas[6]);
    
    /* normalise A, and set R such that it is perpendicular to A */
    dbl_lenA = 1/norm2(atm_vectorA);
    atom_scalarproduct(atm_vectorA, dbl_lenA, &atm_vectorA);
    atom_scalarproduct(atm_vectorA,atom_dotproduct(atm_vectorA,atm_pointR),&atm_AstarLdotA);
    atom_subtract(atm_pointR, atm_AstarLdotA, &atm_pointR);
    
    //redefine zetas
    zetas[1] = atm_pointR.x;
    zetas[2] = atm_pointR.y;
    zetas[3] = atm_pointR.z;
    zetas[4] = atm_vectorA.x;
    zetas[5] = atm_vectorA.y;
    zetas[6] = atm_vectorA.z; 
    
}


  /* start of a cylinderrmsd func */
  /*work out vected ‘int’ but argument is of type ‘struct atom *’
cylinder.c:232:5: warning: passing argument 3 of ‘gradients’ makes pointer from integer without a cast
cylinder.c:211:10: note: expected ‘float *’ but argument is of type ‘int’
cylinder.c:233:12: error: expected expression before ‘float’
tors from the cylinder axis*/
float cylinder_rmsd(float *zetas, atom *atm_Helix, int int_NumberOfAtoms)
    {
    int i;
    float dbl_AxisDistances[int_NumberOfAtoms], dbl_meanD = 0.0, dbl_rmsd; 
    atom atm_VectorsFromAxis[int_NumberOfAtoms], atm_pointR,  atm_vectorA;      
    
    /* normalise A, and set R such that it is perpendicular to A */
    NormalizeZetas(zetas);

    // transfer from zetas to point R and vector A
    atm_pointR.x  = zetas[1]; 
    atm_pointR.y  = zetas[2]; 
    atm_pointR.z  = zetas[3]; 
    atm_vectorA.x = zetas[4]; 
    atm_vectorA.y = zetas[5]; 
    atm_vectorA.z = zetas[6];
    //printf("%f %f %f %f %f %f \n", atm_pointR.x, atm_pointR.y, atm_pointR.z, atm_vectorA.x, atm_vectorA.y, atm_vectorA.z);

    //printf("%f %f %f %f %f %f \n", atm_pointR.x, atm_pointR.y, atm_pointR.z, atm_vectorA.x, atm_vectorA.y, atm_vectorA.z);
 
    CalculateVectorsFromAxis(atm_pointR, atm_vectorA, &dbl_AxisDistances[0], 
                              &atm_VectorsFromAxis[0], atm_Helix, &dbl_meanD,   
                              int_NumberOfAtoms);
    //printf("\n%f \n ", dbl_meanD);
    /*find the rmsd*/
    dbl_rmsd = 0.0;
    for (i=0; i<int_NumberOfAtoms; i++){
        dbl_rmsd = dbl_rmsd + pow((dbl_AxisDistances[i] - dbl_meanD),2);
        //printf("%f ", dbl_rmsd);
    }
    //printf("\n");
    zetas[1] = atm_pointR.x;
    zetas[2] = atm_pointR.y;
    zetas[3] = atm_pointR.z;
    zetas[4] = atm_vectorA.x;
    zetas[5] = atm_vectorA.y;
    zetas[6] = atm_vectorA.z;
    //printf("rmsd = %f", dbl_rmsd);
    return dbl_rmsd;
} 
    // want to calculate the cylinder radius
float cylinder_radius(float *zetas, atom *atm_Helix, int int_NumberOfAtoms)
    {
    int i;
    float dbl_AxisDistances[int_NumberOfAtoms], dbl_meanD = 0.0, dbl_rmsd; 
    atom atm_VectorsFromAxis[int_NumberOfAtoms], atm_pointR,  atm_vectorA;      
    
    /* normalise A, and set R such that it is perpendicular to A */
    NormalizeZetas(zetas);

    // transfer from zetas to point R and vector A
    atm_pointR.x  = zetas[1]; 
    atm_pointR.y  = zetas[2]; 
    atm_pointR.z  = zetas[3]; 
    atm_vectorA.x = zetas[4]; 
    atm_vectorA.y = zetas[5]; 
    atm_vectorA.z = zetas[6];
    //printf("%f %f %f %f %f %f \n", atm_pointR.x, atm_pointR.y, atm_pointR.z, atm_vectorA.x, atm_vectorA.y, atm_vectorA.z);

    //printf("%f %f %f %f %f %f \n", atm_pointR.x, atm_pointR.y, atm_pointR.z, atm_vectorA.x, atm_vectorA.y, atm_vectorA.z);
 
    CalculateVectorsFromAxis(atm_pointR, atm_vectorA, &dbl_AxisDistances[0], 
                              &atm_VectorsFromAxis[0], atm_Helix, &dbl_meanD,   
                              int_NumberOfAtoms);
    //printf("\n%f \n ", dbl_meanD);
    // just return the mean radius 
    return dbl_meanD;
}  



   //calculate the gradients 
void gradients(float *zetas, float *dbl_gradients, atom *atm_Helix, int int_NumberOfAtoms){
    //first the gradients of each delta
    int i, j;
    float dbl_AxisDistances[int_NumberOfAtoms], dbl_meanD = 0.0, 
            dbl_dbyzeta[6][int_NumberOfAtoms]; 
    float dbl_XdotA;
    float dbl_meanDByZeta[6] = {0,0,0,0,0,0};
    atom atm_VectorsFromAxis[int_NumberOfAtoms], atm_pointR, atm_vectorA;      

    //printf("\n bef zetas = %f %f %f %f %f %f", zetas[1], zetas[2], zetas[3], zetas[4], zetas[5], zetas[6]);
    
    /* normalise A, and set R such that it is perpendicular to A */
    NormalizeZetas(zetas);
    // transfer from zetas to point R and vector A
    atm_pointR.x  = zetas[1]; 
    atm_pointR.y  = zetas[2]; 
    atm_pointR.z  = zetas[3]; 
    atm_vectorA.x = zetas[4]; 
    atm_vectorA.y = zetas[5]; 
    atm_vectorA.z = zetas[6];
    
    //printf("\n     zetas = %f %f %f %f %f %f", zetas[1], zetas[2], zetas[3], zetas[4], zetas[5], zetas[6]);
    
    // calculate the vectors atof(argv[1])from the atoms to the axes - dbl_AxisDistances
    CalculateVectorsFromAxis(atm_pointR, atm_vectorA, &dbl_AxisDistances[0], 
                              &atm_VectorsFromAxis[0], atm_Helix, &dbl_meanD,   
                              int_NumberOfAtoms);
    
    for (i=0; i<int_NumberOfAtoms; i++){
        dbl_dbyzeta[0][i] = -1 * atm_VectorsFromAxis[i].x / dbl_AxisDistances[i];
        dbl_dbyzeta[1][i] = -1 * atm_VectorsFromAxis[i].y / dbl_AxisDistances[i]; 
        dbl_dbyzeta[2][i] = -1 * atm_VectorsFromAxis[i].z / dbl_AxisDistances[i]; 
        
        //X dot A is calculated above, so the previous calculation could be used 
        dbl_XdotA = atom_dotproduct(atm_Helix[i],atm_vectorA);
        for (j=0; j<3; j++){
            dbl_dbyzeta[j+3][i] = dbl_dbyzeta[j][i] * dbl_XdotA;
            dbl_meanDByZeta[j] = dbl_meanDByZeta[j] + dbl_dbyzeta[j][i] /       int_NumberOfAtoms;
            dbl_meanDByZeta[j+3] = dbl_meanDByZeta[j+3] + dbl_dbyzeta[j+3][i] / int_NumberOfAtoms;
        } 
    }    
    //printf("\n Gradients: ");
    /*now sum up for each */
    for (j=1; j<=6; j++){
        (dbl_gradients)[j]=0;  
        for (i=0; i<int_NumberOfAtoms; i++){
            (dbl_gradients)[j] += 2 * (dbl_AxisDistances[i] - dbl_meanD) * (dbl_dbyzeta[j-1][i] - dbl_meanDByZeta[j-1]);           
        }
        //printf(" %f",dbl_gradients[j]);
    }     
  zetas[1] = atm_pointR.x;
  zetas[2] = atm_pointR.y;
  zetas[3] = atm_pointR.z;
  zetas[4] = atm_vectorA.x;
  zetas[5] = atm_vectorA.y;
  zetas[6] = atm_vectorA.z; 
  //printf("\n zetas:     %f %f %f %f %f %f \n", zetas[1], zetas[2], zetas[3], zetas[4], zetas[5], zetas[6]) ;
  
}

float Rosenbrock(float x)
  {
  float z=1;
  return 100 * (z- x*x)*(z- x*x)  + (1-x)*(1-x);
}      
float RosenbrockGrad(float x)
  {
  float z=1;
  //grads[0] =
  return -400 * x*(z-x*x)-2*x*(1-x);
  //grads[1] = 200*(z-x*x);
} 

int main(int argc, char *argv[])
{
    /****************************************************************
    * this will just initialise the helix, and call other functions *
    * such as rmsd and helix fit and other things            *     
    ****************************************************************/
    int int_NumberOfAtoms;
    int i,j;
    float dbl_rmsds, dbl_rmsds1, dbl_rmsds2, dbl_CylinderRadius; 
        /*make up a starting point and vector*/
    

    int iter = 100;
    float *zetas;
    double z1 = atof(argv[1]);
    zetas=vector(1,6);
    
    zetas[1] = 1;
    zetas[2] = 1;
    zetas[3] = 1;
    zetas[4] = 1;
    zetas[5] = 1;
    zetas[6] = 1;
    
    /*number of atoms*/
    if (argc == 1){
      printf("No arguments Passed to Cylinder");
      return 1;
    }
    ++argv;
    int_NumberOfAtoms = atoi(*argv);
    //printf("%s ", *argv);
    atom atm_Helix[int_NumberOfAtoms];
    
    if (argc != (int_NumberOfAtoms*3+8)){
      fprintf(stderr, "%d arguments, %d atoms \nNeed num_atoms, 6 x starting point, and 3 x number of atoms (i.e. xyz for each) \n[plus of course one for the program name]\n", argc, int_NumberOfAtoms);
      exit(1);
    }
    
    /*read in starting point */
    for (i=1; i <= 6; i++){
      ++argv;
      //printf("%s ", *argv);
      zetas[i] = atof(*argv);
    } 
    
    i=0;
    while (i < int_NumberOfAtoms){
      ++argv;
      atm_Helix[i].x = atof(*argv); ++argv;
      atm_Helix[i].y = atof(*argv); ++argv;
      atm_Helix[i].z = atof(*argv);
      i+=1;
    }

    /*make a helix*/
    //atm_Helix=malloc(sizeof(atom)*int_NumberOfAtoms);    
    /*for (i=0; i<int_NumberOfAtoms;i++){
        atm_Helix[i].x = sin(PI * i /2) + 4;
        atm_Helix[i].y = cos(PI * i /2);
        atm_Helix[i].z = i;        
    }*/

    //printf("\n initial zetas: %f %f %f %f %f %f \n", zetas[1], zetas[2], 
            //zetas[3], zetas[4], zetas[5], zetas[6]);
  
    dbl_rmsds1 = cylinder_rmsd(zetas, atm_Helix, int_NumberOfAtoms);  
    frprmn(zetas, 6, 0.0002, &iter, &dbl_rmsds, cylinder_rmsd, gradients, 
           atm_Helix, int_NumberOfAtoms, NormalizeZetas);
    
    // lets do it again, to make sure we didnt get stuck
    frprmn(zetas, 6, 0.0002, &iter, &dbl_rmsds, cylinder_rmsd, gradients, 
           atm_Helix, int_NumberOfAtoms, NormalizeZetas);    



    dbl_rmsds2 = cylinder_rmsd(zetas, atm_Helix, int_NumberOfAtoms);         
    
    // calculate the radius
    dbl_CylinderRadius = cylinder_radius(zetas, atm_Helix, int_NumberOfAtoms);
    for (i=0; i<1; i++){
        for (j=1; j<=6; j++){
            printf("%f ", zetas[j]);
        }
    }
    printf("%f %f %f    ",dbl_CylinderRadius, dbl_rmsds2, dbl_rmsds1);
    
    
    return 0;
}
  


