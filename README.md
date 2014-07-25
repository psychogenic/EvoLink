EvoLink
=======

Cross-platform EVO-All data bypass library.

If you want to interact and control real-life cars and trucks, there's no easier way that with the Fortin EVO-All bypass--and the simplest way to use that, is with the EvoLink library.

With EvoLink you can send commands to, and receive event notifications from, the EVO-All module through a 4-wire connection (two for power, and two for RS-232 serial communication).  EvoLink handles timing when issuing instructions and deals with parsing incoming data, which can be handled by custom callback functions for remote starter, brakes, door/trunk/hood open and close, sensors and tachometer events.

Using the library, you're code is legible and doesn't need to worry about timing the byte sends.  For instance, here's how you could arm the system and lock the door:


    EVO.wakeUp()
    EVO.parklightOn()
    EVO.starterKillOn()
    EVO.systemArm()
    EVO.lock()
    delay(750) // wait a bit
    EVO.parklightOff()

EvoLink is released under the GPL (so it's open source and free for hobbiests) and available under a dual-licensing system, should you want to incorporate it into a closed/proprietary product.

