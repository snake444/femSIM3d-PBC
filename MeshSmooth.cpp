// =================================================================== // 
// this is file MeshSmooth.cpp, created at 20-Ago-2009                   //
// maintained by Gustavo Rabello dos Anjos                             //
// e-mail gustavo.rabello@gmail.com                                    //
// =================================================================== //

#include "MeshSmooth.h"

MeshSmooth::~MeshSmooth(){};

MeshSmooth::MeshSmooth(Model3D &_m,real _dt)
{
 m = &_m;
 uc = m->getUC();
 vc = m->getVC();
 wc = m->getWC();
 pc = m->getPC();
 cc = m->getCC();
 X = m->getX();
 Y = m->getY();
 Z = m->getZ();
 IEN = m->getIEN();
 idbcu = m->getIdbcu();
 idbcv = m->getIdbcv();
 idbcw = m->getIdbcw();
 idbcp = m->getIdbcp();
 numVerts = m->getNumVerts();
 numNodes = m->getNumNodes();
 numElems = m->getNumElems();
 neighbourVert = m->getNeighbourVert();
 surfaceViz = m->getSurfaceViz();
 inVert = m->getInVert();
 surface = m->getSurface();
 //nonSurface = m->getNonSurface();
 uSmooth.Dim(numNodes);
 vSmooth.Dim(numNodes);
 wSmooth.Dim(numNodes);
 uSmoothSurface.Dim(numNodes);
 vSmoothSurface.Dim(numNodes);
 wSmoothSurface.Dim(numNodes);
 dt = _dt;
}

clVector MeshSmooth::compute()
{
 //setSurface();

 stepSmooth();
 //stepSmoothSurface();
 setCentroid();

 clVector aux = uSmooth;
 aux.Append(vSmooth);
 aux.Append(wSmooth);
 return aux;
}

// calcula velocidade da malha em todos os vertices
void MeshSmooth::stepSmooth()
{
 real aux;
 list<int> plist;
 list<int>::iterator vert;
 real xSum,ySum,zSum;
 real size; // numero de elementos da lista
 uSmooth.Dim(numNodes);
 vSmooth.Dim(numNodes);
 wSmooth.Dim(numNodes);

 for (list<int>::iterator it=inVert->begin(); it!=inVert->end(); ++it)
 {
  plist = neighbourVert->at(*it);
  size = plist.size();
  xSum = 0.0;
  ySum = 0.0;
  zSum = 0.0;
  for( vert=plist.begin(); vert != plist.end(); ++vert )
  {
   xSum += X->Get(*vert);
   ySum += Y->Get(*vert);
   zSum += Z->Get(*vert);
  }
  real xAverage = xSum/size; // X medio
  aux = (xAverage - X->Get(*it))/dt; // velocidade USmooth
  uSmooth.Set(*it,aux);

  real yAverage = ySum/size; // Y medio
  aux = (yAverage - Y->Get(*it))/dt; // velocidade VSmooth
  vSmooth.Set(*it,aux);

  real zAverage = zSum/size; // Y medio
  aux = (zAverage - Z->Get(*it))/dt; // velocidade WSmooth
  wSmooth.Set(*it,aux);
//--------------------------------------------------
//   if( *it == 0 )
//   {
//   cout << "-------- " << *it << " --------" << endl;
//   cout << "uSmooth: " << uSmooth.Get(*it) << endl;
//   cout << "vSmooth: " << vSmooth.Get(*it) << endl;
//   cout << "wSmooth: " << wSmooth.Get(*it) << endl;
//   cout << "xAverage: " << xAverage << endl;
//   cout << "yAverage: " << yAverage << endl;
//   cout << "zAverage: " << zAverage << endl;
//   cout << "X: " << X->Get(*it) << endl;
//   cout << "Y: " << Y->Get(*it) << endl;
//   cout << "Z: " << Z->Get(*it) << endl;
//   cout << "dt: " << dt << endl;
//   cout << "--------------------" << endl;
//   }
//-------------------------------------------------- 
 }
} // fecha metodo stepSmooth

void MeshSmooth::stepSmooth(clVector &_uVel,clVector &_vVel,clVector &_wVel)
{
 list<int> plist;
 list<int>::iterator vert;
 real uSum,vSum,wSum;
 real size; // numero de elementos da lista
 uSmooth.Dim(numNodes);
 vSmooth.Dim(numNodes);
 wSmooth.Dim(numNodes);

 for (list<int>::iterator it=inVert->begin(); it!=inVert->end(); ++it)
 {
  plist = neighbourVert->at(*it);
  size = plist.size();
  uSum = 0.0;
  vSum = 0.0;
  wSum = 0.0;
  for( vert=plist.begin(); vert != plist.end(); ++vert )
  {
   uSum += _uVel.Get(*vert);
   vSum += _vVel.Get(*vert);
   wSum += _wVel.Get(*vert);
  }
  uSmooth.Set( *it,uSum/size ); 
  vSmooth.Set( *it,vSum/size ); 
  wSmooth.Set( *it,wSum/size ); 
 }
} // fecha metodo stepSmooth

