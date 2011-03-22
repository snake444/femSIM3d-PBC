## =================================================================== ##
## this is file bubble3d, created at 10-Jun-2009                       ##
## maintained by Gustavo Rabello dos Anjos                             ##
## e-mail: gustavo.rabello@gmail.com                                   ##
## =================================================================== ##


TARGET = bubble 
LIBDIR = ../lib
CXX = g++
CXXFLAGS = -O1 -g
LIBS += -lgsl -lgslcblas -lm
LIBS += -L$(HOME)/Programs/tetgen/1.4.3 -ltet
INCLUDES += -I.
INCLUDES += -I$(LIBDIR) 
INCLUDES += -I/opt/local/include
INCLUDES += -I$(HOME)/Programs/tetgen/1.4.3
INCLUDES += -I${PETSC_DIR}/include
INCLUDES += -I$(HOME)/Programs/triangle

OBJECTS += $(HOME)/Programs/triangle/triangle.o
OBJECTS += $(LIBDIR)/clVector.o
OBJECTS += $(LIBDIR)/clMatrix.o
OBJECTS += $(LIBDIR)/clDMatrix.o
OBJECTS += $(LIBDIR)/PCGSolver.o
OBJECTS += $(LIBDIR)/GMRes.o
#OBJECTS += $(LIBDIR)/Mumps_Petsc.o
OBJECTS += $(LIBDIR)/PetscSolver.o
OBJECTS += $(LIBDIR)/FEMLinElement3D.o
OBJECTS += $(LIBDIR)/FEMMiniElement3D.o
OBJECTS += Solver.o
OBJECTS += Model3D.o
OBJECTS += TElement.o
OBJECTS += Galerkin.o
OBJECTS += Interface3D.o
OBJECTS += MeshSmooth.o
OBJECTS += SemiLagrangean.o
OBJECTS += Simulator3D.o
OBJECTS += InOut.o

#OBJECTS += mainBubble.o
#OBJECTS += mainAnnular.o
OBJECTS += main2Bubble.o

$(TARGET): $(OBJECTS)
	-${CLINKER} $(OBJECTS) $(LIBS) ${PETSC_KSP_LIB} -o $(TARGET)

%.o : %.cpp $(wildcard *.h)
	$(CXX) $(INCLUDES) -c $< $(CXXFLAGS) -o $@

# Petsc new config
include ${PETSC_DIR}/conf/variables
include ${PETSC_DIR}/conf/rules

deepclean:
	@rm -f core
	@find $(LIBDIR) -name "*.o" -exec rm {} \;
	@find . -name "*.o" -exec rm {} \;
	@find . -name "*~" -exec rm {} \;
	@rm -f $(TARGET)
	@rm -f ./vtk/*.vtk
	@rm -f ./sim/vk*.dat
	@rm -f ./sim/sim*.dat
	@rm -f ./bin/*.bin
	@rm -f ./bin/*.dat
	@rm -f ./*.dat
	@rm -f ./relatorio.dat
	@rm -f ./info.dat

