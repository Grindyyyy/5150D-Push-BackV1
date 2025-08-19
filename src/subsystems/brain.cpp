#include "subsystems/brain.hpp"
#include "pros/misc.hpp"

#include <iostream>

static lv_style_t button_pressed;
static lv_style_t blue_button_released;
static lv_style_t red_button_released;
static lv_style_t skills_button_released;
static lv_style_t button_selected;
static lv_style_t dynamic_battery;
static lv_style_t background_color;

static lv_obj_t * main_menu;

static lv_obj_t * battery_label;
static lv_obj_t * panel;

static lv_obj_t * menu_label;
static lv_obj_t * label;

static lv_obj_t * red_selected_label;
static lv_obj_t * blue_selected_label;
static lv_obj_t * skills_selected_label;

static lv_obj_t * slider;

static lv_obj_t * cont;
static lv_obj_t * image;

static lv_obj_t * overlay;

static lv_obj_t * menu;

static lv_obj_t * coords_x;
static lv_obj_t * coords_y;

// static buttons
static lv_obj_t * blue_ring_elim;
static lv_obj_t * blue_goal_elim;
static lv_obj_t * blue_solo_wp;

static lv_obj_t * red_ring_elim;
static lv_obj_t * red_goal_elim;
static lv_obj_t * red_solo_wp;

static lv_obj_t * risky_skills;
static lv_obj_t * safe_skills;

// static pages

static lv_obj_t * blue_selector_page;
static lv_obj_t * red_selector_page;
static lv_obj_t * skills_selector_page;

static int selected;

static void slider_event_cb(lv_event_t * e){
    lv_obj_t * slider = lv_event_get_target(e);

    /*Refresh the text*/
	std::cout << lv_slider_get_value(slider) << std::endl;
    lv_label_set_text_fmt(label, "Freak Meter: %d%%", lv_slider_get_value(slider));
    lv_obj_align_to(label, slider, LV_ALIGN_OUT_TOP_MID, 0, 0);

}

void update_battery_percent() {
	auto capacity = pros::battery::get_capacity();

	lv_label_set_text_fmt(battery_label, "Battery Percent: %d%%", (int)pros::battery::get_capacity());
	lv_obj_align_to(battery_label,lv_layer_top(), LV_ALIGN_TOP_RIGHT,-5,5);

	if(capacity > 25){
		lv_img_set_src(image, LV_SYMBOL_BATTERY_2);
		if(capacity > 50){
			lv_img_set_src(image, LV_SYMBOL_BATTERY_3);
			if(capacity > 75){
				lv_img_set_src(image, LV_SYMBOL_BATTERY_FULL);
				lv_style_set_text_color(&dynamic_battery, lv_color_make(191,255,163));
			}
			else{
				lv_img_set_src(image, LV_SYMBOL_BATTERY_3);
				lv_style_set_text_color(&dynamic_battery, lv_color_make(221,255,153));
			}
		}
		else{
			lv_img_set_src(image, LV_SYMBOL_BATTERY_2);
			lv_style_set_text_color(&dynamic_battery, lv_color_make(255,228,156));
		}
	}
	else{
		lv_img_set_src(image, LV_SYMBOL_WARNING);
		lv_style_set_text_color(&dynamic_battery, lv_color_make(255,133,133));
	}
	
	lv_obj_align_to(image,lv_layer_top(),LV_ALIGN_TOP_RIGHT,-160,5);

}

static void remove_button_style(){
	lv_obj_remove_style(red_solo_wp,&button_selected,0);
	lv_obj_remove_style(red_ring_elim,&button_selected,0);
	lv_obj_remove_style(red_goal_elim,&button_selected,0);
	lv_obj_remove_style(blue_ring_elim,&button_selected,0);
	lv_obj_remove_style(blue_goal_elim,&button_selected,0);
	lv_obj_remove_style(blue_solo_wp,&button_selected,0);
	lv_obj_remove_style(risky_skills,&button_selected,0);
	lv_obj_remove_style(safe_skills,&button_selected,0);
}

