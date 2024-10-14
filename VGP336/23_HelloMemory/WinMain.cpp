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

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PSTR lpCmdLine, _In_ int nCmdShow)
{
	TypedAllocator studentPool = TypedAllocator<Student>("StudentPool", 250);

	std::vector<Student*> students;

	//test new
	for (uint32_t i = 0; i < 100; ++i)
	{
		Student* newStudent = studentPool.New("Student_" + std::to_string(i), i);
		students.push_back(newStudent);
	}

	//test deletion
	for (uint32_t i = 0; i < 20; ++i)
	{
		auto student = students.back();
		studentPool.Delete(student);
		students.pop_back();
	}

	//add new students
	for (uint32_t i = 0; i < 150; ++i)
	{
		Student* newStudent = studentPool.New("Student_" + std::to_string(i));
		students.push_back(newStudent);
	}

	for (auto& student : students)
	{
		studentPool.Delete(student);
	}

	students.clear();

	return 0;
}