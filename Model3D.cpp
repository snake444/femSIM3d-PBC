// =================================================================== //
// this is file Model3D.cpp, created at 23-Ago-2007                    //
// maintained by Gustavo Rabello dos Anjos                             //
// e-mail: gustavo.rabello@gmail.com                                   //
// =================================================================== //


#include "Model3D.h"
#include "compare.h"
#include "tetgen.h"

using namespace std;

Model3D::Model3D(){}
Model3D::~Model3D(){}

void Model3D::readVTK( const char* filename )
{
 char auxstr[255];
 real coords[3];
 int i,j,k,idnv;
 int auxvtx[4];

 ifstream vtkFile( filename,ios::in );

 if( !vtkFile )
 {
  cerr << "Esta faltando o arquivo de Malha!" << endl;
  exit(1);
 }

 while( ( !vtkFile.eof())&&(strcmp(auxstr,"POINTS") != 0) )
  vtkFile >> auxstr;

 if( !vtkFile.eof())
 {
  vtkFile >> numVerts;
  vtkFile >> auxstr;
  
  X.Dim(numVerts);
  Y.Dim(numVerts);
  Z.Dim(numVerts);
  
  for (i=0; i < numVerts; i++)
  {
   for(j = 0; j < 3; j++)
	vtkFile >> coords[j];
   
   X.Set(i,coords[0]);
   Y.Set(i,coords[1]);
   Z.Set(i,coords[2]);
  }

  while( (! vtkFile.eof())&&(strcmp(auxstr,"CELLS") != 0) )
   vtkFile >> auxstr;

  if( !vtkFile.eof() )
  {
   vtkFile >> numElems;
   vtkFile >> auxstr;

   IEN.Dim(numElems,4);

   for( i=0; i < numElems; i++ )
   {
	vtkFile >> idnv;
	for( j=0; j < 4 ; j++ )
	{
	 vtkFile >> k;
	 IEN.Set(i,j,k);
	 auxvtx[j] = k;
	}
   }
  }
 }
 vtkFile.close();
} // fim do metodo vtkRead

void Model3D::readVTKCC( const char* filename )
{
 char auxstr[255];
 real fl;
 cc.Dim(numVerts);

 ifstream vtkFile( filename,ios::in );

 if( !vtkFile )
 {
  cerr << "Esta faltando o arquivo de leitura de CC!" << endl;
  exit(1);
 }

 while( (! vtkFile.eof())&&(strcmp(auxstr,"concentration") != 0) )
  vtkFile >> auxstr;

 if( !vtkFile.eof() )
 {
  vtkFile >> auxstr;
  vtkFile >> auxstr;
  vtkFile >> auxstr;

  for( int i=0; i < numVerts; i++ )
  {
   vtkFile >> fl;
   cc.Set(i,fl);
  }
 }
 vtkFile.close();
} // fim do metodo vtkRead

void Model3D::readVTKSurface( const char* filename )
{
 char auxstr[255];
 real coords[3];
 int i,j,k,idnv;
 int auxvtx[4];

 ifstream vtkFile( filename,ios::in );

 if( !vtkFile )
 {
  cerr << "Esta faltando o arquivo de Malha!" << endl;
  exit(1);
 }

 while( ( !vtkFile.eof())&&(strcmp(auxstr,"POINTS") != 0) )
  vtkFile >> auxstr;

 if( !vtkFile.eof())
 {
  vtkFile >> numVerts;
  vtkFile >> auxstr;
  
  X.Dim(numVerts);
  Y.Dim(numVerts);
  Z.Dim(numVerts);
  
  for (i=0; i < numVerts; i++)
  {
   for(j = 0; j < 3; j++)
	vtkFile >> coords[j];
   
   X.Set(i,coords[0]);
   Y.Set(i,coords[1]);
   Z.Set(i,coords[2]);
  }

  while( (! vtkFile.eof())&&(strcmp(auxstr,"CELLS") != 0) )
   vtkFile >> auxstr;

  if( !vtkFile.eof() )
  {
   vtkFile >> numElems;
   vtkFile >> auxstr;

   IEN.Dim(numElems,4);

   for( i=0; i < numElems; i++ )
   {
	vtkFile >> idnv;
	for( j=0; j < 3 ; j++ )
	{
	 vtkFile >> k;
	 IEN.Set(i,j,k);
	 auxvtx[j] = k;
	}
   }
  }
 }
 vtkFile.close();
} // fim do metodo vtkRead

void Model3D::readMSH( const char* filename )
{
 char auxstr[255];
 real coords[3];
 int i,j,k,id;
 int auxvtx[4];
 int elemNumber,type,numberOfTags;

 ifstream mshFile( filename,ios::in );

 if( !mshFile )
 {
  cerr << "Esta faltando o arquivo de Malha!" << endl;
  exit(1);
 }

//--------------------------------------------------
//  int numberOfPhyNames;
//  while( ( !mshFile.eof())&&(strcmp(auxstr,"$PhysicalNames") != 0) )
//   mshFile >> auxstr;
// 
//  mshFile >> numberOfPhyNames;
// 
//  idRegion.resize(numberOfPhyNames);
//  if( ( !mshFile.eof())&&(strcmp(auxstr,"$EndPhysicalNames") != 0) )
//  {
//   for (i=0; i < numberOfPhyNames; i++)
//   {
//    mshFile >> auxstr;
//    mshFile >> auxstr;
//    mshFile >> auxstr;
//    idRegion.at(i)=auxstr;
//   }
//  }
//-------------------------------------------------- 

 while( ( !mshFile.eof())&&(strcmp(auxstr,"$Nodes") != 0) )
  mshFile >> auxstr;

 if( !mshFile.eof()&&(strcmp(auxstr,"$EndNodes") != 0)   )
 {
  mshFile >> numVerts;

  X.Dim(numVerts);
  Y.Dim(numVerts);
  Z.Dim(numVerts);

  for (i=0; i < numVerts; i++)
  {
   mshFile >> auxstr;
   for(j = 0; j < 3; j++)
	mshFile >> coords[j];

   X.Set(i,coords[0]);
   Y.Set(i,coords[1]);
   Z.Set(i,coords[2]);
  }
 }

 while( (!mshFile.eof())&&(strcmp(auxstr,"$Elements") != 0) )
  mshFile >> auxstr;

 if( !mshFile.eof()&&(strcmp(auxstr,"$EndElements") != 0)   )
 {
  mshFile >> numElems;

  IEN.Dim(numElems,4);
  idRegion.Dim(numElems);

  for( i=0; i < numElems; i++ )
  {
   mshFile >> elemNumber;
   mshFile >> type; // 2-2D or 3-3D
   mshFile >> numberOfTags;
   mshFile >> id;
   idRegion.Set(i,id);
   mshFile >> auxstr;
   mshFile >> auxstr;

   for( j=0; j < type+1 ; j++ )
   {
	mshFile >> k;
	k=k-1; // elem .msh comecando com 1
	IEN.Set(i,j,k);
	auxvtx[j] = k;
   }
//--------------------------------------------------
//    if( region == 1 ) // 1 = bubble
//    {
// 	int v1 = IEN.Get(i,0);
// 	int v2 = IEN.Get(i,1);
// 	int v3 = IEN.Get(i,2);
// 	cc.Set(v1,0.5);
// 	cc.Set(v2,0.5);
// 	cc.Set(v3,0.5);
//    }
//-------------------------------------------------- 
  }
 }
 mshFile.close();
} // fim do metodo readMsh

void Model3D::setInterfaceBC()
{
 cc.Dim(numVerts);
 for( int i=0;i<numElems;i++ )
 {
  if( idRegion.Get(i) == 1 )
  {
   int v1 = IEN.Get(i,0);
   int v2 = IEN.Get(i,1);
   int v3 = IEN.Get(i,2);
   real aux = 0.5;
   cc.Set(v1,aux);
   cc.Set(v2,aux);
   cc.Set(v3,aux);
  }
 }
}

/**
 * @brief metodo para leitura de arquivo do tipo BC para condicoes de
 * contorno. O arquivo a ser lido deve conter todos os nos de condicao
 * de contorno contendo colunas de vertices, colunas de u,v e p com
 * numeracao 1 para condicao do tipo Dirichlet e 2 para Newmann e seu
 * respectivo valor
 *
 * @return verdadeiro ou falso dependendo do sucesso da leitura
 **/
void Model3D::readBC( const char* filename )
{

 char auxstr[255];
 real coords[9];
 int i,j,numVertsBC;

 ifstream bcFile( filename,ios::in );

 if( bcFile == NULL )
 {
  cerr << "Esta faltando o arquivo de condicao de contorno!" << endl;
  exit(1);
 }

 while( ( !bcFile.eof())&&(strcmp(auxstr,"BC_DATA") != 0) )
  bcFile >> auxstr; // atribui BC_DATA
 
 if( !bcFile.eof())
 {
  bcFile >> auxstr;
  bcFile >> numVertsBC;

  for (i=0; i < numVertsBC; i++)
  {
   for(j = 0; j < 9; j++)
	bcFile >> coords[j];
   
   if( coords[1] == 1 )
   {
	idbcu.AddItem( (int) coords[0]);
	uc.Set( (int) coords[0],coords[2]);
   }

   if( coords[3] == 1 )
   {
	idbcv.AddItem(coords[0]);
	vc.Set( (int) coords[0],coords[4]);
   }

   if( coords[5] == 1 )
   {
	idbcw.AddItem(coords[0]);
	wc.Set( (int) coords[0],coords[6]);
   }

   if( coords[7] == 1 )
   {
	idbcp.AddItem(coords[0]);
	pc.Set( (int) coords[0],coords[8]);
	outflow.Set( (int) coords[0], 0);
   }

  }
 }
} // fim do metodo readBC

// este metodo cria os pontos de forma ordenada e igualmente espacada e
// depois utiliza a biblioteca tetgen para gerar a tetraedralizacao
// seguida pela atualizacao da matriz de mapeamento de elementos IEN
void Model3D::setMeshStep(int nX,int nY,int nZ)
{
 tetgenio in,out;
 in.mesh_dim = 3;
 in.numberofpoints = nX*nY*nZ;
 in.pointlist = new REAL[in.numberofpoints * 3];

 real dx = nX/(nX-1.0);
 real dy = nY/(nY-1.0);
 real dz = nZ/(nZ-1.0);
 real aux;
 numVerts = nX*nY*nZ;
 X.Dim(numVerts);
 Y.Dim(numVerts);
 Z.Dim(numVerts);
 outflow.Dim(numVerts);

 int count = 0;
 for( int i=0;i<nX;i++ )
 {
  for( int j=0;j<nY;j++ )
  {
   for( int k=0;k<nZ;k++)
   {
	aux = i*dx;
	X.Set(count,aux);
	in.pointlist[3*count+0] = aux;
	aux = j*dy;
	Y.Set(count,aux);
	in.pointlist[3*count+1] = aux;
	aux = k*dz;
	Z.Set(count,aux);
	in.pointlist[3*count+2] = aux;
	count++;
   }
  }
 }

 cout << endl;
 cout << "----> meshing... ";
 tetrahedralize( (char*) "Q",&in,&out,NULL,NULL );
 cout << "finished <---- " << endl;;
 cout << endl;

 //out.save_elements("out");
 //out.save_nodes("out");
 numElems = out.numberoftetrahedra;
 IEN.Dim(numElems,4);

 // varre lista de elementos e passa para estrutura IEN
 for( int i=0;i<out.numberoftetrahedra;i++ )
 {
  for( int j=0;j<4;j++ )
  {
   int vertice = out.tetrahedronlist[i*4+j];
   IEN.Set(i,j,vertice);
  }
 }
}

void Model3D::setStepBC()
{
 for( int i=0;i<numVerts;i++ )
 {
  if( (X.Get(i)==X.Min()) || (Y.Get(i)==Y.Min()) || (Y.Get(i)==Y.Max()) )
  {
   idbcu.AddItem(i);
   idbcv.AddItem(i);

   uc.Set(i,0.0);
   vc.Set(i,0.0);
   if( (X.Get(i)==X.Min()) && (Y.Get(i)>(Y.Max()/2.0)) && (Y.Get(i)<Y.Max()) )
   {

	uc.Set(i,1.0);
   }
  }
  if( (Z.Get(i)==Z.Min()) || (Z.Get(i) == Z.Max()) ||
	  (X.Get(i)==X.Min()) || (Y.Get(i)==Y.Min()) || (Y.Get(i)==Y.Max()) )
  {
   idbcw.AddItem(i);
   wc.Set(i,0.0);
  }
  if( X.Get(i)==X.Max())
  {
   idbcp.AddItem(i);
   pc.Set(i,0.0);
   outflow.Set(i,0);
  }
 }
}

void Model3D::setCStepBC()
{
 cc.Dim(numVerts);
 idbcc.Dim(0);
 for( int i=0;i<numVerts;i++ )
 {
  if( (X.Get(i)==X.Min()) || (Y.Get(i)==Y.Min()) || (Y.Get(i)==Y.Max()) )
  {
   if( (X.Get(i)==X.Min()) && (Y.Get(i)>(Y.Max()/2.0)) && (Y.Get(i)<Y.Max()) )
   {
	idbcc.AddItem(i);

	cc.Set(i,1.0);
   }
  }
 }
}

void Model3D::setStepReservBC()
{
 for( int i=0;i<numVerts;i++ )
 {
  if( (X.Get(i)==X.Min()) || (Y.Get(i)==Y.Min()) || (Y.Get(i)==Y.Max()) 
	  || Z.Get(i)==Z.Min() )
  {
   idbcu.AddItem(i);
   idbcv.AddItem(i);
   idbcw.AddItem(i);

   uc.Set(i,0.0);
   vc.Set(i,0.0);
   wc.Set(i,0.0);
   if( (X.Get(i)==X.Min()) && (Y.Get(i)>(Y.Max()/2.0)) && (Y.Get(i)<Y.Max())  
	    && Z.Get(i)>Z.Min() )
	uc.Set(i,1.0);
  }
  if( Z.Get(i) == Z.Max() && X.Get(i)<X.Max() )
  {
   idbcw.AddItem(i);
   wc.Set(i,0.0);
  }
  if( X.Get(i)==X.Max() && Z.Get(i)>Z.Min() )
  {
   idbcp.AddItem(i);
   pc.Set(i,0.0);
   outflow.Set(i,0);
  }
 }
}

void Model3D::setStepReservInvBC()
{
 for( int i=0;i<numVerts;i++ )
 {
  if( (X.Get(i)==X.Max()) || (Y.Get(i)==Y.Min()) || (Y.Get(i)==Y.Max()) 
	  || Z.Get(i)==Z.Min() )
  {
   idbcu.AddItem(i);
   idbcv.AddItem(i);
   idbcw.AddItem(i);

   uc.Set(i,0.0);
   vc.Set(i,0.0);
   wc.Set(i,0.0);
   if( (X.Get(i)==X.Max()) && (Y.Get(i)>(Y.Max()/2.0)) && (Y.Get(i)<Y.Max())  
	    && Z.Get(i)>Z.Min() )
	uc.Set(i,-1.0);
  }
  if( Z.Get(i) == Z.Max() && X.Get(i)>X.Min() )
  {
   idbcw.AddItem(i);
   wc.Set(i,0.0);
  }
  if( X.Get(i)==X.Min() && Z.Get(i)>Z.Min() )
  {
   idbcp.AddItem(i);
   pc.Set(i,0.0);
   outflow.Set(i,0);
  }
 }
}

void Model3D::setCouetteBC()
{
 for( int i=0;i<numVerts;i++ )
 {
  if( (X.Get(i)==X.Min()) || (Y.Get(i)==Y.Min()) || (Y.Get(i)==Y.Max()) )
  {
   idbcu.AddItem(i);
   idbcv.AddItem(i);
   idbcw.AddItem(i);

   uc.Set(i,0.0);
   vc.Set(i,0.0);
   wc.Set(i,0.0);
   if( (X.Get(i)==X.Min()) && (Y.Get(i)>Y.Min()) && (Y.Get(i)<Y.Max()) )
	uc.Set(i,1.0);
  }
  if( (Z.Get(i)==Z.Min()) || (Z.Get(i) == Z.Max()) )
  {
   idbcw.AddItem(i);
   wc.Set(i,0.0);
  }
  if( X.Get(i)==X.Max())
  {
   idbcp.AddItem(i);
   pc.Set(i,0.0);
   outflow.Set(i,0);
  }
 }
}

void Model3D::setAdimenStep()
{
 real aux;
 real factor = 1.0/(Y.Max()-Y.Min());

 for( int i=0;i<numVerts;i++ )
 {
  aux = (X.Get(i)-X.Min())*factor;
  X.Set(i,aux);
  aux = (Y.Get(i)-Y.Min())*factor;
  Y.Set(i,aux);
  aux = (Z.Get(i)-Z.Min())*factor;
  Z.Set(i,aux);
 }
}

