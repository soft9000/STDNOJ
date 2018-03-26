//#include "sample.hpp"
//#include "treecomp.hpp"

#include "BlastIt.hpp"
#include <FolderFinder.hpp>
//#include <File.hpp>


class Blaster : public FolderReport
{
public:
   Array<StreamString> ary;
   bool bQuery;
   ostream& cout;
   ostream& cerr;
public:
   Blaster(const BlastIt& bi, ostream& out,  ostream& err) : cout(out), cerr(err)
      {
      bQuery = bi.bQuery;
      ary = bi.bb.aPatterns;
      }
   virtual void Dispose(const StdString& pwNode)
      {
      if(bQuery)
         cout << "DEL " << pwNode << endl;
      else
         {
         ::File file;
         if(file.Name(pwNode) == false)
            {
            cerr << "ERROR: Unable to locate file " << pwNode << endl;
            return;
            }
         if(file.Remove() == false)
            {
            cerr << "ERROR: Unable to remove file " << pwNode << endl;
            return;
            }
         cout << "Removed " << pwNode << endl;
         }
      }
   virtual bool Folder(const StdString& pwDir, const WIN32_FIND_DATA& ffd)
      {
      return true;
      }
   virtual bool File(const StdString& pwDir, const WIN32_FIND_DATA& ffd)
      {
      StdString sName;
      sName = ffd.cFileName;
      sName.to_upper();
      size_t whence = NULL;
      for(size_t ss = NULL; ss < ary.Nelem(); ss++)
         {
         StdString sPat = ary[ss].c_str();
         sPat.to_upper();
         whence = sName.rfind(sPat);
         if(whence != NPOS)
            {
            StdString match = sName.substr(whence, NPOS);
            if(match == sPat)
               {
               // We have a winner!
               sPat = pwDir;
               sPat.append('\\');
               sPat.append(ffd.cFileName);
               Dispose(sPat);
               return true;
               }
            }
         }
      return true;
      }
};


BlastIt::BlastIt(void) : iChanged(0), bQuery(true)
   {
   }
void BlastIt::Load(void)
   {
   iChanged = 0;
   bQuery   =true;
   ProgramId pid;
   StdString str;
   pid.ProgramName(str);
   str.append(".dat");
   pid.PlaceInDirectory(str);
   file.Name(str);
   bb.read(file.OpenRead(File::AT_BINARY));
   file.Close();
   }
BlastIt::~BlastIt(void)
   {
   if(iChanged)
      {
      bb.write(file.OpenWrite(File::AT_BINARY));
      file.Close();
      }
   }
StdString BlastIt::RemovePattern(const StdString& sPattern)
   {
   iChanged = 1;
   return bb.RemovePattern(sPattern);
   }
StdString BlastIt::AddPattern(const StdString& sPattern)
   {
   iChanged = 1;
   return bb.AddPattern(sPattern);
   }
StdString BlastIt::Program(const StdString& sParam)
   {
   StdString sResult = "Okay";
   switch(sParam[0])
      {
      case ('-'):
         return RemovePattern(&sParam[1]);
      case ('+'):
         return AddPattern(&sParam[1]);
      default:
         break;
      }
   return sResult;
   }
StdString BlastIt::Run(int argc, char *argv[], ostream& cout, ostream& cerr)
   {
   bQuery = false;
   StdString sRes = Query(argc, argv, cout, cerr);
   bQuery = true;
   return sRes;
   }
bool BlastIt::Query(const StdString& sStartDir, BlastItParams& params, ostream& sOut, ostream& sError)
   {
   bb = params;
   QueryParamDir(sStartDir, sOut, sError);
   return true;
   }
StdString BlastIt::Query(int argc, char *argv[], ostream& cout, ostream& cerr)
   {
   if(argc == 1)
      return RunParamNone(cout, cerr);
   if(argc == 2)
      {
      if(argv[1][0] == '+' || argv[1][0] == '-')
         return Program(argv[1]);
      return RunParamDir(argv[1], cout, cerr);
      }
   Usage(cout);
   StdString sRes = "Okay";
   return sRes;
   }
void BlastIt::Version(ostream& cout)
   {
   cout << endl;
   cout << "BlastIt: Version 1.0" << endl;
   cout << "-------- Copyright (c) 2001 Soft9000.com." << endl;
   cout << "         Freely distributable, All Rights Reserved." << endl;
   cout << "         Works fine on my machine, but USE AT YOUR OWN RISK." << endl;
   cout << endl;
   }
