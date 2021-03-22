/*
*  bumper.cc
* 
*  Sample code for a robot that has two front bumpers and uses them to
*  avoid obstacles. Suitable for use with the Roomba and Create. 
* 
*  Works IRL. Doesn't work so well in Stage, probably because of the
*  crude modeling of the bumpers, and can get stuck in convex shapes
*  when it backs into an object while trying to turn away from it.
*
*  Based on an example provided by Monica Anderson and Jeff Forbes,
*  via Carlos Jaramillo, and changed to (hopefully) make it easier to
*  understand.
*
*  Modified:    Simon Parsons
*  Date:        15th June 2009
*  Last change: 19th September 2011
*  
*/

#include <iostream>
#include <cstdlib>
#include <libplayerc++/playerc++.h>

int main(int argc, char *argv[])
{  
using namespace PlayerCc;  
PlayerClient    robot("localhost");  
BumperProxy     bp(&robot,0); 

Position2dProxy pp(&robot,0);
pp.SetMotorEnable(true);


int Lcounter = 15;
int Rcounter = 15;

double turnrate, speed;
while(true) 
{    
	robot.Read();
		//in general 
		if (bp[0]==0&&bp[1]==0) {
			speed = 1;
			turnrate = 0;
		}
		
		if(bp[0]==1||bp[1]==1) {
			if(bp[0]==1&&bp[1]==0) {
				Lcounter = 0;
			}
			if(bp[0]==0&&bp[1]==1) {
				Rcounter = 0;
			}
			if(bp[0]==1&&bp[1]==1) {
				Lcounter = 0;
				Rcounter = 0;
			}
		}

		//action after hit
		if(Lcounter<10 || Rcounter<10) {
			//after left bumper hit (clockwise)
			if (Lcounter<10 && Rcounter>=10) {
				speed  = -0.2;
				turnrate = -1.5780;
				
			}
			//afet right bumper hit (counterclockwise +)
			if (Lcounter>=10 && Rcounter<10) {
				speed  = -0.2;
				turnrate =1.5780;	
			}
			//after both bumper hit program determines the turnrate according to bot's position
			if (Lcounter<10 && Rcounter<10) {
				///clockwise
				if(pp.GetXPos()<-1||(pp.GetYPos()>1&&pp.GetYPos()<3&&pp.GetXPos()<13)||(pp.GetYPos()>9&&pp.GetYPos()<11&&pp.GetXPos()<13)) {
					speed = -0.2;
					turnrate = -1.5780;
				}
				//counter-clockwise
				else {
					speed = -0.2;
					turnrate = 1.5780;
				}
				


			}
		}


		if(pp.GetXPos()<=-1 && pp.GetYPos()>10 && pp.GetYPos()<14) {
			std::cout << "Done" << std::endl;
			break;
		}
		Lcounter ++;
		Rcounter ++;

		pp.SetSpeed(speed, turnrate);
		
		std::cout << "Speed: " << speed << std::endl;      
      		std::cout << "Turn rate: " << turnrate << std::endl;
		std::cout << "Left  bumper: " << bp[0] << std::endl;
      		std::cout << "Right bumper: " << bp[1] << std::endl;
		std::cout << "Left counter: " << Lcounter << std::endl;
		std::cout << "right counter: " << Rcounter << std::endl;
		std::cout << "x: " << pp.GetXPos()  << std::endl;
      		std::cout << "y: " << pp.GetYPos()  << std::endl;
      		std::cout << "a: " << pp.GetYaw()  << std::endl << std::endl;

		
	}

}