void Model3D::setMeshDisk(int nLados1Poli,int nCircMax,int nZ)
{
 real dr = 1;
 real  r = dr;
 int j = 0;
 real z = 0;
 real dl = ( (2*3.141592)/nLados1Poli)*dr;
 real theta,dTheta,dz;
 real aux;
 //clVector xCirc(1+nLados1Poli*nCircMax!-fatorial);
 //clVector yCirc(1+nLados1Poli*nCircMax!-fatorial);
 clVector xCirc;
 clVector yCirc;
 clVector xAux,yAux,zAux;
 X.Dim(1000);
 Y.Dim(1000);
 Z.Dim(1000);

 xCirc.AddItem(j,0);
 yCirc.AddItem(j,0);
 j++;

 for( int nCirc=1;nCirc<=nCircMax;nCirc++ )
 {
  theta = 0.0;
  dTheta = (dl/nCirc)*dr;
  for( int jj=1;jj<=(nLados1Poli*nCirc);jj++ )
  {
   aux = r*cos(theta);
   xCirc.AddItem(j,aux);
   //xCirc.Set(j,aux);
   aux = r*sin(theta);
   yCirc.AddItem(j,aux);
   //yCirc.Set(j,aux);
   theta = theta + dTheta;
   j++;
   if( theta >= 2*3.141592 ) break;
  }
  r=r+dr;
 }

 rMax = r - dr;
 j=0;
 z=0;
 ///////real factor = 1.1 + 1.0/nZ;
 //real factor = 1.02 + 1.0/nZ;
 for( int jz=1;jz<=nZ;jz++ )
 {
  //if( jz<=nZ/2 ) dz=0.1;
  //else 
  //if( exp(z/nZ) <= exp(2.0/3.0) ) dz=exp(z/nZ);
  //else dz = exp(2.0/3.0);
  //else dz=0.1;
  ////////dz = (factor*exp(z/nZ));
  dz = 0.1;
  //cout << z  << " " << nZ << " " << dz << endl;
  for( int jCirc=1;jCirc<=xCirc.Dim();jCirc++ )
  {
   if( j == X.Dim() ) 
   {
	xAux = X;
	yAux = Y;
	zAux = Z;
	X.Dim(j+X.Dim());
	Y.Dim(j+Y.Dim());
	Z.Dim(j+Z.Dim());
	X.CopyFrom(0,xAux);
	Y.CopyFrom(0,yAux);
	Z.CopyFrom(0,zAux);
   }
   aux = xCirc.Get(jCirc-1);
   X.Set(j,aux);
   aux = yCirc.Get(jCirc-1);
   Y.Set(j,aux);
   Z.Set(j,z);
   j++;
  }
  z=z+dz;
 }
 xAux = X;
 yAux = Y;
 zAux = Z;
 numVerts = j; 
 X.Dim(numVerts);
 Y.Dim(numVerts);
 Z.Dim(numVerts);
 xAux.CopyTo(0,X);
 yAux.CopyTo(0,Y);
 zAux.CopyTo(0,Z);

 tetgenio in,out;
 in.mesh_dim = 3;
 in.numberofpoints = numVerts;
 in.pointlist = new REAL[in.numberofpoints * 3];

 for( int i=0;i<numVerts;i++ )
 {
  in.pointlist[3*i+0] = X.Get(i);
  in.pointlist[3*i+1] = Y.Get(i);
  in.pointlist[3*i+2] = Z.Get(i);
 }

 cout << endl;
 cout << "----> meshing... ";
 tetrahedralize( (char*) "Q",&in,&out );
 cout << "finished <---- " << endl;;
 cout << endl;

 //out.save_elements("out");
 //out.save_nodes("out");
 numElems = out.numberoftetrahedra;
 IEN.Dim(numElems,4);

 // varre lista de elementos e passa para estrutura IEN
 for( int i=0;i<out.numberoftetrahedra;i++ )
 {
  for( int j=0;j<4;j++ )
  {
   int vertice = out.tetrahedronlist[i*4+j];
   IEN.Set(i,j,vertice);
  }
 }
}

void Model3D::mesh2Dto3D()
{
 // cria objeto de malha do tetgen
 tetgenio in,out;
 in.mesh_dim = 3;
 in.numberofpoints = numVerts;
 in.pointlist = new REAL[in.numberofpoints * 3];
 in.pointmarkerlist = new int[in.numberofpoints];

 // adiciona na estrutura tetgen as coordenadas dos pontos
 for( int i=0;i<numVerts;i++ )
 {
  in.pointlist[3*i+0] = X.Get(i);
  in.pointlist[3*i+1] = Y.Get(i);
  in.pointlist[3*i+2] = Z.Get(i);
  if( cc.Get(i) == 0.0 )
   in.pointmarkerlist[i] = 11; // fora
  if( cc.Get(i) == 0.5 )
   in.pointmarkerlist[i] = 22; // interface
  if( cc.Get(i) == 1.0 )
   in.pointmarkerlist[i] = 33; // dentro
 }

 /* ESTE PROCEDIMENTO DEFINE REGIOES NA MALHA E APOS A INSERCAO/RETIRADA
  * DE PONTOS PELO TETGEN, CONSEGUIMOS RECONHECER A LOCALIZACAO DOS
  * PONTOS E ASSIM PODEMOS DEFINIR NOVAMENTE A FUNCAO MARCADORA COMO
  * SENDO 1.0 DENTRO DA BOLHA, 0.5 NA SUPERFICIE E 0.0 FORA 
  * E NECESSARIO DEFINIR 1 PONTO EM CADA REGIAO */
 // fluido interior + fluido exterior + superficie
 in.numberofregions = 1; 
 in.regionlist = new REAL[in.numberofregions*4];

 // fora da bolha
 //in.regionlist[0] = X.Min();
 //in.regionlist[1] = Y.Min();
 //in.regionlist[2] = Z.Min();
 in.regionlist[0] = 0.0;
 in.regionlist[1] = 0.0;
 in.regionlist[2] = 0.0;
 in.regionlist[3] = 1;

 tetgenio::facet *f;   // Define a pointer of facet. 
 tetgenio::polygon *p; // Define a pointer of polygon.
 in.numberoffacets = IEN.DimI(); 
 in.facetlist = new tetgenio::facet[in.numberoffacets]; 
 in.facetmarkerlist = new int[in.numberoffacets];
 //in.trifacemarkerlist = new int[in.numberoffacets];

 // definindo a superficie da bolha e convex-hull
 for( int i=0;i<IEN.DimI();i++ )
 {
  int v1 = IEN.Get(i,0);
  int v2 = IEN.Get(i,1);
  int v3 = IEN.Get(i,2);
  f = &in.facetlist[i];
  f->numberofpolygons = 1;
  f->polygonlist = new tetgenio::polygon[f->numberofpolygons];
  f->numberofholes = 0; 
  f->holelist = NULL;
  p = &f->polygonlist[0];
  p->numberofvertices = 3;
  p->vertexlist = new int[p->numberofvertices];
  p->vertexlist[0] = v1; 
  p->vertexlist[1] = v2; 
  p->vertexlist[2] = v3;
  // melhorar esta configuracao de facet para bolha e convex hull
  if( cc.Get(v1) == 0.5 && cc.Get(v2) == 0.5 && cc.Get(v3) == 0.5 )
   in.facetmarkerlist[i] = 10;
  else
   in.facetmarkerlist[i] = 20;
 }

 // malha 3D de superfice triangular (convex hull + interface)
 numVertsOriginal = numVerts;
 IENOriginal = IEN;

 //in.save_poly("bubble");
 //in.save_nodes("bubble");
 //in.save_elements("in");
 cout << "numElems IN = " << numElems << endl;
 cout << "numNodes IN = " << numNodes << endl;
 cout << "numVerts IN = " << in.numberofpoints << endl;

 cout << endl;
 cout << "----> meshing... ";
 tetrahedralize( (char*) "QYYApq1.4241a0.05",&in,&out );
 //tetrahedralize( (char*) "QpYY",&in,&out );
 cout << "finished <---- " << endl;;
 cout << endl;
 
 numElems = out.numberoftetrahedra;
 numNodes = out.numberofpoints+numElems;
 numVerts = out.numberofpoints;
 cout << "numElems OUT = " << out.numberoftetrahedra << endl;
 cout << "numNodes OUT = " << out.numberofpoints+numElems << endl;
 cout << "numVerts OUT = " << out.numberofpoints << endl;
 cout << "numfacets OUT = " << out.numberoftrifaces << endl;
 //out.save_elements("out");
 //out.save_nodes("out");
 //out.save_poly("out");
 //out.save_faces("out");

 // varre lista de elementos e passa para estrutura IEN
 IEN.Dim(numElems,5);
 cc.Dim(numVerts);
 inElem.resize (0);
 outElem.resize (0);
 for( int i=0;i<out.numberoftetrahedra;i++ )
 {
  // setting de cc = 0 para fora da bolha e cc = 0.5 para interface
  if( out.tetrahedronattributelist[i] == 1 )
  {
   outElem.push_back(i);
   for( int j=0;j<4;j++ )
   {
	int vertice = out.tetrahedronlist[i*4+j];
	IEN.Set(i,j,vertice);
	cc.Set(vertice,0.0);
   }
  }
  // setting de cc = 1 para dentro da bolha e cc = 0.5 para interface
  else 
  {
   inElem.push_back(i);
   for( int j=0;j<4;j++ )
   {
	int vertice = out.tetrahedronlist[i*4+j];
	IEN.Set(i,j,vertice);
	cc.Set(vertice,1.0);
   }
  }
 }

 // atualizando valores de X,Y,Z,uc,vc,wc e pc
 X.Dim(numNodes);
 Y.Dim(numNodes);
 Z.Dim(numNodes);
 for( int i=0;i<numVerts;i++ )
 {
  X.Set(i,out.pointlist[3*i+0]);
  Y.Set(i,out.pointlist[3*i+1]);
  Z.Set(i,out.pointlist[3*i+2]);
  if( out.pointmarkerlist[i] == 10 ||
   	  out.pointmarkerlist[i] == 22 )
   cc.Set(i,0.5);
 }
}
 
void Model3D::mesh2Dto3DOriginal()
{
//--------------------------------------------------
//  // insercao de pontos atraves da media dos vizinhos
//  for( int i=0;i<IENOriginal.DimI();i++ )
//   cout << getArea(i) << endl;
// 
//  int count = 0;
//  real test = 0.05;
//  clMatrix IENaux(3,3);
//  for( int i=0;i<IENOriginal.DimI();i++ )
//  {
//   int v1 = IENOriginal.Get(i,0);
//   int v2 = IENOriginal.Get(i,1);
//   int v3 = IENOriginal.Get(i,2);
//   if( cc.Get(v1) == 0.5 && getArea(i) > test )
//   {
//    cout << getArea(i) << endl;
//    int v4 = numVertsOriginal;
// 
//    real centroidX = ( X.Get(v1)+X.Get(v2)+X.Get(v3) )*0.3334;
//    real centroidY = ( Y.Get(v1)+Y.Get(v2)+Y.Get(v3) )*0.3334;
//    real centroidZ = ( Z.Get(v1)+Z.Get(v2)+Z.Get(v3) )*0.3334;
// 
//    X.AddItem(v4,centroidX);
//    Y.AddItem(v4,centroidY);
//    Z.AddItem(v4,centroidZ);
//    cout << v1 << " " << v2 << " " << v3 << " " << v4 << endl;
//    cc.AddItem(v4,0.5); // pertencente a interface
// 
//    IENOriginal.DelLine(i); // delete elemento
//    IENOriginal.AddRow();
//    IENOriginal.AddRow();
//    IENOriginal.AddRow();
//    IENOriginal.Set(IENOriginal.DimI()+0,0,v1);
//    IENOriginal.Set(IENOriginal.DimI()+0,0,v2);
//    IENOriginal.Set(IENOriginal.DimI()+0,0,v4);
//    IENOriginal.Set(IENOriginal.DimI()+1,0,v1);
//    IENOriginal.Set(IENOriginal.DimI()+1,0,v3);
//    IENOriginal.Set(IENOriginal.DimI()+1,0,v4);
//    IENOriginal.Set(IENOriginal.DimI()+2,0,v2);
//    IENOriginal.Set(IENOriginal.DimI()+2,0,v3);
//    IENOriginal.Set(IENOriginal.DimI()+2,0,v4);
// //--------------------------------------------------
// //    IENaux.Set(0,0,v1);IENaux.Set(0,1,v2);IENaux.Set(0,2,v4);
// //    IENaux.Set(1,0,v1);IENaux.Set(1,1,v3);IENaux.Set(1,2,v4);
// //    IENaux.Set(2,0,v2);IENaux.Set(2,1,v3);IENaux.Set(2,2,v4);
// //-------------------------------------------------- 
//    count++;
//   }
//  }
//  numVertsOriginal = numVertsOriginal+count; // update do numVertsOriginal para novo valor
//-------------------------------------------------- 



 // cria objeto de malha do tetgen
 tetgenio in,out;
 in.mesh_dim = 3;
 in.numberofpoints = numVertsOriginal;
 in.pointlist = new REAL[in.numberofpoints * 3];
 in.pointmarkerlist = new int[in.numberofpoints];

 // adiciona na estrutura tetgen as coordenadas dos pontos atuais
 for( int i=0;i<numVertsOriginal;i++ )
 {
  in.pointlist[3*i+0] = X.Get(i);
  in.pointlist[3*i+1] = Y.Get(i);
  in.pointlist[3*i+2] = Z.Get(i);
  if( cc.Get(i) == 0.0 )
   in.pointmarkerlist[i] = 11;
  if( cc.Get(i) == 0.5 )
   in.pointmarkerlist[i] = 22; // mesma id de facetmarker
  if( cc.Get(i) == 1.0 )
   in.pointmarkerlist[i] = 33;
 }

 /* ESTE PROCEDIMENTO DEFINE REGIOES NA MALHA E APOS A INSERCAO/RETIRADA
  * DE PONTOS PELO TETGEN, CONSEGUIMOS RECONHECER A LOCALIZACAO DOS
  * PONTOS E ASSIM PODEMOS DEFINIR NOVAMENTE A FUNCAO MARCADORA COMO
  * SENDO 1.0 DENTRO DA BOLHA, 0.5 NA SUPERFICIE E 0.0 FORA 
  * E NECESSARIO DEFINIR 1 PONTO EM CADA REGIAO */
 // fluido interior + fluido exterior + superficie
 in.numberofregions = 1; 
 in.regionlist = new REAL[in.numberofregions*4];

 // definindo regiao fora da bolha
 //in.regionlist[0] = X.Min();
 //in.regionlist[1] = Y.Min();
 //in.regionlist[2] = Z.Min();
 in.regionlist[0] = 0.0;
 in.regionlist[1] = 0.0;
 in.regionlist[2] = 0.0;
 in.regionlist[3] = 1;

 tetgenio::facet *f;   // Define a pointer of facet. 
 tetgenio::polygon *p; // Define a pointer of polygon.
 in.numberoffacets = IENOriginal.DimI(); 
 in.facetlist = new tetgenio::facet[in.numberoffacets]; 
 in.facetmarkerlist = new int[in.numberoffacets];
 //in.trifacemarkerlist = new int[in.numberoffacets];

 // definindo a superficie da bolha e convex-hull
 for( int i=0;i<IENOriginal.DimI();i++ )
 {
  int v1 = IENOriginal.Get(i,0);
  int v2 = IENOriginal.Get(i,1);
  int v3 = IENOriginal.Get(i,2);
  f = &in.facetlist[i];
  f->numberofpolygons = 1;
  f->polygonlist = new tetgenio::polygon[f->numberofpolygons];
  f->numberofholes = 0; 
  f->holelist = NULL;
  p = &f->polygonlist[0];
  p->numberofvertices = 3;
  p->vertexlist = new int[p->numberofvertices];
  p->vertexlist[0] = v1; 
  p->vertexlist[1] = v2; 
  p->vertexlist[2] = v3;
  // melhorar esta configuracao de facet para bolha e convex hull
  if( cc.Get(v1) == 0.5 && cc.Get(v2) == 0.5 && cc.Get(v3) == 0.5 )
   in.facetmarkerlist[i] = 10;
  else
   in.facetmarkerlist[i] = 20;
 }

 //in.save_poly("bubble");
 //in.save_nodes("bubble");
 //in.save_elements("in");
 cout << "numElems IN = " << numElems << endl;
 cout << "numNodes IN = " << numNodes << endl;
 cout << "numVerts IN = " << in.numberofpoints << endl;

 cout << endl;
 cout << "----> meshing... ";
 //tetrahedralize( (char*) "QYYApq1.4241",&in,&out );
 tetrahedralize( (char*) "QYYApq1.4241a0.05",&in,&out );
 cout << "finished <---- " << endl;;
 cout << endl;

 numElems = out.numberoftetrahedra;
 numNodes = out.numberofpoints+numElems;
 numVerts = out.numberofpoints;
 cout << "numElems OUT = " << out.numberoftetrahedra << endl;
 cout << "numNodes OUT = " << out.numberofpoints+numElems << endl;
 cout << "numVerts OUT = " << out.numberofpoints << endl;
 cout << "numfacets OUT = " << out.numberoftrifaces << endl;
 //out.save_elements("out");
 //out.save_nodes("out");
 //out.save_poly("out");
 //out.save_faces("out");

 // varre lista de elementos e passa para estrutura IEN
 IEN.Dim(numElems,5);
 cc.Dim(numVerts);
 inElem.resize (0);
 outElem.resize (0);
 for( int i=0;i<out.numberoftetrahedra;i++ )
 {
  // setting de cc = 0 para fora da bolha e cc = 0.5 para interface
  if( out.tetrahedronattributelist[i] == 1 )
  {
   outElem.push_back(i);
   for( int j=0;j<4;j++ )
   {
	int vertice = out.tetrahedronlist[i*4+j];
	IEN.Set(i,j,vertice);
	cc.Set(vertice,0.0);
   }
  }
  // setting de cc = 1 para dentro da bolha e cc = 0.5 para interface
  else 
  {
   inElem.push_back(i);
   for( int j=0;j<4;j++ )
   {
	int vertice = out.tetrahedronlist[i*4+j];
	IEN.Set(i,j,vertice);
	cc.Set(vertice,1.0);
   }
  }
 }

 // atualizando valores de X,Y,Z,uc,vc,wc e pc
 X.Dim(numNodes);
 Y.Dim(numNodes);
 Z.Dim(numNodes);
 for( int i=0;i<numVerts;i++ )
 {
  X.Set(i,out.pointlist[3*i+0]);
  Y.Set(i,out.pointlist[3*i+1]);
  Z.Set(i,out.pointlist[3*i+2]);
  if( out.pointmarkerlist[i] == 10 ||
	  out.pointmarkerlist[i] == 22 )
   cc.Set(i,0.5);
 }
}

