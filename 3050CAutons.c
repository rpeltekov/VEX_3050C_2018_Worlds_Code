#pragma SystemFile


/*
	//////////////////////////////////////////////////////////////Mobile Goal 20////////////////////////
	motor[roller] = 40;

	clearTimer(T2);

	while (time1(T2) < 3000) {

		moveLiftUp(100, 700);

		//Drive
		if (time1(T2) > 500) {
			autoDriveGyroPIDControl(0, 1350, 1, 0.00015, 0.5, 0.00002, 0.000000000079, 1.3);
		}

		//Move mobile goal lifter out
		if (!mobileGoalIsOut) {
			moveMobileGoalOut();
			} else {
			motor[mobileGoal] = -50;
		}

		wait1Msec(20);
	}

	drive(0, 0);
	clearTimer(T2);
	clearDriveEnc();

	while (time1(T2) < 2300) {

		if (mobileGoalIsOut) {
			moveMobileGoalIn();
			} else {
			motor[mobileGoal] = 50;
		}

		//Drive
		if (time1(T2) > 1600) {
			driveBackward(-100, -460);
			moveLiftDown(50, 400);
			//autoDriveGyroPIDControl(0, -1200, 1, 0.00015, 1.0, 1, 0.0000002, 0.4);
		}

		wait1Msec(20);
	}

	drive(0, 0);
	clearTimer(T2);
	clearDriveEnc();

	wait1Msec(200);

	if (allianceSide == RIGHT) {
		while (time1(T2) < 1000) {
			//turn
			autoGyroPIDControl(-300, true);
		}
		} else if (allianceSide == LEFT) {
		while (time1(T2) < 1000) {
			//turn
			autoGyroPIDControl(300, true);
		}
	}

	drive(0, 0);
	clearTimer(T2);
	clearDriveEnc();

	while (time1(T2) < 1000) {
		//drive backward
		driveBackward(-100, -60);
	}

	drive(0, 0);
	clearTimer(T2);
	clearDriveEnc();
	if (allianceSide == RIGHT) {
		while (time1(T2) < 2000) {
			//turn
			autoGyroPIDControl(-1500, true);
		}
		} else if (allianceSide == LEFT) {
		while (time1(T2) < 1000) {
			//turn
			autoGyroPIDControl(1500, true);
		}
	}

	drive(0, 0);
	clearTimer(T2);
	clearDriveEnc();
	SensorValue[rollerEnc] = 0;

	while (time1(T2) < 2000) {
		moveMobileGoalOut();

		moveLiftUp(100, 500);

		rollerOutake(-100, 150);

		if (time1(T2) > 300) {
			drive(100, 100);
			wait1Msec(600);
			drive(0, 0);
		}
		wait1Msec(20);
	}

	drive(0, 0);
	clearTimer(T2);
	clearDriveEnc();

	while (time1(T2) < 600) {
		drive(-60, -60);
		wait1Msec(400);
		drive(0, 0);
	}
	moveMobileGoalInAuto();


	//if (allianceColor == BLUE_ALLIANCE) {
	//	theaterChaseTask(0, 0, 127, 50, 15000);
	//	} else {
	//	theaterChaseTask(127, 0, 0, 50, 15000);
	//}

	///////////////////////////////////////////////////////////////////////End Mobile Goal 20//////////////////////////
	break;

case AUTONOMOUS_MODE_STATIONARY:

	/////////////////////////////////////////////////////////////////////////////AUTO 2///////////////////////////////////////////
	//Apply passive power to roller

	motor [roller] = 40;

	clearTimer(T2);

	//move lift up
	while (time1(T2) < 1000) {
		if (time1(T2) < 1000) {
			moveLiftUp(70, 800);
			} else {
			motor[liftL] = 0;
			motor[liftL] = 0;
		}
	}

	wait1Msec(300);

	//drive forward
	driveForward(60, 200);

	wait1Msec(100);

	//move swinging arm out
	moveArmOutAuto();

	wait1Msec(200);

	moveArmIn();
	motor[roller] = -127;

	wait1Msec(200);

	if (allianceSide == LEFT) {
		driveBackward(-100, -700);
		} else if (allianceSide == RIGHT) {
		driveBackward(-700, -100);
	}

	wait1Msec(200);

	clearTimer(T2);

	while (time1(T2) < 600) {
		autoDrivePIDControl(-300, true);
	}
	wait1Msec(100);

	driveForward(100, 3000);

	//if (allianceColor == BLUE_ALLIANCE) {
	//	theaterChaseTask(0, 0, 127, 50, 15000);
	//	} else {
	//	theaterChaseTask(127, 0, 0, 50, 15000);
	//}

	//////////////////////////////////////////////////////End of auto 2/////////////////////////////////
	break;

case AUTONOMOUS_MODE_2_20:

	////////////////////////////////////////////////////////////////////////////////AUTO 3/////////////////////////////////////////////////
	motor[roller] = 40;

	clearTimer(T2);

	while (time1(T2) < 3000) {

		moveLiftUp(100, 700);

		//Drive
		if (time1(T2) > 500) {
			autoDriveGyroPIDControl(0, 1350, 1, 0.00015, 0.5, 0.00002, 0.000000000079, 1.3);
		}

		//Move mobile goal lifter out
		if (!mobileGoalIsOut) {
			moveMobileGoalOut();
			} else {
			motor[mobileGoal] = -50;
		}

		wait1Msec(20);
	}

	drive(0, 0);
	clearTimer(T2);
	clearDriveEnc();

	while (time1(T2) < 2200) {

		if (mobileGoalIsOut) {
			moveMobileGoalInAuto();
			} else {
			motor[mobileGoal] = 50;
		}

		if (time1(T2) > 1600) {
			moveLiftDown(50, 400);
			//autoDriveGyroPIDControl(0, -1200, 1, 0.00015, 1.0, 1, 0.0000002, 0.4);
		}

		if (time1(T2) > 2050) {
			rollerOutake(-100, 150);
		}

		wait1Msec(20);
	}

	drive(0, 0);
	clearTimer(T2);
	clearDriveEnc();

	while (time1(T2) < 1500) {
		autoDriveGyroPIDControl(0, 200, 1, 0.00015, 0.5, 1, 0.000079, 0.000003);

		moveLiftDown(50, 300);

		moveArmOut();

		motor[roller] = 120;
		if (time1(T2) > 1000) {
			motor[roller] = 40;
			moveLiftUp(100, 550);
		}
		if (time1(T2) > 1300) {
			moveArmIn();
		}
	}

	drive(0, 0);
	clearTimer(T2);
	clearDriveEnc();


	if (allianceSide == LEFT) {
		while (time1(T2) < 1000) {
			moveLiftDown(20, 550);
			driveBackward(-100, -580);
			if (time1(T2) > 200) {
				motor[roller] = -100;
				wait1Msec(400);
				motor[roller] = 0;
			}
		}
		} else if (allianceSide == RIGHT) {
		while (time1(T2) < 1000) {
			moveLiftDown(20, 550);
			driveBackward( -580, -100);
			if (time1(T2) > 200) {
				motor[roller] = -100;
				wait1Msec(400);
				motor[roller] = 0;
			}
		}
	}

	drive(0, 0);
	clearTimer(T2);
	clearDriveEnc();

	wait1Msec(200);

	if (allianceSide == LEFT) {
		while (time1(T2) < 2000) {
			//turn
			autoGyroPIDControl(-1700, true);
		}
		} else if (allianceSide == RIGHT) {
		while (time1(T2) < 2000) {
			//turn
			autoGyroPIDControl(1700, true);
		}
	}

	drive(0, 0);
	clearTimer(T2);
	clearDriveEnc();

	while (time1(T2) < 1500) {
		moveMobileGoalOut();
		moveLiftUp(80, 500);
		motor[roller] = -100;

		drive(50, 50);
		wait1Msec(200);
		drive(0, 0);
	}

	drive(0, 0);
	clearTimer(T2);
	clearDriveEnc();

	while (time1(T2) < 300) {
		drive(-60, -60);
		wait1Msec(200);
		drive(0, 0);
	}

	if (mobileGoalIsOut) {
		moveMobileGoalInAuto();
		} else {
		motor[mobileGoal] = 50;
	}

	//if (allianceColor == BLUE_ALLIANCE) {
	//	theaterChaseTask(0, 0, 127, 50, 15000);
	//	} else {
	//	theaterChaseTask(127, 0, 0, 50, 15000);
	//}

	//////////////////////////////////////////////////////End of auto 3/////////////////////////////////////////////////////
	break;

case AUTONOMOUS_MODE_SKILLS:
	//////////////////////////////////////////////////////////Auto 4/////////////////////////////////////

	motor[roller] = 40;

	clearTimer(T2);

	while (time1(T2) < 3000) {

		if (time1(T2) < 300) {
			moveLiftUp(100, 400);
			} else {
			motor[liftL] = 0;
			motor[liftR] = 0;
		}

		//Drive
		if (time1(T2) > 500 && time1(T2) < 700) {
			motorReq[driveBL] = 90;
			motorReq[driveBR] = 90;
			motorReq[driveFL] = 90;
			motorReq[driveFR] = 90;
		}
		//Drive
		if (time1(T2) > 700) {
			autoDriveGyroPIDControl(0, 1300, 1, 0.00015, 0.5, 0.00002, 0.000000000079, 1.3);
		}

		//Move mobile goal lifter out
		if (!mobileGoalIsOut) {
			moveMobileGoalOut();
			} else {
			motor[mobileGoal] = -50;
		}

		wait1Msec(20);
	}

	drive(0, 0);
	clearTimer(T2);
	clearDriveEnc();

	//Move Mobile goal in and start stacking second cone
	while (time1(T2) < 2200) {

		if (time1(T2) < 300) {
			moveLiftUp(100, 500);
			} else {
			motor[liftL] = 0;
			motor[liftR] = 0;
		}
		if (mobileGoalIsOut) {
			moveMobileGoalIn();
			} else {
			motor[mobileGoal] = 50;
		}

		if (time1(T2) > 1600 && time1(T2) < 2050) {
			moveLiftDown(50, 400);
			//autoDriveGyroPIDControl(0, -1200, 1, 0.00015, 1.0, 1, 0.0000002, 0.4);
		}

		if (time1(T2) > 2050) {
			moveLiftUp(80, 600);
			//autoDriveGyroPIDControl(0, -1200, 1, 0.00015, 1.0, 1, 0.0000002, 0.4);
		}

		if (time1(T2) > 2050) {
			rollerOutake(-100, 150);
		}

		wait1Msec(20);
	}

	drive(0, 0);
	clearTimer(T2);
	clearDriveEnc();

	moveArmOut();
	motor[roller] = 120;

	while (time1(T2) < 1500) {
		autoDriveGyroPIDControl(0, 300, 1, 0.00015, 0.5, 1, 0.000079, 0.000003);

		moveLiftDown(50, 300);

		if (time1(T2) > 1000) {
			motor[roller] = 40;
			moveLiftUp(100, 550);
		}
		if (time1(T2) > 1300) {
			moveArmIn();
		}
	}

	drive(0, 0);
	clearTimer(T2);
	clearDriveEnc();

	if (allianceSide == LEFT) {
		while (time1(T2) < 1000) {
			moveLiftDown(20, 550);
			driveBackward(-580, -100);
		}
		} else if (allianceSide == RIGHT) {
		while (time1(T2) < 1000) {
			moveLiftDown(20, 550);
			driveBackward(-100, -580);
		}
	}

	drive(0, 0);
	clearTimer(T2);
	clearDriveEnc();

	wait1Msec(200);

	if (allianceSide == LEFT) {
		while (time1(T2) < 1000) {
			//turn
			autoGyroPIDControl(300, true);
		}
		} else if (allianceSide == RIGHT) {
		while (time1(T2) < 1000) {
			//turn
			autoGyroPIDControl(-300, true);
		}
	}

	drive(0, 0);
	clearTimer(T2);
	clearDriveEnc();

	while (time1(T2) < 1000) {
		//drive backward
		driveBackward(-100, -60);
	}

	drive(0, 0);
	clearTimer(T2);
	clearDriveEnc();

	if (allianceSide == LEFT) {
		while (time1(T2) < 2000) {
			//turn
			autoGyroPIDControl(1400, true);
		}
		} else if (allianceSide == RIGHT) {
		while (time1(T2) < 2000) {
			//turn
			autoGyroPIDControl(-1400, true);
		}
	}

	drive(0, 0);
	clearTimer(T2);
	clearDriveEnc();
	SensorValue[rollerEnc] = 0;

	while (time1(T2) < 2000) {
		moveMobileGoalOut();

		moveLiftUp(100, 500);

		rollerOutake(-100, 150);

		if (time1(T2) > 300) {
			drive(100, 100);
			wait1Msec(600);
			drive(0, 0);
		}
		wait1Msec(20);
	}

	drive(0, 0);
	clearTimer(T2);
	clearDriveEnc();

	while (time1(T2) < 600) {
		drive(-60, -60);
		wait1Msec(400);
		drive(0, 0);
	}
	moveMobileGoalInAuto();

	//if (allianceColor == BLUE_ALLIANCE) {
	//	theaterChaseTask(0, 0, 127, 50, 15000);
	//	} else {
	//	theaterChaseTask(127, 0, 0, 50, 15000);
	//}

	///////////////////////////////////////////////////////////////Auto 4 End///////////////////////////////////////////
	break;

case AUTONOMOUS_MODE_3_10:
	/////////////////////////////////////////////////////////////////////////////AUTO 5////////////////////////////////
	motor[roller] = 40;

	clearTimer(T2);

	while (time1(T2) < 3000) {

		if (time1(T2) < 300) {
			moveLiftUp(100, 400);
			} else {
			motor[liftL] = 0;
			motor[liftR] = 0;
		}

		//Drive
		if (time1(T2) > 500 && time1(T2) < 700) {
			motorReq[driveBL] = 90;
			motorReq[driveBR] = 90;
			motorReq[driveFL] = 90;
			motorReq[driveFR] = 90;
		}
		//Drive
		if (time1(T2) > 700) {
			autoDriveGyroPIDControl(0, 1300, 1, 0.00015, 0.5, 0.00002, 0.000000000079, 1.3);
		}

		//Move mobile goal lifter out
		if (!mobileGoalIsOut) {
			moveMobileGoalOut();
			} else {
			motor[mobileGoal] = -50;
		}

		wait1Msec(20);
	}

	drive(0, 0);
	clearTimer(T2);
	clearDriveEnc();

	//Move Mobile goal in and start stacking second cone
	while (time1(T2) < 2200) {

		if (time1(T2) < 300) {
			moveLiftUp(100, 500);
			} else {
			motor[liftL] = 0;
			motor[liftR] = 0;
		}
		if (mobileGoalIsOut) {
			moveMobileGoalIn();
			} else {
			motor[mobileGoal] = 50;
		}

		if (time1(T2) > 1600 && time1(T2) < 2050) {
			moveLiftDown(50, 400);
			//autoDriveGyroPIDControl(0, -1200, 1, 0.00015, 1.0, 1, 0.0000002, 0.4);
		}

		if (time1(T2) > 2050) {
			moveLiftUp(80, 600);
			//autoDriveGyroPIDControl(0, -1200, 1, 0.00015, 1.0, 1, 0.0000002, 0.4);
		}

		if (time1(T2) > 2050) {
			rollerOutake(-100, 150);
		}

		wait1Msec(20);
	}

	drive(0, 0);
	clearTimer(T2);
	clearDriveEnc();

	//Start intaking second cone
	moveArmOut();
	motor[roller] = 120;

	while (time1(T2) < 1500) {
		autoDriveGyroPIDControl(0, 260, 1, 0.00015, 0.5, 1, 0.000079, 0.000003);

		moveLiftDown(50, 300);

		if (time1(T2) > 1000) {
			motor[roller] = 40;
			moveLiftUp(100, 550);
		}
		if (time1(T2) > 1300) {
			moveArmIn();
		}
	}

	drive(0, 0);
	clearTimer(T2);
	clearDriveEnc();
	SensorValue[rollerEnc] = 0;

	//Start stacking second cone
	while (time1(T2) < 1000) {
		if (time1(T2) < 750) {
			moveLiftDown(80, 400);
		}

		if (time1(T2) > 750) {
			moveLiftUp(80, 600);
		}

		if (time1(T2) > 750) {
			rollerOutake(-100, 150);
		}

		wait1Msec(20);
	}

	drive(0, 0);
	clearTimer(T2);
	clearDriveEnc();


	//Start intaking third cone
	moveArmOut();
	motor[roller] = 127;

	while (time1(T2) < 1500) {
		autoDriveGyroPIDControl(0, 230, 1, 0.00015, 0.5, 1, 0.000079, 0.000003);

		moveLiftDown(50, 300);

		if (time1(T2) > 1000) {
			motor[roller] = 40;
			moveLiftUp(100, 550);
		}
		if (time1(T2) > 1300) {
			moveArmIn();
		}
	}

	drive(0, 0);
	clearTimer(T2);
	clearDriveEnc();

	while (time1(T2) < 1750) {
		moveLiftDown(20, 550);
		driveBackward(-100, -700);
	}

	drive(0, 0);
	clearTimer(T2);
	clearDriveEnc();

	wait1Msec(150);

	if (allianceSide == LEFT) {
		while (time1(T2) < 2000) {
			//turn
			autoGyroPIDControl(1850, true);
		}
		} else if (allianceSide == RIGHT) {
		while (time1(T2) < 2000) {
			//turn
			autoGyroPIDControl(-1850, true);
		}
	}

	drive(0, 0);
	clearTimer(T2);
	clearDriveEnc();

	if (allianceSide == LEFT) {
		while (time1(T2) < 1500) {
			moveMobileGoalOut();
			moveLiftUp(80, 500);
			motor[roller] = -100;

			if (time1(T2) < 1000) {
				drive(85, 45);
				} else {
				drive(0, 0);
			}
		}
		} else if (allianceSide == RIGHT) {
		while (time1(T2) < 1500) {
			moveMobileGoalOut();
			moveLiftUp(80, 500);
			motor[roller] = -100;

			if (time1(T2) < 1000) {
				drive(45, 85);
				} else {
				drive(0, 0);
			}
		}
	}

	drive(0, 0);
	clearTimer(T2);
	clearDriveEnc();

	if (allianceSide == LEFT) {
		while (time1(T2) < 300) {
			drive(-127, -50);
			wait1Msec(200);
			drive(0, 0);
		}
		} else if (allianceSide == RIGHT) {
		while (time1(T2) < 300) {
			drive(-50, -127);
			wait1Msec(200);
			drive(0, 0);
		}
	}

	if (mobileGoalIsOut) {
		moveMobileGoalInAuto();
		} else {
		motor[mobileGoal] = 50;
	}

	//if (allianceColor == BLUE_ALLIANCE) {
	//	theaterChaseTask(0, 0, 127, 50, 15000);
	//	} else {
	//	theaterChaseTask(127, 0, 0, 50, 15000);
	//}

	//////////////////////////////////////////////////////////Auto 5 end/////////////////////////////////////////////
	break;

case AUTONOMOUS_MODE_STACK_PIPE:

	if (allianceSide == LEFT) {
		clearTimer(T2);
		while (time1(T2) < 800) {
			autoGyroPIDControl(-250, true);
		}

		clearTimer(T2);
		drive(0, 0);

		while (time1(T2) < 2400) {
			drive(127, 127);
		}

		drive(0, 0);

		} else if (allianceSide == RIGHT) {
		clearTimer(T2);
		while (time1(T2) < 800) {
			autoGyroPIDControl(250, true);
		}

		clearTimer(T2);
		drive(0, 0);

		while (time1(T2) < 2400) {
			drive(127, 127);
		}

		drive(0, 0);
	}

	//if (allianceColor == BLUE_ALLIANCE) {
	//	theaterChaseTask(0, 0, 127, 50, 15000);
	//	} else {
	//	theaterChaseTask(127, 0, 0, 50, 15000);
	//}

	break;

case AUTONOMOUS_MODE_STACK_BACK:

	break;

case AUTONOMOUS_MODE_DRIVE_BLOCK:


*/
