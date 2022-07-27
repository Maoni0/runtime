// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

// Tests GC.Collect(1)

using System;
using System.Diagnostics;

public class Test_Collect1
{
    public enum Colors
    {
        Red = 0x1,
        Orange = 0x2,
        Yellow = 0x4,
        Green = 0x8,
        Blue = 0x10
    }

    public string EnumToString(Colors value) => value.ToString();

    public static int Main(string[] args)
    {
		Test_Collect1 test_obj = new Test_Collect1();
		
        Stopwatch stopwatch = new Stopwatch();

        stopwatch.Start();
        
        ulong total_iter = (ulong)Int32.Parse(args[0]) * 1000000;

        for (ulong i = 0; i < total_iter; i++)
        {
            test_obj.EnumToString(Colors.Yellow);
        }

        Console.WriteLine ("gen0: {0}, gen1: {1}; gen2: {2}, heap size: {3:N0} bytes, {4:G6} ms", 
            GC.CollectionCount(0), 
            GC.CollectionCount(1), 
            GC.CollectionCount(2),
            GC.GetTotalMemory(false),
            stopwatch.Elapsed.TotalMilliseconds);

		return 1;
    }
}
