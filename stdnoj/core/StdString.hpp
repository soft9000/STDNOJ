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
// A string class: Will work wherever the STDLIB is sold, by Randall Nagy
// ===============================================================
// The inspiration behind this class came from working with many other string
// classes that were either;
// (1) Too complicated to debug and maintain.
// (2) Too generalized to have members like to_upper().
// (3) Too convoluted to allow for the easy evolution of the base class.
// (4) Did not rely upon the "zero-based" cognative model and STDLIB functions
//     already in most ASCII-oriented software developer's minds (i.e: most
//     of the planet...)
// ****************************************************************************************************
// * We have used this code in many commercial applications. However, the code is beginning to show its
// * age. We anticipate a rewrite very soon - so look for wide character support (etc) in the very near
// * future!
// ****************************************************************************************************
// *                       WE HAVE USED THIS CODE IN THE PRODUCTS AT
// *                              www.TheQuoteForToday.com
// *                                www.Cookpedia.com
// *                                 www.Soft9000.com
// ****************************************************************************************************
//
// 07/18/97: Class created, Randall Nagy
// 07/22/97: Added append() and prepend(), Randall Nagy
// 07/29/97: Added remove(), Randall Nagy
// 07/31/97: Added r/find(const char *, ..), Randall Nagy
// 07/31/97: Added reverse(), Randall Nagy
// 08/26/97: Ported to WIN32. Added default to cstring under bc, Randall Nagy
// 09/15/97: Added operators < and >, Randall Nagy
// 11/10/97: Fixed readline(is.readline(TOO_BIG)) BUG (crash on buffer overflow), Randall Nagy
//           Prompted the addition of the iomax() member functions, Randall Nagy
//           Copied over insert(ss, const char *) from a vectored version, Randall Nagy
// 12/03/97: Added strip() and Clip(), Randall Nagy
// 12/29/97: Added to_upper, to_lower(), is_null(), read_to_delim(), strip(), read_token(), Randall Nagy (MailBase32_1 port)
// 12/30/98: Added snip(), Randall Nagy (GUIShell promotion)
// 01/07/98: Fixed array-postfix bug in strip() and Clip(), Randall Nagy
//           Added r/find(string, long)
// 01/15/98: Fixed bug on write_stream() of null strings, Randall Nagy
// 01/19/98: Fixed rfind() to invert and use the 'pos', Randall Nagy
// 01/26/98: Deliberately "broke" substr() so we can fix it to work like ANSI / Borland. See note at same, Randall Nagy
// 01/27/98: Deliberately "broke" remove() so we can fix it to work like ANSI / Borland. See note at same, Randall Nagy
// 02/16/98: Added support for single-characters, Randall Nagy
// 03/11/98: Added and removed const char *() conversion operator (VC++ conflicted with [], error C2666): LPCSTR would work, Randall Nagy
// 03/24/98: Updated append() and prepend() to "grow" ONLY when absolutely necessary, Randall Nagy
// 03/25/98: Got rid of sizeof(NULL) padding on stream reading, Randall Nagy
// 04/20/98: Added ANSI assign(*, size_t, size_t), append(ibid) and _substr(ibid), Randall Nagy
//           Re-implemented substr() && confidently using _substr() bounds-checking.
//           Fixed bug in remove_bytes().
// 04/28/98: Fixed find() to work like ANSI, Randall Nagy
// 05/20/98: Created streamString. Good for use with Array<T>, Randall Nagy
// 05/22/98: Copied over to string.hpp: Now fixed back to 100% ANSI!
// 07/30/98: Added fix to assign(...) and _substr() to accomidate assignment of NULL's, Randall Nagy
// 09/02/98: Fixed self-assignment bug in assign(const char *), Randall Nagy
// 10/06/98: Added replace() and three (3) case-insensitive (ci_strcmp()...) comparison primitives, Randall Nagy
//           Wired-in case-sensitivity overrides on a per-function basis, Randall Nagy
//           Removed all of the "virtual" nonsense: This is purely a utility class! Randall Nagy
//           Updated and re-ran the test-validation suite. Re-fixed a self-assign()ment bug when pointers are <> but still same buffer, Randall Nagy
// 10/19/98: Renamed the entire class (again): Borland's BCB3's str::string in except.h collision reminds me that the name "string" for this class is ill-advized.., Randall Nagy
// 10/20/98: Fixed a BUG in ci_strcmp(): Occurred when *str1 or *str2 was NULL: Default was 2return 0!
//           Put back virtual functions on the "big 5", Randall Nagy
// 10/29/98: Added macros for read_string and write_string for CODE compatability, Randall Nagy
// 11/02/98: Added operators to work on streaming of nstring and size_t, Randall Nagy
// 11/05/98: Wrote a loosely-compatable version of read_string and write_string for Quotez!, Randall Nagy
// 12/08/98: Added mkwhite(void): Used to teanslate white space characters into REAL ' ', Randall Nagy
// 12/28/98: Set-up n_string as a stand-alone header for code-level enumeration, Randall Nagy
// 02/01/99: Added remove_prefix(), Randall Nagy
// 02/06/99: Added insert(size_t, n_string&), Randall Nagy
// 03/04/99: Fixed BUG in ci_strcmp() that made "fl.jobs" == "fl.jobs.www", Randall Nagy
// 04/24/99: Fixed bug in remove_pos() when `end` is NULL.
//           Fixed strip() and Clip() (both had a similar problem), Randall Nagy
// 07/26/99: Verification port to UNIX: Added strrev(), fixed prototypes. Randall Nagy
// 08/04/99: Added get_token() and remove_token(), Randall Nagy
//           Added is_space(), Randall Nagy
// 08/05/99: Fixed overflow and underflow bugs in get_ and remove_token(), Randall Nagy
// 09/14/99: Added remove/copy_pos(), _bytes(). HotSwapp'ed _bytes_from_pos and _pos_to_pos
//           over ALL ACTIVE dependant (*.?PP, *.H) files, Randall Nagy
// 09/15/99: Added own(char *) to allow management of strstr{eam buffers, Randall Nagy
// 10/02/99: Added instance_growth_space() to allow for tunable threshold to keep from hitting the RAM for every little byte, Randall Nagy
//           Also removed re-arranged ::assign() to use same.
//           Also sped-up the append().
// 12/27/99: Renamed n_string StdString. Name touts the fact that these are still ASCIIZ string, Randall Nagy
// 01/18/00: Added is_ascii(), and seek_binary(), Randall Nagy
// 01/26/00: Added cmp(), Randall Nagy
// 01/28/00: Added is_white() is_alpha(), and is_alnum(), Randall Nagy
// 03/07/00: Added tally(), Randall Nagy
// 03/29/00: Added is_white(void), Randall Nagy
// 03/30/00: Removed bug in `remove_pos()` when 'end' was null (again!). Added DOC comments to public interface, Randall Nagy
// 04/15/00: Added remove_pattern() (a midified form of replace(...)), Randall Nagy
// 08/18/00: Fixed bug in reverse(evil cast operation), added find_last(), Randall Nagy
// 09/28/00: Added integer assignment, and as_int(), Randall Nagy
// 11/09/00: Fixed the .GT. and .LT. operators (yeeesh!), Randall Nagy
// 11/24/00: Added some externs for support of Snip(etc), Randall Nagy
// 12/02/00: Fixed overflow possibility before internal calls to _substr(), Randall Nagy
//           Added subpos(...).
// 03/10/01: Changed our definition if is_white() to match our implementation of mkwhite(), Randall Nagy
//           Added is_white(char) as the common base and made all draw definition from it.
// 03/13/01: Fixed operator >() and operator <() to work as expected with quicksort, Randall Nagy.
// 03/20/01: Fixed remove_pattern() signature to more accurately represent what function does, Randall Nagy
// 06/12/01: Fixed underflow bug in Clip(), Randall Nagy
// 06/21/01: Added assign(istream&), Randall Nagy
// 09/08/01: Updated read() and read_stream() to react better on stream error, Randall Nagy
// 10/14/01: Fixed a bug that had crept into in rfind(), Randall Nagy
// 06/04/02: Fixed bug in remove_token(), Randall Nagy
// 08/09/02: Fixed bug in is_white(), Randall Nagy
// 08/23/02: Finally added additon operators, Randall Nagy
// 10/21/02: Added to_up_low(), Randall Nagy
// 11/25/02: Added find_any() and rfind_any() to avoid confusion while using case-insensitive form of earlier namesakes, Randall Nagy
// 12/26/02: Fixed bug that crept into assign(psz, size_t, size_t), Randall Nagy
// 01/12/03: Porting over to Linux (GCC), Randall Nagy
// 11/09/03: Changed usage of operator+ to concatenate and create a new string. Operator += still appends, Randall Nagy
// 04/06/04: Added enumerate(), Randall Nagy
// 04/26/04: Added to_long(), Randall Nagy
// 05/02/04: Added append(const char ch, size_t count), Randall Nagy
// 06/07/04: Fixed rfind() to work properly when a sub-pos is specified, Randall Nagy
// 10/04/04: Added slice(), Randall Nagy
// 11/24/04: Renamed zstring to StdString to support other compilers better, Randall Nagy
// 12/16/04: Added StdString(const char ch, size_t size), Randall Nagy
// 01/04/05: Fixed a bug that somehow crept into rfind(), Randall Nagy
// 01/08/05: Fixed a bug in remove() that kept ALL occurences of `pattern` from being replaced, Randall Nagy
// 01/25/05: Added tag_remove(), Randall Nagy
// 05/30/05: Removed bug in rfind(StdString&, size_t) - now might cost more, but it works right, Randall Nagy
// 02/24/06: Fixed a bug in StdString(const char ch, size_t size), Randall Nagy
// 08/29/06: Added rCommon() and Common(), Randall Nagy
// 10/28/06: Added static Encode/Decode capability, Randall Nagy
// 11/25/06: Added ends_with(), Randall Nagy
// 12/05/06: Fixed bug in replace(again?) that occurred when a replacement containsed the pattern to replace, Randall Nagy
// 12/22/06: Added remove(const StdString&) - Also fixed a bug that crept into remove_token(), Randall Nagy
//
#ifndef StdString_hpp
#define StdString_hpp

