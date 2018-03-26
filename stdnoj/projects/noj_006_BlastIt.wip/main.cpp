#include "BlastIt.hpp"

void main(int argc, char *argv[])
   {
   BlastIt program;
   program.Version(cout);
   program.Load();
   cout << program.Run(argc, argv, cout, cerr);
   cout << endl;
   }
