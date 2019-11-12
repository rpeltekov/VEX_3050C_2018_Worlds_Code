#pragma systemFile	// eliminate warning for "unreferenced" functions


// --- Prototypes

// Public

task activateLib();

byte readRightJoystickXAxis();
byte readRightJoystickYAxis();
byte readLeftJoystickXAxis();
byte readLeftJoystickYAxis();

bool isButtonPressed(TVexJoysticks button);
bool isButtonClick(TVexJoysticks button);

void turnMotor(tMotor motorPort, byte powerPct);
void turnMotor(tMotor motorPort1, tMotor motorPort2, byte powerPct);
void turnMotor(tMotor motorPort, byte powerPct, int msDuration);
void turnMotor(tMotor motorPort1, tMotor motorPort2, byte powerPct, int msDuration);
void turnMotor(tMotor motorPort, byte fullSpeed, byte slowSpeed, tSensors encoderPort, int encoderSlowPos, int encoderStopPos, int msTimeout, bool brake);
void turnMotor(tMotor motorPort1, tMotor motorPort2, byte fullSpeed, byte slowSpeed, tSensors encoderPort, int encoderSlowPos, int encoderStopPos, int msTimeout, bool brake);

bool isMotorActive();
bool waitForActiveMotors();
void cancelActiveMotors();

bool setPsuedoTimer(unsigned int timerId, int msDuration);
bool cancelTimer(unsigned int timerId);
void timerCallback(int timerId);

TControllerButtons getLcdButtons();

// Private

void checkMotors();
void checkMotorTimer(byte i);
void checkMotorPosition(byte i);
byte reserveAvailableTimer(tMotor motorPort1, tMotor motorPort2, byte powerPct, int ms);
byte reserveAvailableTimer(tMotor motorPort1, tMotor motorPort2, byte fullSpeed, byte slowSpeed, int ms, tSensors encoderPort, int encoderSlowPos, int encoderStopPos, bool brake);
task checkPsuedoTimer();
long map(long x, long in_min, long in_max, long out_min, long out_max);
int map(int x, int in_min, int in_max, int out_min, int out_max);
byte map(byte x, byte in_min, byte in_max, byte out_min, byte out_max);
void dumpDebugInfo();

// --- Global variables

typedef struct {
	bool active;
	TTimers timer;
	int ms;
	tMotor motorPort1;
	tMotor motorPort2;
	byte powerPct;
	byte slowPct;
	bool encoderInUse;
	tSensors encoderPort;
	int encoderSlowPos;
	int encoderStopPos;
	int lastKnownPos;
	bool brake;
} MOTOR_TIMER;
static MOTOR_TIMER motorCtrl[4];

const int PSUEDO_TIMER_SIZE = 9;
typedef struct {
	unsigned int timerId;
	unsigned int duration;
	unsigned int expirationTime;
	bool recurring;
} PSUEDO_TIMER;
static PSUEDO_TIMER psuedoTimer[PSUEDO_TIMER_SIZE];

// --- Initialization and utilities task loop

task activateLib()
{
	for (int i = 0; i < 4; i++) {
		motorCtrl[i].active = false;
		motorCtrl[i].timer = (i == 0 ? T1 : i == 1 ? T2 : i == 2 ? T3 : T4);
		motorCtrl[i].ms = 0;
		motorCtrl[i].powerPct = 0;
		motorCtrl[i].encoderInUse = false;
		motorCtrl[i].brake = false;
	}

	for (int i = 0; i < PSUEDO_TIMER_SIZE; i++) {
		psuedoTimer[i].timerId = 0;
		psuedoTimer[i].duration = 0;
		psuedoTimer[i].expirationTime = 0;
		psuedoTimer[i].recurring = false;
	}

	startTask(checkPsuedoTimer);

	while (true) {
		checkMotors();
		wait1Msec(5);
	}
}

// --- Read Joysticks

//	Ch1 = Right Joystick X-axis: Full Right = 127, Full Left  = -127
//	Ch2 = Right Joystick Y-axis: Full Up    = 127, Full Down  = -127
//	Ch3 = Left Joystick Y-axis:  Full Up    = 127, Full Down  = -127
//	Ch4 = Left Joystick X-axis:  Full Right = 127, Full Left  = -127

