#include "MailBot01.hpp"

void main(int argc, char *argv[])
{
   Array<StdString> aArgs;
   for(int ss = 0; ss < argc; ss++)
      aArgs.Append(StdString(argv[ss]));

   StdLog log;
   bool bRes = MailBot01::Main(aArgs, log);
   if(bRes == false)
      {
      cout << "Error. See " << log.Name() << " for more information." << endl; 
      }
}

