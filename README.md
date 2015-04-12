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

Now torajiro can be built:
   - cd path/to/torajiro/src && make
   