#include <iostream>
#include <fstream>
using namespace std;

#include <cstring>

#include <core/Array.hpp>

namespace stdnoj {
    class StdString;
    class n_stream;


    size_t Strip(StdString& str, const char ch);
    size_t Clip(StdString& str, const char ch);
    int getline(istream& is, StdString& str);


    const size_t MIN_GROW_SPACE = 12;
    const size_t MAX_READ_DEFAULT = 10240;

#ifndef MAX_TOKEN
#define MAX_TOKEN 1024        // a reasonable buffer size for token-parsing
#endif

    const size_t npos = ((size_t) - 1L);
    const size_t NPOS = npos;

#ifdef WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

    class EXPORT StdString // DOC: The `StdString` class and it's derivitaves are copyright (c) 1997 - 2000 by Randall Nagy. Permission is granted to use and modify these classes as long as copyright holder is not held responsible for any problems resulting from use. All rights reserved.
    {
    private:
        char *psz_alloc;
        size_t sz_alloc;
        size_t max_read;
        void _init(void);
        char * strrev(char *) const;

    protected:
        size_t extra_bytes;
        // "case insensitive" comparison functions;
        char *ci_strstr(const char *str1, const char *str2, int ignore_case = 0) const;
        int ci_strcmp(const char *str1, const char *str2, int ignore_case = 0) const;
        char *ci_strchr(const char *str1, char ch, int ignore_case = 0) const;

