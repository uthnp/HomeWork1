/**
@author Raquel Gonzalez
@Date 08-31-2012
**/

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "boost/date_time/posix_time/posix_time.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;

class HomeWork1App : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	void prepareSettings(Settings* settings);
	Color8u gradColor;
	
  private:
	Surface* mySurface_; //The Surface object whose pixel array we will modify
  
	//Track how many frames we have shown, for animation purposes
	int frame_number_;
	boost::posix_time::ptime app_start_time_;

	//Width and height of the screen
	static const int kAppWidth=800;
	static const int kAppHeight=600;
	static const int kTextureSize=1024; //Must be the next power of 2 bigger or equal to app dimensions

	/** This satisfies the "rectangle" requirement, goal A.1
	 */
	void drawRectangles(uint8_t* pixels, int x1, int y1,  int rect_width, int rect_height, Color8u c);
	/** Satisfies the circle requiremtn goal A.2 
	*/
	void drawCircle(uint8_t* pixels, int center_x, int center_y, int r, Color8u c);
	/*Satifies the gradient requiremnt goal A.4
	*/
	void createGradient();

	/**Satifies the tint requirement goal A.6 
	*/
	void tint(uint8_t* pixels);

	/**Satifies the blur requirment goal B.1
	**/
	void blur(uint8_t* pixels);

};

void HomeWork1App::prepareSettings(Settings* settings){
	(*settings).setWindowSize(kAppWidth,kAppHeight);
	(*settings).setResizable(false);
}

//Draws the rectangles for the rectangle requirement.
void HomeWork1App::drawRectangles(uint8_t* pixels, int x1, int y1, int rect_width, int rect_height, Color8u c){
	// cut out the object intermediate since you can just access the color directly - Nick

	for(int x = x1; x<rect_height; x++){
		for(int y = y1; y<rect_width; y++){
	pixels[3*(x + y*kTextureSize)] = c.r;
	pixels[3*(x + y*kTextureSize)+1] = c.g;
	pixels[3*(x + y*kTextureSize)+2] = c.b;
		}

	}
}

//Draws the circles for the circle requirement.
void HomeWork1App::drawCircle(uint8_t* pixels, int center_x, int center_y, int r, Color8u c){
	for(int y=center_y-r; y<=center_y+r; y++){
		for(int x=center_x-r; x<=center_x+r; x++){
			//Bounds test, to make sure we don't access array out of bounds
			if(y < 0 || x < 0 || x >= kAppWidth || y >= kAppHeight) continue;
			// ^that's a nice idea that I wish I had thought to implement XD;; - Nick
					
			int dist = (int)sqrt((double)((x-center_x)*(x-center_x) + (y-center_y)*(y-center_y)));
			if(dist <= r){
					pixels[3*(x + y*kTextureSize)] =  c.r;
					pixels[3*(x + y*kTextureSize)+1] = c.b;
					pixels[3*(x + y*kTextureSize)+2] =  c.g;
				
			}
		}
	}
}

//Creates the gradient for the gradient requirement.
void HomeWork1App::createGradient(){
		//Satisfies requirment gradient requirement A4
	if(gradColor.r<255) gradColor.r = gradColor.r + 10;
	if(gradColor.r>255) gradColor.r = gradColor.r - 10;
	if(gradColor.b<255) gradColor.b = gradColor.b + 10;
	if(gradColor.b>255) gradColor.b = gradColor.b - 10;
	if(gradColor.g<255) gradColor.g = gradColor.g + 10;
	if(gradColor.g>255) gradColor.g = gradColor.g - 10;
}
//Takes in the array of pixels and sets the blue color in each one to change the color
void HomeWork1App::tint(uint8_t* pixels){
	// an off-by-one error was mixing up the g and b values. the function should be working as intended now. - Nick
	Color8u c = Color8u(0,0,100); // was originally setting b = 0. - Nick
	for(int col = 0; col<800; col++){
		for(int row = 0;  row<600; row++){
			pixels[3*(col + row*kTextureSize)+2] = c.b;
		}
	}
}

