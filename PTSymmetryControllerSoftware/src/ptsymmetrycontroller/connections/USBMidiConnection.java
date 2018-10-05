package ptsymmetrycontroller.connections;

import java.util.Scanner;

import javax.sound.midi.MidiDevice;
import javax.sound.midi.MidiDevice.Info;
import javax.sound.midi.MidiSystem;
import javax.sound.midi.Receiver;
import javax.sound.midi.ShortMessage;
import javax.sound.midi.MidiUnavailableException;

/**
 * Allows for the creation of a USB to MIDI connection in order to send constructed {@code MidiMessage}s
 * to the microcontroller. Specifically a USB {@code MidiDevice} is acquired, along with its {@code Receiver}.
 * <p>
 * {@code MidiMessage}s are sent to the microcontroller in order to pulse the {@code Solenoid}s.
 * 
 * @author Jon Mrowczynski
 * @version 2.0
 */

public final class USBMidiConnection {
	
	/**
	 * The singleton instance object of this class. There should only ever be one USB to MIDI connection.
	 */
	
	private static final USBMidiConnection usbMidiConnection = new USBMidiConnection();
		
	/**
	 * Returns the singleton instance of this class.
	 * 
	 * @return the singleton {@code USBMidiConnection} instance of this class.
	 */
	
	public static USBMidiConnection getInstance() { return usbMidiConnection; }
	
	/**
	 * No external instances of a {@code USBMidiConnection} should be made.
	 */
	
	private USBMidiConnection() { }
	
	/**
	 * The USB {@code MidiDevice} that is handling the transmission of MIDI signals to the microcontroller.
	 */
	
	private MidiDevice usbMidiDevice = null;
		
	/**
	 * The {@code Receiver} of the USB {@code MidiDevice}. The constructed {@code MidiMessage}s are sent to
	 * this {@code Receiver} in order to send them to the microcontroller.
	 */
	
	private Receiver usbMidiReceiver = null;
		
	/**
	 * Establishes a connection to a USB {@code MidiDevice}, opens it up, and gets its {@code Receiver} so 
	 * that {@code MidiMessage}s can be ultimately sent to the microcontroller.
	 * 
	 * @return a {@code boolean} representing whether the USB to MIDI connection has been successfully established.
	 */
	
	public final boolean establishUSBMidiConnection() { return getUSBMidiDevice() && openUSBMidiDevice() && getUSBMidiDeviceReceiver(); }
	
	/**
	 * Sends the given MIDI {@code ShortMessage} to the USB {@code MidiDevice}'s {@code Receiver} with
	 * a {@code timeStamp} of -1. This is entirely a convenience method.
	 * 
	 * @param message that is to be sent to the USB {@code MidiDevice}'s {@code Receiver}.
	 */
	
	public final void sendMidiMessage(final ShortMessage message) { usbMidiReceiver.send(message, -1); }
	
	/**
	 * Terminates the connections to the USB {@code MidiDevice}.
	 * <p>
	 * Note that closing the USB {@code MidiDevice} implicitly closes its associated acquired {@code Receiver} as well.
	 * 
	 * @return a {@code boolean} representing whether the USB {@code MidiDevice} is closed.
	 */
	
	public final boolean closeUSB() {
		if (usbMidiDevice == null)
			return false;
		if(usbMidiDevice.isOpen()) {
			usbMidiDevice.close();
			System.out.println("USB MidiDevice has been closed.");
		} else 
			System.out.println("USB MidiDevice was already closed.");
		return !usbMidiDevice.isOpen();
	}
	
	/**
	 * Gets the USB {@code MidiDevice} that is connected to the computer whose name contains the 
	 * {@code String} "USB", and whose description is equal to the {@code String} "External MIDI Port".
	 * Otherwise, no USB {@code MidiDevice} would be suitable to be acquired for the purpose of this program.
	 * <p>
	 * If a USB {@code MidiDevice} was acquired, then {@code true} is returned.
	 * <p>
	 * If no suitable USB {@code MidiDevice} can be found, then {@link #USBMidiDeviceError(String, String)} is called,
	 * prompting the user if they would like to retry to search for the USB {@code MidiDevice}. If none can be 
	 * found and the user doesn't want to retry to search for the USB {@code MidiDevice}, then nothing is acquired 
	 * and {@code false} is returned.
	 * 
	 * @return a {@code boolean} representing whether the USB {@code MidiDevice} was successfully acquired.
	 */
	
	private boolean getUSBMidiDevice() {
		boolean foundUSBMidiDevice = false;
		boolean retry = false;
		do {
			for (final Info deviceInfo : MidiSystem.getMidiDeviceInfo()) {
				final boolean isUSBDevice = deviceInfo.getName().contains("USB");
				final boolean isExternalMidiPort = deviceInfo.getDescription().equals("External Midi Port");
				if (isUSBDevice && isExternalMidiPort) {
					try {
						usbMidiDevice = MidiSystem.getMidiDevice(deviceInfo);
						foundUSBMidiDevice = true;
					} catch(MidiUnavailableException ex) { 
						ex.printStackTrace();
						retry = USBMidiDeviceError("Error connecting to the USB MIDI device.",
                                "Make sure that the USB MIDI cable is plugged into the computer before retrying.");
					}
				}
			}
		} while(retry);
		return foundUSBMidiDevice;
	}
	