        char *_substr(const char *psz, size_t start, size_t end = npos) const;

    public:

        enum strip_type {
            Leading, Trailing, Both
        };

        StdString(void) {
            _init();
        } // DOC: Object constructor - default.

        StdString(const StdString& ss) {
            _init();
            assign(ss);
        } // DOC: Object constructor for itself.

        StdString(const char *ss) {
            _init();
            assign(ss);
        } // DOC: Object constructor for a char pointer.

        StdString(const char ch) {
            _init();
            assign(ch);
        } // DOC: Object constructor for a char.

        StdString(int iss) {
            _init();
            assign(iss);
        } // DOC: Object constructor for an int.

        StdString(long iss) {
            _init();
            assign(iss);
        } // DOC: Object constructor for a long.

        StdString(bool iss) {
            _init();
            assign(int(iss));
        } // DOC: Object constructor for a bool.

        StdString(const char ch, size_t size) // DOC: Object constructor. Creates a string containing the series of n chars.
        {
            _init();
            char *pBuf = new char[size + 1];
            ::memset(pBuf, ch, size);
            pBuf[size] = 0;
            own(pBuf);
        }

        virtual ~StdString(void); // DOC: Virtual object destructor.

        void iomax(size_t sz); // DOC: Allows you to over-ride the default i/o buffer size.
        size_t iomax(void); // DOC: Returns the value set-up by iomax(size_t).

