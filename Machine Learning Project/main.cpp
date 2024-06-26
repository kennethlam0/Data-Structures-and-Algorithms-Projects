// Project UID db1f506d06d84ab787baf250c265e24e

#include <iostream>
#include <fstream>
#include <cstdlib>
#include "csvstream.h"
#include <map>
#include <set>
#include <string>
#include <cmath>
using namespace std;

class Classifier
{
  public: 
  Classifier(string train, string test):training_File(train),test_File(test){}
  void initialize_private()
  {
    csvstream csv_in(training_File);
    map<string,string> row;
    while(csv_in >> row)
    {
      string tag = row.at("tag");
      string content = row.at("content");
      num_posts++;
      num_postswithlabel[tag]++;
      set<string> post_vocab = unique_words(content);
      for(string word:post_vocab)
      {
        num_postswithword[word]++;
        num_postswithlabelword[tag][word]++;
      }
    }
    vocab_size = num_postswithword.size();

    //now initialize the test file
    csvstream csv_test_in(test_File);
    map<string,string> test_row;
    while(csv_test_in >> test_row)
    {
      string test_correct_tag = test_row.at("tag");
      string test_content = test_row.at("content");
      test_data_tag_content.push_back({test_correct_tag, test_content});
    }
  }
  set<string> unique_words(const string &str) 
  {
    istringstream source(str);
    set<string> words;
    string word;
    while (source >> word) 
    {
      words.insert(word);
    }
    return words;
  }
  void print_numposts()
  {
    cout << "trained on " << num_posts << " examples" << endl;
  }


  void print_classes()
  {
    cout << "classes:" << endl;
    for(auto element:num_postswithlabel)
    {
      cout << "  " << element.first << ", " << element.second 
      << " examples, log-prior = " << log_prior(element.first) << endl;
    }
  }

  void print_classifier_parameters()
  {
    cout << "classifier parameters:" << endl;
    for(const auto& label_word_count:num_postswithlabelword)
    {
      for(const auto& word_count : label_word_count.second)
      {
        cout << "  " << label_word_count.first << ":";
        cout << word_count.first << ", count = " << word_count.second 
        << ", log-likelihood = " << 
        log_likelihood(word_count.first, label_word_count.first) << endl;
      }
    }
    cout << endl;
  }

  void print_training_data()
  {
    csvstream csv_in(training_File);
    map<string,string> row;
    cout << "training data:" << endl;
    while(csv_in >> row)
    {
      string tag = row.at("tag");
      string content = row.at("content");
      cout << "  label = " << tag << ", content = " << content << endl;
    }
  }

  //debug fcn for vocab size
  void print_vocab_size()
  {
    cout << "vocabulary size = " << vocab_size << endl << endl;
  }

  //log prior fcn
  double log_prior(string label)
  {
    return log((double)num_postswithlabel.find(label)->second / num_posts);
  }

  //log likelihood
  double log_likelihood(string word,string label)
  {
    if(num_postswithword.find(word)==num_postswithword.end())
    {
      return log((double)1/num_posts);
    }
    else if(num_postswithword.find(word)!=num_postswithword.end() && 
    num_postswithlabelword.find(label)->second.find(word)==
    num_postswithlabelword.find(label)->second.end())
    {
      return log((double)num_postswithword.find(word)->second/num_posts);
    }
    else
    {
      return log((double)num_postswithlabelword.find(label)->second.find(word)->second
      / num_postswithlabel.find(label)->second);
    }
  }

  //log prob fcn
  //use a vector instead of a set to store words
  //need to add a way to check if word has been seen before and dont add if so
  void calc_log_probability(string content)
  {
    for(auto& element:num_postswithlabel)
    {
      istringstream iss(content);
      string word;
      double log_probability = log_prior(element.first);
      vector<string> word_duplicate_vec;
      while(iss>>word)
      {
        vector<string>::iterator it;
        it = find (word_duplicate_vec.begin(), word_duplicate_vec.end(), word);
        if(it == word_duplicate_vec.end()){
        log_probability += log_likelihood(word,element.first);
        word_duplicate_vec.push_back(word);
        }
      }
      log_probability_label.push_back({element.first,log_probability});
    } 
  }

  //calc max prob fcn
  pair<string, double> calc_max_prob()
  {
    string label_maxprob  = (*log_probability_label.begin()).first;
    double max = (*log_probability_label.begin()).second;
    for(auto& element: log_probability_label)
    {
      if(element.second > max)
      {
        max = element.second;
        label_maxprob = element.first;
      }
    }
    pair<string,double> pair = {label_maxprob,max};
    return pair;
  }

  //fcn print test data
  void print_test_data()
  {
    int correct_posts = 0;
    int total_posts = 0;

    cout << "test data:" << endl;
   
    for(auto& post: test_data_tag_content)
    {
      calc_log_probability(post.second);
      string max_prob = calc_max_prob().first;
      double max_prob_score = calc_max_prob().second;
      cout << "  correct = " <<  post.first << ", predicted = " 
      << max_prob << ", log-probability score = " << max_prob_score << endl;
      cout << "  content = " << post.second << endl << endl;
      if(post.first == max_prob)
      {
          correct_posts++;
      }
      log_probability_label.clear();
      total_posts++;
    }

    cout << "performance: " << correct_posts << " / " << total_posts 
    << " posts predicted correctly" << endl;
  }

  void print(bool debug)
  {
    if(debug)
    {
      initialize_private();
      print_training_data();
      print_numposts();
      print_vocab_size();
      print_classes();
      print_classifier_parameters();
      print_test_data();
    }
    else
    {
      initialize_private();
      print_numposts();
      cout << endl;
      print_test_data();
    }
  }

  private:
  string training_File;
  string test_File;
  int num_posts = 0;
  int vocab_size;
  std::map<string,int> num_postswithword;
  std::map<string,int> num_postswithlabel;
  std::map<string,map<string,int>> num_postswithlabelword;
  vector<pair<string,double>> log_probability_label; 
  vector<pair<string,string>> test_data_tag_content;
  
};

int main(int argc, char *argv[]) {
  bool debug = false;
  cout.precision(3);
  //set precision using spec
  if(argc!=3 && argc!=4)
  {
    cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
    return 1;
  }
  if(argc==4)
  {
    if(strcmp(argv[3],"--debug") < 0 || strcmp(argv[3],"--debug") > 0)
    {
      cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
      return 1;
    }
    else
    {
      debug = true;
    }
  }
  string train_file = argv[1];
  string test_file = argv[2];
  try
  {
     csvstream cv_train(train_file);
  }
  
  catch(const csvstream_exception &e)
  {
    cout << e.what();
    return 1;
  }
  try
  {
    csvstream cv_test(test_file);
  }
  catch(const csvstream_exception &e)
  {
    cout << e.what();
    return 1;
  }
  Classifier classifier(train_file,test_file);
  classifier.print(debug);
}