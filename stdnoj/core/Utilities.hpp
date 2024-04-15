/* The MIT License (Open Source Approved)

Copyright (c) 1993 - 2024 R. A. Nagy 

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
// 07/09/2005 - After considering my legacy WordWrap.hpp and SimpleTextFlow.hpp
// work, I decided to add a new Paragraph rather than to include those into
// the official namespace, R. Nagy
//
#ifndef stdnoj_util
#define stdnoj_util

#include <core/StdString.hpp>
#include <core/Array.hpp>
#include <core/File.hpp>


namespace stdnoj {
    //! \class The Paragraph class is used to normalize text in such
    //! a way so as allow for the formatting (and re-formatting) of
    //! text into lines, characters, and paragraphs suitable for ASCII 
    //! rendering.

    class Paragraph {
    public:
        //! Convert a string with unknown or mixed newline conventions over
        //! into a standard, common, vendor-liberated convention: Lines
        //! end in \n, paragraphs will end in \n\n, and any 
        //! irrgular whitespaces are converted to a single ' '.
        //! \param sLine - The line to normalize.
        //! \return The normalized line.
        static StdString Normalize(const StdString& sLine);

        //! Convert an array of strings with unknown or mixed newline 
        //! conventions into a standard, common, vendor-liberated 
        //! format: Uses Normalize(StdString) (above).
        //! \param aLines - The lines to normalize.
        //! \return The normalized line.
        static StdString Normalize(const Array<StdString>& aLines);

        //! Convert an array of strings with unknown or mixed newline 
        //! conventions into a standard, common, vendor-liberated 
        //! format: Uses Normalize(StdString) (above).
        //! \param aLines - The lines to normalize.
        //! \return An array of normalized lines.
        static Array<StdString> Flow(const Array<StdString>& aLines);

        //! Word wrapping attempts to break (\n) a line of text at 
        //! ASCII spaces (32d). Takes a line and "wraps" it into a 
        //! series of lines of the length specified.  
        //! Non-newline whitespace is converted to 32d.
        //! Normalizes all lines - Paragraphs (\n\n) are honored, 
        //! everything else could change (i.e. longer \n's are changed.)
        //! \param sLine - The line to normalize and word-wrap.
        //! \return The normalized & word-wrapped line.
        static StdString WordWrap(const StdString& sLine, int iLen);

        //! Normalizes and extracts newline-laden sub-strings into an array.
        //! \param sLine - The line to normalize and format into an array.
        //! \return The normalized line, as an array.
        static Array<StdString> List(const StdString& sLine);

        //! Word-wraps, normalizes and extracts newline-laden sub-strings 
        //! into an array.
        //! \param sLine - The line to normalize and format into an array.
        //! \param iLen - The desired word-wrap length of each line
        //! \return The normalized & word-wrapped line, as an array.
        static Array<StdString> List(const StdString& sLine, int iLen);

        //! Longest line
        static size_t Longest(const Array<StdString>& array);

        //! Shortest line
        static size_t Shortest(const Array<StdString>& array);

        //! Includes newlines ... or not
        static size_t Size(const Array<StdString>& array, bool bUseNewline = true);

    };

    class PipeArray {
    public:
        static bool Parse(Array<StdString>& ary, const StdString& str, char ch = '|');
        static bool Parse(Array<StdString>& ary, const StdString& str, const StdString& sPattern);
        static bool Gen(Array<StdString>& ary, StdString& str, char ch = '|');
    };

    template <class T, class SZ = size_t>
    class FileObjectHeap {
    public:
        static size_t Tally(stdnoj::File& file, File::TranslationModes tm);
        static bool Load(Array<T>& aResult, File& file, File::TranslationModes tm);
        static bool Save(Array<T>& aResult, File& file, File::TranslationModes tm);
    };

    template <class T, class SZ>
    size_t FileObjectHeap<T, SZ>::Tally(File& file, File::TranslationModes tm) {
        size_t iTally = 0L;
        T ref;
        istream& is = file.OpenRead(tm);
        while (is) {
            ref.read(is);
            if (is)
                iTally++;
        }
        return iTally;
    }

    template <class T, class SZ>
    bool FileObjectHeap<T, SZ>::Load(Array<T>& aResult, File& file, File::TranslationModes tm) {
        if (file.Exists() == false)
            return false;
        aResult.AddZombies(FileObjectHeap<T, SZ>::Tally(file, tm) + 1);
        T ref;
        istream& is = file.OpenRead(tm);
        while (is) {
            ref.read(is);
            if (!is)
                continue;
            aResult.Append(ref);
        }
        return true;
    }

    template <class T, class SZ>
    bool FileObjectHeap<T, SZ>::Save(Array<T>& aResult, File& file, File::TranslationModes tm) {
        bool br = true;
        ostream& os = file.OpenWrite(tm);
        for (size_t ss = 0L; ss < aResult.Nelem(); ss++) {
            aResult[ss].write(os);
            if (!os) {
                br = false;
                break;
            }
        }
        file.Close();
        return br;
    }

}

#endif
