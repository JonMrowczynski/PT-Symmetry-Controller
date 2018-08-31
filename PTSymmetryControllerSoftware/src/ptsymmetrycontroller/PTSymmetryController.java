package ptsymmetrycontroller;

import ptsymmetrycontroller.connections.USBMidiConnection;
import ptsymmetrycontroller.hardware.LabQuest2;
import ptsymmetrycontroller.hardware.Pendulum;
import ptsymmetrycontroller.hardware.Solenoid;
import ptsymmetrycontroller.hardware.Solenoid.PulserThread;

import java.util.Scanner;

import java.util.InputMismatchException;

/** 
 * This software is the master control software for a mechanical PT symmetric system, which 
 * is an analog to a PT symmetric quantum mechanical system. 
 * <p>
 * The mechanical system consists of two {@code Pendulum}s that are energetically coupled 
 * together to each other through a string. The amount of energy that is able to be transferred
 * from one swinging {@code Pendulum} to the other is determined by how taught the coupling 
 * string is. 
 * <p>
 * One of these {@code Pendulum}s will have energy put into its swing while the other 
 * {@code Pendulum} will have energy taken out of its swing. This is done by pulsing the
 * {@code Pendulum}'s corresponding {@code Solenoid} at specific times such that a magnetic, 
 * which is attached to the string of the {@code Pendulum} has a force exerted on it. The
 * driven {@code Pendulum} will have its corresponding {@code Solenoid} pulsed when the 
 * {@code Pendulum} is swinging towards the {@code Solenoid}, while the dampened {@code Pendulum} 
 * will have its corresponding {@code Solenoid} pulsed when the {@code Pendulum} is swinging away
 * from the {@code Solenoid}. 
 * <p>
 * Each {@code Pendulum} has a photogate that will assist with determining where the {@code Pendulum}
 * is in its swing. Each photogate is placed offset from the resting position of the corresponding
 * {@code Pendulum}. This allows for two asymmetrical partial periods to be determined, which is
 * then used by the computer to determine when to pulse the corresponding {@code Solenoid}.
 * Note that the two photogates interface with a LabQuest2, which needs to be connected to the 
 * computer that is running this program.
 * <p>
 * Finally, it is the case that a custom circuit is used to help pulse the {@code Solenoids}.
 * This program communicates with a microcontroller using a USB to MIDI connection. Based on the
 * MIDI message that is sent to this microcontroller, and with the help of high amperage transistors,
 * current can be sent to and cut off from the corresponding {@code Solenoid}.
 * <p>
 * With this entire setup, one can setup either a broken, or an unbroken PT symmetric mechanical 
 * system. Note that this "brokenness" is determined by the tautness of the coupling string.
 * Nothing changes within the code.
 * 
 * @author Jon Mrowczynski
 */

public final class PTSymmetryController {
	
	/**
	 * The {@code USBMidiConnection} that will be used to send {@code MidiMessage}s to the 
	 * microcontroller circuit, so that the {@code Solenoid}s of the corresponding {@code Pendulum}s
	 * can be pulsed at specific times.
	 */
		
	private final USBMidiConnection usbMidiConnection;
	
	/**
	 * The {@code LabQuest2} that is used to interface and poll two digital photogates that are 
	 * connected to it.
	 */
	
	private final LabQuest2 labQuest2;
	
	/**
	 * The {@code Pendulum} that will have energy put into its swing.
	 */
	
	private final Pendulum drivenPendulum;
	
	/**
	 * The {@code Pendulum} that will have energy taken out of its swing.
	 */
	
	private final Pendulum dampenedPendulum;
	
	/**
	 * The {@code UserTerminationThread} is used to allow the user to terminate
	 * the program while the {@code Pendulum}s are being pulsed.
	 */
	
	private final UserTerminationThread userTerminationThread;
	
	/**
	 * Constructs a {@code PTSymmetryController} which can be used to mechanically 
	 * display either a broken or unbroken PT symmetric scenario.
	 */
	
	private PTSymmetryController() {
		usbMidiConnection = USBMidiConnection.getInstance();		
		if (!usbMidiConnection.establishUSBMidiConnection()) {
			System.out.println("Could not successfully establish a USB MIDI connection.");
			System.out.println("Terminating Program.");
			System.exit(-1);
		} else
			System.out.println("Successfully established a USB MIDI connection!");
		
		labQuest2 = LabQuest2.getInstance();
		if (!labQuest2.initLabQuest2AndPhotogates()) {
			System.out.println("Could not successfully initialize the Lab Quest 2 and the two photogates.");
			System.out.println("Terminating Program.");
			System.exit(-1);
		}
		
		drivenPendulum = new Pendulum(labQuest2::pollDrivingPhotogate, Solenoid.PulsingType.DRIVING);
		dampenedPendulum = new Pendulum(labQuest2::pollDampeningPhotogate, Solenoid.PulsingType.DAMPENING);
		
		userTerminationThread = new UserTerminationThread();
	}
	
	/**
	 * Prompts the user to start swinging the {@code Pendulum}s before their
	 * partial periods are determined. That information is then used to 
	 * properly pulse the {@code Pendulum}s with their corresponding {@code Solenoid}s, 
	 * such that one of the {@code Pendulum}s is driven and the other {@code Pendulum} is 
	 * dampened.
	 * 
	 * @param args from the console. These are not used.
	 */
		