        // instance_growth_space can be arbitrarily HUGE so as to create a RAM "byte cache".
        // NOTE THAT THE VALUE APPLIES TO THE LOCAL INSTANCE ONLY! This limitation means
        // that your instance_growth_space WILL NOT be re-assigned via operator=, so any
        // "wasted space" at the end of a huge buffer can be easily reclaimed by simple
        // re-assignemnt;
        void instance_growth_space(size_t sz); // DOC: Allows you to create an instance-based cache-size for string-growth.
        size_t instance_growth_space(void); // DOC: Returns the value set-up by instance_growth_space(size_t).

        const char *own(char *psz); // ASCIIZ buffers, only!               // DOC: CAUTION! ASCIIZ strings only! Member expects a string from an externally, dynamically-allocated, source. Will free same upon object destruction (i.e: ownership transferes in). Allocation *MUST* occur with `new char[n]`, as `delete [] *psz` is used.
        const char *own(char *psz, size_t); // DOC: CAUTION! Member expects a string from an externally, dynamically-allocated, source. Will free same upon object destruction (i.e: ownership transferes in). Allocation *MUST* occur with `new char[n]`, as `delete [] *psz` is used.

        int equals(const StdString& ss, int ignore_case = 0) const;

        StdString& assign(const char *psz, size_t start, size_t end = npos); // DOC: Assigns item to object. Prior contents (if any) are destroyed.
        StdString& assign(const char *psz_alloc); // DOC: Assigns item to object. Prior contents (if any) are destroyed.
        StdString& assign(const StdString& ss); // DOC: Assigns item to object. Prior contents (if any) are destroyed.
        StdString& assign(const char ch); // DOC: Assigns item to object. Prior contents (if any) are destroyed.
        StdString& assign(int); // DOC: Assigns item to object. Prior contents (if any) are destroyed.
        StdString& assign(long); // DOC: Assigns item to object. Prior contents (if any) are destroyed.
        StdString& assign(istream&); // DOC: Assigns item to object. Prior contents (if any) are destroyed.

        StdString& append(const char *psz, size_t start, size_t end = npos); // DOC: Insert item from `start` to `end` (inclusive) at the end of the string. Default includes all characters at and after 'start'.
        StdString& append(const char *psz_alloc); // DOC: Insert item at the end of the string (after last char).
        StdString& append(const StdString& ss); // DOC: Insert item at the end of the string (after last char).
        StdString& append(const char ch); // DOC: Insert item at the end of the string (after last char).
        StdString& append(const char ch, size_t count); // DOC: Append a number of a single char.

        StdString& prepend(const char *psz_alloc); // DOC: Insert item at the begining of the string (before first char).
        StdString& prepend(const StdString& ss); // DOC: Insert item at the begining of the string (before first char).
        StdString& prepend(const char ch); // DOC: Insert item at the begining of the string (before first char).

        size_t length(void) const; // DOC: Returns the number of bytes in the string using strlen(STDLIB). Returns the number of characters in string, excluding the terminal NULL.
        void reverse(void); // DOC: Inverts the string.
        void to_upper(void); // DOC: Converts string to upper-case using toupper(STDLIB).
        void to_lower(void); // DOC: Converts string to lower case using tolower(STDLIB).
        void to_up_low(void);
        const char *c_str(void) const; // DOC: Returns the null-terminated string.

        bool is_null(void) const; // DOC: Returns 1 is the string is empty (i.e: (*this)[0] == NULL).
        bool is_white(char) const; // DOC: Base testing for is_white().
        bool is_white(size_t) const; // DOC: Same as is_white(), but starts at zero-based position in string.
        bool is_alnum(size_t) const; // DOC: Same as is_alnum(), but starts at zero-based position in string.
        bool is_alpha(size_t) const; // DOC: Same as is_alpha(), but starts at zero-based position in string.
        bool is_white(void) const; // DOC: Returns 1 if the string iscntrl(STDLIB) or 0x32.
        bool is_space(void) const; // DOC: Returns 1 is the string isspace(STDLIB).
        bool is_ascii(void) const; // DOC: Returns 1 is string is all ASCII characters (0 - 127).

