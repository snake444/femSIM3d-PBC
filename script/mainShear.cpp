// =================================================================== //
// this is file main.cpp, created at 10-Jun-2007                       //
// maintained by Gustavo Rabello dos Anjos                             //
// e-mail: gustavo.rabello@gmail.com                                   //
// =================================================================== //

#include <cmath>
#include "Model3D.h"
#include "Simulator3D.h"
#include "CGSolver.h"
#include "PCGSolver.h"
#include "TElement.h"
#include "GMRes.h"
#include "InOut.h"
#include "Helmholtz3D.h"
#include "PetscSolver.h"
#include "petscksp.h"
#include "colors.h"

#define NUMPHASES 2

int main(int argc, char **argv)
{
 /* This test case applies a prescribed vortex field in a unit cube to
  * test the re-meshing techinique of the surface mesh. 
  *
  * OBS.: - comment stepSL() on Simulator3D::stepALE
  *       - switch to tetrahedralize( (char*) "QYYAp",&in,&out ) on
  *       Model3D::mesh3DPoints
  *
  * Since the field is prescribed, there is no need of calculating the
  * convection in a Euleurian way (stepSL) and the insertion of nodes on
  * the 3D mesh.
  *
  * */

 PetscInitializeNoArguments();
 
 int iter = 1;
 double c1 = 1.0;  // lagrangian
 double c2 = 0.0;  // smooth vel
 double c3 = 0.0;  // smooth coord (fujiwara)
 double d1 = 0.0;  // surface tangent velocity u_n=u-u_t 
 double d2 = 0.0;  // surface smooth cord (fujiwara)

 double dt = 0.0003;
 double T = 3.0;
 double time = 0.0;

 //string meshFile = "sphere.msh";
 string meshFile = "sphere.msh";

 const char *vtkFolder  = "./vtk/";
 const char *mshFolder  = "./msh/";
 const char *datFolder  = "./dat/";
 string meshDir = (string) getenv("DATA_DIR");
 meshDir += "/gmsh/3d/sphere/shear/" + meshFile;
 const char *mesh = meshDir.c_str();

 Model3D m1;
 Simulator3D s1;

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

 s1(m1);

 s1.setC1(c1);
 s1.setC2(c2);
 s1.setC3(c3);
 s1.setD1(d1);
 s1.setD2(d2);
 s1.setDt(dt);

 InOut save(m1,s1); // cria objeto de gravacao
 save.saveVTK(vtkFolder,"geometry");
 save.saveVTKSurface(vtkFolder,"geometry");
 save.saveMeshInfo(datFolder);
 save.saveInfo(datFolder,"info",mesh);

 save.saveMSH(mshFolder,"newMesh",iter);
 save.saveVTK(vtkFolder,"sim",iter);
 save.saveVTKSurface(vtkFolder,"sim",iter);
 save.saveBubbleInfo(datFolder);
 save.savePoint(datFolder,0);
 save.savePoint(datFolder,1);
 save.savePoint(datFolder,2);
 save.savePoint(datFolder,3);
 iter++;

 int nIter = 1;
 int nReMesh = 10003;
 for( int i=1;i<=nIter;i++ )
 {
  for( int j=0;j<nReMesh;j++ )
  {

   cout << color(none,magenta,black);
   cout << "____________________________________ Iteration: " 
	    << iter << endl << endl;
   cout << resetColor();

   InOut save(m1,s1); // cria objeto de gravacao
   save.printSimulationReport();

   time = s1.getTime();

   s1.stepImposedPeriodicField("3d",T,time);
   //s1.stepALE();
   s1.stepLagrangian();
   //s1.setInterfaceGeo();

   double field = cos(3.14159265358*time/T);
   cout << endl;
   cout << "                             | T:        " << T << endl;
   cout << "                             | dt:       " << dt << endl;
   cout << "                             | time:     " << time << endl;
   cout << "                             | iter:     " << iter << endl;
   cout << "                             | field:    " << field << endl;  
   cout << endl;

   save.saveMSH(mshFolder,"newMesh",iter);
   save.saveVTK(vtkFolder,"sim",iter);
   save.saveVTKSurface(vtkFolder,"sim",iter);
   save.saveBubbleInfo(datFolder);
   save.savePoint(datFolder,0);
   save.savePoint(datFolder,1);
   save.savePoint(datFolder,2);
   save.savePoint(datFolder,3);
   //save.crossSectionalVoidFraction(datFolder,"voidFraction",iter);

   s1.saveOldData();

   s1.timeStep();

   cout << color(none,magenta,black);
   cout << "________________________________________ END of " 
	    << iter << endl << endl;;
   cout << resetColor();

   iter++;
  }
  Model3D mOld = m1; 

  /* *********** MESH TREATMENT ************* */
  // set normal and kappa values
  m1.setNormalAndKappa();
  m1.initMeshParameters();

  // 3D operations
  //m1.insert3dMeshPointsByDiffusion();
  //m1.remove3dMeshPointsByDiffusion();
  //m1.removePointByVolume();
  //m1.removePointsByInterfaceDistance();
  //m1.remove3dMeshPointsByDistance();
  m1.remove3dMeshPointsByHeight();
  m1.delete3DPoints();

  // surface operations
  //m1.smoothPointsByCurvature();

  m1.insertPointsByLength("flat");
  //m1.insertPointsByCurvature("flat");
  //m1.removePointsByCurvature();
  //m1.insertPointsByInterfaceDistance("flat");
  m1.contractEdgesByLength("flat");
  //m1.removePointsByLength();
  //m1.flipTriangleEdges();

  //m1.removePointsByNeighbourCheck();
  //m1.checkAngleBetweenPlanes();
  /* **************************************** */

  //m1.mesh2Dto3DOriginal();
  m1.mesh3DPoints();
  m1.setMapping();
#if NUMGLEU == 5
 m1.setMiniElement();
#else
 m1.setQuadElement();
#endif
  m1.setSurfaceConfig();

  Simulator3D s2(m1,s1);
  s1 = s2;
  s1.setCentroidVelPos();

  InOut saveEnd(m1,s1); // cria objeto de gravacao
  saveEnd.printMeshReport();
  saveEnd.saveMeshInfo(datFolder);
 }

 PetscFinalize();
 return 0;
}


