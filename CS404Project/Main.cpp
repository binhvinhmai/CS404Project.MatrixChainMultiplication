#include <iostream>
#include <iomanip>
#include <cstdlib> 
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

// Naming Conventions TitleCase = function, camelCase = variable

vector<vector<int>> CreateMatrices(vector<int> dimensions) {

	vector<vector<int>> matrixTable;

	matrixTable.resize(dimensions.size());
	// Go through every row
	for (size_t i = 0; i < dimensions.size(); i++)
	{
		// Resize each column
		matrixTable[i].resize(dimensions.size());
		// For each cell
		for (size_t j = 0; j < dimensions.size(); j++)
		{
			matrixTable[i][j] = 0;
		}
	}
	return matrixTable;
}


// Optimal Solution for Matrix Multiplication
int OptimalSolution(vector<int> dimensions, vector<vector<int>>& matrixCosts, int i, int j) {
	int cost;
	int cost1, cost2, cost3;
	if (matrixCosts[i][j] != 0) { // 0 is a placeholder value
		return matrixCosts[i][j];
	}
	else if (i == j) {
		return 0;
	}
	else {
		// TODO: This is not working in the expected way. Take not in programming. 
		matrixCosts[i][j] = numeric_limits<int>::max(); // Assign infinity to the matrix set
		for (int k = i; k < j; k++) {
			cost1 = OptimalSolution(dimensions, matrixCosts, i, k);
			cost2 = OptimalSolution(dimensions, matrixCosts, k + 1, j);
			cost3 = dimensions[i - 1] * dimensions[k] * dimensions[j];
			cost = cost1 + cost2 + cost3;
			if (cost < matrixCosts[i][j]) {
				matrixCosts[i][j] = cost;
			}

		}
		return matrixCosts[i][j];
	}
}

// Wrapper Function for Optimal Solution for Matrix Chain Multiplication
int MatrixChain(vector<int> dimensions, vector<vector<int>>& matrixCosts) {
	int j;
	for (size_t L = 1; L < dimensions.size(); L++) { // L is the length of the chain of matrices that are being examined
		for (size_t i = 1; i < dimensions.size() - L + 1; i++) {
			j = i + L - 1;
			OptimalSolution(dimensions, matrixCosts, i, j);
		}
	}
	return matrixCosts[1][dimensions.size()-1];

}

// The below function is referred to as HeuristicA in the project statement and in the project report. 
// I have renamed it to heuristicLargestFirst to to adhere to naming practice
int HeuristicLargestFirst(vector<int> dimensions) {
	// Function takes in a vector and repeatedly proceeds to find the largest dimension, find the cost associated with it, and then remove it
	int cost = 0;
	int largestValue = 0, index = 0;
	while (dimensions.size() > 2) {
		// Find the largest value within the array
		largestValue = 0;
		index = 0;
		for (size_t i = 1; i < dimensions.size() - 1; i++) {
			if (dimensions[i] > largestValue) {
				largestValue = dimensions[i];
				index = i;
			}
		}
		// Add to cost
		cost += dimensions[index - 1] * dimensions[index] * dimensions[index + 1];
		// Remove from dimensions
		dimensions.erase(dimensions.begin() + index);
	}
	return cost;
}

// The below function is referred to as HeuristicB in the project statement and in the project report. 
// I have renamed it to heuristicMostExpensive to adhere to naming practices
int HeuristicMostExpensive(vector<int> dimensions) {
	// Function takes in a vector and repeatedly proceeds to find the most expensive matrix multiplication, adds it to the cost, and then removes it before repeating
	int cost = 0;
	int currentProduct = 0, greatestProduct = 0, index = 0;
	while (dimensions.size() > 2) {
		// Find the largest value within the array
		currentProduct = 0;
		greatestProduct = 0;
		index = 0;
		for (size_t i = 1; i < dimensions.size() - 1; i++) {
			currentProduct = dimensions[i - 1] * dimensions[i] * dimensions[i + 1];
			if (currentProduct > greatestProduct) {
				greatestProduct = currentProduct;
				index = i;
			}
		}
		// Add to cost
		cost += greatestProduct;
		// Remove from dimensions
		dimensions.erase(dimensions.begin() + index);
	}
	return cost;
}

