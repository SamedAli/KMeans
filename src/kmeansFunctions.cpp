#include "kmeansFunctions.h"

#include <cassert>
#include <cmath>
#include <fstream>
#include <iosfwd>
#include <iostream>
#include <limits>
#include <numeric>
#include <random>
#include <ranges>
#include <string>
#include <vector>

auto readDataPoints(const std::string &fileName, const char delimiter, const std::uint16_t colCount) -> std::vector<std::vector<double>>
{
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

auto dumpToCSV(const std::vector<std::vector<double>> &centroids, const std::string &fileName) -> void
{
	std::ofstream file_(fileName);

	if (!file_.is_open())
	{
		std::cerr << "Failed to write: " << fileName << "\n";
		return;
	}

	for (const auto &row : centroids)
	{
		for (size_t j = 0; j < row.size(); ++j)
		{
			file_ << row[j];

			if (j < row.size() - 1)
			{
				file_ << ",";
			}
		}

		file_ << "\n";
	}
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
	std::cout << "\t# Assigning Clusters\n";

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

auto calculateDistance(const std::vector<double> &datapoints, const std::vector<double> &centroids) -> double
{
	assert((datapoints.size() == centroids.size()) && "Dimensions must be the same!");

	double sum_ = 0.0;
	for (size_t i = 0; i < datapoints.size(); ++i)
	{
		sum_ += (datapoints[i] - centroids[i]) * (datapoints[i] - centroids[i]);
	}

	return std::sqrt(sum_);
}

auto recalculateCentroids(const std::vector<std::vector<double>> &dataPoints, const std::vector<std::uint16_t> &cluster, std::vector<std::vector<double>> &centroids) -> std::vector<std::vector<double>>
{
	std::cout << "\t# Recalculating centroids: \n";

	const size_t                     numCentroids_ = centroids.size();
	const size_t                     numDimension_ = centroids[0].size();
	std::vector<std::vector<double>> newCentroids_{};
	newCentroids_.resize(numCentroids_, std::vector<double>(numDimension_, 0.0));

	for (size_t i = 0; i < centroids.size(); i++) // get cluster numbers
	{
		std::vector<double> zero_(dataPoints[0].size(), 0.0);

		auto indices_ = std::views::iota(size_t{0}, cluster.size()) | std::views::filter(
		                                                                  [&cluster, i](size_t idx) {
			                                                                  return cluster[idx] == i;
		                                                                  });

		auto sum_ = std::accumulate(indices_.begin(),
		                            indices_.end(),
		                            zero_,
		                            [&dataPoints](std::vector<double> accumulate, size_t idx) {
			                            for (size_t i = 0; i < accumulate.size(); i++)
			                            {
				                            accumulate[i] += dataPoints[idx][i];
			                            }

			                            return accumulate;
		                            });

		auto size_ = static_cast<double>(std::ranges::distance(indices_));
		for (size_t idx = 0; idx < centroids[0].size(); ++idx)
		{
			newCentroids_[i][idx] = sum_[idx] / size_;
		}
	}

	return newCentroids_;
}