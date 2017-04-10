#include <iostream>
#include <iomanip>
#include <cstdlib> 
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <string>
#include <sstream>
#include <limits>
using namespace std;

// Naming Conventions TitleCase = function, camelCase = variable
// Binh Mai
// CS404 
// Spring 2017 Semester

vector<vector<int>> CreateMatrices(vector<int> dimensions) {
	// Function to create the 2D vector that is utilized for the optimal solution
	// Takes in a vector and returns a 2D vector whose sizes (amount of rows and columns) come from the given vector

	vector<vector<int>> matrixTable;

	matrixTable.resize(dimensions.size());
	// Go through every row
	for (size_t i = 0; i < dimensions.size(); i++)
	{
		// Resize each column
		matrixTable[i].resize(dimensions.size());
		// For each cell, store a 0 value 
		for (size_t j = 0; j < dimensions.size(); j++)
		{
			matrixTable[i][j] = 0;
		}
	}
	return matrixTable;
}

// Optimal Solution for Matrix Multiplication
int OptimalSolution(vector<int> dimensions, vector<vector<int>>& matrixCosts, int i, int j) {
	// Function that calculates the optimal cost of a given list of dimensions
	// It utilizes a 2D vector that keeps track of which values have been calculated at a given time to avoid redundant recursive calls
	// Expects a vector of integers, a 2D matrix, and two values that point to the indexes
	int cost;
	int cost1, cost2, cost3;
	if (matrixCosts[i][j] != 0) { // 0 is a placeholder value
		return matrixCosts[i][j]; // If not 0, return the value that has already been calculated 
	}
	else if (i == j) { // If pointing to the same dimension, no need for calculation 
		return 0;
	}
	else { 
		matrixCosts[i][j] = numeric_limits<int>::max(); // Assign infinity to the matrix set
		for (int k = i; k < j; k++) { // Go through the dimensions
			cost1 = OptimalSolution(dimensions, matrixCosts, i, k);
			cost2 = OptimalSolution(dimensions, matrixCosts, k + 1, j);
			cost3 = dimensions[i - 1] * dimensions[k] * dimensions[j];
			cost = cost1 + cost2 + cost3; 
			if (cost < matrixCosts[i][j]) { // If the calculated cost for that particular dimension is smaller than the current value, then reassign it as the smaller value is more efficient
				matrixCosts[i][j] = cost;
			}
		}
		return matrixCosts[i][j]; // Return that cost
	}
}

// Wrapper Function for Optimal Solution for Matrix Chain Multiplication
int MatrixChain(vector<int> dimensions, vector<vector<int>>& matrixCosts) {
	// Wrapper function that calls on the Optimal Solution function repeatedly until a value has been reached
	// Expects a vector of integers and a 2D vector of integers 

	int j; // Variable that will be used to determine various lengths of the integers 
	for (size_t L = 1; L < dimensions.size(); L++) { // L is the length of the chain of matrices that are being examined
		for (size_t i = 1; i < dimensions.size() - L + 1; i++) { // To get various lengths/slices of the vector of dimensions 
			j = i + L - 1; // Find a separate partition of i - L
			OptimalSolution(dimensions, matrixCosts, i, j);
		}
	}
	return matrixCosts[1][dimensions.size()-1]; // Final result that contains the optimal solution 
}

