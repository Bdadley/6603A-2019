#include "main.h"
#include "globals.h"

pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::Imu imu (17); //Initialize the inertial sensor


int phase = 0; //What part of the auton selection the screen is currently on (Team selection, Spot selection, etc.)
int teamIndex = -1; //Tempvar used for screen selection
int positionIndex = 1; //Tempvar used for screen selection

int team = 0; //What team you are on (-1 = Red, 1 = Blue)
int position = 0; //What spot you are taking (Big Zone, Small Zone, None)






void initialize() {
	//pros::lcd::initialize();
	frontLeft.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	frontRight.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	backLeft.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	backRight.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

	arm.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	angleAdjuster.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

	intake1.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	intake2.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
}


/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {

}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */

 lv_theme_t* alien;
 lv_obj_t* scr;
 lv_color_t transp;
 static lv_style_t fontStyle;
 lv_obj_t* redBtn;
 lv_obj_t* blueBtn;
 lv_obj_t* skillsBtn;
 static lv_style_t redStyleRel;
 static lv_style_t redStylePr;
 static lv_style_t blueStyleRel;
 static lv_style_t blueStylePr;
 static lv_style_t skillsStyleRel;
 static lv_style_t skillsStylePr;
 static lv_style_t spotSelectRel;
 static lv_style_t spotSelectPr;
 static lv_style_t spinnerStyle;
 lv_obj_t* redLabel;
 lv_obj_t* blueLabel;
 lv_obj_t* teamLabel;
 lv_obj_t* skillsLabel;
 lv_obj_t* spinner;
 lv_obj_t* logo;
 int pickPhase = 1;

 static lv_res_t teamSelect(lv_obj_t* btn)
 {
 	uint8_t id = lv_obj_get_free_num(btn);
 		if (pickPhase == 1)
 		{
 				if (id == 0)
 				{
 						pickPhase = 2;
 						myTeam = -1;
 						lv_obj_clean(skillsBtn);
 						lv_obj_del(skillsBtn);;
 						lv_btn_set_style(redBtn, LV_BTN_STYLE_REL, &spotSelectRel);
 						lv_btn_set_style(blueBtn, LV_BTN_STYLE_REL, &spotSelectRel);
 						lv_btn_set_style(redBtn, LV_BTN_STYLE_PR, &spotSelectPr);
 						lv_btn_set_style(blueBtn, LV_BTN_STYLE_PR, &spotSelectPr);
 						lv_obj_set_pos(redBtn, 270, 60);
 						lv_obj_set_pos(blueBtn, 270, 135);
 						lv_label_set_text(redLabel, "Small Zone");
 						lv_label_set_text(blueLabel, "Big Zone");
 						LV_IMG_DECLARE(smallLogoRed);
 						logo = lv_img_create(scr, NULL);
 						lv_obj_set_pos(logo, 35, 0);
 						lv_img_set_src(logo, &smallLogoRed);
 				}
 				else if (id == 1)
 				{
 						myTeam = 1;
 						pickPhase = 2;
 						lv_obj_del(skillsBtn);
 						lv_obj_set_pos(redBtn, 270, 70);
 						lv_obj_set_pos(blueBtn, 270, 155);
 						lv_btn_set_style(redBtn, LV_BTN_STYLE_REL, &spotSelectRel);
 						lv_btn_set_style(blueBtn, LV_BTN_STYLE_REL, &spotSelectRel);
 						lv_btn_set_style(redBtn, LV_BTN_STYLE_PR, &spotSelectPr);
 						lv_btn_set_style(blueBtn, LV_BTN_STYLE_PR, &spotSelectPr);
 						lv_obj_set_pos(redBtn, 270, 40);
 						lv_obj_set_pos(blueBtn, 270, 125);
 						lv_label_set_text(redLabel, "Small Zone");
 						lv_label_set_text(blueLabel, "Big Zone");
 						LV_IMG_DECLARE(smallLogoBlue);
 						logo = lv_img_create(scr, NULL);
 						lv_obj_set_pos(logo, 35, 0);
 						lv_img_set_src(logo, &smallLogoBlue);

 				}
 				else if (id == 2)
 				{
 						selectedAuton = 4;
 						pickPhase = 3;
 						lv_obj_clean(redBtn);
 						lv_obj_clean(blueBtn);
 						lv_obj_clean(skillsBtn);
 						lv_obj_del(redBtn);
 						lv_obj_del(blueBtn);
 						lv_obj_del(skillsBtn);

 						//Create A Spinner
 						spinner = lv_preload_create(scr, NULL);
 						lv_obj_set_pos(spinner, 190, 65);
 						lv_obj_set_size(spinner, 100, 100);
 						lv_label_set_text(teamLabel, "Skills");
 						lv_obj_set_pos(teamLabel, 220, 180);

 						//Apply spinner theme
 						lv_preload_set_style(spinner, LV_PRELOAD_STYLE_MAIN, &spinnerStyle);
 				}
 		}
 		else if (pickPhase == 2)
 		{
 				if (id == 0)
 				{
 						selectedAuton = 2;
 						pickPhase = 3;
 						lv_label_set_text(teamLabel, "Small Zone");
 						lv_obj_clean(redBtn);
 						lv_obj_clean(blueBtn);
 						lv_obj_del(redBtn);
 						lv_obj_del(blueBtn);

 				}
 				else if (id == 1)
 				{
 						selectedAuton = 3;
 						pickPhase = 3;
 						lv_label_set_text(teamLabel, "Big Zone");
 						lv_obj_clean(redBtn);
 						lv_obj_clean(blueBtn);
 						lv_obj_del(redBtn);
 						lv_obj_del(blueBtn);
 				}

 				//Create A Spinner
 				spinner = lv_preload_create(scr, NULL);
 				lv_obj_set_pos(spinner, 320, 65);
 				lv_obj_set_size(spinner, 100, 100);

 				//Apply spinner theme
 				lv_preload_set_style(spinner, LV_PRELOAD_STYLE_MAIN, &spinnerStyle);
 		}
 		return LV_RES_OK;
 }

