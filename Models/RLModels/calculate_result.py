import numpy as np
from typing import List, Dict, Union, Tuple

def calculate_geometric_mean(baseline_values: List[float], 
                            improved_values: List[float],
                            higher_is_better: bool = False) -> Tuple[float, List[float]]:
    """
    Calculate the geometric mean of improvement ratios between baseline and improved values.
    
    Args:
        baseline_values: List of baseline performance metrics
        improved_values: List of improved performance metrics
        higher_is_better: If True, improvement is improved/baseline, otherwise baseline/improved
                         (Default: False, assuming lower values are better, e.g., latency or area)
    
    Returns:
        Tuple containing:
        - geometric_mean: The geometric mean of all improvement ratios
        - improvement_ratios: List of individual improvement ratios
    """
    if len(baseline_values) != len(improved_values):
        raise ValueError("Baseline and improved value lists must have the same length")
    
    if len(baseline_values) == 0:
        raise ValueError("Input lists cannot be empty")
    
    # Calculate improvement ratios
    improvement_ratios = []
    for baseline, improved in zip(baseline_values, improved_values):
        if baseline <= 0 or improved <= 0:
            raise ValueError("All values must be positive for geometric mean calculation")
        
        # Calculate ratio based on whether higher or lower values are better
        if higher_is_better:
            ratio = improved / baseline  # Higher values are better
        else:
            ratio = baseline / improved  # Lower values are better
            
        improvement_ratios.append(ratio)
    
    # Calculate geometric mean
    geometric_mean = np.exp(np.mean(np.log(improvement_ratios)))
    
    return geometric_mean, improvement_ratios

# Example usage
if __name__ == "__main__":
    # Latency improvement (lower is better)
    baseline_latencies = [66, 231, 259, 631, 1590, 57522]
    # O3
    # improved_latencies = [64, 150, 227, 98, 1479, 5843]
    # DAPO
    improved_latencies = [49, 131, 194, 66, 792, 4041]
    geo_mean, individual_improvements = calculate_geometric_mean(
        baseline_latencies, improved_latencies, higher_is_better=False
    )
    
    print(f"Individual improvement ratios: {individual_improvements}")
    print(f"Geometric mean of improvements: {geo_mean:.4f}x")
    
    # LUT Usage
    baseline_throughputs = [8.3, 1.49, 0.39, 0.67, 4.49, 11.29]
    # O3
    # improved_throughputs = [35, 3.79, 0.58, 6.93, 5.42, 1.35]
    # DAPO
    improved_throughputs = [18, 2.5, 0.54, 4.18, 5.82, 11.09]
    
    geo_mean_throughput, individual_throughput_improvements = calculate_geometric_mean(
        baseline_throughputs, improved_throughputs, higher_is_better=True
    )
    
    print(f"\nIndividual throughput improvement ratios: {individual_throughput_improvements}")
    print(f"Geometric mean of throughput improvements: {geo_mean_throughput:.4f}x")
    
    # DSP Usage
    baseline_throughputs = [47, 15, 23, 6]
    # O3
    # improved_throughputs = [120, 80, 60, 50]
    # DAPO
    improved_throughputs = [96, 6, 60, 45]
    
    geo_mean_throughput, individual_throughput_improvements = calculate_geometric_mean(
        baseline_throughputs, improved_throughputs, higher_is_better=True
    )
    
    print(f"\nIndividual throughput improvement ratios: {individual_throughput_improvements}")
    print(f"Geometric mean of throughput improvements: {geo_mean_throughput:.4f}x")