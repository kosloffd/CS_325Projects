#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct result
{
    int dataLength;
    int maxSum;
    int* data;
};


void enumeration(int inputArray[], int length, struct result* results);
void betterEnumeration(int inputArray[], int length, struct result* results);
void divideConquer(int inputArray[], int length, struct result* results);
int maxSubarray(int inputArray[], int lowIndex, int highIndex, int* maxLIdx, int* maxHIdx, int* highestSum);
int maxSuffix(int inputArray[], int startIndex, int endIndex, int* maxLowIndex);
int maxPrefix(int inputArray[], int startIndex, int endIndex, int* maxHighIndex);
void linearTime(int inputArray[], int length, struct result* results);
int readArray(FILE* fp, int arrayToFill[], int* arrayLength);
int* resizeArray(int numberArray[], int* arraySize);
void writeArray(FILE* fp, int array[], int qtyInArray);
void writeToFile(FILE* fp, const char* description, struct result* results);


int main()
{
    struct result* myResults = (struct result*)malloc(sizeof(struct result));
    assert(myResults != NULL);

    int qtyInArray;
    int arraySize = 20;
    int* numberArray = malloc(arraySize*sizeof(int));
    assert (numberArray != NULL);

    FILE* fp = fopen("MSS_TestProblems.txt", "r");
    FILE* fpout = fopen("results.txt", "w");

    qtyInArray = readArray(fp, numberArray, &arraySize);

    do
    {
        writeArray(fpout, numberArray, qtyInArray);

        myResults->data = malloc(qtyInArray*sizeof(int));
        assert(myResults->data != NULL);

        int* values = numberArray;
        int length = qtyInArray;

        enumeration(values, length, myResults);
        writeToFile(fpout, "Enumeration", myResults);

        betterEnumeration(values, length, myResults);
        writeToFile(fpout, "Better Enumeration", myResults);

        divideConquer(values, length, myResults);
        writeToFile(fpout, "Divide and Conquer", myResults);

        linearTime(values, length, myResults);
        writeToFile(fpout, "Linear Time", myResults);

        qtyInArray = readArray(fp, numberArray, &arraySize);
    }while(qtyInArray > 0);

    fclose(fp);
    fclose(fpout);
    return 0;
}




/*  Fills an array from a file that contains integers in
    array format, i.e. [5, 4, -1,...]
    params:
        fp:             The file pointer for the file to use
        arrayToFill:    The array which will be filled with data from the file
        arrayLength:    The initial length of arrayToFill
    returns:
        totalNums:      The number of integers that were put into the array or -1 if EOF is reached
*/
int readArray(FILE* fp, int arrayToFill[], int* arrayLength)
{
    int totalNums, endOfArray;
    totalNums = endOfArray = 0;
    char readChar;

    do
    {                                   // Put the file pointer just beyond the opening bracket
        if(feof(fp))                    // Exit if EOF reached
            return -1;
        fscanf(fp, "%c", &readChar);
    } while(readChar != '[');
    do
    {
        fscanf(fp, "%d%c%*c", &arrayToFill[totalNums++], &readChar);     //I'm thinking it will read number, [comma/closeBracket], [space/newline]
        if(readChar == ']')
            endOfArray = 1;
        else if(totalNums == *arrayLength)
                arrayToFill = resizeArray(arrayToFill, arrayLength);
    }while(endOfArray == 0);

    return totalNums;
}

int* resizeArray(int numberArray[], int* arraySize)
{
    int i;
    int newSize = *arraySize * 2;
    int* newArray = malloc(newSize*sizeof(int));    //Make a new array twice as large
    assert(newArray != NULL);

    for(i=0; i < *arraySize; i++)       // Copy the old into the new
        newArray[i] = numberArray[i];
    free(numberArray);                  // Free the old array
    *arraySize = newSize;               // Update the arraySize
    return newArray;                    // Return a pointer to the new array
}

void writeArray(FILE* fp, int array[], int qtyInArray)
{
    fprintf(fp, "[");
    int j;
    for(j=0; j<qtyInArray-1; j++)
        fprintf(fp, "%d, ", array[j]);
    fprintf(fp, "%d]\n", array[j]);
}

void writeToFile(FILE* fp, const char* description, struct result* results)
{
    fprintf(fp, "%s Method Results:\n", description);
    writeArray(fp, results->data, results->dataLength);
    fprintf(fp, "%d\n\n", results->maxSum);

}


void enumeration(int inputArray[], int length, struct result* results)
{
    int i, j, k, sum, max, maxLowIdx, maxHiIdx;
    sum = max = maxLowIdx = maxHiIdx =0;
    for(i=0; i<length; i++)
    {
        for(j=i; j<length; j++)     //j is just a placeholder to say when the sums using k should stop
        {
            sum = 0;
            for(k=i; k <= j; k++)    //This sums all individual elements from position i to position j
            {
                sum += inputArray[k];
                if(max < sum)
                {
                    max = sum;
                    maxLowIdx = i;
                    maxHiIdx = k;
                }
            }
        }
    }

     j = 0;
     for(i = maxLowIdx; i <= maxHiIdx; i++, j++)
            results->data[j] = inputArray[i];
        results->dataLength = j;
        results->maxSum = max;
}




