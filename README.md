# Sleep_Mode_DAQ_Task3

The third task - using task 2 as the base, set an internal threshold on a sensor and generate an EXTI on the appropriate line. The EXTI will raise a flag, which will be parsed upon interrupt exit (in the main routine). I've decided to generate an interrupt upon the accelerometer being upside down.

TODO: 
* [x] Research and setup the EXTI line and software
* [ ] Research and setup internal threshold
* [x] Implement the (very simple) state architecture
