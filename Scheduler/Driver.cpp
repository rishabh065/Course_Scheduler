#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <list>
#include <map>
#include <time.h>
#include <ctime>

//OWN

// Total pool in memory
#define  SIZE_IN_MEM 20
// Number of working hours per day
#define DAY_HOURS	9
// Number of days in week
#define DAYS_NUM	6
// Maximum number of iterations
#define ITERATIONS 10000
using namespace std;
float total_time=0;
int curr_id=1;
class Teacher;
class ClassRoom;
class Subject;
class Class;
class StudentsGroup;
class Configuration;
class Solution;
class Teacher
{
private:
	int id;
	string name;
	list<Class*> courses;
public:
	Teacher(int id,string name)
	{
		this->id=id;
		this->name=name;
	}
	void AddSubjectClass(Class* courseClass)
	{
		courses.push_back( courseClass );
	}
	int GetId() { return id; }
	string GetName() { return name; }
	list<Class*>& GetSubjects()  { return courses; }
	bool compare(Teacher* p)
	{
		return id==p->id;
	}
};
class ClassRoom
{
	//This class models the classroom and its related variables into
	//one user defined data type
private:
	// ID counter used to assign IDs automatically
	static int room_counter;
private:
	// ClassRoom ID - automatically assigned
	int id;
	// ClassRoom name
	string name;
	//Whether the class
	bool lab;
	// Capacity of the room
	int capacity;
public:
	// Constructor for the classroom
	ClassRoom(string name, bool lab, int capacity)
	{
		this->id=curr_id++;
		this->name=name;
		this->lab=lab;
		this->capacity=capacity;
	}
	// Returns classroom ID
	int GetId() { return id; }
	// Returns classroom name
	string GetName() { return name; }
	// Returns true if the room is a lab
	bool IsLab() { return lab; }
	// Returns capacity of the room
	int GetStrength() { return capacity; }
	// Restarts automatic room no. assignments
	void RestartIDs() { room_counter = 0; }
};
class Subject
{
	//This class models the Subject and its related variables into
	//one user defined data type
private:

	// Subject ID
	int id;

	// Subject name
	string name;

public:

	// Constructor for the course
	Subject(int id, string name)
	{
		this->id=id;
		this->name=name;
	}

	// Returns course ID
	int GetId() const { return id; }

	// Returns course name
	string GetName() const { return name; }

};
class Class
{
//This class corresponds to a single lecture or lab of any course
private:

	// Teacher who teaches
	Teacher* professor;

	// Subject to which class belongs
	Subject* course;

	// Student groups who attend class
	list<StudentsGroup*> groups;

	// Strength of the room
	int numberOfSeats;

	// True if class has to be held in a lab
	bool requiresLab;

	// Duration of class in hours
	int duration;

public:

	// Constructor for class initiaisation
	Class(Teacher* professor, Subject* course, list<StudentsGroup*> groups,
		bool requiresLab, int duration)
	{
		this->professor=professor;
		this->course=course;
		this->groups=groups;
		this->requiresLab=requiresLab;
		this->duration=duration;
	}


	// Returns TRUE if the students who have this class have another class at the smae time
	bool StudentClash(Class& c )
	{
		return groups==c.GetGroups();
	}

	// Returns TRUE if another class has same professor.
	bool TeacherClash(Class& c )
	{ return professor == &(c.GetTeacher()); }

	// Return pointer to teacher
	Teacher& GetTeacher(){ return *professor; }

	// Return pointer to subject
	Subject& GetSubject(){ return *course; }

	// Returns list of student groups
	list<StudentsGroup*>& GetGroups(){ return groups; }

	// Returns strength of the class
	int GetStrength(){ return numberOfSeats; }

	// Returns TRUE if class is a lab
	bool LabClass()  { return requiresLab; }

	// Returns duration of class in hours
	int GetDuration() { return duration; }

};
class StudentsGroup
{
// This represents the student groups as a user defines data type
private:

	// Student group ID
	int id;

	// Name of student group
	string name;

	// Number of students in group
	int numberOfStudents;

	// List of classes that group attends
	list<Class*> courses;

public:

