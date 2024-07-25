#include <iostream>
#include <windows.h>
#include <vector>
#include "checks/utils/utils.h"
#include "checks/services/servicesRequest.h"
#include "checks/temp/TEMPRequest.h"
#include "checks/prefetch/PrefetcherRequest.h"
#include "checks/bam/BAMRequest.h"
#include "checks/disk/DISKRequest.h"
#include "checks/block/SystemBlockRegeditRequest.h"
#include "checks/recycle/RecycleRequest.h"
#include "checks/pca/PCA11Request.h"
#include "checks/stenographer/StenographyRequest.h"
#include "checks/prefetch/PrefetcherRegeditRequest.h"
#include "checks/host/HOSTRequest.h"
#include "checks/task/ScheduleRequest.h"
#include "checks/memory/DIAGTRACKRequest.h"
#include "checks/recording/RecordingRequest.h"
#include "checks/inetcache/PhoneRequest.h"
#include "checks/macros/MacrosRequest.h"
#include "checks/usnjournal/USNJournal.h"
#include "checks/muicache/MUICacheRequest.h"
#include "checks/ntfs/NTFSRequest.h"  // Incluir el archivo de encabezado de NTFSRequest

void setConsoleTextColor(WORD color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

int main() {
    RecordingRequest recordingRequest;
    if (recordingRequest.isRecording()) {
        setConsoleTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        std::cout << "[#] Warning: Recording software detected!" << std::endl;
    }
    else {
        setConsoleTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        std::cout << "[#] No recording software detected." << std::endl;
    }

    std::vector<std::string> services = {
        "PCASVC", "DPS", "SYSMAIN", "EVENTLOG", "BAM", "APPINFO",
        "DUSMSVC", "DIAGTRACK", "DCOMLAUNCH", "BFE",
        "DNSCACHE", "WSEARCH", "SCHEDULE", "STORSVC", "SGRMBROKER"
    };
    checkServicesStatus(services);

    MacrosRequest macrosRequest;
    macrosRequest.analyzeFolders();

    MUICacheRequest muiCacheRequest;
    if (muiCacheRequest.EnumerateMuiCacheEntries()) {
        muiCacheRequest.DisplayFriendlyOnlyEntries();
    }

    TEMPRequest tempRequest;
    tempRequest.ScanDirectories();

    PhoneRequest phoneRequest;
    phoneRequest.checkINetCacheDirectory();

    NTFSRequest ntfsRequest;
    if (ntfsRequest.Initialize()) {
        ntfsRequest.AnalyzeNTFSEvents();
    }
    else {
        setConsoleTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        std::cerr << "[!] Failed to initialize NTFSRequest." << std::endl;
        ResetConsoleColor();
    }

    USNJournal usnJournal;
    usnJournal.AnalyzeEvents();

    PrefetcherRegeditRequest prefetcherRegeditRequest;
    prefetcherRegeditRequest.AnalyzePrefetcherRegedit();

    PrefetcherRequest prefetcherRequest;
    prefetcherRequest.ScanPrefetch();

    BAMRequest bamRequest;
    bamRequest.analyzeBAM();

    DISKRequest diskRequest;
    diskRequest.analyzeDisks();

    SystemBlockRegeditRequest regeditRequest;
    regeditRequest.checkRegistryKeys();

    RecycleRequest recycleRequest;
    recycleRequest.AnalyzeRecycleBin();

    PCA11Request pca11Request;
    pca11Request.AnalyzePCA11();

    StenographyRequest stenographyRequest;
    stenographyRequest.ScanRegistry();

    HOSTRequest hostRequest;
    hostRequest.AnalyzeHosts();

    ScheduleRequest scheduleRequest;
    scheduleRequest.ScanScheduledTasks();

    DIAGTRACKRequest diagtrackRequest;
    diagtrackRequest.AnalyzeDiagtrack();

    setConsoleTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    std::cout << "-------------------Request Tool/NotDiff/-------------------" << std::endl;

    while (true) {
        Sleep(1000);
    }

    return 0;
}
