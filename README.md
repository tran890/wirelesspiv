Wireless PIV Monitoring System

Overview:

The Wireless PIV Monitoring System is designed to monitor and manage PIV (Post Indicator Valve) devices wirelessly. It comprises two main components:

PIV Code: This code is used on a microcontroller that interfaces with a slide switch acting as the PIV switch. It sends status signals and a periodic "heartbeat" to the Fire Alarm Panel (FAP). Multiple devices will run this code and report to the FAP.

Fire Alarm Panel (FAP) Code: This code runs on another microcontroller and is responsible for receiving and processing signals from multiple PIV devices. It maintains a database of connected PIVs, monitors their status, and handles communication to detect any disconnection or loss of signal.

The system ensures reliable monitoring of PIV devices by continuously sending status updates and checking connectivity.

Features:
Heartbeat Signal: The PIV device sends a heartbeat signal every 10 seconds to confirm its operation and connectivity.

PIV Status Reporting: Provides real-time status of the PIV device, including its activation state.

Database Management: The FAP code manages a database of connected PIVs, including their MAC addresses and status.

Connectivity Monitoring: If a PIV device becomes unresponsive for over 2 minutes, the system flags it as not responding and displays an alert.

Serial Terminal Output: Displays detailed information about each connected PIV device, including Board ID, bytes sent, x and y values, heartbeat status, PIV status, and MAC address.


Components:
PIV Code:
Function: Sends PIV switch status and heartbeat signals.
Hardware Interface: Uses a slide switch to simulate the PIV switch.
Communication: Periodically sends data to the Fire Alarm Panel (FAP) via wireless communication.

FAP Code:
Function: Receives data from PIV devices, updates the database, and monitors device status.
Output: Displays data on a serial terminal, including Board ID, bytes sent, x and y values, heartbeat status, PIV status, and MAC address.

Documentation:
Detailed documentation and schematics can be found in the OneNote section under the ESP-NOW category. Please ensure you have Intel Corporation Access to view the documentation: 

https://intel-my.sharepoint.com/:o:/p/henry_tran/EqcHIvqrZ9ZPtJBnjS63Q2IBnbQwo7-a70yvDKLEi-3_HA?e=aJ1AkE

Setup and Installation:
Microcontrollers compatible with the PIV and FAP codes, ideally an ESP32.
Access to the ESP-NOW communication protocol documentation.
Serial terminal software for monitoring.

PIV Code Setup:
Hardware: Use setup according to the schematic in the documentation.
Software: Upload the PIV code to the microcontroller using your preferred development environment, ideally Arduino IDE.
Configuration: Ensure the microcontroller is configured to communicate with the FAP.

FAP Code Setup:
Hardware: Connect the microcontroller to your serial terminal interface with the scheamtic in the documentation. 
Software: Upload the FAP code to the microcontroller.
Configuration: Configure the FAP to connect to the PIV devices and monitor their signals.

Usage:
Power On: Ensure both PIV and FAP devices are powered on and within communication range.
Monitor: Use the serial terminal to view real-time data from the FAP.
Check Status: The FAP will display connected PIV devices, their statuses, and any connectivity issues.

Troubleshooting:
No Data Received: Verify that the PIV device is powered on and within range. Check connections and ensure the code is properly uploaded.
Disconnected Device Alert: Ensure that the PIV device has not been turned off or moved out of range. Check the heartbeat signal.

Contact
For any inquiries or further information, please contact Henry Tran at henry.tran@intel.com or tran.890@osu.edu
