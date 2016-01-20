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

// Tree initialization requires custom type
typedef int array_type[P][M];

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


// initiliazes huffman tree for symbols in the image
// stores each symbol and increments it value each time it occurs in the image
// ** A symbol is just the value of the pixel in an image

void HuffTreeInit(Mat image, array_type& output){
    
    int index;
    vector<int> charray;
    charray.assign(image.datastart, image.dataend);
    
    
    
    for (int counterinit =0; counterinit < charray.size(); counterinit++){
        
        output[charray[counterinit]][0] = output[charray[counterinit]][0] + 1;
        if (output[charray[counterinit]][0] > 1){
            output[charray[counterinit]][1] = -1;
            output[charray[counterinit]][2] = -1;
            output[charray[counterinit]][3] = -1;
        }
    }
}

void HuffTreeNodeInit(array_type& output){
    for (int counter=0;counter<k;counter++){
        if(output[counter][weight]<i1&&output[counter][weight]>0){
            i1 = counter;
        }
    }
    output[i1][parent]=k;
    output[k][parent] = -1;
    output[k][lchild]= i1;
    
    
    for (int counter1 =0;counter1<k;counter1++){
        if(output[counter1][weight]<i2 &&output[counter1][weight]>0 && counter1!=i1){
            i2 = counter1;
        }
    }
    output[i2][parent]=k;
    output[k][rchild]= i2;
    output[k][weight] = output[i1][weight]+output[i2][weight];
    k++;
}

void HuffTreeNode(array_type& output){
    i1=MAX;i2=MAX;
    for (int counter=0;counter<k;counter++){
        if(output[counter][weight]>0&&output[counter][parent]==-1 && output[counter][weight]<i1){
            i1 = counter;
        }
    }
    output[i1][parent]=k;
    output[k][parent] = -1;
    output[k][lchild]= i1;
    
    
    for (int counter1 =0;counter1<k;counter1++){
        if(output[counter1][weight]>0 && counter1!=i1&&output[counter1][parent]==-1&& output[counter1][weight]<i2){
            i2 = counter1;
        }
    }
    output[i2][parent]=k;
    output[k][rchild]= i2;
    output[k][weight] = output[i1][weight]+output[i2][weight];
    
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
    
    // PART 1 ENTROPY CALUCLATION
    
    /* Entropy Function can be applied to a string using the below logic
    string input = "ONESMARTFELLAHEFELTSMARTTWOSMARTFELLASTHEYFELTSMART";
    vector<char> inputvec(input.begin(), input.end());
    */
    
    map<char,double> mentropy; // stores entropy table for all symbols
    double entValue;  // stores entire image's theoretical entropy value
    
    
    // START OF PART 1
    colorImage sample = getRGB("INSERT IMAGE LOCATION", CV_LOAD_IMAGE_COLOR);

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
    
    
//// PART 2 -- Huffman Table
//    int tree[P][M];
//    memset(tree, -1, sizeof(tree));
//    
//    // Read and split channels, NOT using Methods
//    Mat image;
//    image = imread("INSERT IMAGE LOCATION", CV_LOAD_IMAGE_COLOR);
//    vector<Mat> channels(3);
//    split (image,channels);
//    map<char, string> table;
//    int left, right, nextparent, nextleft, nextright, levelleft=0, levelright=0;
//    char code1[40], code2[40] ;
//    left =0;right=0;
//    
//    Mat ch1 = channels[0];
//    
//    HuffTreeInit(ch1, tree);
//    HuffTreeNodeInit(tree);
//    while (k<442){
//        k++; HuffTreeNode(tree);
//    }
//    
//    
//    
//    for (int counter2 =P-1; counter2 >0; counter2--){
//
//        levelleft++;
//        levelright++;
//        if (tree[counter2][0]!=-1){
//            if (tree[tree[counter2][2]][2]!=-1){
//                  cout << counter2 <<" "<< tree[counter2][0] << " " << tree[counter2][1]<< " " << tree[counter2][2]<< " " << tree[counter2][3]<<"\n";
//                levelright=0;levelright=0;
//                
//            }
//        }
//        
//    }
    
    
    
    
}