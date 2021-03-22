/**
* follow-plan.cc
* 
* Sample code for a robot that has two front bumpers and a laser, and
* which is provided with localization data.
*
* The code also allows the controller to read and write a "plan", a sequence
* of location that the robot should move to.
*
* Written by: Simon Parsons
* Date:       10th November 2011
*  
**/

/*
*Group6
*
*DONGRYUL KIM
*Thomas Chen
*Wilson Tan
*
*/
#include <iostream>
#include <fstream>
#include <libplayerc++/playerc++.h>
using namespace PlayerCc;  


/**
* Function headers
*
**/

player_pose2d_t readPosition(LocalizeProxy& lp);
void printRobotData(BumperProxy& bp, player_pose2d_t pose);
void printLaserData(LaserProxy& sp);

int  readPlanLength(void);
void readPlan(double *, int);
void printPlan(double *,int);  
void writePlan(double *, int);
void makeplan();


int main(int argc, char *argv[]) {  

	// Variables
	int counter = 0;
	double speed;            // How fast do we want the robot to go forwards?
	double turnrate;         // How fast do we want the robot to turn?
	player_pose2d_t  pose;   // For handling localization data

	// The set of coordinates that makes up the plan

	int pLength;
	double *plan;

	int position = 0, started = 1, arrived = 0, bumped = 0, setangle = 0, traveling = 0;
	double x,y,a,anglediff, distdiff, dx, dy;;
	double destix=0, destiy=0, destia=0;
	int sign = 1;

	// Set up proxies. These are the names we will use to connect to 
	// the interface to the robot.
	PlayerClient    robot("localhost");  
	BumperProxy     bp(&robot,0);  
	Position2dProxy pp(&robot,0);
	LocalizeProxy   lp (&robot, 0);
	LaserProxy      sp (&robot, 0);

	// Allow the program to take charge of the motors (take care now)
	pp.SetMotorEnable(true);

	// Plan handling
	// 
	// A plan is an integer, n, followed by n doubles (n has to be
	// even). The first and second doubles are the initial x and y
	// (respectively) coordinates of the robot, the third and fourth
	// doubles give the first location that the robot should move to, and
	// so on. The last pair of doubles give the point at which the robot
	// should stop.
	pLength = readPlanLength(); // Find out how long the plan is from plan.txt
	plan = new double[pLength]; // Create enough space to store the plan
	readPlan(plan, pLength);    // Read the plan from the file plan.txt.
	printPlan(plan,pLength);    // Print the plan on the screen
	writePlan(plan, pLength);   // Write the plan to the file plan-out.txt


	// Main control loop
	while(true) {    
		// Update information from the robot.
		robot.Read();
		// Read new information about position
		pose = readPosition(lp);
		// Print data on the robot to the terminal
		printRobotData(bp, pose);
		// Print information about the laser. Check the counter first to stop
		// problems on startup

		if(counter > 2) {
			printLaserData(sp);
		}

		// Print data on the robot to the terminal --- turned off for now.
		// printRobotData(bp, pose);

		// If either bumper is pressed, stop. Otherwise just go forwards

		// current position
		x = pose.px;
		y = pose.py;
		a = pose.pa;

		// destination
		destix = plan[position];
		destiy = plan[position+1];
		destia = atan2(destiy-y, destix-x);

		// set angle to destination
		anglediff = rtod(destia)-rtod(a);

		// if bumped, get back and find angle
		if (bumped) {
			//the bumper moves backward according to the direction it hit.
			if (counter < 20) {
				speed = -0.5;
			}
			else if (counter < 30) {
				speed = 0.5;
			}
			else if (counter < 45) {
				turnrate = 0.0;
			}
			else {
				counter = 0;
				bumped = 0;
				speed = 0;
				setangle = 1;
				traveling = 0;
				arrived = 0;
				turnrate = 0.0;
			}
			counter++;
			
		} 
		
		/* roomba travels to 
		 * (-2.5, -6) 
		 * (-2.5, 1.5) 
		 * (-1.5, 2.5)
		 * (2.5, 3.5)
		 * (6.5, 6.5) */
		else if (setangle) {
			// roomba travels if angle is fine
			if (abs(anglediff) < 2) {
				turnrate = 0;
				speed = 1.0;
				setangle = 0;
				traveling = 1;
			} 
			else {
				// roomba searches the angle if angle is not appropriate
				if (anglediff < 0) {
					turnrate = -0.4;
				}
				else {
					turnrate = 0.4;
					speed = 0;
				}
			}
		}
		else if (traveling==1) {
			dx = x-destix;
			dy = y-destiy;
			//roombas stop when it close to each destination
			distdiff = sqrt(dx*dx+dy*dy);
			speed = 1.0;
			if (anglediff < 0) {
				turnrate = -0.4;
			}
			else {
				turnrate = 0.4;
			}
			if (distdiff < 0.2) {
				arrived = 1;
				speed = 0;
				traveling = 0;
			}
	
		}
		// roomba starts to find next destination when it arrives
		else if (arrived==1) {
			speed = 0.0;
			turnrate = 0.0;
			position += 2;
			// at the final destination, stop
			if (position == pLength) {
				std::cout << "arrived" << std::endl << std::endl;
				break;
			}
			setangle = 1;
			arrived = 0;
		} 
		else if (started==1) {
			started = 0;
			turnrate = 0;
			speed = 0;
			setangle = 1;
		}
		// if bumped, set variable "bumped" to 1 and set turarate by the side of bumper.
		if (bp[0] || bp[1] || pp.GetStall()) {
			if (bp[0]) {
				sign = -1;
			}
			if (bp[1]) {
				sign = 1;
			}
			turnrate = 0.4 * sign;
			bumped = 1;
		} 

	// What are we doing?
	std::cout << "Speed: " << speed << std::endl;      
	std::cout << "Turn rate: " << turnrate << std::endl << std::endl;
	std::cout << "Local Data: " << std::endl;
	std::cout << "Heading to location: (" << plan[position] << ", " << plan[position+1] << ")" << std::endl;
	std::cout << "Current X: " << x << std::endl;
	std::cout << "Current Y: " << y << std::endl;
	std::cout << "Current A: " << a << std::endl;
	std::cout << "NEXT X:  " << destix << std::endl;
	std::cout << "NEXT Y:  " << destiy << std::endl;
	std::cout << "NEXT A:  " << destia << std::endl << std::endl;



	// Send the commands to the robot
	pp.SetSpeed(speed, turnrate);  
	// Count how many times we do this
	counter++;
	}

} // end of main()

