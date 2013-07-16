
#pragma once

#include "../framework/graphicslib.h"
#include "../framework/memory.h"
#include "../framework/event.h"
#include <list>
#include <vector>
#include <string>
#include <curl/curl.h>

#ifndef Framework
class Framework;
#endif // Framework

class DownloadInformation
{
  public:
    std::string* URL;
    unsigned long TotalDLSize;
    unsigned long CurrentDLSize;

    DownloadInformation( std::string* SourceURL )
    {
      URL = SourceURL;
      TotalDLSize = 0;
      CurrentDLSize = 0;
    };
};

class DownloadManager
{
	private:
	  Framework* systemFramework;

	public:
	  static bool AbortDownloads;

		std::list<std::string*> urlList;
		std::vector<DownloadInformation*> urlDownloading;
		int urlDownloads;

		DownloadManager( Framework* FrameworkObject, int AllowedConcurrents );
		~DownloadManager();

		void RequestURL( std::string* URL );
		void Event( FwEvent* e );
		void Update();

		void Tidy();

};