// The below function is referred to as HeuristicC in the project statement and in the project report. 
// I have renamed it to heuristicSmallestFirst to adhere to naming practices
int HeuristicSmallestFirst(vector<int> dimensions) {
	// Function takes in a vector and repeatedly proceeds to find the smallest dimension, find the cost associated with it, and then remove it
	int cost = 0;
	int smallestValue = 0, index = 0;
	while (dimensions.size() > 2) {
		// Find the smallest value within the array
		smallestValue = dimensions[1]; //Initialize to first value
		index = 1;
		for (size_t i = 1; i < dimensions.size() - 1; i++) {
			if (dimensions[i] < smallestValue) {
				smallestValue = dimensions[i];
				index = i;
			}
		}
		// Add to cost
		cost += dimensions[index - 1] * dimensions[index] * dimensions[index + 1];
		// Remove from dimensions
		dimensions.erase(dimensions.begin() + index);
	}
	return cost;
}

// The below function is referred to as HeuristicD in the project statement and in the project report. 
// I have renamed it to heuristicLeastExpensive to adhere to naming practice
int HeuristicLeastExpensive(vector<int> dimensions) {
	// Function takes in a vector and repeatedly proceeds to find the least expensive matrix multiplication, adds it to the cost, and then removes it before repeating
	int cost = 0;
	int currentProduct = 0, smallestProduct = 0, index = 0;
	while (dimensions.size() > 2) {
		// Find the smallest value within the array
		smallestProduct = dimensions[0] * dimensions[1] * dimensions[2];
		index = 1;
		for (size_t i = 1; i < dimensions.size() - 1; i++) {
			currentProduct = dimensions[i - 1] * dimensions[i] * dimensions[i + 1];
			if (currentProduct < smallestProduct) {
				smallestProduct = currentProduct;
				index = i;
			}
		}
		// Add to cost
		cost += smallestProduct;
		// Remove from dimensions
		dimensions.erase(dimensions.begin() + index);
	}
	return cost;
}

// The below function is referred to as HeuristicF in the project statement and in the project report. 
// I have renamed it to heuristicIgnorantMethod to adhere to naming practices
int HeuristicIgnorantMethod(vector<int> dimensions) {
	// Function takes in a vector and proceeds to multiply it straight across
	int cost = dimensions[0]; //Get the value of the first index
	for (size_t i = 1; i < dimensions.size() - 1; i++) {
		cost *= dimensions[i] * dimensions[i + 1]; // Multiply previous matrix with current one
	}
	return cost;
}

