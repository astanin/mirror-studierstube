# a simple script to flip artoolkit pattern files vertically.
# it accepts a list of files and will convert each one in place
# overwriting the old version.
# Usage : python mirrorpatterns.py file1 file2 ...
# @author Gerhard Reitmayr
# $Id: mirrorpatterns.py 712 2004-07-14 12:44:50Z tamer $

import sys

def flipFile( input ) :
    fin = file( input );
    lines = fin.readlines()
    size = lines[0].replace('  ',' ').replace('  ',' ').strip().count(' ')+1
    fin.close();
    
    fout = file( input, 'w' );

    for images in [0,3,2,1] :
	for components in range(3) :
		for count in range(size) :
			fout.write( lines[images*(size*3+1)+components*size+(size-count-1)] )
	fout.write('\n');

    fout.close()

def main( args ) :
    for arg in args[1:] :
        print 'doing file ',arg
        flipFile( arg )

if __name__ == "__main__":
    main(sys.argv)
