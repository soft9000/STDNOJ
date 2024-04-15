// Note: Operator overloading is used to define the test cases. 
// The class parameter should not be used, so make it const.
//
// 01/28/1998: Updated test cases to reflect new usage paradigm, Randall Nagy
// 06/23/2003: Updated to test initial stdnoj usage.
//   Updated to support changes in remove() usage.
// 08/04/2008: Created _spi::test(), Randall Nagy
//   Runs fine under VS.NET 2003.
// 01/04/2005: Moving test cases here for a more unified regression testing capability, Randall Nagy
//

#ifndef stdnoj_testcases_Hpp
#define stdnoj_testcases_Hpp

#include <stdnoj.hpp>

using namespace stdnoj;


#include <extra/DosMatch.hpp>

class TestCases
{
   private:
      int io_test(const StdString& str, int mo);
   public:
      bool test(const _spi<seek_t> spx, ostream& os);
      int  test(const StdString& str, ostream& os);
      bool test(const Paragraph& ref, ostream& os);
      bool test(const DosMatch& dm, ostream& os);
      
      static bool Test(ostream& os);
};

#endif
