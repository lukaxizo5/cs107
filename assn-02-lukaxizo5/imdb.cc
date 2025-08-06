using namespace std;
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include "imdb.h"

const char *const imdb::kActorFileName = "actordata";
const char *const imdb::kMovieFileName = "moviedata";

imdb::imdb(const string& directory)
{
  const string actorFileName = directory + "/" + kActorFileName;
  const string movieFileName = directory + "/" + kMovieFileName;
  
  actorFile = acquireFileMap(actorFileName, actorInfo);
  movieFile = acquireFileMap(movieFileName, movieInfo);
}

bool imdb::good() const
{
  return !( (actorInfo.fd == -1) || 
	    (movieInfo.fd == -1) ); 
}

// you should be implementing these two methods right here... 
bool imdb::getCredits(const string& player, vector<film>& films) const { 
  int count = *(int*)(actorFile);
  int start = 0;
  int end = count - 1;
  while (start <= end) {
    int mid = (end - start) / 2 + start;
    char* curr_actor_ptr = (char*)(actorFile) + (mid + 1) * sizeof(int);
    int curr_actor_offset = *(int*)(curr_actor_ptr);
    char* curr_actor_record = (char*)(actorFile) + curr_actor_offset;
    string curr_actor_name = "";
    while (*curr_actor_record != '\0') {
      curr_actor_name += *curr_actor_record;
      curr_actor_record++;
    }
    curr_actor_record++; //'\0'
    int total_bytes = 0;
    if (curr_actor_name == player) {
      if (player.size() % 2 == 0) { 
        curr_actor_record++;  
        total_bytes += player.size() + 2;
      }
      else total_bytes += player.size() + 1;
      short movies_count = *(short*)(curr_actor_record);
      curr_actor_record += sizeof(short);
      total_bytes += sizeof(short);
      if (total_bytes % 4 != 0) curr_actor_record += 2;
      for (int i = 0; i < movies_count; i++) {
        int curr_offset = *(int*)(curr_actor_record);
        curr_actor_record += sizeof(int);
        char* movie_record = (char*)(movieFile) + curr_offset;
        string title = "";
        while (*movie_record != '\0') {
          title += *movie_record;
          movie_record++;
        }
        movie_record++;
        int year = *movie_record + 1900;
        films.push_back({title, year});
      }
      return true;
    }
    else if (curr_actor_name < player) start = mid + 1;
    else end = mid - 1;
  }
  return false; 
}

bool imdb::getCast(const film& movie, vector<string>& players) const { 
  int count = *(int*)(movieFile);
  int start = 0;
  int end = count - 1;
  while (start <= end) {
    int mid = (end - start) / 2 + start;
    char* curr_movie_ptr = (char*)(movieFile) + (mid + 1) * sizeof(int);
    int curr_movie_offset = *(int*)(curr_movie_ptr);
    char* curr_movie_record = (char*)(movieFile) + curr_movie_offset;
    string curr_movie_title = curr_movie_record;
    curr_movie_record += curr_movie_title.size() + 1; // get to year byte
    int year = *curr_movie_record + 1900;
    curr_movie_record++; //get past year byte
    film curr_film;
    curr_film.title = curr_movie_title;
    curr_film.year = year;
    if (curr_film == movie) {
      if (movie.title.size() % 2 == 1) curr_movie_record++; // year byte had '\0' also
      short actor_count = *(short*)(curr_movie_record);
      curr_movie_record += sizeof(short);
      if (movie.title.size() % 2 == 0) {
        if (movie.title.size() % 4 == 2) curr_movie_record += 2;
      }
      else {
        if ((movie.title.size() + 1) % 4 == 2) curr_movie_record += 2;
      }
      for (int i=0;i<actor_count;i++) {
        char* actor_record = (char*)(actorFile) + *(int*)(curr_movie_record);
        string name = actor_record;
        players.push_back(name);
        curr_movie_record += sizeof(int);
      }
      return true;
    }
    else if (curr_film < movie) start = mid + 1;
    else end = mid - 1;
  }
  return false;
}

imdb::~imdb()
{
  releaseFileMap(actorInfo);
  releaseFileMap(movieInfo);
}

// ignore everything below... it's all UNIXy stuff in place to make a file look like
// an array of bytes in RAM.. 
const void *imdb::acquireFileMap(const string& fileName, struct fileInfo& info)
{
  struct stat stats;
  stat(fileName.c_str(), &stats);
  info.fileSize = stats.st_size;
  info.fd = open(fileName.c_str(), O_RDONLY);
  return info.fileMap = mmap(0, info.fileSize, PROT_READ, MAP_SHARED, info.fd, 0);
}

void imdb::releaseFileMap(struct fileInfo& info)
{
  if (info.fileMap != NULL) munmap((char *) info.fileMap, info.fileSize);
  if (info.fd != -1) close(info.fd);
}
