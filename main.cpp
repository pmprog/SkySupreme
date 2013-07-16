
#include "framework/framework.h"

Framework* fw;

int main ( int argc, char* argv[] )
{
  fw = new Framework();
  fw->Run();
  delete fw;

  return 0;
}