        int strip(strip_type = Both, char ch = ' '); // DOC: Removes all contiguous occurences of 'ch' from either 'Leading', 'Trailing', or 'Both' relative string-positions. Returns 1 if operation performed. else 0.
        int snip(char delim = ' '); // DOC: Removes 'delim' prefix from string AND ALL SUBSEQUENT CONTIGUIOUS DELIMITERS! Eg: Snip('-') from "123--456--759-0" => "456--789-0";  Returns 1 if operation performed. else 0.
        bool mkwhite(void); // DOC: Maps any !is_white() values over to spaces (0x32). Returns 1 if operation performed. else 0.
        size_t seek_binary(int SkipControlChars = 0) const; // DOC: Returns the location of the first LOW-ascii, non-print-control, or non-ascii, character.

        istream& read_token(istream& is); // DOC: Extracts and stores the first white-space delimited token (i.e: uses '>>').
        StdString get_token(const StdString& sTok) const; // DOC: If token is contained in the string, it is extracted, else result is_null().
        StdString remove_token(const StdString& sTok) const; // DOC: Single case-sensitive token is removed from the string. To remove multiple, use remove_pattern. New string is returned.

        Array<StdString> slice(const StdString& str) const; // DOC: Carve up a string into an array of strings, each separated by a single character. The delimiting character is removed.
        Array<StdString> slice(char ch) const; // DOC: Carve up a string into an array of strings, each separated by a single character. The delimiting character is removed.

        Array<StdString> as_array(char ch) const {
            return slice(ch);
        } // DOC: Carve up a string into an array of strings, each separated by a single character. The delimiting character is removed.

        istream& read(istream& is, size_t); // DOC: Extracts number of charcters from stream. Underflows are treated properly (NULL terminated, etc).
        istream& readline(istream& is, char delim = '\n'); // DOC: Extracts charcters from stream, up-to and including 'delim'. If the delimiter is found, it is extracted but not stored.
        istream& read_line(istream& is, char delim = '\n'); // DOC: See readline()
        istream& read_string(istream& is, char delim = '\0'); // DOC: See readline()
        istream& read_to_delim(istream& is, char delim = '\n'); // DOC: See readline()
        istream& readline(istream& is, const StdString& delim); // DOC: Extracts charcters from stream, up-to and including 'delim'. If the delimiter is found, it is extracted but not stored.

        // "ignore case" option is not ANSI;
        size_t tally(const StdString& pattern, int ignore_case = 0) const; // DOC: Returns the number of times `pattern` is located in object.
        void insert(size_t ss, const char *); // DOC: Places NULL at 'ss', then appends string.
        void insert(size_t ss, const StdString&); // DOC: Places NULL at 'ss', then appends string.
        size_t find_any(const StdString& str, size_t pos = 0L) const; // DOC: Locates caseless pattern in string. Returns zero-based location, else `NPOS`.
        size_t rfind_any(const StdString& str, size_t pos = 0L) const; // DOC: Reverse-locates caseless pattern in string. Returns zero-based location, else `NPOS`.
        size_t find_any(const char ch, size_t pos = 0L) const; // DOC: Locates caseless pattern in string. Returns zero-based location, else `NPOS`.
        size_t rfind_any(const char ch, size_t pos = 0L) const; // DOC: Reverse-locates caseless pattern in string. Returns zero-based location, else `NPOS`.
        size_t find(char ch, size_t pos = 0L, int ignore_case = 0) const; // DOC: Locates pattern in string. Returns zero-based location, else `NPOS`.
        size_t rfind(char ch, size_t pos = 0L, int ignore_case = 0) const; // DOC: Reverse-locates pattern in string. Returns zero-based location, else `NPOS`.
        size_t find(const char *pch, size_t pos = 0L, int ignore_case = 0) const; // DOC: Locates pattern in string. Returns zero-based location, else `NPOS`.
        size_t rfind(const char *pch, size_t pos = 0L, int ignore_case = 0) const; // DOC: Reverse-locates pattern in string. Returns zero-based location, else `NPOS`.
        size_t find(const StdString&, size_t pos = 0L, int ignore_case = 0) const; // DOC: Locates pattern in string. Returns zero-based location, else `NPOS`.
        size_t rfind(const StdString&, size_t pos = 0L, int ignore_case = 0) const; // DOC: Reverse-locates pattern in string. Returns zero-based location, else `NPOS`.
        size_t find_last(const char *pch, size_t pos = 0L, int ignore_case = 0) const; //    // DOC: Locates the last pattern in string. Returns zero-based location, else `NPOS`.
        size_t find_last(const char ch, size_t pos = 0L, int ignore_case = 0) const; //    // DOC: Locates the last pattern in string. Returns zero-based location, else `NPOS`.
        bool ends_with(const StdString& sToken) const; // DOC: Return true if the string ends with a suffux

