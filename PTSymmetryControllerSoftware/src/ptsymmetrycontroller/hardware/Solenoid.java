package ptsymmetrycontroller.hardware;

import java.util.concurrent.Callable;

import javax.sound.midi.InvalidMidiDataException;
import javax.sound.midi.ShortMessage;

import ptsymmetrycontroller.connections.USBMidiConnection;
import ptsymmetrycontroller.utils.StopWatch;

/**
 * A {@code Solenoid} is able to be pulsed to cause a magnetic force to be applied to 
 * a neodymium magnet that is attached to the string of the pendulum. A {@code Solenoid}
 * can either drive or dampen the swing of a {@code Pendulum} by changing the time at 
 * which the {@code Solenoid} is pulsed. 
 * 
 * @author Jon Mrowczynski
 */

public final class Solenoid {
	
	/**
	 * A {@code Solenoid} can either drive or dampen the swing of a {@code Pendulum} through
	 * its pulses.
	 * 
	 * @author Jon Mrowczynski
	 */
	
	public enum PulsingType { DRIVING, DAMPENING }
	
	/**
	 * The {@code PulsingType} of this {@code Solenoid}. This determined whether the 
	 * {@code Solenoid} will drive or dampen the corresponding {@code Pendulum}.
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
	 * The corresponding {@code Pendulum} that is either driven or damped 
	 * by pulsing this {@code Solenoid}.
	 */
	
	private final Pendulum pendulum;
	
	/**
	 * The {@code Thread} that allows for each of the {@code Solenoid}s to be
	 * pulsed independently of each other.
	 */
	
	private final PulserThread pulserThread;
	
	/**
	 * Constructs the {@code ShortMessage}s that are used to pulse the {@code Solenoid}
	 * and sets up the {@code USBMidiConnection} and {@code PulserThread} such that 
	 * the {@code Solenoid} can be pulsed on command.
	 * 
	 * @param pendulum that corresponds to this {@code Solenoid}.
	 * @param photogate that is used to determine when to pulse the {@code Solenoid}.
	 */
	
	Solenoid(final Pendulum pendulum, final Callable<Integer> photogate, final PulsingType pulsingType) {
	    if (pendulum == null)
	        throw new NullPointerException("pendulum cannot be null.");
	    if (photogate == null)
	        throw new NullPointerException("photogate cannot be null.");
	    if (pulsingType == null)
	        throw new NullPointerException("pulsingType cannot be null");
		final int note = pendulum.getPendulumNumber() == 1 ? 70 : 71;
		try {
			startPulseMessage.setMessage(ShortMessage.NOTE_ON, 0, note, 100);
			stopPulseMessage.setMessage(ShortMessage.NOTE_ON, 0, note, 0);
		} catch (InvalidMidiDataException e) { e.printStackTrace(); }
		this.pendulum = pendulum;
		this.pulsingType = pulsingType;
		pulserThread = new PulserThread(photogate);
	}

	public final PulserThread getPulserThread() { return pulserThread; }
	
	/**
	 * A {@code PulserThread} is used to independently, pulse the corresponding 
	 * {@code Solenoid} independently of pulsing the other {@code Solenoid} setup.
	 * 
	 * @author Jon Mrowczynski
	 */
	
	public final class PulserThread extends Thread {
		
		/**
		 * The {@code StopWatch} that helps the {@code PulserThread} more accurately pulse
		 * the associated {@code Solenoid} at an optimal time to either dampen or drive
		 * the corresponding {@code Pendulum}.
		 */
		
		private final StopWatch stopwatch = new StopWatch();
		
		/**
		 * The amount of time that has elapsed since the {@code StopWatch} was started.
		 */
		
		private long elapsed = 0;
		
		/**
		 * The previous amount of time that has elapsed since the {@code StopWatch} was started.
		 */
		
		private long previousElapsedTime = 0;
		
		/**
		 * The photogate that this {@code PulserThread} will be using to help it determine when to 
		 * optimally pulse the corresponding {@code Solenoid}.
		 */
		
		private final Callable<Integer> photogate;
		
		/**
		 * A {@code boolean} that represents whether the {@code PulserThread} is 
		 * pulsing the {@code Solenoid}.
		 */
		
		private boolean isPulsing = false;
				
		/**
		 * Constructs a {@code PulserThread} to independently pulse the corresponding
		 * {@code Solenoid}.
		 * 
		 * @param photogate that should be associated with the corresponding {@code Solenoid}.
		 */
		
		PulserThread(final Callable<Integer> photogate) {
			setName("Pulser " + pendulum.getPendulumNumber() + " Thread");
			System.out.println("Created new Thread: " + getName());
			this.photogate = photogate;
		}
		
		/**
		 * Starts pulsing the {@code Solenoid}. The manner in which it is pulsed 
		 * is dependent on the {@code PulsingType}.
		 */
		
		@Override
		public final void start() {
			isPulsing = true;
			stopwatch.start();
            pulsePendulum();
			stopwatch.stop();
		}

        /**
         * Stops the {@code PulserThread} from pulsing the {@code Solenoid}.
         */

        public final void stopPulsing() { isPulsing = false; }

		/**
		 * Allows the {@code Solenoid} to either drive/dampen a {@code Pendulum} by waiting for the
		 * {@code Pendulum} to be swinging out of its short/longerlonger partial period side. The
		 * {@code Solenoid} is then pulsed, attracting the magnet towards it while the 
		 * {@code Pendulum} is swinging towards/away from the {@code Solenoid}, adding/taking energy to/out
		 * of the swing of the {@code Pendulum} and driving/dampening it.
		 */

		private void pulsePendulum() {
            long interval;
            while(isPulsing) {
                try {
                    while(photogate.call() != 1);
                    while(photogate.call() != 1);
                } catch (Exception e) { e.printStackTrace(); }
                pulseSolenoid();
                elapsed = (int) stopwatch.getElapsedTime();
                interval = elapsed - previousElapsedTime;
                if ((interval > (pendulum.getShorterPartialPeriod() - 100)) && (interval < (pendulum.getShorterPartialPeriod() + 100))) {
                    if (pulsingType == PulsingType.DAMPENING) {
                        try { Thread.sleep(pendulum.getLongerPartialPeriod() / 2); }
                        catch (InterruptedException e) { e.printStackTrace(); }
                    }
                    pulseSolenoid();
                }
                previousElapsedTime = elapsed;
            }
        }

        /**
         * Pulses the corresponding {@code Solenoid} for about a fifth of a second.
         */

        private void pulseSolenoid() {
            try {
                USBMidiConnection.getInstance().sendMidiMessage(startPulseMessage);
                Thread.sleep(200);
                USBMidiConnection.getInstance().sendMidiMessage(stopPulseMessage);
            } catch(InterruptedException ex) {ex.printStackTrace();}
        }
		
	} // end of class PulserThread
	
} // end of class Solenoid