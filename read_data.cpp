#include "util.h"
#include <istream>
#include <tuple>
#include <ctime>
#include <algorithm>

using namespace std;

template<class CharT, class Traits>
tuple<time_t, double, double> from_stream(basic_istream<CharT, Traits>& stream) {
  const double dnan = numeric_limits<double>::quiet_NaN();
  
  time_t timestamp;
  string t_h;
  double temperature, humidity;
    
  stream >> timestamp >> t_h;
  if (t_h.empty()) {
    return make_tuple(timestamp, dnan, dnan);
  }

  // erase whitespace
  auto erase_itr = remove_if(t_h.begin(), t_h.end(), [](unsigned char c) { return isspace(c); });
  t_h.erase(erase_itr, t_h.end());
  
  auto tokens = split(t_h, ',');

  try {
    if (tokens.size() > 0)
      temperature = stod(tokens[0]);
  } catch(const exception& e) {
    temperature = dnan;
  }
  try {
    if (tokens.size() > 1)
      humidity = stod(tokens[1]);
  } catch(const exception& e) {
    humidity = dnan;
  }

  return make_tuple(timestamp, temperature, humidity);
}
