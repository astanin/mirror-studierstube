#ifndef OT_TOOL_MIDICODES
#define OT_TOOL_MIDICODES

#define STATUS_MASK 
#define CHANNEL_MASK           15

#define ACTIVE_SENSING         0xFE
#define AFTER_TOUCH            0xD0
#define CONTINUE               0xFB
#define CONTROL_CHANGE         0xB0
#define SYSTEM_EXCLUSIVE       0xF0
#define END_OF_SYSTEM_EX       0xF7
#define MIDI_TIME_CODE         0xF1
#define NOTE_OFF               0x80
#define NOTE_ON                0x90
#define PITCH_BEND             0xE0
#define KEY_PRESSURE           0xA0
#define PROGRAM_CHANGE         0xC0
#define SONG_POSITION_POINTER  0xF2
#define SONG_SELECT            0xF3
#define START                  0xFA
#define STOP                   0xFC
#define SYSTEM_RESET           0xFF
#define TIMING_CLOCK           0xF8
#define TUNE_REQUEST           0xF6

#endif //OT_TOOL_MIDICODES