	// Initializes student group data
	StudentsGroup(int id, string name, int numberOfStudents)
	{
		this->id=id;
		this->name=name;
		this->numberOfStudents=numberOfStudents;
	}

	// Adds classes that the group has to attend
	void AddClass(Class* courseClass)
	{
		courses.push_back(courseClass);
	}

	// Returns student group ID
	int GetId() { return id; }

	// Returns name of student group
	string GetName() { return name; }

	// Returns number of students in group
	int GetNumberOfStudents()  { return numberOfStudents; }

	// Returns reference to list of classes that group attends
	list<Class*>& GetSubjectClasses() { return courses; }

	// Compares ID's of two objects which represent student groups
	bool compare(StudentsGroup* g)
	{
		return id==g->id;
	}

};

//SUPPORT

class Configuration
{

private:

	// Global instance
	static Configuration _instance;

public:

	// Returns reference to global instance
	Configuration& GetInstance() { return _instance; }

private:

	// Parsed professors
	map<int, Teacher*> _professors;

	// Parsed student groups
	map<int, StudentsGroup*> _studentGroups;

	// Parsed courses
	map<int, Subject*> _courses;

	// Parsed rooms
	map<int, ClassRoom*> _rooms;

	// Parsed classes
	list<Class*> _courseClasses;

	// Inidicate that configuration is not prsed yet
	bool _isEmpty;

public:

	// Initialize data
	Configuration() : _isEmpty(true) { }

	// Frees used resources
	~Configuration()
{
	for( map<int, Teacher*>::iterator it = _professors.begin(); it != _professors.end(); it++ )
		delete ( *it ).second;

	for( map<int, StudentsGroup*>::iterator it = _studentGroups.begin(); it != _studentGroups.end(); it++ )
		delete ( *it ).second;

	for( map<int, Subject*>::iterator it = _courses.begin(); it != _courses.end(); it++ )
		delete ( *it ).second;

	for( map<int, ClassRoom*>::iterator it = _rooms.begin(); it != _rooms.end(); it++ )
		delete ( *it ).second;

	for( list<Class*>::iterator it = _courseClasses.begin(); it != _courseClasses.end(); it++ )
		delete *it;
}

	// Parse file and store parsed object
	void ParseFile(char* fileName)
{
	// clear previously parsed objects
	_professors.clear();
	_studentGroups.clear();
	_courses.clear();
	_rooms.clear();
	_courseClasses.clear();



	// open file
	ifstream input;
	input.open(fileName);

	string line;
	while( input.is_open() && !input.eof() )
	{
		// get lines until start of new object is not found
		getline( input, line );
		TrimString( line );

		// get type of object, parse obect and store it

		if( line.compare("#prof") == 0 )
		{
			Teacher* p = ParseTeacher( input );
			if( p )
				_professors.insert( pair<int, Teacher*>( p->GetId(), p ) );
		}
		else if( line.compare("#group") == 0 )
		{
			StudentsGroup* g = ParseStudentsGroup( input );
			if( g )
				_studentGroups.insert( pair<int, StudentsGroup*>( g->GetId(), g ) );
		}
		else if( line.compare("#course") == 0 )
		{
			Subject* c = ParseSubject( input );
			if( c )
				_courses.insert( pair<int, Subject*>( c->GetId(), c ) );
		}
		else if( line.compare("#room") == 0 )
		{

			ClassRoom* r = ParseClassRoom( input );

			if( r )
				_rooms.insert( pair<int, ClassRoom*>( r->GetId(), r ) );

		}
		else if( line.compare("#class") == 0 )
		{
			Class* c = ParseSubjectClass( input );
			if( c )
				_courseClasses.push_back( c );
		}
	}

	input.close();

	_isEmpty = false;
}
	// Returns pointer to professor with specified ID
	// If there is no professor with such ID method returns NULL
	Teacher* GetTeacherById(int id)
	{
		map<int, Teacher*>::iterator it = _professors.find( id );
		return it != _professors.end() ? ( *it ).second : NULL;
	}

	// Returns number of parsed professors
	int GetNumberOfTeachers() { return (int)_professors.size(); }

