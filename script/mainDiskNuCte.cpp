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
#include "Simulator3D.h"
#include "TElement.h"
#include "InOut.h"
#include "PetscSolver.h"
#include "petscpc.h"
#include "colors.h"

#define NUMPHASES 1

int main(int argc, char **argv)
{
 PetscInitialize(&argc, &argv, PETSC_NULL, PETSC_NULL);

 int iter = 1;
 double Re = 1;
 double cfl = 10;
 double mu_l = 1.0;
 double rho_l = 1.0;
 //Solver *solverP = new PetscSolver(KSPPREONLY,PCNONE);
 Solver *solverP = new PetscSolver(KSPCG,PCSOR);
 Solver *solverV = new PetscSolver(KSPCG,PCICC);

 const char *binFolder  = "./bin/";
 const char *datFolder  = "./dat/";
 const char *vtkFolder  = "./vtk/";
 const char *simFolder  = "./sim/";

 string meshFile = "disk6-10-20.vtk";
 string meshDir = (string) getenv("DATA_DIR");
 meshDir += "/mesh/3d/" + meshFile;
 const char *mesh = meshDir.c_str();

 Model3D m1;
 m1.setMeshDisk(6,8,12);
 m1.setAdimenDisk();
 m1.setMapping();
#if NUMGLEU == 5
 m1.setMiniElement();
#else
 m1.setQuadElement();
#endif

 m1.setNeighbour();
 m1.setTriEdge();
 m1.setMapEdgeTri();
 m1.setInitSurfaceArea();
 m1.setSurfaceArea();
 m1.setInitSurfaceVolume();
 m1.setSurfaceVolume();
 m1.tetMeshStats();

 // F, G and H
 m1.setInfiniteDiskBC(1.3326987e-04,1.7327920e-04,8.8416563E-01);
 //m1.setCDiskBC();
  

 Simulator3D s1(m1);

 s1.setRe(Re);
 s1.setCfl(cfl);
 s1.setMu(mu_l);
 s1.setRho(rho_l);
 s1.setSolverVelocity(solverV);
 s1.setSolverPressure(solverP);

 s1.init();
 //s1.initDiskBaseState("../../db/baseState/nuCte/","analiticoNuCte.dat");

 s1.setDtEulerian();
 s1.assembleNuCte();
 //s1.assembleC();
 s1.matMount();
 //s1.matMountC();
 s1.setUnCoupledBC(); 
 //s1.setUnCoupledCBC(); 
 
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

  iter = s1.loadSolution("./","sim",atoi(*(argv+2)));
 }

 InOut save(m1,s1); // cria objeto de gravacao
 save.saveVTK(vtkFolder,"geometry");
 save.saveInfo("./","info",mesh);
 save.printSimulationReport();

 int nIter = 1000;
 int nR = 10;
 for( int i=0;i<nIter;i++ )
 {
  for( int j=0;j<nR;j++ )
  {
   cout << color(none,magenta,black);
   cout << "____________________________________ Iteration: "
	    << i*nR+j+iter << endl << endl;
   cout << resetColor();

   /* dt variable */
   //s1.setDtEulerian();
   //s1.assembleNuCte();
   ////s1.assembleC();
   //s1.matMount();
   ////s1.matMountC();
   //s1.setUnCoupledBC(); 
   //s1.setUnCoupledCBC(); 

   s1.stepSL();
   s1.setRHS();
   //s1.setCRHS();
   s1.unCoupled();
   //s1.unCoupledC();
   save.saveVonKarman(simFolder,"vk");
   save.saveDiskRadiusError(simFolder,
	                        "vkError",
							"../../db/baseState/nuCte/analiticoNuCte.dat");
   save.saveDiskError(datFolder,
	                  "diskError",
					  "../../db/baseState/nuCte/analiticoNuCte.dat");
   save.saveConvergence(datFolder,"convergence");

   s1.saveOldData();

   s1.timeStep();

   cout << color(none,magenta,black);
   cout << "________________________________________ END of "
	    << i*nR+j+iter << endl << endl;;
   cout << resetColor();
  }
  save.saveVTK(vtkFolder,"sim",(i+1)*nR+iter-1);
  //save.saveVTKSurface(vtkFolder,"sim",(nR-1)+i*nR+iter-1);
  save.saveSol(binFolder,"sim",(i+1)*nR+iter-1);
  save.printMeshReport();
  save.saveMeshInfo(datFolder);
 }

 PetscFinalize();
 return 0;
}


