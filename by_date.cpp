#include "util.h"
#include "ranged_values.h"
#include <utility>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <tuple>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <exception>
#include <limits>
#include <cmath>
#include <regex>


using namespace std;
auto dnan = numeric_limits<double>::quiet_NaN();
auto file_regex = regex(R"(\d+-\d+-\d+)");

int main(const int argc, const char* argv[]) {
  if (argc < 3) {
    cerr << "No input file" << endl;
    return 1;
  }

  bool valid_filename = regex_match(argv[2], file_regex);
  if (!valid_filename) {
    cerr << "invalid filename: " << argv[2] << endl;
    cout << "[]" << endl;
    return 1;
  }
  
  ifstream ifs(string(argv[1]) + "/" + argv[2]);
  
  if (ifs.fail()) {
    cout << "[]" << endl;
    return 1;
  }

  auto stamps = make_stamps(string(argv[2]));
  auto data = ranged_values(stamps);
  
  while(!ifs.eof()) {
    auto row = from_stream(ifs); // make_tuple(timestamp, temperature, humidity);
    if (ifs.eof()) {
      break;
    }

    data.insert(row);
  }

  
  const vector<tuple<time_t, double, double>> averaged = data.average();
  auto json = to_json(averaged);

  cout << json << endl;
  return 0;
}
