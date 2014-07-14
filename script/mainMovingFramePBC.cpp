/* File: mainMovingFramePBC.cpp                                            
 * Created on February 17th, 2014                                      
 * Author: Gustavo Charles P. de Oliveira                              
 * e-mail: tavolesliv@gmail.com
 * Maintainance: Gustavo Rabello dos Anjos                            
 * E-mail: gustavo.rabello@gmail.com                                   
 *
 * Description: version adapted from mainMovingFrame.cpp to include periodic 
 * boundary conditions. Suitable to run two-phase simulations for 
 * longitudinal channels. PBC is implemented to work with meshes whose
 * x-axis is the streamwise direction of spatial periodicity.
 *
 * \remark: PBC working for velocity and pressure. 
 *         To be implemented to further scalar fields, such as
 *         temperature.  
 * 
 *
 *  ============
 *   Guidelines
 *  ============
 * 
 *
 * # PRE-PROCESSING
 *
 * - Choose mesh file extension: vtk or msh;
 * - Introduce path to file;
 * - Call method to check spatial periodicity of the mesh;
 * 
 * # PBC
 *
 * - Impose pressure gradient based on input parameters;
 *
 *
 * # ITERATIVE STEP
 *
 * - Allow gravity effects or not;
 * - Allow pressure gradient or not;
 *
 */

// C++ Header Files
#include <cmath>

// Code Header Files
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
#include "Periodic3D.h"

// Number of phases (Do not change!)
#define NUMPHASES 2

