#include "../src/Parser.h"

#include <stdlib.h>
#include <cppunit/extensions/HelperMacros.h>

class ParserTests : public CppUnit::TestFixture  {
    CPPUNIT_TEST_SUITE( ParserTests );
    CPPUNIT_TEST( testSimpleCommand );
    CPPUNIT_TEST_SUITE_END();

    void testSimpleCommand() {
        Parser parser("solid");
        CPPUNIT_ASSERT_STRINGS_EQUAL("solid", parser.getCommand());
    }
};
