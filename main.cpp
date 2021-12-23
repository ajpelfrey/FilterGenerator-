#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

class FileTime {
public:

  struct Header {
    char IDLength;
    char colorMapType;
    char dataTypeCode;
    short colorMapOrigin;
    short colorMapLength;
    char colorMapDepth;
    short xOrigin;
    short yOrigin;
    short width;
    short height;
    char bitsPerPixel;
    char imageDescriptor;
  };
  struct Pixel {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
  };
  vector<Pixel> pixelVect;
  Header header;
};
void getHeaderInfo(FileTime &temp, FileTime &temp1)
{
  temp.header.IDLength = temp1.header.IDLength; // get header info
  temp.header.colorMapType = temp1.header.colorMapType;
  temp.header.dataTypeCode = temp1.header.dataTypeCode;
  temp.header.colorMapOrigin = temp1.header.colorMapOrigin;
  temp.header.colorMapLength = temp1.header.colorMapLength;
  temp.header.colorMapDepth = temp1.header.colorMapDepth;
  temp.header.xOrigin = temp1.header.xOrigin;
  temp.header.yOrigin = temp1.header.yOrigin;
  temp.header.width = temp1.header.width;
  temp.header.height = temp1.header.height;
  temp.header.imageDescriptor = temp1.header.imageDescriptor;
  temp.header.bitsPerPixel = temp1.header.bitsPerPixel;
}

void read(FileTime &temp, string inFileName)
{

  FileTime::Pixel pixelObj = FileTime::Pixel();
  ifstream inFile(inFileName, ios_base::binary);

  inFile.read((char *)&temp.header.IDLength, 1);
  inFile.read((char *)&temp.header.colorMapType, 1);
  inFile.read((char *)&temp.header.dataTypeCode, 1);
  inFile.read((char *)&temp.header.colorMapOrigin, 2);
  inFile.read((char *)&temp.header.colorMapLength, 2);
  inFile.read((char *)&temp.header.colorMapDepth, 1);
  inFile.read((char *)&temp.header.xOrigin, 2);
  inFile.read((char *)&temp.header.yOrigin, 2);
  inFile.read((char *)&temp.header.width, 2);
  inFile.read((char *)&temp.header.height, 2);
  inFile.read((char *)&temp.header.bitsPerPixel, 1);
  inFile.read((char *)&temp.header.imageDescriptor, 1);

  for (int i = 0; i < (int)(temp.header.height * temp.header.width); i++) {
    inFile.read((char *)&pixelObj.blue, 1);
    inFile.read((char *)&pixelObj.green, 1);
    inFile.read((char *)&pixelObj.red, 1);
    temp.pixelVect.push_back(pixelObj);
  }
  inFile.close();
}

void OutputFile(FileTime &temp, string inFileName)
{

  ofstream inFile(inFileName, ios_base ::binary);
  inFile.write((char *)&temp.header.IDLength, 1);
  inFile.write((char *)&temp.header.colorMapType, 1);
  inFile.write((char *)&temp.header.dataTypeCode, 1);
  inFile.write((char *)&temp.header.colorMapOrigin, 2);
  inFile.write((char *)&temp.header.colorMapLength, 2);
  inFile.write((char *)&temp.header.colorMapDepth, 1);
  inFile.write((char *)&temp.header.xOrigin, 2);
  inFile.write((char *)&temp.header.yOrigin, 2);
  inFile.write((char *)&temp.header.width, 2);
  inFile.write((char *)&temp.header.height, 2);
  inFile.write((char *)&temp.header.bitsPerPixel, 1);
  inFile.write((char *)&temp.header.imageDescriptor, 1);

  for (int i = 0; i < (int)temp.pixelVect.size(); i++) {
    inFile.write((char *)&temp.pixelVect[i].blue, 1);
    inFile.write((char *)&temp.pixelVect[i].green, 1);
    inFile.write((char *)&temp.pixelVect[i].red, 1);
  }
}

FileTime Multiply(FileTime &temp, FileTime &temp1)
{
  FileTime temp2;
  getHeaderInfo(temp2, temp);

  for( int i = 0; i < temp.header.height * temp.header.height; i ++)
  {
    FileTime::Pixel pixelObj;

    pixelObj.red = (((((temp.pixelVect[i].red) / 255.f) *
                      ((temp1.pixelVect[i].red) / 255.f)) *
                     255) +
                    .5);
    pixelObj.green = ((((((temp.pixelVect[i].green) / 255.f)) *
                        ((temp1.pixelVect[i].green) / 255.f)) *
                       255) +
                      .5);
    pixelObj.blue = ((((((temp.pixelVect[i].blue) / 255.f)) *
                       ((temp1.pixelVect[i].blue) / 255.f)) *
                      255) +
                     .5);
    temp2.pixelVect.push_back(pixelObj);
  }
  return temp2;
}

