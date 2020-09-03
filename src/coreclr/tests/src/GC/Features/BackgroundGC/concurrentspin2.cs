// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

using System;
using System.Diagnostics;
using System.Threading;

class PriorityTest
{
    private byte[][] old;
    private byte[][] med;
    private Random rand;

    private int oldDataSize;
    private int medDataSize;
    private int iterCount;
    private int meanAllocSize;
    private int medTime;
    private int youngTime;
    private int allocMbPerRequest;

    public PriorityTest(int oldDataSize, int medDataSize,
                        int iterCount, int meanAllocSize,
                        int medTime, int youngTime, int allocMbPerRequest)
    {
        Console.WriteLine("olddata: {0}, meddata: {1}, medtime: {2}, youngtime: {3}",
            oldDataSize, medDataSize, medTime, youngTime);

        rand = new Random(314159);
        this.oldDataSize = oldDataSize;
        this.medDataSize = medDataSize;
        this.iterCount = iterCount;
        this.meanAllocSize = meanAllocSize;
        this.medTime = medTime;
        this.youngTime = youngTime;
        this.allocMbPerRequest = allocMbPerRequest;
    }

    // creates initial arrays
    void AllocTest(int oldDataSize, int medDataSize, int meanAllocSize)
    {
        old = new byte[oldDataSize][];
        med = new byte[medDataSize][];

        for (int i = 0; i < old.Length; i++)
        {
            old[i] = new byte[meanAllocSize];
        }

        for (int i = 0; i < med.Length; i++)
        {
            med[i] = new byte[meanAllocSize];
        }
    }

    // churns data in the heap by replacing byte arrays with new ones of random length
    // this should induce concurrent GCs
    void SteadyState(int oldDataSize, int medDataSize,
                        int iterCount, int meanAllocSize,
                        int medTime, int youngTime)
    {
        //int totalAllocsPerRequest = allocMbPerRequest * 1024 * 1024 / (meanAllocSize + 24);
        int totalAllocsPerRequest = allocMbPerRequest * 1024 * 1024 / (24 + 24);
        Console.WriteLine("total {0} alloc bytes, {1} per alloc, {2} allocs per request, {3}k iterations",
            (allocMbPerRequest * 1024 * 1024), meanAllocSize, totalAllocsPerRequest, iterCount);

        long allocatedSoFar = GC.GetAllocatedBytesForCurrentThread();

        int allocsPerRequest = 0;
        int medIndex = 0;

        for (ulong i = 0; i < (ulong)iterCount*1000; i++)
        {
            byte[] newarray = new byte[meanAllocSize];
            allocsPerRequest++;

            //if ((i % (ulong)medTime) == 0)
            //{
            //    old[rand.Next(0, old.Length)] = newarray;
            //}

            if ((i % (ulong)youngTime) == 0)
            {
                med[medIndex] = newarray;
                medIndex++;
                if (medIndex >= med.Length)
                {
                    int allocsRemainingPerRequest = totalAllocsPerRequest - allocsPerRequest;
                    
                    long currentAllocatedSoFar = GC.GetAllocatedBytesForCurrentThread();
                    //Console.WriteLine("{0} allocs for this request so far, {1} remaining, allocated {2} bytes so far",
                    //    allocsPerRequest, allocsRemainingPerRequest, (currentAllocatedSoFar - allocatedSoFar));

                    allocatedSoFar = currentAllocatedSoFar;

                    for (int remain_index = 0; remain_index < allocsRemainingPerRequest; remain_index++)
                    {
                        byte[] remainarray = new byte[meanAllocSize];
                        allocsPerRequest++;
                    }

                    currentAllocatedSoFar = GC.GetAllocatedBytesForCurrentThread();
                    //Console.WriteLine("allocated {0} completely temp bytes", (currentAllocatedSoFar - allocatedSoFar));

                    allocsPerRequest = 0;
                    allocatedSoFar = currentAllocatedSoFar;

                    // request is done, only surv a few elements -
                    for (int med_index = 0; med_index < med.Length; med_index++)
                    {
                        if ((med_index % medTime) == 0)
                        {
                            // surv this to old
                            old[rand.Next(0, old.Length)] = med[med_index];
                        }

                        med[med_index] = null;
                    }

                    medIndex = 0;
                }
            }
        }
    }

