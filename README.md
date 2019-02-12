# Cooperative-scheduler
Based on " Patterns for Time-triggered Embedded Systems - Michael J. Pont 2001 ", as a reference, I developed this generic cooperative scheduler
it can be integrated with any timer peripheral whether a generic or Systick one.
The scheduler was integrated and tested with ATMega32 (TIMER0, Normal mode) and ARM CORTEX-M4 TM4C123gh6pm -Tiva C Launchpad (SysTick).
The scheduler time unit is " tick " which can be adjusted using the General-Purpose Timer (Gpt), but it's preferable to operate on
one-mSec tick interval.
