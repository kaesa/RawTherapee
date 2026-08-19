/*
 *  This file is part of RawTherapee.
 *
 *  Copyright (c) 2004-2010 Gabor Horvath <hgabor@rawtherapee.com>
 *
 *  RawTherapee is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  RawTherapee is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with RawTherapee.  If not, see <https://www.gnu.org/licenses/>.
 */
#pragma once

#include <memory>
#include <map>
#include <string>

#include <glibmm/ustring.h>

#include "threadutils.h"

#include "rtengine/noncopyable.h"

class CacheImageData;
class Thumbnail;

class CacheManager :
    public rtengine::NonCopyable
{
private:
    using Entries = std::map<std::string, std::weak_ptr<Thumbnail>>;
    Entries openEntries;
    Glib::ustring    baseDir;
    mutable MyMutex  mutex;

    void deleteDir   (const Glib::ustring& dirName) const;
    void deleteFiles (const Glib::ustring& fname, const std::string& md5, bool purgeData, bool purgeProfile) const;

    void applyCacheSizeLimitation () const;
    void updateImageInfo(const Glib::ustring &fname, CacheImageData &imageData, const Glib::ustring &xmpSidecarMd5) const;

public:
    static CacheManager* getInstance ();

    void        init        ();

    std::shared_ptr<Thumbnail> getEntry (const Glib::ustring& fname);
    void        renameEntry (const std::string& oldfilename, const std::string& oldmd5, const std::string& newfilename);

    void closeCache () const;

    void clearEntry (const Glib::ustring& fname);   ///< Attempts at removing the a Thumbnail from cache. If no one is currently owning the entry through shared_ptr, it will get removed.
    void clearExpiredEntries ();                    ///< Clears all expired entries from the cache.

    void clearAll () const;
    void clearImages () const;
    void clearProfiles () const;
    void clearFromCache (const Glib::ustring& fname, bool purge) const;
    static std::string getMD5 (const Glib::ustring& fname);

    Glib::ustring    getCacheFileName (const Glib::ustring& subDir,
                                       const Glib::ustring& fname,
                                       const Glib::ustring& fext,
                                       const Glib::ustring& md5) const;
};

#define cacheMgr CacheManager::getInstance()