void Model3D::mesh3DPoints()
{
 // cria objeto de malha do tetgen
 tetgenio in,out;
 in.mesh_dim = 3;
 in.numberofpoints = numVerts;
 in.pointlist = new REAL[in.numberofpoints * 3];
 in.pointmarkerlist = new int[in.numberofpoints];

 /* ------------ pontos da malha separados em 2 loops ------------ */
 // adiciona na estrutura tetgen as coordenadas dos pontos da 
 // superficie e do convex-hull
 for( int i=0;i<numVertsOriginal;i++ )
 {
  in.pointlist[3*i+0] = X.Get(i);
  in.pointlist[3*i+1] = Y.Get(i);
  in.pointlist[3*i+2] = Z.Get(i);
  if( cc.Get(i) == 0.0 )
   in.pointmarkerlist[i] = 11;
  if( cc.Get(i) == 0.5 )
   in.pointmarkerlist[i] = 22;
  if( cc.Get(i) == 1.0 )
   in.pointmarkerlist[i] = 33;
 }

 // adicionando pontos que nao sao da interface e do convex-hull
 for( int i=numVertsOriginal;i<numVerts;i++ )
 {
  in.pointlist[3*i+0] = X.Get(i);
  in.pointlist[3*i+1] = Y.Get(i);
  in.pointlist[3*i+2] = Z.Get(i);
  if( cc.Get(i) == 0.0 )
   in.pointmarkerlist[i] = 11;
  if( cc.Get(i) == 0.5 )
   in.pointmarkerlist[i] = 22;
  if( cc.Get(i) == 1.0 )
   in.pointmarkerlist[i] = 33;
 }
 /* -------------------------------------------------------------- */

 // este procedimento foi substiuido pelo flag AA
 /* ESTE PROCEDIMENTO DEFINE REGIOES NA MALHA E APOS A INSERCAO/RETIRADA
  * DE PONTOS PELO TETGEN, CONSEGUIMOS RECONHECER A LOCALIZACAO DOS
  * PONTOS E ASSIM PODEMOS DEFINIR NOVAMENTE A FUNCAO MARCADORA COMO
  * SENDO 1.0 DENTRO DA BOLHA, 0.5 NA SUPERFICIE E 0.0 FORA 
  * E NECESSARIO DEFINIR 1 PONTO EM CADA REGIAO */
 // fluido interior + fluido exterior + superficie
 in.numberofregions = 1; 
 in.regionlist = new REAL[in.numberofregions*4];

 // fora da bolha
 //in.regionlist[0] = X.Min();
 //in.regionlist[1] = Y.Min();
 //in.regionlist[2] = Z.Min();
 in.regionlist[0] = 0.0;
 in.regionlist[1] = 0.0;
 in.regionlist[2] = 0.0;
 in.regionlist[3] = 1;

 /* as regioes do dominio (dentro e fora da bolha) sao definidas usando o 
  * flag AA em  tetrahedralize. O tetgen marca 1 para fora da bolha e 2
  * para dentro da bolha. Eh importante notar que esta marcacao eh em
  * nivel de elementos e NAO de pontos. Para definir os pontos da
  * superficie e com isso a funcao marcadora corretamente eh necessario
  * utilizar uma funcao marcadora de pontos do tetgen: pointmarkerlist.
  * --> este flag foi testado e nao pode ser usado sem a definicao
  *  explicita da regiao externa a bolha (regionlist). O resultado foi
  *  que de um passo para o outro ele inverte o tag da regiao, i.e.,
  *  define a regiao dentro da bolha com 1 e depois de 60 passos define
  *  a mesma regiao com 2.
  */

 tetgenio::facet *f;   // Define a pointer of facet. 
 tetgenio::polygon *p; // Define a pointer of polygon.
 in.numberoffacets = IENOriginal.DimI(); 
 in.facetlist = new tetgenio::facet[in.numberoffacets]; 
 in.facetmarkerlist = new int[in.numberoffacets];
 //in.trifacemarkerlist = new int[in.numberoffacets];

 // definindo a superficie da bolha e convex-hull
 for( int i=0;i<IENOriginal.DimI();i++ )
 {
  int v1 = IENOriginal.Get(i,0);
  int v2 = IENOriginal.Get(i,1);
  int v3 = IENOriginal.Get(i,2);
  f = &in.facetlist[i];
  f->numberofpolygons = 1;
  f->polygonlist = new tetgenio::polygon[f->numberofpolygons];
  f->numberofholes = 0; 
  f->holelist = NULL;
  p = &f->polygonlist[0];
  p->numberofvertices = 3;
  p->vertexlist = new int[p->numberofvertices];
  p->vertexlist[0] = v1; 
  p->vertexlist[1] = v2; 
  p->vertexlist[2] = v3;
  // melhorar esta configuracao de facet para bolha e convex hull
  if( cc.Get(v1) == 0.5 && cc.Get(v2) == 0.5 && cc.Get(v3) == 0.5 )
   in.facetmarkerlist[i] = 10;
  else
   in.facetmarkerlist[i] = 20;
 }

//--------------------------------------------------
//  // tentando remover pontos
//  in.pointmarkerlist[0] = 0;
//  in.pointmarkerlist[1] = 0;
//  in.pointmarkerlist[2] = 0;
//  in.pointmarkerlist[3] = 0;
//  in.pointmarkerlist[4] = 0;
// 
//  for( int i=0;i<numVerts;i++ )
//   cout << in.pointmarkerlist[i] << endl;
//-------------------------------------------------- 

 //in.save_poly("bubble");
 //in.save_nodes("bubble");
 //in.save_elements("in");
 cout << "numElems IN = " << numElems << endl;
 cout << "numNodes IN = " << numNodes << endl;
 cout << "numVerts IN = " << in.numberofpoints << endl;

 cout << endl;
 cout << "----> meshing... ";
 tetrahedralize( (char*) "QYYRApq1.4241q10a0.05",&in,&out );
 cout << "finished <---- " << endl;;
 cout << endl;

 numElems = out.numberoftetrahedra;
 numNodes = out.numberofpoints+numElems;
 numVerts = out.numberofpoints;
 cout << "numElems OUT = " << out.numberoftetrahedra << endl;
 cout << "numNodes OUT = " << out.numberofpoints+numElems << endl;
 cout << "numVerts OUT = " << out.numberofpoints << endl;
 cout << "numfacets OUT = " << out.numberoftrifaces << endl;
 //out.save_elements("out");
 //out.save_nodes("out");
 //out.save_poly("out");
 //out.save_faces("out");

 // varre lista de elementos e passa para estrutura IEN
 IEN.Dim(numElems,5);
 cc.Dim(numVerts);
 inElem.resize (0);
 outElem.resize (0);
 for( int i=0;i<out.numberoftetrahedra;i++ )
 {
  // setting de cc = 0 para fora da bolha e cc = 0.5 para interface
  if( out.tetrahedronattributelist[i] == 1 )
  {
   outElem.push_back(i);
   for( int j=0;j<4;j++ )
   {
	int vertice = out.tetrahedronlist[i*4+j];
	IEN.Set(i,j,vertice);
	cc.Set(vertice,0.0);
   }
  }
  // setting de cc = 1 para dentro da bolha e cc = 0.5 para interface
  else 
  {
   inElem.push_back(i);
   for( int j=0;j<4;j++ )
   {
	int vertice = out.tetrahedronlist[i*4+j];
	IEN.Set(i,j,vertice);
	cc.Set(vertice,1.0);
   }
  }
 }

 // atualizando valores de X,Y,Z,uc,vc,wc e pc
 X.Dim(numNodes);
 Y.Dim(numNodes);
 Z.Dim(numNodes);
 for( int i=0;i<numVerts;i++ )
 {
  X.Set(i,out.pointlist[3*i+0]);
  Y.Set(i,out.pointlist[3*i+1]);
  Z.Set(i,out.pointlist[3*i+2]);
  if( out.pointmarkerlist[i] == 10 ||
	  out.pointmarkerlist[i] == 22 )
   cc.Set(i,0.5);
 }
}


void Model3D::setDiskCouetteBC()
{
 real aux;
 rMax = Y.Max(); // CONFERIR! NAO SEI SE FUNCIONARA DIREITO!

 //for( int i=0;i<numNodes;i++ )
 //{
 // factor = 0.03;
 // aux = X.Get(i)*Z.Get(i)*factor;
 // wc.Set(i,aux);
 //}

 for( int i=0;i<numVerts;i++ )
 {
  if( Z.Get(i) == Z.Max() &&
	 (X.Get(i)*X.Get(i)+Y.Get(i)*Y.Get(i)<rMax*rMax - 0.001) )
  {
   idbcp.AddItem(i);

   aux = 0.0;
   pc.Set(i,aux);
  }

  if( ( (Z.Get(i) == Z.Max()) || (Z.Get(i) == Z.Min()) ) &&
	 (X.Get(i)*X.Get(i)+Y.Get(i)*Y.Get(i)>rMax*rMax - 0.001) )
  {
   idbcu.AddItem(i);
   idbcv.AddItem(i);
   idbcw.AddItem(i);

   aux = 0.0;
   wc.Set(i,aux);
   uc.Set(i,aux);
   vc.Set(i,aux);
  }

  if( Z.Get(i) == Z.Max() &&
	 (X.Get(i)*X.Get(i)+Y.Get(i)*Y.Get(i)<rMax*rMax - 0.001) )
  {
   idbcp.AddItem(i);

   aux = 0.0;
   pc.Set(i,aux);
  }


  if( Z.Get(i) == Z.Min() &&
	 (X.Get(i)*X.Get(i)+Y.Get(i)*Y.Get(i)<rMax*rMax - 0.001) )
  {
   idbcu.AddItem(i);
   idbcv.AddItem(i);
   idbcw.AddItem(i);

   aux = 0.0;
   wc.Set(i,aux);
   aux = 0.0;
   uc.Set(i,aux);
   vc.Set(i,aux);
  }

  if( Z.Get(i)<Z.Max() && Z.Get(i)>Z.Min() && 
	(X.Get(i)*X.Get(i)+Y.Get(i)*Y.Get(i)>rMax*rMax - 0.001) )
  {
   idbcu.AddItem(i);
   idbcv.AddItem(i);
   idbcw.AddItem(i);

   aux = 0.0;
   uc.Set(i,aux);
   vc.Set(i,aux);
   wc.Set(i,aux);
  }
 }
}

void Model3D::setNuCteDiskBC()
{
 real omega,aux;
 real radius;
 rMax = Y.Max();

 for( int i=0;i<numVerts;i++ )
 {
  radius = sqrt( X.Get(i)*X.Get(i)+Y.Get(i)*Y.Get(i) );

  if( Z.Get(i) == Z.Max() )
  {
   idbcu.AddItem(i);
   idbcv.AddItem(i);
   idbcp.AddItem(i); // caso com c.c. livre em w
   //idbcw.AddItem(i);

   //uc.Set(i,radius*2.5666593e-02); // Z=4
   //vc.Set(i,radius*3.4943977e-02); // Z=4
   //wc.Set(i,-8.2505646e-01); // Z=4

   //uc.Set(i,radius*4.5487756e-03); // Z=6
   //vc.Set(i,radius*5.9598499e-03); // Z=6
   //wc.Set(i,-8.7414071e-01); // Z=6

   //uc.Set(i,radius*1.3326987e-04); // Z=10
   //vc.Set(i,radius*1.7327920e-04); // Z=10
   //wc.Set(i,-8.8416563E-01); // Z=10
   
   uc.Set(i,0.0); // Z=12
   vc.Set(i,0.0); // Z=12
   pc.Set(i,-0.391141); // caso com c.c. livre em w
  }

  if( Z.Get(i) == Z.Min() )
  {
   idbcu.AddItem(i);
   idbcv.AddItem(i);
   idbcw.AddItem(i);

   omega=1.0;

   aux = (-1.0)*Y.Get(i)*omega;
   uc.Set(i,aux);
   aux = X.Get(i)*omega;
   vc.Set(i,aux);
   aux = 0.0;
   wc.Set(i,aux);
  }

  if( Z.Get(i)<Z.Max() && Z.Get(i)>Z.Min() && 
	(X.Get(i)*X.Get(i)+Y.Get(i)*Y.Get(i)>rMax*rMax - 0.001) )
  {
   //idbcp.AddItem(i);
   //aux = 0.0;
   //pc.Set(i,aux);
   //outflow.Set(i,aux);
  }
 }
}

void Model3D::setNuCDiskBC()
{
 real omega,aux,radius;
 rMax = Y.Max();

 for( int i=0;i<numVerts;i++ )
 {
  if( Z.Get(i) == Z.Max() )
  {
   radius = sqrt( X.Get(i)*X.Get(i)+Y.Get(i)*Y.Get(i) );

   idbcu.AddItem(i);
   idbcv.AddItem(i);
   idbcw.AddItem(i);
   ////idbcp.AddItem(i); // caso com c.c. livre em w

   //uc.Set(i,radius*2.6505291e-02); // Sc = 2000 Z = 4
   //vc.Set(i,radius*3.6178208e-02); // Sc = 2000 Z = 4
   //wc.Set(i,-8.2427145e-01); // Sc = 2000 Z = 4

   uc.Set(i,radius*1.3690760e-04); // Sc = 2000 Z = 10
   vc.Set(i,radius*1.7819422e-04); // Sc = 2000 Z = 10
   wc.Set(i,-8.8528405e-01); // Sc = 2000 Z = 10
   ////pc.Set(i,0.0); // caso com c.c. livre em w
   
   //wc.Set(i,-8.8559326E-01); // Sc = 2000
   //wc.Set(i,-9.1044679e-01); // Sc = 10
   //wc.Set(i,-9.2281563e-01); // Sc = 5
  }

  if( Z.Get(i) == Z.Min() )
  {
   idbcu.AddItem(i);
   idbcv.AddItem(i);
   idbcw.AddItem(i);

   omega=1.0;

   aux = (-1.0)*Y.Get(i)*omega;
   uc.Set(i,aux);
   aux = X.Get(i)*omega;
   vc.Set(i,aux);
   aux = 0.0;
   wc.Set(i,aux);

  }

  if( Z.Get(i)<Z.Max() && Z.Get(i)>Z.Min() && 
	(X.Get(i)*X.Get(i)+Y.Get(i)*Y.Get(i)>rMax*rMax - 0.001) )
  {
   idbcp.AddItem(i);
   aux = 0.0;
   pc.Set(i,aux);
   outflow.Set(i,aux);
  }
 }
}

void Model3D::readAndSetVelocityDiskBC(const char* _dir,const char* _filename)
{
 int size = 2401;
 real aux;
 real dist1,dist2;
 clMatrix fghMatrix(size,4);

 string fileConcat = (string) _dir + (string) _filename + ".dat";
 const char* filename = fileConcat.c_str();

 ifstream file( filename,ios::in );

 cout << endl;
 cout << "reading: " << filename  << " ...finished!" << endl;
 cout << endl;

 if( !file )
 {
  cerr << "Esta faltando o arquivo de perfil de velocidade!" << endl;
  exit(1);
 }

 // leitura do arquivo e transferencia para matriz
 if( !file.eof() )
 {
  for( int i=0;i<size;i++ )
  {
   file >> aux; // Z
   fghMatrix.Set(i,0,aux);
   file >> aux; // F
   fghMatrix.Set(i,1,aux);
   file >> aux; // G
   fghMatrix.Set(i,2,aux);
   file >> aux; // H
   fghMatrix.Set(i,3,aux);
  }
 }

 int j=0;
 real omega=1.0;
 for( int i=0;i<numNodes;i++ )
 {
  for( j=0;j<size-1;j++ )
  {
   dist1 = fabs( Z.Get(i) - fghMatrix.Get(j,0) );
   dist2 = fabs( Z.Get(i) - fghMatrix.Get(j+1,0) );
   if( dist2 > dist1 ) break;
  }
  aux = ( fghMatrix.Get(j,1)*X.Get(i)-fghMatrix.Get(j,2)*Y.Get(i) )*omega;
  uc.Set(i,aux);
  aux = ( fghMatrix.Get(j,2)*X.Get(i)-fghMatrix.Get(j,1)*Y.Get(i) )*omega;
  vc.Set(i,aux);
  aux = fghMatrix.Get(j,3);
  wc.Set(i,-1.0*aux);
 }
}

void Model3D::readAndSetPressureDiskBC(const char* _dir,const char* _filename)
{
 int size = 2401;
 real aux;
 real dist1,dist2;
 clMatrix pFile(size,2);

 string fileConcat = (string) _dir + (string) _filename + ".dat";
 const char* filename = fileConcat.c_str();

 ifstream file( filename,ios::in );

 cout << endl;
 cout << "reading: " << filename  << " ...finished!" << endl;
 cout << endl;

 if( !file )
 {
  cerr << "Esta faltando o arquivo de perfil da pressao!" << endl;
  exit(1);
 }

 // leitura do arquivo e transferencia para matriz
 if( !file.eof() )
 {
  for( int i=0;i<size;i++ )
  {
   file >> aux;
   pFile.Set(i,0,aux);
   file >> aux;
   pFile.Set(i,1,aux);
  }
 }

 int j;
 for( int i=0;i<numVerts;i++ )
 {
  for( j=0;j<size;j++ )
  {
   dist1 = fabs( Z.Get(i) - pFile(j,0) );
   dist2 = fabs( Z.Get(i) - pFile(j+1,0) );
   if( dist2 > dist1 ) break;
  }
//--------------------------------------------------
//   // applying b.c. only on the sidewall
//   if( Z.Get(i)<Z.Max() && Z.Get(i)>Z.Min() && 
// 	(X.Get(i)*X.Get(i)+Y.Get(i)*Y.Get(i)>rMax*rMax - 0.001) )
//   {
//    aux = pFile(j,1);
//    pc.Set(i,aux);
//    outflow.Set(i,aux);
//   }
//-------------------------------------------------- 
   aux = pFile(j,1);
   pc.Set(i,aux);
   outflow.Set(i,aux);
 }
}

void Model3D::setCDiskBC()
{
 real aux;
 cc.Dim(numVerts);
 idbcc.Dim(0);

 for( int i=0;i<numVerts;i++ )
 {
  if( Z.Get(i) == Z.Min() )
  {
   idbcc.AddItem(i);

   aux = 1.0;
   cc.Set(i,aux);
  }
 }
}

void Model3D::setBubbleBubbleBC()
{
 //real eps2 = 1E-2;
 real eps2 = 10E-3;
 //real eps2 = 60E-02;
 xCenter = 1.5;
 yCenter = 1.5;
 zCenter = 1.5;
 bubbleRadius = 0.5;
 cout << "xMax = " << X.Max() << endl;
 cout << "xCenter = " << xCenter << endl;
 cout << "yCenter = " << yCenter << endl;
 cout << "zCenter = " << zCenter << endl;
 cout << "bubbleRadius = " << bubbleRadius << endl;

 cc.Dim(numVerts);
 cc.SetAll(0.5);
 for( int i=0;i<numVerts;i++ )
 {
//--------------------------------------------------
//   cout << i << " " << ((X.Get(i)-xCenter)*(X.Get(i)-xCenter)+ 
// 	  (Y.Get(i)-yCenter)*(Y.Get(i)-yCenter)+
// 	  (Z.Get(i)-zCenter)*(Z.Get(i)-zCenter)) << " " << bubbleRadius*bubbleRadius << endl;
//-------------------------------------------------- 

  // dentro da bolha
  // [X-xCenter]^2 + [Y-yCenter]^2 < r^2
  if( ((X.Get(i)-xCenter)*(X.Get(i)-xCenter)+ 
	   (Y.Get(i)-yCenter)*(Y.Get(i)-yCenter)+
	   (Z.Get(i)-zCenter)*(Z.Get(i)-zCenter)+eps2<bubbleRadius*bubbleRadius))
  {
   cc.Set(i,1.0);
  }
  if( ((X.Get(i)-xCenter)*(X.Get(i)-xCenter)+ 
	   (Y.Get(i)-yCenter)*(Y.Get(i)-yCenter)+
	   (Z.Get(i)-zCenter)*(Z.Get(i)-zCenter) -eps2>bubbleRadius*bubbleRadius))
  {
   cc.Set(i,0.0);
  }
  if( (X.Get(i)-xCenter)*(X.Get(i)-xCenter) +
	  (Y.Get(i)-yCenter)*(Y.Get(i)-yCenter) +
	  (Z.Get(i)-zCenter)*(Z.Get(i)-zCenter) >
	  (X.Max()-xCenter)*(X.Max()-xCenter)-eps2 )
  {
   idbcu.AddItem(i);
   idbcv.AddItem(i);
   idbcw.AddItem(i);
   uc.Set(i,0.0);
   vc.Set(i,0.0);
   wc.Set(i,0.0);
  }
//--------------------------------------------------
//   if( X.Get(i) == X.Max() )
//   {
//    idbcp.AddItem(i);
//    pc.Set(i,0.0);
//   }
//-------------------------------------------------- 
 }
} // fecham metodo setBubbleBubbleBC

