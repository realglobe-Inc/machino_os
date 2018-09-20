#include <istream>
#include <tuple>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
#include "ranged_values.h"


using namespace std;


vector<string> split(const string& s, char delim) {
  vector<string> elems;
  string item;
  for (char ch: s) {
    if (ch == delim) {
      if (!item.empty())
	elems.push_back(item);
      item.clear();
    }
    else {
      item += ch;
    }
  }
  if (!item.empty())
    elems.push_back(item);
  return elems;
}

string to_json(const vector<tuple<time_t, double, double>>& averaged) {
  stringstream ss;

  stringstream dtosss;
  dtosss.precision(3);
  auto mydtos = [&](double d) {
    dtosss.str(string());
    if (isnan(d)) {
      return string("null");
    }
    
    dtosss << d;
    return dtosss.str();
  };
  
  ss << "[";
  auto write_stream = [&](stringstream& ss, const tuple<time_t, double, double>& value) {
    ss << "{\"time_t\":" << get<0>(value)  << ",\"temp\":" << mydtos(get<1>(value)) << ",\"hmd\":" << mydtos(get<2>(value)) << "}";
  };
  
  if (!averaged.empty()) {
    write_stream(ss, averaged.front());
    for (auto itr = averaged.begin() + 1; itr < averaged.end(); ++itr) {
      ss << ",";
      write_stream(ss, *itr);
    }
  }

  ss << "]";
  return ss.str();
}

std::tm build_date_tm(size_t year, size_t month, size_t day) {
  std::tm t;
  std::memset(&t, 0, sizeof(std::tm));

  t.tm_year = year - 1900;
  t.tm_mon = month - 1;
  t.tm_mday = day;
  return t;
}

std::vector<std::time_t> make_stamps(const std::string& date_str, const size_t duration_secs) {
  auto tokens = split(date_str, '-');
  if (tokens.size() != 3) {
    throw std::invalid_argument("invalid date string");
  }

  auto begin_t = build_date_tm(std::stoul(tokens[0]), std::stoul(tokens[1]), std::stoul(tokens[2]));
  std::tm end_t = std::tm(begin_t);
  end_t.tm_mday = begin_t.tm_mday + 1;

  return make_stamps(begin_t, end_t);
}

std::vector<std::time_t> make_stamps(const std::tm& from, const std::tm& until, const size_t duration_secs) {
  std::tm tmp_f(from);
  std::tm tmp_u(until);
  auto begin_stamp = std::mktime(&tmp_f);
  auto end_stamp = std::mktime(&tmp_u);
  if (begin_stamp > end_stamp) {
    std::swap(begin_stamp, end_stamp);
  }

  std::vector<std::time_t> result;
  for (auto i = begin_stamp; i < end_stamp; i += duration_secs) {
    result.push_back(i);
  }
  return result;
}
