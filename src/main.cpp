#include "kmeansFunctions.h"

#include <chrono>
#include <filesystem>
#include <iostream>

auto kmeansIteration(const std::uint16_t nofIterations, const std::vector<std::vector<double>> &datapoints, std::vector<std::vector<double>> &centroids, std::vector<std::uint16_t> &clusters) -> void
{
	auto start = std::chrono::high_resolution_clock::now();

	for (std::size_t i = 0; i < nofIterations; ++i)
	{
		bool centroidsChanged_ = false;
		std::cout << "# Iteration: " << i << "\n";

		assignClusters(datapoints, centroids, clusters);
		auto newCentroids_ = recalculateCentroids(datapoints, clusters, centroids);

		if (!(centroids == newCentroids_))
		{
			centroids         = std::move(newCentroids_);
			centroidsChanged_ = true;
		}

		if (!centroidsChanged_)
			break;
	}

	auto                          end      = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = end - start;
	std::cout << "# Execution time: " << duration.count() << " seconds\n";
}

auto main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) -> int
{
	const std::uint16_t nofClusters_   = 6;
	const std::uint16_t nofIterations_ = 10'000;

	std::vector<std::uint16_t> dataPointsClusterNumber_;
	const std::string          filename_  = std::filesystem::path("assets") / "data2.csv";
	const char                 delimiter_ = ',';

	const auto dataPoints_ = readDataPoints(filename_, delimiter_);
	auto       centroids_  = initializeCentroids(dataPoints_, nofClusters_);
	dataPointsClusterNumber_.resize(dataPoints_.size());

	kmeansIteration(nofIterations_, dataPoints_, centroids_, dataPointsClusterNumber_);

	std::cout << "# Writing to file.\n";
	dumpToCSV(centroids_, std::filesystem::path("assets") / "centroids.csv");
	std::cout << "# Done.\n";
}