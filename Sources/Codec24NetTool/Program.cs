using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;

namespace Codec24NetTool
{
	class Program
	{
		static void Main(string[] args)
		{
			if (args.Length == 3)
			{
				string command = args[0];

				if (command == "encode")
					Encode(args[1], args[2]);
				else if (command == "decode")
					Decode(args[1], args[2]);
				else
					ShowHelp();
			}
			else
			{
				ShowHelp();
			}
		}

		private static void ShowHelp()
		{
			Console.WriteLine("usage: Codec24NetTool.exe <encode | decode> <input file> <output file>");
		}

		private static void Encode(string inputFileName, string outputFileName)
		{
			if (string.IsNullOrEmpty(inputFileName)) throw new ArgumentNullException("inputFileName");
			if (string.IsNullOrEmpty(outputFileName)) throw new ArgumentNullException("outputFileName");

			using (var inputFile = File.OpenRead(inputFileName))
			using (var outputFile = File.Create(outputFileName))
			using (var reader = new BinaryReader(inputFile))
			using (var writer = new BinaryWriter(outputFile))
			{
				var codec = new Codec24Net.Codec2();
				codec.Encode(reader, writer);
			}
		}

		private static void Decode(string inputFileName, string outputFileName)
		{
			if (string.IsNullOrEmpty(inputFileName)) throw new ArgumentNullException("inputFileName");
			if (string.IsNullOrEmpty(outputFileName)) throw new ArgumentNullException("outputFileName");

			using (var inputFile = File.OpenRead(inputFileName))
			using (var outputFile = File.Create(outputFileName))
			using (var reader = new BinaryReader(inputFile))
			using (var writer = new BinaryWriter(outputFile))
			{
				var codec = new Codec24Net.Codec2();
				codec.Decode(reader, writer);
			}
		}
	}
}
