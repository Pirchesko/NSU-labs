package threadPool;

import java.util.Queue;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.Executor;

public class ThreadPool implements Executor {
    private Queue<Runnable> runQueue = new ConcurrentLinkedQueue<>();
    private boolean isRunning = true;
    private int numberStartThread;
    public ThreadPool(int countThread){
        for(int i = 0; i < countThread; i++){
            new Thread(new TaskRunner()).start();
            if (i == 0){
                numberStartThread = (int)Thread.currentThread().getId();
            }
        }
    }
    @Override
    public void execute(Runnable command) {
        if (isRunning == true) {
            runQueue.offer(command);
        }
    }
    private final class TaskRunner implements Runnable {
        @Override
        public void run() {
            while (isRunning) {
                Runnable nextTask = runQueue.poll();
                if (nextTask != null) {
                    nextTask.run();
                }
            }
        }
    }
    public void runOff() {
        isRunning = false;
    }
    public int getNumberStartThread(){
        return numberStartThread;
    }
}
