import csv
import numpy as np

def generate_clustered_data_csv(n: int, d: int, n_clusters: int, filepath: str):
    """
    Generates a CSV file with n datapoints and d dimensions.
    The datapoints are clustered into n_clusters clusters with random noise.
    
    Parameters:
        n (int): Total number of datapoints (rows).
        d (int): Number of dimensions (columns) for each datapoint.
        n_clusters (int): Number of clusters to generate.
        filepath (str): File path for the CSV file to be created.
    """
    # Determine the number of points per cluster (distribute remainder among first clusters)
    points_per_cluster = n // n_clusters
    remainder = n % n_clusters
    
    clusters = []
    
    for i in range(n_clusters):
        # Randomly set a cluster center in the range [-10, 10] for each dimension.
        center = np.random.uniform(-10, 10, d)
        
        # Determine how many points to generate for this cluster
        num_points = points_per_cluster + (1 if i < remainder else 0)
        
        # Generate points around the center using a normal distribution (sigma = 1.0)
        points = np.random.normal(loc=center, scale=1.0, size=(num_points, d))
        clusters.append(points)
    
    # Combine all clusters into one dataset
    data = np.vstack(clusters)
    
    # Shuffle the rows so clusters are not in order
    np.random.shuffle(data)
    
    # Write the data to a CSV file
    with open(filepath, "w", newline="") as csvfile:
        writer = csv.writer(csvfile)
        for row in data:
            writer.writerow(row)
    
    print(f"CSV file with {n} datapoints and {d} dimensions has been created at: {filepath}")

# Example usage:
if __name__ == "__main__":
    # Generate a CSV with 300 datapoints, 2 dimensions, divided into 3 clusters.
    generate_clustered_data_csv(n=10000, d=3, n_clusters=10, filepath="../assets/data2.csv")