void Model3D::set2BubbleBC()
{
 real aux;

 for( int i=0;i<numVerts;i++ )
 {
  // condicao de velocidade
  if( (Y.Get(i)==Y.Min()) || (Y.Get(i)==Y.Max()) )  
  {
   idbcu.AddItem(i);
   idbcv.AddItem(i);
   idbcw.AddItem(i);

   aux = X.Get(i);
   uc.Set(i,aux);
   aux = (-1.0)*Y.Get(i);
   vc.Set(i,aux);
   aux = 0.0;
   wc.Set(i,aux);
  }
  // condicao de outflow
  if( (X.Get(i)==X.Min()) || X.Get(i)==X.Max() )  
  {
   idbcp.AddItem(i);

   aux = 0.0;
   pc.Set(i,aux);
  }
  if( (Z.Get(i)==Z.Min()) || Z.Get(i)==Z.Max() )  
  {
   idbcw.AddItem(i);

   aux = 0.0;
   wc.Set(i,aux);
  }
 }
}

void Model3D::setSphere(real _xC,real _yC,real _zC,real _r,real _eps)
{
 real eps2 = _eps;
 real r = _r;
 real xCenter = _xC;
 real yCenter = _yC;
 real zCenter = _zC;

 cc.Dim(numVerts);
 for( int i=0;i<numVerts;i++ )
 {
  /* bubble 1 */
  // dentro da bolha
  // [X-xCenter]^2 + [Y-yCenter]^2 < r^2
  if( (X.Get(i)-xCenter)*(X.Get(i)-xCenter) + 
	  (Y.Get(i)-yCenter)*(Y.Get(i)-yCenter) +
	  (Z.Get(i)-zCenter)*(Z.Get(i)-zCenter) < r*r+eps2 )
  {
   cc.Set(i,1.0);
  }
  // na superficie da bolha
  // ( [X-xCenter]^2 + [Y-yCenter]^2 - r^2 ) < 10E-4
  if( fabs((X.Get(i)-xCenter)*(X.Get(i)-xCenter) +
	       (Y.Get(i)-yCenter)*(Y.Get(i)-yCenter) + 
	       (Z.Get(i)-zCenter)*(Z.Get(i)-zCenter) - r*r ) < eps2 ) 

  {
   cc.Set(i,0.5);
  }
 }
}

void Model3D::setCubeBC()
{
 for( int i=0;i<numVerts;i++ )
 {
  // condicao de parede v=0
  if( (X.Get(i)==X.Max()) || (X.Get(i)==X.Min()) || 
      (Y.Get(i)==Y.Max()) || (Y.Get(i)==Y.Min()) || 
      (Z.Get(i)==Z.Max()) || (Z.Get(i)==Z.Min()) )
  {
   idbcu.AddItem(i);
   idbcv.AddItem(i);
   idbcw.AddItem(i);

   real aux = 0.0;
   uc.Set(i,aux);
   vc.Set(i,aux);
   wc.Set(i,aux);
  }
 }
}

void Model3D::setWallBC()
{    
 for (list<int>::iterator it=outVert.begin(); it!=outVert.end(); ++it)
 {
  idbcu.AddItem(*it);
  idbcv.AddItem(*it);
  idbcw.AddItem(*it);

  real aux = 0.0;
  uc.Set(*it,aux);
  vc.Set(*it,aux);
  wc.Set(*it,aux);
 }
}

void Model3D::setCubeBC2()
{
 for( int i=0;i<numVerts;i++ )
 {
  if( (X.Get(i)==X.Min()) || (X.Get(i)==X.Max()) || 
	  (Y.Get(i)==Y.Min()) || (Y.Get(i)==Y.Max()) )
  {
   idbcu.AddItem(i);
   idbcv.AddItem(i);
   idbcw.AddItem(i);

   uc.Set(i,0.0);
   vc.Set(i,0.0);
   wc.Set(i,0.0);
  }
  if( (Z.Get(i)==Z.Max()) &&
      (X.Get(i)>X.Min()) && (X.Get(i)<X.Max()) && 
	  (Y.Get(i)>Y.Min()) && (Y.Get(i)<Y.Max()) )
  {
   idbcw.AddItem(i);
   idbcu.AddItem(i);
   idbcv.AddItem(i);

   uc.Set(i,0.0);
   vc.Set(i,0.0);
   wc.Set(i,1.0);
  }
  if( (Z.Get(i)==Z.Min()) &&
      (X.Get(i)>X.Min()) && (X.Get(i)<X.Max()) && 
	  (Y.Get(i)>Y.Min()) && (Y.Get(i)<Y.Max()) )
  {
   idbcp.AddItem(i);
   pc.Set(i,0.0);
  }
 }
}

void Model3D::setCube(real _lim1,real _lim2,real _eps)
{
 real eps2 = _eps;
 real lim1 = _lim1;
 real lim2 = _lim2;

 cc.Dim(numVerts);
 for( int i=0;i<numVerts;i++ )
 {
  // na interface
  if( (X.Get(i)<lim2+eps2) && (X.Get(i)>lim1-eps2) && 
      (Y.Get(i)<lim2+eps2) && (Y.Get(i)>lim1-eps2) && 
      (Z.Get(i)<lim2+eps2) && (Z.Get(i)>lim1-eps2) )
  {
   cc.Set(i,0.5);
  }
  // dentro da bolha
  if( (X.Get(i)<lim2-eps2) && (X.Get(i)>lim1+eps2) && 
      (Y.Get(i)<lim2-eps2) && (Y.Get(i)>lim1+eps2) && 
      (Z.Get(i)<lim2-eps2) && (Z.Get(i)>lim1+eps2) )
  {
   cc.Set(i,1.0);
  }
 }
}

void Model3D::setCube(real _xlimInf,real _xlimSup,
                      real _ylimInf,real _ylimSup,
					  real _zlimInf,real _zlimSup,real _eps)

{
 real eps2 = _eps;
 real xlimInf = _xlimInf;
 real ylimInf = _ylimInf;
 real zlimInf = _zlimInf;
 real xlimSup = _xlimSup;
 real ylimSup = _ylimSup;
 real zlimSup = _zlimSup;

 cc.Dim(numVerts);
 for( int i=0;i<numVerts;i++ )
 {
  // na interface
  if( (X.Get(i)<xlimSup+eps2) && (X.Get(i)>xlimInf-eps2) && 
      (Y.Get(i)<ylimSup+eps2) && (Y.Get(i)>ylimInf-eps2) && 
      (Z.Get(i)<zlimSup+eps2) && (Z.Get(i)>zlimInf-eps2) )
  {
   cc.Set(i,0.5);
  }
  // dentro da bolha
  if( (X.Get(i)<xlimSup-eps2) && (X.Get(i)>xlimInf+eps2) && 
      (Y.Get(i)<ylimSup-eps2) && (Y.Get(i)>ylimInf+eps2) && 
      (Z.Get(i)<zlimSup-eps2) && (Z.Get(i)>zlimInf+eps2) )
  {
   cc.Set(i,1.0);
  }
 }
}

void Model3D::setBubble3DBC()
{
 real eps2 = 10E-3;
 //real eps2 = 60E-02;
 xCenter = 1.5;
 yCenter = 1.5;
 zCenter = 1.5;
 bubbleRadius = 0.5;
 cout << "xCenter = " << xCenter << endl;
 cout << "yCenter = " << yCenter << endl;
 cout << "zCenter = " << yCenter << endl;
 cout << "bubbleRadius = " << bubbleRadius << endl;

 for( int i=0;i<numVerts;i++ )
 {
  //if( (X.Get(i)==X.Max()) && (Y.Get(i)==Y.Min()) )
  if( (X.Get(i)==X.Max()) && (Y.Get(i)==0) )
  {
//--------------------------------------------------
//    idbcp.AddItem(i);
//    pc.Set(i,0.0);
//    outflow.Set(i,0);
//-------------------------------------------------- 
  }
  // dentro da bolha
  // [X-xCenter]^2 + [Y-yCenter]^2 < r^2
  if( (X.Get(i)-xCenter)*(X.Get(i)-xCenter) + 
	  (Y.Get(i)-yCenter)*(Y.Get(i)-yCenter) +
	  (Z.Get(i)-zCenter)*(Z.Get(i)-zCenter) <bubbleRadius*bubbleRadius+eps2 )
  {
   cc.Set(i,1.0);
  }
  // fora da bolha
  // [X-xCenter]^2 + [Y-yCenter]^2 < r^2
  if( (X.Get(i)-xCenter)*(X.Get(i)-xCenter) + 
	  (Y.Get(i)-yCenter)*(Y.Get(i)-yCenter) +
	  (Z.Get(i)-zCenter)*(Z.Get(i)-zCenter) >bubbleRadius*bubbleRadius-eps2 )
  {
   cc.Set(i,0.0);
  }
  // na superficie da bolha
  // ( [X-xCenter]^2 + [Y-yCenter]^2 - r^2 ) < 10E-4
  if( fabs((X.Get(i)-xCenter)*(X.Get(i)-xCenter) +
	       (Y.Get(i)-yCenter)*(Y.Get(i)-yCenter) + 
	       (Z.Get(i)-zCenter)*(Z.Get(i)-zCenter) - 
	        bubbleRadius*bubbleRadius ) < eps2 ) 

  {
   cc.Set(i,0.5);
  }
  if( X.Get(i) == 0 || X.Get(i) == X.Max() ||
      Y.Get(i) == 0 || Y.Get(i) == Y.Max() ||
      Z.Get(i) == 0 || Z.Get(i) == Z.Max() )
  {
   idbcu.AddItem(i);
   idbcv.AddItem(i);
   idbcw.AddItem(i);
   uc.Set(i,0.0);
   vc.Set(i,0.0);
   wc.Set(i,0.0);
  }
 }
}

void Model3D::setBubbleBC2()
{
 //real eps2 = 10E-02;
 real eps2 = 1E-00;
 xCenter = (X.Max()+X.Min())/2.0;
 yCenter = (Y.Max()+Y.Min())/2.0;
 zCenter = (Z.Max()+Z.Min())/2.0;
 bubbleRadius = (X.Max()-xCenter)/6.0;
 int n = 20;
 clVector zz(n);
 real aux;
 for( int i=0;i<n;i++ )
 {
  aux = Z.Get(180*i+30);
  zz.Set(i,aux);
 }

 cc.SetAll(0.0);
 for( int i=0;i<numVerts;i++ )
 {
//--------------------------------------------------
//   //if( (X.Get(i)==X.Max()) && (Y.Get(i)==Y.Min()) )
//   if( (X.Get(i)==X.Max()) && (Y.Get(i)==0) )
//   {
//    idbcp.AddItem(i);
//    pc.Set(i,0.0);
//    outflow.Set(i,0);
//   }
//-------------------------------------------------- 
  if( ((X.Get(i)-xCenter)*(X.Get(i)-xCenter)+ 
	  (Y.Get(i)-yCenter)*(Y.Get(i)-yCenter) - 3E-00<bubbleRadius*bubbleRadius)
	&& (Z.Get(i) > zz.Get(5)) && (Z.Get(i) < zz.Get(13))
	)
  {
   cc.Set(i,0.5);
  }
  // dentro da bolha
  // [X-xCenter]^2 + [Y-yCenter]^2 < r^2
  if( ((X.Get(i)-xCenter)*(X.Get(i)-xCenter)+ 
	  (Y.Get(i)-yCenter)*(Y.Get(i)-yCenter) - eps2<bubbleRadius*bubbleRadius)
	&& (Z.Get(i) > zz.Get(6)) && (Z.Get(i) < zz.Get(12))
	)
  {
   cc.Set(i,1.0);
  }
//--------------------------------------------------
//   // fora da bolha
//   // [X-xCenter]^2 + [Y-yCenter]^2 > r^2
//   if( ((X.Get(i)-xCenter)*(X.Get(i)-xCenter)+
// 	  (Y.Get(i)-yCenter)*(Y.Get(i)-yCenter) + eps2>bubbleRadius*bubbleRadius)&&
// 	  (Z.Get(i) < zz.Get(7)) && (Z.Get(i) > zz.Get(12))
//     )
//   {
//    cc.Set(i,0.0);
//   }
//-------------------------------------------------- 
  // na superficie da bolha
  // ( [X-xCenter]^2 + [Y-yCenter]^2 - r^2 ) < 10E-4
  if( (Z.Get(i)==Z.Min()) || (Z.Get(i) == Z.Max()) )
  {
   idbcu.AddItem(i);
   idbcv.AddItem(i);
   idbcw.AddItem(i);
   uc.Set(i,0.0);
   vc.Set(i,0.0);
   wc.Set(i,0.0);
  }
  if( Z.Get(i)<Z.Max() && Z.Get(i)>Z.Min() &&
	 (X.Get(i)-xCenter)*(X.Get(i)-xCenter) +
	 (Y.Get(i)-yCenter)*(Y.Get(i)-yCenter) + eps2>X.Max()*X.Max() )
  {
   idbcu.AddItem(i);
   idbcv.AddItem(i);
   idbcw.AddItem(i);
   uc.Set(i,0.0);
   vc.Set(i,0.0);
   wc.Set(i,0.0);
  }
 }
}

void Model3D::readBaseStateNu( const char* _filename )
{
 real omega;
 real aux,dist1,dist2;
 int lines = 2401;
 int j;
 clMatrix fMatrix(lines,2);
 clMatrix gMatrix(lines,2);
 clMatrix hMatrix(lines,2);
 clMatrix dHdZMatrix(lines,2);
 clVector pVector(lines);


 string file = "../db/baseState/nuC/Sc2000/F_" + (string) _filename + ".dat";
 const char* filenameF = file.c_str();
 ifstream fFile( filenameF,ios::in );

 file = "../db/baseState/nuC/Sc2000/G_" + (string) _filename + ".dat"; 
 const char* filenameG = file.c_str();
 ifstream gFile( filenameG,ios::in );

 file = "../db/baseState/nuC/Sc2000/H_" + (string) _filename + ".dat"; 
 const char* filenameH =  file.c_str();
 ifstream hFile( filenameH,ios::in );

 file = "../db/baseState/nuC/Sc2000/dHdZ_" + (string) _filename + ".dat"; 
 const char* filenamedHdZ = file.c_str();
 ifstream dHdZFile( filenamedHdZ,ios::in );

 if( !fFile || !gFile || !hFile || !dHdZFile )
 {
  cerr << "Esta faltando algum arquivo do estado base para NuC!" << endl;
  exit(1);
 }

 for( int i=0;i<2401;i++ )
 {
  fFile >> aux;
  fMatrix.Set(i,0,aux);
  fFile >> aux;
  fMatrix.Set(i,1,aux);
  gFile >> aux;
  gMatrix.Set(i,0,aux);
  gFile >> aux;
  gMatrix.Set(i,1,aux);
  hFile >> aux;
  hMatrix.Set(i,0,aux);
  hFile >> aux;
  hMatrix.Set(i,1,aux);
  dHdZFile >> aux;
  dHdZMatrix.Set(i,0,aux);
  dHdZFile >> aux;
  dHdZMatrix.Set(i,1,aux);
  aux = dHdZMatrix.Get(i,1) - 0.5*hMatrix.Get(i,1)*hMatrix.Get(i,1);
  pVector.Set(i,aux);
 }

 j=0;
 omega=1.0;
 for( int i=0;i<numNodes;i++ )
 {
  for( j=0;j<lines-1;j++ )
  {
   dist1 = fabs( Z.Get(i) - fMatrix.Get(j,0) );
   dist2 = fabs( Z.Get(i) - fMatrix.Get(j+1,0) );
   if( dist2 > dist1 ) break;
  }
  aux = ( fMatrix.Get(j,1)*X.Get(i)-gMatrix.Get(j,1)*Y.Get(i) )*omega;
  uc.Set(i,aux);
  aux = ( gMatrix.Get(j,1)*X.Get(i)-fMatrix.Get(j,1)*Y.Get(i) )*omega;
  vc.Set(i,aux);
  aux = hMatrix.Get(j,1);
  wc.Set(i,aux);
 }

 j=0;
 for( int i=0;i<numVerts;i++ )
 {
  for( j=0;j<lines-1;j++ )
  {
   dist1 = fabs( Z.Get(i) - fMatrix.Get(j,0) );
   dist2 = fabs( Z.Get(i) - fMatrix.Get(j+1,0) );
   if( dist2 > dist1 ) break;
  }
  aux = pVector.Get(j);
  pc.Set(i,aux);
 }
}

void Model3D::setNuZDiskBC()
{
 real omega,aux,radius;
 rMax = Y.Max();

 for( int i=0;i<numVerts;i++ )
 {
  radius = sqrt( X.Get(i)*X.Get(i)+Y.Get(i)*Y.Get(i) );

  if( Z.Get(i) == Z.Max() )
  {
   idbcu.AddItem(i);
   idbcv.AddItem(i);
   idbcw.AddItem(i);
   ////idbcp.AddItem(i); // caso com c.c. livre em w

   //uc.Set(i,radius*4.8979239E-02); // Z=4
   //vc.Set(i,radius*7.9371092E-02); // Z=4
   //wc.Set(i,-9.1936908E-01); // Z=4

   //uc.Set(i,radius*6.8574756E-03); // Z=6
   //vc.Set(i,radius*1.0335366E-02); // Z=6
   //wc.Set(i,-0.10196142E+01); // Z=6

   uc.Set(i,radius*0.11735664E-03); // Z=10
   vc.Set(i,radius*0.17501519E-03); // Z=10
   wc.Set(i,-0.10193840E+01); // Z=10
   /////pc.Set(i,0.0); // Z=10 // caso com c.c. livre em w
  }

  if( Z.Get(i) == Z.Min() )
  {
   idbcu.AddItem(i);
   idbcv.AddItem(i);
   idbcw.AddItem(i);

   omega=1.0;

   aux = (-1.0)*Y.Get(i)*omega;
   uc.Set(i,aux);
   aux = X.Get(i)*omega;
   vc.Set(i,aux);
   aux = 0.0;
   wc.Set(i,aux);
  }

  if( Z.Get(i)<Z.Max() && Z.Get(i)>Z.Min() && 
	(X.Get(i)*X.Get(i)+Y.Get(i)*Y.Get(i)> (rMax*rMax - 0.001) ) )
  {
   idbcp.AddItem(i);
   aux = 0.0;
   pc.Set(i,aux);
   outflow.Set(i,aux);
  }
 }
}

