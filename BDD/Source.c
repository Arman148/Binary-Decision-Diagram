#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <time.h>


typedef struct node {
    struct node* left;
    struct node* right;
    char* value;

} NODE;


typedef struct bdd {
    int variables;
    int nodes;
    NODE* head;

} BDD;


typedef struct bf {
    int length;
    char* vector_array;
} BF;





// Recursive function to build BDD

NODE* buildBDD(NODE* parentNode, char* array) {


    if (strlen(array) > 0) {


        NODE* leftChild = malloc(sizeof(NODE));      // Create a new node for the left child
        NODE* rightChild = malloc(sizeof(NODE));       // Create a new node for the right child


        char* leftArray = malloc(sizeof(char) * strlen(array));         // Allocate memory for the left subarray
        char* rightArray = malloc(sizeof(char) * strlen(array));          // Allocate memory for the left subarray

        strncpy(leftArray, array, strlen(array) / 2);                       // Copy the left half of the array
        leftArray[strlen(array) / 2] = '\0';                                    // Add a null terminator to the left subarray
        strncpy(rightArray, array + strlen(array) / 2, strlen(array) - strlen(array) / 2);      // Copy the right half of the array
        rightArray[strlen(array) / 2] = '\0';                                                // Add a null terminator to the right subarray

        parentNode->value = malloc(sizeof(char) * strlen(array) + 1);                     // Allocate memory for the value of the parent node
        int length = strlen(array);


        // Copy the values from the original array to the parent node's value 
        for (int j = 0; j < length; j++)
        {
            parentNode->value[j] = array[j];
        }
        parentNode->value[strlen(array)] = '\0';                        // Add a null terminator to the parent node's value

        parentNode->left = buildBDD(leftChild, leftArray);              // Recursively build the left subtree
        parentNode->right = buildBDD(rightChild, rightArray);               // Recursively build the right subtree

        return parentNode;                   // Return the constructed node in the BDD
    }
    return parentNode;                           // Return the parent node if the array is empty
}

// Uses a BDD to evaluate inputs and returns the result
// Performs a lookup operation in a Binary Decision Diagram(BDD) based on the given inputs.


// Creates a BDD from a boolean function

BDD* BDD_create(BF* bfunkcia) {

    int totalNodes = 0;                     // Variable to track the total number of nodes in the BDD
    int vectorLength = bfunkcia->length;    // Length of the boolean function's vector array

    NODE* startNode = malloc(sizeof(NODE));;    // Create a new node as the starting node of the BDD
    BDD* bdd = malloc(sizeof(BDD));             // Create a new BDD struct

    startNode = buildBDD(startNode, bfunkcia->vector_array);     // Build the BDD recursively

    while (vectorLength != 1)
    {
        totalNodes += vectorLength;
        vectorLength /= 2;
    }
    // Calculate the total number of nodes in the BDD by halving the vector length iteratively

    bdd->head = startNode;               // Set the start node as the head of the BDD
    bdd->nodes = totalNodes;               // Set the total number of nodes in the BDD
    bdd->variables = log2(strlen(bfunkcia->vector_array));  // Calculate the number of variables in the BDD

    return bdd;          // Return the created BDD
}



char BDD_use(BDD* bdd, char* inputs) {

    signed char result = -1;                            // Initialize the result value as -1 (indicating an error)
    if (bdd->head == NULL)
        return result;                                  // Return the result if the BDD is empty (no head node)


    NODE* currentNode = bdd->head;                         // Start from the head of the BDD

    for (int i = 0; i < bdd->variables; i++)
    {


        if (inputs[i] == '0')
            currentNode = currentNode->left;                // Traverse to the left child if the input is '0'

        if (inputs[i] == '1')
            currentNode = currentNode->right;                   // Traverse to the right child if the input is '1'

    }
    return *currentNode->value;                                 // Return the value stored in the final node
}

// Prints information about the test

void information(long double testTime, long double averageTime, int numVariables, int numFunctions) {
    printf("test was for %d variables and %d fucntions\n", numVariables, numFunctions);
    printf("\n\n");
    printf("\tTime for whole test: %.3lf seconds\n\n", testTime);
    printf("\tAverage time: %.3lf seconds\n", averageTime);
}

// Increments a binary combination
//Performs binary increment operation on a combination represented as a string of '0's and '1's.


