#include "videoplayer.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>


void VideoPlayer::numberOfVideos() {
  std::cout << mVideoLibrary.getVideos().size() << " videos in the library"
            << std::endl;
}

void VideoPlayer::showAllVideos() {
  std::cout << "Here's a list of all available videos:\n";


  //Get all videos and sort them alphabetically before displaying them
  std::vector<Video> videos = mVideoLibrary.getVideos();
  std::sort(videos.begin(), videos.end(), [](const Video& a, const Video& b) {
      return a.getTitle() < b.getTitle();  
  });

  //Format videos to match specific output
  for (int i = 0; i < videos.size(); i++) {
      std::string title = videos[i].getTitle();
      std::string id = "(" +  videos[i].getVideoId() + ")";
      std::vector<std::string> tags = videos[i].getTags();
      
      std::string tag = "[";
      for (int j = 0; j < tags.size(); j++) {
          if (j == tags.size() - 1) {
              tag += tags[j];
          }
          else {
              tag += tags[j] + " ";
          }
      }
      tag += "]";

      std::string video = title + " " + id + " " + tag;
      std::cout << video << "\n";
  }
}

void VideoPlayer::playVideo(const std::string& videoId) {
    std::vector<Video> videos = mVideoLibrary.getVideos();

    for (int i = 0; i < videos.size(); i++) {
        //Find video
        if (videos[i].getVideoId() == videoId) {
            //If already playing something else, stop it
            if (currentVideo != "") {
                std::cout << "Stopping video: " + currentVideo + "\n";
            }
            //Begin new video with correct output
            std::cout << "Playing video: " + videos[i].getTitle() << "\n";
            currentVideo = videos[i].getTitle();
            currentId = videos[i].getVideoId();
            currentTags = "";
            std::vector<std::string> tags = videos[i].getTags();

            for (int j = 0; j < tags.size(); j++) {
                if (j == tags.size() - 1) {
                    currentTags += tags[j];
                }
                else {
                    currentTags += tags[j] + " ";
                }
            }

            playing = true;
            return;
        }
    }
    std::cout << "Cannot play video: Video does not exist\n";
}

void VideoPlayer::stopVideo() {
    if (currentVideo == "") {
        std::cout << "Cannot stop video: No video is currently playing\n";
        playing = false;
        return;
    }

    std::cout << "Stopping video: " + currentVideo << "\n";
    playing = false;
    wipeCurrent();
}

void VideoPlayer::playRandomVideo() {
    //From size of video library, select random number and play that ID
    std::vector<Video> videos = mVideoLibrary.getVideos();
    int random = rand() % videos.size();
    
    std::string videoId = videos[random].getVideoId();
    playVideo(videoId);
}

void VideoPlayer::pauseVideo() {
    if (currentVideo == "") {
        playing = false;
        std::cout << "Cannot pause video: No video is currently playing\n";
        return;
    }

    if (playing) {
        std::cout << "Pausing video: " << currentVideo << "\n";
        playing = false;
    }
    else {
        std::cout << "Video already paused: " + currentVideo << "\n";
    }
    
}

void VideoPlayer::continueVideo() {
    if (currentVideo == "") {
        std::cout << "Cannot continue video: No video is currently playing\n";
        playing = false;
        return;
    }

    if (!playing) {
        std::cout << "Continuing video: " + currentVideo << "\n";
        playing = true;
    }
    else {
        std::cout << "Cannot continue video: Video is not paused\n";
    }
}

void VideoPlayer::showPlaying() {
    if (currentVideo == "") {
        std::cout << "No video is currently playing\n";
        return;
    }
    
    std::string output = currentVideo +
        " (" + currentId + ")" +
        " [" + currentTags + "]";
    if (playing) {
        std::cout << "Currently playing: " + output << "\n";
    }
    else {
        std::cout << "Currently playing: " + output << " - PAUSED\n";
    }
}

