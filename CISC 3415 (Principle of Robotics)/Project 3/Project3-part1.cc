/*
*  blobs.cc
* 
*  The beginnings of a controller that uses the blobfinder.
*
*  Created:     Simon Parsons
*  Date:        7th October 2011
*
*
*  DONGRYUL KIM & WILSON TAN
*  Date:	NOV 3rd, 2020
*/


#include <iostream>
#include <cstdlib>
#include <libplayerc++/playerc++.h>
#include <stdint.h>

int main(int argc, char *argv[]) {  
using namespace PlayerCc;  

	// Set up proxies
	PlayerClient    robot("localhost");  
	BumperProxy     bp(&robot,0);  
	Position2dProxy pp(&robot,0);
	PlayerClient    blobfinder("localhost");
	BlobfinderProxy bf(&blobfinder,0);

	// Variables

	player_blobfinder_data_t  blobList;   // Structure holding the blobs found
	player_blobfinder_blob_t* theBlobs;   // A point to a list of blobs
	player_blobfinder_blob_t  myBlob;     // A single blob

	// Allow the program to take charge of the motors (take care now)
	pp.SetMotorEnable(true);


// Control loop
while(true) {    
	double turnrate, speed;

	// Read from the proxies
	robot.Read();
	blobfinder.Read();

	//if robot finds blobs, do
	if(bf.GetCount() >0) {
		double largearea = 0;
		int indicator = -1;

		for(int i=0; i<bf.GetCount(); i++) {
			//if color of blobs are not red, keep hunting
			if((short)bf.GetBlob(i).color!=0) {
				continue;
			}
			//if color of blobs are red
			//compare each blobs' area to find biggest blob
			if(largearea<bf.GetBlob(i).area) {
				largearea = bf.GetBlob(i).area;
				indicator = i;
			}
		}
		//print the informations 
		std::cout << std::endl;
		std::cout << "Id: "    << bf.GetBlob(indicator).id    << std::endl;
		std::cout << "Color: " << (short)bf.GetBlob(indicator).color << std::endl;
		std::cout << "Area: "  << bf.GetBlob(indicator).area  << std::endl;
		std::cout << "X: "     << bf.GetBlob(indicator).x     << std::endl;
		std::cout << "Y: "     << bf.GetBlob(indicator).y     << std::endl;
		std::cout << std::endl;

		/*
		 *modify blobs position in sensor
		 *camera has a resolution of 320X240
		 *set robot places blobs in the middle of its camera
		*/

		//if the blob is locating near left side(0~130), turn left
		if(bf.GetBlob(indicator).x < 130) {
			turnrate = 0.1;
		}
		//if the blob is lacating near right side(190~320), turn right.
		else if (bf.GetBlob(indicator).x > 190) {
			turnrate = -0.1;
		}
		//if tthe blob placed [130,190], the robot goes to the blob
		else {
			turnrate = 0.0;
			//robot keep moving until the area of blob >= 20000
			if (bf.GetBlob(indicator).area < 20000) {
				speed = 0.1;
			}
			else {
				speed = 0;
			}
		}
	}

	//hunting mode
	//if robot cannot find blobs, turn until it detects blob
	else {
		speed = 0;
		turnrate = 0.4;
		
	}
	pp.SetSpeed(speed, turnrate);

	} // end of while(true) loop
}


