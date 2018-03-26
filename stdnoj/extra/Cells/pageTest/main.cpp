#include "../BasicPageManager.hpp"

int main(int argc, char *argv[])
{
BasicLineManager blm;
if(blm.test(cout) == false)
   {
   cerr << "BasicLineManager: Errors encountered" << endl;
   return -1;
   }
BasicPageManager bpm;
if(bpm.test(cout) == false)
   {
   cerr << "BasicPageManager: Errors encountered." << endl;
   return -1;
   }
return 1;   
}