#include "stdafx.h"
#include "codec2.h"

using namespace System;
using namespace System::IO;

namespace Codec24Net {

	public ref class Codec2
	{
	struct CODEC2 *codecState;
	int nsam;
	int nbit;

	public:		
		void create(int mode)
		{
			codecState = codec2_create(mode);
			nsam = codec2_samples_per_frame(codecState);
			nbit = codec2_bits_per_frame(codecState);
		}
		
		int get_codec2_samples_per_frame(void)
		{
			return codec2_samples_per_frame(codecState);
		}
		
		int get_codec2_bits_per_frame(void)
		{
			return codec2_bits_per_frame(codecState);
		}

		void destroy(void)
		{
			codec2_destroy(codecState);
		}

		void Encode(BinaryReader^ input, BinaryWriter^ output)
		{
			if (input == nullptr) throw gcnew ArgumentNullException(L"input");
			if (output == nullptr) throw gcnew ArgumentNullException(L"output");

//			void* codecState = codec2_create();

			array<unsigned char>^ pcmByteBuffer = gcnew array<unsigned char>(nsam * 2);
			short pcmSamples[320];

			static const int encodedSampleCount = nbit/8;			
			unsigned char encodedSamples[8];
			array<unsigned char>^ encodedByteBuffer = gcnew array<unsigned char>(encodedSampleCount);
			
			int readBytes;
			do
			{
				readBytes = input->Read(pcmByteBuffer, 0, nsam * 2);
				if (readBytes > 0)
				{
					for (int i = 0; i < nsam; i++)
					{
						if (i <= readBytes)
						{
							pcmSamples[i] = (pcmByteBuffer[i * 2 + 1] << 8) + pcmByteBuffer[i * 2];
						}
						else
						{
							pcmSamples[i] = 0;
						}
					}

					codec2_encode(codecState, encodedSamples, pcmSamples);

					for (int i = 0; i < encodedSampleCount; i++)
					{
						encodedByteBuffer[i] = encodedSamples[i];
					}

					output->Write(encodedByteBuffer, 0, encodedSampleCount);
				}
			} while (readBytes > 0);

	//		codec2_destroy(codecState);
		}
				
		void Decode(BinaryReader^ input, BinaryWriter^ output)
		{
			if (input == nullptr) throw gcnew ArgumentNullException(L"input");
			if (output == nullptr) throw gcnew ArgumentNullException(L"output");

	//		void* codecState = codec2_create();

			array<unsigned char>^ pcmByteBuffer = gcnew array<unsigned char>(320 * 2);
			short pcmSamples[320];

			static const int encodedSampleCount = 8;			
			unsigned char encodedSamples[encodedSampleCount];
			array<unsigned char>^ encodedByteBuffer = gcnew array<unsigned char>(encodedSampleCount);

			while(input->Read(encodedByteBuffer, 0, encodedSampleCount) > 0)
			{
				for (int i = 0; i < encodedSampleCount; i++)
				{
					encodedSamples[i] = encodedByteBuffer[i];
				}

				codec2_decode(codecState, pcmSamples, encodedSamples);

				for (int i = 0; i < 320; i++)
				{
					pcmByteBuffer[i * 2 + 1] = pcmSamples[i] >> 8;
					pcmByteBuffer[i * 2] = pcmSamples[i] & 0xFF; 
				}

				output->Write(pcmByteBuffer, 0, 320 * 2);
			}

	//		codec2_destroy(codecState);
		}
	};
}
