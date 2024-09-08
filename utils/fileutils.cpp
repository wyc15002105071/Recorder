#include "fileutils.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "common/log.h"
#include <QFileInfo>
#include <QDir>
#include <dirent.h>

// 创建目标目录
static bool create_dir(const char *dst) {
    struct stat st = {0};
    if (stat(dst, &st) == -1) {
        if (mkdir(dst, 0700) != 0) {
            RLOGE("mkdir failed");
            return false;
        }
    }

	return true;
}

// 拷贝单个文件
static bool copy_file(const char *src, const char *dst) {
    int input_fd, output_fd;
    ssize_t nread;
    char buffer[4096];

    input_fd = open(src, O_RDONLY);
    if (input_fd == -1) {
        RLOGE("open src file failed");
        return false;
    }

    output_fd = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (output_fd == -1) {
        close(input_fd);
        RLOGE("open dst file failed");
        return false;
    }

    while ((nread = read(input_fd, buffer, sizeof(buffer))) > 0) {
        if (write(output_fd, buffer, nread) != nread) {
            RLOGE("write failed");
            return false;
        }
    }

    if (nread == -1) {
        RLOGE("read failed");
        return false;
    }
	fsync(output_fd);
    close(input_fd);
    close(output_fd);

	return true;
}

// 递归拷贝文件夹
static bool copy_dir(const char *src, const char *dst) {
    DIR *dir;
    struct dirent *entry;
    struct stat st;

    if (!(dir = opendir(src))) {
        RLOGE("opendir failed %s", src);
        return false;
    }

    if(!create_dir(dst)) {
		RLOGE("create dir %s failed", dst);
	}

    while ((entry = readdir(dir)) != NULL) {
        char src_path[1024];
        char dst_path[1024];

        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(src_path, sizeof(src_path), "%s/%s", src, entry->d_name);
        snprintf(dst_path, sizeof(dst_path), "%s/%s", dst, entry->d_name);

		RLOGD("src %s, dst %s", src, dst);
		RLOGD("src path %s, dst path %s", src_path, dst_path);

        if (stat(src_path, &st) == -1) {
            RLOGE("stat failed");
            return false;
        }

        if (S_ISDIR(st.st_mode)) {
            // 如果是目录，递归拷贝
            copy_dir(src_path, dst_path);
        } else {
            // 如果是文件，拷贝文件
            copy_file(src_path, dst_path);
        }
    }

    closedir(dir);

	return true;
}

// 递归删除文件夹及其内容
static int delete_dir(const char *dir_path) {
    DIR *dir;
    struct dirent *entry;
    char path[1024];
    struct stat statbuf;

    // 打开目录
    if (!(dir = opendir(dir_path))) {
        RLOGE("opendir failed %s", dir_path);
        return -1;
    }

    // 读取目录内容
    while ((entry = readdir(dir)) != NULL) {
        // 跳过 "." 和 ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);

        // 获取文件信息
        if (stat(path, &statbuf) == -1) {
            RLOGE("stat failed %s", path);
            closedir(dir);
            return -1;
        }

        if (S_ISDIR(statbuf.st_mode)) {
            // 如果是目录，递归删除
            if (delete_dir(path) == -1) {
                closedir(dir);
                return -1;
            }
        } else {
            // 如果是文件，删除文件
            if (unlink(path) == -1) {
                RLOGE("unlink failed %s", path);
                closedir(dir);
                return -1;
            }
        }
    }

    closedir(dir);

    // 删除目录本身
    if (rmdir(dir_path) == -1) {
        RLOGE("rmdir failed");
        return -1;
    }

    return 0;
}


FileUtils::FileUtils()
{

}

FileUtils::~FileUtils()
{
    stopTask();
}

