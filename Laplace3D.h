// =================================================================== //
// this is file Laplace3D.h, created at 21-Sep-2011                    //
// maintained by Gustavo Rabello dos Anjos                             //
// e-mail: gustavo.rabello@gmail.com                                   //
// =================================================================== //

#ifndef LAPLACE3D_H
#define LAPLACE3D_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include "Model3D.h"
#include "FEMLinElement3D.h"
#include "Solver.h"
#include "CGSolver.h"
#include "PCGSolver.h"
#include "clVector.h"
#include "clMatrix.h"
#include "clDMatrix.h"
#include "colors.h"
#include "geometry.h"
#include <algorithm>

class Laplace3D
{
 public:
  Laplace3D(); // construtor padrao
  Laplace3D( Model3D &_m ); // construtor 
  Laplace3D( Model3D &_m,Laplace3D &_d ); // construtor 
  //virtual ~Laplace3D(); // destrutor padrao

  void getModel3DAttrib(Model3D &_m);
  void allocateMemoryToAttrib();
  void init();
  void assemble();
  void matMountC();

  void setCRHS();
  void setBC();
  void setUnCoupledCBC();
  void unCoupledC();

  void setSolver(Solver *s);

  void saveVTK( const char* _dir,const char* _filename, int _iter );

  clVector* getCSol();

 private:
  Model3D *m;
  int numVerts,numElems,numNodes;
  int numVertsOld,numElemsOld,numNodesOld;
  vector<real> triEdge;
  clVector *X,*Y,*Z;
  clMatrix *IEN;
  clVector *heaviside;
  clVector cc,idbcc;
  SurfaceMesh *surfMesh;
  Mesh3D *mesh3d;

  list<int> *boundaryVert;

  clMatrix Kc,Mc;
  clMatrix matc,AcTilde,McLumped;
  clDMatrix invC,invMcLumped;
  clVector vcc,convC;
  clVector cTilde,b1c,ipc;
  clVector cSol,cSolOld;

  Solver *solver;
};

#endif
