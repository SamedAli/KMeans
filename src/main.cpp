#include "kmeansFunctions.h"

#include <filesystem>
#include <iostream>

/*
1 Decide how many clusters you want, i.e. choose k
2 Randomly assign a centroid to each of the k clusters
3 Calculate the distance of all observation to each of the k centroids
4 Assign observations to the closest centroid
5 Find the new location of the centroid by taking the mean of all the observations in each cluster
6 Repeat steps 3-5 until the centroids do not change position
*/
auto main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) -> int
{
	const std::uint16_t nofClusters   = 6;
	const std::uint16_t nofIterations = 10'000;

	std::vector<std::uint16_t> dataPointsClusterNumber;
	const std::string          filename_  = std::filesystem::path("assets") / "data2.csv";
	const char                 delimiter_ = ',';

	const auto dataPoints_ = readDataPoints(filename_, delimiter_);
	auto       centroids_  = initializeCentroids(dataPoints_, nofClusters);

	dataPointsClusterNumber.resize(dataPoints_.size());

	for (std::size_t i = 0; i < nofIterations; ++i)
	{
		bool centroidsChanged_ = false;
		std::cout << "# Iteration: " << i << "\n";

		assignClusters(dataPoints_, centroids_, dataPointsClusterNumber);
		auto newCentroids_ = recalculateCentroids(dataPoints_, dataPointsClusterNumber, centroids_);

		if (!(centroids_ == newCentroids_))
		{
			centroids_        = std::move(newCentroids_);
			centroidsChanged_ = true;
		}

		if (!centroidsChanged_)
			break;
	}

	std::cout << "# Writing to file.\n";
	dumpToCSV(centroids_, std::filesystem::path("assets") / "centroids.csv");
	std::cout << "# Done.\n";
}