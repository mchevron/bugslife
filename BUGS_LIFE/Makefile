# Compilateur a utiliser :
##########################

CC     = gcc
CPP    = g++

# Options de compilation : (-g = pour deverminer)
#################################################

CFLAGS = -Wall


# Librairies a utiliser :
# LIBS: toutes les librairies qu'il faut intégrer
#########################

# LINUX LIBS:
LIBS   = -lglut -lGL -lGLU -lm -Llib -lglui -L/usr/X11R6/lib -lX11 -lXext -lXmu -lXi

# MAC LIBS:
#LIBS   = -framework OpenGL -framework GLUT -framework GLUI -lm -lstdc++

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
	  $(CC) $(OFILES) -o $(CIBLE) ${LIBS}

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
  utilitaire.h nourriture.h fourmi.h modele.h
error.o: error.c error.h constantes.h tolerance.h
fourmi.o: fourmi.c error.h fourmi.h constantes.h tolerance.h utilitaire.h \
  fourmiliere.h modele.h
fourmiliere.o: fourmiliere.c error.h constantes.h tolerance.h fourmi.h \
  utilitaire.h fourmiliere.h modele.h
nourriture.o: nourriture.c error.h constantes.h tolerance.h nourriture.h \
  modele.h
utilitaire.o: utilitaire.c fourmiliere.h constantes.h tolerance.h error.h \
  utilitaire.h nourriture.h fourmi.h
main.o: main.cpp graphic.h modele.h constantes.h tolerance.h
graphic.o: graphic.cpp graphic.h constantes.h tolerance.h
