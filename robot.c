#pragma config(Sensor, S1,     rightTracker,   sensorColorNxtFULL)
#pragma config(Sensor, S2,     leftTracker,    sensorLightActive)
#pragma config(Sensor, S3,     vision,         sensorSONAR)
#pragma config(Sensor, S4,     button,         sensorTouch)
#pragma config(Motor,  motorB,          mLeft,         tmotorNXT, PIDControl, reversed, encoder)
#pragma config(Motor,  motorC,          mRight,        tmotorNXT, PIDControl, reversed, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define SPEED 20
#define ROTATION_SPEED  25
#define lightTreshold  50

int white, black = 0;
const int kMaxSizeOfMessage = 30;
const int INBOX = 5;

TFileIOResult nBTCmdRdErrorStatus;
int nSizeOfMessage;
ubyte nRcvBuffer[kMaxSizeOfMessage];
string command = "";

void forward(int new_speed) {
	motor[mLeft] = new_speed;
	motor[mRight] = new_speed;
}

void left(int rotation_speed) {
	motor[mLeft] = SPEED * 0.3;
	motor[mRight] = SPEED * 2.5;
}

void right(int rotation_speed) {
	motor[mLeft] = SPEED * 2.5;
	motor[mRight] = SPEED * 0.3;
}
void slow_stop(int speed) {
	for(int i = speed; i > 0; i--) {
		forward(i);
		wait1Msec(10);
	}
	motor[mLeft] = 0;
	motor[mRight] = 0;
}

void play_kut_sound(int range) {
	playImmediateTone((range/2), 3);
}

void getBlootoot(string &s) {

	nSizeOfMessage = cCmdMessageGetSize(INBOX);
	if (nSizeOfMessage > kMaxSizeOfMessage)
		nSizeOfMessage = kMaxSizeOfMessage;
	if (nSizeOfMessage > 0){
		nBTCmdRdErrorStatus = cCmdMessageRead(nRcvBuffer, nSizeOfMessage, INBOX);
		nRcvBuffer[nSizeOfMessage] = '\0';
		stringFromChars(s, (char *) nRcvBuffer);
	}
}


//light sensor
// > 60 - white
// < 40 - black

//color sensor
// 6 - white
// 1 - black
void startRobot() {
	int running = 1;
	int blocked = 0;
	int stopped = 0;
	string command = "";

	while(running) {
		int light = SensorValue[leftTracker];
		int color = SensorValue[rightTracker];
		nxtDisplayTextLine(0, "left: %d", SensorValue[leftTracker]);
		nxtDisplayTextLine(1, "right: %d", SensorValue[rightTracker]);
		nxtDisplayTextLine(6, "stopped: %d", stopped);

		getBlootoot(command);

		if(command != "") {
			nxtDisplayTextLine(3, command);
		}

		if(command == "DOWN") {
			stopped = 1;
			slow_stop(SPEED);
			play_kut_sound(0);
			command = "";
		}

		if(command == "UP") {
			stopped = 0;
			forward(SPEED);
			command = "";
		}

		if(!stopped) {
			play_kut_sound(SensorValue[leftTracker]*80);
			if(light < (black + 10) && color == 1) {
				slow_stop(SPEED);
				nSizeOfMessage = 0;
				nxtDisplayTextLine(3, command);

				int passed = 0;
				int offroad = 0;
				if(command == "LEFT") {
					nxtDisplayBigTextLine(0, "ALLAH ACKBAR");
					while(true) {
						int light = SensorValue[leftTracker];

						if(light > (white - 15) && offroad == 0) {
							offroad = 1;
						}
						if(light < (black + 10) && offroad == 1) {
							passed = 1;
						}
						if((light < (black + 10) && offroad == 0) || (light > (white - 15) && passed == 0)) {
							left(ROTATION_SPEED);
						}
						if(light < (black + 10) && offroad == 1 && passed == 1) {
							command = "";
							break;
						}
					}
				}
				if (command == "RIGHT") {
					while(true) {
						int color = SensorValue[rightTracker];

						if(color == 6 && offroad == 0) {
							offroad = 1;
						}
						if(color == 1 && offroad == 1) {
							passed = 1;
						}
						if((color == 1 && offroad == 0) || (color == 6 && passed == 0)) {
							right(ROTATION_SPEED);
						}
						if(color == 1 && offroad == 1 && passed == 1) {
							command = "";
							break;
						}

					}
				}
			}
			else if(light > (white - 15) && color == 6) {
				forward(SPEED);
			}
			else if(color  == 1) {
				right(ROTATION_SPEED);
					wait1Msec(5);
			}
			else if(light < (black + 10)) {
				left(ROTATION_SPEED);
					wait1Msec(5);
			}
		}
		if(SensorValue(vision) < 25 || (SensorValue(vision) == 255 && blocked)) {
			blocked = 1;
			if(blocked) {
				slow_stop(0);
			}
			wait1Msec(500);
			} else {
			blocked = 0;
		}
	}
}

void DisplaySmiley ( int x , int y ) {
	nxtDrawEllipse (x -5 , y +5 , x +5 ,y -5);
	nxtSetPixel (x -2 , y +2);
	nxtSetPixel ( x +2 , y +2);
	nxtSetPixel (x ,y -2);
	nxtSetPixel (x -1 ,y -2);
	nxtSetPixel ( x +1 ,y -2);
	nxtSetPixel (x -2 ,y -1);
	nxtSetPixel ( x +2 ,y -1);
	return ;
}

void calibrate() {
		nxtDisplayTextLine(0, "Dave Calibration");

		while(!white) {
			nxtDisplayTextLine(1, "Please place dave");
			nxtDisplayTextLine(2, "on white and press");
			nxtDisplayTextLine(3, "the button.");
			nxtDisplayTextLine(5, "white: %d", SensorValue[leftTracker]);

			if(SensorValue[button]) {
				white = SensorValue[leftTracker];
				break;
			}
		}

		nxtDisplayTextLine(1, "WHITE COMPLETE");
		nxtDisplayTextLine(2, "");
		nxtDisplayTextLine(3, "");
		wait1Msec(1500);

		while(!black) {
			nxtDisplayTextLine(1, "Please place dave");
			nxtDisplayTextLine(2, "on black and press");
			nxtDisplayTextLine(3, "the button.");
			nxtDisplayTextLine(5, "black: %d", SensorValue[leftTracker]);

			if(SensorValue[button]) {
				black = SensorValue[leftTracker];
				break;
			}
		}

		eraseDisplay();
		nxtDisplayBigTextLine(0, "COMPLETE");
		nxtDisplayTextLine(2, "press button to");
		nxtDisplayTextLine(3, "start the dave.");
		DisplaySmiley(50,25);
		wait1Msec(1000);

		while(true) {
			if(SensorValue[button]) {
				eraseDisplay();
				startRobot();
				break;
			}
		}
}

task main()
{
	calibrate();
}