FileTime Subtract(FileTime &temp, FileTime &temp1)
{

  FileTime temp2;
  getHeaderInfo(temp2, temp);

  for (int i = 0; i < (temp2.header.height * temp2.header.width); i++)
  {
    FileTime::Pixel pixelObj;
    float tempred = (temp.pixelVect[i].red) - (temp1.pixelVect[i].red);
    float tempgreen = (temp.pixelVect[i].green) - (temp1.pixelVect[i].green);
    float tempblue = (temp.pixelVect[i].blue) - (temp1.pixelVect[i].blue);

    if (tempred < 0) { // cap at minimum
      tempred = 0;
    }
    if (tempgreen < 0) {
      tempgreen = 0;
    }
    if (tempblue < 0) {
      tempblue = 0;
    }
    pixelObj.red = tempred;
    pixelObj.green = tempgreen;
    pixelObj.blue = tempblue;
    temp2.pixelVect.push_back(pixelObj);
  }
  return temp2;
}

FileTime Rotate(FileTime &temp)
{
  FileTime temp2;
  getHeaderInfo(temp2, temp);
  FileTime::Pixel pixelObj;

  for (int i = (temp.header.height * temp.header.width-1); i >=0; i--) { // iteration in reverse
    pixelObj.red = temp.pixelVect[i].red;
    pixelObj.green = temp.pixelVect[i].green;
    pixelObj.blue = temp.pixelVect[i].blue;
    temp2.pixelVect.push_back(pixelObj);
  }
  return temp2;
}

FileTime overLay(FileTime &temp, FileTime &temp1)
{

  FileTime temp2;
  getHeaderInfo(temp2, temp1);
  for (int i = 0; i < (temp2.header.height * temp2.header.width); i++) {
    FileTime::Pixel pixelObj;
    if ((float)temp1.pixelVect[i].red / 255 <= 0.5) // 2*a*b
    {
      pixelObj.red = ((2 * ((float)temp.pixelVect[i].red / 255) *
                       ((float)temp1.pixelVect[i].red / 255))) *
                     255 +
                     .5;
    } else { //1-2*(1-A)*(1-B)
      pixelObj.red = (1 - (2 * (1 - (float)(temp.pixelVect[i].red) / 255) *
                           (1 - (float)(temp1.pixelVect[i]).red / 255))) *
                     255 +
                     .5;

    }
    if ((float)temp1.pixelVect[i].green / 255 <= 0.5) {
      pixelObj.green = ((2 * ((float)temp.pixelVect[i].green / 255) *
                         ((float)temp1.pixelVect[i].green / 255))) *
                       255 +
                       .5;
    } else {
      pixelObj.green =
          (1 - ((2 * (1 - (float)(temp.pixelVect[i].green) / 255) *
                 (1 - (float)(temp1.pixelVect[i]).green / 255)))) *
          255 +
          .5;
    }
    if ((float)temp1.pixelVect[i].blue / 255 <= 0.5) {
      pixelObj.blue = ((2 * ((float)temp.pixelVect[i].blue / 255) *
                        ((float)temp1.pixelVect[i].blue / 255))) * 255 +
                      .5;

    } else {
      pixelObj.blue = (1 - (2 * (1 - (float)(temp.pixelVect[i].blue) / 255) *
                            (1 - (float)(temp1.pixelVect[i]).blue / 255))) *
                      255 +
                      .5;
    }

    temp2.pixelVect.push_back(pixelObj);
  }
  return temp2;

}
FileTime GreenUp(FileTime &temp) //only taking in 1 file
{
  FileTime temp2;
  float NewGreen;
  getHeaderInfo(temp2, temp);
  FileTime::Pixel pixelObj;

  for (int i = 0; i < (temp2.header.height * temp2.header.width); i++) {
    NewGreen = temp.pixelVect[i].green + 200; //add 200
    if (NewGreen > 255) //cap at max
    {
      NewGreen = 255;
    }
    pixelObj.green = NewGreen;
    pixelObj.red = temp.pixelVect[i].red;
    pixelObj.blue = temp.pixelVect[i].blue;
    temp2.pixelVect.push_back(pixelObj);
  }
  return temp2;
}

