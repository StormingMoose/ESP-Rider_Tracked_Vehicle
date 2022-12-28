# ESP-Rider_Tracked_Vehicle
L9110S moto driver software for a tracked, crawler type vehicle.

Uses an ESP32 Dev kit module to control a small tracked vehicles direction and speed with a L9110S H-bridge.

The code uses PWM and channels.

This will set up a local wifi connection to the car which will allow a user to access the car's control html page.

Esp32 must be set up in preferences in Arduino IDE. https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/


Parts required:
ESP32 devkit or similar.
L9110S H-bridge.
Power supply.
Electric motors of your choice.

Two motors on crawler tracks.  Go forward, back, spin left and spin right.

January 2022 added a lock to the screen to stop pinch to zoom when using two buttons at once. Works for Android phones. November 2022 edited to work with current Arduino IDE. Moved the pinmode statements above the LedcAttachPin statements.