// FSBC - Free Surface Boundary Condition
void Model3D::setDiskFSBC()
{
 real aux;
 rMax = Y.Max();// CONFERIR! NAO SEI SE FUNCIONARA DIREITO!

 for( int i=0;i<numVerts;i++ )
 {
  if( Z.Get(i) == Z.Min() )
  {
   idbcu.AddItem(i);
   idbcv.AddItem(i);
   idbcw.AddItem(i);

   aux = 0.0;
   uc.Set(i,aux);
   vc.Set(i,aux);
   wc.Set(i,aux);
  }
  if( Z.Get(i)<=Z.Max() && Z.Get(i)>Z.Min() && 
	(X.Get(i)*X.Get(i)+Y.Get(i)*Y.Get(i) > (rMax*rMax - 0.001) ) )
  {
   idbcu.AddItem(i);
   idbcv.AddItem(i);
   idbcw.AddItem(i);

   aux = 0.0;
   uc.Set(i,aux);
   vc.Set(i,aux);
   wc.Set(i,aux);
  }
  if( Z.Get(i)==Z.Max() && 
	 (X.Get(i)*X.Get(i)+Y.Get(i)*Y.Get(i) < (rMax*rMax - 0.001) ) )
  {
   idbcu.AddItem(i);
   idbcv.AddItem(i);
   idbcp.AddItem(i);

   // cc = 0.5 -> noh da superficie
   //cc.Set(i,0.5); // para funcionamento do ALE

   aux = 0.0;
   uc.Set(i,aux);
   vc.Set(i,aux);
   pc.Set(i,aux);
   outflow.Set(i,aux);
  }
 }
}

void Model3D::setAdimenDisk()
{
 real aux;
 real Red = 50;
 //real Red = 100;
 real factorz = 1.0/(Z.Max()-Z.Min());
 rMax = Y.Max();

 for( int i=0;i<numVerts;i++ )
 {
  aux = (X.Get(i)/rMax)*Red;
  X.Set(i,aux);
  aux = (Y.Get(i)/rMax)*Red;
  Y.Set(i,aux);
  //aux = Z.Get(i)*factorz*4;
  //aux = Z.Get(i)*factorz*6;
  aux = Z.Get(i)*factorz*10;
  Z.Set(i,aux);
 }
}

void Model3D::setAdimenDiskCouette()
{
 real aux;
 real Red = 100;
 real factorz = 1.0/(Z.Max()-Z.Min());

 for( int i=0;i<numVerts;i++ )
 {
  aux = (X.Get(i)/rMax)*Red;
  X.Set(i,aux);
  aux = (Y.Get(i)/rMax)*Red;
  Y.Set(i,aux);
  aux = Z.Get(i)*factorz*100;
  Z.Set(i,aux);
 }
}

void Model3D::setPerturbSurf()
{
 real aux;
 real Red = 1;
 real factorz = 1.0/(Z.Max()-Z.Min());

 for( int i=0;i<numVerts;i++ )
 {
  if( Z.Get(i) != Z.Min() &&
	  (X.Get(i)*X.Get(i)+Y.Get(i)*Y.Get(i) < (rMax*rMax - 0.1) ) )
  {
   aux = Z.Get(i)*( 1.0 + (X.Get(i)/Red)*factorz*0.4 );
   Z.Set(i,aux);
  }
 }
}

void Model3D::setPerturbSurf2()
{
 real aux;
 real Red = 1;
 real factorz = 1.0/(Z.Max()-Z.Min());
 real xMid = X.Min()+(X.Max()-X.Min())/2.0;

 for( int i=0;i<numVerts;i++ )
 {
  if( Z.Get(i) != Z.Min() && X.Get(i) > xMid  &&
	  (X.Get(i)*X.Get(i)+Y.Get(i)*Y.Get(i) < (rMax*rMax - 0.001) ) )
  {
   aux = Z.Get(i)*( 0.65 + (X.Get(i)/Red)*factorz*0.6 );
   Z.Set(i,aux);
  }
  if( Z.Get(i) != Z.Min() && X.Get(i) <= xMid  &&
	  (X.Get(i)*X.Get(i)+Y.Get(i)*Y.Get(i) < (rMax*rMax - 0.001) ) )
  {
   aux = Z.Get(i)*( 0.65 - (X.Get(i)/Red)*factorz*0.6 );
   Z.Set(i,aux);
  }
 }
}

void Model3D::setPerturbSurfSquare()
{
 real aux;
 real Red = 1;
 real factorz = 1.0/(Z.Max()-Z.Min());
 real rMid = rMax/10.0;

 for( int i=0;i<numVerts;i++ )
 {
  if( Z.Get(i) != Z.Min() && 
	  (X.Get(i)*X.Get(i)+Y.Get(i)*Y.Get(i) < (rMid*rMid - 0.001) ) )
  {
   aux = Z.Get(i)*( 1.0 + (Z.Get(i)/Red)*factorz*0.6 );
   Z.Set(i,aux);
  }
 }
}

void Model3D::setMiniElement()
{
 V.Dim(numElems);
 real centroidX,centroidY,centroidZ;
 int v1,v2,v3,v4,v5;

 numGLEP = 4; // triangulo linear
 numGLEU = 5; // elemento MINI
 numGLEC = 4; // elemento linear
 numNodes = numVerts + numElems;

 clearBC();
 reAllocStruct();

 for( int i=0;i<numElems;i++ )
 {
  v1 = (int) IEN.Get(i,0);
  v2 = (int) IEN.Get(i,1);
  v3 = (int) IEN.Get(i,2);
  v4 = (int) IEN.Get(i,3);

  real volume = getVolume(i);
  V.Set(i,volume);

  if( fabs(volume)<1.0E-10)
  {
   cout << i << endl;
   cout << volume << endl;
   cerr << "tetraedro singular, verificar a qualidade da malha!" << endl;
  }

  if( volume<0.0 )
  {
   IEN.Set(i,1,v3);
   IEN.Set(i,2,v2);
  };

  v5=numVerts+i;

  IEN.Set(i,4,v5);
  centroidX = ( X.Get(v1)+X.Get(v2)+X.Get(v3)+X.Get(v4) )*0.25;
  centroidY = ( Y.Get(v1)+Y.Get(v2)+Y.Get(v3)+Y.Get(v4) )*0.25;
  centroidZ = ( Z.Get(v1)+Z.Get(v2)+Z.Get(v3)+Z.Get(v4) )*0.25;
  X.Set(v5,centroidX);
  Y.Set(v5,centroidY);
  Z.Set(v5,centroidZ);
 
 }
}

// criando os nos na metade de cada aresta. Para isso eh necessario
// fazer uma lista de arestas e numera-las de forma que uma aresta comum
// tenha apenas 1 numero e seja compartilhada em todos os elementos que
// tem a aresta. 
void Model3D::setQuadElement()
{
 int v1,v2,v3,v4;
 int numFace = 6; // teraedro tem 6 arestas
 V.Dim(numElems);
 clVector faceaux(2);
 IFACE2D *faces = NULL;
 int listSize = numFace*numElems;
 faces = new IFACE2D[listSize];
 for( int mele=0;mele<numElems;mele++ )
 {
  v1 = (int) IEN.Get(mele,0);
  v2 = (int) IEN.Get(mele,1);
  v3 = (int) IEN.Get(mele,2);
  v4 = (int) IEN.Get(mele,3);

  real volume = getVolume(mele);
  V.Set(mele,volume);

  if( fabs(volume)<1.0E-10)
   cerr << "tetraedro singular, verificar a qualidade da malha!" << endl;

  if( volume<0.0 )
  {
   IEN.Set(mele,1,v3);
   IEN.Set(mele,2,v2);
  }
  // -------------------------------------------- //

  faceaux.Set(0,v1);
  faceaux.Set(1,v2);
  faceaux.Sort(); // para ordenar os vertices de uma aresta
  faces[numFace*mele+0].p1 = (int) faceaux.Get(0);
  faces[numFace*mele+0].p2 = (int) faceaux.Get(1);
  faces[numFace*mele+0].p3 = mele;

  faceaux.Set(0,v1);
  faceaux.Set(1,v3);
  faceaux.Sort(); // para ordenar os vertices de uma aresta
  faces[numFace*mele+1].p1 = (int) faceaux.Get(0);
  faces[numFace*mele+1].p2 = (int) faceaux.Get(1);
  faces[numFace*mele+1].p3 = mele;

  faceaux.Set(0,v1);
  faceaux.Set(1,v4);
  faceaux.Sort(); // para ordenar os vertices de uma aresta
  faces[numFace*mele+2].p1 = (int) faceaux.Get(0);
  faces[numFace*mele+2].p2 = (int) faceaux.Get(1);
  faces[numFace*mele+2].p3 = mele;

  faceaux.Set(0,v2);
  faceaux.Set(1,v3);
  faceaux.Sort(); // para ordenar os vertices de uma aresta
  faces[numFace*mele+3].p1 = (int) faceaux.Get(0);
  faces[numFace*mele+3].p2 = (int) faceaux.Get(1);
  faces[numFace*mele+3].p3 = mele;

  faceaux.Set(0,v2);
  faceaux.Set(1,v4);
  faceaux.Sort(); // para ordenar os vertices de uma aresta
  faces[numFace*mele+4].p1 = (int) faceaux.Get(0);
  faces[numFace*mele+4].p2 = (int) faceaux.Get(1);
  faces[numFace*mele+4].p3 = mele;

  faceaux.Set(0,v3);
  faceaux.Set(1,v4);
  faceaux.Sort(); // para ordenar os vertices de uma aresta
  faces[numFace*mele+5].p1 = (int) faceaux.Get(0);
  faces[numFace*mele+5].p2 = (int) faceaux.Get(1);
  faces[numFace*mele+5].p3 = mele;
 }

 // ordena uma estrutura (matriz) em ordem crescente na linha e coluna
 // as faces continuam repetidas neste ponto.
 qsort(faces,listSize,sizeof(IFACE2D),IFACE2DCompare);

 /*        - nome: mapEdge
           - definicao: matrix com mapeamento de arestas
		                Identificacao da aresta, coordenadas X e Y
						Ultima coluna representa o elemento que contem a aresta

     ---   +---+---+---+---+---+---+---+
      |    | a | b | c | d | e | f | g |  a = identificacao da aresta
      |    +---+---+---+---+---+---+---+
	  |    .   .   .   .   .   .   .   .  b = coordenada X do noh
	  |	   .   .   .   .   .   .   .   .
	  g    .   .   .   .   .   .   .   .  c = coordenada Y do noh
      |    +---+---+---+---+---+---+---+                               
      |    | a | b | c | c | e | f | g |  d = coordenada Z do noh
      |    +---+---+---+---+---+---+---+ 
      |    | a | b | c | c | e | f | g |  e = vertice vizinho v1
     ---   +---+---+---+---+---+---+---+                                           
                                          f = vertice vizinho v2
	       |____________ h ____________|                                           
		   |                           |  g = elemento que contem o noh da aresta
                                                                                   
		                                  h = numero de arestas (inclui repetidas) 

										  i = 7 colunas
 
 */

 int it=0;
 clMatrix mapEdge(listSize,7);

 // numeracao de arestas a partir de numVerts e associacao das arestas
 // aos elementos para inclusao na IEN
 for( int i=1;i<=listSize;i++ )
 {
  real x=X.Get(faces[i-1].p1)+(X.Get(faces[i-1].p2)-X.Get(faces[i-1].p1))*0.5;
  real y=Y.Get(faces[i-1].p1)+(Y.Get(faces[i-1].p2)-Y.Get(faces[i-1].p1))*0.5;
  real z=Z.Get(faces[i-1].p1)+(Z.Get(faces[i-1].p2)-Z.Get(faces[i-1].p1))*0.5;
  mapEdge.Set(i-1,0,numVerts+it); // numero da aresta
  mapEdge.Set(i-1,1,x ); // coordenada X da aresta
  mapEdge.Set(i-1,2,y ); // coordenada Y da aresta
  mapEdge.Set(i-1,3,z ); // coordenada Y da aresta
  mapEdge.Set(i-1,4,faces[i-1].p1 ); // 1o noh
  mapEdge.Set(i-1,5,faces[i-1].p2 ); // 2o noh
  mapEdge.Set(i-1,6,faces[i-1].p3 ); // elemento que contem a aresta
  while( (faces[i].p1 == faces[i-1].p1) &&
         (faces[i].p2 == faces[i-1].p2) )
  {
  mapEdge.Set(i,0,mapEdge.Get(i-1,0));
  mapEdge.Set(i,1,mapEdge.Get(i-1,1));
  mapEdge.Set(i,2,mapEdge.Get(i-1,2));
  mapEdge.Set(i,3,mapEdge.Get(i-1,3));
  mapEdge.Set(i,4,faces[i-1].p1 );
  mapEdge.Set(i,5,faces[i-1].p2 );
  mapEdge.Set(i,6,faces[i].p3 );
   i++;
  }
  it++; // numero total de arestas
 }

 // atualizado vetores com numero total de nos
 numGLEP = 4; // tetraedro linear
 numGLEC = 4; // tetraedro linear
 numGLEU = 10; // tetraedro quadratico
 numNodes = numVerts+it; // atualizando numNodes

 clearBC();
 reAllocStruct();

 // adicionando os vertices das arestas nas estruturas X,Y,Z e IEN
 vector< list<int> > edge;
 edge.resize(0);
 edge.resize(numElems);
 list<int> plist;
 list<int>::iterator ele;
 for( int i=0;i<listSize;i++ )
 {
  int node = mapEdge.Get(i,0);
  int elem = mapEdge.Get(i,6);
  X.Set(node,mapEdge.Get(i,1));
  Y.Set(node,mapEdge.Get(i,2));
  Z.Set(node,mapEdge.Get(i,3));
  // vertice v5 que fica entre v1 e v2
  if( (mapEdge.Get(i,4) == IEN.Get(elem,0) && 
	   mapEdge.Get(i,5) == IEN.Get(elem,1)) || 
	  (mapEdge.Get(i,5) == IEN.Get(elem,0) && 
	   mapEdge.Get(i,4) == IEN.Get(elem,1)) )
  {
   IEN.Set(elem,4,node);
  }
  // vertice v6 que fica entre v1 e v3
  if( (mapEdge.Get(i,4) == IEN.Get(elem,0) && 
	   mapEdge.Get(i,5) == IEN.Get(elem,2)) || 
	  (mapEdge.Get(i,5) == IEN.Get(elem,0) && 
	   mapEdge.Get(i,4) == IEN.Get(elem,2)) )
  {
   IEN.Set(elem,5,node);
  }
  // vertice v7 que fica entre v1 e v4
  if( (mapEdge.Get(i,4) == IEN.Get(elem,0) && 
	   mapEdge.Get(i,5) == IEN.Get(elem,3)) || 
	  (mapEdge.Get(i,5) == IEN.Get(elem,0) && 
	   mapEdge.Get(i,4) == IEN.Get(elem,3)) )
  {
   IEN.Set(elem,6,node);
  }
  // vertice v8 que fica entre v2 e v3
  if( (mapEdge.Get(i,4) == IEN.Get(elem,1) && 
	   mapEdge.Get(i,5) == IEN.Get(elem,2)) || 
	  (mapEdge.Get(i,5) == IEN.Get(elem,1) && 
	   mapEdge.Get(i,4) == IEN.Get(elem,2)) )
  {
   IEN.Set(elem,7,node);
  }
  // vertice v9 que fica entre v3 e v4
  if( (mapEdge.Get(i,4) == IEN.Get(elem,2) && 
	   mapEdge.Get(i,5) == IEN.Get(elem,3)) || 
	  (mapEdge.Get(i,5) == IEN.Get(elem,2) && 
	   mapEdge.Get(i,4) == IEN.Get(elem,3)) )
  {
   IEN.Set(elem,8,node);
  }
  // vertice v10 que fica entre v2 e v4
  if( (mapEdge.Get(i,4) == IEN.Get(elem,1) && 
	   mapEdge.Get(i,5) == IEN.Get(elem,3)) || 
	  (mapEdge.Get(i,5) == IEN.Get(elem,1) && 
	   mapEdge.Get(i,4) == IEN.Get(elem,3)) )
  {
   IEN.Set(elem,9,node);
  }
 }

 delete[] faces;
}

void Model3D::setNeighbour()
{
 neighbourElem.resize (0);
 neighbourElem.resize (numVerts);
 for( int i=0;i<numElems;i++ )
  for( int j= 0;j<numGLEP;j++ )
   neighbourElem.at( (int)IEN.Get(i,j) ).push_back(i);
}

void Model3D::setVertNeighbour()
{
 // cria lista de vizinhos para toda a malha
 int v1,v2,v3,v4;
 list<int> plist;
 list<int>::iterator mele;
 neighbourVert.resize (0);
 neighbourVert.resize (numVerts);

 for( int ii=0;ii<numVerts;ii++ )
 {
  clVector vert(0);
  plist = neighbourElem.at(ii);
  for( mele=plist.begin(); mele != plist.end();++mele )
  {
   v1 = (int) IEN.Get(*mele,0);
   v2 = (int) IEN.Get(*mele,1);
   v3 = (int) IEN.Get(*mele,2);
   v4 = (int) IEN.Get(*mele,3);
  // cout << v1 << " " << v2 << " " << v3 << " " << v4 << endl;
   if( v1==ii )
   {
	neighbourVert.at( ii ).push_back(v2);
	neighbourVert.at( ii ).push_back(v3);
	neighbourVert.at( ii ).push_back(v4);
   }
   if( v2==ii )
   {
	neighbourVert.at( ii ).push_back(v1);
	neighbourVert.at( ii ).push_back(v3);
	neighbourVert.at( ii ).push_back(v4);
   }
   if( v3==ii )
   {
	neighbourVert.at( ii ).push_back(v1);
	neighbourVert.at( ii ).push_back(v2);
	neighbourVert.at( ii ).push_back(v4);
   }
   if( v4==ii )
   {
	neighbourVert.at( ii ).push_back(v1);
	neighbourVert.at( ii ).push_back(v2);
	neighbourVert.at( ii ).push_back(v3);
   }
  }
  //cout << "---------" << ii << "------------" << endl;
  neighbourVert.at( ii ).sort();
  neighbourVert.at( ii ).unique();
//--------------------------------------------------
//   std::ostream_iterator< int > output( cout, " " );
//   std::copy( neighbourVert.at(ii).begin(), 
//              neighbourVert.at(ii).end(), output );
//   cout << endl;
//-------------------------------------------------- 
 }
}

