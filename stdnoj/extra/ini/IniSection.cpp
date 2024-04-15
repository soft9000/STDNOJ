/* The MIT License (Open Source Approved)

Copyright (c) 1993 - 2024 Randall Nagy 

Permission is hereby granted, free of charge, to any person obtaining a copy 
of this software and associated documentation files (the "Software"), to 
deal in the Software without restriction, including without limitation the 
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
sell copies of the Software, and to permit persons to whom the Software is 
furnished to do so, subject to the following conditions: 

The above copyright notice and this permission notice shall be included in 
all copies or substantial portions of the Software. 

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
IN THE SOFTWARE. 

*/
// 08/29/2006: Updating, Randall Nagy

//#define DEBUGk

#include "IniFile.hpp"
#define MAX_INI_WIDTH  256                  // WINDOWS limitation.

using namespace ini;



// ***************************************************************************
// ***************************************************************************
// ***************************************************************************
// ***************************************************************************
IniSection::IniSection(void)
  {
  sSectionName = "[DEFAULT]";          // ownership is here.
  _rmbracket(sSectionName);            // remove brackets (if any)
  sEor = "Section=Eor!";               // Latter we need to parse on inter-section \n's <<WIP>>
  }
IniSection::IniSection(const StdString& rStg)
  {
  sSectionName = rStg;                 // ownership is here.
  _rmbracket(sSectionName);            // remove brackets (if any)
  sEor = "Section=Eor!";               // Latter we need to parse on inter-section \n's <<WIP>>
  }

// ***************************************************************************
// ***************************************************************************
IniSection::~IniSection(void)
  {
  }

// ***************************************************************************
// ***************************************************************************
bool IniSection::_chop(StdString &block, StdString& lval, StdString& rval)
  {
  if((block.find('=')== NPOS))
     return false;
  size_t pos = block.find("=");
  lval = block;
  lval.remove(pos);                      // remove from "=", onward.

  const StdString& sSS = block.c_str();
  size_t sz = sSS.length();
  rval.assign(&sSS[pos+1]);
#ifdef DEBUGk
  cout << block << " = " << lval << ", " << rval << endl;
#endif
  return true;
  }

// ***************************************************************************
// This function makes a "...[section]..." into a "section".  If no brackets,
// (or unbalanced baracketing) then it returns false.
// ***************************************************************************
bool IniSection::_rmbracket(StdString& sIn)
   {
   StdString block = sIn;
   size_t iStart = block.find('[');
   if(iStart == NPOS)
      return false;
   size_t iEnd   = block.find(']');
   if(iEnd   == NPOS)
      return false;
   block.remove(iEnd);
   sIn = &block[iStart+1];
   return true;
   }

// ***************************************************************************
// ***************************************************************************
ostream& IniSection::write(ostream& ofs)
  {
  // Stream-out the section-tag;
  ofs << "[" << sSectionName << "]" << endl;

  // Stream-out all of the section-values;
  StdString Tag, Value;
  int ss = EnumValues(Tag, Value);    // -1 == START
  while(ss != EnumStop)
     {
     ofs << Tag << "=" << Value << endl;
#ifdef DEBUGk
     cout << Tag << "=" << Value << endl;
#endif
     ss = EnumValues(Tag, Value, ss); // We'll return your ss to you...
     }
  ofs << endl;                                    // Our new "eor"
  return ofs;
  }

// ***************************************************************************
// Read data into an IniSection.
// ***************************************************************************
istream& IniSection::read(istream& ifs)
  {
  char buffer[MAX_INI_WIDTH];
  StdString rIn(" ");

  StdString Tag, Value;
  while(1)
     {
     ifs.getline(&buffer[0], MAX_INI_WIDTH);
     if(!buffer[0])
        break;                              // We just read a \n.  Our new eor.
     rIn.assign(&buffer[0]);
     _chop(rIn, Tag, Value);                // break 'em apart at the "=".
     AddData(Tag, Value);
     }
  return ifs;
  }


// ***************************************************************************
// ***************************************************************************
IniData *IniSection::FindData(const StdString& str)
{
StdString tag, value;
for(size_t i=NULL; i < aData.Nelem(); i++)
  {
  aData[i].Get(tag, value);
  if(tag == str)
     return &aData[i];
  }
return 0L;
}


// ***************************************************************************
// ***************************************************************************
bool IniSection::FindValue(StdString& value, const StdString& str)
{
StdString tag;
for(size_t i=NULL; i < aData.Nelem(); i++)
  {
  aData[i].Get(tag, value);
  if(tag == str)
     return true;
  }
value = "";
return false;
}


// ***************************************************************************
// ***************************************************************************
void IniSection::AddData(const StdString& ptag, const StdString& pvalue)
{
IniData next(ptag, pvalue);
aData[aData.Nelem()] = next;
}


// ***************************************************************************
// ***************************************************************************
bool IniSection::RmData(const StdString& str)
{
StdString tag, value;
for(size_t i=NULL; i < aData.Nelem(); i++)
  {
  aData[i].Get(tag, value);
  if(tag == str)
     {
     aData.Remove(i);
     return true; // G'night, Gracey.
     }
  }
return false;
}

// ***************************************************************************
// Protocol here is if "last1" == -1, then start at item 0, else, add 1 to
// "last1" to get which one is "next".
// ***************************************************************************
size_t IniSection::EnumValues(StdString& tag, StdString& value, size_t last1)
{
size_t foom = last1;
foom++;
if(foom >= aData.Nelem())
   return IniSection::EnumStop;

aData[foom].Get(tag, value);
return foom;
}
