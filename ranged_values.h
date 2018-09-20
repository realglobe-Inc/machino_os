#ifndef ___FIELD_DATA_RANGED_VALUES_H___
#define  ___FIELD_DATA_RANGED_VALUES_H___

#include <vector>
#include <tuple>
#include <ctime>
#include <cmath>
#include <stdexcept>
#include <limits>
#include <unordered_map>
#include <algorithm>
#include <utility>
#include "util.h"

class ranged_values {
  typedef std::tuple<std::time_t, double, double> data_type;
  typedef std::vector<data_type> container_type;
  typedef std::unordered_map<std::time_t, container_type>::const_iterator const_iterator;
  std::unordered_map<std::time_t, container_type> data;

public:
  ranged_values(const std::vector<std::time_t>& stamps) {
    for (auto item : stamps) {
      this->data.insert(std::make_pair(item, container_type()));
    }
  };

  void insert(const data_type& item) {
    auto stamp = std::get<0>(item);
    auto range = find_insert_range(stamp);

    for (auto& itr : range) {
      this->data[itr->first].push_back(item);
    }
  };

  std::vector<const_iterator> find_insert_range(const std::time_t k) {
    std::vector<const_iterator> result;
    if (this->data.empty()) {
      return result;
    }
    
    for (const_iterator itr = this->data.begin(); itr != this->data.end(); ++itr) {
      if (l1(itr->first, k) < 60) {
        result.push_back(itr);
      }
    }

    return result;
  };

  std::vector<data_type> average() {
    auto ave = [](double sum, size_t count) {
      if (count == 0) {
	      return std::numeric_limits<double>::quiet_NaN();
      }
      return sum / count;
    };

    std::vector<data_type> result;
    for (auto p : this->data) {
      double sum_t = 0, sum_h = 0;
      size_t count_t = 0, count_h = 0;
      
      for (auto item : p.second) {
        auto temperature = std::get<1>(item);
        auto humidity = std::get<2>(item);
        if (!isnan(temperature)) {
          sum_t += temperature;
          count_t++;
        }
        if (!isnan(humidity)) {
          sum_h += humidity;
          count_h++;
        }
      }
      
      sum_t = ave(sum_t, count_t);
      sum_h = ave(sum_h, count_h);
      result.push_back(std::make_tuple(p.first, sum_t, sum_h));
    }

    auto sort_func = [](const decltype(result)::value_type lhs, const decltype(result)::value_type rhs) {
      return std::get<0>(lhs) < std::get<0>(rhs);
    };
    
    std::sort(result.begin(), result.end(), sort_func);
    return result;
  };
};

#endif