player_pose2d_t readPosition(LocalizeProxy& lp) {

	player_localize_hypoth_t hypothesis;
	player_pose2d_t          pose;
	uint32_t                 hCount;

	// Need some messing around to avoid a crash when the proxy is
	// starting up.

	hCount = lp.GetHypothCount();

	if(hCount > 0) {
		hypothesis = lp.GetHypoth(0);
		pose       = hypothesis.mean;
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

	//Uncomment this to print out useful laser data
	//std::cout << "Laser says..." << std::endl;
	//std::cout << "Maximum distance I can see: " << maxRange << std::endl;
	//std::cout << "Number of readings I return: " << points << std::endl;
	//std::cout << "Closest thing on left: " << minLeft << std::endl;
	//std::cout << "Closest thing on right: " << minRight << std::endl;
	//std::cout << "Range of a single point: " << range << std::endl;
	//std::cout << "Bearing of a single point: " << bearing << std::endl;

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

int readPlanLength(void) {
	int length;

	std::ifstream planFile;
	planFile.open("plan.txt");

	planFile >> length;
	planFile.close();

	// Some minimal error checking
	if((length % 2) != 0){
		std::cout << "The plan has mismatched x and y coordinates" << std::endl;
		exit(1);
	}

	return length;

} // End of readPlanLength

void readPlan(double *plan, int length) {
	int skip;

	std::ifstream planFile;
	planFile.open("plan.txt");

	planFile >> skip;
	for(int i = 0; i < length; i++) {
		planFile >> plan[i];
	}

	planFile.close();

} // End of readPlan

void printPlan(double *plan , int length) {
	std::cout << std::endl;
	std::cout << "   x     y" << std::endl;
	for(int i = 0; i < length; i++) {
		std::cout.width(5);
		std::cout << plan[i] << " ";
		if((i > 0) && ((i % 2) != 0)){
			std::cout << std::endl;
		}
	}
	std::cout << std::endl;
} // End of printPlan


void writePlan(double *plan , int length) {
	std::ofstream planFile;
	planFile.open("plan-out.txt");

	planFile << length << " ";
	for(int i = 0; i < length; i++){
		planFile << plan[i] << " ";
	}

	planFile.close();

} // End of writePlan