int main() {
	// Test case used to evaluate that my algorithms were correct
	// Tested these against my own hand written analysis
	/*
	vector<int> testing, testingA, testingB, testingC, testingD, testingF;
	testing.push_back(3);
	testing.push_back(5);
	testing.push_back(7);
	testing.push_back(8);
	testing.push_back(9);
	vector<vector<int>> testingTable = CreateMatrices(testing);
	testingA = testingB = testingC = testingD = testingF = testing;
	cout << MatrixChain(testing, testingTable) << endl;
	cout << HeuristicLargestFirst(testingA) << endl;
	cout << HeuristicMostExpensive(testingB) << endl;
	cout << HeuristicSmallestFirst(testingC) << endl;
	cout << HeuristicLeastExpensive(testingD) << endl;
	cout << HeuristicIgnorantMethod(testingF) << endl;
	system("pause");
	*/
	
	// As given in the project algorithm: Range for the low and high value for the random matrices' dimensions
	int lowValue = 7, highValue = 17;
	int optimal;
	// Array to store costs, and the cost is the corresponding matrix divided by the optimal solution
	// Made to be size 30 as that is the number of vectors to test in the Heuristic Experiment
	// The following letters match up:
	//		A - Remove Largest Dimension First
	//		B - Remove Most Expensive Matrix Multiplication First
	//		A - Remove Smallest Dimension First
	//		A - Remove Least Expensive Matrix Multiplication First
	//		A - Random Execution Tree 
	//		A - Ignorant Method
	vector<float> matricesA(30), matricesB(30), matricesC(30), matricesD(30), matricesE(30), matricesF(30);
	// Vector to store random numbers for the dimensions
	vector<int> dimensions, heuristicA, heuristicB, heuristicC, heuristicD, heuristicE, heuristicF;
	// Vector used for Optimal Solution
	vector<vector<int>> matricesTable;

	// As given in the project algorithm: n = possible size of the vectors
	for (int n = 10; n <= 25; n += 5) {
		// As given in the project algorithm: Create a list of 30 dimensions to be tested
		for (int r = 0; r < 30; r++) {
			//Clear the dimensions and reset it for the following loop for size n
			dimensions.clear();

			for (int i = 0; i < n; i++) {
				//Create vector of random values and store each vector into the array before clearing it
				dimensions.push_back(rand() % (highValue - lowValue + 1) + lowValue);				
			}
			// Make a separate copy of dimensions for each of the heuristics
			heuristicA = heuristicB = heuristicC = heuristicD = heuristicE = heuristicF = dimensions;
			matricesTable = CreateMatrices(dimensions);
			// Note, that all of these will be in percentages in comparison to the Optimal Solution
			optimal = MatrixChain(dimensions, matricesTable);
			// Assign each index of each Heuristic Array the overall cost for each vector
			// Evaluate heuristics a-f - find the cost of each heuristic and how it performs with the matrix multiplication
			matricesA[r] = float(HeuristicLargestFirst(heuristicA)) / float(optimal);
			matricesB[r] = float(HeuristicMostExpensive(heuristicB)) / float(optimal);
			matricesC[r] = float(HeuristicSmallestFirst(heuristicC)) / float(optimal);
			matricesD[r] = float(HeuristicLeastExpensive(heuristicD)) / float(optimal);
			matricesF[r] = float(HeuristicIgnorantMethod(heuristicD)) / float(optimal);

		}
		// Output the summary for the array of vectors (how did one test go for each heuristic)
		cout << "Heuristics' Analysis for Vector of Size " << n << endl;
		cout << setw(30) << "Heuristic" << setw(15) << "Minimum" << setw(15) << "Average" << setw(15) << "Maximum" << endl;
		cout << setw(30) << "Remove Largest Dimension" << setw(15) << *min_element(matricesA.begin(), matricesA.end()) << setw(15) << float(accumulate(matricesA.begin(), matricesA.end(), 0)) / float(matricesA.size()) << setw(15) << *max_element(matricesA.begin(), matricesA.end()) << endl;
		cout << setw(30) << "Remove Most Expensive Mult" << setw(15) << *min_element(matricesB.begin(), matricesB.end()) << setw(15) << float(accumulate(matricesB.begin(), matricesB.end(), 0)) / float(matricesB.size()) << setw(15) << *max_element(matricesB.begin(), matricesB.end()) << endl;
		cout << setw(30) << "Remove Smallest Dimension" << setw(15) << *min_element(matricesC.begin(), matricesC.end()) << setw(15) << float(accumulate(matricesC.begin(), matricesC.end(), 0)) / float(matricesC.size()) << setw(15) << *max_element(matricesC.begin(), matricesC.end()) << endl;
		cout << setw(30) << "Remove Least Expensive Mult" << setw(15) << *min_element(matricesD.begin(), matricesD.end()) << setw(15) << float(accumulate(matricesD.begin(), matricesD.end(), 0)) / float(matricesD.size()) << setw(15) << *max_element(matricesD.begin(), matricesD.end()) << endl;
		cout << setw(30) << "Ignorant Method" << setw(15) << *min_element(matricesF.begin(), matricesF.end()) << setw(15) << float(accumulate(matricesF.begin(), matricesF.end(), 0)) / float(matricesF.size()) << setw(15) << *max_element(matricesF.begin(), matricesF.end()) << endl;
		cout << endl;
	}
	system("pause");
	return 0;
}

// The below for loop was for texting purposes to ensure that the vectors were being created properly 
/*for (int j = 0; j < matrixArray[r].size(); j++) {
	cout << matrixArray[r][j] << " ";
}
cout << endl;*/