void MeshSmooth::setCentroid()
{  
 // calculando os valores de uSmooth e vSmooth nos centroides
 // atraves da media dos valores nos vertices
 int v[5];
 real aux;

 for( int mele=0;mele<numElems;mele++ )
 {
  v[0] = (int) IEN->Get(mele,0);
  v[1] = (int) IEN->Get(mele,1);
  v[2] = (int) IEN->Get(mele,2);
  v[3] = (int) IEN->Get(mele,3);

  v[4] = (int) IEN->Get(mele,4);

  aux = ( uSmooth.Get(v[0])+uSmooth.Get(v[1])+
	      uSmooth.Get(v[2])+uSmooth.Get(v[3]) )*0.25;
  uSmooth.Set( v[4],aux );
  aux = ( vSmooth.Get(v[0])+vSmooth.Get(v[1])+
	      vSmooth.Get(v[2])+vSmooth.Get(v[3]) )*0.25;
  vSmooth.Set( v[4],aux );
  aux = ( wSmooth.Get(v[0])+wSmooth.Get(v[1])+
	      wSmooth.Get(v[2])+wSmooth.Get(v[3]) )*0.25;
  wSmooth.Set( v[4],aux );
 }
}

// calcula velocidade tangencial euleriana nos vertices da interface
// atraves da media dos vizinhos.
void MeshSmooth::stepSmoothSurface()
{
 // velocidade da interface eh igual a velocidade do fluido
 // para isso precisa-se encontrar os vertices da interface e impor a
 // velocidade do fluido (calculada pelo Semi-lagrangeano)
 real aux;
 int surfaceNode;
 list<int> plist;
 list<int>::iterator vert;
 real xSum,ySum,zSum;
 real size; // numero de elementos da lista
 uSmooth.Dim(numNodes);
 vSmooth.Dim(numNodes);
 wSmooth.Dim(numNodes);

 // loop nos vertices da interface
 for( int i=0;i<surface->Dim();i++ )
 {
  surfaceNode = surface->Get(i);
  plist = neighbourVert->at(surfaceNode);
  size = plist.size();
  xSum = 0.0;
  ySum = 0.0;
  zSum = 0.0;
  for( vert=plist.begin(); vert != plist.end(); ++vert )
  {
   xSum += X->Get(*vert);
   ySum += Y->Get(*vert);
   zSum += Z->Get(*vert);
  }
  real xAverage = xSum/size; // X medio
  aux = (xAverage - X->Get( surfaceNode ))/dt;
  uSmooth.Set( surfaceNode,aux );
//--------------------------------------------------
//   cout << surfaceNode << " " 
//        << xAverage << " "
//        << xAverage-X->Get(surfaceNode) << " "
//        << (xAverage-X->Get(surfaceNode)/dt) << " "
//        << X->Get(surfaceNode) << endl;
//-------------------------------------------------- 

  real yAverage = ySum/size; // Y medio
  aux = (yAverage - Y->Get( surfaceNode ))/dt;
  vSmooth.Set( surfaceNode,aux );

  real zAverage = zSum/size; // Z medio
  aux = (zAverage - Z->Get( surfaceNode))/dt;
  wSmooth.Set( surfaceNode,aux );
 }
} // fecha metodo stepSmoothSurface

