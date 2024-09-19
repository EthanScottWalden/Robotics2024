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
pros::MotorGroup left_mg({19, 20, 18});
pros::MotorGroup right_mg({12, 11, 13});

// below are abstracted functions that get the analog values
// for the controller. these exist so that they can be stored
// as std::function variables, which can be reassigned with a button press.
// the current purpose of this is to implement both arcade and tank controls,
// so the driver can get a feel for which one will make more sense for the robot.

// gets analog left y voltage
std::int32_t get_left_y() {
	return master.get_analog(ANALOG_LEFT_Y);
}

// gets analog left x voltage
std::int32_t get_left_x() {
	return master.get_analog(ANALOG_LEFT_X);
}

// gets analog right y voltage
std::int32_t get_right_y() {
	return master.get_analog(ANALOG_RIGHT_Y);
}

// gets analog left x voltage
std::int32_t get_right_x() {
	return master.get_analog(ANALOG_RIGHT_X);
}

void opcontrol() { 
	// the function variables that describe whether the x or y values will be used to move.
	std::function<int32_t()> left_input = get_left_y;
	std::function<int32_t()> right_input = get_right_y;

	while (true) {
		// moves the motor groups based on the input functions.
		left_mg.move(left_input());
		right_mg.move(right_input());

		// switch to tank controls on A press.
		if (master.get_digital(DIGITAL_A)) {
			right_input = get_right_y;
		}

		// switch to arcade controls on B press.
		if (master.get_digital(DIGITAL_B)) {
			right_input = get_right_x;
		}

		pros::delay(15);
	}
}