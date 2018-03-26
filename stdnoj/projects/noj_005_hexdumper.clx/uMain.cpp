//---------------------------------------------------------------------------

#include <clx.h>
#pragma hdrstop

#include "uMain.h"

#include "uDisplayDecoder.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.xfm"
TfrmMain *frmMain;

extern const char *pCmd;

//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
   : TForm(Owner), bAsText(true)
{
sParam = pCmd;
sParam.replace('\"', ' ');
sParam.strip();
StdString str = "Welcome to the Neat Odd Job File Viewer.\n Please select a file to view.\n";
pwBuff.assign(str.c_str(), str.length());
memFile->Text = str.c_str();
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::pntbxViewPaint(TObject *Sender)
{
Array<StdString> array;
DisplayDecoder dd;
if(bAsText == false)
   dd.DecodeHex(pwBuff, array);
else
   dd.DecodeText(pwBuff, array);
int xx = 0;
int yy = 0;
int high = pntbxView->Canvas->TextHeight("X");
for(size_t ss = 0L; ss < array.Nelem(); ss++)
   {
   pntbxView->Canvas->TextOut(5, ss * high, array[ss].c_str());
   }
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::actOpenExecute(TObject *Sender)
{
if(OpenDialog1->Execute())
   {
   File file;
   if(file.Name(OpenDialog1->FileName.c_str()) == false)
      {
      DOMSG("Bad file name.");
      return;
      }
   Open(file, Sender);
   }
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::Open(File& file, TObject *Sender)
{
   if(pager.Open(file) == false)
      {
      DOMSG("Unable to open file.");
      return;
      }
   actAsListExecute(Sender);
   Caption = file.Name();
   actNextExecute(Sender);
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::Clear(void)
{
   // Clear the screen
   TRect tr;
   tr.Right = pntbxView->Height;
   tr.Left  = pntbxView->Width;
   pntbxView->Canvas->Rectangle(tr);
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::actNextExecute(TObject *Sender)
{
pager.GetNext(pwBuff);
Clear();
pntbxView->Repaint();
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::actPreviousExecute(TObject *Sender)
{
pager.GetPrev(pwBuff);
Clear();
pntbxView->Repaint();
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::actAsHexExecute(TObject *Sender)
{
SetBlockMode(true);
bAsText = false;
pntbxView->Repaint();
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::SetBlockMode(bool bMode)
{
actNext->Enabled = bMode;
actPrevious->Enabled = bMode;
actGoto->Enabled = bMode;
if(bMode == false)
   {
   // Keep the splitter in its place...
   Splitter1->Visible = bMode;
   pnlViewAll->Visible = bMode;
   }
else
   {
   pnlViewAll->Visible = bMode;
   Splitter1->Visible = bMode;
   }
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::actQuitExecute(TObject *Sender)
{
Close();
}

//---------------------------------------------------------------------------
void TfrmMain::DOMSG(const char *psz)
   {
   AnsiString ans;
   ans = psz;
   ShowMessage(ans);
   StatusBar1->SimpleText = ans;
   }

//---------------------------------------------------------------------------
void __fastcall TfrmMain::actAsTextExecute(TObject *Sender)
{
bAsText = true;
pntbxView->Repaint();
SetBlockMode(true);
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormCreate(TObject *Sender)
{
File file;
if(file.Name(sParam) == true)
   {
   if(file.Exists())
      Open(file, Sender);
   }
DOMSG(sParam.c_str());   
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::actAsListExecute(TObject *Sender)
{
Array<StdString> array;
if(pager.ReadAll(array) == false)
   {
   DOMSG("Unable to view file.");
   return;
   }
memFile->Clear();
AnsiString ans;
for(size_t ss = 0L; ss < array.Nelem(); ss++)
   {
   ans = array[ss].c_str();
   memFile->Lines->Append(ans);
   }
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::actToggleSplitExecute(TObject *Sender)
{
static bool bSplit = false;
bSplit = !bSplit;
SetBlockMode(bSplit);
}

//---------------------------------------------------------------------------

