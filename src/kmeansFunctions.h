#include <string>
#include <vector>

namespace
{
	constexpr std::uint16_t SEED = 13;
}

auto readDataPoints(const std::string &fileName, const char delimiter, const std::uint16_t colCount = 3) -> std::vector<std::vector<double>>;
auto dumpToCSV(const std::vector<std::vector<double>> &centroids, const std::string &fileName) -> void;

auto initializeCentroids(const std::vector<std::vector<double>> &dataPoints, const std::uint16_t nofClusters, const std::uint16_t seed = SEED) -> std::vector<std::vector<double>>;
auto assignClusters(const std::vector<std::vector<double>> &dataPoints, const std::vector<std::vector<double>> &centroids, std::vector<std::uint16_t> &clusters) -> void;
auto calculateDistance(const std::vector<double> &datapoints, const std::vector<double> &centroids) -> double;
auto recalculateCentroids(const std::vector<std::vector<double>> &dataPoints, const std::vector<std::uint16_t> &cluster, std::vector<std::vector<double>> &centroids) -> void;