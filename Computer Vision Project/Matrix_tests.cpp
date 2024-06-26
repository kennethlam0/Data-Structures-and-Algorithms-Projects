// Project UID af1f95f547e44c8ea88730dfb185559d

#include "Matrix.h"
#include "Matrix_test_helpers.h"
#include "unit_test_framework.h"

using namespace std;


// Here's a free test for you! Model yours after this one.
// Test functions have no interface and thus no RMEs, but
// add a comment like the one here to say what it is testing.
// -----
// Fills a 3x5 Matrix with a value and checks
// that Matrix_at returns that value for each element.
TEST(test_fill_basic) {
  Matrix *mat = new Matrix; // create a Matrix in dynamic memory

  const int width = 3;
  const int height = 5;
  const int value = 42;
  Matrix_init(mat, 3, 5);
  Matrix_fill(mat, value);

  for(int r = 0; r < height; ++r){
    for(int c = 0; c < width; ++c){
      ASSERT_EQUAL(*Matrix_at(mat, r, c), value);
    }
  }

  delete mat; // delete the Matrix
}

// ADD YOUR TESTS HERE
// You are encouraged to use any functions from Matrix_test_helpers.h as needed.

TEST(test_matrix_print) {
  Matrix *mat = new Matrix;
 
  Matrix_init(mat, 2, 2);
  *Matrix_at(mat, 0, 0) = 1;
  *Matrix_at(mat, 0, 1) = 0;
  *Matrix_at(mat, 1, 0) = 0;
  *Matrix_at(mat, 1, 1) = 1;
  ostringstream expected;
  expected << "2 2\n"
           << "1 0 \n" << "0 1 \n";
  ostringstream actual;
  Matrix_print(mat, actual);
  ASSERT_EQUAL(expected.str(), actual.str());
  delete mat;

  Matrix *mat2 = new Matrix;
  Matrix_init(mat2, 1,1);
  *Matrix_at(mat2, 0, 0) = 0;
  ostringstream expected2;
  expected2 << "1 1\n" << "0 \n";
  ostringstream actual2;
  Matrix_print(mat2, actual2);
  ASSERT_EQUAL(expected2.str(), actual2.str());
  delete mat2;

  Matrix *mat3 = new Matrix;
  Matrix_init(mat3, 3,5);
  *Matrix_at(mat3, 0, 0) = 1;
  *Matrix_at(mat3, 0, 1) = 2;
  *Matrix_at(mat3, 0, 2) = 3;
  *Matrix_at(mat3, 1, 0) = 4;
  *Matrix_at(mat3, 1, 1) = 5;
  *Matrix_at(mat3, 1, 2) = 6;
  *Matrix_at(mat3, 2, 0) = 7;
  *Matrix_at(mat3, 2, 1) = 8;
  *Matrix_at(mat3, 2, 2) = 9;
  *Matrix_at(mat3, 3, 0) = 10;
  *Matrix_at(mat3, 3, 1) = 11;
  *Matrix_at(mat3, 3, 2) = 12;
  *Matrix_at(mat3, 4, 0) = 13;
  *Matrix_at(mat3, 4, 1) = 14;
  *Matrix_at(mat3, 4, 2) = 15;
  Matrix_fill_border(mat3,0);
  ostringstream expected3;
  expected3 << "3 5\n" << "0 0 0 \n" << "0 5 0 \n" 
  << "0 8 0 \n" << "0 11 0 \n" << "0 0 0 \n";
  ostringstream actual3;
  Matrix_print(mat3, actual3);
  ASSERT_EQUAL(expected3.str(), actual3.str());
  delete mat3;
}

TEST(test_matrix_width)
{
  Matrix *mat = new Matrix;
  Matrix_init(mat, 2,2);
  ASSERT_EQUAL(Matrix_width(mat),2);
  Matrix *mat2 = new Matrix; 
  Matrix_init(mat2, 1,1);
  ASSERT_EQUAL(Matrix_width(mat2),1)
  Matrix *mat4 = new Matrix;
  Matrix_init(mat4,100,100);
  ASSERT_TRUE(Matrix_width(mat4)==100);
  Matrix *mat5 = new Matrix;
  Matrix_init(mat5,242,300);
  ASSERT_TRUE(Matrix_width(mat5)==242);
  delete mat;
  delete mat2;
  delete mat4;
  delete mat5;
}