byte readRightJoystickXAxis()
{
	return map((byte)vexRT[Ch1], -127, 127, -100, 100);
}

byte readRightJoystickYAxis()
{
	return map((byte)vexRT[Ch2], -127, 127, -100, 100);
}

byte readLeftJoystickXAxis()
{
	return map((byte)vexRT[Ch4], -127, 127, -100, 100);
}

byte readLeftJoystickYAxis()
{
	return map((byte)vexRT[Ch3], -127, 127, -100, 100);
}

// --- Read Button Press

bool isButtonPressed(TVexJoysticks button)
{
	return vexRT[button] == 1 ? true : false;
}

bool isButtonClick(TVexJoysticks button)
{
	bool rc = false;

	static bool Btn5UPressed = false;
	if (isButtonPressed(Btn5U)) {
		if (button == Btn5U && !Btn5UPressed) {
			Btn5UPressed = true;
			rc = true;
		}
	} else {
		Btn5UPressed = false;
	}

	static bool Btn5UXmtr2Pressed = false;
	if (isButtonPressed(Btn5UXmtr2)) {
		if (button == Btn5UXmtr2 && !Btn5UXmtr2Pressed) {
			Btn5UXmtr2Pressed = true;
			rc = true;
		}
	} else {
		Btn5UXmtr2Pressed = false;
	}

	static bool Btn5DPressed = false;
	if (isButtonPressed(Btn5D)) {
		if (button == Btn5D && !Btn5DPressed) {
			Btn5DPressed = true;
			rc = true;
		}
	} else {
		Btn5DPressed = false;
	}

	static bool Btn5DXmtr2Pressed = false;
	if (isButtonPressed(Btn5DXmtr2)) {
		if (button == Btn5DXmtr2 && !Btn5DXmtr2Pressed) {
			Btn5DXmtr2Pressed = true;
			rc = true;
		}
	} else {
		Btn5DXmtr2Pressed = false;
	}

	static bool Btn6UPressed = false;
	if (isButtonPressed(Btn6U)) {
		if (button == Btn6U && !Btn6UPressed) {
			Btn6UPressed = true;
			rc = true;
		}
	} else {
		Btn6UPressed = false;
	}

	static bool Btn6UXmtr2Pressed = false;
	if (isButtonPressed(Btn6UXmtr2)) {
		if (button == Btn6UXmtr2 && !Btn6UXmtr2Pressed) {
			Btn6UXmtr2Pressed = true;
			rc = true;
		}
	} else {
		Btn6UXmtr2Pressed = false;
	}

	static bool Btn6DPressed = false;
	if (isButtonPressed(Btn6D)) {
		if (button == Btn6D && !Btn6DPressed) {
			Btn6DPressed = true;
			rc = true;
		}
	} else {
		Btn6DPressed = false;
	}

	static bool Btn6DXmtr2Pressed = false;
	if (isButtonPressed(Btn6DXmtr2)) {
		if (button == Btn6DXmtr2 && !Btn6DXmtr2Pressed) {
			Btn6DXmtr2Pressed = true;
			rc = true;
		}
	} else {
		Btn6DXmtr2Pressed = false;
	}

	static bool Btn7UPressed = false;
	if (isButtonPressed(Btn7U)) {
		if (button == Btn7U && !Btn7UPressed) {
			Btn7UPressed = true;
			rc = true;
		}
	} else {
		Btn7UPressed = false;
	}

	static bool Btn7UXmtr2Pressed = false;
	if (isButtonPressed(Btn7UXmtr2)) {
		if (button == Btn7UXmtr2 && !Btn7UXmtr2Pressed) {
			Btn7UXmtr2Pressed = true;
			rc = true;
		}
	} else {
		Btn7UXmtr2Pressed = false;
	}

	static bool Btn7DPressed = false;
	if (isButtonPressed(Btn7D)) {
		if (button == Btn7D && !Btn7DPressed) {
			Btn7DPressed = true;
			rc = true;
		}
	} else {
		Btn7DPressed = false;
	}

	static bool Btn7DXmtr2Pressed = false;
	if (isButtonPressed(Btn7DXmtr2)) {
		if (button == Btn7DXmtr2 && !Btn7DXmtr2Pressed) {
			Btn7DXmtr2Pressed = true;
			rc = true;
		}
	} else {
		Btn7DXmtr2Pressed = false;
	}

	static bool Btn7LPressed = false;
	if (isButtonPressed(Btn7L)) {
		if (button == Btn7L && !Btn7LPressed) {
			Btn7LPressed = true;
			rc = true;
		}
	} else {
		Btn7LPressed = false;
	}

	static bool Btn7LXmtr2Pressed = false;
	if (isButtonPressed(Btn7LXmtr2)) {
		if (button == Btn7LXmtr2 && !Btn7LXmtr2Pressed) {
			Btn7LXmtr2Pressed = true;
			rc = true;
		}
	} else {
		Btn7LXmtr2Pressed = false;
	}

	static bool Btn7RPressed = false;
	if (isButtonPressed(Btn7R)) {
		if (button == Btn7R && !Btn7RPressed) {
			Btn7RPressed = true;
			rc = true;
		}
	} else {
		Btn7RPressed = false;
	}

	static bool Btn7RXmtr2Pressed = false;
	if (isButtonPressed(Btn7RXmtr2)) {
		if (button == Btn7RXmtr2 && !Btn7RXmtr2Pressed) {
			Btn7RXmtr2Pressed = true;
			rc = true;
		}
	} else {
		Btn7RXmtr2Pressed = false;
	}

	static bool Btn8UPressed = false;
	if (isButtonPressed(Btn8U)) {
		if (button == Btn8U && !Btn8UPressed) {
			Btn8UPressed = true;
			rc = true;
		}
	} else {
		Btn8UPressed = false;
	}

	static bool Btn8UXmtr2Pressed = false;
	if (isButtonPressed(Btn8UXmtr2)) {
		if (button == Btn8UXmtr2 && !Btn8UXmtr2Pressed) {
			Btn8UXmtr2Pressed = true;
			rc = true;
		}
	} else {
		Btn8UXmtr2Pressed = false;
	}

	static bool Btn8DPressed = false;
	if (isButtonPressed(Btn8D)) {
		if (button == Btn8D && !Btn8DPressed) {
			Btn8DPressed = true;
			rc = true;
		}
	} else {
		Btn8DPressed = false;
	}

	static bool Btn8DXmtr2Pressed = false;
	if (isButtonPressed(Btn8DXmtr2)) {
		if (button == Btn8DXmtr2 && !Btn8DXmtr2Pressed) {
			Btn8DXmtr2Pressed = true;
			rc = true;
		}
	} else {
		Btn8DXmtr2Pressed = false;
	}

	static bool Btn8LPressed = false;
	if (isButtonPressed(Btn8L)) {
		if (button == Btn8L && !Btn8LPressed) {
			Btn8LPressed = true;
			rc = true;
		}
	} else {
		Btn8LPressed = false;
	}

	static bool Btn8LXmtr2Pressed = false;
	if (isButtonPressed(Btn8LXmtr2)) {
		if (button == Btn8LXmtr2 && !Btn8LXmtr2Pressed) {
			Btn8LXmtr2Pressed = true;
			rc = true;
		}
	} else {
		Btn8LXmtr2Pressed = false;
	}

	static bool Btn8RPressed = false;
	if (isButtonPressed(Btn8R)) {
		if (button == Btn8R && !Btn8RPressed) {
			Btn8RPressed = true;
			rc = true;
		}
	} else {
		Btn8RPressed = false;
	}

	static bool Btn8RXmtr2Pressed = false;
	if (isButtonPressed(Btn8RXmtr2)) {
		if (button == Btn8RXmtr2 && !Btn8RXmtr2Pressed) {
			Btn8RXmtr2Pressed = true;
			rc = true;
		}
	} else {
		Btn8RXmtr2Pressed = false;
	}

	return rc;
}

