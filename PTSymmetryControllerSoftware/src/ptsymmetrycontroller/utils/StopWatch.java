package ptsymmetrycontroller.utils;

/**
 * A {@code StopWatch} is a simple class that is used to help determine the asymmetrical partial 
 * periods of the {@code Pendulum}s. 
 * It is also used to determine when it is appropriate to pulse the {@code Solenoid}s to either
 * drive or dampen the motion of the {@code Pendulum}s.
 * 
 * @author Jon Mrowczynski
 * @version 2.0
 */

public final class StopWatch {
	
	/**
	 * The time at which the {@code StopWatch} is started.
	 */
	
    private long startTime = 0;
    
    /**
     * The time at which the (@code StopWatch} is stopped.
     */
    
    private long stopTime = 0;
    
    /**
     * Represents whether the {@code StopWatch} is running.
     */
    
    private boolean running = false;
    
    /**
     * Starts the {@code StopWatch}.
     * <p>
     * Note that the {@code StopWatch} can be restarted by calling this method after it
     * has been stopped.
     */

    public final void start() {
        startTime = System.currentTimeMillis();
        running = true;
    }
    
    /**
     * Stops the {@code StopWatch}.
     */

    public final void stop() {
        stopTime = System.currentTimeMillis();
        running = false;
    }
    
    /**
     * Gets the elapsed time in milliseconds. This can be called either while the 
     * {@code StopWatch} is running, or after it has been stopped.
     * 
     * @return a {@code long} that represents the elapsed time in milliseconds.
     */

    public final long getElapsedTime() { return running ? (System.currentTimeMillis() - startTime) : (stopTime - startTime); }
    
} // end of class StopWatch