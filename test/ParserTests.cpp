#include <stdlib.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../src/Parser.h"
#include "TestHelpers.h"

class ParserTests : public CppUnit::TestFixture  {
    CPPUNIT_TEST_SUITE( ParserTests );
    CPPUNIT_TEST( testSimpleCommand );
    CPPUNIT_TEST( testExtraArguments );
    CPPUNIT_TEST( testInvalidInt );
    CPPUNIT_TEST( testValidInt );
    CPPUNIT_TEST( testNegativeInt );
    CPPUNIT_TEST( testInvalidColor );
    CPPUNIT_TEST( testInvalidColorDigit );
    CPPUNIT_TEST( testInvalidColorLength );
    CPPUNIT_TEST( testInvalidDuration );
    CPPUNIT_TEST( testInvalidDurationUnit );
    CPPUNIT_TEST( testInvalidDurationLongUnit );
    CPPUNIT_TEST( testValidDurationSeconds );
    CPPUNIT_TEST( testValidDurationMinutes );
    CPPUNIT_TEST( testValidDurationHours );
    CPPUNIT_TEST( testInvalidTime );
    CPPUNIT_TEST( testValidTimeNoSeconds );
    CPPUNIT_TEST( testValidTimeSeconds );
    CPPUNIT_TEST( testInvalidDate );
    CPPUNIT_TEST( testValidDate );
    CPPUNIT_TEST( testShortRGB );
    CPPUNIT_TEST( testShortRGBA );
    CPPUNIT_TEST( testLongRGB );
    CPPUNIT_TEST( testLongRGBA );
    CPPUNIT_TEST( testTopLevelBlock );
    CPPUNIT_TEST( testNestedBlock );
    CPPUNIT_TEST( testUnexpectedBlock );
    CPPUNIT_TEST_SUITE_END();

    void testSimpleCommand() {
        Parser parser("ParserTests::testSimpleCommand", "solid");
        CPPUNIT_ASSERT(parser.isName());
        CPPUNIT_ASSERT(parser.getCommand() == "solid");
        parser.endCommand();
        CPPUNIT_ASSERT_EQUAL(0, parser.maxErrorLevel());
    }

    void testExtraArguments() {
        Parser parser("ParserTests::testExtraArguments", "solid blah");
        CPPUNIT_ASSERT(parser.getCommand() == "solid");
        parser.endCommand();
        CPPUNIT_ASSERT_EQUAL(LEXER_ERROR, parser.maxErrorLevel());
    }

    void testInvalidInt() {
        Parser parser("ParserTests::testInvalidInt", "nonsense");
        CPPUNIT_ASSERT(!parser.isInteger());
        CPPUNIT_ASSERT_EQUAL(0, parser.getInteger());
        CPPUNIT_ASSERT_EQUAL(LEXER_ERROR, parser.maxErrorLevel());
    }

    void testValidInt() {
        Parser parser("ParserTests::testValidInt", "1234");
        CPPUNIT_ASSERT(parser.isInteger());
        CPPUNIT_ASSERT_EQUAL(1234, parser.getInteger());
        CPPUNIT_ASSERT_EQUAL(LEXER_OK, parser.maxErrorLevel());
    }

    void testNegativeInt() {
        Parser parser("ParserTests::testValidInt", "-1234");
        CPPUNIT_ASSERT(parser.isInteger());
        CPPUNIT_ASSERT_EQUAL(-1234, parser.getInteger());
        CPPUNIT_ASSERT_EQUAL(LEXER_OK, parser.maxErrorLevel());
    }

    void testInvalidDuration() {
        Parser parser("ParserTests::testInvalidDuration", "1234");
        CPPUNIT_ASSERT_EQUAL(0, (int) parser.getDuration());
        CPPUNIT_ASSERT_EQUAL(LEXER_ERROR, parser.maxErrorLevel());
    }

    void testInvalidDurationUnit() {
        Parser parser("ParserTests::testInvalidDurationUnit", "1234x");
        CPPUNIT_ASSERT_EQUAL(0, (int) parser.getDuration());
        CPPUNIT_ASSERT_EQUAL(LEXER_ERROR, parser.maxErrorLevel());
    }

