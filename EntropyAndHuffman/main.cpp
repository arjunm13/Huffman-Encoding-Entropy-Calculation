// Ryerson computer engineering - Multimedia ELE 725
// Developer: ARJUN MAHENDRAN

// This code has two functions
// Calculate the entropy of the symbols within an image
// Creates a huffman encoding tree table

#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <stdio.h>
#include <vector>
#include <map>

#define N 256
#define M 4
#define P (2*N) -1
#define weight 0
#define parent 1
#define lchild 2
#define rchild 3
#define MAX 345600

int i1=MAX, i2=MAX, k=N, rightChildCounter=0;



using namespace cv;
using namespace std;



// computes total Entropy for image
// formula can be seen here : https://en.wikipedia.org/wiki/Entropy_(information_theory)

// this function returns entropy of entire image(Theoretical BITS NEEDED TO ENCODE symbols of entire image)
map<char,double> myEntropy(vector<char> input){
    
    map<char,double> entropy;
    double normalize = 1 / (double)input.size();
    
    for(int counter : input){
        if (entropy[input[counter]]==0){
            entropy[input[counter]]=normalize;
        } else {
            entropy[input[counter]] +=normalize;
        }
    }
    return entropy;
}


// This function calculates and prints out a table
// describing the normalized value of the entropy of each symbol
// example. if there are 4 symbols A B C D  and there is a set [ACBD] each will have an entropy of 0.25
double entropy(map<char, double> &myEntropy){
    double result = 0.0;
    for(auto elem : myEntropy){
        cout<< +elem.first << " " << +elem.second << "\n";
        result =  result + (elem.second*(log2(1/(elem.second))));
    }
    return result;
}

// struct to store the three channels needed for an image
struct colorImage{
    Mat ch1 ;
    Mat ch2 ;
    Mat ch3 ;
};

//Opens an image file and saves it as an array of three channels for RGB

colorImage getRGB(string input, int type){
    colorImage out;
    Mat image;
    image = imread(input, type);
    vector<Mat> channels(3);
    split (image,channels);
    
    out.ch1 = channels[0]; //R - channel
    out.ch2 = channels[1]; // G - channel
    out.ch3 = channels[2]; //B - channel
    
    return out;
}

//Opens an image file and saves it as an array of three channels for YCrCb
colorImage getYCrCb(string input, int type){
    colorImage out;
    
    Mat image, imageout;
    image = imread(input, type);
    
    cvtColor(image, image, CV_BGR2YCrCb);
    
    vector<Mat> channels(3);
    split (image,channels);
    
    out.ch1 = channels[0]; // Y-channel
    out.ch2 = channels[1]; // Cr-channel
    out.ch3 = channels[2]; // Cb-channel
    
    return out;
}

int main(int ac, char** av) {
    
    /* Entropy Function can be applied to a string using the below logic
    string input = "ONESMARTFELLAHEFELTSMARTTWOSMARTFELLASTHEYFELTSMART";
    vector<char> inputvec(input.begin(), input.end());
    */
    
    map<char,double> mentropy; // stores entropy table for all symbols
    double entValue;  // stores entire image's theoretical entropy value
    
    
    // START OF PART 1
    colorImage sample = getRGB("/Users/arjun/Documents/4th year/semester1/Networks/lab3/lab3/box_in_scene.png", CV_LOAD_IMAGE_COLOR);

    // turn MAT to Vector
    vector<char> ch3array;
    ch3array.assign(sample.ch1.datastart, sample.ch1.dataend);

     //PART 1

    // use entropy function
    mentropy = myEntropy(ch3array);
    entValue = entropy(mentropy);

    // print out total entropy of image
    cout <<"Entropy is :"<< entValue << "\n" << "press anykey on image to exit";
    
    //display image

    namedWindow( "Display window", WINDOW_AUTOSIZE ); // Create a window for display.
    imshow( "Display window", sample.ch1 );                // Show our image inside it.
    waitKey(0); // Wait for a keystroke in the window

    cout << "EXITTING..";
}