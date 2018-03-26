#include <extra/DosMatch.hpp>


void Test::test(const DosMatch& dm, ostream& cout)
   {
   DosMatch match;

   // STEP
   // ****
   // Simple, unary patterns;
   if(match.Match("FOO.EXE", "FOO.EXE") == false)
      {
      cout << "Error 01" << endl;
      return;
      }

   if(match.Match("FOO.*", "FOO.EXE") == false)
      {
      cout << "Error 02" << endl;
      return;
      }

   if(match.Match("*.EXE", "FOO.EXE") == false)
      {
      cout << "Error 03" << endl;
      return;
      }

   if(match.Match("FO?.EXE", "FOO.EXE") == false)
      {
      cout << "Error 04" << endl;
      return;
      }

   // STEP
   // ****
   // Binary patterns;
   if(match.Match("?O?.EXE", "FOO.EXE") == false)
      {
      cout << "Error 05" << endl;
      return;
      }

   if(match.Match("?O*.EXE", "FOO.EXE") == false)
      {
      cout << "Error 06" << endl;
      return;
      }

   if(match.Match("*.?XE", "FOO.EXE") == false)
      {
      cout << "Error 06a" << endl;
      return;
      }

   // STEP
   // ****
   // false unary patterns;
   if(match.Match("FOO.EXE", "FOO.EXX") == true)
      {
      cout << "Error 07" << endl;
      return;
      }

   if(match.Match("FOO.*", "FOO:EXE") == true)
      {
      cout << "Error 08" << endl;
      return;
      }

   if(match.Match("*.EXE", "FOO.EXX") == true)
      {
      cout << "Error 09" << endl;
      return;
      }

   if(match.Match("FO?.EXE", "FO.EXE") == true)
      {
      cout << "Error 10" << endl;
      return;
      }

   if(match.Match("*.EXE", "RELEASE1") == true)
      {
      cout << "Error 10a" << endl;
      return;
      }

   if(match.Match("*", "RELEASE1") == false)
      {
      cout << "Error 10b" << endl;
      return;
      }

   // STEP
   // ****
   // false binary patterns;
   if(match.Match("?O?.EXE", "FFO.EXE") == true)
      {
      cout << "Error 30" << endl;
      return;
      }

   if(match.Match("?O*.EXE", "FFO.EXE") == true)
      {
      cout << "Error 31" << endl;
      return;
      }

   // STEP
   // ****
   // Crash tests;
   if(match.Match("", "") == false)
      {
      cout << "Error 20" << endl;
      return;
      }

   if(match.Match("?", "") == true)
      {
      cout << "Error 21a" << endl;
      return;
      }

   if(match.Match("?", "1") == false)
      {
      cout << "Error 21b" << endl;
      return;
      }

   if(match.Match("?", "111111") == true)
      {
      cout << "Error 21c" << endl;
      return;
      }

   if(match.Match("*", "1") == false)
      {
      cout << "Error 22a" << endl;
      return;
      }

   if(match.Match("*", "111111") == false)
      {
      cout << "Error 22b" << endl;
      return;
      }

   if(match.Match("*", "") == true)    // Hummmmmmm...
      {
      cout << "Error 22c" << endl;
      return;
      }

   if(match.Match("", "?") == true)
      {
      cout << "Error 23" << endl;
      return;
      }

   if(match.Match("", "*") == true)
      {
      cout << "Error 24" << endl;
      return;
      }

   if(match.Match("", "11111") == true)
      {
      cout << "Error 25" << endl;
      return;
      }

   if(match.Match("11111", "1") == true)
      {
      cout << "Error 26" << endl;
      return;
      }

   if(match.Match("1", "11") == true)
      {
      cout << "Error 27" << endl;
      return;
      }


   cout << "Normal end of test." << endl;

   }