// Main function
int main(int argc, char **argv)
{
 /* PRE-PROCESSING SECTION */

 PetscInitialize(&argc,&argv,PETSC_NULL,PETSC_NULL);
 //PetscInitializeNoArguments();

 //** Numerical Parameters
 // bogdan's thesis 2010 (Bhaga and Weber, JFM 1980)
 int iter = 1;
 double alpha = 1.0;   // time discrete method
 double cfl = 0.8;

 //** Physical Parameters
 /*
 double Re = 14.5767; 
 double Re = 14.5767; 
 double We = 0.2007;
 double Fr = 13.2160;
 double mu_in = 1.820E-05;
 double mu_out = 1.002E-3;
 double rho_in = 1.205;
 double rho_out = 998.63;
 double sigma = 0.0728;
 */

 /* test
 double Re = 100.0; 
 double We = 10.0;
 double Fr = 1.0;
 double mu_in = 0.1;
 double mu_out = 1.0;
 double rho_in = 0.001;
 double rho_out = 1.0;
 double sigma = 1.0;
*/
 
 // Gustavo thesis: sugar-syrup 1
 //double N = 194.88;
 double Re = 33.0413; 
 double We = 115.662;
 double Fr = 1.0;
 double mu_in = 1.78E-5;
 //double mu_out = 2.73;
 double mu_out = 0.5396;
 double rho_in = 1.225;
 double rho_out = 1350.0;
 double sigma = 0.078;

 //** Moving Frame Settings
 const char* _frame = "moving";

 // fixed
 double c1 = 0.1;      // lagrangian
 double c2 = 0.1;      // smooth vel
 double c3 = 10.0;      // smooth coord (fujiwara)
 double d1 = 1.0;      // surface tangent velocity u_n=u-u_t 
 double d2 = 0.1;      // surface smooth cord (fujiwara)
 
 // moving
 if ( strcmp( _frame,"moving") == 0 )
 {
   c1 = 0.0;      // lagrangian
   c2 = 1.0;      // smooth vel
   c3 = 10.0;      // smooth coord (fujiwara)
   d1 = 1.0;      // surface tangent velocity u_n=u-u_t 
   d2 = 0.1;      // surface smooth cord (fujiwara)
 }

 //*** File
 //string meshFile = "bubble-jet.msh";
 //string meshFile = "3bubbles-V.msh";
 //string meshFile = "2bubbles-2V.msh";
 //string meshFile = "long-bubble2-3V.msh";
 //string meshFile = "rising-bubble-pbc-g4D.msh";
 //string meshFile = "rising-periodic-mesh-pbc.msh";
 string meshFile = "test.msh";
 
 //** Solver and Pre-Conditioner Choice - pressure, velocity, scalar
 //Solver *solverP = new PetscSolver(KSPGMRES,PCILU);
 Solver *solverP = new PetscSolver(KSPCG,PCJACOBI);
 //Solver *solverP = new PetscSolver(KSPGMRES,PCJACOBI);
 Solver *solverV = new PetscSolver(KSPCG,PCICC);
 Solver *solverC = new PetscSolver(KSPCG,PCJACOBI);
 //Solver *solverC = new PCGSolver();
 //Solver *solverV = new PetscSolver(KSPCG,PCJACOBI);

 //** Data Saving Directories
 const char *binFolder  = "./bin/";
 const char *mshFolder  = "./msh/";
 //const char *datFolder  = "/home/gcpoliveira/post-processing/vtk/3d/slug-nb1-V/dat/";
 //const char *vtkFolder  = "/home/gcpoliveira/post-processing/vtk/3d/slug-nb1-V/";
 //const char *datFolder  = "/home/gcpoliveira/post-processing/vtk/3d/slug-nb2-2V/dat/";
 //const char *vtkFolder  = "/home/gcpoliveira/post-processing/vtk/3d/slug-nb2-2V/";
 //const char *datFolder  = "/home/gcpoliveira/post-processing/vtk/3d/slug-nb3-3V/dat/";
 //const char *vtkFolder  = "/home/gcpoliveira/post-processing/vtk/3d/slug-nb3-3V/";
 //const char *datFolder  = "/home/gcpoliveira/post-processing/vtk/3d/bubble-jet/dat/";
 //const char *vtkFolder  = "/home/gcpoliveira/post-processing/vtk/3d/bubble-jet/";
 const char *datFolder  = "/home/gcpoliveira/post-processing/vtk/3d/rising-periodic-mesh-pbc-circular/dat/";
 const char *vtkFolder  = "/home/gcpoliveira/post-processing/vtk/3d/rising-periodic-mesh-pbc-circular/";
 //const char *vtkFolder  = "./vtk/";
 //const char *datFolder  = "./dat/";
 
 
 //*** Peixoto's tree
 string meshDir = (string) getenv("MESH3D_DIR");
 meshDir += "/" + meshFile;
 const char *mesh = meshDir.c_str();

 //** Model Constructor
 Model3D m1;

 // Printing -- no args
 
  cout << endl;
  cout << "--------------> STARTING FROM 0" << endl;
  cout << endl;

  const char *mesh1 = mesh;

  m1.readMSH(mesh1); 
  m1.setInterfaceBC();
  m1.setTriEdge();
  m1.mesh2Dto3D();
  //m1.setStretchJetMesh(); // mesh transform
  m1.setMapping();
  #if NUMGLEU == 5
 	m1.setMiniElement();
   #else
 	m1.setQuadElement();
  #endif
  m1.setSurfaceConfig(); // configurations of interfaces
  m1.setInitSurfaceVolume(); // gets volume of the bubbles
  m1.setInitSurfaceArea(); // gets surface area of the bubbles

 //*** B.C.
 m1.setGenericBC();
	
 //*** Periodic Constructor
 Periodic3D pbc(m1);
 pbc.MountPeriodicVectorsNew();
 
 //*** Simulator Constructor
 Simulator3D s2(pbc,m1);

 //*** Physics
 s2.setRe(Re);
 s2.setWe(We);
 s2.setFr(Fr);
 s2.setMu(mu_in,mu_out);
 s2.setRho(rho_in,rho_out);
 s2.setSigma(sigma);
  	
 //*** ALE parameters
 s2.setC1(c1);
 s2.setC2(c2);
 s2.setC3(c3);
 s2.setD1(d1);
 s2.setD2(d2);
 
 //*** Numerics
 s2.setCfl(cfl);
 s2.setDtALETwoPhase();
 s2.setAlpha(alpha);
  	
 //*** Solver
 s2.setSolverPressure(solverP);
 s2.setSolverVelocity(solverV);
 s2.setSolverConcentration(solverC);

 //*** Starting flow
 s2.setBetaPressureLiquid();

 s2.init();
 
 /*
  // Point's distribution
 Helmholtz3D h1(m1);
 h1.setBC();
 h1.initThreeBubbles();
 //h1.initJet(1.0,4.0); //<<< 
 //h1.initRisingBubble();
 h1.assemble();
 h1.setk(0.05);
 h1.matMountC();
 h1.setUnCoupledCBC(); 
 h1.setCRHS();
 h1.unCoupledC();
 h1.setModel3DEdgeSize();
 */

 InOut save(m1,s2);
 
 //*** Output (Initial Condition)
 save.saveVTK(vtkFolder,"initial",0);

 /* Saving mesh info */
 save.saveVTK(vtkFolder,"geometry");
 save.saveVTKSurface(vtkFolder,"geometry");
 save.saveMeshInfo(datFolder);
 save.saveInfo(datFolder,"info",mesh);

 //** Moving Frame variables
 double vinst=0;
 double vref=0;
 double xref=0;
 double xinit=0;
 double dx=0;

 if ( strcmp( _frame,"moving") == 0 )
 {
    // moving
	vref = s2.getURef();
	xref = s2.getXRef();
	s2.setCentroidVelPos();
	xinit = s2.getCentroidPosXAverage();
 }

 int nIter = 5;
 int nReMesh = 1;

 for( int i=1;i<=nIter;i++ )
 {
   for( int j=0;j<nReMesh;j++ )
   {
      cout << color(none,magenta,black);
      cout << "____________________________________ Iteration: " 
	       << iter << endl << endl;
      cout << resetColor();

      // moving frame correction
	  if( strcmp( _frame,"moving") == 0 )
	  {
	    dx = s2.getCentroidPosXAverage() - xinit;
		vinst = s2.getCentroidVelXAverage() + dx/s2.getDt();
		vref += vinst;
		xref += vref*s2.getDt();
		cout << "vref: " << vref << " xref: " << xref << endl;
		cout << "dx: "<< dx << endl;
		s2.setUSol(vinst);
		m1.setGenericBC(vref);
		pbc.MountPeriodicVectorsNew();
		s2.setURef(vref);
		s2.setXRef(xref);
	  }

      s2.setDtALETwoPhase();

      save.printSimulationReport();

	  // Convection
      //s2.stepLagrangian();
      s2.stepALE();

      s2.movePoints();

	  // Assembling
      s2.assemble();
      s2.matMount();

	  // B.C.
      s2.setUnCoupledPBC(); // <<<
      //s2.setUnCoupledBC();
      
	  // Physical effects
	  s2.setGravity("-X");
	  s2.setBetaFlowLiq("+X");
      
	  // R.H.S.
	  s2.setRHS_PBC();
	  //s2.setRHS();

	  // Interface
      //s2.setInterface();
      s2.setInterfaceGeo();
      
	  // Copy
	  s2.setCopyDirectionPBC("RL");

	  // System solving
      //s2.unCoupledPBC(); // <<<
      s2.unCoupledPBCNew(); // <<<
      //s2.unCoupled();

	  //*** Solution Saving 
      //save.saveMSH(mshFolder,"newMesh",iter);
      save.saveVTK(vtkFolder,"sim",iter);
      //save.saveVTU(vtkFolder,"sim",iter);
      save.saveVTKSurface(vtkFolder,"sim",iter);
      //save.saveSol(binFolder,"sim",iter);
      //save.saveBubbleInfo(datFolder);
	  //save.saveBubbleShapeFactors(datFolder,"shapeFactors",iter);
      s2.saveOldData();

      s2.timeStep();

      cout << color(none,magenta,black);
      cout << "________________________________________ END of " 
	       << iter << endl << endl;;
      cout << resetColor();

      iter++;
     }
     /* Points distribution - Helmholtz eq. */

     /*
     Helmholtz3D h2(m1,h1);
     h2.setBC();
	 h2.initThreeBubbles();
     //h2.initJet(1.0,4.0); //<<<
	 //h2.initRisingBubble();
     h2.assemble();
     h2.setk(0.05);
     h2.matMountC();
     h2.setUnCoupledCBC(); 
     h2.setCRHS();
     h2.unCoupledC();
     h2.saveChordalEdge(datFolder,"edge",iter-1);
     h2.setModel3DEdgeSize();
	 */

     Model3D mOld = m1; 

	 //m1.setUnstretchJetMesh(); // MESH TRANSFORM

     /* *********** MESH TREATMENT ************* */
     // set normal and kappa values
     m1.setNormalAndKappa();
     //m1.initMeshParameters(); //<<<
	 
     /* 3D operations */
     
	 //m1.insert3dMeshPointsByDiffusion(3.0);
     //m1.remove3dMeshPointsByDiffusion(0.5);
     //m1.removePointByVolume();
     //m1.removePointsByInterfaceDistance();
     //m1.remove3dMeshPointsByDistance();
     //m1.remove3dMeshPointsByHeight();
     //m1.delete3DPoints();

     // surface operations
     //m1.smoothPointsByCurvature();

     //m1.insertPointsByLength("curvature");
     //m1.insertPointsByLength("flat");
     //m1.insertPointsByCurvature("flat");
     //m1.removePointsByCurvature();
     //m1.insertPointsByInterfaceDistance("flat");
     //m1.contractEdgesByLength("curvature");
     //m1.contractEdgesByLength("flat");
     //m1.removePointsByLength();
     //m1.flipTriangleEdges();
     //m1.removePointsByNeighbourCheck();
     //m1.checkAngleBetweenPlanes();
     /* **************************************** */

     //m1.mesh2Dto3DOriginal();
     m1.mesh3DPoints(); //<<<
	 //m1.setStretchJetMesh(); // MESH TRANSFORM
     m1.setMapping(); //<<<
    #if NUMGLEU == 5
     m1.setMiniElement();
    #else
     m1.setQuadElement();
    #endif
    m1.setSurfaceConfig();
    m1.setInterfaceBC();
    
	if ( strcmp( _frame,"moving") == 0 )
	{
	  m1.setGenericBC(vref);
	  pbc.MountPeriodicVectorsNew();
	}
	else
	{
	  m1.setGenericBC();
	  pbc.MountPeriodicVectorsNew();
	}

    Simulator3D s3(m1,s2);
    s3.applyLinearInterpolation(mOld);
    s2 = s3;
    s2.setSolverPressure(solverP);
    s2.setSolverVelocity(solverV);
    s2.setSolverConcentration(solverC);

    InOut saveEnd(m1,s2); // cria objeto de gravacao
    saveEnd.printMeshReport();
    saveEnd.saveMeshInfo(datFolder);
    
   } // end for
 
  PetscFinalize();
  return 0;
}