char* Binary_increment(char* combination, int size) {


    int sizeOfCombination = size;
    char* combinationCopy = malloc(sizeOfCombination * sizeof(char));                // Create a copy of the combination

    strncpy(combinationCopy, combination, sizeOfCombination);

    for (int i = sizeOfCombination - 1; i >= 0; i--)
    {
        if (combinationCopy[i] == '0')
        {
            combinationCopy[i] = '1';                       // If the current digit is '0', increment it to '1'
            return combinationCopy;                              // Return the incremented combination
        }

        combinationCopy[i] = '0';                                // If the current digit is '1', set it to '0' and continue
    }

    strncpy(combination, combinationCopy, sizeOfCombination);           // Copy the incremented combination back to the original combination
    free(combinationCopy);                                              // Free the memory allocated for the copy

    return combination;
}

// Generates a random vector for boolean functions

char* vectorGenerator(int powered_number, int bool_functions) {


    char* result = malloc(sizeof(char) * (powered_number + 1));             // Allocate memory for the binary vector
    srand(time(NULL));                                                       // Seed the random number generator with the current time

    for (int i = 0; i < powered_number; i++) {
        int random_number = rand() % (2 + bool_functions);                     // Generate a random number in the range [0, 2 + bool_functions) 
        switch (random_number) {
        case 0:
            result[i] = '0';                                                // Set the corresponding digit in the binary vector to '0'
            break;
        default:
            result[i] = '1';                                                   // Set the corresponding digit in the binary vector to '1'
            break;
        }
    }
    result[powered_number] = '\0';                                                  // Null-terminate the binary vector string

    return result;                                                                     // Return the generated binary vector


}

// Performs testing based on the number of variables and boolean functions

int testing(int variables, int boolFunctions) {
    printf("processing, please wait\n");

    long double totalProcessingTime = 0, averageProcessingTime = 0;
    int size = 1 << variables;
    clock_t startTime = clock();

    for (int i = 0; i < boolFunctions; i++) {
        int length = 1 << variables;



        char* vectorArray = vectorGenerator(length, boolFunctions);                     // Generate a random vector array


        BF* boolFunction = malloc(sizeof(BF));                                          // Create a Boolean function object
        boolFunction->vector_array = malloc(sizeof(char) * length);
        boolFunction->length = length;



        memcpy(boolFunction->vector_array, vectorArray, sizeof(char) * length);        // Copy the vector array to the Boolean function object



        BDD* bddTree = BDD_create(boolFunction);                              // Create a BDD tree for the Boolean function



        char* key = malloc(sizeof(char) * variables + 1);                   // Initialize the key for evaluating the BDD
        memset(key, '0', variables);
        key[variables] = '\0';

        char answer;
        for (int k = 0; k < size; k++) {

            answer = BDD_use(bddTree, key);             // Evaluate the BDD for the current key

            if (answer == -1) {
                printf("something went wrong\n");
            }

            key = Binary_increment(key, variables);              // Generate the next key
        }


        // Free memory allocated for the key and the Boolean function object
        free(key);
        free(boolFunction->vector_array);
        free(boolFunction);
    }

    clock_t endTime = clock();
    totalProcessingTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;
    averageProcessingTime = (totalProcessingTime / boolFunctions);

    information(totalProcessingTime, averageProcessingTime, variables, boolFunctions);          // Display information about the processing time
    return 0;
}

int main() {
    short int input = 0;
    printf("choose the test\n\n");

    printf("1 -> 2v, 200bf\n");
    printf("2 -> 2v, 300bf\n");
    printf("3 -> 5v, 200bf\n");
    printf("4 -> 5v, 300bf\n");
    printf("5 -> 10v, 200bf\n");
    printf("6 -> 10v, 300bf\n");
    printf("7 -> 13v, 200bf\n");
    printf("8 -> 13v, 300bf\n");
    printf("9 -> 14v, 200bf\n");
    printf("10 -> 14v, 300bf\n");

    scanf("%hi", &input);

    switch (input) {
    case 1:
        printf("\n");
        printf("Test 1\n");
        testing(2, 200);
        break;
    case 2:
        printf("\n");
        printf("Test 2\n");
        testing(2, 300);
        break;
    case 3:
        printf("\n");
        printf("Test 3\n");
        testing(5, 200);
        break;
    case 4:
        printf("\n");
        printf("Test 4\n");
        testing(5, 300);
        break;
    case 5:
        printf("\n");
        printf("Test 5\n");
        testing(10, 200);
        break;
    case 6:
        printf("\n");
        printf("Test 6\n");
        testing(10, 300);
        break;
    case 7:
        printf("\n");
        printf("Test 7\n");
        testing(13, 200);
        break;
    case 8:
        printf("\n");
        printf("Test 8\n");
        testing(13, 300);
        break;

    case 9:
        printf("\n");
        printf("Test 9\n");
        testing(14, 200);
        break;
        /*
    case 10:
        printf("\n");
        printf("Test 10\n");
        testing(14, 300);
        break;
        */
    default:
        printf("please try again\n");
    }
    return 0;
}











