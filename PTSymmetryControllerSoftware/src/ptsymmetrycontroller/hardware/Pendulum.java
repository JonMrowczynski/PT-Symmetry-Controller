package ptsymmetrycontroller.hardware;

import java.util.concurrent.Callable;

import ptsymmetrycontroller.PTSymmetryController;
import ptsymmetrycontroller.hardware.Solenoid.PulsingType;
import ptsymmetrycontroller.utils.StopWatch;

/**
 * A {@code Pendulum} class allows for the acquisition of its asymmetrical periods.
 * <p>
 * A {@code Pendulum} uses a {@code Thread} in order for it's asymmetrical partial periods to 
 * be calculated by being able to independently and more reliably monitor its associated photogate 
 * and record timing information that is relayed back from the Lab Quest 2 with minimal interference 
 * from other occurrences in the program. 
 * <p>
 * Once the {@code Thread} has recorded enough values (currently this is set to 21) the {@code Thread} calls
 * {@code calculatePartialPeriods()} and stores the shorter period in {@code shorterPartialPeriod} and the
 * longer period in the {@code longerPartialPeriod} respectively.
 * 
 * @author Jon Mrowczynski
 */

public final class Pendulum {
	
	/**
	 * The number of {@code Pendulum}s that have been instantiated.
	 */
	
	private static int numberOfPendulums = 0;
	
	/**
	 * The specific number of this {@code Pendulum}.
	 */
		
	private final int pendulumNumber;
	
	/**
	 * A helper {@code Thread} that allows for the asymmetrical partial periods of this
	 * {@code Pendulum} to be acquired.
	 */
	
	private final PartialPeriodsDeterminerThread periodDeterminerThread;
	
	/**
	 * The shorter partial period of this {@code Pendulum}.
	 */
	
	private volatile long shorterPartialPeriod;
	
	/**
	 * The longer partial period of this {@code Pendulum}.
	 */
	
	private volatile long longerPartialPeriod;
	
	/**
	 * The {@code Solenoid} that corresponds to this {@code Pendulum} that will pulse 
	 * the magnet that is attached to it.
	 */
	
	private final Solenoid solenoid;
	
	/**
	 * Constructs a {@code Pendulum} that the user needs to manually start swinging.
	 * 
	 * @param controller that is interested in this {@code Pendulum}.
	 * @param photogate that is associated with this {@code Pendulum}.
	 */
	
	public Pendulum(final PTSymmetryController controller, final Callable<Integer> photogate, final PulsingType pulsingType) {
		++numberOfPendulums;
		pendulumNumber = numberOfPendulums;
		periodDeterminerThread = new PartialPeriodsDeterminerThread(photogate);
		solenoid = new Solenoid(this, photogate, pulsingType);
	}
	
	/**
	 * Determines the asymmetric partial periods of this {@code Pendulum}. Note that the 
	 * {@code Pendulum} must be in a stable swinging state BEFORE this method is called 
	 * in order for it to work properly.
	 */
	
	public final void determineAsymmetricPartialPeriods() { periodDeterminerThread.start(); }
	
	/**
	 * Makes the calling {@code Thread} wait for the {@code periodDeterminerThread} to finish its execution.
	 */
	
	public final void awaitAsymmetricPartialPeriodsDetermination() { 
		try { periodDeterminerThread.join(); } 
		catch (InterruptedException e) { e.printStackTrace(); }	 
	}
	
	/**
	 * Returns the number that is associated with this {@code Pendulum}.
	 * 
	 * @return an {@code int} that is the number associated with this {@code Pendulum}.
	 */
	
	public final int getPendulumNumber() { return pendulumNumber; }
	
	/**
	 * Returns the shorter partial period of this {@code Pendulum} in ms AFTER it has been
	 * calculated by calling the {@code determineAsymmetricPartialPeriods} method.
	 * 
	 * @see #determinAsymmetricPartialPeriods
	 * 
	 * @return a {@code long} which is the amount of milliseconds of the shorter partial period of 
	 * 		   this {@code Pendulum}.
	 */
	
	public final long getShorterPartialPeriod() { return shorterPartialPeriod; }
	
	/**
	 * Returns the longer partial period of this {@code Pendulum} in ms AFTER it has been 
	 * calculated by calling the {@code determineAsymmetricPartialPeriods} method.
	 * 
	 * @see #determineAsymmetricPartialPeriods
	 * 
	 * @return a {@code long} which is the amount of milliseconds of the shorter partial periods of 
	 * 		   this {@code Pendulum}.
	 */
	
