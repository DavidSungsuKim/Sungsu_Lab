using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PracticeCsharp
{
    class Program
    {
        //Mine instance;
        static async Task Main(string[] args)
        {
            Mine instance = new Mine();
            instance.x = 3;

            int x = 0;
            int y = 1;

            Console.Write("Hello C#\r\n");
            Console.Write(x + "\r\n");
            Console.Write(y + "\r\n");
            Console.Write(instance.x + "\r\n");

            var breads = ToastBreadAsync(3);
            var eggs = FryEggAsync(3);

            var resultTasks = new List<Task> { breads, eggs };
            int countTaskRem = resultTasks.Count;
            while(countTaskRem > 0)
            {
                Task finishedTask = await Task.WhenAny(resultTasks);
                if ( finishedTask == breads)
                {
                    Console.Write("Bread task is done...\r\n");
                    countTaskRem--;
                }
                else if ( finishedTask == eggs)
                {
                    Console.Write("Egg task is done...\r\n");
                    countTaskRem--;
                }
            }

            Console.Write("Done...");
        }

        private static async Task<Bread> ToastBreadAsync(int slices)
        {
            for (int slice = 0; slice < slices; slice++)
            {
                Console.WriteLine($"Putting {slice+1}th slice of bread in the toaster");
            }
            Console.WriteLine("Start toasting bread...");
            await Task.Delay(500);
            Console.WriteLine("Remove toast from toaster");
            return new Bread();
        }
        private static async Task<Egg> FryEggAsync(int eggs)
        {
            for (int num = 0; num < eggs; num++)
            {
                Console.WriteLine($"Break Egg{num} in the fan.");
            }
            Console.WriteLine("Start frying eggs...");
            await Task.Delay(1000);
            Console.WriteLine("Remove eggs from the fan");
            return new Egg();
        }

        class Bread
        {
            string name = "Bread";
        }

        class Egg
        {
            string name = "Egg";
        }
    }

    class Mine
    {
        public int x = 0;
    }
}