// --- Handle Servos

void turnMotor(tMotor motorPort, byte powerPct)
{
	turnMotor(motorPort, motorPort, powerPct);
}

void turnMotor(tMotor motorPort1, tMotor motorPort2, byte powerPct)
{
	// Bounds check power value
	if (powerPct < -100) powerPct = -100;
	if (powerPct > 100) powerPct = 100;

	// When asked to stop a motor, first check to see if this motor is
	// on a timer and if so resume turning at the previously set speed.
	// This handles the case where the motor is on a timer and the user
	// manually overrides it.
	if (powerPct == 0) {
		for (byte i = 0; i < 4; i++) {
			if (motorCtrl[i].active && motorCtrl[i].motorPort1 == motorPort1 && motorCtrl[i].motorPort2 == motorPort2)
				powerPct = motorCtrl[i].powerPct;
		}
	}

	// Turn motor(s), translating power % into servo value
	motor[motorPort1] = map(powerPct, -100, 100, -127, 127);

	if (motorPort1 != motorPort2)
		motor[motorPort2] = map(powerPct, -100, 100, -127, 127);
}

void turnMotor(tMotor motorPort, byte powerPct, int msDuration)
{
	turnMotor(motorPort, motorPort, powerPct, msDuration);
}

void turnMotor(tMotor motorPort1, tMotor motorPort2, byte powerPct, int msDuration)
{
	// Nothing to do if duration is invalid
	if (msDuration < 0) return;

	// Get a timer for motor turning duration
	if (reserveAvailableTimer(motorPort1, motorPort2, powerPct, msDuration) < 0) {
		writeDebugStreamLine("ERROR: Unable to reserve timer for turning motor!");
		return;
	}

	// Start the motor(s) turning; pollMotor() will stop it when the timer expires
	turnMotor(motorPort1, motorPort2, powerPct);
}