void BlastIt::Usage(ostream& cout)
   {
   cout << endl;
   cout << "Usage: BlastIt [ [+suffix] [-suffix] [\"path to run on\"] ]";
   cout << endl;
   cout << "Use + to add, or - to remove items from the list." << endl;
   cout << "(i.e: +.OBJ or -.OBJ (etc) (case sensitive))"      << endl;
   cout << "Use \"path to run on\" to *recursively* destroy ALL files" << endl;
   cout << "ending with `suffix` under path and *ALL* sub-directories." << endl;
   cout << endl;
   }
StdString BlastIt::RunParamNone(ostream& out, ostream& err)
   {
   out << "BlastIt: Programmed to recursively remove files ending with;" << endl;
   size_t ss = NULL;
   for(ss = NULL; ss < bb.Nelem(); ss++)
      out << bb[ss].c_str() << endl;
   Usage(out);
   StdString sResult = "Okay";
   if(!ss)
      sResult = "No files";
   return sResult;
   }
long unsigned BlastIt::QueryParamDir(const StdString& sStartDir, ostream& out, ostream& err)
   {
   bQuery = true;
   Blaster blaster(*this, out, err);
   FolderFinder ff;
   unsigned long res = ff.Run(sStartDir, blaster);
   if(res == -1L)
      res = NULL;
   bQuery = false;
   return res;
   }
long unsigned BlastIt::RunParamDir(const StdString& sStartDir, ostream& out, ostream& err)
   {
   Blaster blaster(*this, out, err);
   FolderFinder ff;
   unsigned long res = ff.Run(sStartDir, blaster);
   if(res == -1L)
      res = NULL;
   return res;
   }

StdString BlastItParams::RemovePattern(const StdString& sPattern)
   {
   StdString sResult = "Okay";
   StreamString sss = sPattern;
   for(size_t ss = NULL; ss < aPatterns.Nelem(); ss++)
      {
      if(sss == aPatterns[ss])
         {
         aPatterns.Remove(ss);
         return sResult;
         }
      }
   sResult = "Pattern `";
   sResult.append(sPattern);
   sResult = "` not found.";
   return sResult;
   }
StdString BlastItParams::AddPattern(const StdString& sPattern)
   {
   StdString sResult = "Okay";
   StreamString sss = sPattern;
   aPatterns.Append(sss);
   return sResult;
   }
ostream& BlastItParams::write(ostream& os)
   {
   aPatterns.write(os);
   return os;
   }
istream& BlastItParams::read(istream& is)
   {
   aPatterns.read(is);
   return is;
   }

const char *SetComma(long unsigned raw, StdString& res)
   {
   char buf[MAX_PATH + sizeof(NULL)];
   sprintf(&buf[0], "%lu", raw);
   res = &buf[0];
   res.reverse();
   size_t len = res.length();
   size_t sz = (len / 3) +1;
   for(size_t ss = 1; ss < sz; ss++)
      {
      size_t pos = (ss * 3) + ss;
      if(pos != (len + ss))
         res.insert(pos -1, ",");
      }
   res.reverse();
   return res.c_str();
   }

/* YYYY/MM/DD format assumed;
bool ParseDate(StdString& str_in, CTime& cTime)
   {
   const char delim = '/';
   StdString sDate = str_in;

   StdString yy, mm, dd;
   long sp2;

   sp2 = sDate.find(delim);
   if(sp2 == npos)
      return false;
   sDate[sp2] = NULL;
   yy = sDate.c_str();
   sDate.remove_pos(0, sp2+1);

   sp2 = sDate.find(delim);
   if(sp2 == npos)
      return false;
   sDate[sp2] = NULL;
   mm = sDate.c_str();
   sDate.remove_pos(0, sp2+1);
   dd = sDate.c_str();

   // CTime needs a 1900 base. Make sure it's compliant;
   if(atoi(yy.c_str()) < 1900)
      return false;

   // Record and save the result;
   CTime   cTime2(atoi(yy.c_str()), atoi(mm.c_str()), atoi(dd.c_str()), 0, 0, 0);
   cTime = cTime2;

   return true;
   }
*/