// igual a stepSmoothSurface, porem pega apenas os vertices que estao
// localizados na superficie
void MeshSmooth::stepSmoothSurface2()
{
 // velocidade da interface eh igual a velocidade do fluido
 // para isso precisa-se encontrar os vertices da interface e impor a
 // velocidade do fluido (calculada pelo Semi-lagrangeano)
 real aux;
 int surfaceNode;
 list<int> plist;
 list<int>::iterator vert;
 real xSum,ySum,zSum;
 real size; // numero de elementos da lista
 uSmooth.Dim(numNodes);
 vSmooth.Dim(numNodes);
 wSmooth.Dim(numNodes);

 // loop nos vertices da interface
 for( int i=0;i<surface->Dim();i++ )
 {
  surfaceNode = surface->Get(i);
  plist = neighbourVert->at(surfaceNode);
  size = 0.0;
  xSum = 0.0;
  ySum = 0.0;
  zSum = 0.0;
  for( vert=plist.begin(); vert != plist.end(); ++vert )
  {
   if( cc->Get(*vert) == 0.5 )
   {
	xSum += X->Get(*vert);
	ySum += Y->Get(*vert);
	zSum += Z->Get(*vert);
	size++;
   }
  }
  real xAverage = xSum/size; // X medio
  aux = (xAverage - X->Get( surfaceNode ))/dt;
  uSmooth.Set( surfaceNode,aux );

  real yAverage = ySum/size; // Y medio
  aux = (yAverage - Y->Get( surfaceNode ))/dt;
  vSmooth.Set( surfaceNode,aux );

  real zAverage = zSum/size; // Z medio
  aux = (zAverage - Z->Get( surfaceNode ))/dt;
  wSmooth.Set( surfaceNode,aux );
 }
} // fecha metodo stepSmoothSurface2

// calcula velocidade tangencial nos vertices da interface
// atraves da media da posicao dos vizinhos.
void MeshSmooth::stepSmoothSurface(clVector &_uVel,
                                   clVector &_vVel,
								   clVector &_wVel)
{
 // velocidade da interface eh igual a velocidade do fluido
 // para isso precisa-se encontrar os vertices da interface e impor a
 // velocidade do fluido (calculada pelo Semi-lagrangeano)
 int surfaceNode;
 list<int> plist;
 list<int>::iterator vert;
 real uSum,vSum,wSum;
 real size; // numero de elementos da lista
 uSmooth.Dim(numNodes);
 vSmooth.Dim(numNodes);
 wSmooth.Dim(numNodes);

 // loop nos vertices da interface
 for( int i=0;i<surface->Dim();i++ )
 {
  surfaceNode = surface->Get(i);
  plist = neighbourVert->at(surfaceNode);
  size = plist.size();
  uSum = 0.0;
  vSum = 0.0;
  wSum = 0.0;
  for( vert=plist.begin(); vert != plist.end(); ++vert )
  {
   uSum += _uVel.Get(*vert);
   vSum += _vVel.Get(*vert);
   wSum += _wVel.Get(*vert);
  }
  uSmooth.Set( surfaceNode,uSum/size ); 
  vSmooth.Set( surfaceNode,vSum/size ); 
  wSmooth.Set( surfaceNode,wSum/size ); 
 }
} // fecha metodo stepSmoothSurface

void MeshSmooth::setSurface()
{
 int aux;

 // xSurface representa os valores de X dos nos da interface
 // ySurface representa os valores de Y dos nos da interface
 xSurface.Dim(surface->Dim());
 ySurface.Dim(surface->Dim());
 zSurface.Dim(surface->Dim());
 for( int i=0;i<surface->Dim();i++ )
 {
  aux = surface->Get(i);
  xSurface.Set(i,X->Get(aux));
  ySurface.Set(i,Y->Get(aux));
  zSurface.Set(i,Z->Get(aux));
 }
}

int MeshSmooth::search(int node,real _XI, real _YI, real _ZI)
{
 list<int> plist;
 list<int>::iterator vert;
 real dist, dmin;
 plist = surfaceViz->at(node);

 // o primeiro elemento da lista eh a coordenada de trabalho
 vert=plist.begin();
 ++vert; // indo para o 2o. elemento da lista

 // pega o 2o. elemento da lista para comparar
 dmin = (_XI-X->Get(*vert))*(_XI-X->Get(*vert))+
        (_YI-Y->Get(*vert))*(_YI-Y->Get(*vert))+
        (_ZI-Z->Get(*vert))*(_ZI-Z->Get(*vert));

 int vmin=*vert; // atribuindo o vmin do primeiro elemento
 //cout << "vmin1: " << vmin << endl;

 ++vert; // indo para o 3o. elemento da lista
 //cout << "vmin2: " << *vert << endl;

 for( vert=vert; vert != plist.end(); ++vert )
 {
 dist = (_XI-X->Get(*vert))*(_XI-X->Get(*vert))+
        (_YI-Y->Get(*vert))*(_YI-Y->Get(*vert))+
        (_ZI-Z->Get(*vert))*(_ZI-Z->Get(*vert));

  if (dist<dmin)
  {
   dmin=dist;
   vmin=*vert; 
  }
 }
 return vmin;
}

clVector* MeshSmooth::getUSmooth(){ return &uSmooth; }
clVector* MeshSmooth::getVSmooth(){ return &vSmooth; }
clVector* MeshSmooth::getWSmooth(){ return &wSmooth; }