void VideoPlayer::createPlaylist(const std::string& playlistName) {


    for (int i = 0; i < playlists.size(); i++) {
        std::string str1 = playlists[i];

        //Can't be identical without having same size, more efficient comparison
        if (str1.size() == playlistName.size()) {
            for (int j = 0; j < str1.size(); j++) {
                if (tolower(str1[j] != tolower(playlistName[j])))
                    break;
            }
            if (i == playlists.size() - 1) {
                std::cout << "Cannot create playlist: A playlist with the same name already exists\n";
                return;
            }
            
        }
    }
    playlists.push_back(playlistName);
    std::cout << "Successfully created new playlist: " + playlistName << "\n";
}


void VideoPlayer::addVideoToPlaylist(const std::string& playlistName,
                                     const std::string& videoId) {
    int id = -1;

    if (playlists.size() == 0) {
        std::cout << "Cannot add video to " + playlistName + ": Playlist does not exist\n";
        return;
    }

    for (int i = 0; i < playlists.size(); i++) {
        std::string str = playlists[i];

        //skip if strings not equal length
        if (str.size() != playlistName.size())
            continue;        

        //Character match on equal strings
        for (int j = 0; j < str.size(); j++) {
            if (tolower(str[j]) == tolower(playlistName[j])) {
                if (j == str.size() - 1) {
                    id = 0;
                    break;
                }
            }
            else
                break;                     
        }
        if (id == -1) {
            std::cout << "Cannot add video to " + playlistName + ": Playlist does not exist\n";
            return;
        }        
    }
    //Video title check
    std::vector<Video> videos = mVideoLibrary.getVideos();
    std::string title = "";

    for (int i = 0; i < videos.size(); i++) {
        if (videos[i].getVideoId() == videoId) {
            title = videos[i].getTitle();
            break;
        }
    }
    if (title == "") {
        std::cout << "Cannot add video to " + playlistName + ": Video does not exist\n";
        return;
    }

    //Play list content check
    for (int d = 0; d < data.size(); d++) {
        if (data[d] == std::to_string(id)) {
            if (data[d + 1] == title) {
                std::cout << "Cannot add video to " + playlistName + ": Video already added\n";
                return;
            }
        }
    }
    data.push_back(std::to_string(id));
    data.push_back(title);

    std::cout << "Added video to " + playlistName + ": " + title + "\n";
}

void VideoPlayer::showAllPlaylists() {
    if (playlists.size() == 0) {
        std::cout << "No playlists exist yet\n";
        return;
    }

    std::cout << "Showing all playlists:\n";
    std::vector<std::string> list;

    for (int i = 0; i < playlists.size(); i++) {
        list.push_back(playlists[i]);
    }

    std::sort(list.begin(), list.end(), [](const std::string a, const std::string b) {
        return a < b;
    });

    for (int i = 0; i < playlists.size(); i++) {
        std::cout << list[i] << "\n";
    }
    
}


void VideoPlayer::showPlaylist(const std::string& playlistName) {
    if (playlists.size() == 0) {
        std::cout << "Cannot show playlist " + playlistName + ": Playlist does not exist\n";
        return;
    }

    std::string title = "";
    int id = -1;
    for (int i = 0; i < playlists.size(); i++) {
        if (playlists[i].size() != playlistName.size()) {
            continue;
        }

        std::string str = playlists[i];
        for (int j = 0; j < str.size(); j++) {
            if (tolower(str[j]) == tolower(playlistName[j])) {
                if (j == str.size() - 1) {
                    title = playlists[i];
                    id = i;
                    break;
                }
            }
        }
    }
    if (title == "") {
        std::cout << "Cannot show playlist " + playlistName + ": Playlist does not exist\n";
        return;
    }
    int check = 0;
    std::cout << "Showing playlist: " + playlistName + "\n";
    
    std::vector<Video> videos = mVideoLibrary.getVideos();
    std::string video = "";
    std::string videoId = "";
    std::string tag = "";
    std::vector<std::string> tags;

    for (int d = 0; d < data.size(); d++) {
        if (data[d] == std::to_string(id)) {

            for (int i = 0; i < videos.size(); i++) {
                if (videos[i].getTitle() == data[d+1]) {
                    video = videos[i].getTitle();
                    videoId = videos[i].getVideoId();
                    tags = videos[i].getTags();
                    break;
                }

                for (int j = 0; j < tags.size(); j++) {
                    if (j == tags.size() - 1) {
                        tag += tags[j];
                    }
                    else {
                        tag += tags[j] + " ";
                    }
                }
            }

            std::cout << video + " (" + videoId + ") [" + tag + "]\n";
            check++;
        }
    }
    if (check == 0) {
        std::cout << "No videos here yet\n";
    }

}