void competition_initialize()
{


		//Spinner style
		//Write the screen to a shorter variable
		scr = lv_scr_act();

		lv_style_copy(&spinnerStyle, &lv_style_plain);
		spinnerStyle.line.width = 10;
		spinnerStyle.body.border.width = 10;
		spinnerStyle.line.color = LV_COLOR_WHITE;
		spinnerStyle.body.border.color = LV_COLOR_BLACK;
		spinnerStyle.body.padding.hor = 0;


		//Create init spinner
		lv_obj_t* loadSpinner = lv_preload_create(scr, NULL);
		lv_obj_set_pos(loadSpinner, 190, 65);
		lv_obj_set_size(loadSpinner, 100, 100);

		//Apply spinner theme
		lv_preload_set_style(loadSpinner, LV_PRELOAD_STYLE_MAIN, &spinnerStyle);
		potentiometer.calibrate();
		imu.reset();
		while(imu.is_calibrating())
		{
		}
		lv_obj_clean(loadSpinner);
		lv_obj_del(loadSpinner);


		//Auton Selector
		//Create and apply a style for each team selection button

		//Red button released style
		lv_style_copy(&redStyleRel, &lv_style_plain);
		redStyleRel.body.border.color = LV_COLOR_RED;
		redStyleRel.body.border.width = 2;
		redStyleRel.body.main_color = transp;
		redStyleRel.body.grad_color = transp;
		redStyleRel.body.shadow.width = 0;
		redStyleRel.body.shadow.type = LV_SHADOW_BOTTOM;
		redStyleRel.body.radius = LV_RADIUS_CIRCLE;
		redStyleRel.text.color = LV_COLOR_WHITE;

		//Red button press style
		lv_style_copy(&redStylePr, &redStyleRel);
		redStylePr.body.main_color = LV_COLOR_RED;
		redStylePr.body.grad_color = LV_COLOR_RED;
		redStylePr.text.color = LV_COLOR_RED;

		//Blue button released style
		lv_style_copy(&blueStyleRel, &redStyleRel);
		blueStyleRel.body.border.color = LV_COLOR_BLUE;

		//Blue button press style
		lv_style_copy(&blueStylePr, &blueStyleRel);
		blueStylePr.body.main_color = LV_COLOR_BLUE;
		blueStylePr.body.grad_color = LV_COLOR_BLUE;
		blueStylePr.text.color = LV_COLOR_BLUE;

		//Skills released style
		lv_style_copy(&skillsStyleRel, &redStyleRel);
		skillsStyleRel.body.border.color = LV_COLOR_SILVER;

		//Skills press style
		lv_style_copy(&skillsStylePr, &skillsStyleRel);
		skillsStylePr.body.main_color = LV_COLOR_SILVER;
		skillsStylePr.body.grad_color = LV_COLOR_SILVER;
		blueStylePr.text.color = LV_COLOR_SILVER;

		//Spot selection released style
		lv_style_copy(&spotSelectRel, &redStyleRel);
		spotSelectRel.body.border.color = LV_COLOR_WHITE;

		//Spot selection press style
		lv_style_copy(&spotSelectPr, &spotSelectRel);
		spotSelectPr.body.main_color = LV_COLOR_WHITE;
		spotSelectPr.body.grad_color = LV_COLOR_WHITE;

		//Set Theme
		alien = lv_theme_alien_init(300, NULL);
		lv_theme_set_current(alien);
		lv_style_scr.body.main_color = LV_COLOR_BLACK;
		lv_style_scr.body.grad_color = LV_COLOR_BLACK; // LV_COLOR_MAKE(0x17, 0x17, 0x17);


		//Create a transparent color
		transp.full = 0;


		//Create a custom font style
		lv_style_copy(&fontStyle, &lv_style_plain);
		fontStyle.text.color = LV_COLOR_WHITE;
		//lv_label_set_style(teamLabel, LV_LABEL_STYLE_MAIN, &fontStyle);


		//Create the team selection buttons
		redBtn = lv_btn_create(scr, NULL);
		lv_obj_set_free_num(redBtn, 0);
		blueBtn = lv_btn_create(scr, NULL);
		lv_obj_set_free_num(blueBtn, 1);
		skillsBtn = lv_btn_create(scr, NULL);
		lv_obj_set_free_num(skillsBtn, 2);


		//Set button callback events
		lv_btn_set_action(redBtn, LV_BTN_ACTION_CLICK, teamSelect);
		lv_btn_set_action(blueBtn, LV_BTN_ACTION_CLICK, teamSelect);
		lv_btn_set_action(skillsBtn, LV_BTN_ACTION_CLICK, teamSelect);


		//Create the teamLabel
		teamLabel = lv_label_create(scr, NULL);
		lv_obj_set_pos(teamLabel, 325, 180);
		lv_label_set_align(teamLabel, LV_ALIGN_CENTER);
		lv_label_set_text(teamLabel, "");


		//Apply button styles
		lv_btn_set_style(redBtn, LV_BTN_STYLE_REL ,&redStyleRel);
		lv_btn_set_style(redBtn, LV_BTN_STYLE_PR, &redStylePr);
		lv_btn_set_style(blueBtn, LV_BTN_STYLE_REL, &blueStyleRel);
		lv_btn_set_style(blueBtn, LV_BTN_STYLE_PR, &blueStylePr);
		lv_btn_set_style(skillsBtn, LV_BTN_STYLE_REL, &skillsStyleRel);
		lv_btn_set_style(skillsBtn, LV_BTN_STYLE_PR, &skillsStylePr);


		//Insert labels onto the team selection buttons
		redLabel = lv_label_create(redBtn, NULL);
		blueLabel = lv_label_create(blueBtn, NULL);
		skillsLabel = lv_label_create(skillsBtn, NULL);


		//Realign the team selection buttons
		lv_obj_set_size(redBtn, 200, 70);
		lv_obj_set_size(blueBtn, 200, 70);
		lv_obj_set_size(skillsBtn, 200, 70);

		lv_obj_set_pos(redBtn, 270, 10);
		lv_obj_set_pos(blueBtn, 270, 85);
		lv_obj_set_pos(skillsBtn, 270, 160);

		lv_label_set_text(redLabel, "Red");
		lv_label_set_text(blueLabel, "Blue");
		lv_label_set_text(skillsLabel, "Skills");
}
