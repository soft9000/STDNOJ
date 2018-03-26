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
#include <stdnoj.hpp>

const stdnoj::StdString sToken("!@#@~$^");

namespace stdnoj {

    size_t Paragraph::Longest(const Array<StdString>& array) {
        StdString ref;
        array.At(0, ref);
        size_t ssTot = ref.length();
        for (size_t ss = 1L; ss < array.Nelem(); ss++) {
            array.At(ss, ref);
            ssTot = ssTot > ref.length() ? ssTot : ref.length();
        }
        return ssTot;
    }

    size_t Paragraph::Shortest(const Array<StdString>& array) {
        StdString ref;
        array.At(0, ref);
        size_t ssTot = ref.length();
        for (size_t ss = 1L; ss < array.Nelem(); ss++) {
            array.At(ss, ref);
            ssTot = ssTot < ref.length() ? ssTot : ref.length();
        }
        return ssTot;
    }

    size_t Paragraph::Size(const Array<StdString>& array, bool bUseNewline) {
        size_t ssTot = 0L;
        StdString ref;
        for (size_t ss = 0L; ss < array.Nelem(); ss++) {
            array.At(ss, ref);
            ssTot += ref.length();
            if (bUseNewline == false)
                ssTot -= ref.tally("\n");
        }
        return ssTot;
    }

    StdString Paragraph::Normalize(const StdString& sLine) {
        //re-use "\usr\code\cpp\WordWrap\WordWrap.cpp"
        //re-use "\usr\code\cpp\Paragraph\Paragraph.cpp"
        StdString sText = sLine;
        sText.replace("\r\n", "\n");
        sText.replace("\n\r", "\n");
        sText.replace("\r", "\n");

        while (sText.find(" \n") != npos)
            sText.replace(" \n", "\n");
        while (sText.find("\n ") != npos)
            sText.replace("\n ", "\n");

        if (sText.length() && sText[sText.length() - 1] != '\n')
            sText.append('\n');

        sText.replace("\n", sToken);
        sText.mkwhite();
        sText.replace(sToken, "\n");
        return sText;
    }

    StdString Paragraph::Normalize(const Array<StdString>& aLines) {
        StdString sResult;
        for (size_t ss = 0L; ss < aLines.Nelem(); ss++) {
            if (ss && (sResult[sResult.length() - 1] != '\n'))
                sResult += "\n";
            StdString sLine;
            aLines.At(ss, sLine);
            sResult += sLine;
        }
        return sResult;
    }

    Array<StdString> Paragraph::Flow(const Array<StdString>& aLines) {
        StdString sResult = Normalize(aLines);
        return List(sResult);
    }

    StdString Paragraph::WordWrap(const StdString& sLine, int iLineWidth) {
        StdString sText = Normalize(sLine);

        sText.replace("\n\n", sToken);
        sText.replace("\n", " ");
        sText.replace(sToken, "\n");

        size_t ssBreak = npos;
        size_t ssCount = 0L;

        StdString sResult;
        for (int ss = 0L; ss < sText.length(); ss++) {
            switch (sText[ss]) {
                case '\n':
                {
                    // process a paragraph
                    sResult += sText.subpos(0, ss);
                    sResult += "\n"; // the SECOND newline restored ...
                    sResult.remove_pos(0, ss);
                    // restart
                    ssBreak = npos;
                    ss = -1;
                    ssCount = 0L;
                }
                    continue;

                case ' ':
                {
                    // mark a place to break
                    ssBreak = ss;
                }
                    break;

                default:
                    break;
            }

            // everything else ...
            if (ssCount >= iLineWidth) // ">=" not "==" ... think maintainence ...
            {
                if (ssBreak == npos)
                    ssBreak = ss;

                int delta = 0;
                if (sText[ssBreak] == ' ')
                    delta = 1;

                sResult += sText.subpos(0, ssBreak);
                sResult += "\n";
                sText.remove_pos(0, ssBreak + delta);
                // restart
                ssBreak = npos;
                ss = -1;
                ssCount = 0L;
            } else
                ssCount++;

        }
        sResult.replace(" \n", "\n");
        return sResult;
    }

    Array<StdString> Paragraph::List(const StdString& sLine) {
        StdString sText = Normalize(sLine);
        Array<StdString> ary;
        ary.AddZombies(100);
        size_t result = 0L;
        size_t ssStart = 0L;

        size_t whence = sText.find('\n');
        while (whence != NPOS) {
            size_t tmp = whence - ssStart;
            if (result < tmp)
                result = tmp;
            StdString sLine;
            sText.copy_pos(sLine, ssStart, whence + 1);
            ary[ary.Nelem()] = sLine;
            ssStart = whence + 1;
            whence = sText.find('\n', ssStart);
        }
        return ary;
    }

    Array<StdString> Paragraph::List(const StdString& sLine, int iLen) {
        StdString sText = Normalize(sLine);
        StdString sBest = WordWrap(sLine, iLen);
        return List(sBest);
    }

    bool PipeArray::Parse(Array<StdString>& ary, const StdString& str, char ch) {
        StdString sch(ch);
        return Parse(ary, str, sch);
    }

    bool PipeArray::Parse(Array<StdString>& ary, const StdString& str, const StdString& sch) {
        StdString ssr = str;
        ary.Empty();
        size_t whence = ssr.find(sch);
        while (whence != NPOS) {
            ary[ary.Nelem()] = ssr.substr(0, whence);
            ssr.remove_pos(0, whence + sch.length());
            whence = ssr.find(sch);
        }
        if (ssr.length())
            ary[ary.Nelem()] = ssr;
        if (ary.Nelem())
            return true;
        return false;
    }

    bool PipeArray::Gen(Array<StdString>& ary, StdString& str, char ch) {
        stringstream srm;
        for (size_t ss = 0L; ss < ary.Nelem(); ss++)
            srm << ary[ss] << ch;
        str = PRESTO(srm);
        if (str.length())
            return true;
        return false;
    }

} // namespace