    void testInvalidDurationLongUnit() {
        Parser parser("ParserTests::testInvalidDurationLongUnit", "1234seconds");
        CPPUNIT_ASSERT_EQUAL(0, (int) parser.getDuration());
        CPPUNIT_ASSERT_EQUAL(LEXER_ERROR, parser.maxErrorLevel());
    }

    void testValidDurationSeconds() {
        Parser parser("ParserTests::testValidDurationSeconds", "1234s");
        CPPUNIT_ASSERT_EQUAL(1234000, (int) parser.getDuration());
        CPPUNIT_ASSERT_EQUAL(LEXER_OK, parser.maxErrorLevel());
    }

    void testValidDurationMinutes() {
        Parser parser("ParserTests::testValidDurationMinutes", "2m");
        CPPUNIT_ASSERT_EQUAL(120000, (int) parser.getDuration());
        CPPUNIT_ASSERT_EQUAL(LEXER_OK, parser.maxErrorLevel());
    }

    void testValidDurationHours() {
        Parser parser("ParserTests::testValidDurationHours", "3h");
        CPPUNIT_ASSERT_EQUAL(3 * 60 * 60000, (int) parser.getDuration());
        CPPUNIT_ASSERT_EQUAL(LEXER_OK, parser.maxErrorLevel());
    }

    void testInvalidTime() {
        Parser parser("ParserTests::testInvalidTime", "4o'clock");
        CPPUNIT_ASSERT_EQUAL(0, (int) parser.getTime());
        CPPUNIT_ASSERT_EQUAL(LEXER_ERROR, parser.maxErrorLevel());
    }

    void testValidTimeNoSeconds() {
        Parser parser("ParserTests::testValidTimeNoSeconds", "12:34");
        CPPUNIT_ASSERT_EQUAL(12 * 3600 + 34 * 60, (int) parser.getTime());
        CPPUNIT_ASSERT_EQUAL(LEXER_OK, parser.maxErrorLevel());
    }

    void testValidTimeSeconds() {
        Parser parser("ParserTests::testValidTimeSeconds", "12:34:56");
        CPPUNIT_ASSERT_EQUAL(12 * 3600 + 34 * 60 + 56, (int) parser.getTime());
        CPPUNIT_ASSERT_EQUAL(LEXER_OK, parser.maxErrorLevel());
    }

    void testInvalidDate() {
        Parser parser("ParserTests::testInvalidDate", "14/14");
        CPPUNIT_ASSERT_EQUAL(0, (int) parser.getDate());
        CPPUNIT_ASSERT_EQUAL(LEXER_ERROR, parser.maxErrorLevel());
    }

    void testValidDate() {
        Parser parser("ParserTests::testValidDate", "12/25");
        CPPUNIT_ASSERT_EQUAL((12 << 8) + 25, (int) parser.getDate());
        CPPUNIT_ASSERT_EQUAL(LEXER_OK, parser.maxErrorLevel());
    }

    void testInvalidColor() {
        Parser parser("ParserTests::testInvalidColor", "nonsense");
        CPPUNIT_ASSERT(!parser.isColor());
        CPPUNIT_ASSERT_EQUAL(RGBA_NULL, parser.getColor());
        CPPUNIT_ASSERT_EQUAL(LEXER_ERROR, parser.maxErrorLevel());
    }

    void testInvalidColorDigit() {
        Parser parser("ParserTests::testInvalidColorDigit", "#00zz0000");
        CPPUNIT_ASSERT(!parser.isColor());
        CPPUNIT_ASSERT_EQUAL(RGBA_NULL, parser.getColor());
        CPPUNIT_ASSERT_EQUAL(LEXER_ERROR, parser.maxErrorLevel());
    }

    void testInvalidColorLength() {
        Parser parser("ParserTests::testInvalidColorLength", "#12345");
        CPPUNIT_ASSERT(!parser.isColor());
        CPPUNIT_ASSERT_EQUAL(RGBA_NULL, parser.getColor());
        CPPUNIT_ASSERT_EQUAL(LEXER_ERROR, parser.maxErrorLevel());
    }

