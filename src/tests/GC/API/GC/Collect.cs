// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// Tests GC.Collect()

using System;
using System.Collections.Generic;
using System.IO;
using System.Diagnostics;
using System.Threading;

public class Test {

    private static int iter = 10;

    struct Log
    {
        public String log;
    }

    private static char[] base_string = "GC  TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST COUNT".ToCharArray();

    // normally it does this:
    // GCs, avg pause
    // 8 gen0's, 2.4 ms
    // 1 gen1, 13.8ms
    // 1 gen2 16.7ms

    // if I only NGC2s, the mean pause is 11.9ms
    void mGCTestNoInstru()
    {
        Stopwatch stopwatch = new Stopwatch();
        stopwatch.Reset();
        stopwatch.Start();

        int count = 0;
        int max = 102400;

        List<Log> list = new List<Log>();

        for (int index1 = 0; index1 < 2000; ++index1)
        {
            for (int index2 = 0; index2 < 1000; ++index2)
            {
                String sd = $"GC  TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST COUNT {count} / {max}";
                Log log = new Log();
                log.log = sd;
                list.Add(log);

                if (list.Count == max)
                {
                    while (list.Count > 0)
                    {
                        list.RemoveAt(0);
                    }
                }
                count++;
            }
        }

        stopwatch.Stop();

        Console.WriteLine("{0:G6}ms", stopwatch.Elapsed.TotalMilliseconds);

        Console.WriteLine("gen0: {0}, gen1: {1}; gen2: {2}, heap size: {3:N0} bytes",
            GC.CollectionCount(0),
            GC.CollectionCount(1),
            GC.CollectionCount(2),
            GC.GetTotalMemory(false));

        Console.WriteLine("finished");
    }

    void mGCTest(int longestCount)
    {
        Stopwatch stopwatchTotal = new Stopwatch();
        Stopwatch stopwatch = new Stopwatch();

        List<double> iterationPauses = new List<double>(2000);

        int count = 0;
        int max = 102400;

        stopwatchTotal.Start();

        List<Log> list = new List<Log>();
        //List<Log> list = new List<Log>(102420);

        for (int index1 = 0; index1 < 2000; ++index1)
        {
            int collectionCountStart = GC.CollectionCount(0);
            stopwatch.Restart();

            for (int index2 = 0; index2 < 1000; ++index2)
            {
                String sd = $"GC  TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST COUNT {count} / {max}";
                Log log = new Log();
                log.log = sd;
                list.Add(log);

                if (list.Count == max)
                //if (list.Count >= max)
                {
                    while (list.Count > 0)
                    {
                        list.RemoveAt(0);
                    }
                }
                count++;
            }

            stopwatch.Stop();
            double elapsed = stopwatch.Elapsed.TotalMilliseconds;
            iterationPauses.Add(elapsed);
            if ((elapsed > 10) && (elapsed < 100))
            //if (elapsed > 100)
            {
                int collectionCountEnd = GC.CollectionCount(0);
                Console.WriteLine("iteration {0} took {1}ms (> 100), did {2} GCs (#{3})", index1, elapsed, (collectionCountEnd - collectionCountStart), collectionCountEnd);
                //Console.ReadLine();
            }
        }

        stopwatchTotal.Stop();
        Console.WriteLine("{0:G6}ms", stopwatchTotal.Elapsed.TotalMilliseconds);

        Console.WriteLine("gen0: {0}, gen1: {1}; gen2: {2}, heap size: {3:N0} bytes",
            GC.CollectionCount(0),
            GC.CollectionCount(1),
            GC.CollectionCount(2),
            GC.GetTotalMemory(false));

        Console.WriteLine("finished");

        int numIterationPauses = iterationPauses.Count;
        if (numIterationPauses >= 0)
        {
            iterationPauses.Sort();
            
            Console.WriteLine("===============STATS=================");

            int startIndex = ((numIterationPauses < longestCount) ? 0 : (numIterationPauses - longestCount));
            for (int i = startIndex; i < numIterationPauses; i++)
            {
                Console.WriteLine(iterationPauses[i]);
            }
        }
    }

    // with concurrent disabled, the mean pause is 350ms (vs >1s when we do ephemeral GCs)
    static void JanTest()
    {
        object[] a = new object[100_000_000];
        Stopwatch stopwatch = new Stopwatch();
        long loop = 0;
        double total_elapsed_ms = 0;
        stopwatch.Reset();
        stopwatch.Start();

        for (; ; )
        {
            loop++;
            if ((loop % iter) == 0)
            {
                Console.WriteLine("{0:G6}: Iteration {1}",
                    (stopwatch.Elapsed.TotalMilliseconds - total_elapsed_ms),
                    loop);

                Console.WriteLine("gen0: {0}, gen1: {1}; gen2: {2}, heap size: {3:N0} bytes",
                    GC.CollectionCount(0),
                    GC.CollectionCount(1),
                    GC.CollectionCount(2),
                    GC.GetTotalMemory(false));

                //GC.Collect();

                total_elapsed_ms = stopwatch.Elapsed.TotalMilliseconds;

            }
            // Create a lot of Gen2 -> Gen0 references to keep the GC busy
            object o = new object();
            for (int i = 0; i < a.Length; i++)
            {
                a[i] = o;
                //every 524287th element
                //if ((i & 0x7ffff) == 0)
                //every 32k elements
                if ((i & 0x7fff) == 0)
                {
                    // didn't work
                    //GC.KeepAlive(a);
                    var _ = Thread.CurrentThread.IsThreadPoolThread;
                }
            }
            // Create a lot of short lived objects to trigger Gen0 GC
            for (int i = 0; i < 1000; i++)
            {
                GC.KeepAlive(new string('a', 10000));
            }
        }
    }

    public static void Main(String[] args)
    {
        //iter = int.Parse(args[0]);
        //JanTest();
        Test t = new Test();
        t.mGCTest(int.Parse(args[0]));
    }
}
