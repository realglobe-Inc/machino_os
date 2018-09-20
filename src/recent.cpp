#include "util.h"
#include "ranged_values.h"
#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <chrono>
#include <ctime>
#include <cstring>

using namespace std;

int main(const int argc, const char* argv[]) {
  if (argc < 3) {
    cerr << "require 2 timestamps" << endl;
    return 1;
  }

  time_t from_timestamp, until_timestamp;
  try {
    from_timestamp = stoull(argv[1]);
    until_timestamp = stoull(argv[2]);
  } catch(const exception& e) {
    cerr << "Invalid timestamp" << endl;
    return 1;
  }
  
  if (from_timestamp > until_timestamp) {
    std::swap(from_timestamp, until_timestamp);
  }

  auto align_tm = [](const std::tm* pt) {
    if (pt == nullptr) {
      throw std::invalid_argument("nullptr");
    }
    
    auto t = *pt;
    return std::tm(t);
  };
  
  auto from_tm = align_tm(std::localtime(&from_timestamp));
  from_tm.tm_sec = 0;
  
  auto until_tm = align_tm(std::localtime(&until_timestamp));
  auto timestamps = make_stamps(from_tm, until_tm);

  
  auto data = ranged_values(timestamps);
  while(!cin.eof()) {
    auto row = from_stream(cin); // make_tuple(timestamp, temperature, humidity);
    if (cin.eof()) {
      break;
    }

    data.insert(row);
  }
  
  const vector<tuple<time_t, double, double>> averaged = data.average();
  auto json = to_json(averaged);

  cout << json << endl;
  
  return 0;
}
