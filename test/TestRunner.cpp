#include <cppunit/ui/text/TestRunner.h>

#include "LexerTests.cpp"
#include "ParserTests.cpp"

int main( int argc, char **argv)
{
  CppUnit::TextUi::TestRunner runner;
  runner.addTest( LexerTests::suite() );
  runner.addTest( ParserTests::suite() );
  runner.run();
  return 0;
}
