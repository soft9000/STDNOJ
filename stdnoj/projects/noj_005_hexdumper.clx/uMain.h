//---------------------------------------------------------------------------

#ifndef uMainH
#define uMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <QControls.hpp>
#include <QStdCtrls.hpp>
#include <QForms.hpp>
#include <QActnList.hpp>
#include <QButtons.hpp>
#include <QComCtrls.hpp>
#include <QExtCtrls.hpp>
#include <QImgList.hpp>
#include <QDialogs.hpp>

#include "uFilePager.h"
#include <QMenus.hpp>
#include <QTypes.hpp>

//---------------------------------------------------------------------------
class TfrmMain : public TForm
{
__published:	// IDE-managed Components
   TPanel *Panel1;
   TActionList *ActionList1;
   TToolBar *ToolBar1;
   TStatusBar *StatusBar1;
   TAction *actOpen;
   TAction *actAsHex;
   TAction *actAsText;
   TAction *actQuit;
   TImageList *ImageList1;
   TAction *actNext;
   TAction *actPrevious;
   TAction *actGoto;
   TOpenDialog *OpenDialog1;
   TToolButton *ToolButton1;
   TToolButton *ToolButton2;
   TToolButton *ToolButton3;
   TToolButton *ToolButton4;
   TToolButton *ToolButton5;
   TToolButton *ToolButton6;
   TToolButton *ToolButton7;
   TToolButton *ToolButton8;
   TToolButton *ToolButton9;
   TToolButton *ToolButton10;
   TToolButton *ToolButton11;
   TToolButton *ToolButton12;
   TToolButton *ToolButton13;
   TToolButton *ToolButton14;
   TPopupMenu *PopupMenu1;
   TMenuItem *actOpen1;
   TMenuItem *actGoto1;
   TMenuItem *actPrevious1;
   TMenuItem *actNext1;
   TMenuItem *actQuit1;
   TMainMenu *MainMenu1;
   TMenuItem *File1;
   TMenuItem *Open1;
   TMenuItem *Quit1;
   TMenuItem *Page1;
   TMenuItem *GotoBlock1;
   TMenuItem *NextBlock1;
   TMenuItem *PreviousBlock1;
   TMenuItem *View1;
   TAction *actAsList;
   TToolButton *ToolButton15;
   TToolButton *ToolButton16;
   TMenuItem *Loadtext1;
   TMenuItem *BlockMode1;
   TMenuItem *AsHexidecimal1;
   TMenuItem *AsText1;
   TPanel *pnlViewAll;
   TPaintBox *pntbxView;
   TSplitter *Splitter1;
   TPanel *Panel2;
   TMemo *memFile;
   TAction *actToggleSplit;
   TMenuItem *Inspector1;
   TMenuItem *Blocks1;
   void __fastcall pntbxViewPaint(TObject *Sender);
   void __fastcall actOpenExecute(TObject *Sender);
   void __fastcall actNextExecute(TObject *Sender);
   void __fastcall actPreviousExecute(TObject *Sender);
   void __fastcall actAsHexExecute(TObject *Sender);
   void __fastcall actQuitExecute(TObject *Sender);
   void __fastcall actAsTextExecute(TObject *Sender);
   void __fastcall FormCreate(TObject *Sender);
   void __fastcall actAsListExecute(TObject *Sender);
   void __fastcall actToggleSplitExecute(TObject *Sender);
private:	// User declarations
   StdString sParam;    // command line parameter
   FilePager pager;
   ByteBuffer pwBuff;
   bool bAsText;
   void __fastcall Clear(void);
   void __fastcall SetBlockMode(bool bMode);

public:		// User declarations
   __fastcall TfrmMain(TComponent* Owner);
   void DOMSG(const char *psz);
   void __fastcall Open(File& file, TObject *Sender);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
