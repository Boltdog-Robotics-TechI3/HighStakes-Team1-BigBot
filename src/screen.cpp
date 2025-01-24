#include "main.h"

lv_obj_t* textBox1; // Text box for Button 1
lv_obj_t* textBox2; // Text box for Button 2
lv_obj_t* textBox3; // Text box for Button 3

void initializeScreen() {
    // Get the screen's width and height
    lv_coord_t screenWidth = lv_obj_get_width(lv_scr_act());
    lv_coord_t screenHeight = lv_obj_get_height(lv_scr_act());

    // Divide top half for 3 text boxes
    lv_coord_t textBoxHeight = screenHeight / 6; 

    // Text Box 1
    textBox1 = lv_label_create(lv_scr_act());
    lv_obj_set_size(textBox1, screenWidth, textBoxHeight);
    lv_obj_align(textBox1, LV_ALIGN_TOP_MID, 0, 0); // Align to the top
    lv_label_set_text(textBox1, "Blue side is selected");
    lv_label_set_long_mode(textBox1, LV_LABEL_LONG_WRAP);
    lv_obj_set_style_text_align(textBox1, LV_TEXT_ALIGN_CENTER, 0);
    styleTextBox(textBox1, lv_palette_main(LV_PALETTE_BLUE)); 

    // Text Box 2
    textBox2 = lv_label_create(lv_scr_act());
    lv_obj_set_size(textBox2, screenWidth, textBoxHeight);
    lv_obj_align(textBox2, LV_ALIGN_TOP_MID, 0, textBoxHeight); // Align below Text Box 1
    lv_label_set_text(textBox2, "Match Selected");
    lv_label_set_long_mode(textBox2, LV_LABEL_LONG_WRAP);
    lv_obj_set_style_text_align(textBox2, LV_TEXT_ALIGN_CENTER, 0);
    styleTextBox(textBox2, lv_palette_main(LV_PALETTE_PURPLE)); // Green fill color

    // Text Box 3
    textBox3 = lv_label_create(lv_scr_act());
    lv_obj_set_size(textBox3, screenWidth, textBoxHeight);
    lv_obj_align(textBox3, LV_ALIGN_TOP_MID, 0, textBoxHeight * 2); // Align below Text Box 2
    lv_label_set_text(textBox3, "0 - Match Plus Side Keep Goal Auto (NOT FINISHED)");
    lv_label_set_long_mode(textBox3, LV_LABEL_LONG_WRAP);
    lv_obj_set_style_text_align(textBox3, LV_TEXT_ALIGN_CENTER, 0);
    styleTextBox(textBox3, lv_palette_main(LV_PALETTE_BLUE_GREY)); // Green fill color

    // Create Button Container
    lv_obj_t* buttonContainer = lv_obj_create(lv_scr_act());
    lv_obj_set_size(buttonContainer, screenWidth, screenHeight / 2); // Bottom half
    lv_obj_align(buttonContainer, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_pad_all(buttonContainer, 5, 0);
    lv_obj_set_layout(buttonContainer, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(buttonContainer, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(buttonContainer, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    // **Step 3: Add Buttons**
    // Button 1
    lv_obj_t* btn1 = lv_btn_create(buttonContainer);
    lv_obj_set_size(btn1, screenWidth / 3, screenHeight / 6);
    lv_obj_t* btn1Label = lv_label_create(btn1);
    lv_label_set_text(btn1Label, "Alliance Selector");
    lv_obj_add_event_cb(btn1, [](lv_event_t* e) {
        onLeftButton();
    }, LV_EVENT_CLICKED, NULL);

    // Button 2
    lv_obj_t* btn2 = lv_btn_create(buttonContainer);
    lv_obj_set_size(btn2, screenWidth / 3, screenHeight / 6);
    lv_obj_t* btn2Label = lv_label_create(btn2);
    lv_label_set_text(btn2Label, "Game Type");
    lv_obj_add_event_cb(btn2, [](lv_event_t* e) {
        onCenterButton();
    }, LV_EVENT_CLICKED, NULL);

    // Button 3
    lv_obj_t* btn3 = lv_btn_create(buttonContainer);
    lv_obj_set_size(btn3, screenWidth / 3, screenHeight / 6);
    lv_obj_t* btn3Label = lv_label_create(btn3);
    lv_label_set_text(btn3Label, "Auto Selector");
    lv_obj_add_event_cb(btn3, [](lv_event_t* e) {
        onRightButton();
    }, LV_EVENT_CLICKED, NULL);
}

/** 
 * Updates the given textbox on the screen
 */
void updateTextBox(lv_obj_t* textBox, const char* newText) {
    if (textBox) {
        lv_label_set_text(textBox, newText);
    }
}

/**
 *  Styles the textboxes on the screen. It sets their border to purple, and the 
 *  user can input a color for the background.
 * 
 *  @param textBox textbox that will be styled
 *  @param bgColor color to set the background to.
 */
void styleTextBox(lv_obj_t* textBox, lv_color_t bgColor) {
    lv_obj_set_style_bg_color(textBox, bgColor, 0); // Set background color
    lv_obj_set_style_bg_opa(textBox, LV_OPA_COVER, 0); // Fully opaque background
    lv_obj_set_style_border_width(textBox, 2, 0); // Border width
    lv_obj_set_style_border_color(textBox, lv_palette_main(LV_PALETTE_DEEP_PURPLE), 0); // Border color
    lv_obj_set_style_border_opa(textBox, LV_OPA_COVER, 0); // Border opacity
    lv_obj_set_style_pad_all(textBox, 5, 0); // Padding inside the border
}

/**
 *	Selects whether we are in skills or a match, toggled
 *	Default is Match
 */
void onCenterButton() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		updateTextBox(textBox2, "Skills Run Selected");
		skills = true;
		match = false;
	} else {
		updateTextBox(textBox2, "Match Selected");
		skills = false;
		match = true;
	}
}

/**
*	Selects the auto from a list of autos (Default is Match Plus Side Keep Auto)
*	0 - Match Plus Side Keep Goal Auto
*   1 - Match Plus Side Drop Goal Auto 
*	2 - Match Climb Goal Keep Goal Auto
*   3 - Match Climb Goal Drop Goal Auto 
*	4 - Skills Auto
*   5 - Do Nothing
 */
void onRightButton() {
	
	autoSelection++;

	if (autoSelection >= 6) {
		autoSelection = 0;
	}
	
	switch (autoSelection) {
		case 0: 
			// Auto where we rush for the plus side goal, grab it, 
			// then score rings on it and drop it off in the corner.
			updateTextBox(textBox3, "0 - Match Plus Side Keep Goal Auto (NOT FINISHED)");
			break;
		case 1: 
			// Auto where we rush for the plus side goal, grab it, 
			// then drop it off and go for the alliance side goal instead.
			updateTextBox(textBox3, "1 - Match Plus Side Drop Goal Auto");
			break;
		case 2: 
			updateTextBox(textBox3, "2 - Match Climb Goal Keep Goal Auto (NOT FINISHED)");
			break;
		case 3: 
			updateTextBox(textBox3, "3 - Match Climb Goal Drop Goal Selected (NOT FINISHED)");
			break;
		case 4: 
			updateTextBox(textBox3, "4 - Skills Auto Selected (NOT FINISHED)");
			break;
        case 5:
 			updateTextBox(textBox3, "5 - Do Nothing");
			break;           
	}
}


/**
 * Selects which alliance we are on. Defaults to blue
 */
void onLeftButton() {
	static bool pressed = false;
	pressed = !pressed;

	if (pressed) {
		updateTextBox(textBox1, "Red side is selected");
        styleTextBox(textBox1, lv_palette_main(LV_PALETTE_RED)); 

		blueAlliance = false;
		redAlliance = true;

	} else {
		updateTextBox(textBox1, "Blue side is selected");
        styleTextBox(textBox1, lv_palette_main(LV_PALETTE_BLUE)); 
		redAlliance = false;
		blueAlliance = true;

	}
}