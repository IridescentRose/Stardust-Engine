#include <Profiler/Profiler.h>
#include <Utilities/Logger.h>
#include <algorithm>

namespace Stardust::Profiling {
	Profiler::Profiler(std::string name)
	{
		profilerName = name;
		methodData.clear();
		t.reset();
		subT.reset();
	}

	void Profiler::beginProfileMethod()
	{
		t.reset();
	}

	void Profiler::endProfileMethod(std::string methodName)
	{
		t.deltaTime();
		double time = t.elapsed() * 1000;
		t.reset();


		ProfileRun r;
		r.fromStart = Utilities::g_AppTimer.elapsed() - time;
		r.time = time;

		int methodID = findMethod(methodName);

		if (methodID != -1) {
			methodData[methodID].runs.push_back(r);
		}
		else {
			ProfileData newData;
			newData.method = methodName;
			newData.subProfiles.clear();
			newData.runs.clear();
			newData.runs.push_back(r);

			methodData.push_back(newData);
		}
	}

	void Profiler::beginProfileSubMethod()
	{
		subT.reset();
	}
	void Profiler::endProfileSubMethod(std::string macroMethod, std::string subMethod)
	{
		subT.deltaTime();
		double time = subT.elapsed() * 1000;
		subT.reset();


		ProfileRun r;
		r.fromStart = Utilities::g_AppTimer.elapsed() - time;
		r.time = time;

		int methodID = findMethod(macroMethod);

		if (methodID != -1) {
			int res = -1;
			for (int i = 0; i < methodData[methodID].subProfiles.size(); i++) {
				if (methodData[methodID].subProfiles[i].method == subMethod) {
					res = i;
					break;
				}
			}

			if (res != 1) {
				methodData[methodID].subProfiles[res].runs.push_back(r);
			}
			else {
				ProfileData subData;
				subData.method = subMethod;
				subData.subProfiles.clear();
				subData.runs.clear();
				subData.runs.push_back(r);

				methodData[methodID].subProfiles.push_back(subData);
			}

		}
		else {
			ProfileData newData;
			newData.method = macroMethod;
			newData.runs.clear();
			newData.subProfiles.clear();

			ProfileData subData;
			subData.method = subMethod;
			subData.subProfiles.clear();
			subData.runs.clear();
			subData.runs.push_back(r);

			newData.subProfiles.push_back(subData);

			methodData.push_back(newData);
		}
	}
	
	bool mysort(ProfileRun i, ProfileRun j) {
		return i.time < j.time;
	}

	void calculateData(ProfileData& data) {
		std::sort(data.runs.begin(), data.runs.end(), mysort);

		data.minTime = data.runs[0].time;
		data.maxTime = data.runs[0].time;
		data.avgTime = 0.0;

		for (int i = 0; i < data.runs.size(); i++) {
			data.avgTime += data.runs[i].time;

			if (data.runs[i].time > data.maxTime) {
				data.maxTime = data.runs[i].time;
			}
			if (data.runs[i].time < data.minTime) {
				data.minTime = data.runs[i].time;
			}

			if (i == (data.runs.size()) / 2) {
				data.medianTime = data.runs[i].time;
			}

		}

		data.avgTime /= (double)data.runs.size() + 1.0;
	}

	void writeMethodData(ProfileData data, std::ofstream& file, std::string tabs) {
		//CALCULATE AVERAGE DATA FOR PROFILE
		calculateData(data);

		file << std::endl << "[METHOD NAME]: " << data.method << std::endl;
		tabs += "\t";
		file << "[" << data.method << "]" << tabs << "NUM OF SAMPLES: " << data.runs.size() << " samples" << std::endl;
		file << "[" << data.method << "]" << tabs << "AVERAGE TIME: " << data.avgTime << "ms" << std::endl;
		file << "[" << data.method << "]" << tabs << "MINIMUM TIME: " << data.minTime << "ms" << std::endl;
		file << "[" << data.method << "]" << tabs << "MAXIMUM TIME: " << data.maxTime << "ms" << std::endl;
		file << "[" << data.method << "]" << tabs << "MEDIAN TIME: " << data.medianTime << "ms" << std::endl;


		if (data.subProfiles.size() > 0) {
			//SUB PROFILES
			tabs += "\t";

			for (int i = 0; i < data.subProfiles.size(); i++) {
				writeMethodData(data.subProfiles[i], file, tabs);
			}
		}
	}
	
	void Profiler::outputStats(std::string fileName)
	{
		std::ofstream fs(fileName, std::ios::trunc);

		fs << "[STARDUST ENGINE PROFILER]" << std::endl;
		fs << "[PROFILER NAME]: " << profilerName << std::endl;



		for (int i = 0; i < methodData.size(); i++) {
			writeMethodData(methodData[i], fs, "");
		}

		fs.close();
	}


	int Profiler::findMethod(std::string name)
	{
		int res = -1;

		for (int i = 0; i < methodData.size(); i++) {
			if (methodData[i].method == name) {
				res = i;
				break;
			}
		}

		return res;
	}
}