Design Requirements
====================
Read the regulations at `American Solar Challenge <https://www.americansolarchallenge.org/regulations/2020-american-solar-challenge-regulations/>`_ before creating a BPS. 

Design requirements vary based on the type of batteries you use. For example, lead-acid packs require less protection than lithium-ion cells. BeVolt’s battery pack uses Samsung INR18650-30Q lithium-ion cells. The BPS is required to monitor the current, voltage, and temperature of the battery pack.

.. 
    Packs vs cells - do lead-acid batteries only come as single units or can you get lead-acid cells? Can a battery pack contain one of either li-ion cells or lead-acid cells? If so, 
    we need to be specific. 
    Link to cell datasheet would be good. 

The minimum coverage the BPS should have is on the module level (groups of cells in parallel). There must be a fuse rated for 200% of the typical current draw of the motors. The fuse is not enough as protection so the BPS must know to fault as well. The BPS should trip whenever the voltage of a module is below or above the min and max limits, respectively. The BPS should trip whenever the temperature exceeds the max limit according to the datasheets. The max temperature limit will vary depending on if the battery is charging or discharging so you must account for both situations. During a fault, the BPS must disconnect the battery from the car.

There must be an error light indicator in the dashboard and a strobe light indicator on the canopy of the cockpit. An emergency switch is required to shut down the car which is located on the canopy as well. The emergency switch should be connected between the power distribution board and BPS. The switch is always closed in the off position and whenever it is pressed, the switch should disconnect the power line between the BPS and power distribution board.