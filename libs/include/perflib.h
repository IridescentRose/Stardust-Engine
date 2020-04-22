#pragma once
#include <stdbool.h>

#ifdef __cplusplus  
extern "C" {
#endif 

	/**
	* This method clears the internal timers and mechanisms of PFL
	* @param frameRelative - setting this to true means that you compare percentage of GPU vs CPU. This is useful for finding bottlenecks.
	*/
	void PFL_Init(bool frameRelative);

	/** 
	* This method begins a record of when the CPU has begun doing tasks. This should be put immediately at the beginning of your game loop.
	*/
	void PFL_BeginCPURecord();

	/** 
	* This method ends a record of when the CPU has finished doing tasks. This should be put before the final rendering call of your game loop.
	*/
	void PFL_EndCPURecord();

	/** 
	* This method extracts the CPU time taken between the last Start and End of the CPU performance recording (in milliseconds).
	*/
	double PFL_GetCPUTime();

	/**
	* This method helps in calculation of the CPU and GPU percentages by setting a targeted framerate. Percentages above 100 mean that the CPU and or GPU is taking longer than expected. This only matters in non-full profiling mode.
	*/
	void PFL_SetTargetFrameRate(int framerate);

	/** 
	* Returns the current frame rate target. This only matters in non-full profiling mode.
	*/
	int PFL_GetTargetFrameRate();

	/**
	* This method extracts the CPU time taken between the last Start and End of the CPU performance recording.
	*/
	double PFL_GetCPUPercentage();

	/**
	* This method begins a record of when the GPU begins doing tasks. This should be put immediately after sceGuFinish() and before sceGuSync(0, 0)
	*/
	void PFL_BeginGPURecord();
	
	/**
	* This method ends a record of when the GPU has finished doing tasks. This should be put immediately after sceGuSync(0, 0)
	*/
	void PFL_EndGPURecord();

	/**
	* This method begins a record of when the system is waiting for a VBlank. This should be put immediately before sceDisplayWaitVblankStart() and swap buffers.
	*/
	void PFL_BeginVBLRecord();

	/**
	* This method begins a record of when the system has finished a VBlank. This should be put immediately after sceGuSwapBuffers().
	*/
	void PFL_EndVBLRecord();

	/**
	* This method extracts the total GPU time taken.
	*/
	double PFL_GetGPUTime();

	/**
	* This method extracts the total GPU time as a percentage.
	*/
	double PFL_GetGPUPercentage();

	/**
	* This method extracts the total VBL time taken.
	*/
	double PFL_GetVBLTime();


#ifdef __cplusplus  
}
#endif 