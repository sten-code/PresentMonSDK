# PresentMon SDK

This is a C++ library that provides an interface to the PresentMon event tracing library. It is intended to be used by applications that need to capture and analyze the output of the PresentMon tool.

## Dependencies
- [MSVC](https://visualstudio.microsoft.com/downloads/)
  - Use the `Developer Powershell for VS 20XX` as terminal when compiling.
- [vcpkg](https://learn.microsoft.com/en-us/vcpkg/get_started/get-started?pivots=shell-powershell)
- [CMake](https://cmake.org/download/)
- [Ninja](https://ninja-build.org/)
    - You can install Ninja using [chocolatey](https://chocolatey.org/) by running `choco install ninja`.

## Building from Source

```bash
cmake -DCMAKE_BUILD_TYPE=Release --preset=vcpkg -G Ninja -S . -B build
cmake --build build
```

## Usage

I recommend looking in the [Sample Client](https://github.com/GameTechDev/PresentMon/tree/v2.3.0/IntelPresentMon/SampleClient) provided by the original project. 

Here is an example of how to use the library to track a process and query its metrics.

```c++
#include <iostream>

#include <PresentMonAPIWrapper/FixedQuery.h>
#include <PresentMonAPIWrapper/PresentMonAPIWrapper.h>

int main() {
    using namespace std::chrono_literals;
    using namespace pmapi;

    unsigned int processId = 20112;
    double winSizeMs = 2000.;
    double metricOffsetMs = 1000.;

    Session session;
    ProcessTracker proc = session.TrackProcess(processId);
    PM_BEGIN_FIXED_DYNAMIC_QUERY(MyDynamicQuery)
        FixedQueryElement appName{this, PM_METRIC_APPLICATION, PM_STAT_NONE};
        FixedQueryElement fpsAvg{this, PM_METRIC_PRESENTED_FPS, PM_STAT_AVG};
        FixedQueryElement fps90{this, PM_METRIC_PRESENTED_FPS, PM_STAT_PERCENTILE_90};
        FixedQueryElement fps95{this, PM_METRIC_PRESENTED_FPS, PM_STAT_PERCENTILE_95};
        FixedQueryElement fps99{this, PM_METRIC_PRESENTED_FPS, PM_STAT_PERCENTILE_99};
        FixedQueryElement fpsMax{this, PM_METRIC_PRESENTED_FPS, PM_STAT_MAX};
        FixedQueryElement fpsMin{this, PM_METRIC_PRESENTED_FPS, PM_STAT_MIN};
        FixedQueryElement frameDurationAvg{this, PM_METRIC_CPU_FRAME_TIME, PM_STAT_AVG};
        FixedQueryElement fpStallAvg{this, PM_METRIC_CPU_WAIT, PM_STAT_AVG};
        FixedQueryElement gpuDurationAvg{this, PM_METRIC_GPU_TIME, PM_STAT_AVG};
        FixedQueryElement gpuBusyTimeAvg{this, PM_METRIC_GPU_BUSY, PM_STAT_AVG};
        FixedQueryElement gpuDisplayLatencyAvg{this, PM_METRIC_DISPLAY_LATENCY, PM_STAT_AVG};
        FixedQueryElement gpuDisplayDurationAvg{this, PM_METRIC_DISPLAYED_TIME, PM_STAT_AVG};
        FixedQueryElement gpuPhotonLatencyAvg{this, PM_METRIC_CLICK_TO_PHOTON_LATENCY, PM_STAT_NON_ZERO_AVG};
        FixedQueryElement gpuPower{this, PM_METRIC_GPU_POWER, PM_STAT_AVG, 1};
        FixedQueryElement presentMode{this, PM_METRIC_PRESENT_MODE, PM_STAT_MID_POINT};
        FixedQueryElement gpuName{this, PM_METRIC_GPU_NAME, PM_STAT_NONE, 1};
        FixedQueryElement fanSpeed{this, PM_METRIC_GPU_FAN_SPEED, PM_STAT_AVG, 1};
    PM_END_FIXED_QUERY dq{session, winSizeMs, metricOffsetMs, 1, 1};

    while (true) {
        dq.Poll(proc);
        std::cout << "App Name = " << dq.appName.As<std::string>().c_str() << std::endl;
        std::cout << "Presented FPS Average = " << dq.fpsAvg.As<double>() << std::endl;
        std::cout << "Presented FPS 90% = " << dq.fps90.As<double>() << std::endl;
        std::cout << "Presented FPS 95% = " << dq.fps95.As<double>() << std::endl;
        std::cout << "Presented FPS 99% = " << dq.fps99.As<double>() << std::endl;
        std::cout << "Presented FPS Max = " << dq.fpsMax.As<double>() << std::endl;
        std::cout << "Presented FPS Min = " << dq.fpsMin.As<double>() << std::endl;
        std::cout << "Frame Duration Average = " << dq.frameDurationAvg.As<double>() << std::endl;
        std::cout << "Frame Pacing Stall Average = " << dq.fpStallAvg.As<double>() << std::endl;
        std::cout << "GPU Duration Average = " << dq.gpuDisplayDurationAvg.As<double>() << std::endl;
        std::cout << "GPU Busy Time Average = " << dq.gpuBusyTimeAvg.As<double>() << std::endl;
        std::cout << "Display Latency Average = " << dq.gpuDisplayLatencyAvg.As<double>() << std::endl;
        std::cout << "Display Duration Average = " << dq.gpuDisplayDurationAvg.As<double>() << std::endl;
        std::cout << "Input Latency Average = " << dq.gpuPhotonLatencyAvg.As<double>() << std::endl;
        std::cout << "GPU Power Average = "<< dq.gpuPower.As<double>() << std::endl;
        std::cout << "Present Mode = " << dq.presentMode.As<std::string>().c_str() << std::endl;
        std::cout << "GPU Name = " << dq.gpuName.As<std::string>().c_str() << std::endl;
        std::this_thread::sleep_for(20ms);
    }

    return 0;
}

```