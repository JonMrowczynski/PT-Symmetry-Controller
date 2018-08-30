package ptsymmetrycontroller.hardware;

/**
 * The {@code LabQuest2} is the device that interfaces with the two photogates. It can 
 * poll and return the states of the two photogates such that it can be determined 
 * when they are blocked by the corresponding {@code Pendulum}. 
 * <p>
 * Note that a 1 represents that the photogate is not blocked, while a 0 indicates that 
 * the photogate is blocked.
 * 
 * @author Jon Mrowczynski
 */

public final class LabQuest2 {
	
	/**
	 * Load the dlls required to interface with the {@code LabQuest2}.
	 */
	
	static { System.loadLibrary("LabQuest2Photogates"); }
	
	/**
	 * The singleton instance of this class. There should only ever be one 
	 * instance of the {@code LabQuest2}, since the computer will only ever 
	 * interface with one {@code LabQuest2}.
	 */
	
	private static final LabQuest2 labQuest2 = new LabQuest2();
	
	/**
	 * Returns the singleton instance of this class.
	 * 
	 * @return the single {@code LabQuest2} instance of this class.
	 */
	
	public static final LabQuest2 getInstance() { return labQuest2; }
	
	/**
	 * No external instances of a {@code LabQuest2} should be made.
	 */
	
	private LabQuest2() { }
	
	/**
	 * First, this method makes sure that a {@code LabQuest2} is connected to the computer
	 * via a USB connection. 
	 * 
	 * It then attempts to acquire and open the {@code LabQuest2}. 
	 * 
	 * Finally, it makes sure that two digital photogates are connected to the {@code LabQuest2}, 
	 * and then acquires them. 
	 * 
	 * If all of these steps are performed successfully, then {@code true} is returned. Else, 
	 * {@code false} is returned if any one of them fail.
	 * 
	 * @return a {@code boolean} representing whether the {@code LabQuest2} and the two photogates
	 * 		   have been setup and acquired properly.
	 */

	public final native synchronized boolean initLabQuest2AndPhotogates();
	
	/**
	 * Closes and terminates the photogates connected to the {@code LabQuest2}.
	 * This also closes the {@code LabQuest2} and releases any resources that are
	 * related to the photogates and the {@code LabQuest2}.
	 */
	
	public final native synchronized void closeLabQuest2AndPhotogates();
	
	/**
	 * Returns the current state of the photogate that is associated with the
	 * {@code Pendulum} that is being driven.
	 * 
	 * @return an {@code int} representing the blocked state of this photogate.
	 */
	
	public final native synchronized int pollDrivingPhotogate();
	
	/**
	 * Returns the current state of the photogate is associated with the
	 * {@code Pendulum} that is being dampened.
	 * 
	 * @return an {@code int} representing the blocked state of this photogate.
	 */
	
	public final native synchronized int pollDampeningPhotogate();
	
} // end of class LabQuest2
