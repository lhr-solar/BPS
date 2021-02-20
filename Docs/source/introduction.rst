Overview 
=========

A Battery Protection System (BPS) is a system that makes sure that a battery stays within safe operating conditions.

Another common term for similar systems is battery management system (BMS). 
The difference between a BPS and a BMS is that a BMS manages the battery to optimize its charging, power output, and other factors.

This project uses a BPS in a solar-powered race car to compete against other universities in the American Solar Challenge.

Why use a BPS?
==============

The car has a large lithium-ion (li-ion) battery. If li-ion batteries don't have proper protection, they can catch fire and explode, like the `Samsung Galaxy Note 7 <https://www.bbc.com/news/business-38714461>`_.
For obvious reasons, this should not happen with our car. 

How does it work?
=================

1. The BPS periodically checks the voltage, current and temperature of each battery module
2. If any of the modules are unsafe, the BPS:
 
  a. shuts down the car by opening the contactor and stopping current flow through the battery
  b. turns on a strobe light to alert other cars that the BPS has faulted

Additionally, this revision of the BPS (2020 to 2022) aims to add :abbr:`BMS (Battery Management System)` functionality to the BPS to increase efficiency.