void betterEnumeration(int inputArray[], int length, struct result* results)
{
    int i, j, max, currentSum, maxLowIdx, maxHiIdx;
    currentSum = max = maxLowIdx = maxHiIdx = 0;
    for(i=0; i < length; i++)
    {
        currentSum = inputArray[i];
        for(j=i+1; j<length; j++)
        {
            currentSum += inputArray[j];
            if( max < currentSum )
            {
                max = currentSum;
                maxLowIdx = i;
                maxHiIdx = j;
            }
        }
    }
    j = 0;
    for(i = maxLowIdx; i <= maxHiIdx; i++, j++)
        results->data[j] = inputArray[i];
    results->dataLength = j;
    results->maxSum = max;
}


void divideConquer(int inputArray[], int length, struct result* results)
{
    int i, lowIndex, highIndex, greatestSum;
    int j = lowIndex = highIndex = 0;
    greatestSum = inputArray[0];
    results->maxSum = maxSubarray(inputArray, 0, length-1, &lowIndex, &highIndex, &greatestSum);

    for(i = lowIndex; i <= highIndex; i++, j++)
        results->data[j] = inputArray[i];
    results->dataLength = j;

}

int maxSubarray(int inputArray[], int lowIndex, int highIndex, int* maxLIdx, int* maxHIdx, int* highestSum)
{
    // Base Case
    if(lowIndex >= highIndex)
        return inputArray[lowIndex];

/*  Else we need to compare the max subarray of the high and low sides with the
    max cross for each calculation and return that from the function.
*/
    else
    {
        int mid = (highIndex-lowIndex)/2 + lowIndex;
        int lowSum = maxSubarray(inputArray, lowIndex, mid, maxLIdx, maxHIdx, highestSum);
        int highSum = maxSubarray(inputArray, mid+1, highIndex, maxLIdx, maxHIdx, highestSum);
        int crossLow, crossHigh;
        int crossSum = maxSuffix(inputArray, lowIndex, mid, &crossLow) + maxPrefix(inputArray, mid+1, highIndex, &crossHigh);

        if(lowSum >= crossSum && lowSum >= highSum)
        {
            if(lowSum > *highestSum)         // We don't want to update the indexes unless it's actually a new, greater sum than previously
            {
                *highestSum = lowSum;
                *maxLIdx = lowIndex;
                *maxHIdx = mid;
            }
        }
        else if(crossSum >= lowSum && crossSum >= highSum)
        {
            if(crossSum > *highestSum)
            {
                *highestSum = crossSum;
                *maxLIdx = crossLow;
                *maxHIdx = crossHigh;
            }
        }
        else
        {
            if(highSum > *highestSum)
            {
                *highestSum = highSum;
                *maxLIdx = mid+1;
                *maxHIdx = highIndex;
            }
        }
    }
    return *highestSum;
}

int maxSuffix(int inputArray[], int endIndex, int startIndex, int* maxLowIndex)
{
    int maxSum, currentSum;
    *maxLowIndex = startIndex;
    maxSum = inputArray[startIndex];
    currentSum = 0;
    while(startIndex >= endIndex)
    {
        currentSum += inputArray[startIndex];
        if(currentSum > maxSum)
        {
            maxSum = currentSum;
            *maxLowIndex = startIndex;
        }
        startIndex--;
    }
    return maxSum;
}

int maxPrefix(int inputArray[], int startIndex, int endIndex, int* maxHighIndex)
{
    int maxSum, currentSum;
    *maxHighIndex = startIndex;
    maxSum = inputArray[startIndex];
    currentSum = 0;
    while(startIndex <= endIndex)
    {
        currentSum += inputArray[startIndex];
        if(currentSum > maxSum)
        {
            *maxHighIndex = startIndex;
            maxSum = currentSum;
        }
        startIndex++;
    }
    return maxSum;
}


void linearTime(int inputArray[], int length, struct result* results)
{
    /* Use saved values of the maximum subarray up to each index,
        then if A[i] + maxSubArray[i-1] >= maxSubArray[i-1],
        maxSubarray[i] = maxSubArray[i-1] + A[i].
    */
    int i, testSum, largestSA, maxLowIdx, maxHighIdx;
    maxLowIdx = maxHighIdx = 0;
    int* maxSubarray = malloc(length*sizeof(int));
    largestSA = maxSubarray[0] = inputArray[0];
    for(i=1; i<length; i++)
    {
        testSum = inputArray[i]+ maxSubarray[i-1];
        if( testSum > inputArray[i])            // Couldn't we just make sure inputArray[i] is positive instead?
            maxSubarray[i] = testSum;
        else
            maxSubarray[i] = inputArray[i];

        if(testSum > largestSA)
        {
            largestSA = testSum;
            maxHighIdx = i;
        }
    }

    // Find the lowest index
    maxLowIdx = maxHighIdx;
    testSum = inputArray[maxHighIdx];
    do
    {
        testSum += inputArray[--maxLowIdx];
    }while(testSum != largestSA);

    int j = 0;
    for(i = maxLowIdx; i <= maxHighIdx; i++, j++)
        results->data[j] = inputArray[i];

    results->dataLength = j;
    results->maxSum = largestSA;

    free(maxSubarray);
}