void turnMotor(tMotor motorPort, byte fullSpeed, byte slowSpeed, tSensors encoderPort, int encoderSlowPos, int encoderStopPos, int msTimeout, bool brake)
{
	turnMotor(motorPort, motorPort, fullSpeed, slowSpeed, encoderPort, encoderSlowPos, encoderStopPos, msTimeout, brake);
}

void turnMotor(tMotor motorPort1, tMotor motorPort2, byte fullSpeed, byte slowSpeed, tSensors encoderPort, int encoderSlowPos, int encoderStopPos, int msTimeout, bool brake)
{
	// Must supply a valid timeout value
	if (msTimeout < 1) return;

	//writeDebugStreamLine("Turn Motor, full: %d, slow: %d, slowPos: %d, stopPos: %d",
	//		fullSpeed, slowSpeed, encoderSlowPos, encoderStopPos);

	// Difference between current position and stopping point
	int posDiff = abs(SensorValue[encoderPort] - encoderStopPos);

	// No need to power motor if already close enough to encoder position
	const int CLOSE_ENOUGH = 20;
	if (posDiff <= CLOSE_ENOUGH) return;

	// Get a timer for motor turning duration
	if (reserveAvailableTimer(motorPort1, motorPort2, fullSpeed, slowSpeed, msTimeout, encoderPort, encoderSlowPos, encoderStopPos, brake) < 0) {
		writeDebugStreamLine("ERROR: Unable to reserve timer for turning motor!");
		return;
	}

	// Start the motor turning; pollMotor() will stop it when encoder position reached or timer expires
	turnMotor(motorPort1, motorPort2, posDiff <= abs(encoderStopPos - encoderSlowPos) ? slowSpeed : fullSpeed);
}

bool isMotorActive()
{
	return (motorCtrl[0].active || motorCtrl[1].active || motorCtrl[2].active || motorCtrl[3].active);
}

bool waitForActiveMotors()
{
	while (motorCtrl[0].active || motorCtrl[1].active || motorCtrl[2].active || motorCtrl[3].active) {
//		if (isButtonPressed(Btn8R)) {
//			cancelActiveMotors();
//			return false;
//		}
		wait1Msec(5);
	}

	// This is a work-around for a multi-tasking bug that I've been unable to figure out
	wait1Msec(150);
	return true;
	//writeDebugStreamLine("Done waiting");
}