	// Returns pointer to student group with specified ID
	// If there is no student group with such ID method returns NULL
	StudentsGroup* GetStudentsGroupById(int id)
	{
		map<int, StudentsGroup*>::iterator it = _studentGroups.find( id );
		return it != _studentGroups.end() ? ( *it ).second : NULL;
	}

	// Returns number of parsed student groups
	int GetNumberOfStudentGroups(){ return (int)_studentGroups.size(); }

	// Returns pointer to course with specified ID
	// If there is no course with such ID method returns NULL
	Subject* GetSubjectById(int id)
	{
		map<int, Subject*>::iterator it = _courses.find( id );
		return it != _courses.end() ? ( *it ).second : NULL;
	}

	int GetNumberOfSubjects() { return (int)_courses.size(); }

	// Returns pointer to room with specified ID
	// If there is no room with such ID method returns NULL
	ClassRoom* GetClassRoomById(int id)
	{
		map<int, ClassRoom*>::iterator it = _rooms.find( id );
		return it != _rooms.end() ? ( *it ).second : NULL;
	}

	// Returns number of parsed rooms
	int GetNumberOfClassRooms() { return (int)_rooms.size(); }

	// Returns reference to list of parsed classes
	list<Class*> GetSubjectClasses() { return _courseClasses; }

	// Returns number of parsed classes
	int GetNumberOfSubjectClasses(){ return (int)_courseClasses.size(); }

	// Returns TRUE if configuration is not parsed yet
	bool IsEmpty() { return _isEmpty; }

private:

	// Reads professor's data from config file, makes object and returns pointer to it
	// Returns NULL if method cannot parse configuration data
	Teacher* ParseTeacher(ifstream& file)
{
	int id = 0;
	string name;

	while( !file.eof() )
	{
		string key, value;

		// get key - value pair
		if( !GetConfigBlockLine( file, key, value ) )
			break;

		// get value of key
		if( key.compare("id") == 0 )
			id = atoi( value.c_str() );
		else if( key.compare("name") == 0 )
			name = value;
	}

	// make object and return pointer to it
	return id == 0 ? NULL : new Teacher( id, name );
}


	// Reads professor's data from config file, makes object and returns pointer to it
	// Returns NULL if method cannot parse configuration data
	StudentsGroup* ParseStudentsGroup(ifstream& file)
{
	int id = 0, number = 0;
	string name;

	while( !file.eof() )
	{
		string key, value;

		// get key - value pair
		if( !GetConfigBlockLine( file, key, value ) )
			break;

		// get value of key
		if( key.compare("id") == 0 )
			id = atoi( value.c_str() );
		else if( key.compare("name") == 0 )
			name = value;
		else if( key.compare("size") == 0 )
			number = atoi( value.c_str() );
	}

	// make object and return pointer to it
	return id == 0 ? NULL : new StudentsGroup( id, name, number );
}

	// Reads course's data from config file, makes object and returns pointer to it
	// Returns NULL if method cannot parse configuration data
	Subject* ParseSubject(ifstream& file)
{
	int id = 0;
	string name;

	while( !file.eof() )
	{
		string key, value;

		// get key - value pair
		if( !GetConfigBlockLine( file, key, value ) )
			break;

		// get value of key
		if( key.compare("id") == 0 )
			id = atoi( value.c_str() );
		else if( key.compare("name") == 0 )
			name = value;
	}

	// make object and return pointer to it
	return id == 0 ? NULL : new Subject( id, name );
}

	// Reads rooms's data from config file, makes object and returns pointer to it
	// Returns NULL if method cannot parse configuration data
	ClassRoom* ParseClassRoom(ifstream& file)
{
	int number = 0;
	bool lab = false;
	string name;

	while( !file.eof() )
	{
		string key, value;

		// get key - value pair
		if( !GetConfigBlockLine( file, key, value ) )
			break;

		// get value of key
		if( key.compare("name") == 0 )
			name = value;
		else if( key.compare("lab") == 0 )
			lab = value.compare( "true" ) == 0;
		else if( key.compare("size") == 0 ){
			number = atoi( value.c_str() );
			//cout<<" "<<number<<" "<<endl;
		}
	}

	// make object and return pointer to it
	return number == 0 ? NULL : new ClassRoom( name, lab, number );
}

