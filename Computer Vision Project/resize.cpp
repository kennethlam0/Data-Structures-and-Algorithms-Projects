// Project UID af1f95f547e44c8ea88730dfb185559d

#include <iostream>
#include <cstdlib>
#include <fstream>
#include "Image.h"
#include "processing.h"
using namespace std;

int main(int argc, char *argv[]) 
{
 if(argc!=4 && argc!=5)
  {
    cout << "Usage: resize.exe IN_FILENAME OUT_FILENAME WIDTH [HEIGHT]\n"
     << "WIDTH and HEIGHT must be less than or equal to original" << endl;
     return 1;
  }
  string in_filename = argv[1];
  string out_filename = argv[2];
  int width = stoi(argv[3]);
  int height;
  ifstream fin; 
  fin.open(in_filename.c_str());
  if(!fin.is_open())
  {
    cout << "Error opening file: " << in_filename << endl;
    return 1;
  }
  Image *img = new Image; 
  Image_init(img, fin);
  int original_width  = Image_width(img);
  int original_height = Image_height(img);
  if(argc==5)
  {
    height = stoi(argv[4]);
  }
  else
  {
    height = original_height;
  }
  if(width <= 0 || width > original_width || height <= 0 || height > original_height)
  {
    cout << "Usage: resize.exe IN_FILENAME OUT_FILENAME WIDTH [HEIGHT]\n"
     << "WIDTH and HEIGHT must be less than or equal to original" << endl;
     fin.close();
     return 1; 
  }
  ofstream fout(out_filename.c_str()); 
  if(!fout.is_open())
  {
    cout << "Error opening file: " << out_filename << endl;
    fout.close();
    return 1;
  }
  seam_carve(img, width, height);
  Image_print(img, fout);
  fin.close();
  fout.close();
  delete img;
}