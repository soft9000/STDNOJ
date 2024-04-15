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

#include "IniFile.hpp"

using namespace ini;


IniFile::IniFile(void) : bChanged(false), ss(0)
{
}


IniFile::IniFile(const File& file) : bChanged(false), ss(0)
{
   Use(file);
}

IniFile::~IniFile(void)
{
}


void IniFile::Use(const File& file)
{
   bChanged = false;
   pwFile = file;
}



// ***************************************************************************
// THIS FUNCTION SUPERCEEDS Find();
// ***************************************************************************
StdString IniFile::Get(const StdString& section, const StdString& tag)
{
StdString sResult;
IniSection *pwSection = FindSection(section);
if(pwSection)
   pwSection->FindValue(sResult, tag);
return sResult;
}

// ***************************************************************************
// THIS FUNCTION SUPERCEEDS Find();
// ***************************************************************************
bool IniFile::Get(const StdString& section, const StdString& tag, StdString& sValue)
{
sValue = Get(section, tag);
return !(sValue.is_null());
}

// ***************************************************************************
// THIS FUNCTION IS OUR LEGACY;
// ***************************************************************************
const char *IniFile::Find(StdString& sResult, const StdString& section, const StdString& tag)
{
sResult = Get(section, tag);
if(sResult.is_null())
   return 0L;

return sResult.c_str();
}


// ***************************************************************************
// ***************************************************************************
bool IniFile::Update(const StdString& section, const StdString& tag, const StdString& value)
{
IniSection *pwSection = FindSection(section);
if(!pwSection)
  return false;

IniData *pDat = pwSection->FindData(tag);
if(!pDat)
  return false;

pDat->Put(tag, value);
bChanged = true;
return true;
}


// ***************************************************************************
// ***************************************************************************
bool IniFile::Insert(const StdString& section, const StdString& tag, const StdString& value)
{
IniSection *pwSection = FindSection(section);
if(pwSection)
   {
   // STEP: If a section exists, then update it -
   StdString str = Get(section, tag);
   if(!str.is_null())
      return Update(section, tag, value);
   }
if(!pwSection)
   {
   // STEP: If a section does not exist, then create a new section
   IniSection nSection(section);
   if(AddSection(nSection) == false)
     return false;
   // Verify creation
   pwSection = FindSection(section);
   if(!pwSection)
      return false;
  }
// STEP: Add the data -
pwSection->AddData(tag, value);
bChanged = true;
return true;
}

// ***************************************************************************
// ***************************************************************************
bool IniFile::Delete(const StdString& section, const StdString& tag)
{
IniSection *pwSection = FindSection(section);
if(!pwSection)
  return false;

bool br = pwSection->RmData(tag);
if(br == true)
  bChanged = true;

return br;
}


// ***************************************************************************
// ***************************************************************************
IniSection *IniFile::FindSection(const StdString& str)
{
StdString tag, value;
for(size_t i=NULL; i < aSections.Nelem(); i++)
  {
  if(str == aSections[i].sSectionName.c_str())
     return &aSections[i];
  }
return 0L;
}


// ***************************************************************************
// ***************************************************************************
bool IniFile::AddSection(IniSection& rNew1)
{
   aSections[aSections.Nelem()] = rNew1;
   bChanged = true;
   return true;
}

// ***************************************************************************
// ***************************************************************************
StdString *IniFile::GetSection(istream& ifs)
{
char buf[MAX_PATH];                   // largest SECTION .. .

while(ifs.getline(&buf[0], MAX_PATH-1))        // This is where the EOF will always be.
  {
  if(buf[0] == '[')                   // we have a winner [section]
     return new StdString(buf);
  }

return (StdString *) 0L;                 // "no mo "
}

// ***************************************************************************
// ***************************************************************************
void IniFile::New(void)
{
*this = IniFile(pwFile);
}


// ***************************************************************************
// Loads everything for faster access - From a Directory
// ***************************************************************************
bool IniFile::Load(File& pwFile, const Directory& dir)
{
if(dir.Home(pwFile) == false)
  return false;
return Load(pwFile);
}

// ***************************************************************************
// Loads everything for faster access - From a File
// ***************************************************************************
bool IniFile::Load(const File& file)
{
Use(file);
return Load();
}

// ***************************************************************************
// Re-loads everything for faster access - From default internal file
// ***************************************************************************
bool IniFile::Load(void)
{
aSections.Free();

istream& infile = pwFile.OpenRead();

StdString *pSection = GetSection(infile);          // Look for a "[section]"
while(pSection)                                       // -if one found then
  {
  pwIniSection = CreateIniSection(*pSection);         // create a section object (ownership to pSection passes, too).
  pwIniSection->read(infile);                         // stream the data in.
  if(AddSection(*pwIniSection) == false)
     {
     pwFile.Close();
     return false;
     }
  pSection = GetSection(infile);                   // check for another section.
  }

pwFile.Close();

bChanged = false;
if(aSections.Nelem())
   return true;
return false;
}


// ***************************************************************************
// ***************************************************************************
bool IniFile::Save(void)
{
ostream& ofile = pwFile.OpenWrite();

for(size_t i = NULL; i < aSections.Nelem(); i++)
   aSections[i].write(ofile);                          // stream it out.

pwFile.Close();

bChanged = false;
return true;
}


// ***************************************************************************
// ***************************************************************************
bool IniFile::HasChanged(void)
{
return bChanged;
}


// ***************************************************************************
// ***************************************************************************
bool IniFile::Exists(void)
{
return pwFile.Exists();
}


// ***************************************************************************
// Check for one of a set of options in an INI file. Return the zero based offset if found, -1 on error.
// ***************************************************************************
int IniFile::GetEnum(const StdString& sSection, const StdString& sTag, const StdString& sOptions)
   {
   StdString sData = Get(sSection, sTag);
   if(sData.is_null())
      return -1;
   Array<StdString> aOptions = sOptions.slice('|');
   for(size_t ss = 0L; ss < aOptions.Nelem(); ss++)
      aOptions[ss].strip();
   return aOptions.Contains(sData);
   }

// ***************************************************************************
// Writes one of a set of options in an INI file based upon the offset if found, false on error.
// ***************************************************************************
bool IniFile::PutEnum(const StdString& sSection, const StdString& sTag, const StdString& sOptions, int iWhich)
   {
   Array<StdString> aOptions = sOptions.slice('|');
   for(size_t ss = 0L; ss < aOptions.Nelem(); ss++)
      aOptions[ss].strip();
   if(size_t(iWhich) >= aOptions.Nelem())
      return false;
   if(Get(sSection, sTag).is_null())
      return Insert(sSection, sTag, aOptions[iWhich]);
   return Update(sSection, sTag, aOptions[iWhich]);
   }
// ***************************************************************************
// Writes the entire set of options to the file, allowing a user to edit the file and choose one.
// ***************************************************************************
bool IniFile::PutEnumOptions(const StdString& sSection, const StdString& sTag, const StdString& sOptions)
   {
   if(Get(sSection, sTag).is_null())
      return Insert(sSection, sTag, sOptions);
   return Update(sSection, sTag, sOptions);
   }
// ***************************************************************************
// Add it if new, update it if not -
// ***************************************************************************
bool IniFile::Put(const StdString& section, const StdString& tag, const StdString& value)
   {
   return Insert(section, tag, value);
   }

