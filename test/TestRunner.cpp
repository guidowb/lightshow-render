#include <cppunit/ui/text/TestRunner.h>

#include "LexerTests.cpp"

int main( int argc, char **argv)
{
  CppUnit::TextUi::TestRunner runner;
  runner.addTest( LexerTests::suite() );
  runner.run();
  return 0;
}
