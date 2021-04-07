/*
 FS.cpp - file system wrapper
 Copyright (c) 2015 Ivan Grokhotkov. All rights reserved.
 This file is part of the esp8266 core for Arduino environment.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "FS.h"
#include "FSImpl.h"

using namespace fs;

size_t FileS::write(uint8_t c)
{
    if (!_p) {
        return 0;
    }

    return _p->write(&c, 1);
}

time_t FileS::getLastWrite()
{
    if (!_p) {
        return 0;
    }

    return _p->getLastWrite();
}

size_t FileS::write(const uint8_t *buf, size_t size)
{
    if (!_p) {
        return 0;
    }

    return _p->write(buf, size);
}

int FileS::available()
{
    if (!_p) {
        return false;
    }

    return _p->size() - _p->position();
}

int FileS::read()
{
    if (!_p) {
        return -1;
    }

    uint8_t result;
    if (_p->read(&result, 1) != 1) {
        return -1;
    }

    return result;
}

size_t FileS::read(uint8_t* buf, size_t size)
{
    if (!_p) {
        return -1;
    }

    return _p->read(buf, size);
}

int FileS::peek()
{
    if (!_p) {
        return -1;
    }

    size_t curPos = _p->position();
    int result = read();
    seek(curPos, SeekSet);
    return result;
}

void FileS::flush()
{
    if (!_p) {
        return;
    }

    _p->flush();
}

bool FileS::seek(uint32_t pos, SeekMode mode)
{
    if (!_p) {
        return false;
    }

    return _p->seek(pos, mode);
}

size_t FileS::position() const
{
    if (!_p) {
        return 0;
    }

    return _p->position();
}

size_t FileS::size() const
{
    if (!_p) {
        return 0;
    }

    return _p->size();
}

void FileS::close()
{
    if (_p) {
        _p->close();
        _p = nullptr;
    }
}

FileS::operator bool() const
{
    return !!_p;
}

const char* FileS::name() const
{
    if (!_p) {
        return nullptr;
    }

    return _p->name();
}

//to implement
boolean FileS::isDirectory(void)
{
    if (!_p) {
        return false;
    }
    return _p->isDirectory();
}

FileS FileS::openNextFile(const char* mode)
{
    if (!_p) {
        return FileS();
    }
    return _p->openNextFile(mode);
}

void FileS::rewindDirectory(void)
{
    if (!_p) {
        return;
    }
    _p->rewindDirectory();
}

FileS FS::open(const String& path, const char* mode)
{
    return open(path.c_str(), mode);
}

FileS FS::open(const char* path, const char* mode)
{
    if (!_impl) {
        return FileS();
    }

    return FileS(_impl->open(path, mode));
}

bool FS::exists(const char* path)
{
    if (!_impl) {
        return false;
    }
    return _impl->exists(path);
}

bool FS::exists(const String& path)
{
    return exists(path.c_str());
}

bool FS::remove(const char* path)
{
    if (!_impl) {
        return false;
    }
    return _impl->remove(path);
}

bool FS::remove(const String& path)
{
    return remove(path.c_str());
}

bool FS::rename(const char* pathFrom, const char* pathTo)
{
    if (!_impl) {
        return false;
    }
    return _impl->rename(pathFrom, pathTo);
}

bool FS::rename(const String& pathFrom, const String& pathTo)
{
    return rename(pathFrom.c_str(), pathTo.c_str());
}


bool FS::mkdir(const char *path)
{
    if (!_impl) {
        return false;
    }
    return _impl->mkdir(path);
}

bool FS::mkdir(const String &path)
{
    return mkdir(path.c_str());
}

bool FS::rmdir(const char *path)
{
    if (!_impl) {
        return false;
    }
    return _impl->rmdir(path);
}

bool FS::rmdir(const String &path)
{
    return rmdir(path.c_str());
}


void FSImpl::mountpoint(const char * mp)
{
    _mountpoint = mp;
}

const char * FSImpl::mountpoint()
{
    return _mountpoint;
}
