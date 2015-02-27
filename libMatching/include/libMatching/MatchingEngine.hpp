/* A class providing easy to use functions for common tasks with FaceVACS matching
*
* NOTE: uses C++ 11 constructs, configure g++ compiler to support this
* by appending "-std = c++0x" or c++11 to compiler flags
*
* @author Michael Grupp
*/

#ifdef WIN32
	#define BOOST_ALL_DYN_LINK // Link against the dynamic boost lib. Seems to be necessary because we use /MD, i.e. link to the dynamic CRT.
	#define BOOST_ALL_NO_LIB // Don't use the automatic library linking by boost with VS2010 (#pragma ...). Instead, we specify everything in cmake.
#endif

#include <iostream>
#include <frsdk/config.h>
#include <frsdk/match.h>
#include <boost/filesystem.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

//custom libs
#include "libLogging/LoggerFactory.hpp"

using namespace std;
using namespace logging;
using logging::Logger;
using logging::LoggerFactory;
using logging::LogLevel;
using namespace boost::filesystem;


class MatchingEngine {
private:
	std::shared_ptr<FRsdk::Configuration> cfg;
	std::unique_ptr<FRsdk::FIRBuilder> firBuilder;
	std::unique_ptr<FRsdk::FacialMatchingEngine> me;
protected:

public:
	/* constructor
	 * @param path to your FaceVACS SDK config file (frsdk.cfg)
	 * @param path of the logfile that is generated by this class
	*/
	MatchingEngine(path configFile, path logfile);
	
	//destructor
	~MatchingEngine() {
		Loggers->getLogger("matching").info("closing MatchingEngine");
	}


	/* loads a fir that can be used for matching
	 * @param	the path to the .fir file
	*/
	FRsdk::FIR loadFIR(path fir);


	/* creates a fir-population that can be used for faster matching
	 * @param	a folder containing the .fir files
	*/
	FRsdk::Population createPopulation(path firFolder);


	/* matches two single FIR files
	 * @param	the two paths of the FIRs
	*/
	float matchPair(path probeFIRpath, path galleryFIRpath);

	
	/* matches a single FIR to a complete gallery (population)
	 * @param	the FIR of the probe and the galleryPopulation
	*/
	vector<float> matchToPopulation(FRsdk::FIR probe, const FRsdk::Population& galleryPopulation);


	/* matches two folders containing FIR files and generates a scorefile for each probe
	 * @param	the two paths containing FIRs and the folder for the scorefiles
	*/
    void matchExperiment(path probeFolder, path galleryFolder, path outputFolder);
};

