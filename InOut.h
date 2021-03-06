/**=========================================================================
 *\file Id:InOut.h, GESAR
 *\author Gustavo Rabello dos Anjos
 *\date   23-ago-2007
 *\comment Classe InOut para o modulo HYDRO
 *\references
 *\version 1.0
 *\updates
 	version    date         author             comment
	1.0        23/08/2007   Gustavo            implementacao da classe
 =========================================================================**/

#ifndef INOUT_H
#define INOUT_H

#include "Model3D.h"
#include "Simulator3D.h"
#include "SemiLagrangean.h"
#include "clMatrix.h"
#include "clDMatrix.h"
#include "clVector.h"
#include "PCGSolver.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include "colors.h" 
#include <unistd.h>
#include "interpolations.h" 
#include "searchInterp3D.h" 

/**
 * @brief classe responsavel pela manipulacao de entrada e saida de
 * dados em arquivos ASCII e binario.
 *
 * @return 
 **/
class InOut
{
 public:
  InOut( Model3D &_m );
  InOut( Model3D &_m,Simulator3D &_s );
  virtual ~InOut();

  /**
   * @brief grava arquivo ASCII da solucao do sistema, ou seja, grava
   * os valores de todas matrizes e vetores criadas na inicializacao do
   * simuldador (metodo init) e vetores solucao
   *
   * @param _s objeto do tipo Simulador3D para resgatar os valores
   * nodais dos vetores e matrizes 
   * @param _dir diretorio onde serao gravados os arquivos
   *
   * @return 
   **/
  void saveTXT( const char* _dir,const char* _filename, int _iter );

  /**
   * @brief grava arquivo ASCII da solucao do sistema, ou seja, grava
   * apenas os valores que sao modificados com os passos de tempo. Os
   * vetores sao: velocidade e pressao
   *
   * @param _s objeto do tipo Simulador3D para resgatar os valores
   * nodais dos vetores solucao do problema
   * @param _dir diretorio onde serao gravados os arquivos
   * @param iter numero da iteracao  
   *
   * @return 
   **/
  void saveSolTXT( const char* _dir,const char* _filename, int _iter );

  void saveSol( const char* _dir,const char* _filename,int _iter );
  void loadSol( const char* _dir,const char* _filename,int _iter );

  /**
   * @brief grava arquivo do tipo ASCII da matriz passada como argumento 
   *
   * @param _matrix matriz a ser gravada em arquivo
   *
   * @param _filename campo para inclusao do diretorio + raiz do nome
   *
   * @return 
   **/
  void saveMatrix( clMatrix &_matrix,const char* _filename,string& _filename2 );

  /**
   * @brief grava arquivo do tipo VTK para visualizacao da solucao do
   * problema. Este arquivo inclui a malha utilizada, um campo para a
   * solucao do sistema (U,V) e um campo para a inclusao de um escalar,
   * neste caso o valores nodais da pressao
   *
   * @param _s objeto do tipo Simulador3D para resgatar os valores
   * nodais dos vetores solucao do problema
   * @param _filename campo para inclusao do diretorio + raiz do nome
   *
   * @return 
   **/
  void saveMSH( const char* _dir,const char* _filename );
  void saveVTK( const char* _dir,const char* _filename );
  void saveVTKSurface( const char* _dir,const char* _filename );

  /**
   * @brief grava arquivo do tipo VTK para visualizacao da solucao do
   * problema. Este arquivo inclui a malha utilizada, um campo para a
   * solucao do sistema (U,V) e um campo para a inclusao de um escalar,
   * neste caso o valores nodais da pressao
   *
   * @param _m objeto do tipo Model3D para resgatar os valores
   * nodais dos vetores solucao do problema
   * @param _s objeto do tipo Simulador3D para resgatar os valores das
   * solucoes
   * @param _filename campo para inclusao do diretorio + raiz do nome
   *
   * @return 
   **/
  void saveVTK( const char* _dir,const char* _filename, int _iter );
  void saveMSH( const char* _dir,const char* _filename, int _iter );
  void saveVTKSurface( const char* _dir,const char* _filename, int _iter );
  void saveVTKSurfacePBC( const char* _dir,const char* _filename, int _iter, double _betaGrad );
  void saveVTK( const char* _coord1, const char* _coord2,
                const char* _dir, const char* _filename, 
				int _iter );
  void saveVTU( const char* _dir,const char* _filename, int _iter );