// configura os vetores: 
// surfaceViz  -> vizinhos de cada no na superficie
// xSurfaceViz -> coordenada x dos vizinhos
// ySurfaceViz -> coordenada y dos vizinhos
void Model3D::setSurface()
{
 int surfaceNode;
 list<int> plist;
 list<int>::iterator vert;

 /*        - nome: surfaceViz
           - definicao: vetor de listas de vizinhos de vertices na interface 
		   - obs: no caso 2D n=2. no caso 3D n pode variar de vertice 
		          para vertice.

     ---   +---+----+-----+----+
      |    | b | a1 | ... | an |   a1...an = identificacao dos vizinhos (lista)
      |    +---+----+-----+----+
    c |    | b | a1 | ... | an |   b = identificacao do vertice de trabalho   
      |    +---+----+-----+----+
      |    | b | a1 | ... | an |   c = numero de vertices na interface
     ---   +---+----+-----+----+

 */

 /*        - nome: xSurfaceViz,ySurfaceViz
           - definicao: vetor de listas de coordenadas de vizinhos 
		                de vertices na interface 
		   - obs: no caso 2D n=2. no caso 3D n pode variar de vertice 
		          para vertice.

     ---   +---+----+-----+----+
      |    | b | a1 | ... | an |   a1...an = identificacao da coordenada  X e Y
      |    +---+----+-----+----+             dos vizinhos (lista)
    c |    | b | a1 | ... | an |   b = identificacao das coordenadas do   
      |    +---+----+-----+----+       do vertice de trabalho
      |    | b | a1 | ... | an |   c = numero de vertices na interface
     ---   +---+----+-----+----+

 */

 // procurando vertices da bolha
 clVector surfaceAux = cc==0.5;
 surface = surfaceAux.Find();
 clVector nonSurfaceAux = cc!=0.5;
 nonSurface = nonSurfaceAux.Find();

 // dimensionando vetores
 surfaceViz.resize ( 0);
 xSurfaceViz.resize (0 );
 ySurfaceViz.resize (0 );
 zSurfaceViz.resize (0 );
 surfaceViz.resize ( surface.Dim() );
 xSurfaceViz.resize ( surface.Dim() );
 ySurfaceViz.resize ( surface.Dim() );
 zSurfaceViz.resize ( surface.Dim() );

 for( int i=0;i<surface.Dim();i++ )
 {
  surfaceNode = surface.Get(i);
  plist = neighbourVert.at(surfaceNode); 

  // adicionando no primeiro elemento da lista o valor do vertice de
  // trabalho
  surfaceViz.at( i ).push_back(surfaceNode); 
  xSurfaceViz.at( i ).push_back( X.Get(surfaceNode) );
  ySurfaceViz.at( i ).push_back( Y.Get(surfaceNode) );
  zSurfaceViz.at( i ).push_back( Z.Get(surfaceNode) );
  // procura dos vertices adjacentes na interface (um de cada lado)
  for( vert=plist.begin();vert!=plist.end();++vert )
  {
   if( cc.Get(*vert) == 0.5 )   
   {
	surfaceViz.at( i ).push_back(*vert);
	xSurfaceViz.at( i ).push_back( X.Get(*vert) );
	ySurfaceViz.at( i ).push_back( Y.Get(*vert) );
	zSurfaceViz.at( i ).push_back( Z.Get(*vert) );
   }
  }
 }
} // fecha metodo setSurface

//--------------------------------------------------
// void Model3D::setConvex()
// {
//  IENConvexTri.Dim(freeFace.DimI(),3);
//  for(int i=0;i<freeFace.DimI();i++ )
//  {
//   int v1 = freeFace.Get(i,2);
//   int v2 = freeFace.Get(i,3);
//   int v3 = freeFace.Get(i,4);
//   X.Get(v1);X.Get(v2);X.Get(v3);
//   Y.Get(v1);Y.Get(v2);Y.Get(v3);
//   Z.Get(v1);Z.Get(v2);Z.Get(v3);
//  }
// }
//-------------------------------------------------- 

// as faces da superficie sao numeradas de 0..n-1 onde n eh o numero de
// faces na superficie. Para o caso 3D as faces sao triangulos. Junto a
// esta numeracao ha um mapeamento dos vertices da superficie vizinhos de 
// cada vertice na superficie. Os pontos vizinhos estao ordenados no
// sentido do relogio, com isso eh possivel descobrir facilmente o vetor
// normal a cada ponto da superficie utilizando as arestas dos elementos
// e produto escalar dos vetores localizados na aresta. 
void Model3D::setSurfaceFace()
{
 int v1,v2,v3,v4;
 list<int> plist;
 list<int>::iterator mele;
 int surfaceNode;
 
 // mapeamento de faces da interface com numeracao
 elemSurface.resize ( 0 ); 
 elemSurface.resize ( numVerts ); 
 // mapeamento de vertices das faces numeradas por elemSurface
 // super dimensionado!!!
 neighbourFaceVert.resize ( 0 );
 neighbourFaceVert.resize ( 5*numNodes );
 // contador de faces na superficie
 int count = 0; 

 for( int i=0;i<surface.Dim();i++ )
 {
  surfaceNode = surface.Get(i);
  // lista de elementos que contem surfaceNode
  plist = neighbourElem.at(surfaceNode);
  for( mele=plist.begin(); mele != plist.end();++mele )
  {
   v1 = (int) IEN.Get(*mele,0);
   v2 = (int) IEN.Get(*mele,1);
   v3 = (int) IEN.Get(*mele,2);
   v4 = (int) IEN.Get(*mele,3);

   // pegando o elemento com 3 vertices na superfice e 1 dentro da bolha
   if( cc.Get(v1)==0.5 && cc.Get(v2)==0.5 && 
	   cc.Get(v3)==0.5 && cc.Get(v4)==0 )
   {
	elemSurface.at( surfaceNode ).push_back(count);
	neighbourFaceVert.at( count ).push_back(v1);
	neighbourFaceVert.at( count ).push_back(v2);
	neighbourFaceVert.at( count ).push_back(v3);
	neighbourFaceVert.at( count ).push_back(v4);
	count++;
   }
   if( cc.Get(v1)==0.5 && cc.Get(v2)==0.5 && 
	   cc.Get(v3)==0 && cc.Get(v4)==0.5 )
   {
	elemSurface.at( surfaceNode ).push_back(count);
	neighbourFaceVert.at( count ).push_back(v4);
	neighbourFaceVert.at( count ).push_back(v1);
	neighbourFaceVert.at( count ).push_back(v2);
	neighbourFaceVert.at( count ).push_back(v3);
	count++;
   }
   if( cc.Get(v1)==0.5 && cc.Get(v2)==0 && 
	   cc.Get(v3)==0.5 && cc.Get(v4)==0.5 )
   {
	elemSurface.at( surfaceNode ).push_back(count);
	neighbourFaceVert.at( count ).push_back(v3);
	neighbourFaceVert.at( count ).push_back(v4);
	neighbourFaceVert.at( count ).push_back(v1);
	neighbourFaceVert.at( count ).push_back(v2);
	count++;
   }
   if( cc.Get(v1)==0 && cc.Get(v2)==0.5 && 
	   cc.Get(v3)==0.5 && cc.Get(v4)==0.5 )
   {
	elemSurface.at( surfaceNode ).push_back(count);
	neighbourFaceVert.at( count ).push_back(v2);
	neighbourFaceVert.at( count ).push_back(v3);
	neighbourFaceVert.at( count ).push_back(v4);
	neighbourFaceVert.at( count ).push_back(v1);
	count++;
   }

   // reordenando faces da superfice na ordem dos ponteiros do relogio 
   // (clockwise)
   list<int> plist = elemSurface.at (surfaceNode);
   for( list<int>::iterator face=plist.begin();face!=plist.end();++face )
   {
	list<int> plist2 = neighbourFaceVert.at (*face);
	list<int>::iterator vert=plist2.begin();
	int v1Old = *vert;++vert;
	int v2Old = *vert;++vert;
	int v3Old = *vert;++vert;
	int vIn = *vert; // vertice dentro da bolha
	vert=plist2.end();

	if( v1Old == surfaceNode )
	{
	 if( checkNormal(surfaceNode,v2Old,v3Old,vIn) == true )
	 {
	  neighbourFaceVert.at( *face ).clear();
	  neighbourFaceVert.at( *face ).push_back(v2Old);
	  neighbourFaceVert.at( *face ).push_back(v3Old);
	 }
	 else
	 {
	  neighbourFaceVert.at( *face ).clear();
	  neighbourFaceVert.at( *face ).push_back(v3Old);
	  neighbourFaceVert.at( *face ).push_back(v2Old);
	 }
	}
	if( v2Old == surfaceNode )
	{
	 if( checkNormal(surfaceNode,v1Old,v3Old,vIn) == true )
	 {
	  neighbourFaceVert.at( *face ).clear();
	  neighbourFaceVert.at( *face ).push_back(v1Old);
	  neighbourFaceVert.at( *face ).push_back(v3Old);
	 }
	 else
	 {
	  neighbourFaceVert.at( *face ).clear();
	  neighbourFaceVert.at( *face ).push_back(v3Old);
	  neighbourFaceVert.at( *face ).push_back(v1Old);
	 }
	}
	if( v3Old == surfaceNode )
	{
	 if( checkNormal(surfaceNode,v1Old,v2Old,vIn) == true )
	 {
	  neighbourFaceVert.at( *face ).clear();
	  neighbourFaceVert.at( *face ).push_back(v1Old);
	  neighbourFaceVert.at( *face ).push_back(v2Old);
	 }
	 else
	 {
	  neighbourFaceVert.at( *face ).clear();
	  neighbourFaceVert.at( *face ).push_back(v2Old);
	  neighbourFaceVert.at( *face ).push_back(v1Old);
	 }
	}
   }
  }
  //cout << "---------" << surfaceNode << "------------" << endl;
  //std::ostream_iterator< int > output( cout, " " );
  //std::copy( elemSurface.at(surfaceNode).begin(), elemSurface.at(surfaceNode).end(), output );
  //std::copy( neighbourFaceVert.at(surfaceNode).begin(), neighbourFaceVert.at(surfaceNode).end(), output );
  //cout << endl;
  //
//--------------------------------------------------
//   int c1=0;
//   if( surfaceNode == 29 )
//   //if( surfaceNode == 144 )
//   {
//    list<int> plist = elemSurface.at (surfaceNode);
//    for( list<int>::iterator face=plist.begin();face!=plist.end();++face )
//    {
// 	cout << "Triangulo: ------------------------- " << c1 << endl;
// 	list<int> plist2 = neighbourFaceVert.at (*face);
// 	list<int>::iterator vert=plist2.begin();
// 	int v0 = *vert;++vert;
// 	int v1 = *vert;
// 	vert=plist2.end();
// 	cout << "v0 = " << v0 << " " << "v1 = " << v1 << " " << endl;
// 
// 	real testx = ( ( Y.Get(v1)-Y.Get(v0) )*( Z.Get(v2)-Z.Get(v0) ) )-( (Z.Get(v1)-Z.Get(v0))*(Y.Get(v2)-Y.Get(v0) ) );
// 	real testy = ( ( Z.Get(v1)-Z.Get(v0) )*( X.Get(v2)-X.Get(v0) ) )-( (X.Get(v1)-X.Get(v0))*(Z.Get(v2)-Z.Get(v0) ) );
// 	real testz = ( ( X.Get(v1)-X.Get(v0) )*( Y.Get(v2)-Y.Get(v0) ) )-( (Y.Get(v1)-Y.Get(v0))*(X.Get(v2)-X.Get(v0) ) );
// 	cout << testx << endl;
// 	cout << testy << endl;
// 	cout << testz << endl;
// 
// 	if( testz < 0.0 )
// 	{
// 	 int aux = v0;
// 	 v0=v1;
// 	 v1=aux;
// 	}
// 	//cout << "v0 = " << v0 << " " << "v1 = " << v1 << " " << "v2 = " << v2 << endl;
// 	c1++;
//    }
//   }
//-------------------------------------------------- 
 }
 neighbourFaceVert.resize (count); // trim vector para numero real de itens
}

// cria matriz IEN para os elementos da superficie que no caso 3D sao
// triagulos. Este metodo utiliza neighbourFaceVert e elemSurface 
// como input e a funcao qsort para ordenacao da matriz.
void Model3D::setSurfaceTri()
{
 clVector edgeaux(3);
 IFACE3D *edge = NULL;
 int listSize = numElems;
 edge = new IFACE3D[listSize];
 list<int> plist,plist2;
 list<int>::iterator face,vert;

 int count = 0;
 for( int i=0;i<surface.Dim();i++ )
 {
  int surfaceNode = surface.Get(i);
  plist = elemSurface.at (surfaceNode);

  for(face=plist.begin();face!=plist.end();++face)
  {
   plist2 = neighbourFaceVert.at(*face);
   vert=plist2.begin();
   int v1 = *vert;++vert;
   int v2 = *vert;
   vert=plist2.end();

   edgeaux.Set(0,v1);
   edgeaux.Set(1,v2);
   edgeaux.Set(2,surfaceNode);
   edgeaux.Sort(); // para ordenar os vertices do triangulo

   edge[count].p1 = (int) edgeaux.Get(0);
   edge[count].p2 = (int) edgeaux.Get(1);
   edge[count].p3 = (int) edgeaux.Get(2);
   count++;
  }
 }
 // ordena uma estrutura (matriz) em ordem crescente na linha e coluna
 // as faces continuam repetidas neste ponto.
 qsort(edge,count,sizeof(IFACE3D),IFACE3DCompare);

 // count / 3 pois a numeracao do triangulo pode ser repetida por 3
 // vertices que sao os vertices do proprio triangulo
 IENTri.Dim(count/3.0,3);

 int it=0;
 for( int i=0;i<count/3;i++ )
 {
  IENTri.Set(i,0,edge[it].p3 );
  IENTri.Set(i,1,edge[it].p1 );
  IENTri.Set(i,2,edge[it].p2 );
  it=it+3;
 }
 delete[] edge;
}

void Model3D::setOutTri()
{
 // implementar IENTri dos pontos do convex hull
}

// este metodo cria duas listas com os vertices do convex hull (outVert)
// e todos os outros menos os vertices do convex hull (inVert). Este
// metodo eh especialmente interessante para aplicacao das condicoes de
// contorno para geometrias complexas.
void Model3D::setInOutVert()
{
 inVert.resize (0);
 outVert.resize (0);

 IENConvexTri.Dim(freeFace.DimI(),3);
 for(int i=0;i<freeFace.DimI();i++ )
 {
  int v1 = freeFace.Get(i,2);
  int v2 = freeFace.Get(i,3);
  int v3 = freeFace.Get(i,4);
  outVert.push_back(v1);
  outVert.push_back(v2);
  outVert.push_back(v3);
  IENConvexTri.Set(i,0,v1);
  IENConvexTri.Set(i,1,v2);
  IENConvexTri.Set(i,2,v3);
 }
 outVert.sort();
 outVert.unique();

 // retira de inVert todos os vertices presents em outVert.
 list<int>::iterator it;
 for( int vert=0;vert<numVerts;vert++ )
  inVert.push_back(vert);
 for( it=outVert.begin();it!=outVert.end();++it )
  inVert.remove(*it);

//--------------------------------------------------
//  cout << "outVert contains:";
//  for (it=outVert.begin(); it!=outVert.end(); ++it)
//   cout << " " << *it;
//  cout << endl;
//-------------------------------------------------- 

}
	   
