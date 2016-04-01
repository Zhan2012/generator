#include <iostream>
#include <fstream>
#include <random>
#include <ctime>
#include <stdint.h>
#include "options.h"

int main (int argc, char* argv[]) {
  using namespace std;
  using namespace opt;

  if (chkOption(argv, argv + argc, "-h")) {
    cout
      << "rmat [options]" << endl
      << " -h:\t ask for help" << endl
      << " -s:\t scale,  default: 8" << endl
      << " -d:\t degree, default: 8" << endl
      << " -r:\t srand,  default: current time" << endl
      << " -o:\t output, default: console" << endl;
    return 0;
  }

  int scale  = getValue(argv, argv + argc, "-s", 8);
  int degree = getValue(argv, argv + argc, "-d", 8);
  int seed   = getValue(argv, argv + argc, "-r", time(NULL));
  char* ofn  = getOption(argv, argv + argc, "-o");

  mt19937_64 gen(seed);
  uniform_int_distribution<uint64_t> ud(0, 0xffffffffffffffff);

  uint64_t r, u, v;
  uint8_t b4 = 0, b4_per_rand = 16, nb4 = 0;

  if (ofn) {
    ofstream ofile(ofn, ios::binary);
    for (uint64_t e = (1 << scale) * degree; e > 0; e--) {
      u = 0, v = 0;
      for (int i = 0; i < scale; i++) {
        if (nb4 == 0) { r = ud(gen); nb4 = b4_per_rand; }
        b4 = r & 0xf;
        u <<= 1; v <<= 1;
        if (b4 == 0) { u++; v++; }
        else if (b4 <= 3) u++;
        else if (b4 <= 6) v++;
        r >>= 4; nb4--;
      }
      ofile.write((char *)&u, sizeof(uint64_t));
      ofile.write((char *)&v, sizeof(uint64_t));
    }
    ofile.close();
  } else {
    for (uint64_t e = (1 << scale) * degree; e > 0; e--) {
      u = 0, v = 0;
      for (int i = 0; i < scale; i++) {
        if (nb4 == 0) { r = ud(gen); nb4 = b4_per_rand; }
        b4 = r & 0xf;
        u <<= 1; v <<= 1;
        if (b4 == 0) { u++; v++; }
        else if (b4 <= 3) u++;
        else if (b4 <= 6) v++;
        r >>= 4; nb4--;
      }
      cout << u << " " << v << endl;
    }
  }

  return 0;
}