void cancelActiveMotors()
{
	motorCtrl[0].ms = 0;
	motorCtrl[1].ms = 0;
	motorCtrl[2].ms = 0;
	motorCtrl[3].ms = 0;
}

void checkMotors()
{
	// Syncronize this method (called every 5ms)
	static bool polling = false;
	if (polling) return;
	polling = true;

	// A total of 4 timers are available
	for (byte i = 0; i < 4; i++) {
		// Only interested in active timers
		if (!motorCtrl[i].active) continue;

		// Stop motor(s) and timer when time interval has been reached
		checkMotorTimer(i);

		// When using an encoder check if the specified position has been reached
		checkMotorPosition(i);
	}

	polling = false;
}

void checkMotorTimer(byte i)
{
	if (time1[motorCtrl[i].timer] >= motorCtrl[i].ms) {
		stopMotor(motorCtrl[i].motorPort1);
		if (motorCtrl[i].motorPort1 != motorCtrl[i].motorPort2) stopMotor(motorCtrl[i].motorPort2);
		//writeDebugStreamLine("Motor timer stop");

			// Clear array
		motorCtrl[i].ms = 0;
		motorCtrl[i].powerPct = 0;
		motorCtrl[i].slowPct = 0;
		motorCtrl[i].brake = false;
		motorCtrl[i].encoderInUse = false;
		motorCtrl[i].active = false;
	}
}

void checkMotorPosition(byte i)
{
	if (!motorCtrl[i].encoderInUse) return;

	// Get difference between current position and encoder stopping position
	int posDiff = abs(SensorValue[motorCtrl[i].encoderPort] - motorCtrl[i].encoderStopPos);
	int lastPosDiff = abs(motorCtrl[i].lastKnownPos - motorCtrl[i].encoderStopPos);

	// Set new slower motor speed when getting close to final position
	if (posDiff < abs(motorCtrl[i].encoderStopPos - motorCtrl[i].encoderSlowPos)) {
		turnMotor(motorCtrl[i].motorPort1, motorCtrl[i].motorPort2, motorCtrl[i].slowPct);
	}

	// Stop motor and timer when encoder position is reached or overshot (or turning the wrong way)
	if (posDiff < 50 && posDiff - 1 > lastPosDiff) {	// subtract a bit to allow for some sensor error
		//writeDebugStreamLine("posDiff: %d, lastPosDiff: %d", posDiff, lastPosDiff);
		// Slight motor reverse acts as brake
		if (motorCtrl[i].brake) {
			turnMotor(motorCtrl[i].motorPort1, motorCtrl[i].motorPort2, (motorCtrl[i].powerPct < 0 ? 20 : -20));
			wait1Msec(40);
		}

		// Experiment with fixing overshot by keeping motor reverse running a little longer
		if (abs(SensorValue[motorCtrl[i].encoderPort] - motorCtrl[i].encoderStopPos) > 40) {
			//writeDebugStreamLine("Overshot adj, reqt pos: %d, cur pos: %d",
			//	motorCtrl[i].encoderStopPos, SensorValue[motorCtrl[i].encoderPort]);
			wait1Msec(30);
		}

		// Stop Motor
		stopMotor(motorCtrl[i].motorPort1);
		if (motorCtrl[i].motorPort1 != motorCtrl[i].motorPort2) stopMotor(motorCtrl[i].motorPort2);

		//writeDebugStreamLine("Encoder stop, reqt pos: %d, cur pos: %d",
		//	motorCtrl[i].encoderStopPos, SensorValue[motorCtrl[i].encoderPort]);

		// Clear array
		motorCtrl[i].ms = 0;
		motorCtrl[i].powerPct = 0;
		motorCtrl[i].slowPct = 0;
		motorCtrl[i].brake = false;
		motorCtrl[i].encoderInUse = false;
		motorCtrl[i].active = false;
	}

	motorCtrl[i].lastKnownPos = SensorValue[motorCtrl[i].encoderPort];
}