void FileUtils::run()
{
    QList<QString> src_files_list = std::move(mSrcFilesPath);
    int file_count = src_files_list.count();
    long long total_size = getFilesSize(src_files_list);
    int64_t already_handled_size = 0; //already copy bytes or already delete file nums
	int i = 0;
    int ret = RET_SUCCESS;
    float precent = 0.0f;
    NotifyData ndata;

    ndata.data = &precent;
    ndata.finish = false;
    ndata.result = RET_SUCCESS;
    ndata.flags = mOperation;
    RLOGD("start %s,file count %d total size %d -> %s",mOperation==COPY?"copy":"delete",file_count,total_size,mOperation==COPY?mDstDir.toLatin1().data():"");

    while(!mThreadExit) {
        mLock.lock();
        if(file_count <= 0 || total_size == 0) {
            mLock.unlock();
            break;
        }
		
		char src_file[100] = {0};
		strcpy(src_file, src_files_list[i].toLatin1().data());
		struct stat path_stat;
		if (stat(src_file, &path_stat) != 0) {
			RLOGE("stat failed %s", src_file);
			ret = RET_FAILURE;
			break;
		}
		
        if(mOperation == COPY) {
			QStringList tmp = src_files_list[i].split("/");
			QString fileName = tmp.at(tmp.count() - 1);
	        RLOGD("file name is %s",fileName.toLatin1().data());
			QString dst_path = mDstDir + "/" + fileName;
			
			if (S_ISDIR(path_stat.st_mode)) {				
				RLOGD("copy src dir %s , dst dir %s", src_files_list[i].toLatin1().data(), dst_path.toLatin1().data());
				copy_dir(src_files_list[i].toLatin1().data(), dst_path.toLatin1().data());
				precent = (float)(i + 1) / src_files_list.count() * 100;
	            ndata.data = &precent;
	            notify(ndata);
			} else if (S_ISREG(path_stat.st_mode)) {
	            int source_fd = open(src_file, O_RDONLY);
	            if (source_fd == -1) {
	                mLock.unlock();
	                RLOGE("Error opening source file: %s", src_file);
	                ret = RET_FAILURE;
	                break;
	            }

	            RLOGD("dst file path is %s",dst_path.toLatin1().data());
				int dest_fd = open(dst_path.toLatin1().data(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
				if (dest_fd == -1) {
	                mLock.unlock();
	                RLOGE("Error opening destination file...:%s",strerror(errno));
					close(source_fd);
	                ret = RET_FAILURE;
					break;
				}
				
				char buffer[BUFFER_SIZE];
				ssize_t bytes_read, bytes_written;
				off_t total_bytes = 0, file_size;

				// 获取源文件大小
				file_size = lseek(source_fd, 0, SEEK_END);
				lseek(source_fd, 0, SEEK_SET);

				while ((bytes_read = read(source_fd, buffer, BUFFER_SIZE)) > 0) {
					bytes_written = write(dest_fd, buffer, bytes_read);
					if (bytes_written != bytes_read) {
	                    mLock.unlock();
						RLOGE("Error writing to destination file");
						close(source_fd);
						close(dest_fd);
	                    ret = RET_FAILURE;
						break;
					}

	                already_handled_size += bytes_read;

	                precent = (float)already_handled_size / total_size * 100;
	                ndata.data = &precent;
	                notify(ndata);

	                fflush(stdout);
				}
				
				fsync(dest_fd);
				close(source_fd);
				close(dest_fd);
			}
        } else if(mOperation == DELETE) {
			RLOGD("Deleting file: %s\n", src_file);
			if (S_ISDIR(path_stat.st_mode)) {
				RLOGD("delete dir %s", src_file);
				delete_dir(src_file);
				precent = (float)(i + 1) / src_files_list.count() * 100;
	            ndata.data = &precent;
	            notify(ndata);
			} else if (S_ISREG(path_stat.st_mode)) {
				// 删除文件
				if (remove(src_file) == 0) {
					RLOGD("File deleted successfully.\n");
	                already_handled_size++;
	                precent = (float)already_handled_size / file_count * 100;
	                ndata.data = &precent;
	                notify(ndata);
				} else {
					RLOGE("Error deleting file");
	                ret = RET_FAILURE;
	                mLock.unlock();
					break;
				}
			}
        } else {
            mLock.unlock();
            ret = RET_FAILURE;
            break;
        }

        mLock.unlock();
        usleep(1*1000);
		
		i++;
        if(i >= file_count)
			break;
    }

    RLOGD("file %s %s",mOperation==COPY?"copy":"delete",ret==RET_SUCCESS?"success":"failure");

    ndata.finish = true;
    ndata.result = (Result)ret;
    notify(ndata);
    mThreadExit = true;
}

bool FileUtils::startTask()
{
    if(isRunning())
        return false;
    mThreadExit = false;
    this->start();
    return true;
}

void FileUtils::stopTask()
{
    mThreadExit = true;
    this->quit();
    wait(QUIT_TIMEOUT);
}

const char *FileUtils::getFileSuffix(QString file_path)
{
    QString file_name = file_path.split("/").last();
    return file_name.split(".").last().toLatin1().data();
}

const char *FileUtils::pathToName(QString path)
{
    QStringList list = path.split("/");
    if(list.count() == 0)
        return nullptr;
    return list.at(list.count() - 1).toLatin1().data();
}

const char *FileUtils::pathToDirPath(QString path)
{
    QString file_path = pathToName(path);

    QFileInfo fileinfo(path);
    return fileinfo.absoluteDir().path().toLatin1().data();
}

void FileUtils::mkdirIfNotExit(QString dir)
{
    struct stat st = {0};
    if(::stat(dir.toLatin1().data(), &st) == -1) {
        RLOGD("create directory %s", dir.toLatin1().data());
        mkdir(dir.toLatin1().data(), 0700);
    }
}

void FileUtils::startCopy(QList<QString> &filesPath, QString dst_dir)
{
    mLock.lock();
    if(isRunning()) {
        mLock.unlock();
        return;
    }

    this->mSrcFilesPath = filesPath;
    this->mDstDir = dst_dir;
    mOperation = COPY;
    mLock.unlock();
    startTask();

}

void FileUtils::startDelete(QList<QString> &filesPath)
{
    mLock.lock();
    this->mSrcFilesPath = std::move(filesPath);
    mThreadExit = false;
    mOperation = DELETE;
    this->start();
    mLock.unlock();
}

long long FileUtils::getDirSize(QString dirPath)
{
    DIR *dir;
    struct dirent *entry;
    struct stat st;
    long long total_size = 0;

	const char *dir_path = dirPath.toLatin1().data();
    if (!(dir = opendir(dir_path))) {
        RLOGE("opendir failed %s", dir_path);
        return 0;
    }

    while ((entry = readdir(dir)) != NULL) {
        char path[1024];

        // 跳过 "." 和 ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);
        if (stat(path, &st) == -1) {
            RLOGE("stat failed");
            closedir(dir);
            return 0;
        }

        if (S_ISDIR(st.st_mode)) {
            // 如果是目录，递归计算目录大小
            long long dir_size = getDirSize(path);
            if (dir_size == -1) {
                closedir(dir);
                return 0;
            }
            total_size += dir_size;
        } else {
            // 如果是文件，累加文件大小
            RLOGD("path is %s", path);
            total_size += getFileSize(QString(path));
			RLOGD("total size is %d", total_size);
        }
    }

    closedir(dir);
    return total_size;

}

long long FileUtils::getFileSize(QString filePath)
{
    int file_fd = open(filePath.toLatin1().data(), O_RDONLY);
    long long  file_size = 0;
    if (file_fd == -1) {
        RLOGE("Error opening source file");
        goto RET;
    }

    file_size = lseek(file_fd, 0, SEEK_END);
    lseek(file_fd, 0, SEEK_SET);

RET:
    close(file_fd);
    return file_size;
}

long long  FileUtils::getFilesSize(QList<QString> fileslist)
{
    long long  file_total_size = 0;
    int count = fileslist.count();

    for(int i = 0;i < count;i++) {
		struct stat path_stat;
	    // 获取路径信息

	    if (stat(fileslist.at(i).toLatin1().data(), &path_stat) != 0) {
	        RLOGE("stat failed %s", fileslist.at(i).toLatin1().data());
	        file_total_size += 0;
			continue;
	    }
		if (S_ISREG(path_stat.st_mode)) {
        	file_total_size += getFileSize(fileslist.at(i));
		} else if (S_ISDIR(path_stat.st_mode)) {
			file_total_size += getDirSize(fileslist.at(i));
		}
    }

    return file_total_size;
}