    void testShortRGB() {
        Parser parser("ParserTests::testShortRGB", "#123");
        CPPUNIT_ASSERT(parser.isColor());
        CPPUNIT_ASSERT_EQUAL(RGBA(0x11, 0x22, 0x33, 0xFF), parser.getColor());
        CPPUNIT_ASSERT_EQUAL(LEXER_OK, parser.maxErrorLevel());
    }

    void testShortRGBA() {
        Parser parser("ParserTests::testShortRGBA", "#abcd");
        CPPUNIT_ASSERT(parser.isColor());
        CPPUNIT_ASSERT_EQUAL(RGBA(0xAA, 0xBB, 0xCC, 0xDD), parser.getColor());
        CPPUNIT_ASSERT_EQUAL(LEXER_OK, parser.maxErrorLevel());
    }

    void testLongRGB() {
        Parser parser("ParserTests::testLongRGB", "#123456");
        CPPUNIT_ASSERT(parser.isColor());
        CPPUNIT_ASSERT_EQUAL(RGBA(0x12, 0x34, 0x56, 0xFF), parser.getColor());
        CPPUNIT_ASSERT_EQUAL(LEXER_OK, parser.maxErrorLevel());
    }

    void testLongRGBA() {
        Parser parser("ParserTests::testLongRGBA", "#09ABCDEF");
        CPPUNIT_ASSERT(parser.isColor());
        CPPUNIT_ASSERT_EQUAL(RGBA(0x09, 0xAB, 0xCD, 0xEF), parser.getColor());
        CPPUNIT_ASSERT_EQUAL(LEXER_OK, parser.maxErrorLevel());
    }

    void testTopLevelBlock() {
        Parser parser("ParserTests::testTopLevelBlock", "command1\ncommand2\ncommand3");
        Word indent = parser.startBlock();
        CPPUNIT_ASSERT(parser.inBlock());
        CPPUNIT_ASSERT(parser.getCommand() == "command1"); parser.endCommand();
        CPPUNIT_ASSERT(parser.inBlock());
        CPPUNIT_ASSERT(parser.getCommand() == "command2"); parser.endCommand();
        CPPUNIT_ASSERT(parser.inBlock());
        CPPUNIT_ASSERT(parser.getCommand() == "command3"); parser.endCommand();
        CPPUNIT_ASSERT(!parser.inBlock());
        CPPUNIT_ASSERT_EQUAL(LEXER_OK, parser.maxErrorLevel());
    }

    void testNestedBlock() {
        Parser parser("ParserTests::testNestedBlock", "command1\n  command2\n  command3\ncommand4");
        CPPUNIT_ASSERT(parser.inBlock());
        CPPUNIT_ASSERT(parser.getCommand() == "command1");
        CPPUNIT_ASSERT(parser.hasBlock());
        Word indent = parser.startBlock();
        CPPUNIT_ASSERT(parser.inBlock());
        CPPUNIT_ASSERT(parser.getCommand() == "command2"); parser.endCommand();
        CPPUNIT_ASSERT(parser.inBlock());
        CPPUNIT_ASSERT(parser.getCommand() == "command3"); parser.endCommand();
        CPPUNIT_ASSERT(!parser.inBlock());
        parser.endBlock(indent);
        CPPUNIT_ASSERT(parser.inBlock());
        CPPUNIT_ASSERT(parser.getCommand() == "command4"); parser.endCommand();
        CPPUNIT_ASSERT(!parser.inBlock());
        CPPUNIT_ASSERT_EQUAL(LEXER_OK, parser.maxErrorLevel());
    }

    void testUnexpectedBlock() {
        Parser parser("ParserTests::testUnexpectedBlock", "command1\n  command2\ncommand3");
        CPPUNIT_ASSERT(parser.inBlock());
        CPPUNIT_ASSERT(parser.getCommand() == "command1");
        parser.endCommand();
        CPPUNIT_ASSERT(parser.getCommand() == "command3");
        CPPUNIT_ASSERT_EQUAL(LEXER_ERROR, parser.maxErrorLevel());
    }
};
