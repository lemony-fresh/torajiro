torajiro
========

A Go pattern matcher that
   - learns move patterns from large Go game databases, and
   - makes suggestions for given board positions.

COMPILATION
===========
Prerequisites:
   - boost
      - it's best to use your package manager to install boost, e.g. sudo apt-get install libboost-dev
   - fuego:
      - cd parent/dir/of/torajiro
      - svn checkout svn://svn.code.sf.net/p/fuego/code/trunk fuego
      - cd fuego && ./setup-build.sh && ./configure && make
      - if ./configure fails to detect the boost libs you may have to run ./configure --with-boost-libdir=/usr/lib/x86_64-linux-gnu/
      - if make's final linking step fails due to an issue with libpthread, this may be ok and you may be able to continue with building torajiro

Now torajiro can be built:
   - cd path/to/torajiro/src && make
   
