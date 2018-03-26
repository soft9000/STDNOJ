/* The MIT License (Open Source Approved)

Copyright (c) 1993 - 2009 R. A. Nagy 

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
// This ancient class is used to parse INI files. It has been thru several incarnations.
// Originally written in C, converted to C++, and finaly ported to stdnoj, it could use
// some re-deisgn.
//
// 04/19/1998: TABOUT && converting to WIN32 (weekend #2), R. Nagy
// 05/18/1998: Fixed IniSection::FindValue() to take a buffer for the result (no reference caching on StdString), R. Nagy
// 09/07/1998: Converted to string, R. Nagy.
// 10/19/1998: Fixed a bug in _chop() as a result of conversion of cstring.contains() to string.find(), R. Nagy
// 10/26/1999: Massively updated to work with "const StdString&" over "const char *" (etc), R. Nagy.
// 03/10/2001: Re-wrote IniSection::_rmbracket() so that it would work properly? R. Nagy
// 03/13/2005: Converting (in-place - no port!) to stdnoj, R. Nagy
// 08/29/2006: Updating, R. Nagy
// 08/23/2006: Copied to the stdnoj project as IniFile (formerly known as inistrm), R. Nagy
// 10/16/2006: Updated Insert(...) to reflect the more stand-alone usage paradigm, R. Nagy
//             Depricated Update(...) and Insert(...) in favor of Write(...), R. Nagy
//             Changed Read(void) to Load(void). Changed Write(void) to Save(void), R. Nagy
//             Dropped virtual memberships. As a pattern for re-use, TVRs are far better, R.Nagy
// 10/25/2006: Added New(), R. Nagy
//
// TODO
// ====
// This entire namespace is showing its age - It needs to:
//    (1) Be converted to a more publically accessable MVC pattern for IniData and IniSection.
//    (2) Allow for a more public IniFile management of IniSections AS A WHOLE (CRUD&S.)
//
// IMPORTANT
// =========
// For efficiency sake, I do not think that we should eliminate the use of
// pointers in this version of the namespace. Just be sure to check that
// pointers are populated before use. i.e. "If it is not broke, do not fix it."
//
// Also, we need to add support for comments - both before and within sections. 
// As it is now, they are COMPLETELY ignored on Load(), so they are lost on 
// Save() - Not too bad for our purposes, as the main purpose of the INI is
// to provide an easy way for a USER to edit data - But we should still support
// the verbaium Load() and Save() of INI comments and other non-data lines at 
// some point.
//
#ifndef _INISTRM_HPP
#define _INISTRM_HPP

#include <stdnoj.hpp>
using namespace stdnoj;

namespace ini
   {
class IniFile;
class IniSection;
class IniData;


class IniData
  {
  protected:
     StdString pLval;
     StdString pRval;

  public:
     IniData(void);
     IniData(const StdString& L, const StdString& R);
     ~IniData(void);

     void Get(StdString& L, StdString& R);
     void Put(const StdString& L, const StdString& R);
  };

// ***************************************************************************
// An IniSection is a [section] in an IniFile.  Note that ANY persistance layer
// can be supported with a simple rewrite of the stream operators, since this is
// the only place where []'s and "x=X" expressions are accomodated;
// ***************************************************************************
class IniSection
  {
  private:
     bool     _rmbracket(StdString&);
     bool     _chop(StdString&, StdString&, StdString&);

     // Set / test end of record (section) members;
     ostream& EorPut(ostream&);
     int      EorIs(StdString&);

     enum stuff {EnumStart = -1, EnumStop = -1};
     Array<IniData>  aData;            // Where SECTION OPTIONS are stored.

     StdString sEor;                   // Our end-of-record (section) indicator.

     IniData *FindData(const StdString& DataTag);
     bool     FindValue(StdString& sValueResult, const StdString& sDataTag);

     void     AddData(const StdString& sDataTag, const StdString& sValueResult);
     bool     RmData(const StdString& sDataTag);

  public:
     StdString sSectionName;           // Where the NAME of THIS SECTION is stored.
     
     IniSection(void);
     IniSection(const StdString& sectionTag);
     ~IniSection(void);

     size_t  EnumValues(StdString& tag, StdString& value, size_t last1=EnumStart);
     
     StdString Get(const StdString& sTag) {StdString str; FindValue(str, sTag); return str;}
     bool      Get(const StdString& sTag, StdString& str) {return FindValue(str, sTag);}
     bool      Put(const StdString& sTag, StdString& str) {AddData(sTag, str); return true;}
     bool      Delete(const StdString& sTag)              {return RmData(sTag);}

     ostream& write(ostream&);
     istream& read(istream&);

    friend IniFile;
  };


// ***************************************************************************
// An IniFile manages one or many IniSections;
// ***************************************************************************
class IniFile
  {
  private:
     IniSection        *pwIniSection;
     Array<IniSection> aSections;

     StdString *GetSection(istream&);

     int ss;

     // Find() is obsolete: Use Lookup(), instead;
     const char *Find(StdString& sResult, const StdString& section, const StdString& tag);   // ((LEGACY))

  protected:
     bool bChanged;

     IniSection *CreateIniSection(const StdString& rs) {return new IniSection(rs);}

     // Locate a section;
     IniSection *FindSection(const StdString& section);

  private:
     // Dynamically add a new section to the INI file.
     bool        AddSection(IniSection&);
     // (else, use the section as the lookup-key,
     // and use it to traverse the paIniSections[]...)
  protected:
     // Depricated -
     bool      Update(const StdString& section, const StdString& tag, const StdString& value);  // False if section NF
     bool      Insert(const StdString& section, const StdString& tag, const StdString& value);  // Section created if not found

  public:
     File pwFile;

     IniFile(void);
     IniFile(const File& file);

     ~IniFile(void);

     void      Use(const File& file);

     StdString Get(const StdString& section, const StdString& tag);                          // Return tag value from section.
     bool      Get(const StdString& section, const StdString& tag, StdString& sValue);       // Return bool if non-null.
     bool      Put(const StdString& section, const StdString& tag, const StdString& value);  // Update if it exists, Insert if new -
     bool      Delete(const StdString& section, const StdString& tag);                       // Remove tag from the section

     bool      HasChanged(void); // true if *file has changed.

     void      New(void);              // Reset the MEMORY content - file content (if any) left unchanged

     bool      Load(File& pwFile, const Directory& dir); // True if file was read.
     bool      Load(const File& file); // True if file was read.
     bool      Load(void);             // true if INI FILE was read.
     bool      Save(void);             // true if "    "   was written.
     bool      Exists(void);           // true if "    "   file exists.

     // Check for one of a set of options in an INI file. Return the zero based offset if found, -1 on error.
     // Example: switch(ReadEnum("DATABASE", "CONTENT_TYPE", "XML | TVR | SDF | CSV | FORTUNE")) {...}
     int       GetEnum(const StdString& sSection, const StdString& sTag, const StdString& sOptions);
     // Writes one of a set of options in an INI file based upon the offset if found, false on error.
     // Example: WriteEnum("DATABASE", "CONTENT_TYPE", "XML | TVR | SDF | CSV | FORTUNE"), 4);
     bool      PutEnum(const StdString& sSection, const StdString& sTag, const StdString& sOptions, int iWhich);
     // Writes the entire set of options to the file, allowing a user to edit the file and choose one.
     // Example: PutEnumOptions("DATABASE", "RECORD_ACCESS", "REC_SEQUENTIAL | REC_RANDOM")
     bool      PutEnumOptions(const StdString& sSection, const StdString& sTag, const StdString& sOptions);
  };

}

#endif


