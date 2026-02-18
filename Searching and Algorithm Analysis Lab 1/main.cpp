// ######################################################################
// # Purpose: Generate dataset and compare linear and binary search     #
// # output with clear educational reporting and safe generation guards #
// ######################################################################

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <random>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <vector>

#include "Searcher.hpp"

using std::cout;
using std::left;
using std::mt19937;
using std::numeric_limits;
using std::random_device;
using std::runtime_error;
using std::setw;
using std::size_t;
using std::sort;
using std::string;
using std::to_string;
using std::uniform_int_distribution;
using std::unordered_set;
using std::vector;

// layout constants for clean table output
static constexpr const char *HORIZONTAL_RULE =
    "-------------------------------------------------------------------------";
static constexpr int REPORT_COLUMN_WIDTH = 37;            // width for each report column
static constexpr const char *REPORT_COLUMN_GAP = "     "; // gap between columns

// result from one search test
struct SearchResult {
  bool found = false;        // true if value exists
  ptrdiff_t index = -1;      // position or -1
  size_t comparisons = 0;    // number of key checks
};

// holds both search results for one case
struct ScenarioSummary {
  string label;        // scenario name
  int target = 0;      // number searched for
  SearchResult linear; // linear search result
  SearchResult binary; // binary search result
};

// function declarations
[[nodiscard]] vector<int> GenerateDataset(size_t minimum_unique);
[[nodiscard]] SearchResult MeasureLinearSearch(const vector<int> &data, int target);
[[nodiscard]] SearchResult MeasureBinarySearch(const vector<int> &data, int target);
[[nodiscard]] ScenarioSummary SummarizeScenario(const vector<int> &shared_data, const string &label, int target);
[[nodiscard]] string FormatIndex(bool found, ptrdiff_t index);
[[nodiscard]] int ChooseMissingTarget(const vector<int> &sorted_data);
void PrintHeader(size_t count);
void PrintScenarioBlock(const string &label, int target, const SearchResult &linear_result,
                        const SearchResult &binary_result);

int main() {
  // pick a large set so comparison differences are obvious
  const size_t minimum_unique_values = 250'000;

  // generate one sorted dataset used by both algorithms
  vector<int> sorted_data;
  try {
    sorted_data = GenerateDataset(minimum_unique_values);
  } catch (const runtime_error &error) {
    cout << "Dataset generation failed: " << error.what() << '\n';
    return 1;
  }

  if (sorted_data.empty()) {
    cout << "Dataset generation failed: no values generated\n";
    return 1;
  }

  // choose four scenario targets
  const int target_begin = sorted_data.front();                  // first value
  const int target_middle = sorted_data[sorted_data.size() / 2]; // middle value
  const int target_end = sorted_data.back();                     // last value

  int target_absent = 0; // guaranteed missing value
  try {
    target_absent = ChooseMissingTarget(sorted_data);
  } catch (const runtime_error &error) {
    cout << "Target selection failed: " << error.what() << '\n';
    return 1;
  }

  // print dataset header info
  PrintHeader(sorted_data.size());

  // beginning scenario
  ScenarioSummary begin_summary = SummarizeScenario(sorted_data, "Beginning", target_begin);
  PrintScenarioBlock(begin_summary.label, begin_summary.target, begin_summary.linear, begin_summary.binary);

  // middle scenario
  ScenarioSummary middle_summary = SummarizeScenario(sorted_data, "Middle", target_middle);
  PrintScenarioBlock(middle_summary.label, middle_summary.target, middle_summary.linear,
                     middle_summary.binary);

  // end scenario
  ScenarioSummary end_summary = SummarizeScenario(sorted_data, "End", target_end);
  PrintScenarioBlock(end_summary.label, end_summary.target, end_summary.linear, end_summary.binary);

  // not present scenario
  ScenarioSummary missing_summary = SummarizeScenario(sorted_data, "Not Present", target_absent);
  PrintScenarioBlock(missing_summary.label, missing_summary.target, missing_summary.linear,
                     missing_summary.binary);

  return 0;
}

