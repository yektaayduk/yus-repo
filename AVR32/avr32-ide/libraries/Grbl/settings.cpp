
#include "Grbl.h"

// Method to store startup lines into EEPROM
void Grbl::Settings::store_startup_line(uint8_t n, char *line)
{

}


// Method to store build info into EEPROM
void Grbl::Settings::store_build_info(char *line)
{

}


// Method to store coord data parameters into EEPROM
void Grbl::Settings::write_coord_data(uint8_t coord_select, float *coord_data)
{

}


// Method to store Grbl global settings struct and version number into EEPROM
void Grbl::Settings::write_global_settings()
{

}

// Method to restore EEPROM-saved Grbl global settings back to defaults.
void Grbl::Settings::restore_global_settings() {
  settings.pulse_microseconds = DEFAULT_STEP_PULSE_MICROSECONDS;
  settings.stepper_idle_lock_time = DEFAULT_STEPPER_IDLE_LOCK_TIME;
  settings.step_invert_mask = DEFAULT_STEPPING_INVERT_MASK;
  settings.dir_invert_mask = DEFAULT_DIRECTION_INVERT_MASK;
  settings.status_report_mask = DEFAULT_STATUS_REPORT_MASK;
  settings.junction_deviation = DEFAULT_JUNCTION_DEVIATION;
  settings.arc_tolerance = DEFAULT_ARC_TOLERANCE;
  settings.homing_dir_mask = DEFAULT_HOMING_DIR_MASK;
  settings.homing_feed_rate = DEFAULT_HOMING_FEED_RATE;
  settings.homing_seek_rate = DEFAULT_HOMING_SEEK_RATE;
  settings.homing_debounce_delay = DEFAULT_HOMING_DEBOUNCE_DELAY;
  settings.homing_pulloff = DEFAULT_HOMING_PULLOFF;

  settings.flags = 0;
  if (DEFAULT_REPORT_INCHES) { settings.flags |= BITFLAG_REPORT_INCHES; }
  if (DEFAULT_AUTO_START) { settings.flags |= BITFLAG_AUTO_START; }
  if (DEFAULT_INVERT_ST_ENABLE) { settings.flags |= BITFLAG_INVERT_ST_ENABLE; }
  if (DEFAULT_INVERT_LIMIT_PINS) { settings.flags |= BITFLAG_INVERT_LIMIT_PINS; }
  if (DEFAULT_SOFT_LIMIT_ENABLE) { settings.flags |= BITFLAG_SOFT_LIMIT_ENABLE; }
  if (DEFAULT_HARD_LIMIT_ENABLE) { settings.flags |= BITFLAG_HARD_LIMIT_ENABLE; }
  if (DEFAULT_HOMING_ENABLE) { settings.flags |= BITFLAG_HOMING_ENABLE; }

  settings.steps_per_mm[X_AXIS] = DEFAULT_X_STEPS_PER_MM;
  settings.steps_per_mm[Y_AXIS] = DEFAULT_Y_STEPS_PER_MM;
  settings.steps_per_mm[Z_AXIS] = DEFAULT_Z_STEPS_PER_MM;
  settings.max_rate[X_AXIS] = DEFAULT_X_MAX_RATE;
  settings.max_rate[Y_AXIS] = DEFAULT_Y_MAX_RATE;
  settings.max_rate[Z_AXIS] = DEFAULT_Z_MAX_RATE;
  settings.acceleration[X_AXIS] = DEFAULT_X_ACCELERATION;
  settings.acceleration[Y_AXIS] = DEFAULT_Y_ACCELERATION;
  settings.acceleration[Z_AXIS] = DEFAULT_Z_ACCELERATION;
  settings.max_travel[X_AXIS] = (-DEFAULT_X_MAX_TRAVEL);
  settings.max_travel[Y_AXIS] = (-DEFAULT_Y_MAX_TRAVEL);
  settings.max_travel[Z_AXIS] = (-DEFAULT_Z_MAX_TRAVEL);

  write_global_settings();
}


// Helper function to clear the EEPROM space containing parameter data.
void Grbl::Settings::clear_parameters() {

}


// Helper function to clear the EEPROM space containing the startup lines.
void Grbl::Settings::clear_startup_lines() {

}

// Helper function to clear the EEPROM space containing the user build info string.
void Grbl::Settings::clear_build_info() {

}

// Reads startup line from EEPROM. Updated pointed line string data.
uint8_t Grbl::Settings::read_startup_line(uint8_t n, char *line)
{
  return(false);
}


// Reads startup line from EEPROM. Updated pointed line string data.
uint8_t Grbl::Settings::read_build_info(char *line)
{
  return(false);
}


// Read selected coordinate data from EEPROM. Updates pointed coord_data value.
uint8_t Grbl::Settings::read_coord_data(uint8_t coord_select, float *coord_data)
{
  return(false);
}


// Reads Grbl global settings struct from EEPROM.
uint8_t Grbl::Settings::read_global_settings() {
  return(false);
}

// Initialize the config subsystem
void Grbl::Settings::init()
{
  if(!read_global_settings()) {
    parent->m_report.status_message(STATUS_SETTING_READ_FAIL);

    restore_global_settings();

    // Force clear startup lines and build info user data. Parameters should be ok.
    // TODO: For next version, remove these clears. Only here because line buffer increased.
    clear_startup_lines();
    clear_build_info();

    parent->m_report.grbl_settings();
  }

  // Check all parameter data into a dummy variable. If error, reset to zero, otherwise do nothing.
  float coord_data[N_AXIS];
  uint8_t i;
  for (i=0; i<=SETTING_INDEX_NCOORD; i++) {
    if (!read_coord_data(i, coord_data)) {
      parent->m_report.status_message(STATUS_SETTING_READ_FAIL);
    }
  }
  // NOTE: Startup lines are checked and executed by protocol_main_loop at the end of initialization.
  // TODO: Build info should be checked here, but will wait until v1.0 to address this. Ok for now.
}

// Returns direction pin mask according to Grbl internal axis indexing.
uint8_t Grbl::Settings::get_direction_pin_mask(uint8_t axis_idx)
{
  return 0;
}