	// Reads class' data from config file, makes object and returns pointer to it
	// Returns NULL if method cannot parse configuration data
	Class* ParseSubjectClass(ifstream& file)
{
	int pid = 0, cid = 0, dur = 1;
	bool lab = false;

	list<StudentsGroup*> groups;

	while( !file.eof() )
	{
		string key, value;

		// get key - value pair
		if( !GetConfigBlockLine( file, key, value ) )
			break;

		// get value of key
		if( key.compare("professor") == 0 )
			pid = atoi( value.c_str() );
		else if( key.compare("course") == 0 )
			cid = atoi( value.c_str() );
		else if( key.compare("lab") == 0 )
			lab = value.compare( "true" ) == 0;
		else if( key.compare("duration") == 0 )
			dur = atoi( value.c_str() );
		else if( key.compare("group") == 0 )
		{
			StudentsGroup* g = GetStudentsGroupById( atoi( value.c_str() ) );
			if( g )
				groups.push_back( g );
		}
	}

	// get professor who teaches class and course to which this class belongs
	Teacher* p = GetTeacherById( pid );
	Subject* c = GetSubjectById( cid );

	// does professor and class exists
	if( !c || !p )
		return NULL;

	// make object and return pointer to it
	Class* cc = new Class( p, c, groups, lab, dur );
	return cc;
}

	// Reads one line (key - value pair) from configuration file
	bool GetConfigBlockLine(ifstream& file, string& key, string& value)
{
	string line;

	// end of file
	while( !file.eof() )
	{
		// read line from config file
		getline( file, line );
		TrimString( line );

		// end of object's data
		if( line.compare( "#end" ) == 0 )
			return false;

		size_t p = line.find( '=' );
		if( p != line.length())
		{
			// key
			key = line.substr( 0, p );
			TrimString( key );

			// value
			value = line.substr( p + 1, line.length() );
			TrimString( value );

			// key - value pair read successfully
			return true;
		}
	}

	// error
	return false;
}


	// Removes blank characters from beginning and end of string
	string& TrimString(string& str)
{
	string::iterator it;
	for( it = str.begin(); it != str.end() && isspace( *it ); it++ )
		;
	str.erase( str.begin(), it );

	string::reverse_iterator rit;
	for( rit = str.rbegin(); rit != str.rend() && isspace( *rit ) ; rit++ )
		;
	str.erase( str.begin() + ( str.rend() - rit ), str.end() );

	return str;
}

};

//OWN

