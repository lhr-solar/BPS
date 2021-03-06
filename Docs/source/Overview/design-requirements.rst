Design Requirements
====================
Read the regulations at `American Solar Challenge <https://www.americansolarchallenge.org/regulations/2020-american-solar-challenge-regulations/>`_ before creating a BPS. 

Design requirements vary based on the type of batteries you use. For example, lead-acid cells require less protection than lithium-ion cells. 
BeVolt’s battery pack uses `Samsung INR18650-30Q lithium-ion cells <https://www.18650batterystore.com/v/files/samsung_30q_data_sheet.pdf>`_. 
The BPS is required to monitor the current, voltage, and temperature of the battery pack.

The BPS should have the following components and features:

    * Minimum coverage on the module level (groups of cells in parallel)
    * Fuse rated for 200% of the typical current draw of the motors
    * Trip system that disconnects the battery from the car when:
        
        * the voltage of a module exceeds the safe range
        * the current through the cells exceeds the safe range
        * the temperature exceeds the maximum specified in the datasheet
        * an open wire is detected
        * the BPS RTOS stops functioning properly
        * a hard fault occurs
        * a lot of data is dropped from LTC boards at once

        .. note::
            The temperature and current limits vary depending on whether the battery is charging or discharging. 
            Account for both situations in the design.
        
    * Error light indicator on the dashboard
    * Strobe light indicator on the cockpit canopy
    * Emergency shutoff switch on the cockpit canopy
    
    .. note::
        The shutoff switch connects the BPS to the power distribution board, and is closed by default. 
        When pressed, the switch disconnects the power line between the BPS and the board, shutting down the car.