static void selector_button_manager(lv_event_t * e){
	lv_event_code_t code = e->code;
    lv_obj_t * btn = lv_event_get_target(e); // Get the button object
    if (code == LV_EVENT_PRESSED) {
		if(btn == red_ring_elim){
			selected = 1;
			lv_label_set_text(red_selected_label, "Selected Auton: Red 7 Ring Elim");
			lv_label_set_text(blue_selected_label, "Selected Auton: Red 7 Ring Elim");
			lv_obj_align_to(red_selected_label,red_selector_page,LV_ALIGN_BOTTOM_MID,0,0);
			remove_button_style();
			lv_obj_add_style(red_ring_elim,&button_selected,0);
		}
		else if(btn == red_solo_wp){
			selected = 2;
			lv_label_set_text(red_selected_label, "Selected Auton: Red Solo WP");
			lv_label_set_text(blue_selected_label, "Selected Auton: Red Solo WP");
			lv_obj_align_to(red_selected_label,red_selector_page,LV_ALIGN_BOTTOM_MID,0,0);
			remove_button_style();
			lv_obj_add_style(red_solo_wp,&button_selected,0);
		}
		else if(btn == red_goal_elim){
			selected = 3;
			lv_label_set_text(red_selected_label, "Selected Auton: Red Goal Elim");
			lv_label_set_text(blue_selected_label, "Selected Auton: Red Goal Elim");
			lv_obj_align_to(red_selected_label,red_selector_page,LV_ALIGN_BOTTOM_MID,0,0);
			remove_button_style();
			lv_obj_add_style(red_goal_elim,&button_selected,0);
		}
		else if(btn == blue_ring_elim){
			selected = 4;
			lv_label_set_text(blue_selected_label, "Selected Auton: Blue Ring Elim");
			lv_label_set_text(red_selected_label, "Selected Auton: Blue Ring Elim");
			lv_obj_align_to(blue_selected_label,blue_selector_page,LV_ALIGN_BOTTOM_MID,0,0);
			remove_button_style();
			lv_obj_add_style(blue_ring_elim,&button_selected,0);
		}
		else if(btn == blue_solo_wp){
			selected = 5;
			lv_label_set_text(blue_selected_label, "Selected Auton: Blue Solo WP");
			lv_label_set_text(red_selected_label, "Selected Auton: Blue Solo WP");
			lv_obj_align_to(blue_selected_label,blue_selector_page,LV_ALIGN_BOTTOM_MID,0,0);
			remove_button_style();
			lv_obj_add_style(blue_solo_wp,&button_selected,0);
		}
		else if(btn == blue_goal_elim){
			selected = 6;
			lv_label_set_text(blue_selected_label, "Selected Auton: Blue Goal Elim");
			lv_label_set_text(red_selected_label, "Selected Auton: Blue Goal Elim");
			lv_obj_align_to(blue_selected_label,blue_selector_page,LV_ALIGN_BOTTOM_MID,0,0);
			remove_button_style();
			lv_obj_add_style(blue_goal_elim,&button_selected,0);
		}
		else if(btn == risky_skills){
			selected = 7;
			lv_label_set_text(skills_selected_label, "Selected Auton: Risky Skills");
			lv_label_set_text(red_selected_label, "Selected Auton: Risky Skills");
			lv_label_set_text(blue_selected_label, "Selected Auton: Risky Skills");
			lv_obj_align_to(skills_selected_label,skills_selector_page,LV_ALIGN_BOTTOM_MID,0,0);
			remove_button_style();
			lv_obj_add_style(risky_skills,&button_selected,0);
		}
		else if(btn == safe_skills){
			selected = 8;
			lv_label_set_text(skills_selected_label, "Selected Auton: Safe Skills");
			lv_label_set_text(red_selected_label, "Selected Auton: Safe Skills");
			lv_label_set_text(blue_selected_label, "Selected Auton: Safe Skills");
			lv_obj_align_to(skills_selected_label,skills_selector_page,LV_ALIGN_BOTTOM_MID,0,0);
			remove_button_style();
			lv_obj_add_style(safe_skills,&button_selected,0);
		}
	}
}

