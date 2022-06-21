using System;

namespace PracticeCsharp
{
    public static class Extentions
    {
        public static int findIndex<T>(this T[] array, T item)
        {
            return Array.IndexOf(array, item);
        }
    }
    class Fundamental
    {
        public void Test()
        {
            
        }
        public void TestString()
        {
            string[] test =
            {
                "Seoul",
                "Hongkong",
                "Newyork",
                "Calgary"
            };

            int i = test.findIndex("Hongkong");
        }
    }
}