	public static void main(final String args[]) {
		final PTSymmetryController controller = new PTSymmetryController();				
		final Scanner reader = new Scanner(System.in);
		int input = -1;
		do {
			System.out.print("Enter 1 once the pendulums are in a stable swinging state, or enter 0 to exit: ");
			try {
				input = reader.nextInt();
				if(input == 1) {
					System.out.println("\nStarting Program:");
					Runtime.getRuntime().addShutdownHook(controller.getUserTerminationThread());
					controller.collectAsymmetricalPartialPeriods();
					controller.pulseSolenoids();
				} else if (input == 0)
					System.out.println("\nEnding Program.");
				else
					System.out.println("\nNot an option.");
			} catch(InputMismatchException ex) {
				System.out.println("\nNot an option.");
				reader.next();
			} 
		} while(input != 1 && input != 0);
		reader.close();
		System.out.println("Terminating program.\n");
		if (!USBMidiConnection.getInstance().closeUSB())
			System.err.println("Could not close USB MIDI device");
		LabQuest2.getInstance().closeLabQuest2AndPhotogates();
	}
	
	/**
	 * Collects both of the asymmetrical partial periods of the two {@code Pendulum}s,
	 * and then outputs the results to the command line.
	 */
	
	private void collectAsymmetricalPartialPeriods() {
		drivenPendulum.determineAsymmetricPartialPeriods();
		dampenedPendulum.determineAsymmetricPartialPeriods();
		
		System.out.println("Waiting for both pendulum threads to finish collecting data.");
		drivenPendulum.awaitAsymmetricPartialPeriodsDetermination();
		dampenedPendulum.awaitAsymmetricPartialPeriodsDetermination();
		// Don't really know if this is needed...but it might be a good idea to keep for debugging purposes.
		System.out.println("Pendulum 1: ");
		System.out.println("T1: " + drivenPendulum.getShorterPartialPeriod() + "ms");
		System.out.println("T2: " + drivenPendulum.getLongerPartialPeriod() + "ms");
		System.out.println();
		System.out.println("Pendulum 2: ");
		System.out.println("T1: " + dampenedPendulum.getShorterPartialPeriod() + "ms");
		System.out.println("T2: " + dampenedPendulum.getLongerPartialPeriod() + "ms");
	}
	
	/**
	 * Pulses the solenoids in such a manner that allows for broken or unbroken PT-Symmetry to
	 * be observed in the double pendulum system. The times at which the {@code Pendulums} are
	 * pulsed at are dependent on the asymmetrical partial periods that were calculated from the
	 * {@link #collectAsymmetricalPartialPeriods()} method.
	 */
	
	private void pulseSolenoids() {
		System.out.println("Waiting until the program is terminated by user.");
		final PulserThread drivingPulserThread = drivenPendulum.getSolenoid().getPulserThread();
		final PulserThread dampeningPulserThread = dampenedPendulum.getSolenoid().getPulserThread();
		drivingPulserThread.start();
		dampeningPulserThread.start();
		try {
			drivingPulserThread.join();
			dampeningPulserThread.join();
		} catch(InterruptedException ex) { ex.printStackTrace(); }
	}
	
	/**
	 * Gets the {@code UserTerminationThread} of this {@code PTSymmetryController} instance that
	 * is used to clean up any resources and {@code Thread}s, and gracefully terminate this program.
	 * 
	 * @return the {@code UserTerminationThread} that is used to gracefully terminate the {@code PTSymmetryController}
	 */
	
	private UserTerminationThread getUserTerminationThread() { return userTerminationThread; }
	
	/**
	 * A {@code UserTerminationThread} is used to allow the user to terminate the program
	 * in such a way, where resources can be properly cleaned up and released. This should 
	 * be used while the {@code Pendulum}s are being pulsed by the {@code Solenoid}s so that
	 * the program can terminate gracefully. 
	 * 
	 * @author Jon Mrowczynski
	 */
	
	private final class UserTerminationThread extends Thread {
		
		/**
		 * The {@code Scanner} that waits for user input before this {@code Thread}
		 * cleans up all resources and gracefully terminates the program.
		 */
		
		private final Scanner reader = new Scanner(System.in);
		
		/**
		 * Creates a new {@code UserTerminationThread}.
		 */
		
		UserTerminationThread() {
			super("User Termination Thread");
			System.out.println(getName() + " created.");
		}
		
		/**
		 * Waits for the user to press enter, before gracefully terminating the program.
		 */
		
		@Override
		public void start() {
			System.out.println(getName() + " started.");
			System.out.println("Waiting for user to hit enter to cancel program");
			reader.next();
			System.out.println("Terminating Program.");
			final PulserThread drivingPulserThread = drivenPendulum.getSolenoid().getPulserThread();
			final PulserThread dampeningPulserThread = dampenedPendulum.getSolenoid().getPulserThread();
			if (drivingPulserThread.isPulsing())
				drivingPulserThread.stopPulsing();
			if (dampeningPulserThread.isPulsing())
				dampeningPulserThread.stopPulsing();
			usbMidiConnection.closeUSB();
			labQuest2.closeLabQuest2AndPhotogates();
		}
		
	} // end of class UserTerminationThread
	
} // end of PTSymmetryController