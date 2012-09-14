#ifndef ICUBEXCODES_H
#define ICUBEXCODES_H
//This file contains the definitions of the commnads that the microdig device can
//understand, and the messages it may send as response
// general commands
#define RESET      0x22
#define SYS_RESET  0xFF
#define MUTE       0x20
#define SET_MUTE   0x32
      //   0xxxxxxx xxxxxxx = 0 all sensor inputs unmuted
      //   0xxxxxxx xxxxxxx = [1..127] all sensor inputs muted
#define DUMP_VERSION 0x47
#define DUMP_MODE  0x5B
#define SET_MODE   0x5A
#define SET_ID     0x5C
      //   0xxxxxxx xxxxxxx = [0..127] new system exclusive dev ID
#define MIDI_THRU  0x5D
      //   0xxxxxxx xxxxxxx = 0 MIDI Thru off
      //   0xxxxxxx xxxxxxx = 1 MIDI Thru on
#define MIDI_RUNNING_STATUS 0x5E // Ox00 off 0x01 on

// mDig General Messages 
#define RESET_ACK 0x23
#define STATUS    0x25 // this message denotes a communication error
      //   0x5A configuration setting error
      //   0x5C MIDI protocol error (a data byte is > 127)
      //   0x5E MIDI byte has been scrambled 
      //   0x5F receive buffer is full, data may be lost
              //too much data is being sent too fast to the mDig
#define VERSION   0x47
      //   0xxxxxxx xxxxxxx = [0..127]; firmware version nr. * 10
      //   0aaaaaaa aaaaaaa = [0..99] ; hardware board version nr * 10
      //   0bbbbbbb bbbbbbb = [0..99] ; hardware board version decimals * 1000
      //   0ccccccc ccccccc = [0..99] ; 1st serial nr digits
      //   0ddddddd ddddddd = [0..99] ; 2nd serial nr digits
#define MODE      0x5B     // 0x00 hostmode  0x01 stand-alone mode
#define MDIG_ID   0x5C
      //   0xxxxxxx xxxxxxx = [0..127] new system exclusive dev ID

// Stand-Alone mode commands
#define STREAM    0x01
      //   0x000yyy x = 1 on
      //            x = 0 off
      //            yyy = [0...7] sensor input number
#define INTERVAL  0x03 //2 bytes [BODY]
      //   0xxxxxxx xxxxxxx = [0..127]; sample interval MSB
      //   0aaaaaaa aaaaaaa = [0..99] ; sample interval LSB
      //   where sampling interval = xxxxxxx * 128 + yyyyyyy
#define EDIT_NAME 0x64
      //   00000001 1 = configuration number
      //   0aaaaaaa aaaaaaa = [0..127] ; 1st character, default = "m" (0x6D)
      //   0bbbbbbb bbbbbbb = [0..127] ; 2nd character, default = "i" (0x69)
      //   0ccccccc ccccccc = [0..127] ; 3rd character, default = "n" (0x6E)
      //   0ddddddd ddddddd = [0..127] ; 4th character, default = "i" (0x69)
      //   0eeeeeee eeeeeee = [0..127] ; 5th character, default = "D" (0x44)
      //   0fffffff fffffff = [0..127] ; 6th character, default = "i" (0x69)
      //   0ggggggg ggggggg = [0..127] ; 7th character, default = "g" (0x67)
      //   0hhhhhhh hhhhhhh = [0..127] ; 8th character, default = "" (0x20)
#define DUMP_NAME 0x65
      //   00000001 1 = configuration number
#define CLEAR_CONFIG 0x69
      //   00000001 1 = configuration number
#define EDIT_CONFIG 0x6A
      //   00000001 1 = configuration number
      //   00000aaa aaa = [0..7] ; sensor input number
      //   0bbbcccc bbb = [0..6] ; MIDI mapping type
      //            0 = note-off, 1= note-on, 2=key-pressure, 3=control-change,
      //            4 = program-change, 5=after-touch, 6=pitch-bend
      //            cccc = [0..15] = MIDI channel
      //   0ddddddd ddddddd = [0..127] ; note number (mapping type 0..2)
      //            controller number (mapping type 3), irrelevant (mapping type 4..6)
      //   00efghij efghij = [0..127] ; determine signal processing 
      //                                (see manual for combining)
      //            e = [0,1] impulse end notification (0 = off, 1 = on)
      //            f = [0,1] impulse maximum/minimum constant (0=off, 1=on)
      //            g = [0,1] continuos and/or impulse signal differentiation(0=off, 1=on)
      //            h = [0,1] continuous signal averaging (0=off, 1=on)
      //            i = [0,1] impulse signal analysis (0=off, 1=on)
      //            j = [0,1] continuous signal analysis (0=off, 1=on)
      //   0kkkkkkk kkkkkkk = [0..127] ; sensor input threshold
      //   0mmmmmmm mmmmmmm = [0..127] ; sensor input ceiling
      //   0nnnnnnn nnnnnnn = [0..127] ; noise gate
      //   0pppqqqq ppp = [0..7] ; represents constant value activated with {f}
      //            qqqq = [0..15] ; time window 
