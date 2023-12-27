#include "fileutils.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "common/log.h"

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
    int64_t total_size = getFilesSize(src_files_list);
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

        if(mOperation == COPY) {
            const char *src_copy_path = src_files_list[i].toLatin1().data();
            int source_fd = open(src_copy_path, O_RDONLY);
            if (source_fd == -1) {
                mLock.unlock();
                RLOGE("Error opening source file");
                ret = RET_FAILURE;
                break;
            }

            QStringList tmp = src_files_list[i].split("/");
			QString fileName = tmp.at(tmp.count() - 1);
            RLOGD("file name is %s",fileName.toLatin1().data());
			QString dst_path = mDstDir + "/" + fileName;

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
			
			close(source_fd);
			close(dest_fd);
        } else if(mOperation == DELETE) {
            const char *file_to_delete =  src_files_list[i].toLatin1().data();
			RLOGD("Deleting file: %s\n", file_to_delete);
			// 删除文件
			if (remove(file_to_delete) == 0) {
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

int64_t FileUtils::getFileSize(QString filePath)
{
    int file_fd = open(filePath.toLatin1().data(), O_RDONLY);
    int file_size = 0;
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

int64_t FileUtils::getFilesSize(QList<QString> fileslist)
{
    int64_t file_total_size = 0;
    int count = fileslist.count();
    for(int i = 0;i < count;i++) {
        file_total_size += getFileSize(fileslist.at(i));
    }

    return file_total_size;
}
