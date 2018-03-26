//---------------------------------------------------------------------------

#include <clx.h>
#pragma hdrstop
USEFORM("uMain.cpp", frmMain);
const char *pCmd;
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR psz, int)
{
   try
   {
      pCmd = psz;
       Application->Initialize();
       Application->CreateForm(__classid(TfrmMain), &frmMain);
       Application->Run();
   }
   catch (Exception &exception)
   {
       Application->ShowException(&exception);
   }
   catch(...)
   {
       try
       {
          throw Exception("");
       }
       catch(Exception &exception)
       {
          Application->ShowException(&exception);
       }
   }
   return 0;
}
//---------------------------------------------------------------------------
