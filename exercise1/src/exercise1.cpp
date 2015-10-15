#include <gmpxx.h>
#include <iostream>
#include <sstream>
#include <string>
#define SUCCESS 0
#define FAILURE 1

#define MAX_ARR_SIZE 16777216
#define MAX_PRECISION 65536
#define PRECISSION_MUL 4
//#define LOG_INFO

using namespace std;



/**
 * implemented.
 */
mpf_class periodVal(const mpz_class* valuesArray, size_t size, size_t accuracy) {
  int current_period = 1;
  int minimal_period = size;

  for(;;) 
  {
      bool is_period_valid = true;
      for (long i = 1; i < size - current_period; i++) 
      {   
          if (valuesArray[i] != valuesArray[i+current_period]) 
          {   
              is_period_valid = false;
              break;
          }   
      }   

      if (is_period_valid)
      {   
          minimal_period = current_period;
          break;
      }   
    
      current_period += 1;
  }
  
  mpf_class result(minimal_period, accuracy);
  return result;
}


/**
 * Main function.
 * Current time for test2: 11s
 */
int main (int argc, char **argv) {
  // Cin && cout optimization.
  //std::ios_base::sync_with_stdio(false);
  // Max acc: 2^16 = 65536
  // Max size of array: 2^24 = 16777216
  // Max elem | value |: 2^64 = 18446744073709551616
  int accuracy = MAX_PRECISION; // Default printAccuracy.



  accuracy = atoi(argv[1]);
  if (accuracy < 1  && accuracy > MAX_PRECISION)
    return FAILURE;

  string decimalMeanValue;
  string decimalVarValue;

  mpz_class meanValue(0);
  //mpz_class meanValueSquare(0);
  mpz_class sum(0);
  mpz_class varSum(0);
  mpz_class vasSumTemp(0);
  mpz_class varValue(0);
  mpz_class periodValue(0);
  mpz_class remainderMeanValue(0);
  mpz_class remainderVarValue(0);
  mpz_class temp(0);

  // Main sequence.
  mpz_class* arr = new mpz_class[MAX_ARR_SIZE];
  size_t size = 0;

  string line;
#ifdef LOG_INFO
  cout << "Enter input in one line:" << endl;
#endif
  std::getline(cin, line);
  stringstream lineStream(line);

  mpz_class input_val;

  while (true)
  {
    if(!(lineStream >> input_val)) break;
    sum += input_val;
    arr[size] = input_val * input_val;
    varSum += arr[size];
    size++;
  }

  meanValue = sum / size;
  varSum = size * varSum - sum *sum;
  varValue = varSum/(size*size);

  remainderMeanValue = abs(sum - meanValue*size);
  remainderVarValue = abs(varSum - varValue *(size*size));

  if ( remainderMeanValue != 0){
    char* numberMean;
    for (int i=0;i<accuracy;i++)
    {
      remainderMeanValue *=10;
      temp = remainderMeanValue / size;
      remainderMeanValue = abs(remainderMeanValue - temp * size);
      numberMean = mpz_get_str(NULL,10,temp.get_mpz_t());
      decimalMeanValue.append(numberMean);
    }
    for (int i = decimalMeanValue.size() -1 ; i>=0 ; i--)
    {
      if (decimalMeanValue[i] == '0') decimalMeanValue.erase(i,1);
      else break;
    }

    cout << meanValue << ".";
    cout.precision(accuracy);
    //decimalMeanValue.resize(accuracy);
    cout << decimalMeanValue << endl;

  }
  else {
    cout << meanValue << endl;
  }

  if (remainderVarValue != 0){
    char* numberVar;
    for (int i=0;i<accuracy;i++)
    {
      remainderVarValue *=10;
      temp = remainderVarValue / (size * size);
      remainderVarValue = abs(remainderVarValue - temp * (size * size));
      numberVar = mpz_get_str(NULL,10,temp.get_mpz_t());
      cout << numberVar ;
      decimalVarValue.append(numberVar);
    }
    for (int i = decimalVarValue.size() - 1; i>=0; i-- )
    {
      if (decimalVarValue[i] == '0') decimalVarValue.erase(i,1);
      else break;
    }
    cout << endl;
    cout << varValue << ".";
    cout.precision(accuracy);
    //decimalVarValue.resize(accuracy);
    cout <<  decimalVarValue << endl;
  }
  else {
    cout << varValue <<endl;
  }
  cout.precision(accuracy);
  cout << periodVal(arr,size,accuracy) << endl;

  delete[](arr);
  return 0;
}