        int replace(const StdString& tok, const StdString& rep, int ignore_case = 0); // DOC: (NON-ANSI) Swaps all occurances of 'tok' with 'rep'. Returns number replaced;

        // The member function returns an object whose controlled sequence is a copy of up to `count`
        // elements of the controlled sequence beginning at position `pos`:
        StdString substr(size_t start, size_t end = NPOS) const; // DOC: 'end' is a count of the bytes to use, starting at the zero-based position 'start'.
        StdString subpos(size_t start, size_t end = NPOS) const; // DOC: 'end' is a a position to use, starting at the zero-based position 'start'.

        const char *copy_bytes(StdString& sRes, size_t start, size_t end) const; // DOC: 'end' is inclusive: Puts a NULL at 'end'. Characters at and after 'end' are removed.
        const char *copy_pos(StdString& sRes, size_t start, size_t end) const; // DOC: 'end' is inclusive: Puts a NULL at 'end'. Characters at and after 'end' are removed.
        const char *substr(StdString& sRes, size_t start) const; // DOC: 'start' is inclusive. Characters from 'start' to end of string are copied to 'sRes'.

        StdString& remove(size_t end); // DOC: 'end' is inclusive: Puts a NULL at 'end'. Characters at and after 'end' are removed.
        StdString& remove(const StdString& str); // DOC: Removes all occurances of the pattern from the string. Macro for remove_token.
        StdString& remove_bytes(size_t start, size_t end); // DOC: 'end' is exclusive: Character at 'end' IS PRESERVED IN strING.
        StdString& remove_pos(size_t start, size_t end); // DOC: 'end' is exclusive: Character at 'end' IS PRESERVED IN strING.
        StdString& remove_prefix(const StdString& sPattern, int ignore_case = 0);
        int remove_pattern(const StdString& tok, int ignore_case = 0); // DOC: (NON-ANSI) Removed all occurances of 'tok' in string. Returns number removed;
        StdString enumerate(char ch); // DOC: Removes leading string delimited by ch. Last one does not have to be delimited, but it leaves the string empty. Delimiting character is consumed.

        int cmp(const StdString&, int ignore_case = 0) const; // DOC: Compares strings using `strcmp(STDLIB)` type of results: 0 == equal, 1 == greater, -1 == less than.
        char& operator[](size_t ss) const; // DOC: Zero-based array opererator returns type of 'char&'.
        int operator>(const StdString& str) const; // DOC: (BOOLEAN) Returns 1 if greater than string, else 0.
        int operator<(const StdString& str) const; // DOC: (BOOLEAN) Returns 1 if less than string, else 0.
        int operator==(const StdString& ss) const; // DOC: (BOOLEAN) Returns 1 if equal to string, else 0.
        int operator!=(const StdString& ss) const; // DOC: (BOOLEAN) Returns 1 if not equal to string, else 0.

        StdString & operator=(const StdString& ss); // DOC: Assignment.
        StdString & operator=(const char *ss); // DOC: Assignment.
        StdString & operator=(const char ch); // DOC: Assignment.

        StdString & operator=(int i); // DOC: Integer assignment.
        int as_int(void) const; // DOC: Expression string as an integer value.
        long as_long(void) const; // DOC: Expression string as a long value.

        virtual ostream& write(ostream& os) const; // DOC: Empties entire string out to stream using "<<".
        virtual istream& read(istream& is); // DOC: Reads stream using ">>". Same as read_token().

        // Common stream format - as shared with ByteBuffer  -
        virtual ostream& write_stream(ostream& os) const; // DOC: Writes entire string to stream. Make sure binary-modes ('newline cooking') match on read and write.
        virtual istream& read_stream(istream& is); // DOC: Writes entire string to stream. Make sure binary-modes ('newline cooking') match on read and write.

        virtual ostream& write_string(ostream& os) const; // DOC: Writes entire string to stream, then appends a binary NULL.

        // virtual istream&  read_string (istream& is);

        // Encoding / Decoding is very handy when stream types might change -

        static const char*Encode(StdString& str) {
            str.replace("\n", "\\n");
            str.remove_pattern('\r');
            return str.c_str();
        }

        static const char*DecodeDOS(StdString& str) {
            Encode(str);
            str.replace("\\n", "\r\n");
            return str.c_str();
        }

