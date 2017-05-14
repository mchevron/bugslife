# Compilateur a utiliser :
##########################

CC     = gcc
CPP    = g++

# Options de compilation : (-g = pour deverminer)
#################################################

CFLAGS = -g


# Librairies a utiliser :
# LIBS: toutes les librairies qu'il faut intégrer
#########################

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
# LINUX LIBS:
LIBS   = -lglui -lglut -lGL -lGLU -lm  -L/usr/X11R6/lib -lX11 -lXext -lXmu -lXi -L/usr/lib64 -lstdc++
else
# MAC LIBS:
LIBS   = -framework OpenGL -framework GLUT -framework GLUI -lm -lstdc++
endif

# Liste de fichiers source (.c) a compiler :
############################################

CFILES = modele.c error.c fourmi.c fourmiliere.c nourriture.c utilitaire.c main.cpp graphic.c

# Liste de modules objets (generee a partir de CFILES) :
# Les fichiers objets (.o) ont le même nom que les fichiers source (.c)
# Seulement le suffixe change.
########################################################

OFILES = $(CFILES:.c=.o)

# Nom du fichier executable :
#############################

CIBLE  = rendu2.x

# edition de liens (rassembler tous les objets -> executable)
#############################################################

rendu2.x: $(OFILES) 
	 $(CC) $(OFILES) ${LIBS} -o $(CIBLE) 

# Definitions de cibles particulieres :
#
# "make depend" : genere la liste des dependances
# "make clean"  : efface les fichiers .o et .x
#################################################

depend:
	@echo " *** MISE A JOUR DES DEPENDANCES ***"
	@(sed '/^# DO NOT DELETE THIS LINE/q' Makefile && \
	  $(CC) -MM $(CFLAGS) $(CFILES) | \
	  egrep -v "/usr/include" \
	 ) >Makefile.new
	@mv Makefile.new Makefile

clean:
	@echo " *** EFFACE MODULES OBJET ET EXECUTABLE ***"
	@/bin/rm -f *.o *.x *.c~ *.h~

#
# -- Regles de dependances generees par "make depend"
#####################################################
# DO NOT DELETE THIS LINE
modele.o: modele.c fourmiliere.h constantes.h tolerance.h error.h \
  nourriture.h fourmi.h modele.h
error.o: error.c constantes.h tolerance.h error.h
fourmi.o: fourmi.c nourriture.h constantes.h tolerance.h error.h \
  fourmiliere.h graphic.h utilitaire.h fourmi.h
fourmiliere.o: fourmiliere.c nourriture.h constantes.h tolerance.h \
  error.h fourmi.h graphic.h utilitaire.h fourmiliere.h
nourriture.o: nourriture.c fourmiliere.h constantes.h tolerance.h error.h \
  graphic.h utilitaire.h nourriture.h
utilitaire.o: utilitaire.c fourmiliere.h constantes.h tolerance.h error.h \
  nourriture.h fourmi.h utilitaire.h
main.o: main.cpp modele.h constantes.h tolerance.h
graphic.o: graphic.c constantes.h tolerance.h graphic.h
