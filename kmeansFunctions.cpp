#include "kmeansFunctions.h"

#include <cassert>
#include <cmath>
#include <fstream>
#include <iosfwd>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <vector>

#include <filesystem>

auto readDataPoints(const std::string &fileName, const char delimiter, const std::uint16_t colCount) -> std::vector<std::vector<double>>
{
	// std::filesystem::current_path(std::filesystem::current_path().parent_path());
	std::cout << "Current working directory: " << std::filesystem::current_path() << "\n";

	std::vector<std::vector<double>> data_;
	std::ifstream                    file{fileName};

	if (file.is_open())
	{
		std::string line;

		while (std::getline(file, line))
		{
			std::vector<double> row_;
			row_.reserve(colCount);

			size_t position_ = 0;
			size_t start_pos = 0;

			while ((position_ = line.find(delimiter, start_pos)) != std::string::npos)
			{
				row_.emplace_back(std::stold(line.substr(start_pos, position_ - start_pos)));
				start_pos = position_ + 1;
			}

			// Handle last value after the last delimiter
			if (start_pos < line.length())
				row_.emplace_back(std::stold(line.substr(start_pos)));

			if (!row_.empty())
				data_.emplace_back(std::move(row_));
		}
	}
	else
	{
		std::cerr << "Failed to open file: " << fileName << "\n";
	}

	return data_;
}

auto initializeCentroids(const std::vector<std::vector<double>> &dataPoints, const std::uint16_t nofClusters, const std::uint16_t seed) -> std::vector<std::vector<double>>
{
	std::mt19937 randomGenerator_{seed};

	std::uniform_int_distribution<size_t> distribution_{1, dataPoints.size()};
	std::vector<std::vector<double>>      centroids_;
	centroids_.resize(nofClusters);

	for (size_t i = 0; i < nofClusters; ++i)
	{
		auto randomDatapoint_ = distribution_(randomGenerator_);
		centroids_[i]         = dataPoints[randomDatapoint_];
	}

	return centroids_;
}

auto assignClusters(const std::vector<std::vector<double>> &dataPoints, const std::vector<std::vector<double>> &centroids, std::vector<std::uint16_t> &clusters) -> void
{
	for (size_t i = 0; i < dataPoints.size(); ++i)
	{
		double        minDistance_ = std::numeric_limits<double>::max();
		std::uint16_t bestCluster_ = std::numeric_limits<std::uint16_t>::max();

		for (size_t j = 0; j < centroids.size(); ++j)
		{
			auto distance_ = calculateDistance(dataPoints[i], centroids[j]);

			if (minDistance_ > distance_)
			{
				minDistance_ = distance_;
				bestCluster_ = j;
			}
		}

		clusters[i] = bestCluster_;
	}
}

auto calculateDistance(const std::vector<double> &datapoints, const std::vector<double> &cluster) -> double
{
	assert((datapoints.size() == cluster.size()) && "Vectors must have the same dimensions!");

	double sum_ = 0.0;
	for (size_t i = 0; i < datapoints.size(); ++i)
	{
		sum_ += (datapoints[i] - cluster[i]) * (datapoints[i] - cluster[i]);
	}

	return std::sqrt(sum_);
}

auto recalculateCentroids([[maybe_unused]] const std::vector<std::vector<double>> &dataPoints, [[maybe_unused]] const std::vector<double> &cluster, [[maybe_unused]] std::vector<std::vector<double>> &centroids) -> double
{
	return 0;
}