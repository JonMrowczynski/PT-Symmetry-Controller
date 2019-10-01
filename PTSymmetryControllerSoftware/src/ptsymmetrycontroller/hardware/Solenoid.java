package ptsymmetrycontroller.hardware;

import java.util.concurrent.Callable;

import javax.sound.midi.InvalidMidiDataException;
import javax.sound.midi.ShortMessage;

import ptsymmetrycontroller.connections.UsbMidiConnection;
import ptsymmetrycontroller.utils.StopWatch;

/**
 * A {@code Solenoid} is able to be pulsed to cause a magnetic force to be applied to a neodymium magnet that is
 * attached to the string of the pendulum. A {@code Solenoid} can either drive or dampen the swing of a {@code Pendulum}
 * by changing the time at which the {@code Solenoid} is pulsed relative to the swing direction of the pendulum.
 * 
 * @author Jon Mrowczynski
 */

public final class Solenoid {
	
	/**
	 * A {@code Solenoid} can either drive or dampen the swing of a {@code Pendulum} when it is pulsed.
	 * 
	 * @author Jon Mrowczynski
	 */
	
	public enum PulsingType { DRIVING, DAMPENING }
	
	/**
	 * The {@code PulsingType} of this {@code Solenoid}. This determines whether the {@code Solenoid} will drive or
	 * dampen the corresponding {@code Pendulum}.
	 */
	
	private final PulsingType pulsingType; 
	
	/**
	 * The {@code ShortMessage} that starts a pulse of the {@code Solenoid}. 
	 */
	
	private final ShortMessage startPulseMessage = new ShortMessage();
	
	/**
	 * The {@code ShortMessage} that stops a pulse of the {@code Solenoid}. 
	 */
	
	private final ShortMessage stopPulseMessage = new ShortMessage();
	
	/**
	 * The corresponding {@code Pendulum} that is either driven or damped by pulsing this {@code Solenoid}.
	 */
	
	private final Pendulum pendulum;
	
	/**
	 * The {@code Thread} that allows for each of the {@code Solenoid}s to be pulsed independently of each other.
	 */
	
	private final PulserThread pulserThread;
	
	/**
	 * Constructs the {@code ShortMessage}s that are used to pulse the {@code Solenoid} and sets up the
	 * {@code UsbMidiConnection} and {@code PulserThread} such that the {@code Solenoid} can be pulsed on command.
	 * 
	 * @param pendulum that corresponds to this {@code Solenoid}.
	 * @param photogate that is used to determine when to pulse the {@code Solenoid}.
	 */
	
	Solenoid(final Pendulum pendulum, final Callable<Integer> photogate, final PulsingType pulsingType) {
	    if (pendulum == null) { throw new NullPointerException("pendulum cannot be null."); }
	    if (photogate == null) { throw new NullPointerException("photogate cannot be null."); }
	    if (pulsingType == null) { throw new NullPointerException("pulsingType cannot be null."); }
		final int note = pulsingType.ordinal();
		try {
			startPulseMessage.setMessage(ShortMessage.NOTE_ON, 0, note, 1);
			stopPulseMessage.setMessage(ShortMessage.NOTE_ON, 0, note, 0);
		} catch (InvalidMidiDataException e) { e.printStackTrace(); }
		this.pendulum = pendulum;
		this.pulsingType = pulsingType;
		pulserThread = new PulserThread(photogate);
	}

	/**
	 * Returns the {@code PulsingType} of this {@code Solenoid}.
	 *
	 * @return the {@code PulsingType} of this {@code Solenoid}.
	 */

	final PulsingType getPulsingType() { return pulsingType; }

	/**
	 * Returns the {@code PulserThread} of this {@code Solenoid}.
	 *
	 * @return the {@code PulserThread} of this {@code Solenoid}.
	 */

	public final PulserThread getPulserThread() { return pulserThread; }
	
	/**
	 * A {@code PulserThread} is used to pulse this {@code Solenoid} independently of the other {@code Solenoid}.
	 * 
	 * @author Jon Mrowczynski
	 */
	
	public final class PulserThread extends Thread {
		
		/**
		 * The {@code StopWatch} that helps the {@code PulserThread} more accurately pulse the associated
		 * {@code Solenoid} at an optimal time to either dampen or drive the corresponding {@code Pendulum}.
		 */
		
		private final StopWatch stopwatch = new StopWatch();

		/**
		 * The previous amount of time that has elapsed since the {@code StopWatch} was started.
		 */
		
		private long previousElapsedTime = 0;
		
		/**
		 * The photogate that this {@code PulserThread} will be using to help it determine when to optimally pulse the
		 * corresponding {@code Solenoid}.
		 */
		
		private final Callable<Integer> photogate;
		
		/**
		 * A {@code boolean} that represents whether the {@code PulserThread} is running.
		 */
		
		private boolean isRunning = false;
				
		/**
		 * Constructs a {@code PulserThread} to independently pulse the corresponding {@code Solenoid}.
		 * 
		 * @param photogate that should be associated with the corresponding {@code Solenoid}.
		 */
		
		PulserThread(final Callable<Integer> photogate) {
			if (photogate == null) { throw new NullPointerException("photogate cannot be null."); }
			setName(pulsingType + " Pulser Thread");
			this.photogate = photogate;
		}
		
		/**
		 * Pulses the {@code Solenoid} based on this {@code PulserThread}'s {@code PulsingType}.
		 */
		
		@Override
		public final void run() {
			isRunning = true;
			stopwatch.start();
            pulsePendulum();
			stopwatch.stop();
		}

        /**
         * Stops the {@code PulserThread} from pulsing the {@code Solenoid}.
         */

        public final void stopPulsing() { isRunning = false; }

		/**
		 * Allows the {@code Solenoid} to either drive/dampen a {@code Pendulum} by waiting for the {@code Pendulum} to
		 * be swinging out of its shorter/longer partial period side. The {@code Solenoid} is then pulsed, attracting
		 * the magnet towards it while the {@code Pendulum} is swinging towards/away from the {@code Solenoid},
		 * adding/taking energy to/out of the swing of the {@code Pendulum} and driving/dampening it.
		 */

		private void pulsePendulum() {
            long interval;
            while(isRunning) {
                try {
                    while(photogate.call() != 1);
                    while(photogate.call() != 1);
                } catch (Exception e) { e.printStackTrace(); }
                pulseSolenoid();
				final long elapsed = (int) stopwatch.getElapsedTime();
                interval = elapsed - previousElapsedTime;
                if ((interval > (pendulum.getShorterPartialPeriod() - 100)) && (interval < (pendulum.getShorterPartialPeriod() + 100))) {
                    if (pulsingType == PulsingType.DAMPENING) {
                        try { Thread.sleep(pendulum.getLongerPartialPeriod() / 2); }
                        catch (InterruptedException e) { e.printStackTrace(); }
                    } else { pulseSolenoid(); }
                }
                previousElapsedTime = elapsed;
            }
        }

        /**
         * Pulses the corresponding {@code Solenoid} for about a fifth of a second.
         */

        private void pulseSolenoid() {
            try {
                UsbMidiConnection.getInstance().sendMidiMessage(startPulseMessage);
                Thread.sleep(200);
                UsbMidiConnection.getInstance().sendMidiMessage(stopPulseMessage);
            } catch(InterruptedException ex) {ex.printStackTrace();}
        }
		
	} // end of class PulserThread
	
} // end of class Solenoid