	/**
	 * Opens the USB {@code MidiDevice} for operation. 
	 * <p>
	 * If it was successfully opened, or if it was already opened, then {@code true} is returned. 
	 * <p>
	 * If a USB {@code MidiDevice} has not yet been acquired, then {@code false} is returned. 
	 * <p>
	 * Otherwise, if a USB {@code MidiDevice} has been acquired, but it could not be open, then 
	 * {@link #USBMidiDeviceError(String, String)} is called, prompting the user if they would like to retry
	 * to open the USB {@code MidiDevice}. If the USB {@code MidiDevice} cannot be opened, and the user 
	 * doesn't want to retry to open the USB {@code MidiDevice} then {@code false} is returned.
	 *
	 * @return a {@code boolean} representing whether the USB {@code MidiDevice} was successfully opened
	 * 		   or is currently open.
	 */
	
	private boolean openUSBMidiDevice() {
		if (usbMidiDevice == null) {
			System.err.println("Cannot open a null USB MidiDevice.");
			System.out.println("Make sure that a USB MidiDevice is acquire first.");
			return false;
		}
		boolean retry = false;
		do {
			if (!usbMidiDevice.isOpen()) {
				try { 
					usbMidiDevice.open(); 
					System.out.println("USB MIDI device was opened for use.");
				}
				catch(MidiUnavailableException ex) { 
					ex.printStackTrace();
					retry = USBMidiDeviceError("Error opening USB Midi device.",
                            "Make sure to close any programs that may be using the USB device.");
				}
			} else
				System.out.println("USB MIDI device was already open for use.");
		} while(retry);
		return usbMidiDevice.isOpen();
	}
	
	/**
	 * Gets the {@code Receiver} of the acquired USB {@code MidiDevice}.
	 * <p>
	 * If it has been successfully obtained, or if it already has been obtained, {@code true} is returned.
	 * <p>
	 * If a USB {@code MidiDevice} has not yet been acquired, then {@code false} is returned.
	 * <p>
	 * If a USB {@code MidiDevice} has been acquired, but has not yet been opened, then {@code false} is 
	 * returned since a {@code Receiver} cannot be obtained from a {@code MidiDevice} that is not opened.
	 * 
	 * @return a {@code boolean} representing whether the {@code Receiver} of the USB {@code MidiDevice} has been successfully acquired.
	 */
	
	private boolean getUSBMidiDeviceReceiver() {
		if (usbMidiDevice == null) {
			System.err.println("Must acquire a USB MidiDevice first before its Receiver can be obtained.");
			return false;
		} else if (!usbMidiDevice.isOpen()) {
			System.err.println("Must open the USB MidiDevice first before its Receiver can be obtained.");
			return false;
		}
		try {
			if(usbMidiReceiver == null) {
				usbMidiReceiver = usbMidiDevice.getReceiver();
				System.out.println("USB MIDI device's Receiver obtained.\n");
			} else
				System.out.println("USB MIDI device's Receiver already obtained.\n");
			return usbMidiReceiver != null;
		} catch(MidiUnavailableException ex) { ex.printStackTrace(); }
		return false;
	}

    /**
     * Provides an {@code errorMessage} as well as a {@code helpfulMessage} to help the user resolve the problem. The
     * option to retry to open the USB {@code MidiDevice} is also presented to the user.
     *
     * And error usually occurs when the USB {@code MidiDevice} has not yet been plugged into the computer or when
     * another program has opened the USB {@code MidiDevice} for its own use and has not yet relinquished control of it.
     *
     * @param errorMessage that is to be displayed to the user
     * @param helpfulMessage that is to help the user resolve the error message
     * @return a {@code boolean} representing whether the user wants to retry to open the USB {@code MidiDevice}.
     */

	private boolean USBMidiDeviceError(final String errorMessage, final String helpfulMessage) {
        final Scanner reader = new Scanner(System.in);
        String retryChoice;
        boolean retry = true;
        do {
            System.out.println("\n" + errorMessage);
            System.out.println(helpfulMessage + "\n");
            System.out.println("Would you like to retry? (Y/N)");
            System.out.print("\tChoice: ");
            retryChoice = reader.next().trim().toUpperCase();
            switch(retryChoice) {
                case "Y":
                    break;
                case "N":
                    System.out.println("\nSure, ending program.");
                    retry = false;
                    break;
                default:
                    System.out.println("That wasn't one of the choices.\n");
                    break;
            }
        } while(!(retryChoice.equals("Y") || retryChoice.equals("N")));
        reader.close();
        return retry;
    }
	
} // end of class USBMidiConnection