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


int Lcounter = 5;
int Rcounter = 5;

double turnrate, speed;
while(true) 
{    
	robot.Read();
		//in general 
		if (bp[0]==0&&bp[1]==0) {
			speed = 1;
			turnrate = 0.16;
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
		if(Lcounter<5 || Rcounter<5) {
			//after left bumper hit (clockwise)
			if (Lcounter<5 && Rcounter>=5) {
				if(pp.GetYPos()<2 && pp.GetYPos()>10 && pp.GetXPos()<1 && pp.GetXPos()>10) {
					speed = -1.5;
					turnrate = 0.5;
				}
				else {
					speed  = -1.5;
					turnrate = -0.5;
				}
				
			}
			//afet right bumper hit (counterclockwise +)
			if (Lcounter>=5 && Rcounter<5) {
				if(pp.GetYPos()>2 && pp.GetYPos()<10 && pp.GetXPos()>1 && pp.GetXPos()<10) {
					speed = -1.5;
					turnrate = -0.5;
				}
				else {
					speed  = -1.5;
					turnrate = 0.5;
				}
			}
			//after both bumper hit program determines the turnrate according to bot's position
			if (Lcounter<5 && Rcounter<5) {
				///clockwise approx cave's borderline
				if(pp.GetYPos()>2 && pp.GetYPos()<10 && pp.GetXPos()>1 && pp.GetXPos()<10) {
					speed = -1.5;
					turnrate = -1;
				}
				//counter-clockwise
				else {
					speed = -1.5;
					turnrate = 1;
				}
				


			}
		}
		else if (Lcounter > 50 && Rcounter > 50 && pp.GetXPos() < 2.5 && pp.GetYPos() <2.5) {
			
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

