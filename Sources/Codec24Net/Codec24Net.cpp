#include "stdafx.h"
#include "codec2.h"

using namespace System;
using namespace System::IO;

namespace Codec24Net {

	public ref class Codec2
	{
	public:		

		void Encode(BinaryReader^ input, BinaryWriter^ output)
		{
			if (input == nullptr) throw gcnew ArgumentNullException(L"input");
			if (output == nullptr) throw gcnew ArgumentNullException(L"output");

			void* codecState = codec2_create();

			array<unsigned char>^ pcmByteBuffer = gcnew array<unsigned char>(CODEC2_SAMPLES_PER_FRAME * 2);
			short pcmSamples[CODEC2_SAMPLES_PER_FRAME];

			static const int encodedSampleCount = ((CODEC2_BITS_PER_FRAME + 7) / 8);			
			unsigned char encodedSamples[encodedSampleCount];
			array<unsigned char>^ encodedByteBuffer = gcnew array<unsigned char>(encodedSampleCount);

			int readBytes;
			do
			{
				readBytes = input->Read(pcmByteBuffer, 0, CODEC2_SAMPLES_PER_FRAME * 2);
				if (readBytes > 0)
				{
					for (int i = 0; i < CODEC2_SAMPLES_PER_FRAME; i++)
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

			codec2_destroy(codecState);
		}
				
		void Decode(BinaryReader^ input, BinaryWriter^ output)
		{
			if (input == nullptr) throw gcnew ArgumentNullException(L"input");
			if (output == nullptr) throw gcnew ArgumentNullException(L"output");

			void* codecState = codec2_create();

			array<unsigned char>^ pcmByteBuffer = gcnew array<unsigned char>(CODEC2_SAMPLES_PER_FRAME * 2);
			short pcmSamples[CODEC2_SAMPLES_PER_FRAME];

			static const int encodedSampleCount = ((CODEC2_BITS_PER_FRAME + 7) / 8);			
			unsigned char encodedSamples[encodedSampleCount];
			array<unsigned char>^ encodedByteBuffer = gcnew array<unsigned char>(encodedSampleCount);

			while(input->Read(encodedByteBuffer, 0, encodedSampleCount) > 0)
			{
				for (int i = 0; i < encodedSampleCount; i++)
				{
					encodedSamples[i] = encodedByteBuffer[i];
				}

				codec2_decode(codecState, pcmSamples, encodedSamples);

				for (int i = 0; i < CODEC2_SAMPLES_PER_FRAME; i++)
				{
					pcmByteBuffer[i * 2 + 1] = pcmSamples[i] >> 8;
					pcmByteBuffer[i * 2] = pcmSamples[i] & 0xFF; 
				}

				output->Write(pcmByteBuffer, 0, CODEC2_SAMPLES_PER_FRAME * 2);
			}

			codec2_destroy(codecState);
		}
	};
}