	public final long getLongerPartialPeriod() { return longerPartialPeriod; }
	
	/**
	 * Returns the {@code Solenoid} that corresponds to this {@code Pendulum}, which will
	 * pulse the neodymium magnet that is attached to the {@code Pendulum}
	 * 
	 * @return the {@code Solenoid} that corresponds to this {@code Pendulum}.
	 */
	
	public final Solenoid getSolenoid() { return solenoid; }
	
	/**
	 * A helper {@code Thread} that is used to calculate the asymmetrical partial periods of 
	 * the corresponding {@code Pendulum}. A separate {@code Thread} is needed to do this 
	 * properly in order for the asymmetrical partial periods of more than one {@code Pendulum}
	 * to be properly, and simultaneously determined.
	 * 
	 * Note that the number of data points that correspond to a partial period when time values
	 * are collected in manner that they are, is = (n - 1) / 2, where n is the total number of 
	 * time values taken. We need n to be an odd number >= 3 in order to have enough time values 
	 * to have enough data points to have the same number of data points to calculate the average 
	 * partial periods.
	 * 
	 * @author Jon Mrowczynski
	 */
	
	private final class PartialPeriodsDeterminerThread extends Thread {
		
		/**
		 * A {@code StopWatch} that is used to help determine the asymmetrical partial
		 * periods of the associated {@code Pendulum}.
		 */
		
		private final StopWatch stopwatch = new StopWatch();
		
		/**
		 * The acquired time readings from the associated photogate when this
		 * {@code Pendulum} initially starts to block the beam of the photogate.
		 */
		
		private final long[] photogateTimes = new long[21];
		
		/**
		 * The photogate that is polled to determine when it starts to be blocked
		 * by the {@code Pendulum}.
		 */
		
		private final Callable<Integer> photogate;
		
		/**
		 * Basically just names the {@code PartialPeriodsDeterminerThread} and sets the photogate
		 * that is associated with the corresponding {@code Pendulum} such that it can 
		 * be polled.
		 * 
		 * @param photogate that will be polled to determine state changes.
		 */
		
		PartialPeriodsDeterminerThread(final Callable<Integer> photogate) {
			setName("Pendulum " + pendulumNumber + " Thread");
			this.photogate = photogate;
			System.out.println("Created new Thread: " + getName());
		}
		
		/**
		 * Starts the process to determine the partial periods of the corresponding {@code Pendulum}.
		 * 
		 * Note that the {@code Pendulum} should be in a stable swinging state before this {@code Thread}
		 * is started.
		 */
		
		@Override
		public final void start() {
			stopwatch.start();
			for (int i = 0; i < photogateTimes.length; i++) {
				//Once the associated photogate is blocked, store that time
				try { while(photogate.call() != 1); } 
				catch (Exception e) { e.printStackTrace(); }
				photogateTimes[i] = stopwatch.getElapsedTime();
			}
			stopwatch.stop();
			calculatePartialPeriods(photogateTimes);
			System.out.println(getName() + " has finished collecting and processing data.");
		}
		
		/**
		 * Calculate both of the asymmetrical partial periods of the associated {@code Pendulum},
		 * AFTER the data has already been recorded.
		 * 
		 * @param photogateTimes that were acquired from the photogate associated with the {@code Pendulum}
		 * 		  that corresponds to this {@code PeriodDeterminerThread}.
		 */
		
		private final void calculatePartialPeriods(final long[] photogateTimes) {
			long total1 = 0;
			long total2 = 0;
			
			final int numOfDataPoints = (photogateTimes.length - 1) / 2;
			for(int i = 0; i < numOfDataPoints; ++i) {
				final int first = 2 * i;
				total1 += (photogateTimes[first + 1] - photogateTimes[first]);
				total2 += (photogateTimes[first + 2] - photogateTimes[first + 1]);
			}

			final long avg1 = total1 / numOfDataPoints;
			final long avg2 = total2 / numOfDataPoints;
			
			shorterPartialPeriod = Math.min(avg1, avg2);
			longerPartialPeriod = Math.max(avg1, avg2);
		}
				
	} // end of class PartialPeriodsDeterminerThread
	
} // end of class Pendulum