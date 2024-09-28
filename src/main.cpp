#include "main.h"
/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Testing motors...");
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */


// ----------------
// controller
pros::Controller master(pros::E_CONTROLLER_MASTER);

// these are the motor groups for the left and right wheels.
// each one is in the order of front wheel, middle wheel, back wheel.
pros::MotorGroup left_mg({16, 15, 14, 13});
pros::MotorGroup right_mg({-17, -18, -19, -20});

// below are abstracted functions that get the analog values
// for the controller. these exist so that they can be stored
// as std::function variables, which can be reassigned with a button press.
// the current purpose of this is to implement both arcade and tank controls,
// so the driver can get a feel for which one will make more sense for the robot.

// tank control schemes for the robot.
void tank_controls() {
	left_mg.move(master.get_analog(ANALOG_LEFT_Y));
	right_mg.move(master.get_analog(ANALOG_RIGHT_Y));
}

// arcade control schemes for the robot.
void arcade_controls() {
	std::int32_t left_y = master.get_analog(ANALOG_LEFT_Y);
	std::int32_t right_x = master.get_analog(ANALOG_RIGHT_X);

	left_mg.move(left_y + right_x);
	right_mg.move(left_y - right_x);
}

void opcontrol() { 
	// the function variable that will describe the current control scheme.
	std::function<void()> controls = arcade_controls;

	while (true) {
		// moves the motor groups based on the current control scheme.
		controls();

		// switch to tank controls on A press.
		if (master.get_digital(DIGITAL_A)) {
			controls = tank_controls;
		}

		// switch to arcade controls on B press.
		if (master.get_digital(DIGITAL_B)) {
			controls = arcade_controls;
		}

		pros::delay(15);
	}
}