// create a sorted dataset with a safe uniqueness target
[[nodiscard]] vector<int> GenerateDataset(size_t minimum_unique) {
  if (minimum_unique == 0) {
    return {}; // nothing requested
  }

  // int range [0 .. INT_MAX] gives this many unique values
  const size_t max_distinct_values = static_cast<size_t>(numeric_limits<int>::max()) + 1U;
  if (minimum_unique > max_distinct_values) {
    throw runtime_error("requested unique count exceeds available int value space");
  }

  // scale random range up for larger requests to reduce collision churn
  const size_t default_range_size = 1'000'001U;
  size_t target_range_size = default_range_size;
  if (minimum_unique <= max_distinct_values / 4U) {
    const size_t scaled_range_size = minimum_unique * 4U;
    if (scaled_range_size > target_range_size) {
      target_range_size = scaled_range_size;
    }
  } else {
    target_range_size = max_distinct_values;
  }

  if (target_range_size > max_distinct_values) {
    target_range_size = max_distinct_values;
  }
  const int max_random_value = static_cast<int>(target_range_size - 1U);

  // reserve to cut rehash churn
  size_t reserve_count = minimum_unique;
  if (minimum_unique <= numeric_limits<size_t>::max() / 2U) {
    reserve_count = minimum_unique * 2U;
  }

  unordered_set<int> generated_values;
  generated_values.reserve(reserve_count);

  // deterministic shape is not required here, random_device is fine
  random_device seed_source;
  mt19937 value_generator(seed_source());
  uniform_int_distribution<int> value_picker(0, max_random_value);

  // guard retries so generation cannot run forever on extreme settings
  size_t max_attempts = numeric_limits<size_t>::max();
  if (minimum_unique <= numeric_limits<size_t>::max() / 40U) {
    max_attempts = minimum_unique * 40U;
  }

  size_t attempts = 0;
  while (generated_values.size() < minimum_unique) {
    if (attempts >= max_attempts) {
      throw runtime_error("dataset generation exceeded retry budget");
    }
    generated_values.insert(value_picker(value_generator));
    attempts++;
  }

  // move into vector then sort ascending
  vector<int> sorted_values;
  sorted_values.reserve(generated_values.size());
  for (int value : generated_values) {
    sorted_values.push_back(value);
  }
  sort(sorted_values.begin(), sorted_values.end());
  return sorted_values;
}

// pick a value guaranteed not to exist in sorted_data
[[nodiscard]] int ChooseMissingTarget(const vector<int> &sorted_data) {
  if (sorted_data.empty()) {
    throw runtime_error("cannot select missing target from empty dataset");
  }

  const int first_value = sorted_data.front();
  const int last_value = sorted_data.back();

  if (first_value > numeric_limits<int>::min()) {
    return first_value - 1; // below smallest value
  }
  if (last_value < numeric_limits<int>::max()) {
    return last_value + 1; // above largest value
  }

  throw runtime_error("cannot guarantee missing target for full int domain");
}

// run linear search against provided data
[[nodiscard]] SearchResult MeasureLinearSearch(const vector<int> &data, int target) {
  const SearchOutcome result = LinearSearch(data, target);
  return {result.index != -1, result.index, result.comparisons};
}

// run binary search against sorted data
[[nodiscard]] SearchResult MeasureBinarySearch(const vector<int> &data, int target) {
  const SearchOutcome result = BinarySearch(data, target);
  return {result.index != -1, result.index, result.comparisons};
}

// run both searches for one scenario
[[nodiscard]] ScenarioSummary SummarizeScenario(const vector<int> &shared_data, const string &label, int target) {
  const SearchResult linear_result = MeasureLinearSearch(shared_data, target);
  const SearchResult binary_result = MeasureBinarySearch(shared_data, target);
  return {label, target, linear_result, binary_result};
}

// print dataset info at top
void PrintHeader(size_t count) {
  cout << "Dataset: " << count << " unique integers\n";
  cout << HORIZONTAL_RULE << "\n\n";
}

// print results for one scenario
void PrintScenarioBlock(const string &label, int target, const SearchResult &linear_result,
                        const SearchResult &binary_result) {
  cout << left;
  cout << "Scenario -> " << setw(22) << label << "Target -> " << target << "\n";
  cout << HORIZONTAL_RULE << "\n";

  // column titles
  cout << setw(REPORT_COLUMN_WIDTH) << "Linear Search (sorted data)" << REPORT_COLUMN_GAP
       << setw(REPORT_COLUMN_WIDTH) << "Binary Search (sorted data)" << "\n";
  cout << HORIZONTAL_RULE << "\n";

  // helper to print paired rows
  const auto row = [](const string &first_text, const string &second_text) {
    cout << setw(REPORT_COLUMN_WIDTH) << first_text << REPORT_COLUMN_GAP
         << setw(REPORT_COLUMN_WIDTH) << second_text << "\n";
  };

  // found/not found row
  const string linear_found = linear_result.found ? "Found: Yes" : "Found: No";
  const string binary_found = binary_result.found ? "Found: Yes" : "Found: No";
  row(linear_found, binary_found);

  // index row
  row("Index: " + FormatIndex(linear_result.found, linear_result.index) + " (sorted)",
      "Index: " + FormatIndex(binary_result.found, binary_result.index) + " (sorted)");

  // comparison row
  row("Comparisons: " + to_string(linear_result.comparisons),
      "Comparisons: " + to_string(binary_result.comparisons));

  cout << HORIZONTAL_RULE << "\n\n";
}

// format index text for report rows
[[nodiscard]] string FormatIndex(bool found, ptrdiff_t index) {
  if (!found) {
    return "N/A";
  }
  return to_string(index);
}