FileTime RedUp(FileTime &temp) { //same as GreenUp, with blue being 0s
  FileTime temp2;
  FileTime::Pixel pixelObj;
  float Newred;
  getHeaderInfo(temp2, temp);
  for (int i = 0; i < (temp2.header.height * temp2.header.width); i++) {

    Newred = temp.pixelVect[i].red * 4;
    if (Newred > 255) // cap at max
    {
      Newred = 255;
    }
    pixelObj.red = Newred;
    pixelObj.blue = 0;
    pixelObj.green = temp.pixelVect[i].green;
    temp2.pixelVect.push_back(pixelObj);
  }
  return temp2;
}
FileTime screen(FileTime &temp1, FileTime &temp2)
{
  FileTime temp4;
  getHeaderInfo(temp4, temp1);
  for (int i = 0; i < (temp4.header.height * temp4.header.width);
       i++) {
    float red, green, blue;
    FileTime::Pixel tempPixel; //1-(1-A)*(1-B)
    red = ((1 - (1 - (float)temp1.pixelVect[i].red / 255) *
                (1 - (float)(temp2.pixelVect[i].red) / 255)) *
           255 +
           .5);
    green = ((1 - (1 - ((float)temp1.pixelVect[i].green / 255)) *
                  (1 - (float)(temp2.pixelVect[i].green) / 255)) *
             255 +
             .5);
    blue = ((1 - (1 - ((float)temp1.pixelVect[i].blue / 255)) *
                 (1 - (float)(temp2.pixelVect[i].blue) / 255)) *
            255 +
            .5);
    tempPixel.red = red;
    tempPixel.green = green;
    tempPixel.blue = blue;
    temp4.pixelVect.push_back(tempPixel);
  }
  return temp4;
}

bool Test(FileTime &temp, FileTime &temp1) {
  bool tester = true;
  string confirm = "confirm";
  for (int i = 0; i < temp.header.width * temp.header.height; i++) {
    if (temp.pixelVect[i].red != temp1.pixelVect[i].red) {
      tester = false;
    }
    if (temp.pixelVect[i].green != temp1.pixelVect[i].green) {
      tester = false;
    }
    if (temp.pixelVect[i].blue != temp1.pixelVect[i].blue) {
      tester = false;
    }
  }

  return tester;
}


