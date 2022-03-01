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
        static void Main(string[] args)
        {
            Mine instance = new Mine();
            instance.x = 3;

            int x = 0;
            int y = 1;

            Console.Write("Hello C#\r\n");
            Console.Write(x + "\r\n");
            Console.Write(y + "\r\n");
            Console.Write(instance.x);
        }
    }

    class Mine
    {
        public int x = 0;
    }
}
