using System;
using System.Threading.Tasks;

class Program
{
    static async Task Main(string[] args)
    {
        var task = DoSomethingAsync();
        
        Console.WriteLine("Main: start working...");

        task.Wait();
    }

    private static async Task DoSomethingAsync()
    {
        Console.WriteLine("Async: start working...");
        await Task.Delay(3000);
    }
}