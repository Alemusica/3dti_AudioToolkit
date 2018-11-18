/**
* \class CGammatoneFilter
*
* \brief Declaration of CGammatoneFilter class interface.
* \date	July 2016
*
* \authors 3DI-DIANA Research Group (University of Malaga), in alphabetical order: M. Cuevas-Rodriguez, C. Garre,  D. Gonzalez-Toledo, E.J. de la Rubia-Cuestas, L. Molina-Tanco ||
* Coordinated by , A. Reyes-Lecuona (University of Malaga) and L.Picinali (Imperial College London) ||
* \b Contact: areyes@uma.es and l.picinali@imperial.ac.uk
*
* \b Contributions: (additional authors/contributors can be added here)
* \b The gammatone filter was implemented by Michael Krzyzaniak: m.krzyzaniak@surrey.ac.uk
*
* \b Project: 3DTI (3D-games for TUNing and lEarnINg about hearing aids) ||
* \b Website: http://3d-tune-in.eu/
*
* \b Copyright: University of Malaga and Imperial College London - 2018
*
* \b Licence: This copy of 3dti_AudioToolkit is licensed to you under the terms described in the 3DTI_AUDIOTOOLKIT_LICENSE file included in this distribution.
*
* \b Acknowledgement: This project has received funding from the European Union's Horizon 2020 research and innovation programme under grant agreement No 644051
*/

#ifndef _CGAMMATONEFILTER_H_
#define _CGAMMATONEFILTER_H_

#include <Common/Buffer.h>
#include "CommonDefinitions.h"

namespace Common {

	/** \details This class implements a gammatone filter
	\n using the algorithm described in this paper:
	\n Implementing a GammaTone Filter Bank
	\n Annex C of the SVOS Final Report (Part A: The Auditory Filter Bank)
	\n John Holdsworth, Ian Nimmo-Smith, Roy Patterson, Peter Rice
	\n 26th February 1988
	\n  https://www.pdn.cam.ac.uk/other-pages/cnbh/files/publications/SVOSAnnexC1988.pdf
	*/
	class CGammatoneFilter
	{
	public:
		///////////////////
		// PUBLIC METHODS
		//////////////////

		/** \brief Default constructor.
		*	\details By default, sets sampling frequency to 44100Hz.
		*  \param [in] _order -- the filter order. This cannot be changed after the filter is constructed. The typical value is 4.
		*   \eh On error, an error code is reported to the error handler.
		*/
		CGammatoneFilter(unsigned _order, float _centerFrequency);

		/** \brief Filter the input data according to the filter setup.
		*	\param [in] inBuffer input buffer
		*	\param [out] outBuffer output buffer
		*	\param [in] addResult when true, samples resulting from the	filtering process are added to the current value of the output buffer.
		*	\pre Input and output buffers must have the same size, which should be greater than 0.
		*   \eh On error, an error code is reported to the error handler.
		*/
		//void Process(CMonoBuffer<float> &inBuffer, CMonoBuffer<float> & outBuffer, bool addResult = false);

		/**
		\overload
		*/
		void Process(CMonoBuffer<float> &buffer);

		/** \brief Set the sampling frequency at which audio samples were acquired
		*  \param [in] _samplingFreq sampling frequency, in Hertzs
		*   \eh On success, RESULT_OK is reported to the error handler.
		*       On error, an error code is reported to the error handler.
		*/
		void SetSamplingFreq(float _samplingFreq);

		/** \brief Get the sample rate, in Hz, of the filter
		*  \retval freq filter sampling frequency
		*   \eh Nothing is reported to the error handler.
		*/
		float GetSamplingFreq();
	
		/** \brief Set the gain of the filter 
		*	\param [in] _gain filter gain 
		*   \eh Nothing is reported to the error handler.
		*/
		void SetGeneralGain(float _gain);

		/** \brief Get the gain of the filter
		*	\retval gain filter gain
		*   \eh Nothing is reported to the error handler.
		*/
		float GetGeneralGain();

		/** \brief Get the order of the filter
		*  \retval order filter order
		*   \eh Nothing is reported to the error handler.
		*/
		unsigned GetOrder();

		/** \brief Set the bandwidth of the filter, specified as the width between the 3dB cutoff points, and retaining the current center frequency
		*  \param [in] _bw 3dB bandwidth
		*   \eh Nothing is reported to the error handler.
		*/
		void Set3dBBandwidth(float _bw);

		/** \brief Get the bandwidth of the filter, specified as the width between the 3dB cutoff points
		*  \retval bw filter bandwidth (3db cutoff)
		*   \eh Nothing is reported to the error handler.
		*/
		float Get3dBBandwidth();

		/** \brief Set the bandwidth of the filter, specified as the equivalent rectangular bandwidth, and retaining the current center frequency
		*  \param [in] _freq center frequency
		*   \eh Nothing is reported to the error handler.
		*/
		void SetERBBandwidth(float _erb);

		/** \brief Get the bandwidth of the filter, specified as the equivalent rectangular bandwidth,
		*  \retval erb filter bandwidth (3db cutoff)
		*   \eh Nothing is reported to the error handler.
		*/
		float GetERBBandwidth();
	
		/** \brief Set the center frequency of the filter, retaining the current bandwidth
		*  \param [in] _freq center frequency
		*   \eh Nothing is reported to the error handler.
		*/
		void SetCenterFrequency(float _freq);

		/** \brief Get the center frequency of the filter
		*  \retval freq filter center freq
		*   \eh Nothing is reported to the error handler.
		*/
		float GetCenterFrequency();
	
		/** \brief Set the center frequency of the filter, and adjust the bandwidth to match the Equivalent Rectangular Bandwidth (ERB) of the human auditory filter for the given frequency. This is a convience method that calls SetCenterFrequency, GetERBOfHumanAudirotyFilter, and SetERBBandwidth.
		*  \param [in] _freq center frequency
		*   \eh Nothing is reported to the error handler.
		*/
		void SetFrequencyUsingERBOfHumanAuditoryFilter(float _freq);

		/** \brief Helper function (class method) to calculate the ERB of the human auditory filter for a given frequecy
		*  \param [in] _freq center frequency
		*  \retval erb equivalent rectangular bandwidth (ERB) for the given center frequency
		*   \eh Nothing is reported to the error handler.
		*/
		static float GetERBOfHumanAuditoryFilter(float _freq);



	private:
		////////////////////
		// PRIVATE METHODS
		///////////////////
		static double CalculateAn(unsigned _order);
		static double CalculateCn(unsigned _order);
		static unsigned Factorial(unsigned x);
	
		////////////////
		// ATTRIBUTES
		////////////////
		float generalGain;                                              // Gain applied to every sample obtained with Process

		double samplingFreq;                                            // Keep the sampling rate at which audio samples were taken
		unsigned order;                                                 // Keep the filter order
		double b;                                                       // scale param of gamma distribution
		double an;
		double cn;
		double f0;                                                      // center freq in Hz (also freq of impulse response tone)
		double phase;                                                   // phase of the filter
		float* prev_z_real;                                             // store previous samples between audio buffers
		float* prev_z_imag;
		float* prev_w_real;
		float* prev_w_imag;
	};
}
#endif
