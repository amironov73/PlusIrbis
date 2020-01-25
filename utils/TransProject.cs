// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#region Using directives

using System;
using System.Collections.Generic;
using System.IO;

#endregion

static class TransProject
{
    private const string BeginText = "<!-- BEGIN -->";
    private const string EndText = "<!-- END -->";

    private static string _sourceName, _targetName;

    static int _FindLine(string[] array, string textToFind)
    {
        for (var i = 0; i < array.Length; i++)
        {
            if (array[i].Contains(textToFind))
            {
                return i;
            }
        }

        return -1;
    }

    static string[] _ExtractLines(string[] array, int start, int end)
    {
        var result = new List<string>();
        for (var i = start; i <= end; i++)
        {
            result.Add(array[i]);
        }

        return result.ToArray();
    }

    static string[] _TransplantLines(string[] source, string[] target, int start, int end)
    {
        var result = new List<string>();
        for (var i = 0; i < start; i++)
        {
            result.Add(target[i]);
        }

        foreach (var line in source)
        {
            result.Add(line);
        }

        for (var i = end + 1; i < target.Length; i++)
        {
            result.Add(target[i]);
        }

        return result.ToArray();
    }

    static int Main(string[] args)
    {
        if (args.Length != 2)
        {
            Console.WriteLine("TransProject <source.vcxproj> <target.vcxproj>");

            return -1;
        }

        _sourceName = args[0];
        _targetName = args[1];

        try
        {
            // Read donor file

            var sourceLines = File.ReadAllLines(_sourceName);
            var startLine = _FindLine(sourceLines, BeginText);
            if (startLine < 0)
            {
                Console.WriteLine("Bad source file");
                return -1;
            }

            var endLine = _FindLine(sourceLines, EndText);
            if (endLine < 0)
            {
                Console.WriteLine("Bad source file");
                return -1;
            }

            if (endLine < startLine)
            {
                Console.WriteLine("Bad source file");
                return -1;
            }

            sourceLines = _ExtractLines(sourceLines, startLine, endLine);

            // Read acceptor file

            var targetLines = File.ReadAllLines(_targetName);
            startLine = _FindLine(targetLines, BeginText);
            if (startLine < 0)
            {
                Console.WriteLine("Bad target file");
                return -1;
            }

            endLine = _FindLine(targetLines, EndText);
            if (endLine < 0)
            {
                Console.WriteLine("Bad target file");
                return -1;
            }

            if (endLine < startLine)
            {
                Console.WriteLine("Bad target file");
                return -1;
            }

            // Transplant the lines.

            targetLines = _TransplantLines(sourceLines, targetLines, startLine, endLine);
            File.WriteAllLines(_targetName, targetLines);

        }
        catch (Exception exception)
        {
            Console.WriteLine(exception);
            return -1;
        }

        return 0;
    }
}
