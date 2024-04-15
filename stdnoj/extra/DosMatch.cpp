/* The MIT License (Open Source Approved)

Copyright (c) 1999 - 2024 Randall Nagy 

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
#include <extra/DosMatch.hpp>

// Returns the position AFTER the match;
size_t DosMatch::MatchMaskChars(const StdString& sString, const StdString& sMask)
   {
   StdString sPattern = sString;
   size_t ssA = sPattern.find(sMask);
   if(ssA != NPOS)
      return (ssA + sMask.length() +1);
   return ssA;   
   }

const char *DosMatch::MaskPortion(const StdString& sString, StdString& sMask)
   {
   size_t ss = NULL;
   while(sString[ss] == '?')
      ss++;
   while(sString[ss] == '*')
      ss++;
   if(ss)
      {
      sMask = "";
      return sMask.c_str();
      }
   sMask       = &sString[ss];
   size_t chopA= sMask.find('?');
   size_t chopB= sMask.find('*');

   if( (chopA == (size_t)NPOS) && (chopB == (size_t)NPOS))
      return sMask.c_str();

   if(chopA == NPOS)
      chopA =  NULL;
   if(chopB == NPOS)
      chopB =  NULL;

   size_t chop = (chopA > chopB) ? chopA : chopB;

   sMask[chop] = NULL;
   return sMask.c_str();
   }

bool DosMatch::HasWildcard(const StdString& std)
   {
   if(std.find('?') != NPOS)
      return true;
   if(std.find('*') != NPOS)
      return true;
   return false;
   }

bool DosMatch::Match(const StdString& sPattern, const StdString& sSubject)
   {
   bool br = false;
   size_t pp = NULL;
   size_t ss = NULL;

   // Fix the "1" == "123" fall-through error;
   if(HasWildcard(sPattern) == false)
      return (sPattern == sSubject);

   // Patterns can be smaller or the same size, but not larger;
   // (fixing another fall-through error);
   if(sPattern.length() > sSubject.length())
      return br;

   // Use the string's operator equal 
   // (avoids "" <> "", while "1" == "1") fall-through error;
   if((sPattern.length() == sSubject.length()) && (sPattern == sSubject))
      return true;

   // By this time, we HAVE wildcard(s) to expand. 
   // sSubject is assumed to be sane (asciiz);
   // *****
   while(sPattern[pp] && sSubject[ss])
      {
      if(sPattern[pp] == '?')
         {
         pp++;
         ss++;
         if(!sPattern[pp] && sSubject[ss])
            return false;
         br = true;
         continue;
         }
      if(sPattern[pp] == '*')
         {
         if(sPattern[pp + 1] == sSubject[ss])
            {
            size_t count = NULL;
            const char *pPat = &sPattern[pp + 1];
            const char *pSub = &sSubject[ss];
            while(*pPat && *pSub)
               {
               if(*pPat == *pSub)
                  {
                  count++;
                  pPat++; pSub++;
                  // Where we are going next?
                  if((*pPat == '?') || (*pPat == '*'))
                     {
                     // Okay, everything matched and tallied.
                     // Next pattern, please;
                     pp++;
                     break;
                     }
                  }
               else
                  return false;
               }
            pp += count;
            ss += count;
            br = true;
            }
         else
            {
            ss++;
            br = true;
            }
         continue;
         }
      if(sPattern[pp] != sSubject[ss])
         return false;

      br = true;
      pp++;
      ss++;
      }

   // See if the wild-card skipped-it-all, but failed to match the REST of 
   // the pattern specified in the mask;
   if(sPattern[pp] == '*' && sPattern[pp+1])
      return false;     // Yup: Not match, bubbo.

   return br;
   }
