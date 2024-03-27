// Ali jahanbanian 
// Multiply Matrix 4 × 4 (32 Bit) 
// with Assembly and Cpp, Cpp, Avx Languages.

#include <iostream>
#include <iomanip>
#include <immintrin.h>
#include <chrono>

using namespace std;

extern "C" void CalcMulMatrix_asm_(int* y, const int* x, const int* z, int rowcol);
void CalcMulMatrixCpp(int y[][4], int x[][4], int z[][4], int rowcol);
void CalcMulMatrixAVX(int mat1[][4], int mat2[][4], int result[][4], int rowcol);

int main()
{
	const int rowcol = 4;
	int result[rowcol][rowcol];
	int result1[rowcol][rowcol];
	int result2[rowcol][rowcol];
	int Mat1[rowcol][rowcol]{ {2, 4, 3, 4}, {8, 6, 5, 1}, {9, 1, 4, 5}, {3, 1, 5, 3} };
	int Mat2[rowcol][rowcol]{ {6, 4, 3, 1}, {5, 6, 7, 8}, {1, 0, 3, 2}, {1, 4, 1, 6} };
	//Run Time Assembly:
	auto start1 = chrono::steady_clock::now();
	CalcMulMatrix_asm_(&result[0][0], &Mat1[0][0], &Mat2[0][0], rowcol);
	auto end1 = chrono::steady_clock::now();
	//Run Time Cpp:
	auto start2 = chrono::steady_clock::now();
	CalcMulMatrixCpp(result1, Mat1, Mat2, rowcol);
	auto end2 = chrono::steady_clock::now();
	//Run Time AVX:
	auto start3 = chrono::steady_clock::now();
	CalcMulMatrixAVX(Mat1, Mat2, result2,rowcol);
	auto end3 = chrono::steady_clock::now();

	cout << "Assembly" << endl;
	for (int i = 0; i < rowcol; i++)
	{
		for (int j = 0; j < rowcol; j++)
		{
			cout  << result[i][j] << ' ';
		}
		cout << endl;
	}
	cout << endl;

	cout << "Cpp" << endl;
	for (int i = 0; i < rowcol; i++)
	{
		for (int j = 0; j < rowcol; j++)
		{
			cout << result1[i][j] << ' ';
		}
		cout << endl;
	}
	cout << endl;

	cout << "AVX" << endl;
	for (int i = 0; i < rowcol; i++)
	{
		for (int j = 0; j < rowcol; j++)
		{
			cout << result2[i][j] << ' ';
		}
		cout << endl;
	}
	cout << endl;

	cout << endl << "Assembly: " << chrono::duration_cast<chrono::nanoseconds>(end1 - start1).count() << " nanosec" << endl;
	cout << endl << "Cpp: " << chrono::duration_cast<chrono::nanoseconds>(end2 - start2).count() << " nanosec" << endl;
	cout << endl << "AVX: " << chrono::duration_cast<chrono::nanoseconds>(end3 - start3).count() << " nanosec" << endl;

	return 0;
}
void CalcMulMatrixCpp(int result[4][4], int mat1[4][4], int mat2[4][4], int rowcol)
{
	// Zeroing array elements
	for (int i = 0; i < rowcol; i++)
		for (int j = 0; j < rowcol; j++)
			result[i][j] = 0;

	for (int i = 0; i < rowcol; i++)
	{
		for (int j = 0; j < rowcol; j++)
		{
			for (int k = 0; k < rowcol; k++)
			{
				result[i][j] += mat1[i][k] * mat2[k][j];
			}
		}
	}

}
void CalcMulMatrixAVX(int mat1[4][4], int mat2[4][4], int result[4][4], int rowcol)
{
	//Initialize vector to zero
	__m128i vec_result = _mm_setzero_si128();
	__m128i vec_mat1   = _mm_setzero_si128();
	__m128i vec_mat2   = _mm_setzero_si128();

	// Zeroing array elements
	for (int i = 0; i < rowcol; i++)
	{
		for (int k = 0; k < rowcol; k += 4)
		{
			_mm_storeu_si128((__m128i*) & result[i][k], vec_result);
		}
	}

	for (int i = 0; i < rowcol; i++)
	{
		for (int j = 0; j < rowcol; j++)
		{
			//Stores one element in mat1
			vec_mat1 = _mm_set1_epi32(mat1[i][j]);

			for (int k = 0; k < rowcol; k += 4)
			{
				//Stores row of second matrix
				vec_mat2 = _mm_loadu_si128((__m128i*) & mat2[j][k]);
				//Loads the result matrix row as a vector
				vec_result = _mm_loadu_si128((__m128i*) & result[i][k]);
				//Multiplies the vectors and adds to the result vector
				vec_result = _mm_add_epi32(vec_result, _mm_mullo_epi32(vec_mat1, vec_mat2));
				//Stores into the result array
				_mm_storeu_si128((__m128i*) & result[i][k], vec_result);
			}
		}
	}
}