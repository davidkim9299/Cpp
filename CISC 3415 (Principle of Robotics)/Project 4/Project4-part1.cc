/**
* local-roomba.cc
* 
* Sample code for a roomba-like robot that has two front bumpers and
* magically knows where it is. 
*
* Written by: Simon Parsons
* Date:       24th October 2011
*  
**/


#include <iostream>
#include <libplayerc++/playerc++.h>
using namespace PlayerCc;  

/**
* Function headers
*
**/

player_pose2d_t readPosition(LocalizeProxy& lp);
void printRobotData(BumperProxy& bp, player_pose2d_t pose);

/**
* main()
*
**/

int main(int argc, char *argv[]) { 

	// Variables
	int counter = 0;
	double speed;            // How fast do we want the robot to go forwards?
	double turnrate;         // How fast do we want the robot to turn?
	player_pose2d_t  pose;   // For handling localization data
	double m_radian;
	double desti_x, desti_y;
	double x,y,a;
	double distance;
	double dx, dy;
	int bumper = 40;

	// Set up proxies. These are the names we will use to connect to 
	// the interface to the robot.
	PlayerClient    robot("localhost");  
	BumperProxy     bp(&robot,0);  
	Position2dProxy pp(&robot,0);
	LocalizeProxy   lp (&robot, 0);

	// Allow the program to take charge of the motors (take care now)
	pp.SetMotorEnable(true);



	// Main control loop
	while(true) {
		x = pose.px;
		y = pose.py;
		a = pose.pa;

		// Update information from the robot.
		robot.Read();
		// Read new information about position
		pose = readPosition(lp);

		// Print data on the robot to the terminal
		printRobotData(bp, pose);

		// This part of the code should be very familiar by now.

		// not bumped
		if((bp[0]==0 || bp[1]==0)) {
			desti_x = 5;
			desti_y = -3.5;
			dx = desti_x- x;
			dy = desti_y- y;
			
			distance = sqrt(dx*dx + dy*dy);		
			m_radian = atan2(dy,dx);
			if(m_radian-a<0.2 && m_radian-a>-0.2) {
				speed = sqrt(distance);
				turnrate = 0;
			}
			else {
				if(m_radian-a > 0.2) {
					speed = 0;
					turnrate = 0.2;
				}
				else {
					speed = 0;
					turnrate = -0.2;
				}			
			}
		}
		
		// if bumped
		if(bp[0]==1 || bp[1]==1) {
			bumper = 0;
		}

		if(bumper >= 0 && bumper <10) {
			speed = -0.5;
			turnrate = -0.2;
		}
		
		if(bumper >= 10 && bumper<40) {
			speed = 1;
			turnrate = 0;

		}
		if(distance<0.1) {
			std::cout << "Arrived !"  << std::endl;
			break;
		}

		

		// What are we doing?
		std::cout << "bumper: " << bumper << std::endl;
		std::cout << "m_radian: " << m_radian << std::endl;
		std::cout << "radius differ: " << m_radian-a << std::endl;
		std::cout << "distance: " << distance << std::endl;    
		std::cout << "Speed: " << speed << std::endl;      
		std::cout << "Turn rate: " << turnrate << std::endl << std::endl;

		// Send the commands to the robot
		pp.SetSpeed(speed, turnrate);  
		// Count how many times we do this
		counter++;
		bumper++;
	}

} // end of main()


/**
* readPosition()
*
* Read the position of the robot from the localization proxy. 
*
* The localization proxy gives us a hypothesis, and from that we extract
* the mean, which is a pose. 
*
**/

player_pose2d_t readPosition(LocalizeProxy& lp) {

	player_localize_hypoth_t hypothesis;
	player_pose2d_t          pose;
	uint32_t                 hCount;

	// Need some messing around to avoid a crash when the proxy is
	// starting up.

	hCount = lp.GetHypothCount();

	if(hCount > 0){
		hypothesis = lp.GetHypoth(0);
		pose       = hypothesis.mean;
	}

	return pose;
} // End of readPosition()


/**
*  printRobotData
*
* Print out data on the state of the bumpers and the current location
* of the robot.
*
**/

void printRobotData(BumperProxy& bp, player_pose2d_t pose) {

	// Print out what the bumpers tell us:
	std::cout << "Left  bumper: " << bp[0] << std::endl;
	std::cout << "Right bumper: " << bp[1] << std::endl;

	// Print out where we are
	std::cout << "We are at" << std::endl;
	std::cout << "X: " << pose.px << std::endl;
	std::cout << "Y: " << pose.py << std::endl;
	std::cout << "A: " << pose.pa << std::endl;


} // End of printRobotData()
