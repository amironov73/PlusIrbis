using static System.Console;

static class Program
{
    static void Main(string[] args)
    {
        foreach (var arg in args)
        {
            ProcessString(arg);
        }
    }

    private static void ProcessString(string s)
    {
        foreach (var c in s)
        {
            if (c < 256)
            {
                Write(c);
            }
            else
            {
                Write("\\u{0:X4}", (int)c);
            }
        }
        WriteLine();
    }
}