int main() {

  FileTime layer1Temp; //#1
  FileTime pattern1Temp;

  read(layer1Temp, "input/layer1.tga");
  read(pattern1Temp,
       "input/pattern1.tga");
  FileTime part1 = Multiply(layer1Temp, pattern1Temp);
  OutputFile(part1,
             "output/part1.tga");
  FileTime example1; //for testing
  read(example1,
       "examples/EXAMPLE_part1.tga");


  FileTime layer2; // #2
  FileTime car;

  read(layer2, "input/layer2.tga");
  read(car, "input/car.tga");

  FileTime part2 = Subtract(car, layer2);
  OutputFile(part2, "output/part2.tga");

  FileTime example2; // #2 test
  read(example2,
       "examples/EXAMPLE_part2.tga");

  FileTime pattern2;
  FileTime blender;
  read(pattern2, "input/pattern2.tga");
  blender = Multiply(layer1Temp, pattern2);
  FileTime texttga;
  read(texttga, "input/text.tga");
  FileTime part3;
  part3 = screen(blender, texttga);
  OutputFile(part3, "output/part3.tga");
  FileTime example3;
  read(example3, "examples/EXAMPLE_part3.tga");

  FileTime circles;

  FileTime text;
  FileTime part4;
  FileTime tempResult;
  FileTime example4;// #4
  read(pattern2, "input/pattern2.tga");

  Multiply(layer1Temp, pattern2);

  read(text, "input/text.tga");


  read(circles, "input/circles.tga");

  tempResult = Multiply(layer2, circles);

  part4 = Subtract(tempResult, pattern2);
  OutputFile(
      part4, "output/part4.tga");

  read(example4,
       "examples/EXAMPLE_part4.tga");

  FileTime part5;
  FileTime example5;// #5
  read(pattern1Temp,"input/pattern1.tga");
  read(layer1Temp, "input/layer1.tga");
  part5 = overLay(layer1Temp, pattern1Temp);
  OutputFile(part5, "output/part5.tga");
  read(example5,
       "examples/EXAMPLE_part5.tga");

  FileTime part6;
  FileTime example6;//#6
  read(car, "input/car.tga");
  part6 = GreenUp(car);
  OutputFile(part6, "output/part6.tga");

  read(example6,
       "examples/EXAMPLE_part6.tga");


  FileTime part7 = RedUp(car); //#7
  OutputFile(part7, "output/part7.tga");
  FileTime example7;
  read(example7,
       "examples/EXAMPLE_part7.tga");

  FileTime part81; //#8
  FileTime part82;
  FileTime part83;
  read(part81, "input/car.tga");
  for (int i = 0; i < part81.header.width * part81.header.height; i++) {
    part81.pixelVect[i].blue = part81.pixelVect[i].red; //converts to red
    part81.pixelVect[i].green = part81.pixelVect[i].red;
    part81.pixelVect[i].red = part81.pixelVect[i].red;
  }
  OutputFile(part81, "output/part8_r.tga");
  read(part82, "input/car.tga");
  for (int i = 0; i < part82.header.width * part82.header.height; i++) { //converts to green
    part82.pixelVect[i].blue = part82.pixelVect[i].green;
    part82.pixelVect[i].red = part82.pixelVect[i].green;
    part82.pixelVect[i].green = part82.pixelVect[i].green;
  }
  OutputFile(part82, "output/part8_g.tga");
  read(part83, "input/car.tga");
  for (int i = 0; i < part83.header.width * part83.header.height; i++) { //converts to blue
    part83.pixelVect[i].green = part83.pixelVect[i].blue;
    part83.pixelVect[i].red = part83.pixelVect[i].blue;
    part83.pixelVect[i].blue = part83.pixelVect[i].blue;
  }
  OutputFile(part83, "output/part8_b.tga");

  FileTime blue; //for testing
  FileTime green;
  FileTime red;

  read(blue,
       "examples/EXAMPLE_part8_b.tga");
  read(green,
       "examples/EXAMPLE_part8_g.tga");
  read(red,
       "examples/EXAMPLE_part8_r.tga");

  FileTime Red1; //9
  FileTime Green1;
  FileTime Blue1;
  FileTime part9;
  read(Red1, "input/layer_red.tga");
  read(Green1,
       "input/layer_green.tga");
  read(Blue1, "input/layer_blue.tga");
  read(part9, "input/layer_red.tga");

  for (int i = 0; i < part9.header.width * part9.header.height; i++) {
    part9.pixelVect[i].red = Red1.pixelVect[i].red;
    part9.pixelVect[i].green = Green1.pixelVect[i].green;
    part9.pixelVect[i].blue = Blue1.pixelVect[i].blue;
  }

  OutputFile(part9, "output/part9.tga");
  FileTime example9;
  read(example9,
       "examples/EXAMPLE_part9.tga");

  FileTime example10;
  FileTime part101;
  read(example10,
       "examples/EXAMPLE_part10.tga");
  read(part101, "output/part10.tga");
  FileTime part10; //#10
  FileTime word;
  read(word, "input/text2.tga");
  part10 = Rotate(word);
  OutputFile(part10, "output/part10.tga");

  FileTime EC1;  FileTime EC2;  FileTime EC3;  FileTime EC4;FileTime EC5;
  read(EC1,"input/car.tga");
  read(EC2,"input/circles.tga" );
  read(EC3,"input/pattern1.tga" );
  read(EC4 ,"input/text.tga" );


  if (Test(example1, part1))
  { //test #1
    cout << "Test1 Passed" << endl;
  }
  if (Test(example2, part2))
  {
    cout << "Test2 Passed" << endl;
  }
  if (Test(example3, part3)) {
    cout << "Test3 Passed" << endl;
  }
  if (Test(example4, part4))
  { // #4 test
    cout << "Test4 Passed" << endl;
  }
  if (Test(example5, part5)) { // #5 test
    cout << "Test5 Passed" << endl;
  }
  if (Test(example6, part6)) {  //#6 test
    cout << "Test6 Passed" << endl;
  }

  if (Test(example7, part7)) { //#7 test
    cout << "Test7 Passed" << endl;
  }
  if (Test(blue, part83)) { //#8 test
    cout << "Test8a Passed" << endl;
  }
  if (Test(green, part82)) {
    cout << "Test8b Passed" << endl;
  }
  if (Test(red, part81)) {
    cout << "Test8c Passed" << endl;
  }

  if (Test(example9, part9)) { //#9 test
    cout << "Test9 Passed" << endl;
  }

  if (Test(example10, part10)) { //#10 test
    cout << "Test10 Passed" << endl;
  }
  return 0;
}