TEST(test_matrix_height)
{
  Matrix *mat = new Matrix;
  Matrix_init(mat,2,2);
  ASSERT_EQUAL(Matrix_height(mat),2);
  Matrix *mat2 = new Matrix;
  Matrix_init(mat2,1,1);
  ASSERT_EQUAL(Matrix_height(mat2),1);
  Matrix *mat3 = new Matrix;
  Matrix_init(mat3,0,0);
  ASSERT_EQUAL(Matrix_height(mat3),0);
  Matrix *mat4 = new Matrix;
  Matrix_init(mat4,100,100);
  ASSERT_TRUE(Matrix_height(mat4)==100);
  Matrix *mat5 = new Matrix;
  Matrix_init(mat5,223,300);
  ASSERT_TRUE(Matrix_height(mat5)==300);
  delete mat;
  delete mat2; 
  delete mat3;
  delete mat4;
  delete mat5;
}

TEST(test_matrix_row)
{
  Matrix *mat = new Matrix;
  Matrix_init(mat, 2, 2);
  *Matrix_at(mat,0,0) = 1;
  *Matrix_at(mat,0,1) = 2;
  *Matrix_at(mat,1,0) = 3;
  *Matrix_at(mat,1,1) = 4;
  int *ptr = Matrix_at(mat,1,0);
  ASSERT_EQUAL(Matrix_row(mat,ptr),1);
  Matrix *mat2 = new Matrix; 
  Matrix_init(mat2, 1, 1);
  *Matrix_at(mat2,0,0) = 1;
  int *ptr2 = Matrix_at(mat2,0,0);
  ASSERT_EQUAL(Matrix_row(mat2,ptr2),0);
  delete mat;
  delete mat2;
}

TEST(test_matrix_column)
{
  Matrix *mat = new Matrix;
  Matrix_init(mat,2,2);
  *Matrix_at(mat,0,0)=1;
  *Matrix_at(mat,0,1)=2;
  *Matrix_at(mat,1,0)=3;
  *Matrix_at(mat,1,1)=4;
  int *ptr=Matrix_at(mat,0,0);
  ASSERT_EQUAL(Matrix_column(mat,ptr),0);
  delete mat;
  Matrix *mat2 = new Matrix;
  Matrix_init(mat2,1,1);
  *Matrix_at(mat2,0,0)=0;
  int *ptr2=Matrix_at(mat2,0,0);
  ASSERT_EQUAL(Matrix_column(mat2,ptr2),0);
  delete mat2;
}

TEST(test_matrix_at)
{
  Matrix *mat1 = new Matrix;
  Matrix_init(mat1,2,2);
  *Matrix_at(mat1,0,0)=1;
  *Matrix_at(mat1,0,1)=2;
  *Matrix_at(mat1,1,0)=3;
  *Matrix_at(mat1,1,1)=4;
  Matrix *mat2 = new Matrix;
  Matrix_init(mat2,2,2);
  *Matrix_at(mat2,0,0)=1;
  *Matrix_at(mat2,0,1)=2;
  *Matrix_at(mat2,1,0)=3;
  *Matrix_at(mat2,1,1)=4;
  ASSERT_TRUE(Matrix_equal(mat1,mat2));
  delete mat1;
  delete mat2;
  Matrix *mat3 = new Matrix;
  Matrix_init(mat3,1,1);
  *Matrix_at(mat3,0,0)=1;
  Matrix *mat4 = new Matrix;
  Matrix_init(mat4,1,1);
  *Matrix_at(mat4,0,0)=1;
  ASSERT_TRUE(Matrix_equal(mat3,mat4));
  delete mat3;
  delete mat4;
}

TEST(test_matrix_fill_border)
{
  Matrix *mat = new Matrix;
  Matrix_init(mat,3,3);
  *Matrix_at(mat,0,0) = 1;
  *Matrix_at(mat,0,1) = 2;
  *Matrix_at(mat,0,2) = 3;
  *Matrix_at(mat,1,0) = 4;
  *Matrix_at(mat,1,1) = 5;
  *Matrix_at(mat,1,2) = 6;
  *Matrix_at(mat,2,0) = 7;
  *Matrix_at(mat,2,1) = 8;
  *Matrix_at(mat,2,2) = 9;
  Matrix_fill_border(mat,2);
  Matrix *filled =new Matrix;
  Matrix_init(filled,3,3);
  *Matrix_at(filled,0,0) = 2;
  *Matrix_at(filled,0,1) = 2;
  *Matrix_at(filled,0,2) = 2;
  *Matrix_at(filled,1,0) = 2;
  *Matrix_at(filled,1,1) = 5;
  *Matrix_at(filled,1,2) = 2;
  *Matrix_at(filled,2,0) = 2;
  *Matrix_at(filled,2,1) = 2;
  *Matrix_at(filled,2,2) = 2;
  ASSERT_TRUE(Matrix_equal(mat,filled));
  delete mat;
  delete filled;
  Matrix *mat2 = new Matrix;
  Matrix_init(mat2,1,1);
  *Matrix_at(mat2,0,0) = 1;
  Matrix_fill_border(mat2,2);
  Matrix *filled2 = new Matrix;
  Matrix_init(filled2,1,1);
  *Matrix_at(filled2,0,0) = 2;
  ASSERT_TRUE(Matrix_equal(mat2,filled2));
  delete mat2;
  delete filled2;
}

