#include "encoder.h"
#include "tim.h"
#include "cmsis_os.h"

// Encoder state variables
static int32_t ticks = 0;         // Total encoder ticks (raw count)
static int16_t steps = 0;         // Logical steps (processed from ticks)
static int16_t min = 0;           // Minimum step value
static int16_t max = 100;         // Maximum step value
static bool is_cyclic = true;     // If true, steps wrap around min/max
static bool is_reversed = false;  // If true, encoder direction is reversed

static volatile bool button = false; // Button press flag

// Initialize encoder state
void encoder_init(void) {
	ticks = 0;
	steps = 0;
}

// Update encoder state based on hardware counter
void encoder_update(void) {
	static int16_t last = 0; // Last counter value
	int16_t current = __HAL_TIM_GET_COUNTER(&htim4); // Read hardware counter
	int16_t delta = current - last; // Calculate difference since last update
	last = current;

	// Reverse direction if needed
	if (is_reversed)
		delta = -delta;

	ticks += delta; // Update total ticks
	int16_t new_steps = (int16_t)(ticks / 4); // Convert ticks to steps (assuming 4 ticks per step)

	taskENTER_CRITICAL();
	if (is_cyclic) {
		// Wrap steps around min/max if cyclic
		int32_t range = max - min + 1;
		steps = ((new_steps - min) % range + range) % range + min;
	} else {
		// Clamp steps to min/max if not cyclic
		if (new_steps < min) new_steps = min;
		if (new_steps > max) new_steps = max;
		steps = new_steps;
	}
	taskEXIT_CRITICAL();
}

// Get current tick count
int32_t encoder_get_ticks(void) {
	return ticks;
}

// Get current step value
int16_t encoder_get_steps(void) {
	return steps;
}

// Set minimum step value
void encoder_set_min(int16_t min_val) {
	taskENTER_CRITICAL();
	min = min_val;
	taskEXIT_CRITICAL();
}

// Set maximum step value
void encoder_set_max(int16_t max_val) {
	taskENTER_CRITICAL();
	max = max_val;
	taskEXIT_CRITICAL();
}

// Reset encoder position (ticks and steps)
void encoder_reset_position(void) {
	taskENTER_CRITICAL();
	ticks = 0;
	steps = 0;
	taskEXIT_CRITICAL();
}

// Enable or disable cyclic mode
void encoder_set_cyclic(bool enable) {
	is_cyclic = enable;
}

// Set encoder direction (normal or reversed)
void encoder_set_direction(bool reverse) {
	is_reversed = reverse;
}

// Update button state (debounced)
void encoder_button_update(void) {
	static bool last_state = true; // Last button state
	bool current = HAL_GPIO_ReadPin(ENC_BUTT_GPIO_Port, ENC_BUTT_Pin) == GPIO_PIN_RESET; // Read button (active low)
	if (current && !last_state) {
		// Button pressed (rising edge)
		last_state = true;
		taskENTER_CRITICAL();
		button = true;
		taskEXIT_CRITICAL();
	} else if (!current) {
		// Button released
		last_state = false;
	}
}

// Get and clear button press flag
bool encoder_get_button(void) {
	if (button) {
		taskENTER_CRITICAL();
		button = false;
		taskEXIT_CRITICAL();

		return true;
	}

	return button;
}

// Set step value and update ticks accordingly
void encoder_set_step(int16_t step) {
	taskENTER_CRITICAL();
	steps = step;
	ticks = step * 4; // Convert steps to ticks
	taskEXIT_CRITICAL();
}