//applies the blur for the blur requirement.
void HomeWork1App::blur(uint8_t* pixels){
	// changed it so that the size of the frame could be changed without any problems if someone wanted - Nick
	for(int col = 2; col<(kAppWidth-1); col++){
		for(int row = 2;  row<(kAppHeight-1); row++){
	
				for (int i = 0; i < 3; i++){ 
					// put the math in one for loop for readability
					// also spaced out the equation so it can all be seen at once. - Nick
					pixels[3*(col + row*kTextureSize)+i] = (pixels[3*(col + row*kTextureSize)+i] + 
						pixels[3*((col+1) + row*kTextureSize)+i] + 
						pixels[3*((col+1) + (row+1)*kTextureSize)+i] + 
						pixels[3*((col) + (row+1)*kTextureSize)+i] + 
						pixels[3*((col-1) + (row+1)*kTextureSize)+i] + 
						pixels[3*((col-1) + (row-1)*kTextureSize)+i] + 
						pixels[3*((col) + (row-1)*kTextureSize)+i])/9;
				}
		}
	}
}

void HomeWork1App::setup()
{
	frame_number_=0;
		
   //This is the setup that everyone needs to do
	mySurface_ = new Surface(kTextureSize,kTextureSize,false);

	//Set up for the gradient in the circles requiremnt A4
	gradColor = Color8u(132,10,10);
}

void HomeWork1App::mouseDown( MouseEvent event )
{
	//Get our array of pixel information
	uint8_t* dataArray = (*mySurface_).getData();
	blur(dataArray);
}

void HomeWork1App::update()
{
	//Get our array of pixel information
	uint8_t* dataArray = (*mySurface_).getData();

	//Tints the background and every rectangle after they pop-up. So they flash another color and then tint a purplish blue.
	tint(dataArray);

	//creates the random colors for each rectangle on the screen.
	int red = (rand() % 200 + 50);
	int blue = (rand() % 200 + 50);
	int green = (rand() % 200 + 50);

	Color8u fill1 = Color8u(red,green,blue);

	//creates random dimension for each rectangle.
	int xValue = (rand() % 400 + 90);
	int yValue = (rand() % 400 + 90);
	int width = (rand() % 400 + 100);
	int height = (rand() % 400 + 100);
	
	//draws the rectangles.
	drawRectangles(dataArray,xValue,yValue,width,height, fill1);
	
//Fulfills the gradient requirment A4 inside the circles.
//Variable initialization needed to create the gradient circle

	//Draws a row of circles.
	int circleX = 10;
	int circleY = 60;
	while(circleX<800){
		createGradient();
		drawCircle(dataArray, circleX, 20, 10, gradColor);
		drawCircle(dataArray, circleX, 570, 10, gradColor);
		circleX = circleX + 40;
	}
	while(circleY<560){
		createGradient();
		drawCircle(dataArray, 10, circleY, 10, gradColor);
		drawCircle(dataArray, 770, circleY, 10, gradColor);
		circleY = circleY + 40;
	}
	
 //Only save the first frame of drawing as output
	if(frame_number_ == 0){
		writeImage("raquelImage.png",*mySurface_);
		//We do this here, instead of setup, because we don't want to count the writeImage time in our estimate
		app_start_time_ = boost::posix_time::microsec_clock::local_time();
	}

	//keeps track of how many frames we have shown.
	frame_number_++;
	
	//For debugging: Print the actual frames per second
	boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
	boost::posix_time::time_duration msdiff = now - app_start_time_;
    console() << (1000.0*frame_number_) / msdiff.total_milliseconds() << std::endl;
	

}

void HomeWork1App::draw()
{
	
	gl::draw(*mySurface_);
	 
}

CINDER_APP_BASIC( HomeWork1App, RendererGl )