TEST(test_matrix_max)
{
  Matrix *mat = new Matrix;
  Matrix_init(mat,3,3);
  *Matrix_at(mat,0,0) = 1;
  *Matrix_at(mat,0,1) = 2;
  *Matrix_at(mat,0,2) = 3;
  *Matrix_at(mat,1,0) = 4;
  *Matrix_at(mat,1,1) = 5;
  *Matrix_at(mat,1,2) = 6;
  *Matrix_at(mat,2,0) = 7;
  *Matrix_at(mat,2,1) = 8;
  *Matrix_at(mat,2,2) = 9;
  ASSERT_TRUE(Matrix_max(mat) == 9);
  Matrix *mat2 = new Matrix;
  Matrix_init(mat2,1,1);
  *Matrix_at(mat2,0,0) = 1;
  ASSERT_TRUE(Matrix_max(mat2) == 1);
  delete mat;
  delete mat2;
}

TEST(test_matrix_min_value_in_row)
{
  Matrix *mat = new Matrix; 
  Matrix_init(mat,3,3);
  *Matrix_at(mat,0,0) = 1;
  *Matrix_at(mat,0,1) = 2;
  *Matrix_at(mat,0,2) = 3;
  *Matrix_at(mat,1,0) = 4;
  *Matrix_at(mat,1,1) = 5;
  *Matrix_at(mat,1,2) = 6;
  *Matrix_at(mat,2,0) = 7;  
  *Matrix_at(mat,2,1) = 8;
  *Matrix_at(mat,2,2) = 9;
  ASSERT_EQUAL(Matrix_min_value_in_row(mat,0,0,2),1);
  ASSERT_TRUE(Matrix_min_value_in_row(mat,1,0,2)==4);
  ASSERT_TRUE(Matrix_min_value_in_row(mat,1,1,2)==5);
  Matrix *mat2 = new Matrix;
  Matrix_init(mat2,2,2);
  *Matrix_at(mat2,0,0) = 1;
  *Matrix_at(mat2,0,1) = 1;
  *Matrix_at(mat2,1,0) = 1;
  *Matrix_at(mat2,1,1) = 1;
  ASSERT_TRUE(Matrix_min_value_in_row(mat,0,0,1)==1);
  Matrix *mat3= new Matrix;
  Matrix_init(mat3,1,1);
  ASSERT_TRUE(Matrix_min_value_in_row(mat,0,0,1)==1)
  delete mat;
  delete mat2;
  delete mat3;
}

TEST(test_matrix_column_of_min_value_in_row)
{
  Matrix *mat = new Matrix; 
  Matrix_init(mat,3,3);
  *Matrix_at(mat,0,0) = 1;
  *Matrix_at(mat,0,1) = 2;
  *Matrix_at(mat,0,2) = 3;
  *Matrix_at(mat,1,0) = 4;
  *Matrix_at(mat,1,1) = 5;
  *Matrix_at(mat,1,2) = 6;
  *Matrix_at(mat,2,0) = 7;
  *Matrix_at(mat,2,1) = 8;
  *Matrix_at(mat,2,2) = 9;
  ASSERT_TRUE(Matrix_column_of_min_value_in_row(mat,0,0,2)==0);
  Matrix *mat2 = new Matrix;
  Matrix_init(mat2,3,3);
  *Matrix_at(mat2,0,0) = 1;
  *Matrix_at(mat2,0,1) = 1;
  *Matrix_at(mat2,0,2) = 1;
  *Matrix_at(mat2,1,0) = 4;
  *Matrix_at(mat2,1,1) = 5;
  *Matrix_at(mat2,1,2) = 6;
  *Matrix_at(mat2,2,0) = 7;
  *Matrix_at(mat2,2,1) = 8;
  *Matrix_at(mat2,2,2) = 9;
  ASSERT_TRUE(Matrix_column_of_min_value_in_row(mat2,0,0,2)==0);
  Matrix *mat3 = new Matrix;
  Matrix_init(mat3,1,1);
  *Matrix_at(mat3,0,0) = 1;
  ASSERT_TRUE(Matrix_column_of_min_value_in_row(mat3,0,0,0)==0);
  delete mat;
  delete mat2;  
  delete mat3;
}



// NOTE: The unit test framework tutorial in Lab 2 originally
// had a semicolon after TEST_MAIN(). Although including and
// excluding the semicolon are both correct according to the c++
// standard, the version of g++ we are using reports a warning
// when there is a semicolon. Therefore do NOT add a semicolon
// after TEST_MAIN()
TEST_MAIN() // Do NOT put a semicolon here
