/**
* real-local.cc
* 
* Sample code for a robot that has two front bumpers and a laser scanner,
* and. Suitable for use with the roomba.
*
* This version is intended to run with the AMCL localization proxy,
* which provides multiple hypotheses.
*
* Written by: Simon Parsons
* Date:       22nd November 2009
*  
**/


#include <iostream>
#include <fstream>
#include <libplayerc++/playerc++.h>
using namespace PlayerCc;  

/**
* Function headers
*
**/

player_pose2d_t readPosition(LocalizeProxy& lp);
void printLaserData(LaserProxy& sp);
void printRobotData(BumperProxy& bp, player_pose2d_t pose);

/**
* main()
*
**/

int main(int argc, char *argv[]) {  

	// Variables
	int mc = 0;
	int bumper = 40;
	int counter = 0;
	int hpct = 0;
	double speed;            // How fast do we want the robot to go forwards?
	double turnrate;         // How fast do we want the robot to turn?
	player_pose2d_t  pose;   // For handling localization data
	player_laser_data laser; // For handling laser data
	std::ofstream ofs;
  	ofs.open("log.txt");

	// Set up proxies. These are the names we will use to connect to 
	// the interface to the robot.
	PlayerClient    robot("localhost");  
	BumperProxy     bp(&robot,0);  
	Position2dProxy pp(&robot,0);
	LocalizeProxy   lp (&robot, 0);
	LaserProxy      sp (&robot, 0);

	// Allow the program to take charge of the motors (take care now)
	pp.SetMotorEnable(true);

	// Main control loop
	while(true) {    
		// Update information from the robot.
		robot.Read();
		// Read new information about position
		pose = readPosition(lp);
		// Print information about the laser. Check the counter first to stop
		// problems on startup
		if(counter > 2){
			printLaserData(sp);
		}

		// Print data on the robot to the terminal
		printRobotData(bp, pose);

		hpct = lp.GetHypothCount();

		// If either bumper is pressed
		// set bumper counter to 0
		if(bp[0]==1 || bp[1]==1) {
			bumper = 0;
		}
		// if bumper counter is between 0 to 10, it moves back and turn
		if(bumper >= 0 && bumper <10) {
			speed = -0.5;
			turnrate = -0.2;
		}
		// after moving back, it just goes straight		
		if(bumper >= 10 && bumper<40) {
			speed = 1;
			turnrate = 0;

		}
		// after 500 times of loop find the best accurated position
		// and if the position has over 99% certification, done
		else if(mc > 1000 && hpct <=2){
			player_localize_hypoth hypo;
			player_pose2d_t pose;
			double acc = 0;
			int j = 0;
			for (int i=0; i<hpct; i++) {
				hypo = lp.GetHypoth(i);
				pose = hypo.mean;
				if(hypo.alpha>acc) {
					acc = hypo.alpha;
					j = i;
				}
			}
			hypo = lp.GetHypoth(j);
			pose = hypo.mean;
			std::cout << "The most accurate hypothesis is" << std::endl;
      			std::cout << "X: " << pose.px  << std::endl;
			std::cout << "Y: " << pose.py  << std::endl;
			std::cout << "A: " << pose.pa  << std::endl;
			std::cout << "W: " << acc << std::endl;
			ofs << "The most accurate hypothesis is" << std::endl;
			ofs << "X: " << pose.px  << std::endl;
			ofs << "Y: " << pose.py  << std::endl;
			ofs << "A: " << pose.pa  << std::endl;
			ofs << "W: " << acc << std::endl; 			
			if(acc > 0.99) {
				std::cout << "Success Localization" << std::endl;
         				std::cout << "I am " << acc*100 << "% sure that I am at (" << pose.px << ", " << pose.py << ")..." << std::endl;
				ofs << "Success Localization" << std::endl;
				ofs << "I am " << acc*100 << "% sure that I am at (" << pose.px << ", " << pose.py << ")" << std::endl;
				pp.SetSpeed(0, 0);
				break;
			}
			mc = 100;
		}
		//navigate the world42.cfg map when nothing affects to roomba
		else {
        			speed = 1.0;
        			if (sp.MinLeft() < 1.2) {
          			turnrate = -0.8;
        			} 
			else if (sp.MinRight() < 1.2) {
          			turnrate = 0.8;
        			} 
			else {
          				if (sp.MinLeft() < sp.MinRight()) {
					turnrate = -0.4;
				}
          				else {
					turnrate = 0.4;
				}
        			}
      		}     

		// What are we doing?
		std::cout << "Speed: " << speed << std::endl;      
      		std::cout << "Turn rate: " << turnrate << std::endl;
     		ofs << "Speed: " << speed << std::endl;      
     		ofs << "Turn rate: " << turnrate << std::endl;
		// Send the commands to the robot
		pp.SetSpeed(speed, turnrate);  
		// Count how many times we do this
		mc++;
		counter++;
	}
	ofs.close();
} // end of main()


player_pose2d_t readPosition(LocalizeProxy& lp) {

	player_localize_hypoth_t hypothesis;
	player_pose2d_t          pose;
	uint32_t                 hCount;
	double                   weight;

	// Need some messing around to avoid a crash when the proxy is
	// starting up.

	hCount = lp.GetHypothCount();

	std::cout << "AMCL gives us " << hCount + 1 << " possible locations:" << std::endl;

	if(hCount > 0){
		for(int i = 0; i <= hCount; i++){
			hypothesis = lp.GetHypoth(i);
			pose       = hypothesis.mean;
			weight     = hypothesis.alpha;
			std::cout << "X: " << pose.px << "\t";
			std::cout << "Y: " << pose.py << "\t";
			std::cout << "A: " << pose.pa << "\t";
			std::cout << "W: " << weight  << std::endl;
		}
	}
	return pose;
} // End of readPosition()

void printLaserData(LaserProxy& sp) {

	double maxRange, minLeft, minRight, range, bearing;
	int points;

	maxRange  = sp.GetMaxRange();
	minLeft   = sp.MinLeft();
	minRight  = sp.MinRight();
	range     = sp.GetRange(5);
	bearing   = sp.GetBearing(5);
	points    = sp.GetCount();

	//Print out useful laser data
	std::cout << "Laser says..." << std::endl;
	std::cout << "Maximum distance I can see: " << maxRange << std::endl;
	std::cout << "Number of readings I return: " << points << std::endl;
	std::cout << "Closest thing on left: " << minLeft << std::endl;
	std::cout << "Closest thing on right: " << minRight << std::endl;
	std::cout << "Range of a single point: " << range << std::endl;
	std::cout << "Bearing of a single point: " << bearing << std::endl;

	return;
} // End of printLaserData()

void printRobotData(BumperProxy& bp, player_pose2d_t pose) {

	// Print out what the bumpers tell us:
	std::cout << "Left  bumper: " << bp[0] << std::endl;
	std::cout << "Right bumper: " << bp[1] << std::endl;
	// Can also print the bumpers with:
	//std::cout << bp << std::endl;

	// Print out where we are
	std::cout << "We are at" << std::endl;
	std::cout << "X: " << pose.px << std::endl;
	std::cout << "Y: " << pose.py << std::endl;
	std::cout << "A: " << pose.pa << std::endl;  
} // End of printRobotData()