  /**
   * @brief imprime em arquivo ASCII a visualizacao de uma matriz
   * qualquer do tipo clMatrix. Este metodo imprimi '-' em valores
   * iguais a 0 e 'X' em valores diferentes de zero. Sua melhor
   * utilizacao eh direcionada a malhas bem pequenas.
   *
   * @param _m matriz do tipo clMatriz para impressao
   * @param _filename no do arquivo a ser criado com a matriz impressa.
   *
   * @return 
   **/
  void matrixPrint( clMatrix &_m,const char* _filename );

  /**
   * @brief imprime em arquivo ASCII a visualizacao de uma matriz
   * qualquer do tipo clDMatrix. Este metodo imprimi '-' em valores
   * iguais a 0 e 'X' em valores diferentes de zero. Sua melhor
   * utilizacao eh direcionada a malhas bem pequenas.
   *
   * @param _m matriz do tipo clDMatriz para impressao
   * @param _filename no do arquivo a ser criado com a matriz impressa.
   *
   * @return 
   **/
  void matrixPrint( clDMatrix &_m,const char* _filename );

  void saveVonKarman(const char* _dir,const char* _filename);
  void saveVonKarman(Model3D &_m,const char* _dir,
	                 const char* _filename);
  void saveDiskError(const char* _dir,
                     const char* _filename,
					 const char* _fileAnalytic);
  void saveDiskRadiusError(const char* _dir,
                           const char* _filename,
						   const char* _fileAnalytic);
  void savePert( const char* _dir,const char* _filename,int _iter, int vertice);

  void saveVortX(const char* _dir,const char* _filename,int _iter);
  void saveVortY(const char* _dir,const char* _filename,int _iter);
  void saveVortZ(const char* _dir,const char* _filename,int _iter);
  void saveTime(const char* _comment);
  void saveSimTime(int _iter);
  void saveSimTime( const char* _dir,const char* _filename, int _iter );
  int loadIter();
  int loadIter( const char* filename );
  void saveInfo(const char* _dir,const char* _filename,const char* _mesh);
  void printInfo(const char* _mesh);
  void oscillating(int point1,int point2,int point3,const char* _filename);
  void oscillatingD(int point1,int point2,int point3,int point4,
	                int point5,int point6,const char* _filename);
  void oscillating(const char* _dir,const char* _filename, int _iter);
  void oscillatingD(const char* _dir,const char* _filename, int _iter);
  void oscillatingKappa(const char* _dir,const char* _filename, int _iter);
  void bubblesDistance(const char* _dir,const char* _filename,int _iter);
  void saveMeshInfo(const char* _dir);
  void saveConvergence(const char* _dir,const char* _filename);
  void chordalPressure( const char* _dir,const char* _filename, int _iter );
  void crossSectionalPressure( const char* _dir,const char* _filename, int _iter );
  void crossSectionalPlane(const char* _dir,const char* _filename, int _iter);

  /* Save bubble error data */
  void saveBubbleInfo(const char* _dir);
  void saveKappaErrorSphere(const char* _dir);
  void saveKappaErrorCylinder(const char* _dir);
  void saveKappaErrorHyperboloid(const char* _dir);
  void saveKappaErrorTorus(const char* _dir);
  void savePressureError(const char* _dir);
  void saveVolumeError(const char* _dir);
  void saveVolumeCorrection(const char* _dir);
  void saveOscillatingError(const char* _dir);
  void saveTimeError(const char* _dir);
  void saveParasiticCurrent(const char* _dir);
  void copyLastFile(const char* _dir,const char* _filename,const char* _name);

