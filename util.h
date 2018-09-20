#ifndef ___FIELD_DATA_UTIL_H___
#define ___FIELD_DATA_UTIL_H___

#include <istream>
#include <tuple>
#include <ctime>
#include <algorithm>
#include <vector>

template<typename T>
T l1(T lhs, T rhs) {
  return std::max(lhs, rhs) - std::min(lhs, rhs);
}

std::tm build_date_tm(size_t year, size_t month, size_t day);

std::vector<std::time_t> make_stamps(const std::string& date_str, const size_t duration_secs = 60);
std::vector<std::time_t> make_stamps(const std::tm& from, const std::tm& until, const size_t duration_secs = 60);

std::vector<std::string> split(const std::string& s, char delim);
template<class CharT, class Traits>
  std::tuple<std::time_t, double, double> from_stream(std::basic_istream<CharT, Traits>& stream) {
  using namespace std;
  
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

std::string to_json(const std::vector<std::tuple<time_t, double, double>>& averaged);

class ranged_values;
std::vector<std::tuple<std::time_t, double, double>> average_ranges(const std::vector<ranged_values>& ranges);

#endif
