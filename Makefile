#################################
# HAM Makefile
#################################

include $(HAMDIR)/system/master.mak

#
# Set the name of your desired GBA image name here
#
PROGNAME = FuriousAdvanceTracker_v0.3.0

#
# Set a list of files you want to compile 
# Please let the libgbfs.o at last position. IMPORTANT !
# 
OFILES += main.o soundApi/soundApi.o soundApi/libgbfs.o

######################################
# Standard Makefile targets start here
######################################

all : $(PROGNAME).$(EXT) clean

#
# Most Makefile targets are predefined for you, suchas
# vba, clean ... in the following file
#
include $(HAMDIR)/system/standard-targets.mak

######################################
# custom  Makefile targets start here
######################################


gfx: makefile
	gfx2gba -t8 -m -fsrc gfx\\*.bmp
	gfx2gba -t8 -m -rs -fsrc gfx\\facerot.bmp
	