// cria matrizes de mapeamentos de vizinhos opostos ao vertice em
// questao. Este metodo funciona com Semi-Lagrangian pois permite a
// procura de elementos considerando o vertice em questao. Uma descricao
// completa das matrizes eh definida dentro do proprio metodo.
void Model3D::setOFace()
{
 clMatrix mapViz(numElems,numElems);
 clMatrix faceFace(1000,numGLEP+1);
 //clMatrix freeFace(1000,numGLEP+1);
 freeFace.Dim(1000,numGLEP+1);
 clMatrix faceFaceAux;
 clMatrix freeFaceAux;
 clMatrix mapVizAux(numElems*numGLEP,numGLEP);
 clMatrix mapVizAux2(numElems*numGLEP,numGLEP);
 clMatrix comb(numGLEP,numGLEP-1);  // triangulo = 2
 clVector verts(numGLEP);           // tetraedro = 3
 clVector face(numGLEP-1);
 clVector index(numElems*numGLEP);
 clVector index2(numElems*numGLEP);
 clVector index3(numElems*numGLEP);
 clVector idcol(numElems*numGLEP);
 clVector idcol2(numElems*numGLEP);
 clVector idcol3(numElems*numGLEP);
 clVector vect1(numGLEP+1);
 clVector vert(numGLEP-1);

 //        - nome: comb
 //        - definicao:  matriz de aresta/face para um elemento de referencia
 //
 //            +---+---+---+
 //            | a | a | a |   a = identificacao dos vertices
 //  ---   +---+---+---+---+
 //   |    | b |   |   |   |   b = identificacao das faces   
 //   |    +---+---+---+---+
 //   |    | b |   |   |   |   c = quantidade de faces    
 //   c    +---+---+---+---+
 //   |    | b |   |   |   |   d = quantidade de vertices para formar face
 //   |    +---+---+---+---+
 //   |    | b |   |   |   |
 //  ---   |---+---+---+---+
 //
 //        |______ d ______|
 //        |               |
 //

 comb.Set(0,0,0);
 comb.Set(0,1,1);
 comb.Set(0,2,2);
 comb.Set(1,0,0);
 comb.Set(1,1,1);
 comb.Set(1,2,3);
 comb.Set(2,0,1);
 comb.Set(2,1,2);
 comb.Set(2,2,3);
 comb.Set(3,0,0);
 comb.Set(3,1,2);
 comb.Set(3,2,3);

 int k = 0;
 neighbourElem.resize (0);
 neighbourElem.resize (numVerts);
 for( int i=0;i<numElems;i++ )
 {
  for( int j=0;j<numGLEP;j++ )
  {
   neighbourElem.at( (int)IEN.Get(i,j) ).push_back(i);
  }

  verts.CopyRow(i,IEN);
  for( int j=0;j<numGLEP;j++ )
  {
   face.Set(0,comb.Get(j,0));
   face.Set(1,comb.Get(j,1));
   face.Set(2,comb.Get(j,2));
   vert.Set(0, (int) verts.Get( (int) face.Get(0) ) );
   vert.Set(1, (int) verts.Get( (int) face.Get(1) ) );
   vert.Set(2, (int) verts.Get( (int) face.Get(2) ) );
   vert.Sort();
   mapVizAux.Set(k,0,vert.Get(0));
   mapVizAux.Set(k,1,vert.Get(1));
   mapVizAux.Set(k,2,vert.Get(2));
   mapVizAux.Set(k,3,i);
   k++;
  }
 }

//--------------------------------------------------
//  // ------------------ HELP
//  int v1,v2,v3,v4;
//  list<int> plist5;
//  list<int>::iterator mele5;
//  for( int ii=0;ii<numVerts;ii++ )
//  {
//   if( ii== 675 )
//   {
//    plist5 = neighbourElem.at(ii);
//    cout << "surfaceNode " << ii << endl;
//    for( mele5=plist5.begin(); mele5 != plist5.end();++mele5 )
//    {
// 	v1 = (int) IEN.Get(*mele5,0);
// 	v2 = (int) IEN.Get(*mele5,1);
// 	v3 = (int) IEN.Get(*mele5,2);
// 	v4 = (int) IEN.Get(*mele5,3);
// 	cout << v1 << " " << cc.Get(v1) << " " 
// 	     << v2 << " " << cc.Get(v2) << " "
// 	     << v3 << " " << cc.Get(v3) << " "
// 	     << v4 << " " <<  cc.Get(v4) << " " << *mele5 << endl;
//    }
//    cout << endl;
//   }
//  }
//-------------------------------------------------- 
//--------------------------------------------------
//  for( int i=0;i<numVerts;i++ )
//  {
//   int surfaceNode = i;
//   if( surfaceNode == 659 )
//   {
//    cout << "---------" << surfaceNode << "------------" << endl;
//    std::ostream_iterator< int > output( cout, " " );
//    std::copy( neighbourElem.at(surfaceNode).begin(), neighbourElem.at(surfaceNode).end(), output );
//    cout << endl;
//   }
//  }
//  // ------------------ HELP
//-------------------------------------------------- 
 




 //cout << "mapVizAux: " << endl;
 //mapVizAux.Display();
 //cout << " -------------- " << endl;

 clVector faceaux(3);
 IFACE3D *faces = NULL;
 faces = new IFACE3D[4*(int)mapVizAux.DimI()];
 for( int i=0;i<mapVizAux.DimI();i++ )
 {
  int v1 = (int) mapVizAux.Get(i,0);
  int v2 = (int) mapVizAux.Get(i,1);
  int v3 = (int) mapVizAux.Get(i,2);
  int v4 = (int) mapVizAux.Get(i,3);

  faceaux.Set(0,v1);
  faceaux.Set(1,v2);
  faceaux.Set(2,v3);
  faceaux.Sort(); // ordena a linha
  faces[i].p1 = (int)faceaux(0);
  faces[i].p2 = (int)faceaux(1);
  faces[i].p3 = (int)faceaux(2);
  faces[i].p4 = v4;
 }

 // rotina em C que compara vertices de 2 elementos, ordenando-os
 qsort(faces,(int)mapVizAux.DimI(),sizeof(IFACE3D),IFACE3DCompare);

 for( int i=0;i<(int) mapVizAux.DimI();i++ )
 {
  mapVizAux2.Set(i,0,faces[i].p1);
  mapVizAux2.Set(i,1,faces[i].p2);
  mapVizAux2.Set(i,2,faces[i].p3);
  mapVizAux2.Set(i,3,faces[i].p4);
 }

 //        - nome: mapViz
 //        - definicao: matrix com mapeamento de vizinhos de cada
 //                     elemento
 //  					Cada linha da matriz so podera apresentar no 
 //  					maximo o numero de vizinhos de cada elemento, no
 //  					caso do tetraedro os valores diferentes de zero por
 //  					linha nao ultrapassa de 3. A estrutura desta matriz
 //  					deve ser esparsa.
 //
 //              +---+---+---+ ... +---+
 //              | a | a | a |     | a |
 //    ---   +---+---+---+---+ ... +---+
 //     |    | b | e | e | e |     | e |  a = identificacao do elemento
 //     |    +---+---+---+---+ ... +---+
 //     |    .   .   .   .   . ... .   .  b = identificacao do elemento viz.
 //     |	 .   .   .   .   . ... .   .
 //     c    .   .   .   .   . ... .   .  c = numero total de elementos
 //     |    +---+---+---+---+ ... +---+                               
 //     |    | b | e | e | e |     | e |  d = numero total de elementos
 //     |    +---+---+---+---+ ... +---+
 //     |    | b | e | e | e |     | e |  e = identificacao da aresta
 //    ---   +---+---+---+---+ ... +---+
 //
 //          |___________ d ___________|
 //   	     |                         |
 //
 
 //          - nome: faceFace
 //          - definicao: matrix com mapeamento de faces vizinhas de cada
 //   	                elemento. As arestas dobradas sao retiradas da
 //   					matriz mapViz. Se apresenta aresta dobrada quer 
 //   					dizer que 2 elementos possuem a mesma aresta, 
 //   					entao sao elementos vizinhos
 //
 //              +---+---+---+---+---+
 //              | a | b | c | d | e |  a = identificacao do 1o. elemento
 //    ---   +---+---+---+---+---+---+  b = identificacao do 2o. elemento
 //     |    | f |   |   |   |   |   |  c = 1o. vertice da face em comum
 //     |    +---+---+---+---+---+---+  d = 2o. vertice da face em comum
 //     |    .   .   .   .   .   .   .  e = 3o. vertice da face comum
 //     |	 .   .   .   .   .   .   .  f = ident da face dobrada
 //     g    .   .   .   .   .   .   .
 //     |    +---+---+---+---+---+---+
 //     |    | f |   |   |   |   |   |
 //     |    +---+---+---+---+---+---+  g = numero de arestas dobradas
 //     |    | f |   |   |   |   |   |  h = 2 elem + 3 vertices de aresta = 5
 //    ---   +---+---+---+---+---+---+
 //
 //          |________ h ________|
 //          |                   |
 //
 
 //        - nome: freeFace
 //          - definicao: matrix com mapeamento de faces de fronteira 
 //   	                de cada elemento, pois nao possuem arestas 
 //   					dobradas. Como a identificacao segue o 
 //   					padram da matriz faceFace, a 1a coluna eh igual a 
 //   					zero pois em condicao de contorno o elemento nao
 //   					tem vizinho
 //
 //              +---+---+---+---+---+
 //              | a | b | c | d | e |  a = identificacao do 1o. elemento = 0
 //    ---   +---+---+---+---+---+---+  b = identificacao do 2o. elemento
 //     |    | f |   |   |   |   |   |  c = 1o. vertice da face em comum
 //     |    +---+---+---+---+---+---+  d = 2o. vertice da face em comum
 //     |    .   .   .   .   .   .   .  e = 3o. vertice da face comum
 //     |	 .   .   .   .   .   .   .  f = ident da face dobrada
 //     g    .   .   .   .   .   .   .
 //     |    +---+---+---+---+---+---+
 //     |    | f |   |   |   |   |   |
 //     |    +---+---+---+---+---+---+  g = numero de arestas dobradas
 //     |    | f |   |   |   |   |   |  h = 2 elem + 3 vertices de aresta = 5
 //    ---   +---+---+---+---+---+---+
 //
 //          |________ h ________|
 //          |                   |
 //
 
 // procura de elementos que apresentam 3 vertices iguais -> faceFace
 // procura de elementos que nao possuem vizinhos -> freeFace
 mapVizAux = mapVizAux2;
 int iFace = 0;
 int iFree = 0;
 for ( int ii=0;ii<mapVizAux.DimI()-1;ii++ )
 {
  if( (mapVizAux.Get(ii,0)==mapVizAux.Get(ii+1,0)) &&
	  (mapVizAux.Get(ii,1)==mapVizAux.Get(ii+1,1)) &&  
	  (mapVizAux.Get(ii,2)==mapVizAux.Get(ii+1,2)) )  
  {
   // a matriz faceFace eh pre-alocada com 1000 linhas, caso ultrapasse
   // esse valor realoca-se para uma dimensao maior
   if( iFace == faceFace.DimI() ) 
   {
	faceFaceAux = faceFace;
	faceFace.Dim(iFace+faceFaceAux.DimI(),numGLEP+1);
	faceFace.CopyFrom(0,0,faceFaceAux);
   }
   faceFace.Set(iFace,0,mapVizAux.Get(ii+1,3));
   faceFace.Set(iFace,1,mapVizAux.Get(ii,3));
   faceFace.Set(iFace,2,mapVizAux.Get(ii,0));
   faceFace.Set(iFace,3,mapVizAux.Get(ii,1));
   faceFace.Set(iFace,4,mapVizAux.Get(ii,2));
   ii++; // pois existem 2 faces
   iFace++;
  } 
  if((ii==0) || 
	(   (ii!=0) && ( mapVizAux.Get(ii,0)!=mapVizAux.Get(ii-1,0) ||
					 mapVizAux.Get(ii,1)!=mapVizAux.Get(ii-1,1) ||     
					 mapVizAux.Get(ii,2)!=mapVizAux.Get(ii-1,2) ) ))     
  {    
   // a matriz freeFace eh pre-alocada com 1000 linhas, caso ultrapasse
   // esse valor realoca-se para uma dimensao maior
   if( iFree == freeFace.DimI() ) 
   {
	freeFaceAux = freeFace;
	freeFace.Dim(iFree+freeFaceAux.DimI(),numGLEP+1);
	freeFace.CopyFrom(0,0,freeFaceAux);
   }
   freeFace.Set(iFree,0,0);
   freeFace.Set(iFree,1,mapVizAux.Get(ii,3));
   freeFace.Set(iFree,2,mapVizAux.Get(ii,0));
   freeFace.Set(iFree,3,mapVizAux.Get(ii,1));
   freeFace.Set(iFree,4,mapVizAux.Get(ii,2));
   iFree++;
  }
 }
 int ii = mapVizAux.DimI()-1;
 if( ( mapVizAux.Get(ii,0)!=mapVizAux.Get(ii-1,0) ||
	   mapVizAux.Get(ii,1)!=mapVizAux.Get(ii-1,1) ||     
	   mapVizAux.Get(ii,2)!=mapVizAux.Get(ii-1,2) ) )     
 {
   if( iFree == freeFace.DimI() ) 
   {
	freeFaceAux = freeFace;
	freeFace.Dim(iFree+freeFaceAux.DimI(),numGLEP+1);
	freeFace.CopyFrom(0,0,freeFaceAux);
   }
   freeFace.Set(iFree,0,0);
   freeFace.Set(iFree,1,mapVizAux.Get(ii,3));
   freeFace.Set(iFree,2,mapVizAux.Get(ii,0));
   freeFace.Set(iFree,3,mapVizAux.Get(ii,1));
   freeFace.Set(iFree,4,mapVizAux.Get(ii,2));
   iFree++;
 }

 // como as matrizes faceFace e freeFace possuem dimensoes variaveis,
 // precisa-se aloca-las com um numero maior que o necessario. Apos suas
 // atribuicoes, necessita-se dimensiona-las para o tamanho final
 // redimensionalizando...
 faceFaceAux = faceFace;
 freeFaceAux = freeFace;
 faceFace.Dim(iFace,numGLEP+1);
 freeFace.Dim(iFree,numGLEP+1);
 faceFaceAux.CopyTo(0,0,faceFace);
 freeFaceAux.CopyTo(0,0,freeFace);

 mapVizAux.Dim(0,0);
 mapVizAux2.Dim(0,0);

 //        - nome: oFace
 //        - definicao: matrix com mapeamento de elementos opostos ao 
 //   	                vertice em questao. Se encontrar o valor (-1) quer
 //   					dizer que o vertice nao apresenta elemento oposto,
 //   					ou seja, eh um elemento de fronteira
 //
 //              +---+---+---+---+
 //              | a | a | a | a |  a = identificacao dos vertices do elemento
 //    ---   +---+---+---+---+---+      seguindo a ordem local 0-1-2-3
 //     |    | b | e | e | e | e |  
 //     |    +---+---+---+---+---+  b = idendificacao dos elementos
 //     |    .   .   .   .   .   .  
 //     |	 .   .   .   .   .   .  e = identificacao do elemento oposto ao 
 //     c    .   .   .   .   .   .      vertice
 //     |    +---+---+---+---+---+
 //     |    | b | e | e | e | e |
 //     |    +---+---+---+---+---+  c = numero total de elementos
 //     |    | b | e | e | e | e |  d = numero de total de vertices do elemento 
 //    ---   +---+---+---+---+---+
 //
 //          |________ d ________|
 //     	 |                   |
 
 oFace.Dim(numElems,numGLEP);
 for( int i=0;i<numElems;i++ )
  for( int j=0;j<numGLEP;j++ )
   oFace.Set(i,j,-1);

 clVector tetra(IEN.DimJ());

 for( int ii=0;ii<faceFace.DimI();ii++ )
 {
  int elem1 = (int) faceFace.Get(ii,0);
  int elem2 = (int) faceFace.Get(ii,1);
  vert.Set(0, (int) faceFace.Get(ii,2) );
  vert.Set(1, (int) faceFace.Get(ii,3) );
  vert.Set(2, (int) faceFace.Get(ii,4) );

  // armazena o numero da aresta + 1, para manter a matriz esparsa
  mapViz.Set(elem1,elem2,ii+1);
  mapViz.Set(elem2,elem1,ii+1);

  tetra.CopyRow(elem1,IEN);

  for( int kk=0;kk<numGLEP;kk++)
   if( (tetra.Get(kk) != vert.Get(0)) && 
	   (tetra.Get(kk) != vert.Get(1)) && 
	   (tetra.Get(kk) != vert.Get(2)) )
   {
	oFace.Set(elem1,kk,elem2);
	break;
   }

  tetra.CopyRow(elem2,IEN);
  for ( int kk=0;kk<numGLEP;kk++ )
   if( (tetra.Get(kk) != vert.Get(0)) && 
	   (tetra.Get(kk) != vert.Get(1)) && 
	   (tetra.Get(kk) != vert.Get(2)) )
   {
	oFace.Set(elem2,kk,elem1);
	break;
   }
 }

 // correcao da oFace (funcionando para malha step40-20-10)
 int vec[3];
 int v[4];
 int aresta = 0;
 int jAresta = 0;
 int vertOp1 = 0;
 int vertOp2 = 0;
 int kFace = 0;
 int nele;
 int count;
 list<int> plist;
 list<int>::iterator mele; // definicao do iterador
 for( int i=0;i<freeFace.DimI();i++ )
 {
  nele = (int) freeFace.Get(i,1);
  vec[0] = (int) freeFace.Get(i,2); 
  vec[1] = (int) freeFace.Get(i,3); 
  vec[2] = (int) freeFace.Get(i,4); 
  for( int j=0;j<numGLEP-1;j++ )
  {
   ii = (int) freeFace.Get(i,j+2);
   plist = neighbourElem.at(ii);
   for (mele=plist.begin(); mele != plist.end(); ++mele)
   {
	v[0] = (int) IEN.Get(*mele,0);
	v[1] = (int) IEN.Get(*mele,1);
	v[2] = (int) IEN.Get(*mele,2);
	v[3] = (int) IEN.Get(*mele,3);
	count = 0;
	for( int m=0;m<3;m++ )
	 for( int n=0;n<4;n++ )
	  if( (vec[m] == v[n]) && (vec[m] != ii) ){ aresta = vec[m];count++; }
	  
	if( count == 1 ) // aresta localizada!
	{
	 for( int m=0;m<3;m++ )
	  if( vec[m] == aresta ) jAresta = m; 
	  else if( vec[m] != ii ) kFace = m; 
	 
	 for( int m=0;m<3;m++ )
	 {
	  if( (v[m] != ii) && (v[m] != aresta) )
	  {
	   if( testFace(ii,aresta,vec[kFace],v[m]) )
	   {
		for( int n=0;n<4;n++ )
		{
		 if( (IEN.Get(nele,n) != ii) && 
		     (IEN.Get(nele,n) != aresta) && 
			 (IEN.Get(nele,n) != vec[kFace]) )  
		  vertOp1 = n;
		}
		for( int n=0;n<4;n++ )
		{
		 if( (IEN.Get(*mele,n) != ii) && 
		     (IEN.Get(*mele,n) != aresta) && 
			 (IEN.Get(*mele,n) != v[m]) )  
		  vertOp2 = n;
		}
		if( oFace.Get(nele,vertOp1) == -1 )
		 oFace.Set(nele,vertOp1,*mele);

		if( oFace.Get(*mele,vertOp2) == -1 )
		 oFace.Set(*mele,vertOp2,nele);
	   }
	  }
	 }
	}
   }
  }
 }
 delete[] faces;

}

// aplica configuracoes referentes a superficie da modelagem 2 fases.
void Model3D::setSurfaceConfig()
{
 setVertNeighbour(); // neighbourVert
 setInOutVert(); // inVert e outVert
 setSurface(); // surface e nonSurface
 setSurfaceFace(); // elemSurface e neighbourFaceVert
 setSurfaceTri(); // IENTri para superficie
}

bool Model3D::testFace(int v1, int v2, int v3, int v4)
{
 real V,Ax1,Ax2,Ay1,Ay2,Az1,Az2;
 real prodEsc;

  V = (-1.0/6.0) * (+1*( (X.Get(v2)*Y.Get(v3)*Z.Get(v4)) 
	                  +(Y.Get(v2)*Z.Get(v3)*X.Get(v4)) 
	                  +(Z.Get(v2)*X.Get(v3)*Y.Get(v4)) 
	                  -(Y.Get(v2)*X.Get(v3)*Z.Get(v4)) 
	                  -(X.Get(v2)*Z.Get(v3)*Y.Get(v4)) 
	                  -(Z.Get(v2)*Y.Get(v3)*X.Get(v4)) )
	      -X.Get(v1)*( +Y.Get(v3)*Z.Get(v4)
		               +Y.Get(v2)*Z.Get(v3) 
			           +Z.Get(v2)*Y.Get(v4)
		               -Y.Get(v2)*Z.Get(v4)
					   -Z.Get(v3)*Y.Get(v4) 
					   -Z.Get(v2)*Y.Get(v3) )
	      +Y.Get(v1)*( +X.Get(v3)*Z.Get(v4)
					   +X.Get(v2)*Z.Get(v3)
					   +Z.Get(v2)*X.Get(v4)
		               -X.Get(v2)*Z.Get(v4)
					   -Z.Get(v3)*X.Get(v4) 
					   -Z.Get(v2)*X.Get(v3) )
		  -Z.Get(v1)*( +X.Get(v3)*Y.Get(v4)
			           +X.Get(v2)*Y.Get(v3) 
					   +Y.Get(v2)*X.Get(v4)
		               -X.Get(v2)*Y.Get(v4)
				       -Y.Get(v3)*X.Get(v4) 
					   -Y.Get(v2)*X.Get(v3) ) );

  if( fabs(V)<1e-10)
  {
     Az1=0.5*(((X.Get(v2)-X.Get(v1))*(Y.Get(v3)-Y.Get(v1)))
	         -((X.Get(v3)-X.Get(v1))*(Y.Get(v2)-Y.Get(v1))));

     Ay1=0.5*(((X.Get(v2)-X.Get(v1))*(Z.Get(v3)-Z.Get(v1)))
	         -((X.Get(v3)-X.Get(v1))*(Z.Get(v2)-Z.Get(v1))));

     Ax1=0.5*(((Z.Get(v2)-Z.Get(v1))*(Y.Get(v3)-Y.Get(v1)))
	         -((Z.Get(v3)-Z.Get(v1))*(Y.Get(v2)-Y.Get(v1))));

     Az2=0.5*(((X.Get(v2)-X.Get(v1))*(Y.Get(v4)-Y.Get(v1)))
	         -((X.Get(v4)-X.Get(v1))*(Y.Get(v2)-Y.Get(v1))));

     Ay2=0.5*(((X.Get(v2)-X.Get(v1))*(Z.Get(v4)-Z.Get(v1)))
	         -((X.Get(v4)-X.Get(v1))*(Z.Get(v2)-Z.Get(v1))));

     Ax2=0.5*(((Z.Get(v2)-Z.Get(v1))*(Y.Get(v4)-Y.Get(v1)))
	         -((Z.Get(v4)-Z.Get(v1))*(Y.Get(v2)-Y.Get(v1))));

	 prodEsc = Az1*Az2 + Ay1*Ay2 + Ax1*Ax2;
	 return (prodEsc>0);
  }
  else
   return false;
}

