// =================================================================== //
// this is file mainDiskNuCte.cpp, created at 10-Jun-2007              //
// maintained by Gustavo Rabello dos Anjos                             //
// e-mail: gustavo.rabello@gmail.com                                   //
// =================================================================== //

#include <cmath>
#include "Model3D.h"
#include "CGSolver.h"
#include "PCGSolver.h"
#include "GMRes.h"
#include "InOut.h"
#include "Simulator3D.h"
#include "PetscSolver.h"
#include "petscpc.h"

#define NUMPHASES 1

int main(int argc, char **argv)
{
 PetscInitialize(&argc, &argv, PETSC_NULL, PETSC_NULL);

 int iter = 0;
 real Re = 1;
 real cfl = 10;
 real rho_l = 1.0;
 Solver *solverP = new PetscSolver(KSPCG,PCSOR);
 Solver *solverV = new PetscSolver(KSPCG,PCICC);

 const char *mesh = "../../db/mesh/3d/disk6-10-20.vtk";
 const char *binFolder  = "./bin/";
 const char *datFolder  = "./dat/";
 const char *vtkFolder  = "./vtk/";
 const char *simFolder  = "./sim/";

 Model3D m1;
 m1.setMeshDisk(5,5,20);
 m1.setAdimenDisk();
 m1.setMiniElement();
 //m1.setQuadElement();
 m1.setNuZDiskBC();;
 m1.setOFace();
 //m1.readBaseStateNu("NuCte");

 Simulator3D s1(m1);

 s1.setRe(Re);
 s1.setCfl(cfl);
 s1.setRho(rho_l);
 s1.setSolverVelocity(solverV);
 s1.setSolverPressure(solverP);

 s1.init();
 s1.setDtDisk();
 s1.assembleNuZ();
 s1.matMount();
 s1.setUnCoupledBC(); 

 if( (*(argv+1)) == NULL )
 {
  cout << endl;
  cout << "--------------> STARTING FROM 0" << endl;
  cout << endl;
 }
 else if( strcmp( *(argv+1),"restart") == 0 )
 {
  cout << endl;
  cout << "--------------> RE-STARTING..." << endl;
  cout << endl;

  iter = s1.loadSolution("sim",atoi(*(argv+2)));
 }

 InOut save(m1,s1); // cria objeto de gravacao
 save.saveVTK(vtkFolder,"geometry");
 save.saveInfo("./","info",mesh);
 save.printInfo(mesh);

 int nIter = 1000;
 int nR = 10;
 for( int i=0;i<nIter;i++ )
 {
  for( int j=0;j<nR;j++ )
  {
   cout << "____________________________________ Iteration: " 
	    << i*nR+j+iter << endl;

   /* dt variable */
   //s1.setDtDisk();
   //s1.assembleNuZ();
   //s1.matMount();
   //s1.setUnCoupledBC(); 

   s1.stepSL();
   s1.setRHS();
   s1.unCoupled();
   save.saveVonKarman(simFolder,"vk",i*nR+j+iter);
   save.saveVTK(vtkFolder,"sim",i*nR+j+iter);
   save.saveSol(binFolder,"sim",i*nR+j+iter);
   save.saveConvergence(datFolder,"convergence");

   s1.saveOldData();

   cout << "__________________________________________ End: " 
	    << i*nR+j+iter << endl;
  }
 }

 PetscFinalize();
 return 0;
}


