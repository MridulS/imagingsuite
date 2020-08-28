#ifndef BUILDFILELIST_H
#define BUILDFILELIST_H

#include <list>
#include <map>
#include <string>

#include "readerconfig_global.h"
#include "datasetbase.h"
std::list<std::string> READERCONFIGSHARED_EXPORT BuildFileList(FileSet &il);
std::list<std::string> READERCONFIGSHARED_EXPORT BuildFileList(std::list<FileSet> &il);
std::list<std::string> READERCONFIGSHARED_EXPORT BuildFileList(std::list<FileSet> &il, std::list<int> &skiplist);
std::map<float,std::string> READERCONFIGSHARED_EXPORT BuildProjectionFileList(std::list<FileSet> &il, int sequence, int goldenStartIdx, double arc);
std::map<float,std::string> READERCONFIGSHARED_EXPORT BuildProjectionFileList(std::list<FileSet> &il, std::list<int> &skiplist, int sequence, int goldenStartIdx, double arc);
#endif // BUILDFILELIST_H
