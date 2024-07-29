# wirelesspiv
Summer 2024 LSS Intern Project

This is a wireless PIV monitoring system. There are 2 codes, the fire alarm panel (FAP) and the piv code. The PIV code uses a slide switch that acts like the switch for the PIV and will send a signal to the FAP upon activation. The PIV code also sends a "heartbeat" every 10 seconds to say that the board is still alive. The FAP code constantly looks for PIVs to add to the database and monitors it. It receives 3 fields, a heartbeat signal, piv status, and a MAC address. It displays it in the serial terminal with the Board ID, number of bytes sent, the x value, the y value, heartbeat status, piv status, and mac address. If a board is disconnected or if the signal is lost, after 2 minutes, it will display that the board is not responding. The documentation for this and the schematics can be found in the OneNote: 

https://intel-my.sharepoint.com/:o:/p/henry_tran/EqcHIvqrZ9ZPtJBnjS63Q2IBnbQwo7-a70yvDKLEi-3_HA?e=GrvrZf 

Note: You must have Intel Corporation Access to access this.
