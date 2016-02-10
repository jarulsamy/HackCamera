i am new in image processing and computer vision and i would like to detect blobs in an image using Laplacian of Gaussian with different scale spaces. The following links explain in detail. http://www.cs.utah.edu/~jfishbau/advimproc/project1/ http://www.cs.utah.edu/~manasi/coursework/cs7960/p1/project1.html So far by using opencv2 i have managed to get the images, apply the Gaussian filter with various kernels and apply the Laplacian filter. The i multiply with sigma squared the whole image to amplify the signal (see description in links) and then i apply a threshhold. The next step is to detect local maxima and minima so i can get the blob center and be able to draw circles, but i am not sure how to do it and whether the image processing i have done so far is correct. Here is my code:

int main(){

    image1 = imread("butterfly.jpg",0);

    drawing1 = imread("butterfly.jpg");

    blobDetect(image1,drawing1);
waitKey();
    return 0;
}

void blobDetect(Mat image, Mat drawing){

    int ksize = 1;
    int n =1;

    Mat result[10];
    for(int i=0; i<10; i++){
    cv::GaussianBlur(image,result[i],cv::Size(ksize,ksize),ksize/3,0);
    n+=1;
    ksize = 2*n-1;
    }

    ksize = 1;
    n =1;
    for(int i=0; i<10; i++){
    cv::Laplacian(result[i],result[i],CV_8U,ksize,1,0);
    n+=1;
        ksize = 2*n-1;
   }

   ksize = 1;
   int cols = image.cols;
   int rows = image.rows;
   for(int a=0; a<10; a++){
    for(int i=0; i<rows; i++){
        //uchar* data = result[a].ptr<uchar>(rows);
        for(int j=0; j<cols; j++){
            result[a].at<uchar>(i,j) *= (ksize/3)*(ksize/3);                
        }   
    }
    ksize++;
    ksize = 2*ksize-1;
}


for(int i=0; i<10; i++){
    cv::threshold(result[i], result[i], 100, 255, 0);
}

}
