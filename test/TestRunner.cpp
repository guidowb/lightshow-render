#include <cppunit/ui/text/TestRunner.h>

#include "LexerTests.cpp"
#include "ParserTests.cpp"
#include "CompilerTests.cpp"

long millis() {
  return 0;
}

int main( int argc, char **argv)
{
  CppUnit::TextUi::TestRunner runner;
  runner.addTest( LexerTests::suite() );
  runner.addTest( ParserTests::suite() );
  runner.addTest( CompilerTests::suite() );
  runner.run();
  return 0;
}
