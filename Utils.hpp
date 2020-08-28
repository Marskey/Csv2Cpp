#pragma once
#ifdef WIN32
#include <io.h>
#include <filesystem>
#else
#include <dirent.h>
#endif
#include <vector>
#include <string>
#include <sstream>

class CUtils
{
public:
    static std::vector<std::string> Split(const std::string& s, const char delim) {
        std::stringstream ss(s);
        std::string item;
        std::vector<std::string> elems;
        while (std::getline(ss, item, delim)) {
            elems.emplace_back(item);
        }
        return elems;
    }

    static bool SnakeCaseToCamelCase(std::string input, std::string* output) {
        output->clear();
        bool after_underscore = false;
        for (size_t i = 0; i < input.size(); ++i) {
            //if (input[i] >= 'A' && input[i] <= 'Z') {
            //  // The field name must not contain uppercase letters.
            //  return false;
            //}
            if (after_underscore) {
                if (input[i] >= 'a' && input[i] <= 'z') {
                    output->push_back(input[i] + 'A' - 'a');
                    after_underscore = false;
                } else {
                    // The character after a "_" must be a lowercase letter.
                    return false;
                }
            } else if (input[i] == '_') {
                after_underscore = true;
            } else {
                output->push_back(input[i]);
            }
        }
        if (after_underscore) {
            // Trailing "_".
            return false;
        }
        return true;
    }

    static void GetFileList(std::string path, std::vector<std::string>& out) {
        if (path.size() != 0
            && path.back() != '\\') {
            path.push_back('\\');
        }
        std::string temp = path + '*';
#ifdef WIN32
        intptr_t hFile = 0;
        struct _finddata_t fileinfo;
        if ((hFile = _findfirst(temp.c_str(), &fileinfo)) != -1) {
            do {
                if (!(fileinfo.attrib & _A_HIDDEN)) {
                    if (!(fileinfo.attrib & _A_SUBDIR)) {
                        out.emplace_back(path + fileinfo.name);
                    } else if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
                        GetFileList(path + fileinfo.name, out);
                    }
                }
            } while (_findnext(hFile, &fileinfo) == 0);
            _findclose(hFile);
        }
#else
        DIR* dir = opendir("/home/hanchao/picture");//打开指定目录
        dirent* p = NULL;//定义遍历指针
        while ((p = readdir(dir)) != NULL)//开始逐个遍历
        {
            //这里需要注意，linux平台下一个目录中有"."和".."隐藏文件，需要过滤掉
            if (p->d_name[0] != '.')//d_name是一个char数组，存放当前遍历到的文件名
            {
                string name = "/home/hanchao/picture/" + string(p->d_name);
                cout << name << endl;
            }
        }
        closedir(dir);//关闭指定目录   
#endif
    }

};
