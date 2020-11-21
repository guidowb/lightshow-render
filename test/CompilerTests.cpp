#include "../src/Compiler.h"

#include <stdlib.h>
#include <cppunit/extensions/HelperMacros.h>
#include "TestHelpers.h"

class CompilerTests : public CppUnit::TestFixture  {
    CPPUNIT_TEST_SUITE( CompilerTests );
    CPPUNIT_TEST( testFailure );
    CPPUNIT_TEST_SUITE_END();

    void testFailure() {
        Compiler compiler("testFailure", "nonsense");
        Renderer *renderer = compiler.compile();
        CPPUNIT_ASSERT_NULL(renderer);
        CPPUNIT_ASSERT_EQUAL(LEXER_ERROR, compiler.maxErrorLevel());
    }

    void testSolid() {
        Compiler compiler("testSolid", "solid #000");
        Renderer *renderer = compiler.compile();
        CPPUNIT_ASSERT_NOT_NULL(renderer);
    }
};