// monta um vetor de comprimento de arestas do elemento de menor area e 
// retorna o menor deltaX diferente de 0
// o valor de deltaX retornado nao e necessariamente o menor deltaX da malha,
// porem para malhas regulares o valor tende a ser o menor deltaX da malha
real Model3D::getDeltaXMin()
{
 int v[4];
 int elemIndex=(int) V.MinIndex();
 clVector deltaX(4);

 v[0]=(int) IEN.Get(elemIndex,0);
 v[1]=(int) IEN.Get(elemIndex,1);
 v[2]=(int) IEN.Get(elemIndex,2);
 v[3]=(int) IEN.Get(elemIndex,3);

 deltaX.Set(0,fabs(X.Get(v[0])-X.Get(v[1])));
 deltaX.Set(1,fabs(X.Get(v[1])-X.Get(v[2])));
 deltaX.Set(2,fabs(X.Get(v[2])-X.Get(v[3])));
 deltaX.Set(3,fabs(X.Get(v[3])-X.Get(v[0])));
 deltaX.Sort();

 if( deltaX.Get(0) == 0.0 )
  if( deltaX.Get(1) == 0.0 )
   return deltaX.Get(2);
  else 
   return deltaX.Get(1);
 else
  return deltaX.Get(0);
}

// monta um vetor de comprimento de arestas do elemento de menor area e 
// retorna o menor deltaY diferente de 0
// o valor de deltaY retornado nao e necessariamente o menor deltaY da malha,
// porem para malhas regulares o valor tende a ser o menor deltaY da malha
real Model3D::getDeltaYMin()
{
 int v[4];
 int elemIndex=(int) V.MinIndex();
 clVector deltaY(4);

 v[0] = (int) IEN.Get(elemIndex,0);
 v[1] = (int) IEN.Get(elemIndex,1);
 v[2] = (int) IEN.Get(elemIndex,2);
 v[3] = (int) IEN.Get(elemIndex,3);

 deltaY.Set(0,fabs(Y.Get(v[0])-Y.Get(v[1])));
 deltaY.Set(1,fabs(Y.Get(v[1])-Y.Get(v[2])));
 deltaY.Set(2,fabs(Y.Get(v[2])-Y.Get(v[3])));
 deltaY.Set(3,fabs(Y.Get(v[3])-Y.Get(v[0])));
 deltaY.Sort();

 if( deltaY.Get(0) == 0.0 )
  if( deltaY.Get(1) == 0.0 )
   return deltaY.Get(2);
  else 
   return deltaY.Get(1);
 else
  return deltaY.Get(0);
}

// monta um vetor de comprimento de arestas do elemento de menor area e 
// retorna o menor deltaZ diferente de 0
// o valor de deltaZ retornado nao e necessariamente o menor deltaZ da malha,
// porem para malhas regulares o valor tende a ser o menor deltaZ da malha
real Model3D::getDeltaZMin()
{
 int v[4];
 int elemIndex=(int) V.MinIndex();
 clVector deltaZ(4);

 v[0] = (int) IEN.Get(elemIndex,0);
 v[1] = (int) IEN.Get(elemIndex,1);
 v[2] = (int) IEN.Get(elemIndex,2);
 v[3] = (int) IEN.Get(elemIndex,3);

 deltaZ.Set(0,fabs(Y.Get(v[0])-Y.Get(v[1])));
 deltaZ.Set(1,fabs(Y.Get(v[1])-Y.Get(v[2])));
 deltaZ.Set(2,fabs(Y.Get(v[2])-Y.Get(v[3])));
 deltaZ.Set(3,fabs(Y.Get(v[3])-Y.Get(v[0])));
 deltaZ.Sort();

 if( deltaZ.Get(0) == 0.0 )
  if( deltaZ.Get(1) == 0.0 )
   return deltaZ.Get(2);
  else 
   return deltaZ.Get(1);
 else
  return deltaZ.Get(0);
}

real Model3D::getMaxAbsUC()
{
 clVector aux = uc.Abs();
 real r = aux.Max();
 return r;
}

real Model3D::getMinAbsUC()
{
 clVector aux = uc.Abs();
 real r = aux.Min();
 return r;
}

real Model3D::getMaxAbsVC()
{
 clVector aux = vc.Abs();
 real r = aux.Max();
 return r;
}

real Model3D::getMinAbsVC()
{
 clVector aux = vc.Abs();
 real r = aux.Min();
 return r;
}

real Model3D::getMaxAbsWC()
{
 clVector aux = wc.Abs();
 real r = aux.Max();
 return r;
}

real Model3D::getMinAbsWC()
{
 clVector aux = wc.Abs();
 real r = aux.Min();
 return r;
}

real Model3D::getVolume(int _elem)
{
 int v1=(int)IEN.Get(_elem,0);
 int v2=(int)IEN.Get(_elem,1);
 int v3=(int)IEN.Get(_elem,2);
 int v4=(int)IEN.Get(_elem,3);

 real volume = (-1.0/6.0) * (+1*( (X.Get(v2)*Y.Get(v3)*Z.Get(v4)) 
	                             +(Y.Get(v2)*Z.Get(v3)*X.Get(v4)) 
								 +(Z.Get(v2)*X.Get(v3)*Y.Get(v4)) 
								 -(Y.Get(v2)*X.Get(v3)*Z.Get(v4)) 
								 -(X.Get(v2)*Z.Get(v3)*Y.Get(v4)) 
								 -(Z.Get(v2)*Y.Get(v3)*X.Get(v4)) )
	                 -X.Get(v1)*( +Y.Get(v3)*Z.Get(v4)
		                          +Y.Get(v2)*Z.Get(v3) 
								  +Z.Get(v2)*Y.Get(v4)
								  -Y.Get(v2)*Z.Get(v4)
								  -Z.Get(v3)*Y.Get(v4) 
								  -Z.Get(v2)*Y.Get(v3) )
					 +Y.Get(v1)*( +X.Get(v3)*Z.Get(v4)
	            			      +X.Get(v2)*Z.Get(v3)
								  +Z.Get(v2)*X.Get(v4)
								  -X.Get(v2)*Z.Get(v4)
								  -Z.Get(v3)*X.Get(v4) 
								  -Z.Get(v2)*X.Get(v3) )
					 -Z.Get(v1)*( +X.Get(v3)*Y.Get(v4)
		      	                  +X.Get(v2)*Y.Get(v3) 
								  +Y.Get(v2)*X.Get(v4)
								  -X.Get(v2)*Y.Get(v4)
								  -Y.Get(v3)*X.Get(v4) 
								  -Y.Get(v2)*X.Get(v3) ) );
 return volume;
}

real Model3D::getArea(int _elem)
{
 int v1=(int)IENOriginal.Get(_elem,0);
 int v2=(int)IENOriginal.Get(_elem,1);
 int v3=(int)IENOriginal.Get(_elem,2);

 
 // vectors
 real x1 = X.Get(v2) - X.Get(v1);
 real y1 = Y.Get(v2) - Y.Get(v1);
 real z1 = Z.Get(v2) - Z.Get(v1);

 real x2 = X.Get(v3) - X.Get(v1);
 real y2 = Y.Get(v3) - Y.Get(v1);
 real z2 = Z.Get(v3) - Z.Get(v1);

 real crossX = (y1*z2)-(z1*y2);
 real crossY = -( (x1*z2)-(z1*x2) );
 real crossZ = (x1*y2)-(y1*x2);

 real area = 0.5*sqrt( crossX*crossX+crossY*crossY+crossZ*crossZ );

 return area;
}

real Model3D::getAreaHeron(int _elem)
{
 int v1=(int)IENOriginal.Get(_elem,0);
 int v2=(int)IENOriginal.Get(_elem,1);
 int v3=(int)IENOriginal.Get(_elem,2);

 real a = sqrt( (X.Get(v2) - X.Get(v1))*(X.Get(v2) - X.Get(v1)) +
                (Y.Get(v2) - Y.Get(v1))*(Y.Get(v2) - Y.Get(v1)) +
				(Z.Get(v2) - Z.Get(v1))*(Z.Get(v2) - Z.Get(v1)) );

 real b = sqrt( (X.Get(v3) - X.Get(v1))*(X.Get(v3) - X.Get(v1)) +
                (Y.Get(v3) - Y.Get(v1))*(Y.Get(v3) - Y.Get(v1)) +
				(Z.Get(v3) - Z.Get(v1))*(Z.Get(v3) - Z.Get(v1)) );

 real c = sqrt( (X.Get(v3) - X.Get(v2))*(X.Get(v3) - X.Get(v2)) +
                (Y.Get(v3) - Y.Get(v2))*(Y.Get(v3) - Y.Get(v2)) +
				(Z.Get(v3) - Z.Get(v2))*(Z.Get(v3) - Z.Get(v2)) );

 real s = (a+b+c)/2.0;

 real area = sqrt( s*(s-a)*(s-b)*(s-c) );

 return area;
}

void Model3D::clearBC()
{
 uc.Dim(numNodes);
 vc.Dim(numNodes);
 wc.Dim(numNodes);
 pc.Dim(numVerts);
 idbcu.Dim(0);
 idbcv.Dim(0);
 idbcw.Dim(0);
 idbcp.Dim(0);
 // nos metodos com Concentracao cc.Dim(numVerts) esta definido, nao
 // precisando defini-lo aqui.
 //cc.Dim(numVerts);
 outflow.Dim(numNodes,1); // usado no metodo Galerkin
}

// re-allocation of vectors and IEN matrix
void Model3D::reAllocStruct()
{
 clVector aux;
 aux = X;
 X.Dim(numNodes);
 X.CopyFrom(0,aux);
 aux = Y;
 Y.Dim(numNodes);
 Y.CopyFrom(0,aux);
 aux = Z;
 Z.Dim(numNodes);
 Z.CopyFrom(0,aux);
 clMatrix IENaux;
 IENaux = IEN;
 IEN.Dim(numElems,numGLEU); // 4 nos por elemento + 1 centroide
 IEN.CopyFrom(0,0,IENaux);
}

bool Model3D::checkNormal(int _surfaceNode,int _v1,int _v2,int _vIn)
{
 real xSurfaceNode = X.Get(_surfaceNode);
 real ySurfaceNode = Y.Get(_surfaceNode);
 real zSurfaceNode = Z.Get(_surfaceNode);
 real xV1 = X.Get(_v1);
 real yV1 = Y.Get(_v1);
 real zV1 = Z.Get(_v1);
 real xV2 = X.Get(_v2);
 real yV2 = Y.Get(_v2);
 real zV2 = Z.Get(_v2);
 real xIn = X.Get(_vIn);
 real yIn = Y.Get(_vIn);
 real zIn = Z.Get(_vIn);

 // vetor surfaceNode -> in
 real x1 = xIn - xSurfaceNode;
 real y1 = yIn - ySurfaceNode;
 real z1 = zIn - zSurfaceNode;
 
 // vetores surfaceNode -> (v1 e v2)
 real xVec1 = xV1 - xSurfaceNode; 
 real yVec1 = yV1 - ySurfaceNode; 
 real zVec1 = zV1 - zSurfaceNode; 
 real xVec2 = xV2 - xSurfaceNode; 
 real yVec2 = yV2 - ySurfaceNode; 
 real zVec2 = zV2 - zSurfaceNode; 

 // produto vetorial: surfaceNode -> v1 X surfaceNode -> v2
 real x2 = (yVec1*zVec2)-(zVec1*yVec2);
 real y2 = -( (xVec1*zVec2)-(zVec1*xVec2) );
 real z2 = (xVec1*yVec2)-(yVec1*xVec2);

 // produto escalar para saber se os vetores estao no mesmo sentido
 real prodEsc = x1*x2 + y1*y2 + z1*z2;

 if( prodEsc < 0 )
  return true;
 else
  return false;
}

clVector* Model3D::getX(){ return &X; }
real Model3D::getMaxX(){ return X.Max(); }
real Model3D::getMinX(){ return X.Min(); }
void Model3D::setX(clVector _X){ X = _X; }
clVector* Model3D::getY(){ return &Y; }
real Model3D::getMinY(){ return Y.Min(); }
real Model3D::getMaxY(){ return Y.Max(); }
void Model3D::setY(clVector _Y){ Y = _Y; }
real Model3D::getMaxZ(){ return Z.Max(); }
real Model3D::getMinZ(){ return Z.Min(); }
clVector* Model3D::getZ(){ return &Z; }
void Model3D::setZ(clVector _Z){ Z = _Z; }
clVector* Model3D::getUC(){ return &uc; }
clVector* Model3D::getVC(){ return &vc; }
clVector* Model3D::getWC(){ return &wc; }
clVector* Model3D::getPC(){ return &pc; }
clVector* Model3D::getCC(){ return &cc; }
clVector* Model3D::getOutflow(){ return &outflow; }
clVector* Model3D::getIdbcu(){ return &idbcu; }
clVector* Model3D::getIdbcv(){ return &idbcv; }
clVector* Model3D::getIdbcw(){ return &idbcw; }
clVector* Model3D::getIdbcp(){ return &idbcp; }
clVector* Model3D::getIdbcc(){ return &idbcc; }
clMatrix* Model3D::getIEN(){ return &IEN; }
clMatrix* Model3D::getIENTri(){ return &IENTri; }
clMatrix* Model3D::getIENConvexTri(){ return &IENConvexTri; }
int Model3D::getNumVerts(){ return numVerts; }
int Model3D::getNumNodes(){ return numNodes; }
int Model3D::getNumElems(){ return numElems; }
int Model3D::getNumGLEU(){ return numGLEU; }
int Model3D::getNumGLEP(){ return numGLEP; }
int Model3D::getNumGLEC(){ return numGLEC; }
//clMatrix Model3D::getMapViz(){ return mapViz; }
//clMatrix Model3D::getFaceFace(){ return faceFace; }
clMatrix* Model3D::getOFace(){ return &oFace; }
real Model3D::getXCenter(){ return xCenter; }
real Model3D::getYCenter(){ return yCenter; }
real Model3D::getZCenter(){ return zCenter; }
real Model3D::getBubbleRadius(){ return bubbleRadius; }
clVector* Model3D::getSurface(){ return &surface; }
vector< list<int> >* Model3D::getNeighbourElem(){return &neighbourElem;}
vector< list<int> >* Model3D::getNeighbourVert(){return &neighbourVert;}
vector< list<int> >* Model3D::getNeighbourFace(){return &neighbourFace;}
vector< list<int> >* Model3D::getElemSurface(){return &elemSurface;}
vector< list<int> >* Model3D::getNeighbourFaceVert(){return &neighbourFaceVert;}
vector< list<int> >* Model3D::getSurfaceViz(){return &surfaceViz;}
vector< list<int> >* Model3D::getFaceIEN(){return &faceIEN;}
list<int>* Model3D::getInVert(){return &inVert;}
list<int>* Model3D::getOutVert(){return &outVert;}
list<int>* Model3D::getInElem(){return &inElem;}
list<int>* Model3D::getOutElem(){return &outElem;}

//-------------------------------------------------- 
// Atribui o Model3D do argumento no corrente
//-------------------------------------------------- 
void Model3D::operator=(Model3D &_mRight) 
{
  // ints and floats
  numVerts = _mRight.numVerts;
  numNodes = _mRight.numNodes;
  numGLEU = _mRight.numGLEU;
  numGLEP = _mRight.numGLEP;
  numGLEC = _mRight.numGLEC;
  rMax = _mRight.rMax;
  xCenter = _mRight.xCenter;
  yCenter = _mRight.yCenter;
  zCenter = _mRight.zCenter;
  bubbleRadius = _mRight.bubbleRadius;

  // clVector and clMatrix
  surface = _mRight.surface;
  nonSurface = _mRight.nonSurface;
  uc = _mRight.uc;
  vc = _mRight.vc;
  wc = _mRight.wc;
  pc = _mRight.pc;
  cc = _mRight.cc;
  X = _mRight.X;
  Y = _mRight.Y;
  Z = _mRight.Z;
  outflow = _mRight.outflow;
  idbcu = _mRight.idbcu;
  idbcv = _mRight.idbcv;
  idbcw = _mRight.idbcw;
  idbcp = _mRight.idbcp;
  idbcc = _mRight.idbcc;
  V = _mRight.V;
  IEN = _mRight.IEN;
  IENTri = _mRight.IENTri;
  IENConvexTri = _mRight.IENConvexTri;
  faceFace = _mRight.faceFace;
  freeFace = _mRight.freeFace;
  mapViz = _mRight.mapViz;
  oFace = _mRight.oFace;

  // STL: list and vectors
  neighbourElem = _mRight.neighbourElem; 
  neighbourVert = _mRight.neighbourVert;
  neighbourFace = _mRight.neighbourFace;
  faceIEN = _mRight.faceIEN;
  neighbourFaceVert = _mRight.neighbourFaceVert;
  elemSurface = _mRight.elemSurface;
  surfaceViz = _mRight.surfaceViz;
  xSurfaceViz = _mRight.xSurfaceViz;
  ySurfaceViz = _mRight.ySurfaceViz;
  zSurfaceViz = _mRight.zSurfaceViz;
  outVert = _mRight.outVert;
  inVert = _mRight.inVert;
  outElem = _mRight.outElem;
  inElem = _mRight.inElem;
}


