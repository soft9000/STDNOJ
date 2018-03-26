// Class MailBot01: Manage an INI file in such a way as to allow it to
// drive the downloading of many POP accounts into many user-specified
// folders.
//
// 11/26/2006: Class created using Visual Studio 6.x (.NET 2005 is tragic in its C++ / POSIX / WIN32 support.)
// 12/24/2007: Creation resumed to assist the RSS Robot Project.
// 01/19/2008: Addded the -m, -o, and -p options to support the GUI interface easier, R. Nagy
//
#include <stdnoj.hpp>
#include <Extra/ini/IniFile.hpp>
#include <Extra/StdLog.hpp>

using namespace stdnoj;
using namespace ini;

#include <Server/InetRfc.hpp>
//using namespace BetterMail;

class MailBot01
{
protected:
   static void ListItem(const File& file, EMailHeader& emh, size_t iTally, ostream& cout);

public:

   static File      FindIni(const StdString& sFileBase, StdLog& log);

   static StdString ProgramName(void);
   static StdString ProgramVersion(void);

   static bool      StatUserDestination(const File& file, StdLog& log);
   static bool      StatUserDestination(IniFile& file, StdLog& log);
   static StdString GetPopServer(IniFile& ini);
   static StdString GetPopPort(IniFile& ini);
   static StdString GetPopUser(IniFile& ini);
   static StdString GetPopPassword(IniFile& ini);
   static StdString GetPopFolder(IniFile& ini);
   static StdString GetPopDebug(IniFile& ini);

   static bool      QueryIni(File& file);

   static StdString ProgramByline(void);
   static void      Usage(ostream& cout);

   static bool CreateDefault(StdLog& log);
   static bool CreateDefault(const File& fileINI, StdLog& log);

   static void ListConsole(IniFile& file, EMailHeader& emh, size_t iTally);
   static bool List(IniFile& fileINI, StdLog& log, ostream& cout);

   static bool Move(IniFile& file, StdLog& log, size_t ssWhich, ostream& cout);

   static bool Main(const File& fileINI, char chOption, int sasWhich, StdLog& log);
	static bool Main(Array<StdString>& aArgs, StdLog& log);
};
