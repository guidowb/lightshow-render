#include <stdlib.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../src/Compiler.h"
#include "TestHelpers.h"

class CompilerTests : public CppUnit::TestFixture  {
    CPPUNIT_TEST_SUITE( CompilerTests );
    CPPUNIT_TEST( testFailure );
    CPPUNIT_TEST( testSolid );
    CPPUNIT_TEST( testSolidMissingColor );
    CPPUNIT_TEST( testBlock );
    CPPUNIT_TEST( testSubCommand );
    CPPUNIT_TEST( testSubBlock );
    CPPUNIT_TEST( testNoBlock );
    CPPUNIT_TEST_SUITE_END();

    void testFailure() {
        Compiler compiler("CompilerTests::testFailure", "nonsense");
        Renderer *renderer = compiler.compile();
        CPPUNIT_ASSERT_NOT_NULL(renderer);
        CPPUNIT_ASSERT_EQUAL(LEXER_ERROR, compiler.maxErrorLevel());
    }

    void testSolid() {
        Compiler compiler("CompilerTests::testSolid", "solid #000");
        Renderer *renderer = compiler.compile();
        CPPUNIT_ASSERT_NOT_NULL(renderer);
        CPPUNIT_ASSERT_EQUAL(LEXER_OK, compiler.maxErrorLevel());
    }

    void testSolidMissingColor() {
        Compiler compiler("CompilerTests::testSolid", "solid");
        Renderer *renderer = compiler.compile();
        CPPUNIT_ASSERT_NOT_NULL(renderer);
        CPPUNIT_ASSERT_EQUAL(LEXER_ERROR, compiler.maxErrorLevel());
    }

    void testBlock() {
        Compiler compiler("CompilerTests::testBlock", "solid #000\ndots 100 #111 #222\ndots 10 #333 #444\n");
        Renderer *renderer = compiler.compile();
        CPPUNIT_ASSERT_NOT_NULL(renderer);
        CPPUNIT_ASSERT_EQUAL(LEXER_OK, compiler.maxErrorLevel());
    }

    void testSubCommand() {
        Compiler compiler("CompilerTests::testSubCommand", "segment 0 1 solid #000\nsolid #fff\n");
        Renderer *renderer = compiler.compile();
        CPPUNIT_ASSERT_NOT_NULL(renderer);
        CPPUNIT_ASSERT_EQUAL(LEXER_OK, compiler.maxErrorLevel());
    }

    void testSubBlock() {
        Compiler compiler("CompilerTests::testSubBlock", "segment 0 1\n  solid #000\nsolid #fff\n");
        Renderer *renderer = compiler.compile();
        CPPUNIT_ASSERT_NOT_NULL(renderer);
        CPPUNIT_ASSERT_EQUAL(LEXER_OK, compiler.maxErrorLevel());
    }

    void testNoBlock() {
        Compiler compiler("CompilerTests::testNoBlock", "segment 0 1\nsolid #000\nsolid #fff\n");
        Renderer *renderer = compiler.compile();
        CPPUNIT_ASSERT_NOT_NULL(renderer);
        CPPUNIT_ASSERT_EQUAL(LEXER_OK, compiler.maxErrorLevel());
    }
};