// --- Timer Routines

byte reserveAvailableTimer(tMotor motorPort1, tMotor motorPort2, byte powerPct, int ms)
{
	for (byte i = 0; i < 4; i++)
	{
		// Check for existing port first to reset, afterwards look for inactive slot to activate
		if ((motorCtrl[i].motorPort1 == motorPort1 && motorCtrl[i].motorPort2 == motorPort2) || !motorCtrl[i].active) {
			clearTimer(motorCtrl[i].timer);
			motorCtrl[i].ms = (ms > 32767 ? 32767 : ms);
			motorCtrl[i].motorPort1 = motorPort1;
			motorCtrl[i].motorPort2 = motorPort2;
			motorCtrl[i].powerPct = powerPct;
			motorCtrl[i].encoderInUse = false;
			motorCtrl[i].active = true;		// Activate last
			return i;
		}
	}
	return -1;
}

byte reserveAvailableTimer(tMotor motorPort1, tMotor motorPort2, byte fullSpeed, byte slowSpeed, int ms, tSensors encoderPort, int encoderSlowPos, int encoderStopPos, bool brake)
{
	for (byte i = 0; i < 4; i++)
	{
		// Check for existing port first to reset, afterwards look for inactive slot to activate
		if ((motorCtrl[i].motorPort1 == motorPort1 && motorCtrl[i].motorPort2 == motorPort2) || !motorCtrl[i].active) {
			clearTimer(motorCtrl[i].timer);
			motorCtrl[i].ms = (ms > 32767 ? 32767 : ms);
			motorCtrl[i].motorPort1 = motorPort1;
			motorCtrl[i].motorPort2 = motorPort2;
			motorCtrl[i].powerPct = fullSpeed;
			motorCtrl[i].slowPct = slowSpeed;
			motorCtrl[i].encoderInUse = true;
			motorCtrl[i].encoderPort = encoderPort;
			motorCtrl[i].encoderSlowPos = encoderSlowPos;
			motorCtrl[i].encoderStopPos = encoderStopPos;
			motorCtrl[i].lastKnownPos = SensorValue[encoderPort];
			motorCtrl[i].brake = brake;
			motorCtrl[i].active = true;		// Activate last
			//writeDebugStreamLine("Reserve Timer: %d", i);
			return i;
		}
	}
	return -1;
}

bool setPsuedoTimer(unsigned int timerId, int msDuration, bool recurring = false)
{
	for (int i = 0; i < PSUEDO_TIMER_SIZE; i++) {
		if (psuedoTimer[i].expirationTime == 0) {
			psuedoTimer[i].expirationTime = nSysTime + msDuration;
			psuedoTimer[i].timerId = timerId;
			psuedoTimer[i].duration = msDuration;
			psuedoTimer[i].recurring = recurring;
			return true;
		}
	}
	return false;
}

bool cancelTimer(unsigned int timerId)
{
	for (int i = 0; i < PSUEDO_TIMER_SIZE; i++) {
		if (psuedoTimer[i].timerId == timerId) {
			psuedoTimer[i].expirationTime = 0;
			psuedoTimer[i].timerId = 0;
			psuedoTimer[i].duration = 0;
			psuedoTimer[i].recurring = false;
			return true;
		}
	}
	return false;
}

task checkPsuedoTimer()
{
	while (true) {
		for (int i = 0; i < PSUEDO_TIMER_SIZE; i++) {
			if (psuedoTimer[i].expirationTime > 0) {
				if (psuedoTimer[i].expirationTime <= nSysTime) {
					int timerId = psuedoTimer[i].timerId;
					if (psuedoTimer[i].recurring) {
						psuedoTimer[i].expirationTime = nSysTime + psuedoTimer[i].duration;
					} else {
						psuedoTimer[i].timerId = 0;
						psuedoTimer[i].expirationTime = 0;
						psuedoTimer[i].duration = 0;
					}
					timerCallback(timerId);
				}
			}
		}
		wait1Msec(1);
	}
}

// --- LCD Routines

