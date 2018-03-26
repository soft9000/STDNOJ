// unix2dos source code
// ====================
// This program demonstrates using the NOJ ProgramId and File classes to create
// a newline conversion tool. Handy for use when copying files from Linux / Unix
// to DOS / WIN32. As a bonus, the unix2dos tool checks to ensure that a file 
// is "really" textual before performing the conversion. A minimalist newline detection 
// facility also prevents redundant file conversions (e.g. converting windows source
// code files twice.)   It also features a reasonable error recovery strategy.
//
// (This source code example could also be called linux2dos, or maybe even posix2bill!)
//
// Demonstration Version: 1.0
// ----------------------
// Work fine on my machine. Use at your own risk. Educational use only. 
//
// Any comments for improvement can be submitted to feedback@Soft9000.com.
//
#include <stdnoj.hpp> // standard header use (<iostream>, <iostream>, etc.) are always hidden.
using namespace std;

using namespace stdnoj;

void usage(void) {
   cout << "usage:" << endl;
   cout << "\tunix2dos [!] file_name" << endl;
   cout << "\t Where '!' is used to force the conversion of non-text files." << endl;
}

void main(int argc, char *argv[]) {

// STEP: Check parameter range
// =====
if((argc < 2) || (argc > 3))
   {
   usage();
   return;
   }

// STEP: Adjust for options
// =====
bool bForce = false;
StdString sFile = argv[1];
if(sFile == "!")
   {
   bForce = true;
   if(argc != 3)
      {
      usage();
      return;
      }
   sFile = argv[2];
   }

// STEP: Verify that file exists.
// =====
File file;
if(file.Name(sFile) == false)
   {
   cerr << "'" << file.Name() <<"' is not a valid file name on this system." << endl;
   return;
   }
if(file.Exists() == false)
   {
   cerr << "Unable to locate input file " << file.Name() <<"." << endl;
   return;
   }
File output;
ProgramId pid;
if(pid.GetTemp(output) == false)
   {
   cerr << "Error: Unable to create temp file." << endl;
   return;
   }

// STEP: Verify a textual nature for the file.
// =====
if((IsTextFile(file) == false) && (bForce == false))
   {
   cerr << "Error: Not a text file. Use the ! option to force file conversion." << endl;
   return;
   }

// STEP: Save the original file (just in case)
// =====
StdString sNewName = file.Name();
sNewName.append(".original");
File backup;
if(backup.Name(sNewName) == false)
   {
   cerr << "Error: Unable to create backup file mame." << endl;
   return;
   }
if(file.CopyTo(backup) == false)
   {
   cerr << "Error: Unable to create backup file (actual)." << endl;
   return;
   }

// STEP: Convert the file
// =====
bool lastWas = false;
ostream& os = output.OpenWrite(File::AT_BINARY);
istream& is = file.OpenRead(File::AT_BINARY);
char ch =0;
while(is)
   {
   is.read(&ch, 1);
   if(is)
      {
      // We could do a little more checking here, but we assume that the user
      // knows what they are doing.

      // CASE: Avoid the the double conversion error;
      if(ch == '\n' && lastWas == false)
         os << "\r\n";
      else
         os << ch;

      if(ch == '\r')
         lastWas = true;
      else
         lastWas = false;
      }
   }

file.Close();
output.Close();

// STEP: Copy the converted file over the original
// =====
file.Remove();
if(output.CopyTo(file) == false)
   {
   cerr << "Unable to convert file. Restoring original file." << endl;
   if(backup.Rename(file.Name()) == false)
      cerr << "Original file is in " << backup.Name() << endl;
   else
      cerr << "Orignal file restored." << endl;
   return;
   }

// STEP: Remove the backup file.
// =====
backup.Remove();
cout << "\t" << file.Name() << " converted successfully.";
}
