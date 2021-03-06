// =================================================================== //
// this is file mainOscillatingDrop.cpp, created at 10-Jan-2011        //
// maintained by Gustavo Rabello dos Anjos                             //
// e-mail: gustavo.rabello@gmail.com                                   //
// =================================================================== //

#include <cmath>
#include "Model3D.h"
#include "Simulator3D.h"
#include "CGSolver.h"
#include "PCGSolver.h"
#include "GMRes.h"
#include "TElement.h"
#include "InOut.h"
#include "PetscSolver.h"
#include "petscksp.h"
#include "colors.h"

#define NUMPHASES 2

int main(int argc, char **argv)
{
 PetscInitialize(&argc,&argv,PETSC_NULL,PETSC_NULL);
 
 int iter = 1;
 double Re = 1000;
 double Sc = 1;
 double We = 1;
 double Fr = 1;
 double c1 = 0.0;  // lagrangian
 double c2 = 1.0;  // smooth vel
 double c3 = 0.0;  // smooth coord (fujiwara)
 double d1 = 0.0;  // surface tangent velocity u_n=u-u_t 
 double d2 = 0.0;  // surface smooth cord (fujiwara)
 double alpha = 1;

 double mu_in = 1;
 double mu_out = 0.01;

 double rho_in = 1; 
 double rho_out = 0.001;

 double cfl = 0.5;

 //string meshFile = "sphere.msh";
 string meshFile = "staticDumped1.msh";

 Solver *solverP = new PetscSolver(KSPGMRES,PCILU);
 Solver *solverV = new PetscSolver(KSPCG,PCICC);
 Solver *solverC = new PetscSolver(KSPCG,PCICC);

 const char *binFolder  = "./bin/";
 const char *vtkFolder  = "./vtk/";
 const char *mshFolder  = "./msh/";
 const char *datFolder  = "./dat/";
 string meshDir = (string) getenv("DATA_DIR");
 meshDir += "/gmsh/3d/sphere/oscillating/" + meshFile;
 const char *mesh = meshDir.c_str();

 Model3D m1;
 Simulator3D s1;

 if( *(argv+1) == NULL )     
 {
  cout << endl;
  cout << "--------------> STARTING FROM 0" << endl;
  cout << endl;

  const char *mesh1 = mesh;
  m1.readMSH(mesh1);
  m1.setInterfaceBC();
  m1.setTriEdge();
  m1.mesh2Dto3D();
  m1.setMapping();
#if NUMGLEU == 5
 m1.setMiniElement();
#else
 m1.setQuadElement();
#endif
  m1.setSurfaceConfig();
  m1.setInitSurfaceVolume();
  m1.setInitSurfaceArea();
  m1.setGenericBC();
  m1.setSphereToEllipsoid(1.01);

  s1(m1);

  s1.setRe(Re);
  s1.setSc(Sc);
  s1.setWe(We);
  s1.setFr(Fr);
  s1.setC1(c1);
  s1.setC2(c2);
  s1.setC3(c3);
  s1.setD1(d1);
  s1.setD2(d2);
  s1.setAlpha(alpha);
  //s1.setDtALETwoPhase(dt);
  s1.setMu(mu_in,mu_out);
  s1.setRho(rho_in,rho_out);
  s1.setCfl(cfl);
  s1.init();
  s1.setDtALETwoPhase();
  s1.setSolverPressure(solverP);
  s1.setSolverVelocity(solverV);
  s1.setSolverConcentration(solverC);
 }
 else if( strcmp( *(argv+1),"restart") == 0 ) 
 {
  cout << endl;
  cout << "--------------> RE-STARTING..." << endl;
  cout << endl;

  // load surface mesh
  string aux = *(argv+2);
  string file = (string) "./msh/newMesh-" + *(argv+2) + (string) ".msh";
  const char *mesh2 = file.c_str();
  m1.readMSH(mesh2);
  m1.setInterfaceBC();
  m1.setTriEdge();
  m1.mesh2Dto3D();

  s1(m1);

  // load 3D mesh
  file = (string) "./vtk/sim-" + *(argv+2) + (string) ".vtk";
  const char *vtkFile = file.c_str();

  m1.readVTK(vtkFile);
  m1.setMapping();
#if NUMGLEU == 5
 m1.setMiniElement();
#else
 m1.setQuadElement();
#endif
  m1.readVTKHeaviside(vtkFile);
  m1.setSurfaceConfig();
  m1.setInitSurfaceVolume();
  m1.setInitSurfaceArea();
  m1.setGenericBC();

  s1(m1);

  s1.setSolverPressure(solverP);
  s1.setSolverVelocity(solverV);
  s1.setSolverConcentration(solverC);

  iter = s1.loadSolution("./","sim",atoi(*(argv+2)));
 }
 else
  cout << "The options are: NULL or restart" << endl;


 InOut save(m1,s1); // cria objeto de gravacao
 save.saveVTK(vtkFolder,"geometry");
 save.saveVTKSurface(vtkFolder,"geometry");
 save.saveMeshInfo(datFolder);
 save.saveInfo(datFolder,"info",mesh);
 save.printMeshReport();

 int nIter = 13000;
 int nReMesh = 1;
 for( int i=1;i<=nIter;i++ )
 {
  for( int j=0;j<nReMesh;j++ )
  {

   cout << color(none,magenta,black);
   cout << "____________________________________ Iteration: " 
	    << iter << endl << endl;
   cout << resetColor();

   //s1.stepLagrangian();
   s1.stepALE();
   s1.setDtALETwoPhase();

   InOut save(m1,s1); // cria objeto de gravacao
   save.printSimulationReport();

   s1.movePoints();
   s1.assemble();
   s1.matMount();
   s1.setUnCoupledBC();
   s1.setRHS();
   //s1.setGravity("Z");
   //s1.setInterface();
   s1.setInterfaceGeo();
   s1.unCoupled();

   save.saveMSH(mshFolder,"newMesh",iter);
   save.saveVTK(vtkFolder,"sim",iter);
   save.saveVTKSurface(vtkFolder,"sim",iter);
   save.saveSol(binFolder,"sim",iter);
   save.saveBubbleInfo(datFolder);
   //save.crossSectionalVoidFraction(datFolder,"voidFraction",iter);

   s1.saveOldData();

   s1.timeStep();

   cout << color(none,magenta,black);
   cout << "________________________________________ END of " 
	    << iter << endl << endl;;
   cout << resetColor();

   iter++;
  }
//--------------------------------------------------
//   Model3D mOld = m1; 
// 
//   /* *********** MESH TREATMENT ************* */
//   // set normal and kappa values
//   //m1.setNormalAndKappa();
//   //m1.initMeshParameters();
// 
//   // 3D operations
//   //m1.insert3dMeshPointsByDiffusion();
//   //m1.remove3dMeshPointsByDiffusion();
//   //m1.removePointByVolume(0.005);
//   //m1.removePointsByInterfaceDistance();
//   //m1.remove3dMeshPointsByDistance();
//   //m1.delete3DPoints();
// 
//   // surface operations
//   //m1.insertPointsByLength("flat");
//   //m1.insertPointsByCurvature("flat");
//   //m1.removePointsByCurvature();
//   //m1.insertPointsByInterfaceDistance("flat");
//   //m1.contractEdgesByLength("flat");
//   //m1.removePointsByLength();
//   //m1.flipTriangleEdges();
//   //m1.removePointsByNeighbourCheck();
//   /* **************************************** */
// 
//   //m1.mesh2Dto3DOriginal();
//   m1.mesh3DPoints();
//   m1.setMapping();
// #if NUMGLEU == 5
//  m1.setMiniElement();
// #else
//  m1.setQuadElement();
// #endif
//   m1.setSurfaceConfig();
//   m1.setInterfaceBC();
//   m1.setGenericBC();
// 
//   Simulator3D s2(m1,s1);
//   s2.applyLinearInterpolation(mOld);
//   s1 = s2;
//   s1.setSolverPressure(solverP);
//   s1.setSolverVelocity(solverV);
//   s1.setSolverConcentration(solverC);
// 
//   InOut saveEnd(m1,s1); // cria objeto de gravacao
//   saveEnd.printMeshReport();
//   saveEnd.saveMSH(mshFolder,"newMesh",iter-1);
//   saveEnd.saveVTK(vtkFolder,"sim",iter-1);
//   saveEnd.saveVTKSurface(vtkFolder,"sim",iter-1);
//   saveEnd.saveSol(binFolder,"sim",iter-1);
//   //saveEnd.saveVTU(vtkFolder,"sim",iter-1);
//   //saveEnd.saveSolTXT(binFolder,"sim",iter-1);
//   saveEnd.saveMeshInfo(datFolder);
//-------------------------------------------------- 
 }

 PetscFinalize();
 return 0;
}


