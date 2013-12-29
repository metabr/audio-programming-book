# top-level makefile for DVD Ch 4 programs
# to include building the test programs, uncomment
# the lines below, and add tests to both the .PHONY list
# and the all: list
#
#Êthe .PHONY directive is used for targets where no dependencies
#  are, or can be, specified.
# It enables a makefile to be used a little like a shell script or batch file

.PHONY:	portsf vdelaypp siggenpp clean veryclean

all:	portsf vdelaypp siggenpp

portsf:
	cd portsf; make; make install;

vdelaypp:
	cd vdelaycpp; make;

#tests:
#	cd tests; make

siggenpp:	
	cd siggencpp; make

clean: 
	cd portsf; make clean
	cd siggencpp; make clean
	cd vdelaycpp; make clean
#	cd tests; make clean

# NB this will not remove libportsf.a from the lib directory
veryclean:
	cd portsf; make veryclean
	cd siggencpp; make veryclean
	cd vdelaycpp; make veryclean
#	cd tests; make clean