#define DUMP_CONFIG 0x6B
      //   00000001 1 = configuration number
      //   00000aaa aaa = [0..7] ; sensor input number

// Stand-Alone Transmitted Messages
#define NAME        0x65
      //   00000001 1 = configuration number
      //   0aaaaaaa aaaaaaa = [0..127] ; 1st character, default = "m" (0x6D)
      //   0bbbbbbb bbbbbbb = [0..127] ; 2nd character, default = "i" (0x69)
      //   0ccccccc ccccccc = [0..127] ; 3rd character, default = "n" (0x6E)
      //   0ddddddd ddddddd = [0..127] ; 4th character, default = "i" (0x69)
      //   0eeeeeee eeeeeee = [0..127] ; 5th character, default = "D" (0x44)
      //   0fffffff fffffff = [0..127] ; 6th character, default = "i" (0x69)
      //   0ggggggg ggggggg = [0..127] ; 7th character, default = "g" (0x67)
      //   0hhhhhhh hhhhhhh = [0..127] ; 8th character, default = "" (0x20)
#define CONFIG      0x6A
      //   00000001 1 = configuration number
      //   00000aaa aaa = [0..7] ; sensor input number
      //   0bbbcccc bbb = [0..6] ; MIDI mapping type
      //            0 = note-off, 1= note-on, 2=key-pressure, 3=control-change,
      //            4 = program-change, 5=after-touch, 6=pitch-bend
      //            cccc = [0..15] = MIDI channel
      //   0ddddddd ddddddd = [0..127] ; note number (mapping type 0..2)
      //            controller number (mapping type 3), irrelevant (mapping type 4..6)
      //   00efghij efghij = [0..127] ; determine signal processing 
      //                                (see manual for combining)
      //            e = [0,1] impulse end notification (0 = off, 1 = on)
      //            f = [0,1] impulse maximum/minimum constant (0=off, 1=on)
      //            g = [0,1] continuos and/or impulse signal differentiation(0=off, 1=on)
      //            h = [0,1] continuous signal averaging (0=off, 1=on)
      //            i = [0,1] impulse signal analysis (0=off, 1=on)
      //            j = [0,1] continuous signal analysis (0=off, 1=on)
      //   0kkkkkkk kkkkkkk = [0..127] ; sensor input threshold
      //   0mmmmmmm mmmmmmm = [0..127] ; sensor input ceiling
      //   0nnnnnnn nnnnnnn = [0..127] ; noise gate
      //   0pppqqqq ppp = [0..7] ; represents constant value activated with {f}
      //            qqqq = [0..15] ; time window 

// Host mode commands
// STREAM is already defined
      //   0x000yyy x = 1 on
      //            x = 0 off
      //            yyy = [0...7] sensor input number

#define RES         0x02
      //   0x000yyy x = 1 ; hi-res
      //            x = 0 ; lo-res
      //            yyy = [0...7] sensor input number
// INTERVAL is already defined
      //   0xxxxxxx xxxxxxx = [0..127]; sample interval MSB
      //   0aaaaaaa aaaaaaa = [0..99] ; sample interval LSB
      //   where sampling interval = xxxxxxx * 128 + yyyyyyy
#define SAMPLE      0x04
      //   00000yyy yyy = [0...7] sensor input number
#define POWER_LEVEL 0x6F
#define I2C_PORT_SET 0x7D 
#define I2C_WRITE   0x7E      
      //   0aaaaaaa aaaaaaa = [0..127] ; I2C address of device
      //   0000bbbb 
      //   0000cccc bbbbcccc = [0..255] ; register number of device
      //   0000dddd 
      //   0000eeee ddddeeee = [0..255] ; data to write
#define I2C_READ    0x7F
      //   0aaaaaaa aaaaaaa = [0..127] ; I2C address of device
      //   0000bbbb 
      //   0000cccc bbbbcccc = [0..255] ; register number of device
      //   0ddddddd ddddddd = [0..127] ; number of bytes to read
// Host mode transmitted messages
#define STREAM_DATA 0x00
      //   0yyyyyyy: 7 bit lo-res mode
      //   or
      //   0yyyyyyy, 00000zzz 10-bit hi-res mode
#define SAMPLE_DATA 0x04
      //   00000xxx, 0yyyyyyy: 7 bit lo-res mode
      //   or
      //   00000xxx, 0yyyyyyy, 00000zzz 10-bit hi-res mode
      //             xxx = [0..7] sensor input number
      //             yyyyyyy= [0..127] the most significant bits
      //             zzz = least significant bits if in hi-res
#define I2C_PORT    0x7D
      //   0xxxxxxx  xxxxxxx = 0 (0x00) to 6 (0x06) defines an opened port
      //             xxxxxxx = 127 (0xFF) defines no opened port
// I2C_WRITE is already defined
// I2C_READ  is already defined
#endif // ICUBEXCODES_H
