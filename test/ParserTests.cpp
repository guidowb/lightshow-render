#include "../src/Parser.h"

#include <stdlib.h>
#include <cppunit/extensions/HelperMacros.h>

class ParserTests : public CppUnit::TestFixture  {
    CPPUNIT_TEST_SUITE( ParserTests );
    CPPUNIT_TEST( testSingleLine );
    CPPUNIT_TEST( testMultipleLines );
    CPPUNIT_TEST_SUITE_END();

    void testSingleLine() {
        Parser *parser = Parser::create("solid #000");
        CPPUNIT_ASSERT_STRINGS_EQUAL("solid", parser->getWord());
        CPPUNIT_ASSERT_STRINGS_EQUAL("#000", parser->getWord());
        CPPUNIT_ASSERT_STRINGS_EQUAL(PARSER_EOC, parser->getWord());
        delete parser;
    };

    void testMultipleLines() {
        Parser *parser = Parser::create("solid #000\nsolid #fff");
        CPPUNIT_ASSERT_STRINGS_EQUAL("solid", parser->getWord());
        CPPUNIT_ASSERT_STRINGS_EQUAL("#000", parser->getWord());
        CPPUNIT_ASSERT_STRINGS_EQUAL(PARSER_EOC, parser->getWord());
        CPPUNIT_ASSERT_STRINGS_EQUAL("solid", parser->getWord());
        CPPUNIT_ASSERT_STRINGS_EQUAL("#fff", parser->getWord());
        CPPUNIT_ASSERT_STRINGS_EQUAL(PARSER_EOC, parser->getWord());
        delete parser;
    };

    void testWhitespace() {
        Parser *parser = Parser::create("\nsolid #000\n\nsolid #fff\n");
        CPPUNIT_ASSERT_STRINGS_EQUAL("solid", parser->getWord());
        CPPUNIT_ASSERT_STRINGS_EQUAL("#000", parser->getWord());
        CPPUNIT_ASSERT_STRINGS_EQUAL(PARSER_EOC, parser->getWord());
        CPPUNIT_ASSERT_STRINGS_EQUAL("solid", parser->getWord());
        CPPUNIT_ASSERT_STRINGS_EQUAL("#fff", parser->getWord());
        CPPUNIT_ASSERT_STRINGS_EQUAL(PARSER_EOC, parser->getWord());
        delete parser;
    };

    void CPPUNIT_ASSERT_STRINGS_EQUAL(const char *expected, const char *actual) {
        if (expected == NULL && actual == NULL) return;
        if (expected == NULL) { CPPUNIT_FAIL("expected NULL but received value"); }
        if (actual == NULL) { CPPUNIT_FAIL("expected value but received NULL"); }
        CPPUNIT_ASSERT_EQUAL(std::string(expected), std::string(actual));
    }
};