void initialize_brain(){
	lv_style_init(&button_pressed);
	lv_style_set_bg_color(&button_pressed,lv_palette_main(LV_PALETTE_ORANGE));
	lv_style_set_text_color(&button_pressed,lv_palette_main(LV_PALETTE_NONE));

	lv_style_init(&red_button_released);
	lv_style_set_bg_color(&red_button_released,lv_palette_main(LV_PALETTE_RED));
	lv_style_set_text_color(&red_button_released,lv_palette_main(LV_PALETTE_NONE));

	lv_style_init(&blue_button_released);
	lv_style_set_bg_color(&blue_button_released,lv_palette_main(LV_PALETTE_BLUE));
	lv_style_set_text_color(&blue_button_released,lv_palette_main(LV_PALETTE_NONE));

	lv_style_init(&skills_button_released);
	lv_style_set_bg_color(&skills_button_released,lv_palette_main(LV_PALETTE_YELLOW));
	lv_style_set_text_color(&skills_button_released,lv_palette_main(LV_PALETTE_NONE));

	lv_style_init(&button_selected);
	lv_style_set_bg_color(&button_selected,lv_palette_main(LV_PALETTE_ORANGE));
	lv_style_set_text_color(&button_selected,lv_palette_main(LV_PALETTE_NONE));

	lv_style_init(&dynamic_battery);

	lv_style_init(&background_color);
	lv_style_set_bg_color(&background_color,lv_color_make(2, 18, 41));

	coords_x = lv_label_create(lv_layer_top());
    lv_label_set_text(coords_x, "Odometry coords: NULL");
	lv_obj_add_style(coords_x,&dynamic_battery,0);
    lv_obj_align_to(coords_x, lv_layer_top(), LV_ALIGN_BOTTOM_MID,10,-35);

	coords_y = lv_label_create(lv_layer_top());
    lv_label_set_text(coords_y, "Odometry coords: NULL");
	lv_obj_add_style(coords_y,&dynamic_battery,0);
    lv_obj_align_to(coords_y, lv_layer_top(), LV_ALIGN_BOTTOM_MID,10,-15);

	menu = lv_menu_create(lv_scr_act());
	lv_obj_set_size(menu, lv_disp_get_hor_res(NULL) , lv_disp_get_ver_res(NULL));
	lv_obj_add_style(menu,&background_color,0);
    lv_obj_center(menu);

	// sub-pages
	red_selector_page = lv_menu_page_create(menu, NULL);

	// red container
	cont = lv_menu_cont_create(red_selector_page);

	// subpage buttons
	red_ring_elim = lv_btn_create(cont);

	lv_obj_add_style(red_ring_elim,&button_pressed,LV_STATE_PRESSED);
	lv_obj_add_style(red_ring_elim,&red_button_released,0);
	lv_obj_set_size(red_ring_elim, 140, 50);
	lv_obj_align(red_ring_elim, LV_ALIGN_CENTER, 10, 10);

	label = lv_label_create(red_ring_elim);     
    lv_label_set_text(label, "Red Ring Elim");              
    lv_obj_center(label);

	lv_obj_add_event_cb(red_ring_elim,selector_button_manager, LV_EVENT_PRESSED,NULL);

	red_solo_wp = lv_btn_create(cont);

	lv_obj_add_style(red_solo_wp,&button_pressed,LV_STATE_PRESSED);
	lv_obj_add_style(red_solo_wp,&red_button_released,0);
	lv_obj_set_size(red_solo_wp, 140, 50);
	lv_obj_align(red_solo_wp, LV_ALIGN_CENTER, 10, 10);

	label = lv_label_create(red_solo_wp);      
    lv_label_set_text(label, "Red Solo WP");          
    lv_obj_center(label);

	lv_obj_add_event_cb(red_solo_wp,selector_button_manager, LV_EVENT_PRESSED,NULL);

	red_goal_elim = lv_btn_create(cont);

	lv_obj_add_style(red_goal_elim,&button_pressed,LV_STATE_PRESSED);
	lv_obj_add_style(red_goal_elim,&red_button_released,0);
	lv_obj_set_size(red_goal_elim, 140, 50);
	lv_obj_align(red_goal_elim, LV_ALIGN_CENTER, 10, 10);

	label = lv_label_create(red_goal_elim);  
    lv_label_set_text(label, "Red Goal Elim");  
    lv_obj_center(label);

	lv_obj_add_event_cb(red_goal_elim,selector_button_manager, LV_EVENT_PRESSED,NULL);

	// selected label

	red_selected_label = lv_label_create(red_selector_page);
	lv_label_set_text(red_selected_label, "Selected Auton: NONE");
	lv_obj_align_to(red_selected_label,red_selector_page,LV_ALIGN_BOTTOM_MID,0,0);

	// blue
	blue_selector_page = lv_menu_page_create(menu, NULL);

	// blue container
	cont = lv_menu_cont_create(blue_selector_page);

	// subpage buttons
	blue_ring_elim = lv_btn_create(cont);

	lv_obj_add_style(blue_ring_elim,&button_pressed,LV_STATE_PRESSED);
	lv_obj_add_style(blue_ring_elim,&blue_button_released,0);
	lv_obj_set_size(blue_ring_elim, 140, 50);
	lv_obj_align(blue_ring_elim, LV_ALIGN_CENTER, 10, 10);

	label = lv_label_create(blue_ring_elim);     
    lv_label_set_text(label, "Blue Ring Elim");              
    lv_obj_center(label);

	lv_obj_add_event_cb(blue_ring_elim,selector_button_manager, LV_EVENT_PRESSED,NULL);

	blue_solo_wp = lv_btn_create(cont);

	lv_obj_add_style(blue_solo_wp,&button_pressed,LV_STATE_PRESSED);
	lv_obj_add_style(blue_solo_wp,&blue_button_released,0);
	lv_obj_set_size(blue_solo_wp, 140, 50);
	lv_obj_align(blue_solo_wp, LV_ALIGN_CENTER, 10, 10);

	label = lv_label_create(blue_solo_wp);      
    lv_label_set_text(label, "Blue Solo WP");          
    lv_obj_center(label);

	lv_obj_add_event_cb(blue_solo_wp,selector_button_manager, LV_EVENT_PRESSED,NULL);

	blue_goal_elim = lv_btn_create(cont);

	lv_obj_add_style(blue_goal_elim,&button_pressed,LV_STATE_PRESSED);
	lv_obj_add_style(blue_goal_elim,&blue_button_released,0);
	lv_obj_set_size(blue_goal_elim, 140, 50);
	lv_obj_align(blue_goal_elim, LV_ALIGN_CENTER, 10, 10);

	label = lv_label_create(blue_goal_elim);  
    lv_label_set_text(label, "Blue Goal Elim");  
    lv_obj_center(label);

	lv_obj_add_event_cb(blue_goal_elim,selector_button_manager, LV_EVENT_PRESSED,NULL);

	// selected label

	blue_selected_label = lv_label_create(blue_selector_page);
	lv_label_set_text(blue_selected_label, "Selected Auton: NONE");
	lv_obj_align_to(blue_selected_label,blue_selector_page,LV_ALIGN_BOTTOM_MID,0,0);

	skills_selector_page = lv_menu_page_create(menu, NULL);

	// skills container
	cont = lv_menu_cont_create(skills_selector_page);

	// subpage buttons
	risky_skills = lv_btn_create(cont);

	lv_obj_add_style(risky_skills,&button_pressed,LV_STATE_PRESSED);
	lv_obj_add_style(risky_skills,&skills_button_released,0);
	lv_obj_set_size(risky_skills, 140, 50);
	lv_obj_align(risky_skills, LV_ALIGN_CENTER, 10, 10);

	label = lv_label_create(risky_skills);     
    lv_label_set_text(label, "Risky Skills");              
    lv_obj_center(label);

	lv_obj_add_event_cb(risky_skills,selector_button_manager, LV_EVENT_PRESSED,NULL);

	safe_skills = lv_btn_create(cont);

	lv_obj_add_style(safe_skills,&button_pressed,LV_STATE_PRESSED);
	lv_obj_add_style(safe_skills,&skills_button_released,0);
	lv_obj_set_size(safe_skills, 140, 50);
	lv_obj_align(safe_skills, LV_ALIGN_CENTER, 10, 10);

	label = lv_label_create(safe_skills);      
    lv_label_set_text(label, "Safe Skills");          
    lv_obj_center(label);

	lv_obj_add_event_cb(safe_skills,selector_button_manager, LV_EVENT_PRESSED,NULL);

	skills_selected_label = lv_label_create(skills_selector_page);
	lv_label_set_text(skills_selected_label, "Selected Auton: NONE");
	lv_obj_align_to(skills_selected_label,skills_selector_page,LV_ALIGN_BOTTOM_MID,0,0);


	lv_obj_t * diagnostic_page = lv_menu_page_create(menu, NULL);

	lv_obj_t * freak_meter_page = lv_menu_page_create(menu, NULL);

	cont = lv_menu_cont_create(freak_meter_page);
	slider = lv_slider_create(cont);
    lv_obj_set_width(slider, 200);                          /*Set the width*/                                 /*Align to the center of the parent (screen)*/
    lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
	lv_obj_align(slider,LV_ALIGN_CENTER,0,0);     /*Assign an event function*/

	
    /*Create a label above the slider*/
    label = lv_label_create(cont);
    lv_label_set_text(label, "Freak Meter: 0%");
    lv_obj_align_to(label, slider, LV_ALIGN_OUT_TOP_MID, 0, -5);
    

	lv_obj_t * main_menu = lv_menu_page_create(menu, NULL);

	cont = lv_menu_cont_create(main_menu);
	menu_label = lv_label_create(cont);
	lv_label_set_text(menu_label, "Red Autons");
	lv_menu_set_load_page_event(menu, cont, red_selector_page);

	cont = lv_menu_cont_create(main_menu);
	menu_label = lv_label_create(cont);
	lv_label_set_text(menu_label, "Blue Autons");
	lv_menu_set_load_page_event(menu, cont, blue_selector_page);

	cont = lv_menu_cont_create(main_menu);
	menu_label = lv_label_create(cont);
	lv_label_set_text(menu_label, "Skills");
	lv_menu_set_load_page_event(menu, cont, skills_selector_page);

	cont = lv_menu_cont_create(main_menu);
	menu_label = lv_label_create(cont);
	lv_label_set_text(menu_label, "Freak Meter");
	lv_menu_set_load_page_event(menu, cont, freak_meter_page);

	lv_menu_set_page(menu, main_menu);


	image = lv_img_create(lv_layer_top());
	lv_obj_add_style(image,&dynamic_battery,0);

	battery_label = lv_label_create(lv_layer_top());
    lv_label_set_text(battery_label, "Battery Percent: NULL%");
	lv_obj_add_style(battery_label,&dynamic_battery,0);
    lv_obj_align_to(battery_label, lv_layer_top(), LV_ALIGN_RIGHT_MID,10,0);
	
}

int get_selected(){
	return selected;
}

void print_coords(Robot& robot){
	dlib::Pose2d pose = robot.odom.get_position();

	lv_label_set_text(coords_x, std::format("X: {:.4f}", pose.x.in(meters)).c_str());
	lv_label_set_text(coords_y, std::format("Y: {:.4f}", pose.y.in(meters)).c_str());
}