#include <iostream>
#include "aquila/global.h"
#include "aquila/source/generator/SineGenerator.h"
#include "aquila/transform/Mfcc.h"
#include "aquila/source/WaveFile.h"
#include "aquila/transform/FftFactory.h"
#include "aquila/tools/TextPlot.h"
#include "aquila\ml\Dtw.h"
#include <fstream>

const int SIZE = 64;
using namespace std;
double calculateRMS(std::vector<double> values);
Aquila::SpectrumType plotFourierTransform(Aquila::WaveFile wav)
{
	std::ofstream out("Fourier.txt", ios::app);
	out << endl << endl;
	Aquila::TextPlot plt("Fast Fourier Transform");
	auto fft = Aquila::FftFactory::getFft(SIZE);
	Aquila::SpectrumType spectrum = fft->fft(wav.toArray());
	for (int i = 0; i < spectrum.size(); i++)
		out << spectrum[i] <<std::endl;
	//plt.plotSpectrum(spectrum);
	spectrum.shrink_to_fit();
	return spectrum;
}
std::vector<double> displayFeatureVectors(Aquila::WaveFile wav,string owner)
{
	ofstream out("Features.txt",ios::app);
	out << endl << endl;
	out << owner<<endl;
	Aquila::TextPlot plt("Feature Vector");
	Aquila::Mfcc mfcc(SIZE);
	auto Values = mfcc.calculate(wav);
	for (int i = 0; i < Values.size(); i++)
		out<< Values[i] << std::endl;
	out << "RMS " << calculateRMS(Values);
	Values.shrink_to_fit();
	return Values;
}
double calculateRMS(std::vector<double> values)
{
	//TODO: Check for calculation accuracy
	int i;
	double total = 0;
	for ( i = 0; i < values.size(); i++)
	{
		total += (values[i]*values[i]);
	}
	return sqrt(total)/i;
}
int main()
{
	ifstream sound("sound/sound.txt");
	string file,owner;
	int i = 0, b=0,c=0;
	vector<vector<double>> features;
	vector<vector<double>> features2;
	auto it = features.begin();
	while (sound>>file)
	{	
		if (i < 1)
		{
			sound >> owner;
			try
			{
				Aquila::WaveFile wav("sound/" + file);
				plotFourierTransform(wav);
				features.insert(features.begin() + b, displayFeatureVectors(wav, owner + " " + file));
				b++;
			}
			catch (std::bad_alloc &a)
			{
				std::cout << a.what() << endl;
			}
		}
		else
		{
			sound >> owner;
			try
			{
				Aquila::WaveFile wav("sound/" + file);
				plotFourierTransform(wav);
				features2.insert(features2.begin() + c, displayFeatureVectors(wav, owner + " " + file));
				c++;
			}
			catch (std::bad_alloc &a)
			{
				std::cout << a.what() << endl;
			}
		}

		i++;

	}
	Aquila::Dtw dynamic;

	cout <<" distance" << dynamic.getDistance(features,features2)<<endl;

	//std::cout << "RMS " <<  << std::endl;
	
	system("pause");
	return 0;
}