    // method that runs the test
    public void RunTest()
    {
        //for (int iteration = 0; iteration < iterCount; iteration++)
        {
            AllocTest(oldDataSize, medDataSize, meanAllocSize);

            SteadyState(oldDataSize, medDataSize,
                iterCount, meanAllocSize,
                medTime, youngTime);

            //if (((iteration + 1) % 20) == 0)
            //    Console.WriteLine("Thread: {1} Finished iteration {0}", iteration, System.Threading.Thread.CurrentThread.Name);
        }

    }

}


class ConcurrentRepro
{

    public static void Usage()
    {
        Console.WriteLine("Usage:");
        Console.WriteLine("\t<num iterations> <num threads>");
    }

    public static int[] ParseArgs(string[] args)
    {
        int[] parameters = new int[7];

        // set defaults
        parameters[0] = 100;
        parameters[1] = 4;
        parameters[2] = 1000000;
        parameters[3] = 500;
        parameters[4] = 30;
        parameters[5] = 30;
        parameters[6] = 3;

        if (args.Length == 0)
        {
            //use defaults
            Console.WriteLine("Using defaults: 100 iterations, 4 threads");
            return parameters;
        }
        if (args.Length == parameters.Length)
        {
            for (int i = 0; i < args.Length; i++)
            {
                int j = 0;
                if (!int.TryParse(args[i], out j))
                {
                    Usage();
                    return null;
                }
                parameters[i] = j;
            }

            return parameters;
        }

        // incorrect number of arguments        
        Usage();
        return null;
    }

    public static int Main(string[] args)
    {

        // parse arguments
        int[] parameters = ParseArgs(args);
        if (parameters == null)
        {
            return 0;
        }

        // set process affinity to 1 to repro bug easier
        //Process.GetCurrentProcess().ProcessorAffinity = (IntPtr)1;

        Console.WriteLine("{0} threads, olddata: {1}, meddata: {2}, medtime: {3}, youngtime: {4}, total mb/rq {5}",
            parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);

        PriorityTest[] priorityTestArray = new PriorityTest[parameters[1]];
        // create threads
        Thread[] threads = new Thread[parameters[1]];

        Stopwatch stopwatch = new Stopwatch();
        stopwatch.Start();

        for (int i = 0; i < threads.Length; i++)
        {
            // 780:260:7
            // 438:146:4
            //priorityTestArray[i] = new PriorityTest(1000000, 500, parameters[0], 17, 300, 3);

            //515:172:4
            //priorityTestArray[i] = new PriorityTest(1000000, 500, parameters[0], 17, 300, 30);

            //51:12:7
            //priorityTestArray[i] = new PriorityTest(1000000, 500, parameters[0], 17, 30, 30);

            //51:12:8
            //priorityTestArray[i] = new PriorityTest(1000000, 5000, parameters[0], 17, 30, 30);

            //51:12:8
            //priorityTestArray[i] = new PriorityTest(1000000, 5000, parameters[0], 17, 30, 3);

            priorityTestArray[i] = new PriorityTest(parameters[2], parameters[3], parameters[0], 17, parameters[4], parameters[5], parameters[6]);

            ThreadStart startDelegate = new ThreadStart(priorityTestArray[i].RunTest);
            threads[i] = new Thread(startDelegate);
            threads[i].Name = String.Format("Thread{0}", i);
            //if (i % 2 == 0)
            //{
            //    threads[i].Priority = ThreadPriority.Lowest;
            //}
            threads[i].Start();
        }

        // wait for threads to complete
        for (int i = 0; i < threads.Length; i++)
        {
            threads[i].Join();
        }

        stopwatch.Stop();
        Console.WriteLine("{0:00.0}s, gen0:1:2 - {1}:{2}:{3}- {4:00.00}",
            stopwatch.Elapsed.TotalSeconds,
            GC.CollectionCount(0),
            GC.CollectionCount(1),
            GC.CollectionCount(2), ((double)GC.CollectionCount(0) / (double)GC.CollectionCount(1)));

        return 100;
    }
}