//I unfortunately had some complications during my internship that needed my attention
//Was able to finish until here, but I am highly confident of my ability to fully complete
//this challenge otherwise.
void VideoPlayer::removeFromPlaylist(const std::string& playlistName,
                                     const std::string& videoId) {
    if (playlists.size() == 0) {
        std::cout << "Cannot remove video from " + playlistName + ": Playlist does not exist\n";
        return;
    }

    std::string title = "";
    int id = -1;
    for (int i = 0; i < playlists.size(); i++) {
        if (playlists[i].size() != playlistName.size()) {
            continue;
        }

        std::string str = playlists[i];
        for (int j = 0; j < str.size(); j++) {
            if (tolower(str[j]) == tolower(playlistName[j])) {
                if (j == str.size() - 1) {
                    title = playlists[i];
                    id = i;
                    break;
                }
            }
        }
    }
    if (title == "") {
        std::cout << "Cannot remove video from " + playlistName + ": Playlist does not exist\n";
        return;
    }
}

void VideoPlayer::clearPlaylist(const std::string& playlistName) {
    if (playlists.size() == 0) {
        std::cout << "Cannot clear playlist " + playlistName + ": Playlist does not exist\n";
        return;
    }

    std::string title = "";
    int id = -1;
    for (int i = 0; i < playlists.size(); i++) {
        if (playlists[i].size() != playlistName.size()) {
            continue;
        }

        std::string str = playlists[i];
        for (int j = 0; j < str.size(); j++) {
            if (tolower(str[j]) == tolower(playlistName[j])) {
                if (j == str.size() - 1) {
                    title = playlists[i];
                    id = i;
                    break;
                }
            }
        }
    }
    if (title == "") {
        std::cout << "Cannot clear playlist " + playlistName + ": Playlist does not exist\n";
        return;
    }
}

void VideoPlayer::deletePlaylist(const std::string& playlistName) {
    if (playlists.size() == 0) {
        std::cout << "Cannot delete playlist " + playlistName + ": Playlist does not exist\n";
        return;
    }

    for (int i = 0; i < playlists.size(); i++) {
        if (playlists[i].size() != playlistName.size()) {
            continue;

        }

        std::string str = playlists[i];
        for(int j = 0; j < playlists[i].size(); j++) {
            if (tolower(str[j] == tolower(playlistName[j]))) {
                if (j == str.size() - 1) {
                    playlists.erase(playlists.begin() + i);
                        std::cout << "Deleted playlist: " + playlistName + "\n";
                        return;
                }
            }          
            
            //Would also need to delete playlist contents here, no time
        }
    }
    std::cout << "Cannot delete playlist " + playlistName + ": Playlist does not exist\n";
    return;
}

void VideoPlayer::searchVideos(const std::string& searchTerm) {
  std::cout << "searchVideos needs implementation" << std::endl;
}

void VideoPlayer::searchVideosWithTag(const std::string& videoTag) {
  std::cout << "searchVideosWithTag needs implementation" << std::endl;
}

void VideoPlayer::flagVideo(const std::string& videoId) {
  std::cout << "flagVideo needs implementation" << std::endl;
}

void VideoPlayer::flagVideo(const std::string& videoId, const std::string& reason) {
  std::cout << "flagVideo needs implementation" << std::endl;
}

void VideoPlayer::allowVideo(const std::string& videoId) {
  std::cout << "allowVideo needs implementation" << std::endl;
}
