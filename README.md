c-bbb-pwm
======================

[![Build Status](https://travis-ci.org/Coderlane/c-bbb-pwm.svg)](https://travis-ci.org/Coderlane/c-bbb-pwm)

A C library for managing pwm output from a beagle bone black.

## Installation

c-bbb-pwm relies on udev for finding devices, you'll need to install it to be able to build c-bbb-pwm. On Ubuntu the package you want is libudev-dev.

      git clone https://github.com/Coderlane/c-bbb-pwm.git
      cd c-bbb-pwm
      mkdir build && cd build
      ccmake ..
      make
      sudo make install

## Usage

Unfortunatly v0.1.0 isn't super usable. You'll want to include both bbb_pwm.h and bbb_pwm_internal.h. You'll then need to find the pwms you want to interface with on your own. Here is an example of how that'll look in your code.

      #include <bbb_pwm/bbb_pwm_internal.h>
      #include <bbb_pwm/bbb_pwm.h>
      
      struct bbb_capemgr_t* cm;
      struct bbb_pwm_t* bp;
      
      // This isn't strictly necessary.
      // Especially if you enable it yourself.
      cm = bbb_capemgr_new();
      bbb_capemgr_enable(cm, "am33xx_pwm");
      
      // Make a pwm, we need the path to it. That is your job.
      bp = bbb_pwm_new("test_pwm", "PATH TO THIS PWM's ROOT");
      // Claim it so we can open some files.
      bbb_pwm_claim(bp);
      // Lets try setting the duty_percent to FULL SPEED.
      bbb_pwm_set_duty_percent(bp, 100.0f);
      
      // free up your memory
      bbb_pwm_delete(&bp);
      bbb_capemgr_delete(&cm);
 
 In the future, I hope to make this all automatic! All you'd have to do is use the bbb_pwm_controller_t. Look for that in future releases.
 
## Documentation
 
 Build yer own! I use doxygen, just open the Doxyfile with doxywizard or simmilar and output whatever type of files you want. In the future I'll upload these online, but not yet.
 
## Known Issues
 
 * No automatic detection
 * Requires root :/
 * Probably not thread safe
 * Others. It's v0.1.0, I'm working on it!
