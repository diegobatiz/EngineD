#include <EngineD/Inc/EngineD.h>

using namespace EngineD;
using namespace EngineD::Core;

static int sUniqueId = 0;
class Student
{
public:
	Student(const std::string& name)
		: mName(name)
		, mId(++sUniqueId)
	{
	}

	Student(const std::string& name, int id)
		: mName(name)
		, mId(id)
	{
	}

private:
	int mId = 0;
	std::string mName;
};

class Lion
{
public:
	Lion(const std::string& name, int id, const std::string& food)
		: mName(name)
		, mId(id)
		, mFavouriteFood(food)
	{
	}

private:
	int mId = 0;
	std::string mName;
	std::string mFavouriteFood;
};

class Song
{
public:
	Song(const std::string& title, const std::string& artist, float length)
		: mTitle(title)
		, mArtistName(artist)
		, mLength(length)
	{
	}

private:
	float mLength;
	std::string mTitle;
	std::string mArtistName;
};

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PSTR lpCmdLine, _In_ int nCmdShow)
{
	TypedAllocator studentPool = TypedAllocator<Student>("StudentPool", 200);
	TypedAllocator lionPool = TypedAllocator<Lion>("Animal Pool", 150);
	TypedAllocator songPool = TypedAllocator<Song>("Song Pool", 400);

	int size = sizeof(Student);

	std::vector<Student*> students;
	std::vector<Lion*> lions;
	std::vector<Song*> songs;

	//test new
	for (uint32_t i = 0; i < 10; ++i)
	{
		Student* newStudent = studentPool.New("Student_" + std::to_string(i), i);
		Lion* lion = lionPool.New("Lion_" + std::to_string(i), i, "Meat");
		students.push_back(newStudent);
		lions.push_back(lion);
	}
	for (uint32_t i = 0; i < 350; ++i)
	{
		Song* newSong = songPool.New("Song No. " + std::to_string(i), "Diego Batiz", 90.0f + i * 0.1f);
		songs.push_back(newSong);
	}

	//test deletion
	for (uint32_t i = 0; i < 5; ++i)
	{
		auto& student = students.back();
		studentPool.Delete(student);
		students.pop_back();
	}
	for (uint32_t i = 0; i < 5; ++i)
	{
		auto& lion = lions.back();
		lionPool.Delete(lion);
		lions.pop_back();
	}
	for (uint32_t i = 0; i < 100; ++i)
	{
		auto& song = songs.back();
		songPool.Delete(song);
		songs.pop_back();
	}

	//add new items
	for (uint32_t i = 0; i < 150; ++i)
	{
		Student* newStudent = studentPool.New("Student_" + std::to_string(i));
		students.push_back(newStudent);
	}
	for (uint32_t i = 0; i < 50; ++i)
	{
		Lion* lion = lionPool.New("Lion_" + std::to_string(i), i, "Meat");
		lions.push_back(lion);
	}
	for (uint32_t i = 0; i < 150; ++i)
	{
		Song* newSong = songPool.New("Song No. " + std::to_string(i), "Diego Batiz", 90.0f + i * 0.1f);
		songs.push_back(newSong);
	}


	for (auto& student : students)
	{
		studentPool.Delete(student);
	}

	for (auto& lion : lions)
	{
		lionPool.Delete(lion);
	}

	for (auto& song : songs)
	{
		songPool.Delete(song);
	}

	students.clear();
	lions.clear();
	songs.clear();

	return 0;
}