  /* VTK Building Tools  */
  void setCutPlane(ofstream& _file);
  void vtkHeader(ofstream& _file);
  void vtkHeader(ofstream& _file,int _iter);
  void vtkCoords(ofstream& _file);
  void vtkSurfaceCoords(ofstream& _file);
  void vtkCellArray(ofstream& _file);
  void vtkCellType(ofstream& _file);
  void vtkScalarHeader(ofstream& _file);
  void vtkSurfaceScalarHeader(ofstream& _file);
  void vtkSurfaceCellHeader(ofstream& _file);
  void vtkScalar(ofstream& _file,string _name,clVector &_scalar);
  void vtkScalar(ofstream& _file,string _name,clDMatrix &_scalar);
  void vtkSurfaceScalar(ofstream& _file,string _name,clVector &_scalar);
  void vtkSurfaceScalar(ofstream& _file,string _name,clDMatrix &_scalar);
  void vtkScalarCellHeader(ofstream& _file);
  void vtkScalarCell(ofstream& _file,string _name,clVector &_scalar);
  void vtkScalarCell(ofstream& _file,string _name,clDMatrix &_scalar);
  void vtkVector(ofstream& _file,string _name,clVector &_v);
  void vtkVector(ofstream& _file,string _name,clVector &_vx,clVector &_vy,clVector &_vz);
  void vtkSurfaceVector(ofstream& _file,string _name,clVector &_v);
  void vtkSurfaceVector(ofstream& _file,string _name,clVector &_vx,clVector &_vy,clVector &_vz);
  void vtkSurfaceNormalVector(ofstream& _file,string _name,clVector &_v);
  void vtkSurfaceNormalVector(ofstream& _file,string _name,clVector &_vx,clVector &_vy,clVector &_vz);
  void vtkSurfaceCellNormalVector(ofstream& _file,string _name);
  void vtkSurfaceCellNormalVector(ofstream& _file,string _name,
                                  clVector &_vx,clVector &_vy,clVector &_vz);
  void printMeshReport();
  void printSimulationReport();
  void bubbleWallDistance( const char* _dir,const char* _filename, int _iter );
  void saveFilmThickness(const char* _dir);
  void savePoint( const char* _dir,int _point );
  void saveBubbleShapeFactors(const char* _dir,const char* _filename, int _iter); ///<<< 
  void saveTaylorVortexError(const char* _dir);
  void saveBetaPressLiq(const char* _dir);

  void crossSectionSol( const char* _var,const char* _plane,
	                    double _fraction,const char* _dir,
						const char* _filename,int _iter );
private:
  Model3D *m;
  int numVerts,numNodes,numElems;
  clVector *X,*Y,*Z;
  clVector *uc,*vc,*wc,*pc,*cc;
  clVector *idbcu,*idbcv,*idbcw,*idbcp,*idbcc;
  clVector *surface,*outflow;
  clMatrix *IEN;
  SurfaceMesh *surfMesh;
  clVector *interfaceDistance;
  vector<double> triEdge;
  clVector *heaviside,*edgeSize;
  clVector *elemIdRegion,*vertIdRegion;
  list<int> *inElem,*outElem;
  vector< list<int> >* neighbourPoint;
  vector<double> averageTriLength;
  // mesh indexes
  vector<int> isp,ispc,rsp,rspn,rspc,ip,ipd; 
  vector<int> rp,rpi,rpd,rpdist,rph,rpv,csp,flip; 
  vector<int> spc,spp;
  vector<int> idMaxVolume,idMinVolume,intet;
  vector<double> maxVolume,minVolume;
  vector<int> idMaxArea,idMinArea;
  vector<double> maxArea,minArea;
  vector<double> maxLength,minLength;
  vector<int> numSurfElems,numSurfVerts;
  vector<double> initSurfaceVolume,initSurfaceArea,initSurfaceRadius;
  vector<double> surfaceVolume,surfaceArea,surfaceRadius;
  vector<double> errorVolume,errorArea;
  vector<double> dVolume,dArea;

  Simulator3D *s;
  double Re,Sc,We,Fr,alpha,beta,simTime;
  double dt,cfl,dtGravity,dtSurfaceTension;
  double dtSemiLagrangian,dtLagrangian;
  double mu_in,mu_out,rho_in,rho_out,sigma;
  double uRef,vRef,wRef,xRef,yRef,zRef;
  clVector *uAnt,*cAnt;
  clMatrix *M,*K,*G,*D,*gx,*gy,*gz;
  clVector *uSol,*vSol,*wSol,*pSol,*cSol;
  clVector *uSolOld,*vSolOld,*wSolOld,*pSolOld,*cSolOld;
  clVector *uALE,*vALE,*wALE;
  clVector *fint,*heatFlux;
  clDMatrix *kappa;
  clVector *mu,*rho;
  clVector *hSmooth,*gravity;
  clVector *betaFlowLiq; //<<< PBC
  int iter;
  double c1,c2,c3,d1,d2;
};



#endif /* ifndef INOUT_H */

