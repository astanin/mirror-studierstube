#ifndef FILE_CONFIGURATION_THREAD_HH
#define FILE_CONFIGURATION_THREAD_HH
#include "../core/ConfigurationThread.h"
#include <OpenTracker/misc/xml/XMLWriter.h>
#include <ace/Thread.h>
#include <string>
#include <fstream>



#ifdef __WIN32__
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#   undef WIN32_LEAN_AND_MEAN
#   define DIR_HDL HANDLE
#   define FILE_TIME FILETIME
#   define CLOSEDIR(blah) FindClose(blah)
#   define COMPARE_FTIME(later, earlier) CompareFileTime(&later, &earlier)
#   define SEP "\\"
#else
#   include <dirent.h>
#   define DIR_HDL  DIR*
#   define FILE_TIME time_t
#   define CLOSEDIR(blah) closedir(blah)
#   define COMPARE_FTIME(later, earlier) later > earlier
#   define SEP "/"
#endif //__WIN32__


namespace ot{
 
    class FileConfigurationThread: public ConfigurationThread{

    protected :
            
        Configurator * config;
    	void * thread;
        bool finishflag;
        std::string filename;
        FILE_TIME last;
        FILE_TIME current;

        DIR_HDL hFile;

        void backupCurrentGraph(){
            XMLWriter writer(*(config->getContext()));
            logPrintI( "CONFIG_FUNC::writing backup \n");
            writer.write("backup.xml");
        };

        void initializeFileTime(){
#       ifdef __WIN32__
            if (! timestamp(filename.c_str(), &last)){
                last.dwLowDateTime =0;
                last.dwHighDateTime=0;
            }
#       else
            if ( !timestamp(filename.c_str(), &last)){
                last =0;
            }
#       endif
            current = last;
        }
    
        bool timestamp(const char *file, FILE_TIME * time){
            bool result = false;
#      ifdef __WIN32__
            
            struct _WIN32_FIND_DATAA data;
            DIR_HDL dir = FindFirstFile(file, &data);
            if (dir != INVALID_HANDLE_VALUE){
                (*time) = data.ftLastWriteTime;
                FindClose(dir);
                result = true;
            }
            
           
#      else
            struct stat info;
            if (stat(file, &info)==0){
                *time =info.st_mtime;
                result = true;
            }
#      endif
            return result;
	}


        std::string readFileIntoString(const char * fname){
            std::string line, result;
            std::ifstream in;
            in.open(fname);
            while(getline(in, line)){
                result += line + "\n";
            }
            return result;
        }

    public:
        FileConfigurationThread( const char * fname): 
            thread(0), finishflag(false), filename(fname){
            config = Configurator::instance();
            initializeFileTime();

        }
        ~FileConfigurationThread(){
            closeThread();

        }



        void start()
        {
            logPrintI( "START_THREAD::starting FileConfiguration thread\n");
            thread = new ACE_thread_t;
            ACE_Thread::spawn((ACE_THR_FUNC)ConfigurationThread::thread_func, 
                              this, 
                              THR_NEW_LWP | THR_JOINABLE,
                              (ACE_thread_t *)thread );
        }    



        void configTask(){
            logPrintI("CONFIG_FUNC::starting the new thread\n");
       

#     ifdef WIN32
            logPrintI( "CONFIG_FUNC:: initial timestamp: %d %d\n", last.dwLowDateTime ,last.dwHighDateTime );
            // try to open the file
#     endif //WIN32
        
            while (!finishflag){
          

                timestamp(filename.c_str(), &current);
                //                    cout << "CONFIG_FUNC:: current timestamp: " << ftWrite.dwLowDateTime << " "<< ftWrite.dwHighDateTime << endl;
                if (COMPARE_FTIME(current, last)){
                
                    // if yes push the new configuration string into the Configurator to reconfigure OpenTracker
#ifdef OT_BACKUP_ON_RECONFIG
                    backupCurrentGraph();
#endif //OT_BACKUP_ON_RECONFIG
                    logPrintI( "CONFIG_FUNC::changing configuration\n");
                    //xmlstring = readFileIntoString("reconfig.xml");
                    config->changeConfiguration(filename);
                    logPrintI( "CONFIG_FUNC::configuration changed\n");
                    last = current;
            
            
                
                }
                OSUtils::sleep(1000);
            }
        }
  

        void closeThread()
        {
#     ifdef WIN32
            ACE_Thread::join( (ACE_thread_t*)thread );
#     else
            ACE_Thread::join( (ACE_thread_t)thread );
#     endif
            // ACE_Thread::cancel( *(ACE_thread_t *)thread );
            delete ((ACE_thread_t *)thread);
        }

        void finish(){
            finishflag = true;
        }
  
    };



}

#endif  //FILE_CONFIGURATION_THREAD_HH

/*
 * ------------------------------------------------------------
 *   End of FileConfigurationThread.h
 * ------------------------------------------------------------
 *   Automatic Emacs configuration follows.
 *   Local Variables:
 *   mode:c++
 *   c-basic-offset: 4
 *   eval: (c-set-offset 'substatement-open 0)
 *   eval: (c-set-offset 'case-label '+)
 *   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
 *   eval: (setq indent-tabs-mode nil)
 *   End:
 * ------------------------------------------------------------
 */