/*-----------------------------------------------------------------------------*/
/*  This function is used to get the LCD button status but also acts as a      */
/*  "wait for button release" feature.                                         */
/*  Use it in place of nLcdButtons.                                            */
/*  The function blocks until a button is pressed.                             */
/*-----------------------------------------------------------------------------*/

// Little macro to keep code cleaner, masks both disable/enable and autonomous/driver
#define vexCompetitionState (nVexRCReceiveState & (vrDisabled | vrAutonomousMode))

TControllerButtons getLcdButtons()
{
	TVexReceiverState   competitionState = vexCompetitionState;
	TControllerButtons  buttons;

	// Blocks until either:
	// 1. A button is pressd on the LCD
	//    (pressed button must be released before a new button is detected)
	// 2. Robot competition state changes

	// Wait for all buttons to be released
	while (nLCDButtons != kButtonNone) {
		// Check competition state; exit if it changes
		if (vexCompetitionState != competitionState) return kButtonNone;
	}

	// block until an LCD button is pressed
	do {
		// Use a copy of the lcd buttons to avoid state changing between the test and return
		buttons = nLCDButtons;

		// Check competition state; exit if it changes
		if (vexCompetitionState != competitionState) return kButtonNone;

		wait1Msec(10);
	} while (buttons == kButtonNone);

	return buttons;
}

// --- Utility Routines

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int map(int x, int in_min, int in_max, int out_min, int out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

byte map(byte x, byte in_min, byte in_max, byte out_min, byte out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void dumpDebugInfo()
{
	writeDebugStreamLine("VEX Master Firmware version: %d", version);
	writeDebugStreamLine("Firmware version: %d", nVexMasterVersion);

	writeDebugStreamLine("Main battery level: %3.2f", nAvgBatteryLevel * 0.001);
	writeDebugStreamLine("Backup battery level: %3.2f", BackupBatteryLevel * 0.001);
	writeDebugStreamLine("Is autonomous mode: %s", bIfiAutonomousMode ? "True" : "False");

	writeDebugStreamLine("Transmitter state:");
	TVexReceiverState state = nVexRCReceiveState;
	if (state & vrNoXmiters)
		writeDebugStreamLine("\tTransmitter powered off");
	if (state & vrXmit1)
		writeDebugStreamLine("\tTransmitter 1 on");
	if (state & vrXmit2)
		writeDebugStreamLine("\tTransmitter 2 on");
	if (state & vrCompetitionSwitch)
		writeDebugStreamLine("\tCompetition switch attached");
	if (state & vrGameController)
		writeDebugStreamLine("\tVEXnet Controller");
	else
		writeDebugStreamLine("\tLegacy 75MHz controller");
	if (state & vrAutonomousMode)
		writeDebugStreamLine("\tAutonomous mode");
	else
		writeDebugStreamLine("\tDriver control mode");
	if (state & vrDisabled)
		writeDebugStreamLine("\tDisabled");
	else
		writeDebugStreamLine("\tEnabled");

	int upTime = nSysTime;
	int hr = upTime / 1000 / 60 / 60;
	if (hr > 0) upTime -= hr * 60 * 60 * 1000;
	int min = upTime / 1000 / 60;
	if (min > 0) upTime -= min * 60 * 1000;
	int sec = upTime / 1000;
	if (sec > 0) upTime -= sec * 1000;
	int ms = upTime;

	writeDebugStreamLine("System up time: %02d:%02d:%02d.%04d", hr, min, sec, ms);

	int pgmTime = nPgmTime;
	int pHr = pgmTime / 1000 / 60 / 60;
	if (pHr > 0) pgmTime -= pHr * 60 * 60 * 1000;
	int pMin = pgmTime / 1000 / 60;
	if (pMin > 0) pgmTime -= pMin * 60 * 1000;
	int pSec = pgmTime / 1000;
	if (pSec > 0) pgmTime -= pSec * 1000;
	int pMs = pgmTime;

	writeDebugStreamLine("Program run time: %02d:%02d:%02d.%04d", pHr, pMin, pSec, pMs);
}