// The below function is referred to as HeuristicA in the project statement and in the project report. 
// I have renamed it to heuristicLargestFirst to to adhere to naming practice
int HeuristicLargestFirst(vector<int> dimensions) {
	// Function takes in a vector and repeatedly proceeds to find the largest dimension, find the cost associated with it, and then remove it
	// Takes in a vector of integers and returns the associated cost with this heuristic 

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
	// Takes in a vector of integers and returns the associated cost with this heuristic 

	int cost = 0;
	int currentProduct = 0, greatestProduct = 0, index = 0; // Variables to keep track of the products
	
	// Find the largest value within the array
	while (dimensions.size() > 2) {
		// Reset the values
		currentProduct = 0;
		greatestProduct = 0;
		index = 0;
		for (size_t i = 1; i < dimensions.size() - 1; i++) {
			// Get product and compare with current product associated with the dimension
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
	// Takes in a vector of integers and returns the associated cost with this heuristic 

	int cost = 0;
	int smallestValue = 0, index = 0;
	// Find the smallest value within the array
	while (dimensions.size() > 2) {		
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
	// Takes in a vector of integers and returns the associated cost with this heuristic 

	int cost = 0;
	int currentProduct = 0, smallestProduct = 0, index = 0;
	while (dimensions.size() > 2) {
		
		// Initialize it to the first product as a comparison point
		smallestProduct = dimensions[0] * dimensions[1] * dimensions[2];
		index = 1;
		// Find the smallest product within the array 
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

// The below function is referred to as HeuristicE in the project statement and in the project report. 
// I have renamed it to heuristicLeastExpensive to adhere to naming practice
int RandomExecutionTree(vector<int> matrixChain) {
	// Function to randomly produce cost with an associated vector of dimensions
	// Takes in a vector of integers, two indices, and returns the associated cost with this heuristic 
	int cost = 0;
	// Indexes for the random execution tree 
	int random;
	vector<int> dimensions = matrixChain; 
	while (dimensions.size() > 2) {
		random = rand() % (dimensions.size() - 2) + 1; // Obtain a random dimension of the inner dimensions
		cost += dimensions[random - 1] * dimensions[random] * dimensions[random + 1]; //multiply by that dimension
		dimensions.erase(dimensions.begin() + random); // Erase that random dimension
	}
	return cost;
}

float RandomExecutionTreeWrapper(vector<int> dimensions) {
	// Vector used to store all of the various calculations 
	// Takes in a vector of integers and returns the associated cost with this heuristic 

	vector<float> calculatedTrees;
	size_t counter = 0; 
	size_t stopCount = dimensions.size() * 2; 

	// Note that the professor has changed the algorithm such that we stop when we find 2n random execution trees, and return the minimum cost found

	while (counter < stopCount ){
		calculatedTrees.push_back(float(RandomExecutionTree(dimensions)));
		counter += 1;
	}
	return *min_element(calculatedTrees.begin(), calculatedTrees.end());
	
}

// The below function is referred to as HeuristicF in the project statement and in the project report. 
// I have renamed it to heuristicIgnorantMethod to adhere to naming practices
int HeuristicIgnorantMethod(vector<int> dimensions) {
	// Function takes in a vector and proceeds to multiply it straight across

	int cost = 0; //Get the value of the first index
	int currentIndex = 1; 
	while (dimensions.size() > 2) {
		cost += dimensions[0] * dimensions[1] * dimensions[2]; 
		dimensions.erase(dimensions.begin() + 1); 
	}
	return cost;
}

// MATRIX CHAIN HEURISTIC EXPERIMENT
void RandomMatrixChainExperiment() {
	// This function is the Matrix Chain Heuristic Experiment that was described in the project statement 
	// As given in the project algorithm: Range for the low and high value for the random matrices' dimensions
	int lowValue = 7, highValue = 17;
	int optimal;

	// Array to store costs, and the cost is the corresponding matrix divided by the optimal solution
	// Made to be size 30 as that is the number of vectors to test in the Heuristic Experiment
	// The following letters match up:
	//		A - Remove Largest Dimension First
	//		B - Remove Most Expensive Matrix Multiplication First
	//		C - Remove Smallest Dimension First
	//		D - Remove Least Expensive Matrix Multiplication First
	//		E - Random Execution Tree 
	//		F - Ignorant Method
	vector<float> matricesA(30), matricesB(30), matricesC(30), matricesD(30), matricesE(30), matricesF(30);

	// Vector to store random numbers for the dimensions
	vector<int> dimensions, heuristicA, heuristicB, heuristicC, heuristicD, heuristicE, heuristicF;

	// Vector used for Optimal Solution
	vector<vector<int>> matricesTable;

	cout << "Matrix Chain Heuristic Experiment" << endl;
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
			// Overwrites the previous file image
			matricesA[r] = float(HeuristicLargestFirst(heuristicA)) / float(optimal);
			matricesB[r] = float(HeuristicMostExpensive(heuristicB)) / float(optimal);
			matricesC[r] = float(HeuristicSmallestFirst(heuristicC)) / float(optimal);
			matricesD[r] = float(HeuristicLeastExpensive(heuristicD)) / float(optimal);
			matricesE[r] = float(RandomExecutionTreeWrapper(heuristicE)) / float(optimal);
			matricesF[r] = float(HeuristicIgnorantMethod(heuristicF)) / float(optimal);

		}
		// Output the summary for the array of vectors (how did one test go for each heuristic)
		cout << "Heuristics' Analysis for Vector of Size " << n << endl;
		cout << setw(30) << "Heuristic" << setw(15) << "Minimum" << setw(15) << "Average" << setw(15) << "Maximum" << endl;
		cout << setw(30) << "Remove Largest Dimension" << setw(15) << *min_element(matricesA.begin(), matricesA.end()) << setw(15) << float(accumulate(matricesA.begin(), matricesA.end(), 0)) / float(matricesA.size()) << setw(15) << *max_element(matricesA.begin(), matricesA.end()) << endl;
		cout << setw(30) << "Remove Most Expensive Mult" << setw(15) << *min_element(matricesB.begin(), matricesB.end()) << setw(15) << float(accumulate(matricesB.begin(), matricesB.end(), 0)) / float(matricesB.size()) << setw(15) << *max_element(matricesB.begin(), matricesB.end()) << endl;
		cout << setw(30) << "Remove Smallest Dimension" << setw(15) << *min_element(matricesC.begin(), matricesC.end()) << setw(15) << float(accumulate(matricesC.begin(), matricesC.end(), 0)) / float(matricesC.size()) << setw(15) << *max_element(matricesC.begin(), matricesC.end()) << endl;
		cout << setw(30) << "Remove Least Expensive Mult" << setw(15) << *min_element(matricesD.begin(), matricesD.end()) << setw(15) << float(accumulate(matricesD.begin(), matricesD.end(), 0)) / float(matricesD.size()) << setw(15) << *max_element(matricesD.begin(), matricesD.end()) << endl;
		cout << setw(30) << "Random Execution Tree" << setw(15) << *min_element(matricesE.begin(), matricesE.end()) << setw(15) << float(accumulate(matricesE.begin(), matricesE.end(), 0)) / float(matricesE.size()) << setw(15) << *max_element(matricesE.begin(), matricesE.end()) << endl;
		cout << setw(30) << "Ignorant Method" << setw(15) << *min_element(matricesF.begin(), matricesF.end()) << setw(15) << float(accumulate(matricesF.begin(), matricesF.end(), 0)) / float(matricesF.size()) << setw(15) << *max_element(matricesF.begin(), matricesF.end()) << endl;
		cout << endl;
	}
}

void DimensionsCalculation(vector<int> dimensions) {
	// Variables that hold the cost of each of the various heuristics 
	int optimal;
	vector<vector<int>> matricesTable;

	// Calculate the optimal solution 
	matricesTable = CreateMatrices(dimensions);
	optimal = MatrixChain(dimensions, matricesTable);

	vector<int> heuristicA, heuristicB, heuristicC, heuristicD, heuristicE, heuristicF;
	heuristicA = heuristicB = heuristicC = heuristicD = heuristicE = heuristicF = dimensions;

	// Calculate and output all of the heuristics
	cout << "Calculated Heuristics for Given Input File => " << endl;
	cout << setw(30) << "Remove Largest Dimension" << setw(15) << float(HeuristicLargestFirst(heuristicA)) / float(optimal) << endl;
	cout << setw(30) << "Remove Most Expensive Mult" << setw(15) << float(HeuristicMostExpensive(heuristicB)) / float(optimal) << endl;
	cout << setw(30) << "Remove Smallest Dimension" << setw(15) << float(HeuristicSmallestFirst(heuristicC)) / float(optimal) << endl;
	cout << setw(30) << "Remove Least Expensive Mult" << setw(15) << float(HeuristicLeastExpensive(heuristicD)) / float(optimal) << endl;
	cout << setw(30) << "Random Execution Tree" << setw(15) << float(RandomExecutionTreeWrapper(heuristicE)) / float(optimal) << endl;
	cout << setw(30) << "Ignorant Method" << setw(15) << float(HeuristicIgnorantMethod(heuristicF)) / float(optimal) << endl;
}

void FileDimensionsInput(string fileName) {
	/*Primary function that takes in a string that refers to a file name, and and extracts the data that is present within. 
	Inputs: a string that refers to a file name
	Outputs: no variable is returned but all heuristics are calculated */

	// Variable Declarations
	vector<int> dimensions; 
	ifstream infile;
	string line; 
	infile.open(fileName);

	cout << "Heuristic Analysis for" << fileName << endl;

	// Get data from the file input
	if (infile) {
		while (getline(infile, line)) {
			stringstream linestream(line);
			string value; 

			while (getline(linestream, value, ',')) {
				dimensions.push_back(stof(value)); 
			}
		}
	}
	cout << "File Data" << endl;

	// Output the data for reference to help put context to output
	for (size_t i = 0; i < dimensions.size(); i++) {
		cout << dimensions[i] << " ";
	}
	cout << endl;

	// Calculate heuristics with the following function: 
	DimensionsCalculation(dimensions); 
	// Close the file
	infile.close(); 
}

void UserInputExperiment() {
	// Function to get input from the user and call all of the heuristics on that example
	// While the function doesn't return any values per se, it does output all of the values to the console
	vector<int> dimensions;
	int number; 
	cout << "Enter the numbers you wish to evaluate. When you are done, enter in a non-number to submit your responses." << endl;
	while (cin >> number) {
		dimensions.push_back(number);
	}
	DimensionsCalculation(dimensions);
}

void TestCases() {
	// Test case used to evaluate that my algorithms were correct
	// Tested these against my own hand written analysis
	
	vector<int> testing, testingA, testingB, testingC, testingD, testingE, testingF;
	// Add values to be tested 
	testing.push_back(3);
	testing.push_back(5);
	testing.push_back(7);
	testing.push_back(8);
	testing.push_back(9);
	vector<vector<int>> testingTable = CreateMatrices(testing);
	testingA = testingB = testingC = testingD = testingE = testingF = testing;
	cout << "Optimal Solution       - should be 489 ->" << MatrixChain(testing, testingTable) << endl; // Optimal Value should be 489
	cout << "Remove Largest Value   - should be 954 ->" << HeuristicLargestFirst(testingA) << endl; // Value should be 954
	cout << "Remove Most Expensive  - should be 954 ->" << HeuristicMostExpensive(testingB) << endl; // Value should be 954
	cout << "Remove Smallest Value  - should be 489 ->" << HeuristicSmallestFirst(testingC) << endl; // Value should be 489
	cout << "Remove Least Expensive - should be 489 ->" << HeuristicLeastExpensive(testingD) << endl; // Value should be 489
	cout << "Random Execution Tree  - unknown value ->" << RandomExecutionTreeWrapper(testingE) << endl; // Value should be 489
	cout << "Ignorant Method        - should be 489 ->" << HeuristicIgnorantMethod(testingF) << endl;	// Value should be 489
}

int main() {

	char userInput; 
	cout << "Welcome to the CS404 Project involving Matrix Multiplication" << endl; 
	while (true) {
		cout << "Press 1 to see the Matrix Chain Experiment with random numbers" << endl
			<< "Press 2 to see the Matrix Chain Experiment with the given files" << endl
			<< "Press 3 to input your own values for the matrix chain" << endl
			<< "Press 4 to see the test case that was created" << endl
			<< "Press Q to quit the program" << endl << endl;
		cin >> userInput;
		if (userInput == '1') {
			// MATRIX CHAIN HEURISTIC EXPERIMENT 
			RandomMatrixChainExperiment();
		}
		else if (userInput == '2') {
			// MATRIX CHAIN FILE EXPERIMENT 
			cout << "Matrix Chain File Experiment" << endl << endl;
			// These are all of the various input files given to us, and one can hardcode the input file if necessary
			FileDimensionsInput("CS404SP17MatrixChainHeuristicsInput1.txt");
			FileDimensionsInput("CS404SP17MatrixChainHeuristicsInput2.txt");
			FileDimensionsInput("CS404SP17MatrixChainHeuristicsInput3.txt");
			FileDimensionsInput("CS404SP17MatrixChainHeuristicsInput4.txt");
			FileDimensionsInput("CS404SP17MatrixChainHeuristicsInput5.txt");
			FileDimensionsInput("CS404SP17MatrixChainHeuristicsInput6.txt");
			FileDimensionsInput("CS404SP17MatrixChainHeuristicsInput7.txt");
		}
		else if (userInput == '3') {
			// ALLOW USER INPUT
			UserInputExperiment();
		}
		else if (userInput == '4') {
			// SHOW TEST CASES
			TestCases(); 
		}
		else if (toupper(userInput) == 'Q') {
			// QUIT
			return 1;
		}
		else {
			cout << "Invalid Response" << endl; 
		}

		// Reset the input stream
		cin.clear();
		cin.ignore(10000, '\n'); 
	}
}