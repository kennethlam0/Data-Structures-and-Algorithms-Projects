// Project UID c1f28c309e55405daf00c565d57ff9ad
#include <iostream>
#include <list>
#include "json.hpp"
using nlohmann::json;
using namespace std;


class OHQueue
{
  public:
    int contentLength;
    string method;
    string endpoint;
    int count=0;
    OHQueue(){}
    void read_Routes()
    {
      json routes = {
        {"queue_head_url","http://localhost/queue/head/"},
        {"queue_list_url","http://localhost/queue/"},
        {"queue_tail_url","http://localhost/queue/tail/"}
        };
        string all_routes= routes.dump(4) + "\n";
        cout << "HTTP/1.1 200 OK" << endl;
        cout << "Content-Type: application/json; charset=utf-8" << endl;
        cout << "Content-Length: " << all_routes.length() << endl;
        cout << endl; 
        cout << all_routes;
    }
    void read_Queue()
    {
    json student_list;
    for (auto& student : queue)
    {
      json student_data = {
        {"location", student.location},
        {"position", student.position},
        {"uniqname", student.uniqname}
      };
      student_list.push_back(student_data);
    }
   string result = student_list.dump(4) + "\n";
    json output = {
        {"count", count},
        {"results", student_list}
    };
    string output_string = output.dump(4) + "\n";

    cout << "HTTP/1.1 200 OK" << endl;
    cout << "Content-Type: application/json; charset=utf-8" << endl;
    cout << "Content-Length: " << output_string.length() << endl; 
    cout << endl;
    cout << output_string;
    }
    void read_Head()
    {
      Student front_student = queue.front();
      json front_Student = {
        {"location", front_student.location},
        {"position", front_student.position},
        {"uniqname", front_student.uniqname}
      };
      string head_student = front_Student.dump(4) + "\n";
      cout << "HTTP/1.1 200 OK" << endl;
      cout << "Content-Type: application/json; charset=utf-8" << endl;
      cout << "Content-Length: " << head_student.length() << endl; 
      cout << endl;
      cout << head_student;
    }
    void create_Tail()
    {
      json student_data;
      cin >> student_data;
      Student student = {student_data["location"], 1+count,student_data["uniqname"]};
      queue.push_back(student);
      count+=1; 
      json print = {
        {"location", student.location},
        {"position", student.position},
        {"uniqname", student.uniqname}
      };
      string tail_student = print.dump(4) + "\n";
      cout << "HTTP/1.1 201 Created" << endl;
      cout << "Content-Type: application/json; charset=utf-8" << endl;
      cout << "Content-Length: " << tail_student.length() << endl;
      cout << endl;
      cout << tail_student; 
    }
    void delete_Head()
    {
        queue.pop_front();
        count-=1;
        for(auto& student:queue)
        {
          student.position-=1;
        }
        cout << "HTTP/1.1 204 No Content" << endl;
        cout << "Content-Type: application/json; charset=utf-8" << endl;
        cout << "Content-Length: 0" << endl;
        cout << endl;
    }

  private:
    struct Student
    {
      std::string location;
       int position;
      std::string uniqname;
    };
    std::list<Student> queue;
};


int main() {
  OHQueue oh;
  string not_important_info;

  while(cin >> oh.method >> oh.endpoint)
  {
    for(int i = 0; i < 7; ++i)
    {
      cin >> not_important_info;
    }
    cin >> oh.contentLength;
    if(oh.method == "GET" && oh.endpoint == "/api/")
    {
      oh.read_Routes();
    }
    else if(oh.method == "GET" && oh.endpoint == "/api/queue/")
    {
      oh.read_Queue();
    }
    else if(oh.method == "GET" && oh.endpoint == "/api/queue/head/")
    {
      oh.read_Head();
    }
    else if(oh.method == "POST" && oh.endpoint == "/api/queue/tail/")
    {
      oh.create_Tail();
    }
    else if(oh.method == "DELETE" && oh.endpoint == "/api/queue/head/")
    {
      oh.delete_Head();
    }
    else
    {
      cerr << "Invalid HTTP method: " << oh.method << endl;
      cout << "HTTP/1.1 400 Bad Request" << endl;
      cout << "Content-Type: application/json; charset=utf-8" << endl;
      cout << "Content-Length: 0" << endl;
      cout << endl;
      return 0;
    }
  }
  return 0;
}