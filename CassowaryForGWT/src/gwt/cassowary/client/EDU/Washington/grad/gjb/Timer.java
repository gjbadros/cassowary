// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This Java Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, 1999 Greg J. Badros and Alan Borning
// See ../LICENSE for legal details regarding this software
//
// Timer, adapted from John P. Russo's C++ Timer class

// LM--Reworked for Java 5+ and modified for millisecond time in GWT

package gwt.cassowary.client.EDU.Washington.grad.gjb;

public class Timer {
   public Timer() {
      timerIsRunning = false; // Start not yet called.
      elapsedTime = 0; // No time on timer object yet.
   }

   public void Start() {
      // Stopwatch is now running
      timerIsRunning = true;
      // Look at internal clock and remember reading
      startReading = getTime();
   }

   public void Stop() {
      timerIsRunning = false; // Stop timer object.
      elapsedTime += getTime() - startReading;
   }

   // Clears a Timer of previous elapsed times, so that a new event
   // can be timed.
   public void Reset() {
      timerIsRunning = false; // Start not yet called.
      elapsedTime = 0; // No time on timer object yet.
   }

   // The data member, "TimerIsRunning" is used to keep track of
   // whether a timer is active, i.e. whether an event is being
   // timed. While we want those using the timer class to know when a
   // timer is active, we do NOT want them to directly access the
   // TimerIsRunning variable. We solve this problem, by making
   // TimerIsRunning private and providing the public "access function"
   // below.

   public boolean IsRunning() {
      return timerIsRunning;
   }

   // This function allows a client to determine the amount of time that has
   // elapsed on a timer object. Note that there are two possibilities:

   // 1) A timer object has been started and stopped. We can detect this
   // case, because the variable "TimerIsRunning" is false.

   // 2) A timer object is "running", i.e. is still in the process of timing
   // an event. It is not expected that this case will occur as frequently
   // as case 1).

   // In either case, this function converts ticks to seconds. Note that
   // since the function TicksPerSecond() returns a value of type double,
   // an implicit type conversion takes place before doing the division
   // required in either case.

   public double ElapsedTime() {
      if (!timerIsRunning) // Normal case
         return elapsedTime / 1e3;
      else
         return elapsedTime + (getTime() - startReading) / 1e3;
   }

   private long getTime() {
      return (new java.util.Date()).getTime();
   }

   private boolean timerIsRunning;
   private long    elapsedTime;
   private long    startReading;
}
