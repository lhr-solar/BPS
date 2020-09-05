Design Requirements
====================
Read the regulations at `American Solar Challenge <https://www.americansolarchallenge.org/regulations/2020-american-solar-challenge-regulations/>`_ before creating a BPS. 

Design requirements vary based on the type of batteries you use. For example, lead-acid packs require less protection than lithium-ion cells. BeVolt’s battery pack uses Samsung INR18650-30Q lithium-ion cells. The BPS is required to monitor the current, voltage, and temperature of the battery pack.

.. 
    Packs vs cells - do lead-acid batteries only come as single units or can you get lead-acid cells? Can a battery pack contain one of either li-ion cells or lead-acid cells? If so, 
    we need to be specific. 
    Link to cell datasheet would be good. 

The BPS should have the following components and features:

    * Minimum coverage on the module level (groups of cells in parallel)
    * Fuse rated for 200% of the typical current draw of the motors
    * Trip system that disconnects the battery from the car when:
        
        * the voltage of a module exceeds the safe range
        * the temperature exceeds the maximum specified in the datasheets
        .. note::
            The temperature limit varies depending on whether the battery is charging or discharging. 
            Account for both situations in the design.
    * Error light indicator in the dashboard
    * Strobe light indicator on the cockpit canopy
    * Emergency shutoff switch on the cockpit canopy
    .. note::
        The shutoff switch connects the BPS to the power distribution board, and is closed by default. 
        When pressed, the switch disconnects the power line between the BPS and the board, shutting down the car.