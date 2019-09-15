package ptsymmetrycontroller.hardware;

import java.util.concurrent.Callable;

import ptsymmetrycontroller.hardware.Solenoid.PulsingType;
import ptsymmetrycontroller.utils.StopWatch;

/**
 * A {@code Pendulum} allows for the acquisition of the asymmetrical periods.
 *
 * A {@code PartialPeriodsDeterminerThread} is used to independently calculate its asymmetrical partial periods. This
 * allows for the photogate's timing information to be acquired with minimal interference from the rest of the program.
 *
 * Once the {@code Thread} has recorded enough values to calculate this {@code Pendulum}'s asymmetric partial periods,
 * the {@code Thread} calls {@code calculatePartialPeriods()}.
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
	 * A helper {@code Thread} that allows for the asymmetrical partial periods of this {@code Pendulum} to be acquired.
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
	 * The {@code Solenoid} that corresponds to this {@code Pendulum} that will pulse the magnet that is attached to it.
	 */
	
	private final Solenoid solenoid;
	
	/**
	 * Constructs a {@code Pendulum} that the user needs to manually start swinging.
	 * 
	 * @param photogate that is associated with this {@code Pendulum}.
	 */
	
	public Pendulum(final Callable<Integer> photogate, final PulsingType pulsingType) {
	    if (photogate == null) { throw new NullPointerException("photogate cannot be null."); }
	    if (pulsingType == null) { throw new NullPointerException("photogate cannot be null."); }
		++numberOfPendulums;
		pendulumNumber = numberOfPendulums;
		periodDeterminerThread = new PartialPeriodsDeterminerThread(photogate);
		solenoid = new Solenoid(this, photogate, pulsingType);
	}
	
	/**
	 * Determines the asymmetric partial periods of this {@code Pendulum}. The {@code Pendulum} must be in a stable
	 * swinging state before this method is called in order to get the best results.
	 */
	
	public final synchronized void determineAsymmetricPartialPeriods() { periodDeterminerThread.start(); }
	
	/**
	 * Makes the calling {@code Thread} wait for the {@code periodDeterminerThread} to finish its execution.
	 */
	
	public final synchronized void awaitAsymmetricPartialPeriodsDetermination() {
		try { periodDeterminerThread.join(); } 
		catch (InterruptedException e) { e.printStackTrace(); }	 
	}
	
	/**
	 * Returns the number that is associated with this {@code Pendulum}.
	 * 
	 * @return an {@code int} that is the number associated with this {@code Pendulum}.
	 */
	
	final int getPendulumNumber() { return pendulumNumber; }
	
	/**
	 * Returns the shorter partial period of this {@code Pendulum} in ms after it has been calculated by calling
	 * {@code determineAsymmetricPartialPeriods}.
	 * 
	 * @see #determineAsymmetricPartialPeriods()
	 * 
	 * @return a {@code long} which is the amount of milliseconds of the shorter partial period of this
	 * 		   {@code Pendulum}.
	 */
	
	public final long getShorterPartialPeriod() { return shorterPartialPeriod; }
	
	/**
	 * Returns the longer partial period of this {@code Pendulum} in ms after it has been calculated by calling
	 * {@code determineAsymmetricPartialPeriods}.
	 * 
	 * @see #determineAsymmetricPartialPeriods()
	 * 
	 * @return a {@code long} which is the amount of milliseconds of the longer partial period of this {@code Pendulum}.
	 */
	
	public final long getLongerPartialPeriod() { return longerPartialPeriod; }
	
	/**
	 * Returns the {@code Solenoid} that corresponds to this {@code Pendulum} which will pulse the neodymium magnet that
	 * is attached to the {@code Pendulum}.
	 * 
	 * @return the {@code Solenoid} that corresponds to this {@code Pendulum}.
	 */
	
	public final Solenoid getSolenoid() { return solenoid; }
	
	/**
	 * A helper {@code Thread} that is used to calculate the asymmetrical partial periods of the corresponding
	 * {@code Pendulum}. A separate {@code Thread} is needed in order for the asymmetrical partial periods of more than
	 * one {@code Pendulum} to be properly, and simultaneously determined.
	 * 
	 * The number of data points that correspond to a partial period when time values are collected in the manner that
	 * they are, is = (n - 1) / 2, where n is the total number of time values taken. We need n to be an odd number >= 3
	 * in order to have enough time values and to have the same number of data points to calculate both average
	 * asymmetrical partial periods.
	 * 
	 * @author Jon Mrowczynski
	 */
	
	private final class PartialPeriodsDeterminerThread extends Thread {

		/**
		 * The number of timing samples that this {@code PartialPeriodsDeterminerThread} should take in order to
		 * calculate this {@code Pendulum}'s asymmetric partial periods.
		 */

		private static final int NUM_OF_SAMPLES = 21;
		
		/**
		 * A {@code StopWatch} that is used to help determine the asymmetrical partial periods of the associated
		 * {@code Pendulum}.
		 */
		
		private final StopWatch stopwatch = new StopWatch();
		
		/**
		 * The acquired time readings from the associated photogate when this {@code Pendulum} initially starts to block
		 * the beam of the photogate.
		 */
		
		private final long[] photogateTimes = new long[NUM_OF_SAMPLES];
		
		/**
		 * The photogate that is polled to determine when it starts to be blocked by the {@code Pendulum}.
		 */
		
		private final Callable<Integer> photogate;
		
		/**
		 * Sets the name of the {@code PartialPeriodsDeterminerThread} and sets the photogate that is associated with
		 * the corresponding {@code Pendulum} so that it can be polled to acquire timing information.
		 * 
		 * @param photogate that will be polled to determine blocked/unblocked state changes.
		 */
		
		PartialPeriodsDeterminerThread(final Callable<Integer> photogate) {
            if (photogate == null) { throw new NullPointerException("photogate cannot be null."); }
			setName("Pendulum " + pendulumNumber + " Thread");
			this.photogate = photogate;
			System.out.println("Created " + getName());
		}
		
		/**
		 * Starts this {@code PartialPeriodsDeterminerThread} to determine the partial periods of the corresponding
		 * {@code Pendulum}.
		 * 
		 * The {@code Pendulum} should be in a stable swinging state before this method is called to get the best
		 * results.
		 */
		
		@Override
		public final void start() {
			stopwatch.start();
			for (int i = 0; i < photogateTimes.length; ++i) {
				//Once the associated photogate is blocked, store that time
				try {
					while(photogate.call() != 1);
					photogateTimes[i] = stopwatch.getElapsedTime();
				}
				catch (Exception e) {
					e.printStackTrace();
					System.out.println("Failed to successfully collect enough samples.");
					System.exit(1);
				}
			}
			stopwatch.stop();
			calculatePartialPeriods(photogateTimes);
			System.out.println(getName() + " has finished collecting and processing data.");
		}
		
		/**
		 * Calculate both asymmetrical partial periods of the associated {@code Pendulum} after the data has already
		 * been recorded.
		 * 
		 * @param photogateTimes that were acquired from the photogate associated with the {@code Pendulum} that
		 *                       corresponds to this {@code PeriodDeterminerThread}.
         * @throws NullPointerException if {@code photogateTimes} is {@code null}.
         * @throws IllegalArgumentException if the length of {@code photogateTimes} is not {@link #NUM_OF_SAMPLES}.
		 */

		private void calculatePartialPeriods(final long[] photogateTimes) throws NullPointerException, IllegalArgumentException{
		    if (photogateTimes == null) { throw new NullPointerException("photogateTimes cannot be null."); }
		    if (photogateTimes.length != NUM_OF_SAMPLES) {
				throw new IllegalArgumentException("photogateTimes required length: " + NUM_OF_SAMPLES
						+ ", but found length " + photogateTimes.length);
			}
			long total1 = 0;
			long total2 = 0;
			
			final int numOfDataPoints = (photogateTimes.length - 1) / 2;
			for(int i = 0; i < numOfDataPoints; i = 2 * (i + 1)) {
				total1 += (photogateTimes[i + 1] - photogateTimes[i]);
				total2 += (photogateTimes[i + 2] - photogateTimes[i + 1]);
			}

			final long avg1 = total1 / numOfDataPoints;
			final long avg2 = total2 / numOfDataPoints;
			
			shorterPartialPeriod = Math.min(avg1, avg2);
			longerPartialPeriod = Math.max(avg1, avg2);
		}
				
	} // end of class PartialPeriodsDeterminerThread
	
} // end of class Pendulum