class Solution
{
	// This class defines the chromosome upon which the GA works
private:
	// Fitness value of chromosome
	float fitness;
	// Flags of class requirements satisfaction
	vector<bool> criteria;
	// Slots of the time table coupled as one vector
	vector<list<Class*> > slots;
	// Class table for chromosome
	// Map store key value pairs where class pointer is the key and slot index is the value
	map<Class*, int> classes;
public:
	// Initializes pool
	Solution(int rooms,int no_of_classes)
	{
		slots.resize( DAYS_NUM * DAY_HOURS * rooms );
		criteria.resize( no_of_classes * 5 );
	}
	// Returns fitness value of chromosome
	float& GetFitness(){ return fitness; }
	// Returns refrence to map
	map<Class*, int>& GetClasses() { return classes; }
	// Returns the flags that define fitness
	vector<bool>& GetCriteria(){ return criteria; }
	// Returns the slot that correspond to a particular chromosome
	vector<list<Class*> >& GetSlots(){ return slots; }
};
Configuration config;
list<Class*> all_classes;
vector<Solution*> pool;
vector<bool> bestFlags;
bool initialised=false;
vector<int> best_pool;
int current_bestsize=1;
int no_of_rooms;
int numberOfCrossoverPoints;
int no_of_classes; // Total number of classes in dataset
// Number of classes that is moved randomly by single mutation operation
int mutationSize;
// Probability that mutation will occure in an interation
int mutationProbability;
// The number of chromosomes that will undergo evolution in an iteration
int replace_iteration;
// Prototype of the CalculateFitness function
void CalculateFitness(Solution*);
ofstream myfile,myfile2,myfile3;
Solution* init()
{
	// Initialises the pool of chromosomes
	Solution* chromosome=new Solution(no_of_rooms,no_of_classes);
	for(list<Class*>::const_iterator it = all_classes.begin(); it != all_classes.end(); it++)
	{
		int day=rand()%DAYS_NUM;
		int room_no=rand()%no_of_rooms;
		int duration=(*it)->GetDuration();
		int time=rand() % ( DAY_HOURS + 1 - duration );
		int position=(day)*DAY_HOURS*no_of_rooms+room_no*DAY_HOURS+time;
		for(int i=duration-1;i>=0;i--)
		{
			chromosome->GetSlots()[position+i].push_back(*it);
		}
		chromosome->GetClasses().insert(pair<Class*, int>(*it,position));
	}
	CalculateFitness(chromosome);
	return chromosome;
}
Solution* Crossover(Solution* parent1,Solution* parent2)
{
	// Takes two parents and performs crossover and returns the offspring
	vector<bool> c_point(all_classes.size());
	for( int i = 5; i > 0; i-- )
	{
		// Generates random number of crossover points
		while( 1 )
		{
			int p = rand() % all_classes.size();
			if( !c_point[ p ] )
			{
				c_point[ p ] = true;
				break;
			}
		}
	}
	// Initializing the offspring
	Solution* offspring=new Solution(no_of_rooms,no_of_classes);
	// iterators for the parent lists
	map<Class*, int>::const_iterator p1list_iter = (parent1->GetClasses()).begin();
	map<Class*, int>::const_iterator p2list_iter = (parent2->GetClasses()).begin();
	int first=rand()%2;
	for(int i=0;i<all_classes.size();i++)
	{
		// when a crossover point is encountered the prent list being copied changes
		if(c_point[i])
			first=1-first;
		if(first)
		{
			(offspring->GetClasses()).insert(pair<Class*, int>((*p1list_iter).first,(*p1list_iter).second));
			for(int i=0;i<(*p1list_iter).first->GetDuration();i++)
			{
				offspring->GetSlots().at((*p1list_iter).second+i).push_back((*p1list_iter).first);
			}
		}
		else
		{
			offspring->GetClasses().insert(pair<Class*, int>((*p2list_iter).first,(*p2list_iter).second));
			for(int i=0;i<(*p2list_iter).first->GetDuration();i++)
			{
				offspring->GetSlots().at((*p2list_iter).second+i).push_back((*p2list_iter).first);
			}
		}
		// iterating the parent lists
		p1list_iter++;
		p2list_iter++;
	}
	// filling the fitness value in the offspring
	CalculateFitness(offspring);
	return offspring;
}
void CalculateFitness(Solution* ch)
{
	int criteria_number=0;
	int daySize=DAY_HOURS*no_of_rooms;
	vector<bool>  parameter=ch->GetCriteria();
	int score=0;
	for(map<Class*, int>::const_iterator it = ch->GetClasses().begin(); it != ch->GetClasses().end(); ++it, criteria_number += 5 )
	{
		int position=(*it).second;
		int day = position / daySize;
		int time = position % daySize;
		int room = time / DAY_HOURS;
		time = time % DAY_HOURS;
		int duration = ( *it ).first->GetDuration();
		bool room_overlap=false;
		for(int i=0;i<duration;i++)
		{
			if(ch->GetSlots()[position+i].size()>1)
				room_overlap=true;
		}
		if(!room_overlap)
			score++;
		parameter[criteria_number+0]=!room_overlap;
		Class* curr=(*it).first;
		ClassRoom* r=config.GetClassRoomById(room+1);
		parameter[criteria_number+1]=r->GetStrength()>=curr->GetStrength();
		if(parameter[criteria_number+1])
			score++;
		parameter[criteria_number+2]=!curr->LabClass() || ( curr->LabClass() && r->IsLab());
		if(parameter[criteria_number+2])
			score++;
		bool teacher_clash=false,student_clash=false;
		int break_check=0;
		for(int i=0,p=day*daySize+time;i<no_of_rooms;i++,p+=DAY_HOURS)
		{
			for(int i=0;i<duration;i++)
			{
				list<Class*>& cl = ch->GetSlots()[ p + i ];
				for( list<Class*>::const_iterator it = cl.begin(); it != cl.end(); it++ )
				{
					if( curr != *it )
					{
						// teacher clash
						if( !teacher_clash && curr->TeacherClash( **it ) )
							teacher_clash = true;
						// student clash
						if( !student_clash && curr->StudentClash( **it ) )
							student_clash = true;
						// both clash indicates break
						if( teacher_clash && student_clash )
							break_check=1;
					}
				}
				if(break_check)
					break;
			}
			if(break_check)
					break;
		}

		if(!teacher_clash)
			score++;
		parameter[criteria_number+3]=!teacher_clash;
		if(!student_clash)
			score++;
		parameter[criteria_number+4]=!student_clash;
	}
	ch->GetFitness()=(float)score/(config.GetNumberOfSubjectClasses()*5);
}
Solution* Mutation(Solution* ini)
{
	// check probability of mutation
	if( rand() % 100 > mutationProbability )
		//if less return chromosome as it is
		return ini;
	int size = DAYS_NUM*DAY_HOURS*no_of_rooms;
	// shuffle classes randomly
	for( int i = mutationSize; i > 0; i-- )
	{
		// select ranom chromosome for movement
		int mutation_position = rand() % no_of_classes;
		int pos1 = 0;
		map<Class*, int>::iterator it = ini->GetClasses().begin();
		for( ; mutation_position > 0; it++, mutation_position-- );
		// current slot occupied by class
		pos1 = ( *it ).second;
		Class* mutation_class = ( *it ).first;
		// new position of class determined randomly
		int rooms = config.GetNumberOfClassRooms();
		int duration_class = mutation_class->GetDuration();
		int day = rand() % DAYS_NUM;
		int room = rand() % rooms;
		int time = rand() % ( DAY_HOURS + 1 - duration_class );
		int pos2 = day * rooms * DAY_HOURS + room * DAY_HOURS + time;
		// move all slots of a particular class
		for( int i = duration_class - 1; i >= 0; i-- )
		{
			// remove this hour of the class from its old position
			list<Class*>& slot_iterator = ini->GetSlots()[ pos1 + i ];
			for( list<Class*>::iterator it =slot_iterator.begin(); it != slot_iterator.end(); it++ )
			{
				if( *it == mutation_class )
				{
					slot_iterator.erase( it );
					break;
				}
			}
			// move this hour of class to the new position
			ini->GetSlots().at( pos2 + i ).push_back( mutation_class );
		}
		// Alter the map of the chromosome
		ini->GetClasses()[ mutation_class ] = pos2;
	}
	// fill the fitness value of offspring
	CalculateFitness(ini);
	return ini;
}
bool CheckBest(int chromosomeIndex)
{
	// returns if the chromosome at supplied index is in the best ones in the pool
	return bestFlags[ chromosomeIndex ];
}
int AlterSet(int index)
{
	int i=current_bestsize;
	// Fitness value low or already in the best
	if(bestFlags[index] || pool[index]->GetFitness()<=(pool[best_pool[current_bestsize-1]]->GetFitness()))
		return 0;
	while(true)
	{
		if(i<best_pool.size())
		{
			if(i==0){
				//cout<<pool[index]->GetFitness()<<endl;
				break;
			}
			if(pool[index]->GetFitness()<pool[best_pool[i-1]]->GetFitness())
				break;
			best_pool[i]=best_pool[i-1];
			// making space to accomodate new entry
		}
		else bestFlags[best_pool[best_pool.size()-1]]=false;
		// deleting the entry that falls out
		i--;
	}
	// if the best array is not filled expand the current best size
	if(current_bestsize<best_pool.size())
		current_bestsize++;
	best_pool[i]=index;
	bestFlags[index]=true;
	return 0;
}
float Execute(int i)
{
	// Performs single iterationclock_t start, end;
	clock_t start, end1;

float elapsed_time;
start = clock();
	vector<Solution*> child;
	// replace a randomly chosen number of chromosomes in an iteration
		child.resize( replace_iteration );
		for( int j = 0; j < replace_iteration; j++ )
		{
			// randomly select parents from the pool
			Solution* before1 = pool[ rand() % pool.size() ];
			Solution* before2 = pool[ rand() % pool.size() ];

			child[j] = Crossover( before1,before2 );
			child[j]=Mutation(child[ j ]);
		}
		// replace pool of current operation with offspring
		for( int j = 0; j < replace_iteration; j++ )
		{
			int counter=rand() % pool.size();
			while( CheckBest( counter ) )
			{
				counter = rand() % (int)pool.size();
			}
			// replace pool
			delete pool[ counter ];
			pool[ counter ] = child[ j ];
			// Check if the offspring fits in best pool
			AlterSet(counter);
		}
		end1 = clock();
       elapsed_time = (float)(end1 - start) / (float)CLOCKS_PER_SEC;
       total_time=elapsed_time+total_time;
	   myfile<<" "<<pool[best_pool[0]]->GetFitness()<<endl;
		myfile3<<"Iteration: "<<i+1<<" "<<"Elapsed time"<<":"<<" "<<elapsed_time<<endl;
		return total_time;
}
int main()
{
	srand(time(NULL));
	cout<<"Hello! Welcome to TimeTable Scheduler"<<endl;
	cout<<"Check results.txt file to find the fitness value in each iteration.\n";
	cout<< "Check timetable.txt to get the final timetable.\n";
	myfile.open ("results.txt");
	myfile2.open ("timetable.txt");
	myfile3.open ("time.txt");
	config.ParseFile("data.cfg");
	string s;
	//Initialising Variables
	all_classes=config.GetSubjectClasses();
	no_of_rooms=config.GetNumberOfClassRooms();
	no_of_classes=config.GetNumberOfSubjectClasses();
	//cout<<all_classes.size()<<endl;
	//cout<<config.GetNumberOfTeachers()<<endl;
	//cout<<config.GetNumberOfSubjects()<<endl;
	//cout<<no_of_rooms<<endl;
	mutationSize=rand()%no_of_classes;
	mutationProbability=rand()%100;
	replace_iteration=rand()%SIZE_IN_MEM;
	bestFlags.resize(SIZE_IN_MEM);
	pool.resize(SIZE_IN_MEM);
	best_pool.resize(SIZE_IN_MEM/3);
	// Random initialisation
	for(int i=0;i<pool.size();i++)
	{
		pool[i]=init();
		AlterSet(i);
	}
	for(int i=0;i<ITERATIONS;i++)
	{
		myfile<<"Iteration "<<i+1<<": ";
		total_time=Execute(i);
		if(pool[best_pool[0]]->GetFitness()==1)
			break;
	}
	myfile3<<"Total time"<<":"<<" "<<total_time;
	//cout<<pool[best_pool[0]]->GetFitness();
	int result=best_pool[0];
	// Printing the generated time table
	Solution* time_table=pool[result];
	myfile2<<"Schedule"<<endl<<endl;
	int daySize=DAY_HOURS*no_of_rooms;
	for(list<Class*>::const_iterator it = all_classes.begin(); it != all_classes.end(); it++)
	{
		myfile2<<((*it)->GetSubject()).GetName()<<endl;
		myfile2<<"Subject ID: "<<((*it)->GetSubject()).GetId()<<endl;
		myfile2<<"Teacher: "<<((*it)->GetTeacher()).GetName()<<endl;
		int position=time_table->GetClasses()[(*it)];
		int day = position / daySize;
		switch(day){
			case 1:
			s="Monday";
			break;
			case 2:
			s="Tuesday";
			break;
			case 3:
			s="Wednesday";
			break;
			case 4:
			s="Thursday";break;
			case 5:
			s="Friday";
			break;
			case 6:
			s="Saturday";
			break;
			default:
			break;
		}
		int t = position % daySize;
		int room = t / DAY_HOURS;
		t = t % DAY_HOURS;
		ClassRoom* r=config.GetClassRoomById(room+1);
		myfile2<<"ClassRoom: "<<r->GetName()<<endl;
		myfile2<<"Day: "<<s<<" "<<"Slot: "<<t+1<<endl;
		for(list<StudentsGroup*>::const_iterator i = (*it)->GetGroups().begin(); i != (*it)->GetGroups().end(); i++)
			myfile2<<"Groups: "<<(*i)->GetId()<<" ";
		myfile2<<endl<<endl;
	}
	return 0;
}