        static const char*EncodeDOS(StdString& str) {
            Encode(str);
            DecodeDOS(str);
            return str.c_str();
        }

        static const char*DecodeUNIX(StdString& str) {
            Encode(str);
            str.replace("\\n", "\n");
            return str.c_str();
        }

        static const char*EncodeUNIX(StdString& str) {
            Encode(str);
            DecodeUNIX(str);
            return str.c_str();
        }
    };

    class n_stream : public StdString // DOC: Use this class to allow .read() and .write() to alias over to read/write _stream().
    {
    public:

        n_stream(void) {
        }

        n_stream(const StdString& str) {
            *this = str;
        }

        n_stream(const n_stream& str) {
            *this = str;
        }

        virtual ~n_stream(void) {
        }

        int operator==(const n_stream& str) const {
            return (StdString::operator==(str.c_str()));
        }

        int operator==(const StdString& str) const {
            return (StdString::operator==(str.c_str()));
        }

        n_stream & operator=(const n_stream& str) {
            if (this == &str)
                return *this;
            StdString::operator=(str.c_str());
            return *this;
        }

        n_stream & operator=(const StdString& str) {
            if (this == &str)
                return *this;
            StdString::operator=(str.c_str());
            return *this;
        }

        virtual ostream& write(ostream& os) const {
            return StdString::write_stream(os);
        }

        virtual istream& read(istream& os) {
            return StdString::read_stream(os);
        }
    };



    extern ostream & operator <<(ostream& os, const StdString& ss);
    extern istream & operator >>(istream& is, StdString& ss);

    extern StdString & operator <<(StdString& str, size_t sz);
    extern StdString & operator >>(StdString& str, size_t sz);

    extern StdString & operator <<(StdString& str, const StdString& str2);
    extern StdString & operator <<(StdString& str, const char *psz);

    //ofstream&   operator << (ofstream& os, const StdString& ss);
    //ifstream&   operator >> (ifstream& is, StdString& ss);

    extern StdString & operator >>(StdString& str, size_t sz);
    extern StdString & operator <<(StdString& str, size_t sz);
    extern StdString & operator <<(StdString& str, const StdString& str2);
    extern StdString & operator <<(StdString& str, const char *psz);

    extern istream & operator >>(istream& is, StdString& ss);
    extern ostream & operator <<(ostream& os, const StdString& ss);

    extern size_t strip(StdString& str, const char ch);
    extern size_t Clip(StdString& str, const char ch);
    extern size_t Snip(const StdString& str, StdString& sLeft, StdString& sRight, const char ch);

    extern int getline(istream& is, StdString& str);

    extern StdString & operator +=(StdString& str, const StdString& cstr); // DOC: Append a string.
    extern StdString operator +(const StdString& str, const StdString& cstr); // DOC: Create a new string.

    inline
    size_t rfind(const StdString& sSrc, const StdString& sPat, int ignore_case = 0) {
        StdString str = sSrc;
        str.reverse();
        StdString spat = sPat;
        spat.reverse();
        size_t whence = str.find(spat, ignore_case);
        if (whence == npos)
            return whence;
        whence += spat.length() - 1;
        return (sSrc.length() - whence - 1);
    }

    inline
    StdString tag_remove(const StdString& sSrc, char chOn = '<', char chOff = '>') {
        // Suitable for html and xml tag removal (only - no diacretic conversions!)
        StdString sResult;
        bool bCopying = true;
        char ch;
        for (size_t ss = 0L; ss < sSrc.length(); ss++) {
            ch = sSrc[ss];
            if (ch == chOn) {
                bCopying = false;
                continue;
            }
            if (ch == chOff) {
                bCopying = true;
                continue;
            }
            if (bCopying)
                sResult.append(ch);
        }
        return sResult;
    }

    inline
    StdString Common(const StdString& sC, const StdString& sD) // Returns what to string have in common
    {
        StdString sResult;
        for (size_t xx = 0; xx < sD.length(); xx++) {
            if (sC[xx] != sD[xx])
                break;
            sResult.append(sC[xx]);
        }
        return sResult;
    }

    inline
    StdString rCommon(const StdString& s1, const StdString& s2) // Returns what the END of two strings have in common
    {
        StdString sA = s1;
        StdString sB = s2;
        sA.reverse();
        sB.reverse();
        StdString sC = Common(sA, sB);
        sC.reverse();
        return sC;
    }


} // stdnoj

#endif

